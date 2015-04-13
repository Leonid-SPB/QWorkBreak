#include <QIcon>
#include <QApplication>
#include <stdexcept>

#include "QWorkBreak.hpp"
#include "resource.hpp"

QWorkBreak::QWorkBreak(QWidget *parent)
    : QSystemTrayIcon(parent), pAboutBox_(nullptr), pNotificationBox_(nullptr)
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
    pNotificationBox_ = new QMessageBox(QMessageBox::Warning, tr("Time to break!"),
                                 tr("It's time to take a break ;-)              "), QMessageBox::Cancel | QMessageBox::Ok, nullptr,
                                 Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint |
                                 Qt::WindowMinimizeButtonHint);
    connect(pNotificationBox_, SIGNAL(accepted()), this, SLOT(onBreakAccepted()));
    connect(pNotificationBox_, SIGNAL(rejected()), this, SLOT(onBreakRejected()));

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
    setToolTip(tr("QWorkBreak test tooltip"));

    // start timer
    onReset();
}

QWorkBreak::~QWorkBreak() {
    delete pAboutBox_;
    delete pNotificationBox_;
}

void QWorkBreak::onQuit() {
    QApplication::quit();
}

void QWorkBreak::onStop() {
    myTimer_.stop();

    closeNotificationWindows();
}

void QWorkBreak::onReset() {
    closeNotificationWindows();

    //reset timer
    int t = settings_.value(SettingBreakInterval, SettingBreakDurationDefVal).toInt();
    myTimer_.start(t);
}

void QWorkBreak::onSettings() {
    closeNotificationWindows();

    //show settings dialog
}

void QWorkBreak::onTimeout() {
    myTimer_.stop();

    pNotificationBox_->show();
    pNotificationBox_->raise();
    pNotificationBox_->setFocus(Qt::PopupFocusReason);
 }

void QWorkBreak::onAbout() {
    pAboutBox_->show();
    pAboutBox_->raise();
    pAboutBox_->setFocus(Qt::PopupFocusReason);
}

void QWorkBreak::onBreakAccepted() {
    // show break time progress bar
    // then restart timer
}

void QWorkBreak::onBreakRejected() {
    // just restart timer
    onReset();
}

void QWorkBreak::closeNotificationWindows() {
    pAboutBox_->hide();
    pNotificationBox_->hide();

}
