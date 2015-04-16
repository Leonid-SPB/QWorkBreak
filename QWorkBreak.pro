#-------------------------------------------------
#
# Project created by QtCreator 2015-04-13T15:11:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QWorkBreak
TEMPLATE = app


SOURCES += main.cpp\
        QWorkBreak.cpp \
    BreakProgressBox.cpp \
    SettingsDialog.cpp \
    BreakNotification.cpp \
    SysEventMonitor.cpp

HEADERS  += QWorkBreak.hpp\
            resource.hpp \
    BreakProgressBox.hpp \
    SettingsDialog.hpp \
    BreakNotification.hpp \
    SysEventMonitor.hpp

RESOURCES += resource.qrc

RC_ICONS = res/myappicon.ico

FORMS += \
    BreakProgressBox.ui \
    SettingsDialog.ui \
    BreakNotification.ui

CONFIG += c++11
