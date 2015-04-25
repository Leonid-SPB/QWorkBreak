#pragma once
#include <QtDebug>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <stdexcept>

namespace Logger {

enum LogEvents {
    RuntimeError,
    ApplicationStarted,
    ApplicationFinished,
    WorkBreakNotificationDisplayed,
    WorkBreakNotificationPostponed,
    WorkBreakPostponed,
    WorkBreakIgnored,
    WorkBreakAccepted,
    WorkBreakFinished,
    WorkstationLocked,
    WorkstationUnlocked,
    WorkBreakProgressDisplayed,
    WorkBreakProgressClosed,
    PowerModeSuspended,
    PowerModeResumed,
    UserStateInactive,
    UserStateActive,
    StopNotifications,
    ResetNotifications,
    SettingsDisplayed,
    SettingsChanged
};

const char LogFileName[] = "QWorkBreak.log";


inline void logMsg(const QString &msg) {
    static QFile logFile(LogFileName);
    static QTextStream qts;

    if (!logFile.isOpen()) {
        if (!logFile.open(QFile::WriteOnly | QIODevice::Append)) {
            throw std::runtime_error("Failed to open log file");
        }
        qts.setDevice(&logFile);
    }

    qts << msg << "\n";
    qts.flush();
}


inline void logEvent(LogEvents e) {
    const QString DateTimeFormat("yyyy.MM.dd hh:mm:ss");
    const QString MessageFormat("%1 - %2");
    QDateTime localTime = QDateTime::currentDateTime();
    QString msg;

    switch (e) {
    case RuntimeError:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Runtime error");
        break;
    case ApplicationStarted:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Application started");
        break;
    case ApplicationFinished:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Application finished");
        break;
    case WorkBreakNotificationDisplayed:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Work break notification displayed");
        break;
    case WorkBreakPostponed:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Work break postponed");
        break;
    case WorkBreakNotificationPostponed:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Work break postponed, fullscreen app");
        break;
    case WorkBreakIgnored:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Work break ignored");
        break;
    case WorkBreakAccepted:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Work break accepted");
        break;
    case WorkBreakFinished:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Work break finished");
        break;
    case WorkstationLocked:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Workstation locked");
        break;
    case WorkstationUnlocked:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Workstation unlocked");
        break;
    case WorkBreakProgressDisplayed:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Work break progress displayed");
        break;
    case WorkBreakProgressClosed:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Work break progress closed");
        break;
    case PowerModeSuspended:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Power mode suspended");
        break;
    case PowerModeResumed:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Power mode resumed");
        break;
    case UserStateInactive:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "User state inactive");
        break;
    case UserStateActive:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "User state active");
        break;
    case StopNotifications:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Notifications stopped");
        break;
    case ResetNotifications:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Notifications reset");
        break;
    case SettingsDisplayed:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Settings displayed");
        break;
    case SettingsChanged:
        msg = MessageFormat.arg(localTime.toString(DateTimeFormat), "Settings changed");
        break;
    }

    logMsg(msg);
}

}
