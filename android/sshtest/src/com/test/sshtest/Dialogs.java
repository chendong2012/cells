package com.test.sshtest;

import java.io.File;
import java.io.IOException;

import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Vibrator;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import ch.ethz.ssh2.KnownHosts;




public class Dialogs 
{
	
	public static final String STRING_ERRORENTRY = "Please check your entries";
	public static final String STRING_SAVED = "Saved!";

	public Dialog createRaspberryPiInformationDialog(final Context context)
	{	
		final Vibrator vibrator = (Vibrator) 
				context.getSystemService(Context.VIBRATOR_SERVICE);
    	final Dialog dialog = new Dialog(context);
    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_raspberrypiinformation);
    	dialog.setCancelable(true);

    	final EditText ipaddressEditText = (EditText)
    			dialog.findViewById(R.id.ipaddressEditText);
    	ipaddressEditText.setText("preferenceHolder.getIPAddress(context)");
    	final EditText usernameEditText = (EditText)
    			dialog.findViewById(R.id.usernameEditText);
    	usernameEditText.setText("preferenceHolder.getUsername(context)");
    	final EditText passwordEditText = (EditText)
    			dialog.findViewById(R.id.passwordEditText);
    	passwordEditText.setText("preferenceHolder.getUserPassword(context)");
    	final EditText portEditText = (EditText)
    			dialog.findViewById(R.id.portEditText);
    	portEditText.setText("preferenceHolder.getPort(context)");
    	final Button saveButton = (Button)
    			dialog.findViewById(R.id.saveButton);
    	saveButton.setOnClickListener(new OnClickListener()
    	{

			@Override
			public void onClick(View v) 
			{
				
				vibrator.vibrate(1000);
				
				
				final String ipaddressString = ipaddressEditText.
						getText().toString();
				
				final String usernameString = usernameEditText.
						getText().toString();
				
				final String passwordString = passwordEditText.
						getText().toString();
				
				final String portString = portEditText.
						getText().toString();
				
					if(ipaddressString.equals("") || usernameString.equals("") || 
							passwordString.equals("") || portString.equals(""))
					{
						
						Toast.makeText(context.getApplicationContext(), 
								STRING_ERRORENTRY, Toast.LENGTH_LONG).show();
						
					}
					
					else
					{					
						//preferenceHolder.saveIPAddress(context, ipaddressString);
						//preferenceHolder.saveUsername(context, usernameString);
						//preferenceHolder.saveUserPassword(context, passwordString);
						//preferenceHolder.savePort(context, portString);
						Toast.makeText(context.getApplicationContext(), 
								STRING_SAVED, Toast.LENGTH_LONG).show();				
					}	
				dialog.dismiss();					
			}
    	});
    	return dialog;
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//VOICE CONTROL AND BUTTON CONTROL DIALOG FOR WEBIOPI//////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	public Dialog webiopiDialog(final Context context)
	{	
		final Vibrator vibrator = (Vibrator) 
				context.getSystemService(Context.VIBRATOR_SERVICE);
    	final Dialog dialog = new Dialog(context);
    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_webiopi);
    	dialog.setCancelable(true);
	
    	final EditText WEBIOPI_ipaddressEditText = (EditText)
    			dialog.findViewById(R.id.ipaddressEditText_webiopi);
    	WEBIOPI_ipaddressEditText.setText("mountain.eicp.net");
    	final EditText WEBIOPI_usernameEditText = (EditText)
    			dialog.findViewById(R.id.usernameEditText_webiopi);
    	WEBIOPI_usernameEditText.setText("xbian");
    	final EditText WEBIOPI_passwordEditText = (EditText)
    			dialog.findViewById(R.id.passwordEditText_webiopi);
    	WEBIOPI_passwordEditText.setText("raspberry");
    	final EditText WEBIOPI_portEditText = (EditText)
    			dialog.findViewById(R.id.portEditText_webiopi);
    	WEBIOPI_portEditText.setText("8000");
    	final EditText WEBIOPI_pinEditText = (EditText)
    			dialog.findViewById(R.id.pinEditText_webiopi);
    	WEBIOPI_pinEditText.setText("24");
    	final Button WEBIOPI_saveButton = (Button)
    			dialog.findViewById(R.id.saveButton);
    	WEBIOPI_saveButton.setOnClickListener(new OnClickListener()
    	{
			@Override
			public void onClick(View v) 
			{
				vibrator.vibrate(1000);
				final String WEBIOPI_ipaddressString = WEBIOPI_ipaddressEditText.
						getText().toString();
				final String WEBIOPI_usernameString = WEBIOPI_usernameEditText.
						getText().toString();
				final String WEBIOPI_passwordString = WEBIOPI_passwordEditText.
						getText().toString();
				final String WEBIOPI_portString = WEBIOPI_portEditText.
						getText().toString();
				final String WEBIOPI_pinString = WEBIOPI_pinEditText.
						getText().toString();
					if(WEBIOPI_ipaddressString.equals("") || WEBIOPI_portString.equals(""))
					{
						Toast.makeText(context.getApplicationContext(), 
								STRING_ERRORENTRY, Toast.LENGTH_LONG).show();	
					}
					else
					{

						Toast.makeText(context.getApplicationContext(), 
								STRING_SAVED, Toast.LENGTH_LONG).show();						
					}
				dialog.dismiss();					
			}
    	});
    	return dialog;
	}
	
	public Dialog show_options(final Context context)
	{
    	final Dialog dialog = new Dialog(context);
    	
    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_controls);
    	
    	final TextView option1 = (TextView) dialog.findViewById(R.id.option1);
    	option1.setOnClickListener(new OnClickListener()
    	{
    		@Override
    		public void onClick(View v)
    		{
    			Toast.makeText(context, "GPIO pin to control.", Toast.LENGTH_SHORT).show();
    		}
    	});
    	
    	final TextView option2 = (TextView) dialog.findViewById(R.id.option2);
    	option2.setOnClickListener(new OnClickListener()
    	{
    		@Override
    		public void onClick(View v)
    		{
    			Toast.makeText(context, "Set pin as output.", Toast.LENGTH_SHORT).show();
    		}
    	});
    	
    	final TextView option3 = (TextView) dialog.findViewById(R.id.option3);
    	option3.setOnClickListener(new OnClickListener()
    	{
    		@Override
    		public void onClick(View v)
    		{
    			Toast.makeText(context, "Set pin as input.", Toast.LENGTH_SHORT).show();
    		}
    	});
    	
    	final TextView option4 = (TextView) dialog.findViewById(R.id.option4);
    	option4.setOnClickListener(new OnClickListener()
    	{
    		@Override
    		public void onClick(View v)
    		{
    			Toast.makeText(context, "Turn pin on.", Toast.LENGTH_SHORT).show();
    		}
    	});
    	
    	final TextView option5 = (TextView) dialog.findViewById(R.id.option5);
    	option5.setOnClickListener(new OnClickListener()
    	{
    		@Override
    		public void onClick(View v)
    		{
    			Toast.makeText(context, "Turn pin on.", Toast.LENGTH_SHORT).show();
    		}
    	});
    	
    	final TextView option6 = (TextView) dialog.findViewById(R.id.option6);
    	option6.setOnClickListener(new OnClickListener()
    	{
    		@Override
    		public void onClick(View v)
    		{
    			Toast.makeText(context, "Turn pin off.", Toast.LENGTH_SHORT).show();
    		}
    	});
    	
    	final TextView option7 = (TextView) dialog.findViewById(R.id.option7);
    	option7.setOnClickListener(new OnClickListener()
    	{
    		@Override
    		public void onClick(View v)
    		{
    			Toast.makeText(context, "Turn pin off.", Toast.LENGTH_SHORT).show();
    		}
    	});
    	
    	final TextView option8 = (TextView) dialog.findViewById(R.id.option8);
    	option8.setOnClickListener(new OnClickListener()
    	{
    		@Override
    		public void onClick(View v)
    		{
    			Toast.makeText(context, "Get status of current pin.", Toast.LENGTH_SHORT).show();
    		}
    	});
    	
    	final Button ok = (Button)
    			dialog.findViewById(R.id.ok);
    	ok.setOnClickListener(new OnClickListener()
    	{

			@Override
			public void onClick(View v) 
			{
				dialog.dismiss();
			}
    	});
	    	
	    return dialog;	

	}
	
	public Dialog createGPIOListDialog(final Context context, final boolean iscancelableBoolean,
			final ImageView gpioImageView)
	{		
    	
		final Vibrator vibrator = (Vibrator) 
				context.getSystemService(Context.VIBRATOR_SERVICE);

		final int vibratedurationInteger = 1000;
		
    	final Dialog dialog = new Dialog(context);

    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_list);
    	dialog.setCancelable(iscancelableBoolean);
    			

    	final TextView titleTextView = (TextView)
    			dialog.findViewById(R.id.titleTextView);
    	titleTextView.setText("GPIO image");
    	
    	final ListView itemListView = (ListView)
    			dialog.findViewById(R.id.itemListView);
    	        
        ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(context,
                    android.R.layout.simple_list_item_1, context.getResources().getStringArray(R.array.gpioimageselection));
        
        itemListView.setAdapter(arrayAdapter);			
    	
        itemListView.setOnItemClickListener(new OnItemClickListener()
        {
			@Override
			public void onItemClick(AdapterView<?> arg0, View view, int positionInteger, long arg3) 
			{
				vibrator.vibrate(vibratedurationInteger);
				switch(positionInteger)
				{
					case 0:
	
						if(gpioImageView != null)
						{
							//gpioImageView.setImageDrawable(context.getResources().getDrawable(R.drawable.image_p5topside));
							
						}
						break;
				
					case 1:
						
						if(gpioImageView != null)
						{
							//gpioImageView.setImageDrawable(context.getResources().getDrawable(R.drawable.image_p5underside));
							
						}
							
						break;
				
					case 2:
						
						if(gpioImageView != null)
						{
							//gpioImageView.setImageDrawable(context.getResources().getDrawable(R.drawable.image_revisionone));
							
						}
						
						break;
			
					case 3:
				
						if(gpioImageView != null)
						{
							//gpioImageView.setImageDrawable(context.getResources().getDrawable(R.drawable.image_revisiontwo));
							
						}
						
					break;
			
					default:
				
						if(gpioImageView != null)
						{
							//gpioImageView.setImageDrawable(context.getResources().getDrawable(R.drawable.image_p5topside));
							
						}
						
					break;	
				}
				dialog.dismiss();
			}
        });
    	return dialog;
	}
	
	public Dialog createInformationDialog(final Context context, final String titleString, 
			final String bodyString, final boolean iscancelableBoolean)
	{		
    	final Dialog dialog = new Dialog(context);
    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_information);
    	dialog.setCancelable(iscancelableBoolean);
    	
    	final TextView titleTextView = (TextView)
    			dialog.findViewById(R.id.titleTextView);
    	
	    	if(titleString != null)
	    	{
	        	titleTextView.setText(String.valueOf(titleString));	
	    	}
    	
    	final TextView bodyTextView = (TextView)
    			dialog.findViewById(R.id.bodyTextView);
		
	    	if(bodyString != null)
	    	{
	    		bodyTextView.setText(String.valueOf(bodyString));
	    	}
	    return dialog;
	}
	
	public Dialog createInformationConfirmDialog(final Context context, final String titleString, 
			final String bodyString, final boolean iscancelableBoolean)
	{		
		final Vibrator vibrator = (Vibrator) 
				context.getSystemService(Context.VIBRATOR_SERVICE);
		
		final int vibratedurationInteger = 1000;
		
    	final Dialog dialog = new Dialog(context);

    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_informationconfirm);
    	dialog.setCancelable(iscancelableBoolean);
    	
    	final TextView titleTextView = (TextView)
    			dialog.findViewById(R.id.titleTextView);
    	
	    	if(titleString != null)
	    	{
	        	titleTextView.setText(String.valueOf(titleString));
	    	}
    	
    	final TextView bodyTextView = (TextView)
    			dialog.findViewById(R.id.bodyTextView);
		
	    	if(bodyString != null)
	    	{
	    		bodyTextView.setText(String.valueOf(bodyString));
	    	}
	    	
    	final Button confirmButton = (Button)
    			dialog.findViewById(R.id.confirmButton);	
    	
    	confirmButton.setOnClickListener(new OnClickListener()
    	{

			@Override
			public void onClick(View view) 
			{
				vibrator.vibrate(vibratedurationInteger);
				dialog.dismiss();
			}
    	});
	    return dialog;
	}
	
	public Dialog librariesused(final Context context, final boolean iscancelableBoolean, final String[] contentStringArray, final String titleString)
	{
		
    	final Dialog dialog = new Dialog(context);
    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_list);
    	dialog.setCancelable(iscancelableBoolean);
    	final TextView titleTextView = (TextView)
    			dialog.findViewById(R.id.titleTextView);
    	titleTextView.setText(titleString);    	
    	final ListView itemListView = (ListView)
    			dialog.findViewById(R.id.itemListView);
    	
        ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(context,
                    android.R.layout.simple_list_item_1, contentStringArray);
        itemListView.setAdapter(arrayAdapter);
        
        itemListView.setOnItemClickListener(new ListView.OnItemClickListener() {

   		 public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
   		        switch(position)
   		        {
   		        case 0:
   		         final Intent abs = new Intent
         		 (Intent.ACTION_VIEW, Uri.parse("http://actionbarsherlock.com/"));
   		         context.startActivity(abs);
   		        	break;
   		        case 1:
   		        	final Intent viewpageindicator = new Intent
            		(Intent.ACTION_VIEW, Uri.parse("http://viewpagerindicator.com/"));
      		        context.startActivity(viewpageindicator);
   		        	break;
   		     case 2:
   		    	 	final Intent slidingmenu = new Intent
   		    	 	(Intent.ACTION_VIEW, Uri.parse("http://github.com/jfeinstein10/SlidingMenu"));
   		    	 	context.startActivity(slidingmenu);
   		        	break;
   		     case 3:
   		        	final Intent jsch = new Intent
            		(Intent.ACTION_VIEW, Uri.parse("http://www.jcraft.com/jsch/"));
      		        context.startActivity(jsch);
   		        	break;
   		     case 4:
		        	final Intent jzlib = new Intent
         		(Intent.ACTION_VIEW, Uri.parse("http://www.jcraft.com/jzlib/"));
   		        context.startActivity(jzlib);
		        	break;
	   		  case 5:
		        	final Intent animationfactory = new Intent
	   		(Intent.ACTION_VIEW, Uri.parse("http://code.google.com/p/android-3d-flip-view-transition/"));
			        context.startActivity(animationfactory);
		        	break;
   		        }
   		    }
   	});
        return dialog;
	}
	
	public Dialog contributors(final Context context, final boolean iscancelableBoolean, final String[] contentStringArray, final String titleString)
	{
		
    	final Dialog dialog = new Dialog(context);
    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_list);
    	dialog.setCancelable(iscancelableBoolean);
    	final TextView titleTextView = (TextView)
    			dialog.findViewById(R.id.titleTextView);
    	titleTextView.setText(titleString);    	
    	final ListView itemListView = (ListView)
    			dialog.findViewById(R.id.itemListView);
    	
        ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(context,
                    android.R.layout.simple_list_item_1, contentStringArray);
        itemListView.setAdapter(arrayAdapter);
        
        itemListView.setOnItemClickListener(new ListView.OnItemClickListener() {

   		 public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
   		        switch(position)
   		        {
   		        case 0:
   		         final Intent john = new Intent
         		 (Intent.ACTION_VIEW, Uri.parse("http://plus.google.com/110460893608300268290"));
   		         context.startActivity(john);
   		        	break;
   		        case 1:
   		        	final Intent dustin = new Intent
            		(Intent.ACTION_VIEW, Uri.parse("http://plus.google.com/108019745978944993252"));
      		        context.startActivity(dustin);
   		        	break;
   		     case 2:
   		    	 	final Intent Maciej = new Intent
   		    	 	(Intent.ACTION_VIEW, Uri.parse("http://plus.google.com/113021226683267437990"));
   		    	 	context.startActivity(Maciej);
   		        	break;
   		        }
   		    }
   	});
        return dialog;
	}
	
	
	public Dialog createHostKeyNewDialog(final Context context, final KnownHosts databaseKnownHosts, final String hostnameString,
			final String hostkeyalgorithmString, final byte[] hostkeyByteArray)
	{
    	final Dialog dialog = new Dialog(context);

    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_addhost);
    	dialog.setCancelable(true);
    	
    	final TextView bodyTextView = (TextView)
    			dialog.findViewById(R.id.bodyTextView);
    	
    	bodyTextView.setText("You are attempting to connect to a host (" + (hostnameString) + ") for the first time. " +
    			"You should connect to this host only if you trust the host AND the network you are on.");
    	
		
    	View.OnClickListener clickListener = new View.OnClickListener() 
		{		
			
			public void onClick(View view) 
			{
				//vibrator.vibrate(vibratedurationInteger);

				switch (view.getId()) 
				{
					case R.id.cancelButton:
						dialog.dismiss();
						break;
						
					case R.id.connectButton:
						
							if(databaseKnownHosts != null && !hostnameString.equals("") && 
								!hostkeyalgorithmString.equals("") && hostkeyByteArray != null)
							{
								try
								{
									databaseKnownHosts.addHostkey(new String[] { hostnameString }, 
											hostkeyalgorithmString, hostkeyByteArray);
									
									File knownhostsFile = new File(context.getFilesDir(), "known_hosts");
										if (!knownhostsFile.exists()) {
									        try {
									        	knownhostsFile.createNewFile();
									        } 
									      catch (IOException e) {
									        	//TODO error toast
									        }
										}
										
									KnownHosts.addHostkeyToFile(knownhostsFile, new String[] { hostnameString }, 
											hostkeyalgorithmString, hostkeyByteArray);
								}
								catch(Exception exception)
								{
						        	//TODO error toast
								}
								dialog.cancel();
							}
							else
							{
					        	//TODO error toast
							}
						break;
						
					default:
						
						break;
				}
			}
		};
		
		dialog.findViewById(R.id.cancelButton)
			.setOnClickListener(clickListener);

		dialog.findViewById(R.id.connectButton)
			.setOnClickListener(clickListener);	
				
		return dialog;
	}
	
	public Dialog createHostKeyChangedDialog(final Context context, final KnownHosts databaseKnownHosts, final String hostnameString,
			final String hostkeyalgorithmString, final byte[] hostkeyByteArray)
	{
    	final Dialog dialog = new Dialog(context);

    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_addhost);
    	dialog.setCancelable(true);
    	
    	final TextView bodyTextView = (TextView)
    			dialog.findViewById(R.id.bodyTextView);
    	
    	bodyTextView.setText("WARNING! You are attempting to connect to a host (" + (hostnameString) + ") which has changed it's host key. " +
    			"This may mean that you are under attack! You should only continue connecting to this host if you know for sure that the host key has changed.");    	
		
    	View.OnClickListener clickListener = new View.OnClickListener() 
		{		
			public void onClick(View view) 
			{
				//vibrator.vibrate(vibratedurationInteger);

				switch (view.getId()) 
				{
					case R.id.cancelButton:
		
						dialog.dismiss();
						
						break;
						
					case R.id.connectButton:
						
							if(databaseKnownHosts != null && !hostnameString.equals("") && 
								!hostkeyalgorithmString.equals("") && hostkeyByteArray != null)
							{
								
								try
								{
									databaseKnownHosts.addHostkey(new String[] { hostnameString }, 
											hostkeyalgorithmString, hostkeyByteArray);
									
									File knownhostsFile = new File(context.getFilesDir(), "known_hosts");
	
										if (!knownhostsFile.exists()) 
										{
									        try 
									        {
									        	knownhostsFile.createNewFile();
									        } 
									        
									        catch (IOException e) 
									        {
									        		
									        	//TODO error toast
									        
									        }
									        
										}
										
									KnownHosts.addHostkeyToFile(knownhostsFile, new String[] { hostnameString }, 
											hostkeyalgorithmString, hostkeyByteArray);
									
								}
								
								catch(Exception exception)
								{
						        	//TODO error toast
								}
								dialog.cancel();
							}
							
							else
							{
					        	//TODO error toast	
							}
							
						break;
						
					default:
						
						break;
				}
			}	
		};
		
		dialog.findViewById(R.id.cancelButton)
			.setOnClickListener(clickListener);

		dialog.findViewById(R.id.connectButton)
			.setOnClickListener(clickListener);	
				
		return dialog;	
	}
}