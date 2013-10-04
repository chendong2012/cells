/**    
* Copyright 2013, John Persano
* This project includes software developed by John Persano and Dustin Evans.
* http://www.androidianlabs.com/
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this software except in compliance with the License.
* You may obtain a copy of the License at:
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/



package com.raspdroid;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Stack;
import java.util.Vector;

import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.DialogInterface.OnDismissListener;
import android.content.Intent;
import android.content.DialogInterface.OnKeyListener;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;

import ch.ethz.ssh2.Connection;
import ch.ethz.ssh2.KnownHosts;
import ch.ethz.ssh2.SCPClient;
import ch.ethz.ssh2.SFTPv3Client;
import ch.ethz.ssh2.SFTPv3DirectoryEntry;
import com.actionbarsherlock.app.ActionBar;
import com.actionbarsherlock.app.ActionBar.OnNavigationListener;
import com.actionbarsherlock.app.SherlockActivity;
import com.actionbarsherlock.view.Menu;
import com.actionbarsherlock.view.MenuInflater;
import com.actionbarsherlock.view.MenuItem;
import com.raspdroid.helper.Dialogs;
import com.raspdroid.helper.FileArrayAdapter;
import com.raspdroid.helper.General;
import com.raspdroid.helper.Option;
import com.raspdroid.helper.PreferenceHolder;
import com.raspdroid.helper.SimpleVerifier;
import com.raspdroid.helper.SimpleVerifier.HostDataCallback;

public class ActivityRemoteFileTransfer extends SherlockActivity implements OnNavigationListener, HostDataCallback
{	
	
	private static final String CONNECTION_ERROR = "connectionError";
	private static final String CONNECTED = "connected";

	
	private File REMOTE_DIRECTORY = new File("/home");
	private File LOCAL_DIRECTORY = new File
			(Environment.getExternalStorageDirectory().getPath());
	

	private File REMOTE_PENDING_FILE, LOCAL_PENDING_FILE;
			
	
	private Stack<File> remotedirectoryStack  = new Stack<File>();
	private Stack<File> localdirectoryStack  = new Stack<File>();

	
	private FileArrayAdapter remotefileArrayAdapter, localfileArrayAdapter;
	
	
	private TextView sendrecieveTextView;
	
	private Button sendfileButton;
	
	private Dialog localfileDialog, remotefileDialog;

		
	private Connection connection;

	private KnownHosts databaseKnownHosts;
	
	private String hostnameString, hostkeyalgorithmString, hostverificationString; 
	
	private byte[] hostkeyByteArray;
	
	private SFTPv3Client sftpClient;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_filetransfer);
        
        
        /** Attempt to connect to the remote computer passing a 'true' parameter to display a dialog if 
         *  the user has bad credentials **/
        
        connectToPi(true);
       		
		
        /** Get a reference to the Action Bar and decide not to show the application's title **/

	    final ActionBar actionBar = getSupportActionBar();
	    actionBar.setDisplayShowTitleEnabled(false);

	    
        /** Get a reference to the Action Bar's context **/

	    final Context actionbarContext = actionBar.getThemedContext();
	    
	    
        /** Set up an Array Adapter for the List Navigation **/

	    final ArrayAdapter<CharSequence> arrayAdapter = ArrayAdapter
	    		.createFromResource(actionbarContext, R.array.transferselection, R.layout.sherlock_spinner_item);
	    
	    arrayAdapter.setDropDownViewResource(R.layout.sherlock_spinner_dropdown_item);
	    
	    
        /** Change the action bar navigation to List Navigation and set the home icon to display an arrow **/

	    actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_LIST);
	    actionBar.setListNavigationCallbacks(arrayAdapter, ActivityRemoteFileTransfer.this);
		actionBar.setSelectedNavigationItem(0);				
		actionBar.setDisplayHomeAsUpEnabled(true);
		
		
        /** Initialize two views that display text based on the current List Navigation selection **/

		sendrecieveTextView = (TextView)
				findViewById(R.id.sendrecieveTextView);
		
		sendfileButton = (Button)
				findViewById(R.id.sendfileButton);
		
		
        /** Set an OnClickListener to the buttons **/

		View.OnClickListener clickListener = new View.OnClickListener() 
		{
			
			public void onClick(View view) 
			{
				
				//vibrator.vibrate(vibratedurationInteger);

				switch (view.getId()) 
				{
				
					case R.id.choosefileButton:
						
						
				        	/** Check what type of file transfer the user has selected **/

							if(actionBar.getSelectedNavigationIndex() == 0)
							{
	
							    /** Create and show a dialog that displays all of the local files on the user's device **/
								
								localfileDialog = getLocalFileDialog(new General().getDeviceExternalStorageDirectoryList(LOCAL_DIRECTORY));
								localfileDialog.show();

							}
	
							else
							{

						        /** If the user selects Pi to Device transfer check and see if the Connection and SFTP Client is null **/

								if(sftpClient != null && connection != null)
								{
										
								    /** Create and show a dialog that displays all of the remote files on the user's device
								      * NOTE: We must run this operation away from the main thread because it uses network connections **/
	
									new GetComputerDirectoryList().execute(REMOTE_DIRECTORY.toString());
										
								}
								
								else
								{
									
							        /** Show an error Toast if the SFTP Client is null **/
		
									Toast.makeText(ActivityRemoteFileTransfer.this.getApplicationContext(), 
											getResources().getString(R.string.toast_noconnection), Toast.LENGTH_LONG).show();
									
								}
								
							}

						break;
						
						
					case R.id.sendfileButton:
						
						
			        	/** Check what type of file transfer the user has selected **/

						if(actionBar.getSelectedNavigationIndex() == 0)
						{
										
							
				        	/** If the user selects Pi to Device transfer make sure the user has selected a file **/
							
							if(LOCAL_PENDING_FILE != null)
							{
								
								
					        	/** Check if the Connection is null **/

								if(connection != null)
								{
									
									/** Attempt to send the selected file to the remote computer **/
	
									new SendFileToRaspberryPi().execute();
									
								}
									
								else
								{
										
									/** Show an error Toast if the Connection is null **/
	
									Toast.makeText(ActivityRemoteFileTransfer.this.getApplicationContext(), 
											getResources().getString(R.string.toast_noconnection), Toast.LENGTH_LONG).show();
										
								}
									
							}
								
							else
							{
									
								/** Show an error Toast if the user did not select a file to upload **/

								Toast.makeText(ActivityRemoteFileTransfer.this.getApplicationContext(), 
										getResources().getString(R.string.toast_nofileselected), Toast.LENGTH_LONG).show();
									
							}
								
						}							
						
						else
						{
							
				        	/** If the user selects Device to Pi transfer make sure the user has selected a file **/
							
							if(REMOTE_PENDING_FILE != null)
							{
								
								
					        	/** Check if the Connection is null **/

								if(connection != null)
								{
										
									/** Attempt to download the selected file from remote computer **/

									new DownloadFileFromRaspberryPi().execute();
									
								}
								
								else
								{
									
							        /** Show an error Toast if the Connection is null **/

									Toast.makeText(ActivityRemoteFileTransfer.this.getApplicationContext(), 
											getResources().getString(R.string.toast_noconnection), Toast.LENGTH_LONG).show();
									
								}
	
								
							}
							
							else
							{
								
								/** Show an error Toast if the user did not select a file to download **/

								Toast.makeText(ActivityRemoteFileTransfer.this.getApplicationContext(), 
										getResources().getString(R.string.toast_nofileselected), Toast.LENGTH_LONG).show();
								
							}

						}
							
						break;	 

				}
				
			}
			
		};

		findViewById(R.id.choosefileButton)
			.setOnClickListener(clickListener);
		
		findViewById(R.id.sendfileButton)
			.setOnClickListener(clickListener);

	}
	

	@Override
	public boolean onNavigationItemSelected(int itemPosition, long itemId) 
	{
		
		switch(itemPosition)
		{
		
			case 0:
				
		        /** Change the text of two views to correspond with the transfer selected **/
							
				sendrecieveTextView.setText("Send file");
				
				sendfileButton.setText("Send file");
				
				break;
				
				
			case 1:
				
		        /** Change the text of two views to correspond with the transfer selected **/
								
				sendrecieveTextView.setText("Download file");
				
				sendfileButton.setText("Download file");
				
				break;
				
		}
		
		return false;
		
	}
	
	
	@Override
	 public boolean onCreateOptionsMenu(Menu menu) 
	 {
		 
        /** Inflate the menu for the Action Bar **/
		
	    MenuInflater menuInflater = getSupportMenuInflater();
	    menuInflater.inflate(R.menu.menu_remoteterminal, menu);
	    
	    return true;
	    
	 }


	@Override
	public boolean onOptionsItemSelected(MenuItem item) 
	{

		switch (item.getItemId()) 
		{

			case R.id.help: 
				
		        /** Show a simple help dialog **/
								
				new Dialogs().createInformationDialog(ActivityRemoteFileTransfer.this, 
						"Help", ActivityRemoteFileTransfer.this.getResources().getString(R.string.dialog_information_remotefiletransfer), true).show();

				return true;
				
				
			case R.id.reconnect:
				
		        /** Attempt to connect to the Pi with a parameter to show the connection dialog if credentials are blank **/
				
				connectToPi(true);

				return true;

				
			case R.id.preferences:
				
		        /** Launch an Intent to the Preferences class **/
				
				final Intent preferencesIntent = new Intent(ActivityRemoteFileTransfer.this,
						ActivityPreferences.class);
				startActivity(preferencesIntent);

				return true;	
				
				
			case R.id.piinformation:
				
		        /** Show a Dialog that collects the users login information **/
				
				final Dialog dialog = new Dialogs()
					.createRaspberryPiInformationDialog(ActivityRemoteFileTransfer.this);
				dialog.show();
				
				return true;
				
				
			case android.R.id.home:
				
		        /** Finish this Activity if the user presses the home icon **/
				
				ActivityRemoteFileTransfer.this.finish();
				
				return true;
				
				
			default:
					
				return super.onOptionsItemSelected(item);

		}
		
	} 
	
	
	@Override
	protected void onDestroy() 
	{
		super.onDestroy();
		
        /** When this Activity is destroyed clean up the Session and the SFTP Channel**/

		if(connection != null)
		{
			
			connection.close();
			connection = null;
			
		}
		
		if(sftpClient != null)
		{
			
			sftpClient.close();
			sftpClient = null;
			
		}
		
	}

	
	@Override
	public void setData(final KnownHosts databaseKnownHosts, final String hostnameString, final String hostkeyalgorithmString, final byte[] hostkeyByteArray,
			final String hostverificationString) 
	{
		
		this.databaseKnownHosts = databaseKnownHosts;
		this.hostnameString = hostnameString;
		this.hostkeyalgorithmString = hostkeyalgorithmString;
		this.hostkeyByteArray = hostkeyByteArray;
		this.hostverificationString = hostverificationString;
		
	}

	 
	private void connectToPi(final boolean showdialogBoolean)
	{
		
        /** Check and see if the login credentials are filled, if not than show the credentials dialog, if yes than connect
         * to the pi **/

		if(new General().areLogonCredentialsFilled(ActivityRemoteFileTransfer.this))
		{

			new ConnectToRaspberryPi().execute();
			
		}
		
		else
		{			
			
			if(showdialogBoolean)
			{
				
				final Dialog dialog = new Dialogs()
					.createRaspberryPiInformationDialog(ActivityRemoteFileTransfer.this);
				dialog.show();
		
			
				dialog.setOnDismissListener(new OnDismissListener()
				{

					@Override
					public void onDismiss(DialogInterface dialog) 
					{
	
						connectToPi(false);
						
					}				
				
				});
				
			}

			Toast.makeText(ActivityRemoteFileTransfer.this.getApplicationContext(), 
					getResources().getString(R.string.toast_blanklogincredentials), Toast.LENGTH_SHORT).show();

		}
		
	}
	
	
	public Dialog getLocalFileDialog(List<Option> list)
	{
		
    	final Dialog dialog = new Dialog(ActivityRemoteFileTransfer.this);

    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_list);
    	dialog.setCancelable(false); 
    	
    	final TextView textView = (TextView)
    			dialog.findViewById(R.id.titleTextView);
    	
    	textView.setText("Local files");
    	
    	localfileArrayAdapter = new FileArrayAdapter
       		 (ActivityRemoteFileTransfer.this, R.layout.file_view, list);
    	
    	dialog.setOnKeyListener(new OnKeyListener()
    	{

			@Override
			public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) 
			{
				
				if (event.getAction()!=KeyEvent.ACTION_DOWN)
	            {

					if (keyCode == KeyEvent.KEYCODE_BACK)
				    {
						
						if (localdirectoryStack.size() == 0)
						{
								
							dialog.dismiss();
								
						}
							
						else
						{
							
							localfileDialog.dismiss();
							
							LOCAL_DIRECTORY = localdirectoryStack.pop();
							
							localfileDialog = getLocalFileDialog(new General().getDeviceExternalStorageDirectoryList(LOCAL_DIRECTORY));
							localfileDialog.show();
									
						 }
						
						return true;
								
				    }
								
				}
				
				return false;
											
			}
    		
    	}); 
    	
    	
    	final ListView listView = (ListView)
    			dialog.findViewById(R.id.itemListView);
		
    	listView.setAdapter(localfileArrayAdapter);
    	
    	listView.setOnItemClickListener(new OnItemClickListener()
		{

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int position, long arg3)
			{

				Option option = localfileArrayAdapter.getItem(position);

				
				if(option.getData().equalsIgnoreCase("folder"))
				{
					
					localfileDialog.dismiss();
					localdirectoryStack.push(LOCAL_DIRECTORY);	
					LOCAL_DIRECTORY = new File(option.getPath());
					
					localfileDialog = getLocalFileDialog(new General().getDeviceExternalStorageDirectoryList(LOCAL_DIRECTORY));
					localfileDialog.show();
				
				}
				
				else if(option.getData().equalsIgnoreCase("parent directory"))
				{			
					
					if (localdirectoryStack.size() != 0)
					{
						
						localfileDialog.dismiss();

						LOCAL_DIRECTORY = localdirectoryStack.pop();
						
						localfileDialog = getLocalFileDialog(new General().getDeviceExternalStorageDirectoryList(LOCAL_DIRECTORY));
						localfileDialog.show();
						
					}
					
					else
					{
						
//						Toast.makeText(getActivity().getApplicationContext(), 
//								TOAST_NOPARENTDIRECTORY, Toast.LENGTH_LONG).show();
						
					}

				}
				
				else
				{
					
					LOCAL_PENDING_FILE = new File(option.getPath());

					TextView filenameTextView = (TextView)
							findViewById(R.id.filenameTextView);
										
					String filenameString = option.getName();
				    
					filenameTextView.setText(filenameString);

					dialog.dismiss();
								
				}

				
			}

		});
  			
		return dialog;
		
	}
	
	
	public Dialog getRemoteFileDialog(List<Option> list)
	{
		
    	final Dialog dialog = new Dialog(ActivityRemoteFileTransfer.this);

    	dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dialog.setContentView(R.layout.dialog_list);
    	dialog.setCancelable(false);   
    	
    	final TextView textView = (TextView)
    			dialog.findViewById(R.id.titleTextView);
    	
    	textView.setText("Raspberry Pi files");
    	
        remotefileArrayAdapter = new FileArrayAdapter
       		 (ActivityRemoteFileTransfer.this, R.layout.file_view, list);
    	
    	dialog.setOnKeyListener(new OnKeyListener()
    	{

			@Override
			public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) 
			{
				
				if (event.getAction()!=KeyEvent.ACTION_DOWN)
	            {

					if (keyCode == KeyEvent.KEYCODE_BACK)
				    {
						
						if (remotedirectoryStack.size() == 0)
						{
								
							dialog.dismiss();
								
						}
							
						else
						{
								
							REMOTE_DIRECTORY = remotedirectoryStack.pop();
							
							new GetComputerDirectoryList().execute(REMOTE_DIRECTORY.getAbsolutePath());
									
						 }
						
						return true;
								
				    }
								
				}
				
				return false;
											
			}
    		
    	}); 
    	
    	
    	final ListView listView = (ListView)
    			dialog.findViewById(R.id.itemListView);
		
    	listView.setAdapter(remotefileArrayAdapter);
    	
    	listView.setOnItemClickListener(new OnItemClickListener()
		{

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int position, long arg3)
			{

				Option option = remotefileArrayAdapter.getItem(position);

				
				if(option.getData().equalsIgnoreCase("folder"))
				{
					
					remotedirectoryStack.push(REMOTE_DIRECTORY);
					REMOTE_DIRECTORY = new File(REMOTE_DIRECTORY + "/" + option.getName());
										
					new GetComputerDirectoryList().execute(REMOTE_DIRECTORY.getAbsolutePath());
				
				}
				
				else
				{
					
					REMOTE_PENDING_FILE = new File(REMOTE_DIRECTORY + "/" + option.getName());

					TextView filenameTextView = (TextView)
							findViewById(R.id.filenameTextView);
										
					String filenameString = option.getName();
				    
					filenameTextView.setText(filenameString);

					dialog.dismiss();
								
				}

				
			}

		});
  			
		return dialog;
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//TODO Connect to Raspberry Pi ASyncTask
	private class ConnectToRaspberryPi extends AsyncTask<Void, Void, String> 
    {
				
	    private ProgressDialog progressDialog;
	    
	    
	    
		@Override
		protected void onPreExecute() 
		{

			progressDialog = new ProgressDialog(ActivityRemoteFileTransfer.this);
			progressDialog.setIndeterminate(true);
			progressDialog.setCancelable(false);
			progressDialog.setTitle("Connecting");
			progressDialog.setMessage("Attempting to connect...");
			progressDialog.show();

		}


		@Override
		protected String doInBackground(Void... params)
		{
			
			
			/** Create a new KnownHosts database **/
			
			final KnownHosts databaseKnownHosts = new KnownHosts();
			
			
			/** Get a reference to a file with saved hosts on it **/
	
			final File knownhostsFile = new File
					(ActivityRemoteFileTransfer.this.getFilesDir(), "known_hosts");

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
							ActivityRemoteFileTransfer.this;
					
					
					/** Get the necessary log-in credentials **/
					
					final String username = preferenceHolder
							.getUsername(activityContext);
					
					final String password = preferenceHolder
							.getUserPassword(activityContext);
	
					final String ipaddress = preferenceHolder
							.getIPAddress(activityContext);
						
						
					/** Set-up a new connection passing the host as a parameter **/

					connection = new Connection(ipaddress);
					
	
					/** Connect to the host verifying the host key **/					
					
					connection.connect(new SimpleVerifier(ActivityRemoteFileTransfer.this, databaseKnownHosts));
					
					
					/** Authenticate the login credentials **/
	
					final boolean isAuthenticated = connection
							.authenticateWithPassword(username, password);
	
						if (!isAuthenticated)
						{
							
			    			return CONNECTION_ERROR;
											
						}
						
				    sftpClient = new SFTPv3Client(connection);
						
		    		return CONNECTED;
									
				} 
			
				catch (Exception exception)
				{
										
					
	    			return CONNECTION_ERROR;
					
				}
	        	
		}
		

		@Override
		protected void onPostExecute(String result)
		{
			
			progressDialog.cancel();

			if(result.equals(CONNECTED))
			{
				
				Toast.makeText(ActivityRemoteFileTransfer.this.getApplicationContext(), 
						getResources().getString(R.string.toast_connected), Toast.LENGTH_SHORT).show();			
				
			}

			else
			{
				
				if(hostverificationString != null && databaseKnownHosts != null && hostnameString != null 
						&& hostkeyalgorithmString != null && hostkeyByteArray != null)
				{

					if(hostverificationString.equals(SimpleVerifier.HOST_NEW))
					{
						
						Dialog dialog = new Dialogs().createHostKeyNewDialog(ActivityRemoteFileTransfer.this, databaseKnownHosts,
								hostnameString, hostkeyalgorithmString, hostkeyByteArray);
						
						dialog.setOnCancelListener(new OnCancelListener()
						{

							@Override
							public void onCancel(DialogInterface dialog) 
							{
								
								new ConnectToRaspberryPi().execute();							
								
							}

						});					
						
						dialog.show();	
						
					}
					
					else if(hostverificationString.equals(SimpleVerifier.HOST_KEY_CHANGED))
					{
						
						Dialog dialog = new Dialogs().createHostKeyChangedDialog(ActivityRemoteFileTransfer.this, databaseKnownHosts,
								hostnameString, hostkeyalgorithmString, hostkeyByteArray);
						
						dialog.setOnCancelListener(new OnCancelListener()
						{

							@Override
							public void onCancel(DialogInterface dialog) 
							{

								new ConnectToRaspberryPi().execute();													
								
							}

						});					
						
						dialog.show();	

					}
					
					else
					{
						
						final Dialog dialog = new Dialogs().createInformationConfirmDialog(ActivityRemoteFileTransfer.this, 
								"General error", getResources().getString(R.string.error_general), true);
						
						dialog.show();
						
					}

				}
				
				else
				{
					
					final Dialog dialog = new Dialogs().createInformationConfirmDialog(ActivityRemoteFileTransfer.this, 
							"Connection error", getResources().getString(R.string.error_connection), true);
					
					dialog.show();
					
				}

			}
			
		}
		
	}
	
	
	
	private class SendFileToRaspberryPi extends AsyncTask<String, Integer, String> 
    {
				
	    private ProgressDialog progressDialog;

		@Override
		protected void onPreExecute() 
		{
			
			progressDialog = new ProgressDialog(ActivityRemoteFileTransfer.this);
			progressDialog.setIndeterminate(true);
			progressDialog.setCancelable(false);
			progressDialog.setTitle("Sending");
			progressDialog.setMessage("Attempting to send file...");
			progressDialog.show();
			
		}


		@Override
		protected String doInBackground(String... command) 
		{
				
			if(connection != null)
			{			

					try
					{
						
						final SCPClient scpClient = new SCPClient(connection);
						
						scpClient.put(LOCAL_PENDING_FILE.getPath(), "");
																
						return (LOCAL_PENDING_FILE.getName()) + " sent to Pi!";
						
					}
					
					catch (Exception e) 
					{

						Log.e("TAGAGEGEGE", e.toString());
						
						return ("File error...");

					}
					
//					catch (IOException e) 
//					{
//
//						return ("File error...");
//
//					}
					
			}
			
			else
			{
			
				return "Connection error!";
			
			}
	        	
		}
		

		@Override
		protected void onPostExecute(String result)
		{
			
			progressDialog.cancel();
			
			Toast.makeText(ActivityRemoteFileTransfer.this.getApplicationContext(), 
					String.valueOf(result), Toast.LENGTH_LONG).show();
			
		}
		
	}
	
	
	
	private class GetComputerDirectoryList extends AsyncTask<String, Void, List<Option>> 
    {
				
	    ProgressDialog progressDialog;
	    
	  	    
		@Override
		protected void onPreExecute() 
		{

			progressDialog = new ProgressDialog(ActivityRemoteFileTransfer.this);
			progressDialog.setIndeterminate(true);
			progressDialog.setCancelable(false);
			progressDialog.setTitle("Connecting");
			progressDialog.setMessage("Getting directory list...");
			progressDialog.show();
			
			if(remotefileDialog != null)
			{
				
				if(remotefileDialog.isShowing())
				{
					
					remotefileDialog.dismiss();
					
				}				
				
			}
			
		}


		@Override
		protected List<Option> doInBackground(String... directory)
		{
						
		    List<Option> directoryList = new ArrayList<Option>();

		    List<Option> fileList = new ArrayList<Option>();
			  
		        
	    	if(sftpClient != null)
	        {

	    		try 
	    		{
	    						    				
	    			Vector<?> vector = sftpClient.ls(directory[0]);
	    				
    			 	for(Object object: vector)
    			 	{		    			 			
    			 		
    			 		if(object instanceof SFTPv3DirectoryEntry)    			 			
    					{
    			 			
        			 		
        			 		SFTPv3DirectoryEntry file = (SFTPv3DirectoryEntry) object;
    			 				
    			 				if(file.attributes.isDirectory())
    			 				{
    			 					
	    					 		  directoryList.add(new Option(file.filename.toString(), "Folder", 
	    					 				 file.longEntry.toString())); 
    			 					
    			 				}
    			 				
    			 				else
    			 				{
    			 					
	    			            	  fileList.add(new Option(file.filename.toString(), "File Size: "+ file.attributes.size.toString(),
	    			            			  file.longEntry.toString()));
	    			            	  
		    			 		}

    					 	}

		    			}
			    			 		
	    				Collections.sort(directoryList);
	    				Collections.sort(fileList);
	    				 	
	    				directoryList.addAll(fileList);
 
		    			 		
  					 	return directoryList;		    			 		

		    			}
	    		
	    		
	    				catch (IOException e)
		    			{
			    			
		    				return null;

						}	
	        	
		        }
		        
		        else 
		        {
		        	
	    			return null;
		        	
		        }
        	
		}
		

		@Override
		protected void onPostExecute(List<Option> result)
		{
			
			progressDialog.cancel();
			
			if (result != null)
			{
				
				remotefileDialog = getRemoteFileDialog(result);
				
				remotefileDialog.show();
				
			}
			
			else
			{
				
				Toast.makeText(ActivityRemoteFileTransfer.this.getApplicationContext(), 
						"Connection error!", Toast.LENGTH_LONG).show();
				
			}
						
		}
		
	}
	
	
	private class DownloadFileFromRaspberryPi extends AsyncTask<Void, Integer, String> 
    {
				
	    private ProgressDialog progressDialog;

		@Override
		protected void onPreExecute() 
		{
			
			progressDialog = new ProgressDialog(ActivityRemoteFileTransfer.this);
			progressDialog.setIndeterminate(true);
			progressDialog.setCancelable(false);
			progressDialog.setTitle("Downloading");
			progressDialog.setMessage("Attempting to download file...");
			progressDialog.show();
			
		}


		@Override
		protected String doInBackground(Void... command) 
		{
				
			if(connection != null)
			{			
		
				try 
				{
					
					final SCPClient scpClient = new SCPClient(connection);
					
					scpClient.get(REMOTE_PENDING_FILE.getPath(), Environment.getExternalStorageDirectory().getPath());
					
					return (REMOTE_DIRECTORY.getName()) + " downloaded from Pi!";

				} 
					
				catch (Exception e) 
				{
						
					return "Connection error!";
						
				}            

								
			}
			
			else
			{
			
				return "Connection error!";
			
			}
	        	
		}
		

		@Override
		protected void onPostExecute(String result)
		{
			
			progressDialog.cancel();
			
			Toast.makeText(ActivityRemoteFileTransfer.this.getApplicationContext(), 
					String.valueOf(result), Toast.LENGTH_LONG).show();
			
		}
		
	}
	
	
	
	
	
	
	

}
