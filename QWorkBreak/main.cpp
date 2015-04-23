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
        QWorkBreak w;
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

