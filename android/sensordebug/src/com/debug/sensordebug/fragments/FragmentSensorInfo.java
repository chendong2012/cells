package com.debug.sensordebug.fragments;

import java.io.IOException;

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
import android.widget.Button;
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
	final String LOW_FILE_PATH = "/sys/devices/platform/als_ps/driver/proximity_low";
	final String HIGH_FILE_PATH = "/sys/devices/platform/als_ps/driver/proximity_high";
	final String OPSREG_FILE_PATH = "/sys/devices/platform/als_ps/driver/opsreg";


	final String TAGS = "chendong";
	final int UPDATE_CYCLE = 1000;
	final boolean isdebug = false;

	TextView mALS_Value_TextView;
	TextView mPS_Value_TextView;

	TextView mLOW_Value_TextView;
	TextView mHIGH_Value_TextView;
	
	TextView mReg_Value_TextView;	
	
	Button mRegisterSaveButton;
	
	Spinner mSensorRegister;
	int mSensorRegister_index;	
	
	private boolean stopQuery;
	private Handler handler;
	private Handler handler1;
	private Runnable runnable;

	private boolean mIsDestroyView = false;
	SensorOps mSensorOps;

	void showToast(CharSequence msg) {
		if (isdebug)
		Toast.makeText(getActivity(), msg, Toast.LENGTH_SHORT).show();
	}

	void showToast1(CharSequence msg) {
		Toast.makeText(getActivity(), msg, Toast.LENGTH_SHORT).show();
	}
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		final View view = inflater.inflate(R.layout.flagment_sensorinfo,
				container, false);
		mIsDestroyView = false;
		//Log.e(TAGS, "global_switch--oncreate");
		showToast("onCreateView");
//		ToolsFileOps.global_switch = 0;
		handler = new Handler() {
			@Override
			public void handleMessage(Message msg) {
				super.handleMessage(msg);
				String[] a;
				a = (String[]) msg.obj;

				mALS_Value_TextView.setText(a[0]);
				mPS_Value_TextView.setText(a[1]);
				mLOW_Value_TextView.setText(a[2]);
				mHIGH_Value_TextView.setText(a[3]);
			}
		};

		handler1 = new Handler() {
			@Override
			public void handleMessage(Message msg) {
				super.handleMessage(msg);
				String a;
				a = (String) msg.obj;
				mALS_Value_TextView.setText(a);
				
			}
		};


		mALS_Value_TextView = (TextView) view
				.findViewById(R.id.mALS_Value_TextView);
		mPS_Value_TextView = (TextView) view
				.findViewById(R.id.mPS_Value_TextView);

		mLOW_Value_TextView = (TextView) view
				.findViewById(R.id.mLOW_Value_TextView);
		mHIGH_Value_TextView = (TextView) view
				.findViewById(R.id.mHIGH_Value_TextView);
		
		mReg_Value_TextView = (TextView) view
				.findViewById(R.id.mReg_Value_TextView);		
		
/*spinner start*/
	  	class SpinnerSelectedListener_mSensorRegister implements Spinner.OnItemSelectedListener {  
		    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
		            long arg3) {
		    	//showToast(String.format("mALS_Gain:%d,%d,%d", arg1.getId(), arg2,arg3));
		    	mSensorRegister_index = arg2;	
		    }  
		    public void onNothingSelected(AdapterView<?> arg0) {  
		    }  
		}		

		mSensorRegister = (Spinner) view.findViewById(R.id.mSensorRegister);
      ArrayAdapter<String> adapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, ToolsFileOps.data128to158_table2);
      adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
      mSensorRegister.setAdapter(adapter);
      mSensorRegister.setOnItemSelectedListener(new SpinnerSelectedListener_mSensorRegister());
/*spinner end*/

		View.OnClickListener clickListener = new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				try {
					ToolsFileOps.setFileCxt(OPSREG_FILE_PATH, ToolsFileOps.data128to158_table[mSensorRegister_index]);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				String fileDatas4;
				try {
					fileDatas4 = ToolsFileOps.getFileCxt(OPSREG_FILE_PATH);
					mReg_Value_TextView.setText(fileDatas4);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		};

		mRegisterSaveButton = (Button) view.findViewById(R.id.mRegisterSaveButton);
		
		mRegisterSaveButton.setOnClickListener(clickListener);
		
		runnable = new Runnable() {
			public void run() {
				try {
					if (ToolsFileOps.global_switch == 0) {
						//showToast1(".global_switch == 0");
						//Log.e(TAGS, "global_switch");
					String fileDatas = ToolsFileOps.getFileCxt(ALS_FILE_PATH);
					String fileDatas1 = ToolsFileOps.getFileCxt(PS_FILE_PATH);
					String fileDatas2 = ToolsFileOps.getFileCxt(LOW_FILE_PATH);
					String fileDatas3 = ToolsFileOps.getFileCxt(HIGH_FILE_PATH);
					
					String[] ddd = { fileDatas, fileDatas1, fileDatas2, fileDatas3 };

					Message msg = new Message();
					msg.what = 1;
					msg.obj = ddd;
					handler.sendMessage(msg);
					}

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

		showToast("onPause");
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();


		showToast("info:onResume");
	}

	@Override
	public void onStart() {
		// TODO Auto-generated method stub
		super.onStart();

		showToast("onStart");
	}

	@Override
	public void onStop() {
		// TODO Auto-generated method stub
		super.onStop();

		showToast("onStop");
	}

	@Override
	public void onDestroyView() {
		// TODO Auto-generated method stub
		super.onDestroyView();

		showToast("onDestroyView");
		
		mIsDestroyView = true;
	}

	@Override
	public void onHiddenChanged(boolean hidden) {
		// TODO Auto-generated method stub
		super.onHiddenChanged(hidden);

		showToast("onHiddenChanged");
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);

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
