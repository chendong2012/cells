package com.raspdroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ViewFlipper;

import com.raspdroid.AnimationFactory.FlipDirection;

public class TutorialTwo extends Activity {
	Button button1;
	ViewFlipper flippy1;
	TextView tv;
	int i;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.tutorial_two);
		flippy1 = (ViewFlipper) findViewById(R.id.viewFlipper1);
		tv = (TextView) findViewById(R.id.tv);
		i = 0;
		tv.setText("Click WebIOPi Settings to setup your information.");
		button1 = (Button) findViewById(R.id.button1);
		button1.setOnClickListener(new OnClickListener (){
        @Override
        public void onClick(View v)
        {
        	i++;
        	AnimationFactory.flipTransition(flippy1, FlipDirection.LEFT_RIGHT);

        	switch(i)
        	{
        	case 0:
        		tv.setText("Click WebIOPi Settings to setup your information.");
        		break;
        	case 1:
        		tv.setText("Fill out your information here. Need at least an ip address and port number.");
        		break;
        	case 2:
        		tv.setText("Use Hide Button to hide the buttons you don't use.");
        		break;
        	case 3:
        		tv.setText("Only the unhidden buttons will be shown.");
        		break;
        	case 4:
        		tv.setText("Click Reset to unhide all the buttons.");
        		break;
        	case 5:
        		tv.setText("All of the buttons will be shown.");
        		break;
        	case 6:
        		tv.setText("Use Voice will provide audible feedback.");
        		break;
        	case 7:
        		tv.setText("Mute Voice will turn off audible feedback.");
        		break;
        	}
        	if(i >= 8)
        	{
        		tv.setText("Click WebIOPi Settings to setup your information.");
        		i = 0;
        	}
        }
    });
	}
}