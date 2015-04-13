#include <QIcon>
#include <QApplication>
#include <stdexcept>

#include "QWorkBreak.hpp"
#include "resource.hpp"

QWorkBreak::QWorkBreak(QWidget *parent)
    : QSystemTrayIcon(parent), pAboutBox_(nullptr)
{
    QPixmap pm(IconPath);
    if (pm.isNull()) {
        throw std::runtime_error("MySystemTrayIcon:: failed to load icon");
    }
    myIcon_ = QIcon(pm);
    setIcon(myIcon_);

    myMenu_.addAction(tr("Pause"), this, SLOT(onPause()));
    myMenu_.addAction(tr("Reset"), this, SLOT(onReset()));
    myMenu_.addAction(tr("Settings"), this, SLOT(onSettings()));
    myMenu_.addAction(tr("About"), this, SLOT(onAbout()));
    myMenu_.addAction(tr("Quit"), this, SLOT(onQuit()));

    setContextMenu(&myMenu_);

    setToolTip(tr("QWorkBreak test"));
}

QWorkBreak::~QWorkBreak() {
    delete pAboutBox_;
}

void QWorkBreak::onQuit() {
    QApplication::quit();
}

void QWorkBreak::onPause() {

}

void QWorkBreak::onReset() {

}

void QWorkBreak::onSettings() {

}

void QWorkBreak::onAbout() {
    /*@todo use version info const*/
    if (!pAboutBox_) {
        pAboutBox_ = new QMessageBox(QMessageBox::Information, tr("About"),
                                     tr("QWorkBreak v0.2 x86/win32\nAn open source minimalist Qt based work break reminder"), QMessageBox::Close, nullptr,
                                     Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint |
                                     Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
        //pAboutBox_->setWindowModality(Qt::NonModal);
    }

    pAboutBox_->show();
    pAboutBox_->raise();
    pAboutBox_->setFocus(Qt::PopupFocusReason);
}
