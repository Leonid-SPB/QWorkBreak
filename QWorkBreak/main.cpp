#include <QApplication>
#include <QtDebug>
#include <QSettings>
#include <QMessageBox>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

#include "QWorkBreak.hpp"
#include "SysEventMonitor.hpp"
#include "Logger.hpp"
#include "resource.hpp"

int main(int argc, char *argv[]) {
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);

    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(OrganizationName);
    QCoreApplication::setOrganizationDomain(OrganizationDomain);
    QCoreApplication::setApplicationName(ApplicationName);

    qsrand(time(0));

    try {
        Logger::logEvent(Logger::ApplicationStarted);
        QWorkBreak w;
        w.setVisible(true);
        a.setQuitOnLastWindowClosed(false);
        int ret = a.exec();
        Logger::logEvent(Logger::ApplicationFinished);
        return ret;
    } catch (std::runtime_error &e) {
        qDebug() << "Something wrong happened: " << e.what();
    } catch (...) {
        qDebug() << "Something very wrong happened";
    }

    Logger::logEvent(Logger::RuntimeError);
    Logger::logEvent(Logger::ApplicationFinished);
    QMessageBox::critical(nullptr, QCoreApplication::tr("Runtime error"), QCoreApplication::tr("Something went wrong, sorry :-("));
    exit(1);
}

