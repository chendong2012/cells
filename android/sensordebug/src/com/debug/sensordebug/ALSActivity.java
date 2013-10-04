package com.debug.sensordebug;

import java.io.IOException;

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

public class ALSActivity extends Activity {
	String REG_FILE_PATH = "/sys/devices/platform/als_ps/driver/reg";
	private ArrayAdapter<String> adapter;  

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
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.alsconfig);

        mALS_Gain = (Spinner) findViewById(R.id.mALS_Gain);
        mALS_Persist = (Spinner) findViewById(R.id.mALS_Persist);

        mALS_Time = (Spinner) findViewById(R.id.mALS_Time);
        mALS_Rate = (Spinner) findViewById(R.id.mALS_Rate);

        mALSSaveButton = (Button) findViewById(R.id.mALSSaveButton);
         
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, ToolsFileOps.data0to255_table);         
        
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mALS_Gain.setAdapter(adapter);
        mALS_Persist.setAdapter(adapter);
        mALS_Time.setAdapter(adapter);
        mALS_Rate.setAdapter(adapter);        
        
        //添加事件Spinner事件监听    
        mALS_Gain.setOnItemSelectedListener(new SpinnerSelectedListener());
        mALS_Persist.setOnItemSelectedListener(new SpinnerSelectedListener());
        mALS_Time.setOnItemSelectedListener(new SpinnerSelectedListener());
        mALS_Rate.setOnItemSelectedListener(new SpinnerSelectedListener());        
        //设置默认值  
        mALS_Gain.setVisibility(View.VISIBLE);  
        mALS_Persist.setVisibility(View.VISIBLE);
        mALS_Time.setVisibility(View.VISIBLE);  
        mALS_Rate.setVisibility(View.VISIBLE);        
        
        mALSSaveButton.setOnClickListener(
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





