package com.chendong.debugtools;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class DebugTools extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_debug_tools);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.debug_tools, menu);
        return true;
    }
    
}
