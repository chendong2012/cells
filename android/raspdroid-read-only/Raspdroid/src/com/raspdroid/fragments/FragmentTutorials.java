package com.raspdroid.fragments;


import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;

import com.actionbarsherlock.app.SherlockFragment;
import com.raspdroid.R;
import com.raspdroid.TutorialOne;
import com.raspdroid.TutorialThree;
import com.raspdroid.TutorialTwo;

public class FragmentTutorials extends SherlockFragment
{

	public Button tutorial1, tutorial2, tutorial3;
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) 
	{
		
		final View view = inflater.inflate(R.layout.fragment_tutorials,
				container, false);		
		
		tutorial1 = (Button) view.findViewById(R.id.installSoftwareTut);
		tutorial1.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v) 
			{
					Intent intent = new Intent(getActivity(), TutorialOne.class);
			        startActivity(intent);
			}
		});
		
		tutorial2 = (Button) view.findViewById(R.id.remoteButtonsTut);
		tutorial2.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v) 
			{
					Intent intent = new Intent(getActivity(), TutorialTwo.class);
			        startActivity(intent);
			}
		});
		
		tutorial3 = (Button) view.findViewById(R.id.remoteVoiceTut);
		tutorial3.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v) 
			{
					Intent intent = new Intent(getActivity(), TutorialThree.class);
			        startActivity(intent);
			}
		});
		
		return view;
	}

	
}
