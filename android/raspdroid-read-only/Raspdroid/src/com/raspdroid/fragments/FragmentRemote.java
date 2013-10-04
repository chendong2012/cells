package com.raspdroid.fragments;


import com.actionbarsherlock.app.SherlockFragment;
import com.raspdroid.ActivityRemoteFileTransfer;
import com.raspdroid.ActivityRemoteTerminal;
import com.raspdroid.R;
import com.raspdroid.helper.Dialogs;
import com.raspdroid.helper.General;

import android.app.Dialog;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class FragmentRemote extends SherlockFragment
{

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) 
	{
		
		final View view = inflater.inflate(R.layout.fragment_remote,
				container, false);		
		
		
	    View.OnClickListener clickListener = new View.OnClickListener() 
		{

			public void onClick(View view) 
			{
				
				switch (view.getId()) 
				{
					
					case R.id.remoteterminalButton:
						
							if(new General().areLogonCredentialsFilled(getActivity()))
							{
								
								final Intent terminalIntent = new Intent(getActivity(), 
										ActivityRemoteTerminal.class);
								
								getActivity().startActivity(terminalIntent);
						
								getActivity().overridePendingTransition(R.anim.translate_leftin,R.anim.translate_leftout);
								
							}
							
							else
							{
								
								final Dialog dialog = new Dialogs()
									.createRaspberryPiInformationDialog(getActivity());
								dialog.show();
								
							}


						break;
						
						
					case R.id.filetransferButton:
						
							if(new General().areLogonCredentialsFilled(getActivity()))
							{
	
								final Intent transferIntent = new Intent(getActivity(), 
										ActivityRemoteFileTransfer.class);
								
								getActivity().startActivity(transferIntent);
								
								getActivity().overridePendingTransition(R.anim.translate_leftin,R.anim.translate_leftout);
								
							}
							
							else
							{
								
								final Dialog dialog = new Dialogs()
									.createRaspberryPiInformationDialog(getActivity());
								dialog.show();
								
							}
						
						break;
			
				}
			}

		};
			
		view.findViewById(R.id.remoteterminalButton)
			.setOnClickListener(clickListener);
		
		view.findViewById(R.id.filetransferButton)
			.setOnClickListener(clickListener);
		
		return view;
	}

	
}
