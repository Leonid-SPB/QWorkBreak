#pragma once
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

#include "BreakNotification.hpp"
#include "BreakProgressBox.hpp"
#include "SettingsDialog.hpp"

class QWorkBreak : public QSystemTrayIcon {
    Q_OBJECT

    QIcon myIcon_;
    QMenu myMenu_;
    QMessageBox *pAboutBox_;
    BreakProgressBox *pBreakProgressBox_;
    BreakNotification *pBreakNotification_;
    SettingsDialog *pSettingsDialog;

    QTimer myTimer_;
    QTimer tooltipUpdateTimer_;

    QSettings settings_;
public:
    QWorkBreak(QWidget *parent = 0);
    ~QWorkBreak();

private:
    void closeNotificationWindows();
    void restartTimer(int timeOutMs);

private slots:
    // context menu actions
    void onQuit();
    void onStop();
    void onReset();
    void onSettings();
    void onAbout();

    // break notification actions
    void onBreakNotificationClosed(int res);
    void onWorkBreakFinished();

    // break timer expired
    void onTimeout();

    // tooltip related
    void onTooltipUpdate();
    void onActivatd(QSystemTrayIcon::ActivationReason reason);
};
