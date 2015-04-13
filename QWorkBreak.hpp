#pragma once
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMenu>
#include <QMessageBox>

class QWorkBreak : public QSystemTrayIcon {
    Q_OBJECT

    QIcon myIcon_;
    QMenu myMenu_;
    QMessageBox *pAboutBox_;
public:
    QWorkBreak(QWidget *parent = 0);
    ~QWorkBreak();

private slots:
    void onQuit();
    void onPause();
    void onReset();
    void onSettings();
    void onAbout();
};
