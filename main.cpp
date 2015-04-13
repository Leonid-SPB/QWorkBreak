#include <QApplication>
#include <stdexcept>
#include <QtDebug>
#include <cstdlib>

#include "QWorkBreak.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    try {
        QWorkBreak w;
        w.setVisible(true);
        //w.showMessage(a.tr("Hi"), a.tr("Just Hi =)"));

        return a.exec();
    } catch (std::runtime_error &e) {
        qDebug() << "Something wrong happened: " << e.what();
    }

    exit(1);
}
