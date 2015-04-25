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

    ui->IntervalTimeEdit->setTimeRange(
                QTime::fromMSecsSinceStartOfDay(0),
                QTime::fromMSecsSinceStartOfDay(MaxWorkBreakInterval));

    ui->DurationTimeEdit->setTimeRange(
                QTime::fromMSecsSinceStartOfDay(0),
                QTime::fromMSecsSinceStartOfDay(MaxWorkBreakDuration));

    ui->InactivityThresholdEdit->setTimeRange(
                QTime::fromMSecsSinceStartOfDay(0),
                QTime::fromMSecsSinceStartOfDay(MaxInactivityThreshold));
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

    int inactivityThreshMsec = settings_.value(SettingInactivityThreshold, SettingInactivityThresholdDefVal).toInt();
    Q_ASSERT(inactivityThreshMsec >= MinInactivityThreshold);
    Q_ASSERT(inactivityThreshMsec <= MaxInactivityThreshold);
    ui->InactivityThresholdEdit->setTime(QTime::fromMSecsSinceStartOfDay(inactivityThreshMsec));

    update();
    QDialog::show();
    Logger::logEvent(Logger::SettingsDisplayed);
}

void SettingsDialog::accept() {
    //save settings
    int intervalMsec = std::max(ui->IntervalTimeEdit->time().msecsSinceStartOfDay(), MinWorkBreakInterval);
    settings_.setValue(SettingBreakInterval, QVariant::fromValue(intervalMsec));

    int durationMsec = std::max(ui->DurationTimeEdit->time().msecsSinceStartOfDay(), MinWorkBreakDuration);
    settings_.setValue(SettingBreakDuration, QVariant::fromValue(durationMsec));

    int inactivityThreshMsec = std::max(ui->InactivityThresholdEdit->time().msecsSinceStartOfDay(), MinInactivityThreshold);
    settings_.setValue(SettingInactivityThreshold, QVariant::fromValue(inactivityThreshMsec));

    QDialog::accept();
    Logger::logEvent(Logger::SettingsChanged);
    emit settingsChanged();
}

void SettingsDialog::reject() {
    QDialog::reject();
}
