#include <QDebug>
#include "SettingsDialog.hpp"
#include "ui_SettingsDialog.h"
#include "resource.hpp"
#include "Logger.hpp"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->IntervalTimeEdit->setRange(
                msecToMin(MinWorkBreakInterval),
                msecToMin(MaxWorkBreakInterval));

    ui->DurationTimeEdit->setRange(
                msecToMin(MinWorkBreakDuration),
                msecToMin(MaxWorkBreakDuration));

    ui->InactivityThresholdEdit->setRange(
                msecToMin(MinInactivityThreshold),
                msecToMin(MaxInactivityThreshold));
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

int SettingsDialog::msecToMin(int timeMsec) {
    return timeMsec / (1000 * 60);
}

int SettingsDialog::minToMsec(int timeMin) {
    return timeMin * 60 * 1000;
}


void SettingsDialog::show() {
    //update gui
    int intervalMsec = settings_.value(SettingBreakInterval, SettingBreakIntervalDefVal).toInt();
    Q_ASSERT(intervalMsec >= MinWorkBreakInterval);
    Q_ASSERT(intervalMsec <= MaxWorkBreakInterval);
    ui->IntervalTimeEdit->setValue(msecToMin(intervalMsec));

    int durationMsec = settings_.value(SettingBreakDuration, SettingBreakDurationDefVal).toInt();
    Q_ASSERT(durationMsec >= MinWorkBreakDuration);
    Q_ASSERT(durationMsec <= MaxWorkBreakDuration);
    ui->DurationTimeEdit->setValue(msecToMin(durationMsec));

    int inactivityThreshMsec = settings_.value(SettingInactivityThreshold, SettingInactivityThresholdDefVal).toInt();
    Q_ASSERT(inactivityThreshMsec >= MinInactivityThreshold);
    Q_ASSERT(inactivityThreshMsec <= MaxInactivityThreshold);
    ui->InactivityThresholdEdit->setValue(msecToMin(inactivityThreshMsec));

    update();
    QDialog::show();
    Logger::logEvent(Logger::SettingsDisplayed);
}

void SettingsDialog::accept() {
    //save settings
    settings_.setValue(SettingBreakInterval, QVariant::fromValue(minToMsec(ui->IntervalTimeEdit->value())));
    settings_.setValue(SettingBreakDuration, QVariant::fromValue(minToMsec(ui->DurationTimeEdit->value())));
    settings_.setValue(SettingInactivityThreshold, QVariant::fromValue(minToMsec(ui->InactivityThresholdEdit->value())));

    QDialog::accept();
    Logger::logEvent(Logger::SettingsChanged);
    emit settingsChanged();
}

void SettingsDialog::reject() {
    QDialog::reject();
}
