#include <QIcon>
#include <QApplication>
#include <stdexcept>

#include "QWorkBreak.hpp"
#include "resource.hpp"

QWorkBreak::QWorkBreak(QWidget *parent)
    : QSystemTrayIcon(parent)
{
    QPixmap pm(IconPath);
    if (pm.isNull()) {
        throw std::runtime_error("MySystemTrayIcon:: failed to load icon");
    }
    myIcon_ = QIcon(pm);
    setIcon(myIcon_);

    myMenu_.addAction(tr("Quit"), this, SLOT(onQuit()));
    setContextMenu(&myMenu_);

    setToolTip(tr("QWorkBreak test"));
}

QWorkBreak::~QWorkBreak()
{

}

void QWorkBreak::onQuit() {
    QApplication::quit();
}
