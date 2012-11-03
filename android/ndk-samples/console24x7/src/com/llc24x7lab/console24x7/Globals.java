package com.llc24x7lab.console24x7;

import android.content.Context;

public class Globals {

    // external intent for alarms
    public static final String APK_UPDATE = "com.llc24x7lab.console24x7.intent.action.APK_UPDATE";
    public static final String AUTO_START = "com.llc24x7lab.console24x7.intent.action.AUTO_START";

    // external intent for starting display24x7
    public static final String MAIN = "com.llc24x7lab.console24x7.intent.action.MAIN";

    // internal intent for ftp status
    public static final String FTP_STARTED = "com.llc24x7lab.console24x7.FTPServer.intent.action.STARTED";
    public static final String FTP_STOPPED = "com.llc24x7lab.console24x7.FTPServer.intent.action.STOPPED";
    public static final String FTP_FAILEDTOSTART = "com.llc24x7lab.console24x7.FTPServer.intent.action.FAILEDTOSTART";
    public static final String FTP_UPDATED = "com.llc24x7lab.console24x7.FTPServer.intent.action.UPDATED";

    private static Context context = null;
    private static boolean interactiveMode = false;

    private Globals() {
        // suppress default constructor for non-instantiation
        throw new AssertionError();
    }

    // ----------------------------------------------------------------
    // properties
    // ----------------------------------------------------------------
    public static Context getContext() {
        return context;
    }

    public static void setContext(Context ctx) {
        if (ctx != null) {
            context = ctx;
        }
    }

    public static boolean getInteractiveMode() {
        return interactiveMode;
    }

    public static void setInteractiveMode(boolean mode) {
        interactiveMode = mode;
    }
}