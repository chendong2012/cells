package com.example.mydialog;



import android.app.Dialog;
import android.content.Context;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class Dialogs {

	/*创建一个对话框*/
	public Dialog creatDemoDialog(final Context context) {

		
    	final Dialog dialog = new Dialog(context);
    	
    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog1);
    	dialog.setCancelable(true);

    	final Button saveButton = (Button)
    			dialog.findViewById(R.id.mDialog1button);
    	saveButton.setOnClickListener(new OnClickListener()
    	{
			@Override
			public void onClick(View v) 
			{
				dialog.dismiss();					
			}
    	});
    	return dialog;		
	}
	/*end*/
}
