package com.raspdroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ViewFlipper;

import com.raspdroid.AnimationFactory.FlipDirection;

public class TutorialOne extends Activity {
	Button button1;
	ViewFlipper flippy1;
	TextView tv;
	int i;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.tutorial_one);
		flippy1 = (ViewFlipper) findViewById(R.id.viewFlipper1);
		tv = (TextView) findViewById(R.id.tv);
		i = 0;
		tv.setText("Click Session information to setup your information.");
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
        		tv.setText("Click Session information to setup your information.");
        		break;
        	case 1:
        		tv.setText("Fill out your information here. Need at least an ip address and port number.");
        		break;
        	case 2:
        		tv.setText("Choose the item you want intalled.");
        		break;
        	case 3:
        		tv.setText("Click to install.");
        		break;
        	case 4:
        		tv.setText("Accept the agreement.");
        		break;
        	case 5:
        		tv.setText("Your software should be installed.");
        		break;
        	}
        	if(i >= 6)
        	{
        		tv.setText("Click Session information to setup your information.");
        		i = 0;
        	}
        }
    });
	}
}