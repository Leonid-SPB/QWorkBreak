#pragma once
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMenu>

class QWorkBreak : public QSystemTrayIcon {
    Q_OBJECT

    QIcon myIcon_;
    QMenu myMenu_;
public:
    QWorkBreak(QWidget *parent = 0);
    ~QWorkBreak();

private slots:
    void onQuit();
};
