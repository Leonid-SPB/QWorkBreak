#pragma once
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

#include "BreakProgressBox.hpp"
#include "SettingsDialog.hpp"

class QWorkBreak : public QSystemTrayIcon {
    Q_OBJECT

    QIcon myIcon_;
    QMenu myMenu_;
    QMessageBox *pAboutBox_;
    QMessageBox *pNotificationBox_;
    BreakProgressBox *pBreakProgressBox_;
    SettingsDialog *pSettingsDialog;

    QTimer myTimer_;

    QSettings settings_;
public:
    QWorkBreak(QWidget *parent = 0);
    ~QWorkBreak();

private:
    void closeNotificationWindows();

private slots:
    // context menu actions
    void onQuit();
    void onStop();
    void onReset();
    void onSettings();
    void onAbout();

    // break notification actions
    void onBreakNotificationClosed(int res);

    // break timer expired
    void onTimeout();
};
