package com.raspdroid.fragments;


import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;
import ch.ethz.ssh2.Connection;
import ch.ethz.ssh2.KnownHosts;
import ch.ethz.ssh2.Session;
import ch.ethz.ssh2.StreamGobbler;

import com.actionbarsherlock.app.SherlockFragment;
import com.raspdroid.ActivityMainScreen;
import com.raspdroid.ActivityRemoteTerminal;
import com.raspdroid.GpioButtons;
import com.raspdroid.R;
import com.raspdroid.VoiceActivity;
import com.raspdroid.helper.Dialogs;
import com.raspdroid.helper.PreferenceHolder;
import com.raspdroid.helper.SimpleVerifier;

public class FragmentSoftware extends SherlockFragment
{
	
	public static final String INSTALLED = "installed";
	public static final String NOT_INSTALLED = "notInstalled";
	public static final String CONNECTION_ERROR = "connectionError";
	public static final String GENERAL_ERROR = "generalError";

    Spinner selectionSpinner;
	
	public Button installButton;
	public Button remoteButton;
	public Button voiceButton;
	
	public int SPINNER_POSITION;

	KnownHosts database;
	
	String hostname, serverHostKeyAlgorithm;
	
	byte[] serverHostKey;
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) 
	{
		
		final View view = inflater.inflate(R.layout.fragment_software,
				container, false);	
		
		
		selectionSpinner = (Spinner)
				view.findViewById(R.id.selectionSpinner);
		
		selectionSpinner.setOnItemSelectedListener(new OnItemSelectedListener()
		{

			@Override
			public void onItemSelected(AdapterView<?> adapterView, View view, int position, long arg3) 
			{
				
				switch(position)
				{
				
					case 0:
					
						//Do nothing
					
						break;
				
					default:
						
						SPINNER_POSITION = selectionSpinner.getSelectedItemPosition();
						
						final String[] softwareStringArray = getResources()
								.getStringArray(R.array.softwarenames);
						
						final String softwareString = softwareStringArray[SPINNER_POSITION];
						
						new CheckIfSoftwareInstalled().execute(softwareString);
						
						break;

				}
				
				
				

				
			}

			@Override
			public void onNothingSelected(AdapterView<?> arg0) 
			{

				// Do nothing
				
			}

		});
		
		installButton = (Button)
				view.findViewById(R.id.installButton);
		
		
		installButton.setOnClickListener(new OnClickListener()
		{

			@Override
			public void onClick(View v) 
			{
				
				
				if(SPINNER_POSITION > 0)
				{
					
					final String[] softwarenameArray = getActivity().getResources()
							.getStringArray(R.array.softwarenames);
					
					final String softwarenameString = 
							softwarenameArray[SPINNER_POSITION];
					
					new InstallSoftware().execute(softwarenameString);
					
				}
				
				else
				{
					
					//You must select toast
					
				}

				
			}
						
		});
		
		remoteButton = (Button)
				view.findViewById(R.id.remoteButton);
		
		
		remoteButton.setOnClickListener(new OnClickListener()
		{

			@Override
			public void onClick(View v) 
			{
				Intent intent = new Intent(getActivity(), GpioButtons.class);
		        startActivity(intent);
			}
		});
		
		voiceButton = (Button)
				view.findViewById(R.id.voiceButton);
		
		
		voiceButton.setOnClickListener(new OnClickListener()
		{

			@Override
			public void onClick(View v) 
			{
					Intent intent = new Intent(getActivity(), VoiceActivity.class);
			        startActivity(intent);
			}
		});
		
		return view;
	}

	@Override
	public void onPause() 
	{
		super.onPause();
		
		selectionSpinner.setSelection(0);
		
		SPINNER_POSITION = 0;
		
	}

private class InstallSoftware extends AsyncTask<String, Integer, String> 
{
			
    ProgressDialog progressDialog;


	@Override
	protected void onPreExecute() 
	{
		
		final String[] programnameArray = getActivity().getResources()
				.getStringArray(R.array.softwareoptions);
		
		final String programnameString = 
				programnameArray[SPINNER_POSITION];
		
		progressDialog = new ProgressDialog(getActivity());
		progressDialog.setIndeterminate(true);
		progressDialog.setCancelable(false);
		progressDialog.setTitle(programnameString);
		progressDialog.show();
		progressDialog.setProgress(1);

	}

	@Override
	protected String doInBackground(String... command) 
	{
	
		/** Create a new KnownHosts database **/
			
		final KnownHosts databaseKnownHosts = new KnownHosts();
			
			
		/** Get a reference to a file with saved hosts on it **/
	
		final File knownhostsFile = new File
					(getActivity().getFilesDir(), "known_hosts");

			try 
			{
					
				/** If the file exists load the known hosts into memory **/

					if (knownhostsFile.exists())
					{
							
						databaseKnownHosts.addHostkeys(knownhostsFile);
								
					}

						
				/** Create an instance of the PreferenceHolder class **/

				final PreferenceHolder preferenceHolder = 
						new PreferenceHolder();
					
					
				/** Get the Activity's Context **/
					
				final Context activityContext = 
						getActivity();
					
					
				/** Get the necessary log-in credentials **/
					
				final String username = preferenceHolder
						.getUsername(activityContext);
					
				final String password = preferenceHolder
						.getUserPassword(activityContext);
	
				final String ipaddress = preferenceHolder
						.getIPAddress(activityContext);
						
						
				/** Set-up a new connection passing the host as a parameter **/

				Connection connection = new Connection(ipaddress);
					
	
				/** Connect to the host verifying the host key **/					
					
				connection.connect(new SimpleVerifier(activityContext, databaseKnownHosts));
					
					
				/** Authenticate the login credentials **/
	
				final boolean isAuthenticated = connection
						.authenticateWithPassword(username, password);
	
					if (!isAuthenticated)
					{
							
			    		return ActivityRemoteTerminal.CONNECTION_ERROR;
											
					}						

			/** Create a session **/
			
			Session session = connection.openSession();

			session.execCommand("sudo -S -p '' apt-get install "+ command[0]);
						
			OutputStream outputStream = new BufferedOutputStream(session.getStdin(), 8192);
					
			outputStream.write((new PreferenceHolder()
				   	.getUserPassword(activityContext) + "\n").getBytes());
				   
			outputStream.flush();
				   
		    outputStream.close();

					
			final InputStream inputStream = new StreamGobbler
					(session.getStdout());

		           
		    final BufferedReader bufferedReader = new BufferedReader
		        	(new InputStreamReader(inputStream));
		           
		    final StringBuilder stringBuilder = new StringBuilder();
		           
		    String line;
		           
		    	while ((line = bufferedReader.readLine()) != null) 
		        {
		        	   
		    		stringBuilder.append(line);
		        	stringBuilder.append('\n');
		               
		        }
		            
		    bufferedReader.close();
		         
		    session.close();

		    return stringBuilder.toString();  
		           
		}
				
		catch (Exception exception) 
		{
		        	
			return "Connection error!";
		        	
		} 

	}


	@Override
	protected void onProgressUpdate(Integer... values) 
	{
		
		switch (values[0])
		{
		
		case 1:
			
			progressDialog.setMessage("Connecting to your Raspberry Pi...");

			break;
			
			
		case 2:
			
			progressDialog.setMessage("Opening channel...");

			break;
			
			
		case 3:
			
			progressDialog.setMessage("Downloading...");

			break;
			
			
		case 4:
			
			progressDialog.setMessage("Gathering results...");

			break;
			
			
		default:
							
			progressDialog.setMessage("Doing something crazy...");

			break;
				
			
		
		}
		
	}

	

	@Override
	protected void onPostExecute(String result)
	{
		
		progressDialog.cancel();
		
        new Dialogs().createInformationDialog(getActivity(), "Results", result, true).show();

	}


	
}



private class CheckIfSoftwareInstalled extends AsyncTask<String, Integer, String> 
{
			
    private ProgressDialog progressDialog;
    
    
    
	@Override
	protected void onPreExecute() 
	{

		progressDialog = new ProgressDialog(getActivity());
		progressDialog.setIndeterminate(true);
		progressDialog.setCancelable(false);
		progressDialog.setTitle("Checking");
		progressDialog.show();
	    publishProgress(1);

	}


	@Override
	protected String doInBackground(String... command)
	{
		
		
		/** Create a new KnownHosts database **/
		
		final KnownHosts databaseKnownHosts = new KnownHosts();
		
		
		/** Get a reference to a file with saved hosts on it **/

		final File knownhostsFile = new File
				(getActivity().getFilesDir(), "known_hosts");

			try 
			{
				
				/** If the file exists load the known hosts into memory **/

					if (knownhostsFile.exists())
					{
						
						databaseKnownHosts.addHostkeys(knownhostsFile);
							
					}

					
				/** Create an instance of the PreferenceHolder class **/

				final PreferenceHolder preferenceHolder = 
						new PreferenceHolder();
				
				
				/** Get the Activity's Context **/
				
				final Context activityContext = 
						getActivity();
				
				
				/** Get the necessary log-in credentials **/
				
				final String username = preferenceHolder
						.getUsername(activityContext);
				
				final String password = preferenceHolder
						.getUserPassword(activityContext);

				final String ipaddress = preferenceHolder
						.getIPAddress(activityContext);
					
					
				/** Set-up a new connection passing the host as a parameter **/

				Connection connection = new Connection(ipaddress);
				

				/** Connect to the host verifying the host key **/					
				
				connection.connect(new SimpleVerifier(activityContext, databaseKnownHosts));
				
				/** Authenticate the login credentials **/

				final boolean isAuthenticated = connection
						.authenticateWithPassword(username, password);

					if (!isAuthenticated)
					{
						
		    			return CONNECTION_ERROR;
										
					}
					
				publishProgress(2);
					
				Session session = connection.openSession();
						
				session.execCommand("dpkg --get-selections | less | grep " + command[0]);

				final InputStream inputStream = new StreamGobbler
						(session.getStdout());

		           
		        final BufferedReader bufferedReader = new BufferedReader
		        		(new InputStreamReader(inputStream));
		           
		        final StringBuilder stringBuilder = new StringBuilder();
		           
		        String line;
		           
		            while ((line = bufferedReader.readLine()) != null) 
		            {
		        	   
		        	   stringBuilder.append(line);
		        	   stringBuilder.append('\n');
		               
		            }
		            
		        bufferedReader.close();
		        
			    publishProgress(3);

		        	if(!stringBuilder.toString().equals(""))
		        	{
		        		
		        		return INSTALLED;
		        		
		        		
		        	}
					
		        	else
		        	{
		        		
			    		return NOT_INSTALLED;

		        		
		        	}
					
					
								
			} 
		
			catch (IOException ioException)
			{
						
	    		return CONNECTION_ERROR;
				
			}
			
			catch(Exception exception)
			{
				
	    		return GENERAL_ERROR;
				
			}
        	
	}
	
	
	@Override
	protected void onProgressUpdate(Integer... values) 
	{
		
		final String[] programnameArray = getActivity().getResources()
				.getStringArray(R.array.softwareoptions);
		
		final String programnameString = 
				programnameArray[SPINNER_POSITION];
		
		
		switch (values[0])
		{
		
			case 1:
				
				progressDialog.setMessage("Connecting to your Raspberry Pi...");
	
				break;
				
				
			case 2:
				
				progressDialog.setMessage("Searching for " + (programnameString) + "...");
	
				break;
				
				
			case 3:
				
				progressDialog.setMessage("Gathering results...");
	
				break;
				
				
			default:
								
				progressDialog.setMessage("Doing something crazy...");
	
				break;			
				
		}
		
	}
	

	@Override
	protected void onPostExecute(String result)
	{
		
		progressDialog.cancel();
		
		final String[] programnameArray = getActivity().getResources()
				.getStringArray(R.array.softwareoptions);
		
		final String programnameString = 
				programnameArray[SPINNER_POSITION];
		
			if(result.equals(INSTALLED))
			{
				
				installButton.setText("Update " + programnameString);
				
				Toast.makeText(getActivity().getApplicationContext(), 
						programnameString + " is installed" , Toast.LENGTH_SHORT).show();		
				
			}
			
			else if(result.equals(NOT_INSTALLED))
			{
				
				installButton.setText("Install " + programnameString);
				
				Toast.makeText(getActivity().getApplicationContext(), 
						programnameString + " is not installed" , Toast.LENGTH_SHORT).show();	
				
			}
			
			else
			{
				
				final String hostverificationString = ((ActivityMainScreen)getActivity())
						.getHostVerificationString();

				final KnownHosts databaseKnownHosts = ((ActivityMainScreen)getActivity())
						.getKnownHostsDatabase();
				
				final String hostnameString = ((ActivityMainScreen)getActivity())
						.getHostNameString();
				
				final String hostkeyalgorithmString = ((ActivityMainScreen)getActivity())
						.getHostKeyAlgorithmString();
				
				final byte[] hostkeyByteArray = ((ActivityMainScreen)getActivity())
						.getHostKeyByteArray();
				
				
				if(hostverificationString != null && databaseKnownHosts != null && hostnameString != null 
						&& hostkeyalgorithmString != null && hostkeyByteArray != null)
				{
					

					if(hostverificationString.equals(SimpleVerifier.HOST_NEW))
					{
						
						Dialog dialog = new Dialogs().createHostKeyNewDialog(getActivity(), databaseKnownHosts,
								hostnameString, hostkeyalgorithmString, hostkeyByteArray);
						
						dialog.setOnCancelListener(new OnCancelListener()
						{

							@Override
							public void onCancel(DialogInterface dialog) 
							{

								final String[] softwareStringArray = getResources()
										.getStringArray(R.array.softwarenames);
								
								final String softwareString = softwareStringArray[SPINNER_POSITION];
								
								new CheckIfSoftwareInstalled().execute(softwareString);							
								
							}

						});					
						
						dialog.show();	
						
					}
					
					else if(hostverificationString.equals(SimpleVerifier.HOST_KEY_CHANGED))
					{
						
						Dialog dialog = new Dialogs().createHostKeyChangedDialog(getActivity(), databaseKnownHosts,
								hostnameString, hostkeyalgorithmString, hostkeyByteArray);
						
						dialog.setOnCancelListener(new OnCancelListener()
						{

							@Override
							public void onCancel(DialogInterface dialog) 
							{

								final String[] softwareStringArray = getResources()
										.getStringArray(R.array.softwarenames);
								
								final String softwareString = softwareStringArray[SPINNER_POSITION];
								
								new CheckIfSoftwareInstalled().execute(softwareString);							
								
							}

						});					
						
						dialog.show();	

					}
					
					else
					{
						
						final Dialog dialog = new Dialogs().createInformationConfirmDialog(getActivity(), 
								"General error", getResources().getString(R.string.error_general), true);
						
						dialog.show();
						
					}

				}
				
				else
				{
					final Dialog dialog = new Dialogs().createInformationConfirmDialog(getActivity(), 
							"Connection error", getResources().getString(R.string.error_connection), true);
					dialog.show();	
				}
			}				
		}
	}
}
















