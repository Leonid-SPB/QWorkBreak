﻿using System;
using System.Windows.Forms;
using System.Drawing;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Microsoft.Win32;

namespace SysEventNotificationProvider {
    public class Program : Form {
        private const int WM_USER = 0x0400;

        private enum SysEventMessages {
            DesktopLock      = WM_USER + 0,
            DesktopUnlock    = WM_USER + 1,
            StartScreenSaver = WM_USER + 2,
            StopScreenSaver  = WM_USER + 3,
            PowerModeSusped  = WM_USER + 4,
            PowerModeResume  = WM_USER + 5
        };

        // window identificator for system event notifications
        private const string WindowGuid = "QWorkBreak::SysEventMonitor::63e3c948-0472-4b7b-98ef-3e7c81cd5d08";

        [DllImport("user32.dll", CharSet=CharSet.Auto)]
        private static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        private static extern int SendMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);

        private IntPtr SysEventMonitor_hWnd;
        
        Program() {
            //make form invisible
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.ShowInTaskbar = false;
            this.Load += new EventHandler(onFormLoad);

            //find target window
            SysEventMonitor_hWnd = FindWindow(null, WindowGuid);
            if (SysEventMonitor_hWnd.ToInt32() == 0) {
                throw new SystemException();
            }
            
            //subscribe to sys events
            SystemEvents.SessionSwitch += this.onSessionSwitch;
            SystemEvents.PowerModeChanged += this.onPowerModeChanged;
        }

        ~Program() {
            //unsubscibe from sys events
            try {
                SystemEvents.SessionSwitch -= this.onSessionSwitch;
                SystemEvents.PowerModeChanged -= this.onPowerModeChanged;
            } catch (Exception ex) {
                // ignore exception
            }
        }

        void onFormLoad(object sender, EventArgs e) {
            this.Size = new System.Drawing.Size(0, 0);
        }

        private void onSessionSwitch(object sender, SessionSwitchEventArgs e) {
            if (e.Reason == SessionSwitchReason.SessionLock) {
                SendMessage(SysEventMonitor_hWnd, (int)SysEventMessages.DesktopLock, 0, 0);
            } else if (e.Reason == SessionSwitchReason.SessionUnlock) {
                SendMessage(SysEventMonitor_hWnd, (int)SysEventMessages.DesktopUnlock, 0, 0);
            } 
        }

        private void onPowerModeChanged(object sender, PowerModeChangedEventArgs e) {
            if (e.Mode == PowerModes.Suspend) {
                SendMessage(SysEventMonitor_hWnd, (int)SysEventMessages.PowerModeSusped, 0, 0);
            } else if (e.Mode == PowerModes.Resume) {
                SendMessage(SysEventMonitor_hWnd, (int)SysEventMessages.PowerModeResume, 0, 0);
            }
        }

        [STAThread]
        static void Main(string[] args) {
            try {
                Application.Run(new Program());
            } catch (Exception ex) {
                MessageBox.Show("Something went wrong, sorry :-(", "SysEventNotificationProvider", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}