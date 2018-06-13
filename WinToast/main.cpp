#define UNICODE

#include "wintoastlib.h"
#include "event_handler.h"
#include <string>
#include <activator_helper.h>

#include "iactivator.h"

using namespace WinToastLib;

int main(int argc, char *argv[]) {

    // Check if application was executed with parameter -ToastActivated
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-ToastActivated") == 0) {
            std::cout << "Activated from notification" << std::endl;
            std::cout << "Param: " << argv[1] << std::endl;
            return 0;
        }
    }

    // Parameters for notification
    std::wstring appName(L"Console WinToast Example");
    std::wstring aumi(L"company.product");
    std::wstring text(L"Text");

    // Register Activator class
    RegisterAumidAndComServer(aumi.c_str(), __uuidof(Activator));
    RegisterActivator();

	if (!WinToast::isCompatible()) {
		std::cerr << "Error, your system in not supported!" << std::endl;
        return -1;
	}

	WinToast::instance()->setAppName(appName);
	WinToast::instance()->setAppUserModelId(aumi);

	if (!WinToast::instance()->initialize()) {
		std::cerr << "Error, your system in not compatible!" << std::endl;
		return -2;
	}

    // Use ::Generic for new TemplateType which should support Activator
    // Or any of other types.
	WinToastTemplate templ(WinToastTemplate::Generic);

	templ.setTextField(text, WinToastTemplate::FirstLine);

    if (WinToast::instance()->showToast(templ, new EventHandler()) < 0) {
	    std::cerr << "Could not launch your toast notification!";
		return -3;
	}

	Sleep(15000);

    return 0;
}
