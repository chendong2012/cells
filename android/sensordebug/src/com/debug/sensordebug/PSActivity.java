package com.debug.sensordebug;

import java.io.IOException;

import com.debug.sensordebug.ALSActivity.SpinnerSelectedListener;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

public class PSActivity extends Activity {
	String REG_FILE_PATH = "/sys/devices/platform/als_ps/driver/reg";

	private ArrayAdapter<String> adapter;  
	/*spinner*/
	Spinner mPS_Freq;
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
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.flagment_psconfig);

        mPS_Freq = (Spinner) findViewById(R.id.mPS_Freq);
        mPS_Persist = (Spinner) findViewById(R.id.mALS_Rate);
        mPS_Time = (Spinner) findViewById(R.id.mALS_Time);
        mPS_Cycle = (Spinner) findViewById(R.id.mPS_Cycle);
        mPS_Current = (Spinner) findViewById(R.id.mPS_Current);
        mPS_PulseCount = (Spinner) findViewById(R.id.mALS_Persist);

 
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, ToolsFileOps.data0to255_table);         
        
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mPS_Freq.setAdapter(adapter);
        mPS_Persist.setAdapter(adapter);
        mPS_Time.setAdapter(adapter);
        mPS_Cycle.setAdapter(adapter);        
        mPS_Current.setAdapter(adapter);
        mPS_PulseCount.setAdapter(adapter);               
        
        //添加事件Spinner事件监听    
        mPS_Freq.setOnItemSelectedListener(new SpinnerSelectedListener());
        mPS_Persist.setOnItemSelectedListener(new SpinnerSelectedListener());
        mPS_Time.setOnItemSelectedListener(new SpinnerSelectedListener());
        mPS_Cycle.setOnItemSelectedListener(new SpinnerSelectedListener());       
        mPS_Current.setOnItemSelectedListener(new SpinnerSelectedListener());
        mPS_PulseCount.setOnItemSelectedListener(new SpinnerSelectedListener());    
        
        //设置默认值  
        mPS_Freq.setVisibility(View.VISIBLE);  
        mPS_Persist.setVisibility(View.VISIBLE);
        mPS_Time.setVisibility(View.VISIBLE);  
        mPS_Cycle.setVisibility(View.VISIBLE);        
        mPS_Current.setVisibility(View.VISIBLE);  
        mPS_PulseCount.setVisibility(View.VISIBLE);    

              
        
        mPSSaveButton = (Button) findViewById(R.id.mRegisterSaveButton);        
        mPSSaveButton.setOnClickListener(
        		new OnClickListener() {
					
					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						//发送数据到设备
					}
				}
        		);
        
    }

	class SpinnerSelectedListener implements OnItemSelectedListener {  
	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
	            long arg3) {
	    }  
	    public void onNothingSelected(AdapterView<?> arg0) {  
	    }  
	}
	
}





