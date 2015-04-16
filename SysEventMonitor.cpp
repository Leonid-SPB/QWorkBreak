#include "SysEventMonitor.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

namespace {
// window identificator for receiving system event notifications
const char SysEventMonitor::WindowGuid[] = "QWorkBreak::SysEventMonitor::63e3c948-0472-4b7b-98ef-3e7c81cd5d08";

#ifdef _WIN32
enum SysEventMessages {
    DesktopLock      = WM_USER + 0,
    DesktopUnlock    = WM_USER + 1,
    StartScreenSaver = WM_USER + 2,
    StopScreenSaver  = WM_USER + 3
};
#endif
}

SysEventMonitor::SysEventMonitor(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(WindowGuid);
}

SysEventMonitor::~SysEventMonitor() {
}

bool SysEventMonitor::nativeEvent(const QByteArray & eventType, void * message, long * result) {
#ifdef _WIN32
    static const QByteArray GenericMsgType("windows_generic_MSG");

    if (eventType == GenericMsgType) {
        const MSG * const pMsg = static_cast<const MSG * const>(message);

        if (pMsg->message == SysEventMessages::DesktopLock) {
            emit desktopLocked();
        } else if (pMsg->message == SysEventMessages::DesktopUnlock) {
            emit desktopUnlocked();
        } else if (pMsg->message == SysEventMessages::StartScreenSaver) {
            emit screensaverStarted();
        } else if (pMsg->message == SysEventMessages::StopScreenSaver) {
            emit screensaverStopped();
        } else {
            //unknown message
            return false;
        }

        *result = 0;
        return true;
    }
#else
    Q_UNUSED(result);
    Q_UNUSED(message);
    Q_UNUSED(eventType);
#endif

    //not our message
    return false;
}
