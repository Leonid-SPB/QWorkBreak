#pragma once

const char OrganizationName[]   = "GNU";
const char OrganizationDomain[] = "GNU.org";
const char ApplicationName[]    = "QWorkBreak";

const char IconPath[] = ":/res/timer.png";

const char SettingBreakInterval[]            = "settings/BreakInterval";
const int  SettingBreakIntervalDefVal        = 1 * 60 * 60 * 1000; //ms
const char SettingBreakDuration[]            = "settings/BreakDuration";
const int  SettingBreakDurationDefVal        =      5 * 60 * 1000; //ms
const char SettingResetOnDesktopLocked[]     = "settings/ResetOnDesktopLocked";
const bool SettingResetOnDesktopLockedDefVal = true;
const char SettingPostponeTime[]             = "settings/PostponeTime";
const int  SettingPostponeTimeDefVal         =       5 * 60 * 1000; //ms
const char SettingInactivityThreshold[]      = "settings/InactivityThreshold";
const int  SettingInactivityThresholdDefVal  =      10 * 60 * 1000; //ms

const int  MinWorkBreakInterval =     30 * 60 * 1000;// ms
const int  MaxWorkBreakInterval = 4 * 60 * 60 * 1000;// ms
const int  MinWorkBreakDuration =      1 * 60 * 1000;// ms
const int  MaxWorkBreakDuration = 1 * 60 * 60 * 1000;// ms
const int  MinInactivityThreshold =      1 * 60 * 1000;// ms
const int  MaxInactivityThreshold =     30 * 60 * 1000;// ms
