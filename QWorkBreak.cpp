#include <QIcon>
#include <QApplication>
#include <QtDebug>
#include <QTime>
#include <stdexcept>

#include "QWorkBreak.hpp"
#include "resource.hpp"

const int TooltipUpdateInterval = 10 * 1000; //ms

QWorkBreak::QWorkBreak(QWidget *parent)
    : QSystemTrayIcon(parent), pAboutBox_(nullptr), pNotificationBox_(nullptr), pBreakProgressBox_(nullptr)
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
    pAboutBox_->setModal(false);


    // create notification pop-up
    pNotificationBox_ = new QMessageBox(QMessageBox::Warning, tr("Time to break!"),
                                 tr("It's time to take a break ;-)              "), QMessageBox::Cancel | QMessageBox::Ok, nullptr,
                                 Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint |
                                 Qt::WindowMinimizeButtonHint);
    //pNotificationBox_->setModal(false);
    connect(pNotificationBox_, SIGNAL(finished(int)), this, SLOT(onBreakNotificationClosed(int)));

    // create work break progress box
    pBreakProgressBox_ = new BreakProgressBox();
    //pBreakProgressBox_->setModal(false);
    connect(pBreakProgressBox_, SIGNAL(breakFinished()), this, SLOT(onReset()));

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
    delete pNotificationBox_;
    delete pBreakProgressBox_;
}

void QWorkBreak::onQuit() {
    QApplication::quit();
}

void QWorkBreak::onStop() {
    qDebug() << "onStop()";
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
    myTimer_.start(t);
    tooltipUpdateTimer_.start(TooltipUpdateInterval);
    onTooltipUpdate();
}

void QWorkBreak::onSettings() {
    closeNotificationWindows();

    //show settings dialog
    pSettingsDialog->show();
    pSettingsDialog->raise();
    pSettingsDialog->activateWindow();
}

void QWorkBreak::onTimeout() {
    myTimer_.stop();
    tooltipUpdateTimer_.stop();

    pNotificationBox_->show();
    pNotificationBox_->raise();
    pNotificationBox_->activateWindow();
 }

void QWorkBreak::onAbout() {
    pAboutBox_->show();
    pAboutBox_->raise();
    pAboutBox_->activateWindow();
}

void QWorkBreak::onBreakNotificationClosed(int res) {
    qDebug() << "onBreakNotificationClosed()" << res;

    if (res == QMessageBox::Ok) {
        // show break time progress bar
        qDebug() << "break accepted";
        pBreakProgressBox_->show();
        pBreakProgressBox_->raise();
        pBreakProgressBox_->activateWindow();
    } else {
        // just restart timer
        qDebug() << "break rejected";
        onReset();
    }
}

void QWorkBreak::closeNotificationWindows() {
    pAboutBox_->hide();
    pNotificationBox_->hide();
    pBreakProgressBox_->hide();

}

void QWorkBreak::onActivatd(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Unknown) {
        qDebug() << "Activated, unknown";
    } else if (reason == QSystemTrayIcon::Trigger) {
        qDebug() << "Activated, click";
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
