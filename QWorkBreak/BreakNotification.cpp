#include "BreakNotification.hpp"
#include "ui_BreakNotification.h"

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
    done(Result::Accepted);
}

void BreakNotification::postpone() {
    done(Result::Postponed);
}

void BreakNotification::reject() {
    done(Result::Rejected);
}
