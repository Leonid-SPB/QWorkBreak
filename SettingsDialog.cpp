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
    qDebug() << "SettingsDialog::show()";

    int intervalMsec = settings_.value(SettingBreakInterval, SettingBreakIntervalDefVal).toInt();
    Q_ASSERT(intervalMsec > 1000);
    ui->IntervalTimeEdit->setTime(QTime::fromMSecsSinceStartOfDay(intervalMsec));

    int durationMsec = settings_.value(SettingBreakDuration, SettingBreakDurationDefVal).toInt();
    Q_ASSERT(durationMsec > 1000);
    ui->DurationTimeEdit->setTime(QTime::fromMSecsSinceStartOfDay(durationMsec));

    bool rstOnLock = settings_.value(SettingResetOnDesktopLocked, SettingResetOnDesktopLockedDefVal).toBool();
    ui->ResetOnLockChk->setChecked(rstOnLock);

    update();
    QDialog::show();
}

void SettingsDialog::accept() {
    //save settings
    qDebug() << "SettingsDialog::update settings";

    int intervalMsec = std::max(ui->IntervalTimeEdit->time().msecsSinceStartOfDay(), MinWorkBreakInterval);
    settings_.setValue(SettingBreakInterval, QVariant::fromValue(intervalMsec));

    int durationMsec = std::max(ui->DurationTimeEdit->time().msecsSinceStartOfDay(), MinWorkBreakDuration);
    settings_.setValue(SettingBreakDuration, QVariant::fromValue(durationMsec));

    settings_.setValue(SettingResetOnDesktopLocked, QVariant::fromValue(ui->ResetOnLockChk->isChecked()));

    QDialog::accept();
}

void SettingsDialog::reject() {
    qDebug() << "SettingsDialog::reject settings";
    QDialog::reject();
}
