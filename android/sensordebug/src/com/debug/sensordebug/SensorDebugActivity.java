package com.debug.sensordebug;


import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class SensorDebugActivity extends Activity {

	Button mRegisterSaveButton;
	Button mALSSetButton;
	Button mPSSetButton;	

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_sensor_debug);
		//my code
		mRegisterSaveButton = (Button)findViewById(R.id.mRegisterSaveButton);
		mALSSetButton = (Button)findViewById(R.id.mALSSetButton);
		mPSSetButton = (Button)findViewById(R.id.mPSSetButton);

		//save button trigger
		mRegisterSaveButton.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {

					}
				});
		
		//als set button trigger
		mALSSetButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
			    Intent intent=new Intent();	
			    intent.setClass(SensorDebugActivity.this, ALSActivity.class);
			    startActivity(intent);
			}
		});
		
		//ps set button trigger
		mPSSetButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
			    Intent intent=new Intent();	
			    intent.setClass(SensorDebugActivity.this, PSActivity.class);
			    startActivity(intent);
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.sensor_debug, menu);
		return true;
	}

}
