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

    // convert milliseconds to minutes
    int msecToMin(int timeMsec);

    // convert minutes to milliseconds
    int minToMsec(int timeMin);

public slots:
    void show();
    void accept();
    void reject();

signals:
    void settingsChanged();
};
