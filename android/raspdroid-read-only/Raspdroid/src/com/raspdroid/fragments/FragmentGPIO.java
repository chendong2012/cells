package com.raspdroid.fragments;


import com.actionbarsherlock.app.SherlockFragment;
import com.raspdroid.R;
import com.raspdroid.helper.Dialogs;
import com.raspdroid.helper.RaspdroidApplication;
import com.raspdroid.helper.Test;

import android.app.Dialog;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.widget.ImageView;

public class FragmentGPIO extends SherlockFragment
{

	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) 
	{
		
		final View view = inflater.inflate(R.layout.fragment_gpio,
				container, false);		
		
		
		
		Test test = new Test();
		Log.e("TAG", String.valueOf(test.getInteger()));
		
		
		
		
		final RaspdroidApplication raspdroidApplication = 
				(RaspdroidApplication)getActivity().getApplicationContext();
	
		final ImageView gpioImageView = (ImageView)
				view.findViewById(R.id.gpioImageView);
		
		final Animation fadeinAnimation = new AlphaAnimation(0, 1);
		fadeinAnimation.setDuration(1500);
		gpioImageView.startAnimation(fadeinAnimation);
		
		
		switch(raspdroidApplication.getGPIOImageIndex())
		{
			
			case 0:
				
				gpioImageView.setImageDrawable(getActivity().getResources().getDrawable(R.drawable.image_p5topside));
				
				break;
				
				
			case 1:
				
				gpioImageView.setImageDrawable(getActivity().getResources().getDrawable(R.drawable.image_p5underside));
				
				break;
				
				
			case 2:
				
				gpioImageView.setImageDrawable(getActivity().getResources().getDrawable(R.drawable.image_revisionone));
				
				break;
				
				
			case 3:
				
				gpioImageView.setImageDrawable(getActivity().getResources().getDrawable(R.drawable.image_revisiontwo));
				
				break;
				
				
			default:
				
				gpioImageView.setImageDrawable(getActivity().getResources().getDrawable(R.drawable.image_p5topside));
				
				break;

		}
		
		

		
		gpioImageView.setOnClickListener(new OnClickListener()
		{

			@Override
			public void onClick(View view)
			{

				Dialog dialog = new Dialogs().createGPIOListDialog
						(getActivity(), true, gpioImageView);
				
				dialog.show();
				
			}

		});
		
		
		return view;
	}
	
	
	


	
}
