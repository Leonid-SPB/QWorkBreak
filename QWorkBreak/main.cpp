#include <QApplication>
#include <QtDebug>
#include <QSettings>
#include <QMessageBox>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

#include "QWorkBreak.hpp"
#include "SysEventMonitor.hpp"
#include "resource.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(OrganizationName);
    QCoreApplication::setOrganizationDomain(OrganizationDomain);
    QCoreApplication::setApplicationName(ApplicationName);

    qsrand(time(0));

    try {
        QWorkBreak w;
        SysEventMonitor m;

        QObject::connect(&m, SIGNAL(desktopLocked()), &w, SLOT(onStop()));
        QObject::connect(&m, SIGNAL(screensaverStarted()), &w, SLOT(onStop()));
        QObject::connect(&m, SIGNAL(powerModeSuspended()), &w, SLOT(onStop()));
        QObject::connect(&m, SIGNAL(desktopUnlocked()), &w, SLOT(onReset()));
        QObject::connect(&m, SIGNAL(screensaverStopped()), &w, SLOT(onReset()));
        QObject::connect(&m, SIGNAL(powerModeResumed()), &w, SLOT(onReset()));

        m.createWinId();

        w.setVisible(true);
        a.setQuitOnLastWindowClosed(false);
        return a.exec();
    } catch (std::runtime_error &e) {
        qDebug() << "Something wrong happened: " << e.what();
    } catch (...) {
        qDebug() << "Something very wrong happened";
    }

    QMessageBox::critical(nullptr, QCoreApplication::tr("Runtime error"), QCoreApplication::tr("Something went wrong, sorry :-("));
    exit(1);
}

