package com.llc24x7lab.console24x7.vncserver;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import android.app.Service;
import android.content.ClipData;
import android.content.Intent;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.content.ClipboardManager;
import android.util.Log;
import android.widget.Toast;

public class VNCServerService extends Service {

    // ------------------------
    // ipc socket listener
    // ------------------------
    public class IPCSocketListener extends Thread {
        DatagramSocket server = null;
        boolean finished = false;

        public void finishThread() {
            finished = true;
        }

        @Override
        public void run() {
            try {
                server = new DatagramSocket(MyUtil.getIPCSocketListenPort());
                log("Listening...");
                while (!finished) {
                    DatagramPacket answer = new DatagramPacket(new byte[1024], 1024);
                    server.receive(answer);
                    String resp = new String(answer.getData());
                    resp = resp.substring(0, answer.getLength());
                    log("RECEIVED " + resp);
                    if (resp.length() > 5 && resp.substring(0, 6).equals("~CLIP|")) {
                        resp = resp.substring(7, resp.length() - 1);
                        ClipboardManager clipboard = (ClipboardManager) getSystemService(CLIPBOARD_SERVICE);
                        ClipData cd = ClipData.newPlainText(resp.toString(), resp.toString());
                        clipboard.setPrimaryClip(cd);
                    } else if (resp.length() > 5 && resp.substring(0, 6).equals("~SHOW|")) {
                        resp = resp.substring(6, resp.length() - 1);
                        showTextOnScreen(resp);
                    } else if (resp.length() > 14 && resp.substring(0, 15).equals("~SERVERSTARTED|")) {
                        resp = resp.substring(15, resp.length() - 1);
                        showTextOnScreen(resp);
                    } else if (resp.length() > 14 && resp.substring(0, 15).equals("~SERVERSTOPPED|")) {
                        resp = resp.substring(15, resp.length() - 1);
                        showTextOnScreen(resp);
                    } else if (resp.length() > 10 && resp.substring(0, 11).equals("~CONNECTED|")) {
                        resp = resp.substring(11, resp.length() - 1);
                        showTextOnScreen(resp);
                    } else if (resp.length() > 13 && resp.substring(0, 14).equals("~DISCONNECTED|")) {
                        resp = resp.substring(14, resp.length() - 1);
                        showTextOnScreen(resp);
                    } else {
                        log("Received: " + resp);
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Class used for the client Binder. Because we know this service always runs in the same process as its clients, we don't need to deal with IPC.
     */
    public class VNCServerBinder extends Binder {
        public VNCServerService getService() {
            // return this instance of VNCServerService so clients can call public methods
            return VNCServerService.this;
        }
    }

    private static final String VNC_LOG = "VNCserver";
    private IPCSocketListener ipcSocketListener = null;
    private final IBinder vncBinder = new VNCServerBinder(); // binder given to clients
    private Handler handler;

    // return the binder class upon a call of bindService
    @Override
    public IBinder onBind(Intent intent) {
        return vncBinder;
    }

    @Override
    public void onCreate() {
        super.onCreate();

        handler = new Handler(Looper.getMainLooper());
        ipcSocketListener = new IPCSocketListener();
        ipcSocketListener.start();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        stopServer();
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    // ------------------------
    // public API
    // ------------------------
    public void startServer() {
        try {
            Process sh;
            String files_dir = getFilesDir().getAbsolutePath();

            String password = MyUtil.getVncPassWord();
            String password_opt = "-p " + password;

            String vncport = MyUtil.getVncPortNum();
            String vncport_opt = "-P " + vncport;

            String reverse_host = MyUtil.getVncReverseHost();
            String reverse_port = MyUtil.getVncReversePort();
            String reverse_opt = "-R " + reverse_host + ":" + reverse_port;

            String display_method = MyUtil.getVncDisplayMethod();
            String display_method_opt = "-m " + display_method;

            String rotation_opt = "-r 0";
            String scaling_opt = "-s 100";
            String display_zte_opt = "";

            // our exec file is disguised as a library so it will get packed to lib folder according to cpu_abi
            String vnc_exec = getFilesDir().getParent() + "/lib/libandroidvncserver.so";
            File f = new File(vnc_exec);
            if (!f.exists()) {
                String e = "Error! Could not find daemon file, " + vnc_exec;
                showTextOnScreen(e);
                log(e);
                return;
            }

            String permission_string = "chmod 777 " + vnc_exec;
            String vnc_exec_string = vnc_exec + " " + password_opt + " " + vncport_opt + " " + reverse_opt + " " + display_method_opt + " " + rotation_opt
                    + " " + scaling_opt + " " + display_zte_opt;
            String vnc_exec_string_nopassword = vnc_exec + " " + vncport_opt + " " + reverse_opt + " " + display_method_opt + " " + rotation_opt + " "
                    + scaling_opt + " " + display_zte_opt;

            Runtime.getRuntime().exec(permission_string);
            boolean root = true;
            if (root) {
                log("Running as root...");
                sh = Runtime.getRuntime().exec("su", null, new File(files_dir));
                OutputStream os = sh.getOutputStream();
                writeCommand(os, permission_string);
                writeCommand(os, vnc_exec_string_nopassword);
            } else {
                log("Not running as root...");
                Runtime.getRuntime().exec(permission_string);
                Runtime.getRuntime().exec(vnc_exec_string, null, new File(files_dir));
            }
            log("Starting " + vnc_exec_string_nopassword);
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void stopServer() {
        try {
            DatagramSocket clientSocket = new DatagramSocket();
            InetAddress addr = InetAddress.getLocalHost();
            String toSend = "~KILL|";
            byte[] buffer = toSend.getBytes();
            DatagramPacket question = new DatagramPacket(buffer, buffer.length, addr, MyUtil.getIPCSocketSendPort());
            clientSocket.send(question);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void doReverseConnection() {
        try {
            if (isServerRunning()) {
                DatagramSocket clientSocket = new DatagramSocket();
                InetAddress addr = InetAddress.getLocalHost();
                String toSend = "~REVERSECONN|";
                byte[] buffer = toSend.getBytes();
                DatagramPacket question = new DatagramPacket(buffer, buffer.length, addr, MyUtil.getIPCSocketSendPort());
                clientSocket.send(question);
            }
        } catch (SocketException e) {
            e.printStackTrace();
        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static boolean isServerRunning() {
        try {
            byte[] receiveData = new byte[1024];
            DatagramSocket clientSocket = new DatagramSocket();
            InetAddress addr = InetAddress.getLocalHost();

            clientSocket.setSoTimeout(200);
            String toSend = "~PING|";
            byte[] buffer = toSend.getBytes();
            DatagramPacket question = new DatagramPacket(buffer, buffer.length, addr, MyUtil.getIPCSocketSendPort());
            clientSocket.send(question);

            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            clientSocket.receive(receivePacket);
            String receivedString = new String(receivePacket.getData());
            receivedString = receivedString.substring(0, receivePacket.getLength());
            return receivedString.equals("~PONG|");
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    // ------------------------
    // private methods
    // ------------------------
    private static void writeCommand(OutputStream os, String command) throws Exception {
        os.write((command + "\n").getBytes("ASCII"));
    }

    private void showTextOnScreen(final String t) {
        handler.post(new Runnable() {
            public void run() {
                Toast.makeText(getApplicationContext(), t, Toast.LENGTH_LONG).show();
            }
        });
    }

    private void log(String s) {
        Log.v(VNC_LOG, s);
    }
}