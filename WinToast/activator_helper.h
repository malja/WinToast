#define UNICODE //< This define is required for GetModuleFileName to work with wchar_t
#include <Windows.h>
#include <wrl.h>
#include <string>

/*!
 * A parameter application is run with when activated from notification
 */
#define TOAST_ACTIVATED_LAUNCH_ARG L"-ToastActivated"

/*!
 * Macro for checking return value of HRESULT type for failure.
 */
#define RETURN_IF_FAILED(hr) do { HRESULT _hrTemp = hr; if (FAILED(_hrTemp)) { return _hrTemp; } } while (false)

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

// Following functions were taken from https://raw.githubusercontent.com/WindowsNotifications/desktop-toasts/master/CPP-WRL/DesktopToastsCppWrlApp/DesktopNotificationManagerCompat.cpp
// mostly without any change.
// In the future, they should be embeded into WinToastLib::Util namespace

HRESULT RegisterComServer(GUID clsid, const wchar_t exePath[]) {

    // Turn the GUID into a string
	OLECHAR* clsidOlechar;
	StringFromCLSID(clsid, &clsidOlechar);
	std::wstring clsidStr(clsidOlechar);
	::CoTaskMemFree(clsidOlechar);

	// Create the subkey
	// Something like SOFTWARE\Classes\CLSID\{23A5B06E-20BB-4E7E-A0AC-6982ED6A6041}\LocalServer32
	std::wstring subKey = LR"(SOFTWARE\Classes\CLSID\)" + clsidStr + LR"(\LocalServer32)";

	// Include -ToastActivated launch args on the exe
	std::wstring exePathStr(exePath);
	exePathStr = L"\"" + exePathStr + L"\" " + TOAST_ACTIVATED_LAUNCH_ARG;

	// We don't need to worry about overflow here as ::GetModuleFileName won't
	// return anything bigger than the max file system path (much fewer than max of DWORD).
	DWORD dataSize = static_cast<DWORD>((exePathStr.length() + 1) * sizeof(WCHAR));

	// Register the EXE for the COM server
	return HRESULT_FROM_WIN32(::RegSetKeyValue(
		HKEY_CURRENT_USER,
		subKey.c_str(),
		nullptr,
		REG_SZ,
		reinterpret_cast<const BYTE*>(exePathStr.c_str()),
		dataSize));
}

HRESULT RegisterAumidAndComServer(const wchar_t *aumid, GUID clsid) {
	
	// Get the EXE path
	wchar_t exePath[MAX_PATH];
	DWORD charWritten = ::GetModuleFileName(nullptr, exePath, ARRAYSIZE(exePath));
	RETURN_IF_FAILED(charWritten > 0 ? S_OK : HRESULT_FROM_WIN32(::GetLastError()));

	// Register the COM server
	RETURN_IF_FAILED(RegisterComServer(clsid, exePath));
	return S_OK;
}

HRESULT RegisterActivator() {
	// Module<OutOfProc> needs a callback registered before it can be used.
	// Since we don't care about when it shuts down, we'll pass an empty lambda here.
	Module<OutOfProc>::Create([] {});

	// If a local server process only hosts the COM object then COM expects
	// the COM server host to shutdown when the references drop to zero.
	// Since the user might still be using the program after activating the notification,
	// we don't want to shutdown immediately.  Incrementing the object count tells COM that
	// we aren't done yet.
	Module<OutOfProc>::GetModule().IncrementObjectCount();

	RETURN_IF_FAILED(Module<OutOfProc>::GetModule().RegisterObjects());

	return S_OK;
}

