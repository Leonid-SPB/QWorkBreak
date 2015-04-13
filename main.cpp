#include <QApplication>
#include <QtDebug>
#include <QSettings>
#include <QMessageBox>
#include <stdexcept>
#include <cstdlib>

#include "QWorkBreak.hpp"
#include "resource.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(OrganizationName);
    QCoreApplication::setOrganizationDomain(OrganizationDomain);
    QCoreApplication::setApplicationName(ApplicationName);

    try {
        QWorkBreak w;
        w.setVisible(true);
        //w.showMessage(a.tr("Hi"), a.tr("Just Hi =)"));

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
