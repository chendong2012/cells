package com.debug.sensordebug.fragments;



import com.debug.sensordebug.*;
import com.debug.sensordebug.sensor.SensorOps;

import com.actionbarsherlock.app.SherlockFragment;

import android.app.Dialog;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class FragmentSensorInfo extends SherlockFragment {
	final String ALS_FILE_PATH = "/sys/devices/platform/als_ps/driver/als";
	final String PS_FILE_PATH = "/sys/devices/platform/als_ps/driver/ps";
	private String fileDatas;
	final String TAGS = "chendong";
	final int mDeviceDatsUpdateCycle = 500;
	
	TextView mALS_Value_TextView;
	TextView mPS_Value_TextView;
	
    private boolean stopQuery;

    private Handler handler = new Handler() ;
	SensorOps mSensorOps;
	void showToast(CharSequence msg) {
		Toast.makeText(getActivity(), msg, Toast.LENGTH_SHORT).show();
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		final View view = inflater.inflate(R.layout.flagment_sensorinfo,
				container, false);

		
		
		mALS_Value_TextView = (TextView)view.findViewById(R.id.mALS_Value_TextView);
		mPS_Value_TextView = (TextView)view.findViewById(R.id.mPS_Value_TextView);
		View.OnClickListener clickListener = new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub

				switch (v.getId()) {
				}
			}
		};
		
		mSensorOps = new SensorOps(getActivity());
		mSensorOps.init();
		mSensorOps.register();
	//	handler.postDelayed(runnable, mDeviceDatsUpdateCycle);

		return view;
	}

	Runnable runnable = new Runnable() {
		@Override
		public void run() {
			try {
				fileDatas = ToolsFileOps.getFileCxt(ALS_FILE_PATH);
				mALS_Value_TextView.setText(fileDatas);
				
				fileDatas = ToolsFileOps.getFileCxt(PS_FILE_PATH);
				mPS_Value_TextView.setText(fileDatas);
			}// try end
			catch (Exception e) {
				// TODO: handle exception
			}
			
			handler.postDelayed(this, mDeviceDatsUpdateCycle);
		}// run end
	};

	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		Log.e(TAGS, "onDestroy");
		mSensorOps.unregister();
		super.onDestroy();
	}

	@Override
	public void onPause() {
		// TODO Auto-generated method stub


		super.onPause();
		
		Log.e(TAGS, "onPause");
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	//	mSensorOps.register();
		Log.e(TAGS, "onResume");
	}

	@Override
	public void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		Log.e(TAGS, "onStart");
	}

	@Override
	public void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		Log.e(TAGS, "onStop");
	}
}
