package com.debug.sensordebug.fragments;
import com.debug.sensordebug.*;
import com.debug.sensordebug.ALSActivity.SpinnerSelectedListener;

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
import android.widget.SpinnerAdapter;
import android.widget.Toast;
import android.widget.AdapterView.OnItemSelectedListener;

public class FragmentALS extends SherlockFragment
{
	/*spinner*/
	Spinner mALS_Gain;
	Spinner mALS_Persist;
	Spinner mALS_Time;
	Spinner mALS_Rate;
	
	int mGain_Idx;
	int mPersist_Idx;
	int mTime_Idx;
	int mRate_Idx;
	
	String mGain_String;
	String mPersist_String;
	String mTime_String;
	String mRate_String;	
	/*spinner end*/
	
	EditText mALSLow_Value;
	EditText mALSHigh_Value;
	
	Button mALSSaveButton;	
	
	
    void showToast(CharSequence msg) {
        Toast.makeText(getActivity(), msg, Toast.LENGTH_SHORT).show();
    }

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) 
	{
		final View view = inflater.inflate(R.layout.flagment_alsconfig,container, false);		

    	class SpinnerSelectedListener_mALS_Gain implements Spinner.OnItemSelectedListener {  
    	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
    	            long arg3) {
    	    	//showToast(String.format("mALS_Gain:%d,%d,%d", arg1.getId(), arg2,arg3));
    	    }  
    	    public void onNothingSelected(AdapterView<?> arg0) {  
    	    }  
    	}		

    	class SpinnerSelectedListener_mALS_Persist implements Spinner.OnItemSelectedListener {  
    	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
    	            long arg3) {
    	    	//showToast(String.format("mALS_Persist:%d,%d,%d", arg1.getId(), arg2,arg3));
    	    }  
    	    public void onNothingSelected(AdapterView<?> arg0) {  
    	    }  
    	}		
	
    	class SpinnerSelectedListener_mALS_Time implements Spinner.OnItemSelectedListener {  
    	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
    	            long arg3) {
    	    	//showToast(String.format("mALS_Time:%d,%d,%d", arg1.getId(), arg2,arg3));
    	    }  
    	    public void onNothingSelected(AdapterView<?> arg0) {  
    	    }  
    	}		
    	
    	class SpinnerSelectedListener_mALS_Rate implements Spinner.OnItemSelectedListener {  
    	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
    	            long arg3) {
    	    	//showToast(String.format("mALS_Rate:%d,%d,%d", arg1.getId(), arg2,arg3));
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
         
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, ToolsFileOps.data0to255_table);         
        
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mALS_Gain.setAdapter(adapter);
        mALS_Persist.setAdapter(adapter);
        mALS_Time.setAdapter(adapter);
        mALS_Rate.setAdapter(adapter);        
        
        //添加事件Spinner事件监听    
        mALS_Gain.setOnItemSelectedListener(new SpinnerSelectedListener_mALS_Gain());
        mALS_Persist.setOnItemSelectedListener(new SpinnerSelectedListener_mALS_Persist());
        mALS_Time.setOnItemSelectedListener(new SpinnerSelectedListener_mALS_Time());
        mALS_Rate.setOnItemSelectedListener(new SpinnerSelectedListener_mALS_Rate());        
        //设置默认值  
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
}
