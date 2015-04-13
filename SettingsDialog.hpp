#pragma once
#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;
    QSettings settings_;

public slots:
    virtual void show();
    virtual void accept();
    virtual void reject();
};
