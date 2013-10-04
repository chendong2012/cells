package com.raspdroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ViewFlipper;

import com.raspdroid.AnimationFactory.FlipDirection;

public class TutorialThree extends Activity {
	Button button1;
	ViewFlipper flippy1;
	TextView tv;
	int i;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.tutorial_three);
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
        		tv.setText("List of commands shows a list of commands the app understands.");
        		break;
        	case 3:
        		tv.setText("Touch a command and a tost message will show what is supported.");
        		break;
        	case 4:
        		tv.setText("Use Voice will provide audible feedback.");
        		break;
        	case 5:
        		tv.setText("Mute Voice will turn off audible feedback.");
        		break;
        	case 6:
        		tv.setText("Touch the microphone.");
        		break;
        	case 7:
        		tv.setText("Begin speaking");
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