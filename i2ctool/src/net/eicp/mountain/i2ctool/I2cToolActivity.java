package net.eicp.mountain.i2ctool;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.Toast;
import android.widget.AdapterView.OnItemSelectedListener;

public class I2cToolActivity extends Activity {
    void showToast(CharSequence msg) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_i2c_tool);
		
		spinner_i2c_num = (Spinner) findViewById(R.id.spinner_bus_num);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
                this, R.array.busnum, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner_i2c_num.setAdapter(adapter);
        spinner_i2c_num.setOnItemSelectedListener(i2cbusnum_listener);		
        
        spinner_access_flag = (Spinner) findViewById(R.id.spinner_access);
        ArrayAdapter<CharSequence> adapter1 = ArrayAdapter.createFromResource(
                this, R.array.access, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner_access_flag.setAdapter(adapter1);
        spinner_access_flag.setOnItemSelectedListener(access_listener);		
        
        
        
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_i2c_tool, menu);
		return true;
	}

	/*add sppiner listener for bus num*/
	private Spinner.OnItemSelectedListener i2cbusnum_listener  = 
	new Spinner.OnItemSelectedListener() {    	
			public void onItemSelected(android.widget.AdapterView av, View v,
               int position, long id) {
				curi2c_num = position;
				showToast("i2cbusnum_listener: position=" + curi2c_num + " id=" + id);
        	}

			@Override
			public void onNothingSelected(AdapterView<?> arg0) {
				// TODO Auto-generated method stub				
			}
	};

	/*add sppiner listener for write or read flag*/
	private Spinner.OnItemSelectedListener access_listener  = 
	new Spinner.OnItemSelectedListener() {    	
			public void onItemSelected(android.widget.AdapterView av, View v,
               int position, long id) {
				access_flag = position;
				showToast("access_flag: position=" + access_flag + " id=" + id);
        	}

			@Override
			public void onNothingSelected(AdapterView<?> arg0) {
				// TODO Auto-generated method stub				
			}
	};

	private int curi2c_num;
	private int curi2c_devaddr;
	private int curi2c_devreg;
	private int access_flag;
	private Spinner spinner_i2c_num;
	private Spinner spinner_access_flag;
	
}
