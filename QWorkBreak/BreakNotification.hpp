#pragma once
#include <QDialog>

namespace Ui {
class BreakNotification;
}

class BreakNotification : public QDialog {
public:
    enum Result {
        Accepted = 0,
        Postponed,
        Rejected
    };

private:
    Q_OBJECT

public:
    explicit BreakNotification(QWidget *parent = 0);
    ~BreakNotification();

private:
    Ui::BreakNotification *ui;

public slots:
    virtual void accept();
    virtual void postpone();
    virtual void reject();
};
