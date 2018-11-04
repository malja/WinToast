#include <wrl/implements.h>
#include <NotificationActivationCallback.h>

#include <fstream>
#include <map>
#include <string>

#include "activator_helper.h"
#include <windows.ui.notifications.h>

using namespace ABI::Windows::Data::Xml::Dom;
using namespace ABI::Windows::UI::Notifications;
using namespace Microsoft::WRL;

class DECLSPEC_UUID("8aa59b6c-1401-42a5-b1cf-3ccb3b360e77") Activator WrlSealed WrlFinal : public RuntimeClass<RuntimeClassFlags<ClassicCom>, INotificationActivationCallback> {
	public:

	HRESULT STDMETHODCALLTYPE Activate(_In_ LPCWSTR appUserModelId, _In_ LPCWSTR invokedArgs, _In_reads_(dataCount) const NOTIFICATION_USER_INPUT_DATA* data, ULONG dataCount) override {

        MessageBox( 0, L"Hello World!", L"Activator", 0 );
		return S_OK;
	}
};

CoCreatableClass(Activator);