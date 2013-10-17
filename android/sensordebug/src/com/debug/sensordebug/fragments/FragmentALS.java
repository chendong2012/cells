package com.debug.sensordebug.fragments;

import java.io.IOException;

import com.debug.sensordebug.*;
import com.debug.sensordebug.ALSActivity.SpinnerSelectedListener;

import com.actionbarsherlock.app.SherlockFragment;

import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.SpinnerAdapter;
import android.widget.Toast;
import android.widget.AdapterView.OnItemSelectedListener;

public class FragmentALS extends SherlockFragment {
	
	//echo 30 > /sys/devices/platform/als_ps/driver/als_gain
    final String path_als_gain="/sys/devices/platform/als_ps/driver/als_gain";
    final String path_als_persist="/sys/devices/platform/als_ps/driver/als_persist";    
    final String path_als_time="/sys/devices/platform/als_ps/driver/als_integrationtime";
    final String path_als_rate="/sys/devices/platform/als_ps/driver/als_measrepeatrate";
//    final String path_als_low="/sys/devices/platform/als_ps/driver/ex_als_low";
 //   final String path_als_high="/sys/devices/platform/als_ps/driver/ex_als_high";
    
	/* spinner */
	Spinner mALS_Gain;	
	Spinner mALS_Persist;
	Spinner mALS_Time;	
	Spinner mALS_Rate;	

	
	int mALS_Gain_Index;
	int mALS_Persist_Index;
	int mALS_Time_Index;
	int mALS_Rate_Index;

	String mGain_String;
	String mPersist_String;
	String mTime_String;
	String mRate_String;
	/* spinner end */

	EditText mALSLow_Value;
	EditText mALSHigh_Value;

	Button mALSSaveButton;

	void showToast(CharSequence msg) {
		Toast.makeText(getActivity(), msg, Toast.LENGTH_SHORT).show();
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		final View view = inflater.inflate(R.layout.flagment_alsconfig,
				container, false);

		class SpinnerSelectedListener_mALS_Gain implements
				Spinner.OnItemSelectedListener {
			public void onItemSelected(AdapterView<?> arg0, View arg1,
					int arg2, long arg3) {
				// showToast(String.format("mALS_Gain:%d,%d,%d", arg1.getId(),
				// arg2,arg3));
				mALS_Gain_Index = arg2;
			}

			public void onNothingSelected(AdapterView<?> arg0) {
			}
		}

		class SpinnerSelectedListener_mALS_Persist implements
				Spinner.OnItemSelectedListener {
			public void onItemSelected(AdapterView<?> arg0, View arg1,
					int arg2, long arg3) {
				// showToast(String.format("mALS_Persist:%d,%d,%d",
				// arg1.getId(), arg2,arg3));
				mALS_Persist_Index = arg2;
			}

			public void onNothingSelected(AdapterView<?> arg0) {
			}
		}

		class SpinnerSelectedListener_mALS_Time implements
				Spinner.OnItemSelectedListener {
			public void onItemSelected(AdapterView<?> arg0, View arg1,
					int arg2, long arg3) {
				// showToast(String.format("mALS_Time:%d,%d,%d", arg1.getId(),
				// arg2,arg3));
				mALS_Time_Index = arg2; 
			}

			public void onNothingSelected(AdapterView<?> arg0) {
			}
		}

		class SpinnerSelectedListener_mALS_Rate implements
				Spinner.OnItemSelectedListener {
			public void onItemSelected(AdapterView<?> arg0, View arg1,
					int arg2, long arg3) {
				// showToast(String.format("mALS_Rate:%d,%d,%d", arg1.getId(),
				// arg2,arg3));
				mALS_Rate_Index = arg2;
			}

			public void onNothingSelected(AdapterView<?> arg0) {
			}
		}

		View.OnClickListener clickListener = new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub

				switch (v.getId()) {
				}
			}
		};

		mALS_Gain = (Spinner) view.findViewById(R.id.mALS_Gain);
		mALS_Persist = (Spinner) view.findViewById(R.id.mALS_Persist);

		mALS_Time = (Spinner) view.findViewById(R.id.mALS_Time);
		mALS_Rate = (Spinner) view.findViewById(R.id.mALS_Rate);

		
		mALSSaveButton = (Button) view.findViewById(R.id.mALSSaveButton);
		mALSSaveButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				MyAsyncTask MyAsyncTask = new MyAsyncTask();
				MyAsyncTask.execute();
			}
		});

		ArrayAdapter<String> mALS_GainAdapter = new ArrayAdapter<String>(getActivity(),
				android.R.layout.simple_spinner_item,
				ToolsFileOps.als_gain_table);

		ArrayAdapter<String> mALS_PersistAdapter = new ArrayAdapter<String>(getActivity(),
				android.R.layout.simple_spinner_item,
				ToolsFileOps.als_persist_table);
		
		ArrayAdapter<String> mALS_TimeAdapter = new ArrayAdapter<String>(getActivity(),
				android.R.layout.simple_spinner_item,
				ToolsFileOps.als_integration_time_table);
		
		ArrayAdapter<String> mALS_RateAdapter = new ArrayAdapter<String>(getActivity(),
				android.R.layout.simple_spinner_item,
				ToolsFileOps.als_meas_repeat_rate_table);
		
		mALS_GainAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		mALS_PersistAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		mALS_TimeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		mALS_RateAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		
		mALS_Gain.setAdapter(mALS_GainAdapter);
		mALS_Persist.setAdapter(mALS_PersistAdapter);
		mALS_Time.setAdapter(mALS_TimeAdapter);
		mALS_Rate.setAdapter(mALS_RateAdapter);
		
		
//		mALS_Gain.setSelection(3);
		
		
		
		// 添加事件Spinner事件监听
		mALS_Gain
				.setOnItemSelectedListener(new SpinnerSelectedListener_mALS_Gain());
		mALS_Persist
				.setOnItemSelectedListener(new SpinnerSelectedListener_mALS_Persist());
		mALS_Time
				.setOnItemSelectedListener(new SpinnerSelectedListener_mALS_Time());
		mALS_Rate
				.setOnItemSelectedListener(new SpinnerSelectedListener_mALS_Rate());
		// 设置默认值
		mALS_Gain.setVisibility(View.VISIBLE);
		mALS_Persist.setVisibility(View.VISIBLE);
		mALS_Time.setVisibility(View.VISIBLE);
		mALS_Rate.setVisibility(View.VISIBLE);

		return view;
	}

	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}

	@Override
	public void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub

		super.onResume();
	}

	@Override
	public void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
	}

	@Override
	public void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
	}
/*-------------------------------async task-------------------------------*/
	private class MyAsyncTask extends AsyncTask<Void, Void, String> {
		ProgressDialog progressDialog;
		@Override
		protected void onPreExecute() {
			
			progressDialog = new ProgressDialog(getActivity());
			progressDialog.setIndeterminate(true);
			progressDialog.setCancelable(true);
			progressDialog.setTitle("Save");
			progressDialog.setMessage("Saving...");
			progressDialog.show();
			progressDialog.setOnCancelListener(new OnCancelListener() {
				@Override
				public void onCancel(DialogInterface dialog) {
					// cancel(true);
				}
			});
		}

		@Override
		protected String doInBackground(Void... params) {



			try {
				
				ToolsFileOps.setFileCxt(path_als_gain, ToolsFileOps.als_gain_table[mALS_Gain_Index]);
				ToolsFileOps.setFileCxt(path_als_persist, ToolsFileOps.als_persist_table[mALS_Persist_Index]);
				ToolsFileOps.setFileCxt(path_als_time, ToolsFileOps.als_integration_time_table[mALS_Time_Index]);
				ToolsFileOps.setFileCxt(path_als_rate, ToolsFileOps.als_meas_repeat_rate_table[mALS_Rate_Index]);
			//	ToolsFileOps.setFileCxt(path_als_low, );
				//ToolsFileOps.setFileCxt(path_als_high, ");
				
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			return "ok";
		}

		@Override
		protected void onCancelled() {
		}

		@Override
		protected void onPostExecute(String result) {
			progressDialog.cancel();
			showToast("save ok");
		}
	}
	/*---------------------async task end-------------*/
}
