#pragma once
#include <QWidget>
#include <QProcess>
#include <QSettings>
#include <QTimer>

/** @brief SysEventMonitor provides notifications about following events:
 * desktop locked, desktop unlocked, screensaver started, screensaver stopped.
 *
 * On Windows platform SysEventMonitor relies on external SysEventNotificationProvider service, which
 * uses Win32 API messages to inform SysEventMonitor about system events.
*/
class SysEventMonitor : public QWidget {
    Q_OBJECT

    QProcess sysEventProviderSvc_;
    QTimer myTimer_;
    QSettings settings_;
    bool activeState_;
public:

    explicit SysEventMonitor(QWidget *parent = 0);
    ~SysEventMonitor();

    void init();

    bool isFullScreenAppRunning();

private:
    bool nativeEvent(const QByteArray & eventType, void * message, long * result) Q_DECL_OVERRIDE;
    void restartTimer();

signals:
    // System event notification signals
    void desktopLocked();
    void desktopUnlocked();
    void screensaverStarted();
    void screensaverStopped();
    void powerModeSuspended();
    void powerModeResumed();
    void userStateInactive();
    void userStateActive();

private slots:
    void onProcessError(QProcess::ProcessError error);
    void onTimeout();
};
