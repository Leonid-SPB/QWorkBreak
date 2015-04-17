#include "SysEventMonitor.hpp"
#include <QDebug>
#ifdef _WIN32
#include <windows.h>
#endif

namespace {
// window identificator for receiving system event notifications
const char WindowGuid[] = "QWorkBreak::SysEventMonitor::63e3c948-0472-4b7b-98ef-3e7c81cd5d08";

const char SysEventProviderSvcPath[] = "SysEventNotificationProvider.exe";

#ifdef _WIN32
enum SysEventMessages {
    DesktopLock      = WM_USER + 0,
    DesktopUnlock    = WM_USER + 1,
    StartScreenSaver = WM_USER + 2,
    StopScreenSaver  = WM_USER + 3,
    PowerModeSusped  = WM_USER + 4,
    PowerModeResume  = WM_USER + 5
};
#endif
}

SysEventMonitor::SysEventMonitor(QWidget *parent) :
    QWidget(parent), sysEventProviderSvc(this)
{
#ifdef _WIN32
    setWindowTitle(WindowGuid);
#endif
}

SysEventMonitor::~SysEventMonitor() {
#ifdef _WIN32
    sysEventProviderSvc.terminate();
#endif
}

void SysEventMonitor::init() {
#ifdef _WIN32
    createWinId();
    sysEventProviderSvc.start(SysEventProviderSvcPath);
#endif
}

bool SysEventMonitor::nativeEvent(const QByteArray & eventType, void * message, long * result) {
#ifdef _WIN32
    static const QByteArray GenericMsgType("windows_generic_MSG");

    if (eventType == GenericMsgType) {
        const MSG * const pMsg = static_cast<const MSG * const>(message);

        if (pMsg->message == SysEventMessages::DesktopLock) {
            qDebug() << "SysEventMonitor::desktopLocked()";
            emit desktopLocked();
        } else if (pMsg->message == SysEventMessages::DesktopUnlock) {
            qDebug() << "SysEventMonitor::desktopUnlocked()";
            emit desktopUnlocked();
        } else if (pMsg->message == SysEventMessages::StartScreenSaver) {
            qDebug() << "SysEventMonitor::screensaverStarted()";
            emit screensaverStarted();
        } else if (pMsg->message == SysEventMessages::StopScreenSaver) {
            qDebug() << "SysEventMonitor::screensaverStopped()";
            emit screensaverStopped();
        } else if (pMsg->message == SysEventMessages::PowerModeSusped) {
            qDebug() << "SysEventMonitor::powerModeSuspended()";
            emit powerModeSuspended();
        } else if (pMsg->message == SysEventMessages::PowerModeResume) {
            qDebug() << "SysEventMonitor::powerModeResumed()";
            emit powerModeResumed();
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

void SysEventMonitor::onProcessError(QProcess::ProcessError error) {
    Q_UNUSED(error);
    qDebug() << "SysEventMonitor::something wrong with SysEventProviderSvc";
}
