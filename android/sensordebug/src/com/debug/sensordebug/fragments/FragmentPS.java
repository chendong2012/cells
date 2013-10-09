package com.debug.sensordebug.fragments;
import com.debug.sensordebug.*;


import com.actionbarsherlock.app.SherlockFragment;

import android.app.Dialog;
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
import android.widget.Toast;

public class FragmentPS extends SherlockFragment
{
	String tags="mytest123";
	private ArrayAdapter<String> adapter;  
	/*spinner*/
	Spinner mPS_Freq;
	Spinner mPS_Gain;	
	Spinner mPS_Persist;
	Spinner mPS_Time;
	Spinner mPS_Cycle;
	Spinner mPS_Current;
	Spinner mPS_PulseCount;
	
	int mPS_Freq_Idx;
	int mPS_Persist_Idx;
	int mPS_Time_Idx;
	int mPS_Cycle_Idx;
	int mPS_Current_Idx;
	int mPS_PulseCount_Idx;

	String mPS_Freq_String;
	String mPS_Persist_String;
	String mPS_Time_String;
	String mPS_Cycle_String;
	String mPS_Current_String;
	String mPS_PulseCount_String;

	/*button*/
	Button mPSSaveButton;	
	
	/*edittext*/
	EditText mPSLow_Value;
	EditText mPSHigh_Value;
	
	
	
	
    void showToast(CharSequence msg) {
        Toast.makeText(getActivity(), msg, Toast.LENGTH_SHORT).show();
    }

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) 
	{
		final View view = inflater.inflate(R.layout.flagment_psconfig,container, false);		


    	
		View.OnClickListener clickListener = new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
			//	showToast(String.format("click:%d",v.getId()));
				switch (v.getId()) {
				}
			}
		};		

        mPS_Freq = (Spinner) view.findViewById(R.id.mPS_Freq);
        mPS_Gain = (Spinner) view.findViewById(R.id.mPS_Gain);
        
        mPS_Persist = (Spinner) view.findViewById(R.id.mALS_Rate);
        mPS_Time = (Spinner) view.findViewById(R.id.mALS_Time);
        mPS_Cycle = (Spinner) view.findViewById(R.id.mPS_Cycle);
        mPS_Current = (Spinner) view.findViewById(R.id.mPS_Current);
        mPS_PulseCount = (Spinner) view.findViewById(R.id.mALS_Persist);

 
    	class SpinnerSelectedListener_mPS_Freq implements Spinner.OnItemSelectedListener {  
    	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
    	            long arg3) {
    	    	//showToast(String.format("mPS_Freq:%d,%d,%d", arg1.getId(), arg2,arg3));
    	    	Log.e(tags,"SpinnerSelectedListener_mPS_Freq");
    
    	    }  
    	    public void onNothingSelected(AdapterView<?> arg0) {  
    	    }  
    	}
    	class SpinnerSelectedListener_mPS_Gain implements Spinner.OnItemSelectedListener {  
    	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
    	            long arg3) {
    	    	//showToast(String.format("mPS_Freq:%d,%d,%d", arg1.getId(), arg2,arg3));
    	    	Log.e(tags,"SpinnerSelectedListener_mPS_Freq");
    	    }  
    	    public void onNothingSelected(AdapterView<?> arg0) {  
    	    }  
    	}		
    	class SpinnerSelectedListener_mPS_Persist implements Spinner.OnItemSelectedListener {  
    	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
    	            long arg3) {
//    	    	Log.e(tags,String.format("%d", arg2));
    	    }  
    	    public void onNothingSelected(AdapterView<?> arg0) {  
    	    }  
    	}		
    	class SpinnerSelectedListener_mPS_Time implements Spinner.OnItemSelectedListener {  
    	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
    	            long arg3) {
    	//    	Log.e(tags,"SpinnerSelectedListener_mPS_Time");
    	    }  
    	    public void onNothingSelected(AdapterView<?> arg0) {  
    	    }  
    	}		
    	class SpinnerSelectedListener_mPS_Cycle implements Spinner.OnItemSelectedListener {  
    	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
    	            long arg3) {
    	   // 	Log.e(tags,"SpinnerSelectedListener_mPS_Cycle");
    	    }  
    	    public void onNothingSelected(AdapterView<?> arg0) {  
    	    }  
    	}		
    	class SpinnerSelectedListener_mPS_Current implements Spinner.OnItemSelectedListener {  
    	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
    	            long arg3) {
    	  //  	Log.e(tags,"SpinnerSelectedListener_mPS_Current");
    	    }  
    	    public void onNothingSelected(AdapterView<?> arg0) {  
    	    }  
    	}		
    	class SpinnerSelectedListener_mPS_PulseCount implements Spinner.OnItemSelectedListener {  
    	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
    	            long arg3) {
    	 //   	Log.e(tags,"SpinnerSelectedListener_mPS_PulseCount");
    	    }  
    	    public void onNothingSelected(AdapterView<?> arg0) {  
    	    }  
    	}		

        ArrayAdapter<String> mPS_FreqAdapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, ToolsFileOps.led_pulse_freq_table);   
        
        ArrayAdapter<String> mPS_GainAdapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, ToolsFileOps.ps_gain_table); 
        
        ArrayAdapter<String> mPS_PersistAdapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, ToolsFileOps.ps_persist_table);         

        ArrayAdapter<String> mPS_TimeAdapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, ToolsFileOps.ps_measurement_time_table);         

        ArrayAdapter<String> mPS_CycleAdapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, ToolsFileOps.led_duty_cycle_table);         

        ArrayAdapter<String> mPS_CurrentAdapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, ToolsFileOps.led_driving_peak_count_table);         

        ArrayAdapter<String> mPS_PulseCountAdapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, ToolsFileOps.data0to255_table);         

        mPS_FreqAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mPS_GainAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        mPS_PersistAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mPS_TimeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mPS_CycleAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mPS_CurrentAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mPS_PulseCountAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        

        mPS_Freq.setAdapter(mPS_FreqAdapter);
        mPS_Gain.setAdapter(mPS_GainAdapter);
        mPS_Persist.setAdapter(mPS_PersistAdapter);
        mPS_Time.setAdapter(mPS_TimeAdapter);
        mPS_Cycle.setAdapter(mPS_CycleAdapter);        
        mPS_Current.setAdapter(mPS_CurrentAdapter);
        mPS_PulseCount.setAdapter(mPS_PulseCountAdapter);               
        
        //添加事件Spinner事件监听    
        mPS_Freq.setOnItemSelectedListener(new SpinnerSelectedListener_mPS_Freq());
        mPS_Gain.setOnItemSelectedListener(new SpinnerSelectedListener_mPS_Gain());
        mPS_Persist.setOnItemSelectedListener(new SpinnerSelectedListener_mPS_Persist());
        mPS_Time.setOnItemSelectedListener(new SpinnerSelectedListener_mPS_Time());
        mPS_Cycle.setOnItemSelectedListener(new SpinnerSelectedListener_mPS_Cycle());       
        mPS_Current.setOnItemSelectedListener(new SpinnerSelectedListener_mPS_Current());
        mPS_PulseCount.setOnItemSelectedListener(new SpinnerSelectedListener_mPS_PulseCount());    
        
        //设置默认值  
        mPS_Freq.setVisibility(View.VISIBLE);  
        mPS_Gain.setVisibility(View.VISIBLE);         
        mPS_Persist.setVisibility(View.VISIBLE);
        mPS_Time.setVisibility(View.VISIBLE);  
        mPS_Cycle.setVisibility(View.VISIBLE);        
        mPS_Current.setVisibility(View.VISIBLE);  
        mPS_PulseCount.setVisibility(View.VISIBLE);    

        mPSSaveButton = (Button) view.findViewById(R.id.mPSSaveButton);  		
        mPSSaveButton.setOnClickListener(clickListener);
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
		//showToast("onPause");
		super.onPause();
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub
	//	showToast("ps:resume");
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
}
