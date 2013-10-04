package com.chendong.debugtools;

import java.io.IOException;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;

public class Command extends Activity {
	String REG_FILE_PATH = "/sys/devices/platform/als_ps/driver/reg";
//	private static final String[] m={"A型","B型","O型","AB型","其他"};  
	private ArrayAdapter<String> adapter;  
	Spinner s1;
	Spinner s2;
	Button btn_do;
	
	String current_reg;
	String current_value;
	int reg_idx;
	int value_idx;
	
    /**
     * Names corresponding to the different example menu resources.
     */
    private static final String device_table[] = {
        "0", "1", "2","3", "4", "5","6", "7", "8","9", 
        "10", "11", "12","13", "14", "15","16", "17", "18","19",
        "20", "21", "22","23", "24", "25","26", "27", "28","29",
        "30", "31", "32","33", "34", "35","36", "37", "38","39",
        "40", "41", "42","43", "44", "45","46", "47", "48","49",
        "50", "51", "52","53", "54", "55","56", "57", "58","59",
        "60", "61", "62","63", "64", "65","66", "67", "68","69",
        "70", "71", "72","73", "74", "75","76", "77", "78","79",
        "80", "81", "82","83", "84", "85","86", "87", "88","89",
        "90", "91", "92","93", "94", "95","96", "97", "98","99",

        "100", "101", "102","103", "104", "105","106", "107", "108","109",
        "110", "111", "112","113", "114", "115","116", "117", "118","119",
        "120", "121", "122","123", "124", "125","126", "127", "128","129",
        "130", "131", "132","133", "134", "135","136", "137", "138","139",
        "140", "141", "142","143", "144", "145","146", "147", "148","149",
        "150", "151", "152","153", "154", "155","156", "157", "158","159",
        "160", "161", "162","163", "164", "165","166", "167", "168","169",
        "170", "171", "172","173", "174", "175","176", "177", "178","179",
        "180", "181", "182","183", "184", "185","186", "187", "188","189",
        "190", "191", "192","193", "194", "195","196", "197", "198","199",

        "200", "201", "202","203", "204", "205","206", "207", "208","209",
        "210", "211", "212","213", "214", "215","216", "217", "218","219",
        "220", "221", "222","223", "224", "225","226", "227", "228","229",
        "230", "231", "232","233", "234", "235","236", "237", "238","239",
        "240", "241", "242","243", "244", "245","246", "247", "248","249",
        "250", "251", "252","253", "254", "255",

    };
	
    void showToast(CharSequence msg) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.runcommand);

         s1 = (Spinner) findViewById(R.id.spinner1);
         s2 = (Spinner) findViewById(R.id.spinner2);
         btn_do = (Button) findViewById(R.id.button1);
         
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, device_table);         
        
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        s1.setAdapter(adapter);
        s2.setAdapter(adapter);
        
        
        //添加事件Spinner事件监听    
        s1.setOnItemSelectedListener(new Spinner1SelectedListener());
        s2.setOnItemSelectedListener(new Spinner2SelectedListener());
        
        //设置默认值  
        s1.setVisibility(View.VISIBLE);  
        s2.setVisibility(View.VISIBLE);
        btn_do.setOnClickListener(
        		new OnClickListener() {
					
					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						//发送数据到设备
						showToast(device_table[reg_idx]+"--->"+device_table[value_idx]);
						String dataString=device_table[reg_idx]+" "+device_table[value_idx];
						try {
							ToolsFileOps.setFileCxt(REG_FILE_PATH, dataString);
						} catch (IOException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						
					}
				}
        		);
        
    }

	class Spinner1SelectedListener implements OnItemSelectedListener {  
	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
	            long arg3) {
	    	reg_idx = arg2;
	    	/*
	    	if (arg2 > 4) {
	    	showToast("error");
	    	} else
	    	showToast("你选择的是："+m[arg2]);
	    	*/
	    }  
	    public void onNothingSelected(AdapterView<?> arg0) {  
	    }  
	}
	
	class Spinner2SelectedListener implements OnItemSelectedListener {  
	    public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
	            long arg3) {  
	    	value_idx = arg2;
	    	/*
	    	if (arg2 > 4) {
	    	showToast("error");
	    	} else
	    	showToast("你选择的是："+m[arg2]);  */
	    }  
	    public void onNothingSelected(AdapterView<?> arg0) {  
	    }  
	}
}





