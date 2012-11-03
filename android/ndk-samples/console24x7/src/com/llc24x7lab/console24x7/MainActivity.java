package com.llc24x7lab.console24x7;

import android.os.Bundle;
import android.os.IBinder;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import com.llc24x7lab.console24x7.vncserver.VNCServerService;
import com.llc24x7lab.console24x7.vncserver.VNCServerService.VNCServerBinder;

public class MainActivity extends Activity {

    private TextView textViewStatus;
    private Button buttonConfigure;
    private Button buttonCheckUpdate;
    private Button buttonCheckAlive;
    private Button buttonStartFTP;
    private Button buttonVNCRevConn;
    private Button buttonQuit;

    private VNCServerService vnc;
    private boolean vncBound = false;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        textViewStatus = (TextView) findViewById(R.id.textView1);
        buttonConfigure = (Button) findViewById(R.id.button1);
        buttonCheckUpdate = (Button) findViewById(R.id.button2);
        buttonCheckAlive = (Button) findViewById(R.id.button3);
        buttonStartFTP = (Button) findViewById(R.id.button4);
        buttonVNCRevConn = (Button) findViewById(R.id.button5);
        buttonQuit = (Button) findViewById(R.id.button6);
        buttonConfigure.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent i = new Intent(MainActivity.this, SettingsPrefActivity.class);
                startActivity(i);
            }
        });
        buttonCheckUpdate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            }
        });
        buttonCheckAlive.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            }
        });
        buttonStartFTP.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            }
        });
        buttonVNCRevConn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (vncBound) {
                    vnc.startServer();
                    vnc.doReverseConnection();
                }
            }
        });
        buttonQuit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
        textViewStatus.setText("Console24x7, Ver=1.0, copyright(c) 24x7Lab LLC");

        // set context
        Context ctx = getApplicationContext();
        Globals.setContext(ctx);

        // bind vnc service
        Intent intent = new Intent(this, VNCServerService.class);
        bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        // unbind vnc service
        if (vncBound) {
            unbindService(mConnection);
            vncBound = false;
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }

    // -----------------------------------------------------
    // defines callbacks for service binding, passed to bindService()
    // -----------------------------------------------------
    private ServiceConnection mConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName className, IBinder service) {
            // We've bound to VNCServerService, cast the IBinder and get VNCServerService instance
            VNCServerBinder binder = (VNCServerBinder) service;
            vnc = binder.getService();
            vncBound = true;
        }

        @Override
        public void onServiceDisconnected(ComponentName arg0) {
            vncBound = false;
        }
    };
}