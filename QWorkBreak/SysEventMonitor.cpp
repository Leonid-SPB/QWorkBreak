#include "SysEventMonitor.hpp"
#include "resource.hpp"
#include "Logger.hpp"
#include <QDebug>
#ifdef _WIN32
#include <windows.h>
#endif

namespace {
// window identificator for receiving system event notifications
const char WindowGuid[] = "QWorkBreak::SysEventMonitor::63e3c948-0472-4b7b-98ef-3e7c81cd5d08";

const char SysEventProviderSvcPath[] = "QWBEvtProvdr.exe";

const int ActivityCheckInterval = 10 * 1000; //ms

#ifdef _WIN32
enum SysEventMessages {
    DesktopLock      = WM_USER + 0,
    DesktopUnlock    = WM_USER + 1,
    StartScreenSaver = WM_USER + 2,// not supported currently
    StopScreenSaver  = WM_USER + 3,// not supported currently
    PowerModeSusped  = WM_USER + 4,
    PowerModeResume  = WM_USER + 5
};
#endif
}

SysEventMonitor::SysEventMonitor(QWidget *parent) :
    QWidget(parent), sysEventProviderSvc_(this), activeState_(true)
{
#ifdef _WIN32
    setWindowTitle(WindowGuid);
#endif

    // setup timer
    connect(&myTimer_, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

SysEventMonitor::~SysEventMonitor() {
#ifdef _WIN32
    // @todo make a gaceful shutdown for sysEventProviderSvc_
    sysEventProviderSvc_.terminate();
#endif
}

void SysEventMonitor::init() {
#ifdef _WIN32
    createWinId();
    sysEventProviderSvc_.start(SysEventProviderSvcPath);
    restartTimer();
#endif
}

void SysEventMonitor::restartTimer() {
    activeState_ = true;
    Q_ASSERT(ActivityCheckInterval > 0);
    myTimer_.start(ActivityCheckInterval);
}

bool SysEventMonitor::nativeEvent(const QByteArray & eventType, void * message, long * result) {
#ifdef _WIN32
    static const QByteArray GenericMsgType("windows_generic_MSG");

    if (eventType == GenericMsgType) {
        const MSG * const pMsg = static_cast<const MSG * const>(message);

        if (pMsg->message == SysEventMessages::DesktopLock) {
            Logger::logEvent(Logger::WorkstationLocked);
            myTimer_.stop();
            emit desktopLocked();
        } else if (pMsg->message == SysEventMessages::DesktopUnlock) {
            Logger::logEvent(Logger::WorkstationUnlocked);
            restartTimer();
            emit desktopUnlocked();
        } else if (pMsg->message == SysEventMessages::StartScreenSaver) {
            myTimer_.stop();
            emit screensaverStarted();
        } else if (pMsg->message == SysEventMessages::StopScreenSaver) {
            restartTimer();
            emit screensaverStopped();
        } else if (pMsg->message == SysEventMessages::PowerModeSusped) {
            Logger::logEvent(Logger::PowerModeSuspended);
            myTimer_.stop();
            emit powerModeSuspended();
        } else if (pMsg->message == SysEventMessages::PowerModeResume) {
            Logger::logEvent(Logger::PowerModeResumed);
            restartTimer();
            emit powerModeResumed();
        } else {
            //unknown message, allow further processing
            return false;
        }

        //stop processing
        *result = 0;
        return true;
    }
#else
    Q_UNUSED(result);
    Q_UNUSED(message);
    Q_UNUSED(eventType);
#endif

    //not our message, allow further processing
    return false;
}

void SysEventMonitor::onProcessError(QProcess::ProcessError error) {
    Q_UNUSED(error);
    qDebug() << "SysEventMonitor::something wrong with SysEventProviderSvc";
}

void SysEventMonitor::onTimeout() {
#ifdef _WIN32
    LASTINPUTINFO li;
    li.cbSize = sizeof(LASTINPUTINFO);
    if (!GetLastInputInfo(&li)) {
        qDebug() << "SysEventMonitor::something wrong with GetLastInputInfo()";
    }

    DWORD timeDiffMs = GetTickCount() - li.dwTime;
    DWORD inactThreshMs = settings_.value(SettingInactivityThreshold, SettingInactivityThresholdDefVal).toUInt();
    Q_ASSERT(inactThreshMs > 0);

    if (timeDiffMs < inactThreshMs) {
        if (!activeState_) {
            activeState_ = true;
            Logger::logEvent(Logger::UserStateActive);
            emit userStateActive();
        }
    } else {
        if (activeState_) {
            activeState_ = false;
            Logger::logEvent(Logger::UserStateInactive);
            emit userStateInactive();
        }
    }
#endif
}


#ifdef _WIN32
//code from http://stackoverflow.com/questions/3797802/how-to-check-if-an-other-program-is-running-in-fullscreen-mode-eg-a-media-play
namespace {
bool AreSameRECT(RECT& lhs, RECT& rhs) {
    return   (lhs.bottom == rhs.bottom) && (lhs.left == lhs.left) &&
             (lhs.right == rhs.right)   && (lhs.top == rhs.top);
}


bool IsFullscreenAndMaximized(HWND hWnd) {
    RECT screen_bounds;
    GetWindowRect(GetDesktopWindow(), &screen_bounds);

    RECT app_bounds;
    GetWindowRect(hWnd, &app_bounds);

    if (hWnd != GetDesktopWindow() && hWnd != GetShellWindow()) {
        return AreSameRECT(app_bounds, screen_bounds);
    }

    return false;
}

BOOL CALLBACK CheckFullScreenMode(HWND hwnd, LPARAM lParam) {
    Q_UNUSED(hwnd);

    if (IsFullscreenAndMaximized(GetForegroundWindow())) {
        *reinterpret_cast<bool *>(lParam) = true;
        return FALSE;
    }
    return TRUE;
}

}//namespace
#endif

bool SysEventMonitor::isFullScreenAppRunning() {
    bool bThereIsAFullscreenWin = false;
    EnumWindows(CheckFullScreenMode, reinterpret_cast<LPARAM>(&bThereIsAFullscreenWin));
    return bThereIsAFullscreenWin;
}
