#pragma once
#include <QDialog>
#include <QTimer>
#include <QSettings>


namespace Ui {
class BreakProgressBox;
}

class BreakProgressBox : public QDialog {
    Q_OBJECT

    QTimer progressUpdateTimer_;
    QSettings settings_;
public:
    explicit BreakProgressBox(QWidget *parent = 0);
    ~BreakProgressBox();

private:
    Ui::BreakProgressBox *ui;

    //virtual void closeEvent(QCloseEvent * event);

signals:
    void breakFinished();

public slots:
    virtual void show();
    virtual void hide();

private slots:
    void onProgressUpdate();
};
