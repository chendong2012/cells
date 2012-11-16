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
		
        Spinner s1 = (Spinner) findViewById(R.id.spinner1);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
                this, R.array.busnum, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        s1.setAdapter(adapter);
        s1.setOnItemSelectedListener(i2cbusnum_listener);		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_i2c_tool, menu);
		return true;
	}

	/*add sppiner listener*/
	private Spinner.OnItemSelectedListener i2cbusnum_listener  = 
	new Spinner.OnItemSelectedListener() {    	
			public void onItemSelected(android.widget.AdapterView av, View v,
               int position, long id) {
				curi2c_cnum = position;
				showToast("i2cbusnum_listener: position=" + curi2c_cnum + " id=" + id);
        	}

			@Override
			public void onNothingSelected(AdapterView<?> arg0) {
				// TODO Auto-generated method stub				
			}
	};
	
	private int curi2c_cnum;
	private int curi2c_devaddr;
	private int curi2c_devreg;
	
}
