#include <QIcon>
#include <QApplication>
#include <QtDebug>
#include <QTime>
#include <stdexcept>

#include "QWorkBreak.hpp"
#include "resource.hpp"

namespace {
const int TooltipUpdateInterval = 10 * 1000; //ms
}

QWorkBreak::QWorkBreak(QWidget *parent)
    : QSystemTrayIcon(parent)
{
    // setup tray icon
    QPixmap pm(IconPath);
    if (pm.isNull()) {
        throw std::runtime_error("MySystemTrayIcon:: failed to load icon");
    }
    myIcon_ = QIcon(pm);
    setIcon(myIcon_);

    // create about pop-up
    /*@todo use version info const*/
    pAboutBox_ = new QMessageBox(QMessageBox::Information, tr("About"),
                                 tr("QWorkBreak v0.2 x86/win32\nAn open source minimalist Qt based work break reminder"), QMessageBox::Close, nullptr,
                                 Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint |
                                 Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);


    // create notification pop-up
    pBreakNotification_ = new BreakNotification();
    pBreakNotification_->setModal(false);
    connect(pBreakNotification_, SIGNAL(finished(int)), this, SLOT(onBreakNotificationClosed(int)));

    // create work break progress box
    pBreakProgressBox_ = new BreakProgressBox();
    pBreakProgressBox_->setModal(false);
    connect(pBreakProgressBox_, SIGNAL(breakFinished()), this, SLOT(onWorkBreakFinished()));

    // create settings box
    pSettingsDialog = new SettingsDialog();
    connect(pSettingsDialog, SIGNAL(settingsChanged()), this, SLOT(onReset()));

    // setup context menu
    myMenu_.addAction(tr("Stop"), this, SLOT(onStop()));
    myMenu_.addAction(tr("Reset"), this, SLOT(onReset()));
    myMenu_.addAction(tr("Settings"), this, SLOT(onSettings()));
    myMenu_.addAction(tr("About"), this, SLOT(onAbout()));
    myMenu_.addAction(tr("Quit"), this, SLOT(onQuit()));

    setContextMenu(&myMenu_);

    // setup timer
    connect(&myTimer_, SIGNAL(timeout()), this, SLOT(onTimeout()));

    // setup tooltip
    connect(&tooltipUpdateTimer_, SIGNAL(timeout()), this, SLOT(onTooltipUpdate()));
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onActivatd(QSystemTrayIcon::ActivationReason)));

    // start timer
    onReset();
}

QWorkBreak::~QWorkBreak() {
    myTimer_.stop();
    tooltipUpdateTimer_.stop();
    delete pAboutBox_;
    delete pBreakNotification_;
    delete pBreakProgressBox_;
}

void QWorkBreak::onQuit() {
    QApplication::quit();
}

void QWorkBreak::onStop() {
    myTimer_.stop();
    tooltipUpdateTimer_.stop();
    onTooltipUpdate();
    closeNotificationWindows();
}

void QWorkBreak::onReset() {
    closeNotificationWindows();

    //reset timer
    int t = settings_.value(SettingBreakInterval, SettingBreakDurationDefVal).toInt();
    Q_ASSERT(t > 0);
    restartTimer(t);
}

void QWorkBreak::onSettings() {
    closeNotificationWindows();

    //show settings dialog
    pSettingsDialog->show();
    pSettingsDialog->raise();
    pSettingsDialog->activateWindow();
}

void QWorkBreak::onWorkBreakFinished() {
    static const QString TimeFormat("hh:mm:ss");
    static const QString Message(tr("Next work break in %1"));

    closeNotificationWindows();

    //reset timer
    int t = settings_.value(SettingBreakInterval, SettingBreakDurationDefVal).toInt();
    Q_ASSERT(t > 0);
    restartTimer(t);

    QString msg = TimeFormat.arg(QTime::fromMSecsSinceStartOfDay(t).toString(TimeFormat));
    showMessage(tr("Work break finished"), msg);
}

void QWorkBreak::onTimeout() {
    myTimer_.stop();
    tooltipUpdateTimer_.stop();

    pBreakNotification_->show();
    pBreakNotification_->raise();
    pBreakNotification_->activateWindow();
 }

void QWorkBreak::onAbout() {
    pAboutBox_->show();
    pAboutBox_->raise();
    pAboutBox_->activateWindow();
}

void QWorkBreak::restartTimer(int timeOutMs) {
    Q_ASSERT(timeOutMs > 0);

    myTimer_.start(timeOutMs);
    tooltipUpdateTimer_.start(TooltipUpdateInterval);
    onTooltipUpdate();
}

void QWorkBreak::onBreakNotificationClosed(int res) {
    static const QString TimeFormat("hh:mm:ss");
    static const QString Message(tr("Next work break in %1"));

    if (res == BreakNotification::Accepted) {
        // show break time progress bar
        pBreakProgressBox_->show();
        pBreakProgressBox_->raise();
        pBreakProgressBox_->activateWindow();
    } else if (res == BreakNotification::Postponed) {
        // restart timer with postpone timeout
        int t = settings_.value(SettingPostponeTime, SettingPostponeTimeDefVal).toInt();
        Q_ASSERT(t > 0);
        restartTimer(t);

        QString msg = TimeFormat.arg(QTime::fromMSecsSinceStartOfDay(t).toString(TimeFormat));
        showMessage(tr("Work break postponed"), msg);
    } else if (res == BreakNotification::Rejected) {
        // restart timer with random timeout

        //calc max random
        const int MSecInMinute = 60 * 1000;
        int rangeMinutes = (MaxWorkBreakInterval - MinWorkBreakInterval) / MSecInMinute;
        Q_ASSERT(rangeMinutes > 0);
        int timeToBreak = MinWorkBreakInterval + (qrand() % rangeMinutes) * MSecInMinute;
        restartTimer(timeToBreak);

        QString msg = TimeFormat.arg(QTime::fromMSecsSinceStartOfDay(timeToBreak).toString(TimeFormat));
        showMessage(tr("Work break ignored"), msg);
    }
}

void QWorkBreak::closeNotificationWindows() {
    pAboutBox_->hide();
    pBreakNotification_->hide();
    pBreakProgressBox_->hide();

}

void QWorkBreak::onActivatd(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Unknown) {
    } else if (reason == QSystemTrayIcon::Trigger) {
    }
    onTooltipUpdate();
}

void QWorkBreak::onTooltipUpdate() {
    int timeLeft = myTimer_.remainingTime();
    if (timeLeft == -1) { // timer inactive
        setToolTip(tr("Work break reminder stopped"));
    } else {
        int timePassed = myTimer_.interval() - timeLeft;
        static const QString tooltipFormat(tr("Time to break %1\nTime passed %2"));
        static const QString timeFormat("hh:mm:ss");
        QString tooltip = tooltipFormat.
                            arg(QTime::fromMSecsSinceStartOfDay(timeLeft).toString(timeFormat)).
                            arg(QTime::fromMSecsSinceStartOfDay(timePassed).toString(timeFormat));
        setToolTip(tooltip);
    }
}
