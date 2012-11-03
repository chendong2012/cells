package com.llc24x7lab.console24x7.vncserver;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import com.llc24x7lab.console24x7.Globals;

public class MyUtil {

    // suppress default constructor for non-instantiation
    private MyUtil() {
        throw new AssertionError();
    }

    // -------------------------------------------
    // application runtime utilities
    // -------------------------------------------
    public static int getIPCSocketListenPort() {
        return 13131;
    }

    public static int getIPCSocketSendPort() {
        return 13132;
    }

    public static String getVncPassWord() {
        Context ctx = Globals.getContext();
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(ctx);
        String password = settings.getString("vncpassword", "MyPassWord");
        return password;
    }

    public static String getVncPortNum() {
        Context ctx = Globals.getContext();
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(ctx);
        String portnum = settings.getString("vncportnum", "5901");
        return portnum;
    }

    public static String getVncReverseHost() {
        Context ctx = Globals.getContext();
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(ctx);
        String rhost = settings.getString("vncreversehost", "127.0.0.1");
        return rhost;
    }

    public static String getVncReversePort() {
        Context ctx = Globals.getContext();
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(ctx);
        String rport = settings.getString("vncreverseport", "5500");
        return rport;
    }

    public static String getVncDisplayMethod() {
        Context ctx = Globals.getContext();
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(ctx);
        String method = settings.getString("vncmethod", "auto");
        return method;
    }
}