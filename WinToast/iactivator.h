#include <wrl/implements.h>
#include <NotificationActivationCallback.h>

#include <fstream>
#include <map>
#include <string>

using namespace Microsoft::WRL;

// Following code is an interface for future Activator class.

/*class IWinToastActivator : public RuntimeClass<RuntimeClassFlags<ClassicCom>, INotificationActivationCallback> {
    public:
        HRESULT STDMETHODCALLTYPE Activate(_In_ LPCWSTR appUserModelId, _In_ LPCWSTR invokedArgs, _In_reads_(dataCount) const NOTIFICATION_USER_INPUT_DATA* data, ULONG dataCount) override {

            std::map<std::wstring, std::wstring> params;

            for (ULONG i = 0; i < dataCount; i++ ) {
                params.insert(std::pair<std::wstring, std::wstring>( (data[i]).Key, (data[i]).Value ));
            }

            // Call user handler
            notificationActivated(std::wstring(appUserModelId), std::wstring(invokedArgs), params);
        }

        virtual void notificationActivated(const std::wstring &appUserModelId, const std::wstring &parameters, std::map<std::wstring, std::wstring> &user_input) const = 0;
};*/

// This class is just for testing.

class DECLSPEC_UUID("8aa59b6c-1401-42a5-b1cf-3ccb3b360e77") Activator WrlSealed WrlFinal : public RuntimeClass<RuntimeClassFlags<ClassicCom>, INotificationActivationCallback> {
	public:

	HRESULT STDMETHODCALLTYPE Activate(_In_ LPCWSTR appUserModelId, _In_ LPCWSTR invokedArgs, _In_reads_(dataCount) const NOTIFICATION_USER_INPUT_DATA* data, ULONG dataCount) override {

        // Change username
		std::wofstream file("C:\\Users\\<uname>\\Desktop");

		file << appUserModelId << std::endl;
		file << invokedArgs << std::endl;

		for (ULONG i = 0; i < dataCount; i++) {
			file << (data[i]).Key << " " << (data[i]).Value << std::endl;
		}

		return S_OK;
	}
};

CoCreatableClass(Activator);