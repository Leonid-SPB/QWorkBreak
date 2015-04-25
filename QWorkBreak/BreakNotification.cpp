#include "BreakNotification.hpp"
#include "ui_BreakNotification.h"
#include "Logger.hpp"

BreakNotification::BreakNotification(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BreakNotification)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

BreakNotification::~BreakNotification() {
    delete ui;
}

void BreakNotification::accept() {
    Logger::logEvent(Logger::WorkBreakAccepted);
    done(Result::Accepted);
}

void BreakNotification::postpone() {
    Logger::logEvent(Logger::WorkBreakPostponed);
    done(Result::Postponed);
}

void BreakNotification::reject() {
    Logger::logEvent(Logger::WorkBreakIgnored);
    done(Result::Rejected);
}
