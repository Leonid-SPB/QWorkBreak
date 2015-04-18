#pragma once
#include <QWidget>
#include <QProcess>

/** @brief SysEventMonitor provides notifications about following events:
 * desktop locked, desktop unlocked, screensaver started, screensaver stopped.
 *
 * On Windows platform SysEventMonitor relies on external SysEventNotificationProvider service, which
 * uses Win32 API messages to inform SysEventMonitor about system events.
*/
class SysEventMonitor : public QWidget {
    Q_OBJECT

    QProcess sysEventProviderSvc;
public:

    explicit SysEventMonitor(QWidget *parent = 0);
    ~SysEventMonitor();

    void init();

private:
    bool nativeEvent(const QByteArray & eventType, void * message, long * result);

signals:
    // System event notification signals
    void desktopLocked();
    void desktopUnlocked();
    void screensaverStarted();
    void screensaverStopped();
    void powerModeSuspended();
    void powerModeResumed();
    void displayPwrOff();
    void displayPwrOn();

    //void fullscreenModeAppStarted();
    //void fullscreenModeAppStopped();

private slots:
    void onProcessError(QProcess::ProcessError error);
};
