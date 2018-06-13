#include "wintoastlib.h"

#include <iostream>

class EventHandler : public WinToastLib::IWinToastHandler {

    public:

        void toastActivated() const override {
            std::cout << "Toast Clicked" << std::endl;
        }

        void toastActivated(int actionIndex) const override {
            std::cout << "Action Clicked #" << actionIndex << std::endl;
        }

        void toastDismissed(WinToastDismissalReason state) const {
            switch (state) {

                case UserCanceled:
                    std::cout << "Dissmissed" << std::endl;
                break;

                case TimedOut:
                    std::cout << "Timed out" << std::endl;
                break;

                case ApplicationHidden:
                    std::cout << "Hidden from app" << std::endl;
                break;

                default:
                    std::cout << "Toast not activated" << std::endl;
                break;
            }
        }

        void toastFailed() const {
            std::cout << "Failed" << std::endl;
        }
};
