package com.debug.sensordebug.fragments;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.SpinnerAdapter;
import android.widget.TextView;
import android.widget.Toast;

import com.actionbarsherlock.app.SherlockFragment;
import com.debug.sensordebug.R;
import com.debug.sensordebug.ToolsFileOps;
import com.debug.sensordebug.sensor.SensorOps;

public class FragmentSensorInfo extends SherlockFragment {
	final String ALS_FILE_PATH = "/sys/devices/platform/als_ps/driver/als";
	final String PS_FILE_PATH = "/sys/devices/platform/als_ps/driver/ps";

	final String TAGS = "chendong";
	final int UPDATE_CYCLE = 1000;
	final boolean isdebug = false;

	TextView mALS_Value_TextView;
	TextView mPS_Value_TextView;
	
	Spinner mSensorRegister;
	
	private boolean stopQuery;
	private Handler handler;
	private Runnable runnable;

	private boolean mIsDestroyView = false;
	SensorOps mSensorOps;

	void showToast(CharSequence msg) {
		Toast.makeText(getActivity(), msg, Toast.LENGTH_SHORT).show();
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		final View view = inflater.inflate(R.layout.flagment_sensorinfo,
				container, false);
		mIsDestroyView = false;
		if(isdebug)
		showToast("onCreateView");
		handler = new Handler() {
			@Override
			public void handleMessage(Message msg) {
				super.handleMessage(msg);
				String[] a;
				a = (String[]) msg.obj;

				mALS_Value_TextView.setText(a[0]);
				mPS_Value_TextView.setText(a[1]);
			}
		};

		mALS_Value_TextView = (TextView) view
				.findViewById(R.id.mALS_Value_TextView);
		mPS_Value_TextView = (TextView) view
				.findViewById(R.id.mPS_Value_TextView);
		
/*spinner start*/
	  	class SpinnerSelectedListener_mALS_Gain implements Spinner.OnItemSelectedListener {  
		    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
		            long arg3) {
		    	//showToast(String.format("mALS_Gain:%d,%d,%d", arg1.getId(), arg2,arg3));
		    }  
		    public void onNothingSelected(AdapterView<?> arg0) {  
		    }  
		}				
		mSensorRegister = (Spinner) view.findViewById(R.id.mSensorRegister);
      ArrayAdapter<String> adapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, ToolsFileOps.data128to158_table);
      adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
      mSensorRegister.setAdapter(adapter);
      mSensorRegister.setOnItemSelectedListener(new SpinnerSelectedListener_mALS_Gain());
/*spinner end*/
      
		View.OnClickListener clickListener = new View.OnClickListener() {
			@Override
			public void onClick(View v) {
			}
		};

		runnable = new Runnable() {
			public void run() {
				try {
					String fileDatas = ToolsFileOps.getFileCxt(ALS_FILE_PATH);

					String fileDatas1 = ToolsFileOps.getFileCxt(PS_FILE_PATH);
					
					String[] ddd = { fileDatas, fileDatas1 };

					Message msg = new Message();
					msg.what = 1;
					msg.obj = ddd;
					handler.sendMessage(msg);
					
				}// try end
				catch (Exception e) {
					// TODO: handle exception
				}
				handler.postDelayed(this, UPDATE_CYCLE);
			}// run end
		};

		handler.postDelayed(runnable, UPDATE_CYCLE);
		return view;
	}

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
		if(isdebug)
		showToast("onPause");
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();

		if(isdebug)
		showToast("info:onResume");
	}

	@Override
	public void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		if(isdebug)
		showToast("onStart");
	}

	@Override
	public void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		if(isdebug)
			
		showToast("onStop");
	}

	@Override
	public void onDestroyView() {
		// TODO Auto-generated method stub
		super.onDestroyView();
		if(isdebug)
		showToast("onDestroyView");
		
		mIsDestroyView = true;
	}

	@Override
	public void onHiddenChanged(boolean hidden) {
		// TODO Auto-generated method stub
		super.onHiddenChanged(hidden);
		if(isdebug)
		showToast("onHiddenChanged");
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		if(isdebug)
		showToast("onCreate");
		mSensorOps = new SensorOps(getActivity());
		mSensorOps.init();
		mSensorOps.register();
	}

	@Override
	public void onAttach(Activity activity) {
		// TODO Auto-generated method stub
		super.onAttach(activity);
		
		showToast("onAttach");
	}

	@Override
	public void onDetach() {
		// TODO Auto-generated method stub
		super.onDetach();
		
		showToast("onDetach");
	}
}
