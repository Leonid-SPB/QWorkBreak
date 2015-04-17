#include <QDebug>
#include "SettingsDialog.hpp"
#include "ui_SettingsDialog.h"
#include "resource.hpp"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->IntervalTimeEdit->setTimeRange(
                QTime::fromMSecsSinceStartOfDay(0),
                QTime::fromMSecsSinceStartOfDay(MaxWorkBreakInterval));

    ui->DurationTimeEdit->setTimeRange(
                QTime::fromMSecsSinceStartOfDay(0),
                QTime::fromMSecsSinceStartOfDay(MaxWorkBreakDuration));
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::show() {
    //update gui
    int intervalMsec = settings_.value(SettingBreakInterval, SettingBreakIntervalDefVal).toInt();
    Q_ASSERT(intervalMsec >= MinWorkBreakInterval);
    Q_ASSERT(intervalMsec <= MaxWorkBreakInterval);
    ui->IntervalTimeEdit->setTime(QTime::fromMSecsSinceStartOfDay(intervalMsec));

    int durationMsec = settings_.value(SettingBreakDuration, SettingBreakDurationDefVal).toInt();
    Q_ASSERT(durationMsec >= MinWorkBreakDuration);
    Q_ASSERT(durationMsec <= MaxWorkBreakDuration);
    ui->DurationTimeEdit->setTime(QTime::fromMSecsSinceStartOfDay(durationMsec));

    update();
    QDialog::show();
}

void SettingsDialog::accept() {
    //save settings
    int intervalMsec = std::max(ui->IntervalTimeEdit->time().msecsSinceStartOfDay(), MinWorkBreakInterval);
    settings_.setValue(SettingBreakInterval, QVariant::fromValue(intervalMsec));

    int durationMsec = std::max(ui->DurationTimeEdit->time().msecsSinceStartOfDay(), MinWorkBreakDuration);
    settings_.setValue(SettingBreakDuration, QVariant::fromValue(durationMsec));

    QDialog::accept();
    emit settingsChanged();
}

void SettingsDialog::reject() {
    QDialog::reject();
}
