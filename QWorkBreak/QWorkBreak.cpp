#include <QIcon>
#include <QApplication>
#include <QtDebug>
#include <QTime>
#include <stdexcept>

#include "QWorkBreak.hpp"
#include "Logger.hpp"
#include "resource.hpp"

namespace {
const int TooltipUpdateInterval = 5 * 1000; //ms
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
                                 tr("QWorkBreak v1.1\nAn open source minimalist Qt based work break reminder utility"), QMessageBox::Close, nullptr,
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
    pSettingsDialog_ = new SettingsDialog();
    connect(pSettingsDialog_, SIGNAL(settingsChanged()), this, SLOT(onReset()));

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

    // setup event monitor
    QObject::connect(&evtMon_, SIGNAL(desktopLocked()), this, SLOT(onStop()));
    QObject::connect(&evtMon_, SIGNAL(screensaverStarted()), this, SLOT(onStop()));
    QObject::connect(&evtMon_, SIGNAL(powerModeSuspended()), this, SLOT(onStop()));
    QObject::connect(&evtMon_, SIGNAL(userStateInactive()), this, SLOT(onStop()));
    QObject::connect(&evtMon_, SIGNAL(desktopUnlocked()), this, SLOT(onReset()));
    QObject::connect(&evtMon_, SIGNAL(screensaverStopped()), this, SLOT(onReset()));
    QObject::connect(&evtMon_, SIGNAL(powerModeResumed()), this, SLOT(onReset()));
    QObject::connect(&evtMon_, SIGNAL(userStateActive()), this, SLOT(onReset()));
    evtMon_.init();

    // start timer
    onReset();
}

QWorkBreak::~QWorkBreak() {
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
    Logger::logEvent(Logger::StopNotifications);
}

void QWorkBreak::onReset() {
    closeNotificationWindows();

    lastBreakTime_ = QDateTime::currentDateTimeUtc();

    //reset timer
    int t = settings_.value(SettingBreakInterval, SettingBreakDurationDefVal).toInt();
    Q_ASSERT(t > 0);
    restartTimer(t);
    Logger::logEvent(Logger::ResetNotifications);
}

void QWorkBreak::onSettings() {
    closeNotificationWindows();

    //show settings dialog
    pSettingsDialog_->show();
    pSettingsDialog_->raise();
    pSettingsDialog_->activateWindow();
}

void QWorkBreak::onWorkBreakFinished() {
    static const QString TimeFormat("hh:mm:ss");
    static const QString Message(tr("Next work break in %1"));

    closeNotificationWindows();

    lastBreakTime_ = QDateTime::currentDateTimeUtc();

    //reset timer
    int t = settings_.value(SettingBreakInterval, SettingBreakDurationDefVal).toInt();
    Q_ASSERT(t > 0);
    restartTimer(t);

    QString msg = Message.arg(QTime::fromMSecsSinceStartOfDay(t).toString(TimeFormat));
    showMessage(tr("Work break finished"), msg);
    Logger::logEvent(Logger::WorkBreakFinished);
}

void QWorkBreak::onTimeout() {
    myTimer_.stop();
    tooltipUpdateTimer_.stop();

    if (evtMon_.isFullScreenAppRunning()) {
        // restart timer with postpone timeout
        int t = settings_.value(SettingPostponeTime, SettingPostponeTimeDefVal).toInt();
        Q_ASSERT(t > 0);
        restartTimer(t);
        Logger::logEvent(Logger::WorkBreakNotificationPostponed);
    } else {
        Logger::logEvent(Logger::WorkBreakNotificationDisplayed);
        pBreakNotification_->show();
        pBreakNotification_->raise();
        pBreakNotification_->activateWindow();
    }
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

        QString msg = Message.arg(QTime::fromMSecsSinceStartOfDay(t).toString(TimeFormat));
        showMessage(tr("Work break postponed"), msg);
    } else if (res == BreakNotification::Rejected) {
        // restart timer with random timeout

        //calc max random
        const int MSecInMinute = 60 * 1000;
        int workbreakInterval = settings_.value(SettingBreakInterval, SettingBreakDurationDefVal).toInt();
        int rangeMinutes = std::max((workbreakInterval - MinWorkBreakInterval) / MSecInMinute, 1);
        int timeToBreak = MinWorkBreakInterval + (qrand() % rangeMinutes) * MSecInMinute;
        restartTimer(timeToBreak);

        QString msg = Message.arg(QTime::fromMSecsSinceStartOfDay(timeToBreak).toString(TimeFormat));
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
    static const int MsInDay = 24 * 60 * 60 * 1000;
    static const QString tooltipFormat(tr("Time left till the next break %1\nTime elapsed since the last break %2"));
    static const QString timeFormat("hh:mm:ss");

    int timeLeft = myTimer_.remainingTime();
    if (timeLeft == -1) { // timer inactive
        setToolTip(tr("Work break reminder stopped"));
    } else {
        QString timePassedStr;
        int timePassed = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() - lastBreakTime_.toMSecsSinceEpoch();
        if (timePassed > MsInDay) {
            timePassedStr = tr(">24 hours!");
        } else {
            timePassedStr = QTime::fromMSecsSinceStartOfDay(timePassed).toString(timeFormat);
        }

        QString tooltip = tooltipFormat.
                            arg(QTime::fromMSecsSinceStartOfDay(timeLeft).toString(timeFormat)).
                            arg(timePassedStr);
        setToolTip(tooltip);
    }
}
