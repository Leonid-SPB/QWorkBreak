#include <QtDebug>
#include "BreakProgressBox.hpp"
#include "ui_BreakProgressBox.h"
#include "resource.hpp"
#include "Logger.hpp"

BreakProgressBox::BreakProgressBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BreakProgressBox)
{
    ui->setupUi(this);
    connect(&progressUpdateTimer_, SIGNAL(timeout()), this, SLOT(onProgressUpdate()));
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

BreakProgressBox::~BreakProgressBox() {
    delete ui;
}

void BreakProgressBox::show() {
    int duration_sec = settings_.value(SettingBreakDuration, SettingBreakDurationDefVal).toInt() / 1000;
    Q_ASSERT(duration_sec > 0);

    ui->progressBar->setRange(0, duration_sec);
    ui->progressBar->setValue(0);

    const int OneSecond = 1000;
    progressUpdateTimer_.start(OneSecond);

    QDialog::show();
    Logger::logEvent(Logger::WorkBreakProgressDisplayed);
}

void BreakProgressBox::hide() {
    if (isVisible()) {
        QDialog::hide();
        progressUpdateTimer_.stop();
        Logger::logEvent(Logger::WorkBreakProgressClosed);
        emit breakFinished();
    }
}

void BreakProgressBox::onProgressUpdate(){
    int val = ui->progressBar->value();

    if (++val < ui->progressBar->maximum()) { // work break in progress
        ui->progressBar->setValue(val);
        return;
    } else { // work break finished
        hide();
    }
}
