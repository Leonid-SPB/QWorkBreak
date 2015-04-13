#include <QtDebug>
#include "BreakProgressBox.hpp"
#include "ui_BreakProgressBox.h"
#include "resource.hpp"

BreakProgressBox::BreakProgressBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BreakProgressBox)
{
    ui->setupUi(this);
    connect(&progressUpdateTimer_, SIGNAL(timeout()), this, SLOT(onProgressUpdate()));
}

BreakProgressBox::~BreakProgressBox() {
    progressUpdateTimer_.stop();
    delete ui;
}

void BreakProgressBox::show() {
    qDebug() << "BreakProgressBox::show()";
    int duration_sec = settings_.value(SettingBreakDuration, SettingBreakDurationDefVal).toInt() / 1000;
    Q_ASSERT(duration_sec > 0);

    ui->progressBar->setRange(0, duration_sec);
    ui->progressBar->setValue(0);

    const int OneSecond = 1000;
    progressUpdateTimer_.start(OneSecond);

    QDialog::show();
}

void BreakProgressBox::hide() {
    if (isVisible()) {
        qDebug() << "BreakProgressBox::hide()";
        QDialog::hide();
        progressUpdateTimer_.stop();
        emit breakFinished();
    }
}

void BreakProgressBox::onProgressUpdate(){
    int val = ui->progressBar->value();

    if (++val < ui->progressBar->maximum()) { // work break in progress
        ui->progressBar->setValue(val);
        return;
    } else { // work break finished
        qDebug() << "Work break finished";
        hide();
    }
}
