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

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import ch.ethz.ssh2.Connection;
import ch.ethz.ssh2.KnownHosts;
import ch.ethz.ssh2.Session;
import ch.ethz.ssh2.StreamGobbler;

import com.actionbarsherlock.app.ActionBar;
import com.actionbarsherlock.app.SherlockFragmentActivity;
import com.actionbarsherlock.app.ActionBar.OnNavigationListener;
import com.actionbarsherlock.view.Menu;
import com.actionbarsherlock.view.MenuInflater;
import com.actionbarsherlock.view.MenuItem;
import com.raspdroid.helper.Dialogs;
import com.raspdroid.helper.General;
import com.raspdroid.helper.PreferenceHolder;
import com.raspdroid.helper.SimpleVerifier;
import com.raspdroid.helper.SimpleVerifier.HostDataCallback;

import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnCancelListener;
import android.content.DialogInterface.OnDismissListener;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Vibrator;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;


public class ActivityRemoteTerminal extends SherlockFragmentActivity implements OnNavigationListener, HostDataCallback
{

	
	public static final String CONNECTION_ERROR = "connectionError";
	public static final String CONNECTED = "connected";
	public static final String USER_CANCELLED = "userCancelled";

	
	private boolean USER_IS_SUPERUSER;
			
	
	private Connection connection;
		
	private TextView usersessionTextView;	
	
	
	private KnownHosts databaseKnownHosts;
	
	private String hostnameString, hostkeyalgorithmString, hostverificationString; 
	
	private byte[] hostkeyByteArray;
	
	private ConnectToRaspberryPi connectToRaspberryPi;


	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_remoteterminal);
        
         
        /** Create a Vibrator object **/
        
		final Vibrator vibrator = (Vibrator) 
				getSystemService(Context.VIBRATOR_SERVICE);
		
		
        /** Return 25 milliseconds if the user has enabled haptic feedback via Preferences, else 0 **/

		final int vibratedurationInteger = new PreferenceHolder()
			.getVibrateDuration(ActivityRemoteTerminal.this);

		
        /** Attempt to connect to the remote computer passing a 'true' parameter to display a dialog if 
         *  the user has bad credentials **/

		connectToPi(true);
		
		
        /** Get a reference to the Action Bar and disable showing the application's title **/
			
	    final ActionBar actionBar = getSupportActionBar();
	    actionBar.setDisplayShowTitleEnabled(false);
	    
	    
        /** Get the current Context of the Action Bar **/

	    final Context absContext = actionBar.getThemedContext();
	    
	    
        /** Set up an Array Adapter to pass to the Action Bar's List Navigation **/
	    
	    final ArrayAdapter<CharSequence> arrayAdapter = ArrayAdapter
	    		.createFromResource(absContext, R.array.sessionselection, R.layout.sherlock_spinner_item);
	    
	    arrayAdapter.setDropDownViewResource(R.layout.sherlock_spinner_dropdown_item);
	    
	    
        /** Set the Action Bar's navigation mode to List Navigation and display an arrow next to the home icon **/

	    actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_LIST);
	    actionBar.setListNavigationCallbacks(arrayAdapter, ActivityRemoteTerminal.this);
		actionBar.setSelectedNavigationItem(0);				
		actionBar.setDisplayHomeAsUpEnabled(true);
		
		
        /** Initialize the user session TextView, this will display either a '$' or a '#' depending on the List Navigation selection**/

		usersessionTextView = (TextView)
				findViewById(R.id.usersessionTextView);
		
		
        /** Initialize the command EditText. This is where the user will enter commands **/

		final AutoCompleteTextView commandlineEditText = (AutoCompleteTextView)
			findViewById(R.id.commandlineEditText);
		
		final String[] commandsStringArray = getResources()
				.getStringArray(R.array.terminalcommands);
		
		final ArrayAdapter<String> commandsArrayAdapter = new ArrayAdapter<String>(ActivityRemoteTerminal.this, 
				android.R.layout.simple_list_item_1, commandsStringArray);
		
		commandlineEditText.setAdapter(commandsArrayAdapter);

		
        /** Initialize the command Button. This is what the user will press to send a command **/

        final Button commandButton = (Button)
        		findViewById(R.id.commandButton);
        
        
        /** Set the OnClick event for the command button **/

        commandButton.setOnClickListener(new OnClickListener()
        {

			@Override
			public void onClick(View view) 
			{
				
		        /** Vibrate the users device for the integer retrieved before **/

				vibrator.vibrate(vibratedurationInteger);
				
				
		        /** Get the String entered by the user, check if it is empty, if it is not empty than send the command **/

				final String commandString = commandlineEditText.getText().toString();
						
					if(!commandString.equals(""))
					{
								
						new SendCommandToRaspberryPi().execute(commandString);
	
								
					}
							
					else
					{
								
						Toast.makeText(ActivityRemoteTerminal.this.getApplicationContext(), 
								getResources().getString(R.string.toast_commandblank), Toast.LENGTH_LONG).show();
								
					}
				
			}

        });
	
	}
	 
	 
	 @Override
	 public boolean onCreateOptionsMenu(Menu menu) 
	 {
		 
		 
	    /** Inflate the options menu in the Action Bar **/
		 
	    MenuInflater inflater = getSupportMenuInflater();
	    inflater.inflate(R.menu.menu_remoteterminal, menu);
	    
	    return true;
	    
	 }


	@Override
	public boolean onOptionsItemSelected(MenuItem item) 
	{

		switch (item.getItemId()) 
		{

			case R.id.help:
				
				
			    /** Show a dialog with some help information **/
								
				new Dialogs().createInformationDialog(ActivityRemoteTerminal.this, 
						"Help", ActivityRemoteTerminal.this.getResources().getString(R.string.dialog_information_remoteterminal), true).show();

				return true;
				
				
			case R.id.reconnect:
				
				
			    /** Attempt a connection with the remote device passing a 'true' parameter to show a dialog if the users credentials are bad **/
				
				connectToPi(true);

				return true;

				
			case R.id.preferences:
				
				
			    /** Navigate to the Preference Activity **/
				
				final Intent preferencesIntent = new Intent(ActivityRemoteTerminal.this,
						ActivityPreferences.class);
				startActivity(preferencesIntent);

				return true;	
				
				
			case R.id.piinformation:
				
				
			    /** Show a dialog with editable fields for the desired connection **/
				
				Dialog dialog = new Dialogs()
					.createRaspberryPiInformationDialog(ActivityRemoteTerminal.this);
				dialog.show();
				
				return true;
				
				
			case android.R.id.home:
				
				
			    /** When the user presses the icon in the top right corner close this Activity **/

				ActivityRemoteTerminal.this.finish();
				
				return true;
				
				
			default:
					
				return super.onOptionsItemSelected(item);

		}
		
	} 
	 
	
	@Override
	protected void onDestroy() 
	{
		super.onDestroy();
		
		
	    	/** Check if the connection is null, if the connection is not null than close the connection and null it for memory resources **/
		
			if(connection != null)
			{
				
				connection.close();
				connection = null;
				
			}
		
		
	}
	
	
	@Override
	public void onBackPressed()
	{
		super.onBackPressed();
		
		
	    /** Show an animation when the user presses the back button **/

		overridePendingTransition(R.anim.translate_rightin,R.anim.translate_rightout);
		
		
			if(connectToRaspberryPi != null)
			{
								
				connectToRaspberryPi.cancel(true);
				
			}

	}

	
	@Override
	protected void onPause()
	{
		
		super.onPause();
		
			if(connectToRaspberryPi != null)
			{
								
				connectToRaspberryPi.cancel(true);
				
			}
		
	}


	@Override
	public boolean onNavigationItemSelected(int itemPosition, long itemId) 
	{
		
		switch(itemPosition)
		{
		
			case 0:
			
				USER_IS_SUPERUSER = false;
				
				usersessionTextView.setText("$");
				
				break;
				
				
			case 1:
				
				USER_IS_SUPERUSER = true;
				
				new Dialogs().createInformationConfirmDialog(ActivityRemoteTerminal.this, 
						"WARNING!", getResources().getString(R.string.dialog_information_superuserwarning), true).show();
				
				usersessionTextView.setText("#");

				break;
				
				
			default:
				
				USER_IS_SUPERUSER = false;
				
				usersessionTextView.setText("$");

				break;
				
		}
		
		return false;
		
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

		if(new General().areLogonCredentialsFilled(ActivityRemoteTerminal.this))
		{

			connectToRaspberryPi = new ConnectToRaspberryPi();
			connectToRaspberryPi.execute();
			
		}
		
		else
		{			
			
			if(showdialogBoolean)
			{
				
				final Dialog dialog = new Dialogs()
					.createRaspberryPiInformationDialog(ActivityRemoteTerminal.this);
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

			Toast.makeText(ActivityRemoteTerminal.this.getApplicationContext(), 
					getResources().getString(R.string.toast_blanklogincredentials), Toast.LENGTH_SHORT).show();

		}
		
	}
	
	
	
	private class ConnectToRaspberryPi extends AsyncTask<Void, Void, String> 
    {


		private ProgressDialog progressDialog;
	    
	    
	    
		@Override
		protected void onPreExecute() 
		{

			progressDialog = new ProgressDialog(ActivityRemoteTerminal.this);
			progressDialog.setIndeterminate(true);
			progressDialog.setCancelable(true);
			progressDialog.setTitle("Connecting");
			progressDialog.setMessage("Attempting to connect...");
			progressDialog.show();
			
			progressDialog.setOnCancelListener(new OnCancelListener()
			{

				@Override
				public void onCancel(DialogInterface dialog) 
				{

					cancel(true);

				}

			});
			

		}

	    
		@Override
		protected String doInBackground(Void... params)
		{
			
			
			/** Create a new KnownHosts database **/
			
			final KnownHosts databaseKnownHosts = new KnownHosts();
			
			
			/** Get a reference to a file with saved hosts on it **/
	
			final File knownhostsFile = new File
					(ActivityRemoteTerminal.this.getFilesDir(), "known_hosts");
		
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
							ActivityRemoteTerminal.this;
					
					
					/** Get the necessary log-in credentials **/
					
					final String username = preferenceHolder
							.getUsername(activityContext);
					
					final String password = preferenceHolder
							.getUserPassword(activityContext);
	
					final String ipaddress = preferenceHolder
							.getIPAddress(activityContext);
					
					
						if(this.isCancelled())
						{
							
							return USER_CANCELLED;
							
						}
						
						
					/** Set-up a new connection passing the host as a parameter **/

					connection = new Connection(ipaddress);
					
						if(this.isCancelled())
						{
							
							return USER_CANCELLED;
							
						}
						
					/** Connect to the host verifying the host key **/					
					
					connection.connect(new SimpleVerifier(ActivityRemoteTerminal.this, databaseKnownHosts));
					
						if(this.isCancelled())
						{
							
							return USER_CANCELLED;
							
						}
					
					
					/** Authenticate the login credentials **/
	
					final boolean isAuthenticated = connection
							.authenticateWithPassword(username, password);
	
						if (!isAuthenticated)
						{
							
			    			return CONNECTION_ERROR;
											
						}						
						
		    		return CONNECTED;
									
				} 
			
				catch (Exception exception)
				{
					
	    			return CONNECTION_ERROR;
					
				}

		}
		

		@Override
		protected void onCancelled() 
		{
			super.onCancelled();
			
			Toast.makeText(ActivityRemoteTerminal.this.getApplicationContext(), 
					getResources().getString(R.string.toast_connectioncanceled), Toast.LENGTH_SHORT).show();	
			
		}


		@Override
		protected void onPostExecute(String result)
		{
			
			progressDialog.cancel();

			if(result.equals(CONNECTED))
			{
				
				Toast.makeText(ActivityRemoteTerminal.this.getApplicationContext(), 
						getResources().getString(R.string.toast_connected), Toast.LENGTH_SHORT).show();			
				
			}

			else
			{
				
				if(hostverificationString != null && databaseKnownHosts != null && hostnameString != null 
						&& hostkeyalgorithmString != null && hostkeyByteArray != null)
				{

					if(hostverificationString.equals(SimpleVerifier.HOST_NEW))
					{
						
						Dialog dialog = new Dialogs().createHostKeyNewDialog(ActivityRemoteTerminal.this, databaseKnownHosts,
								hostnameString, hostkeyalgorithmString, hostkeyByteArray);
						
						dialog.setOnCancelListener(new OnCancelListener()
						{

							@Override
							public void onCancel(DialogInterface dialog) 
							{
								
								connectToPi(false);
								
							}

						});					
						
						dialog.show();	
						
					}
					
					else if(hostverificationString.equals(SimpleVerifier.HOST_KEY_CHANGED))
					{
						
						Dialog dialog = new Dialogs().createHostKeyChangedDialog(ActivityRemoteTerminal.this, databaseKnownHosts,
								hostnameString, hostkeyalgorithmString, hostkeyByteArray);
						
						dialog.setOnCancelListener(new OnCancelListener()
						{

							@Override
							public void onCancel(DialogInterface dialog) 
							{

								connectToPi(false);
								
							}

						});					
						
						dialog.show();	

					}
					
					else
					{
						
						final Dialog dialog = new Dialogs().createInformationConfirmDialog(ActivityRemoteTerminal.this, 
								"General error", getResources().getString(R.string.error_general), true);
						
						dialog.show();
						
					}

				}
				
				else
				{
					
					final Dialog dialog = new Dialogs().createInformationConfirmDialog(ActivityRemoteTerminal.this, 
							"Connection error", getResources().getString(R.string.error_connection), true);
					
					dialog.show();
					
				}

			}
			
		}
		
	}
	
	
	
	private class SendCommandToRaspberryPi extends AsyncTask<String, Void, String> 
    {
				
	    ProgressDialog progressDialog;


		@Override
		protected void onPreExecute() 
		{
			
			progressDialog = new ProgressDialog(ActivityRemoteTerminal.this);
			progressDialog.setIndeterminate(true);
			progressDialog.setCancelable(false);
			progressDialog.setTitle("Sending command...");
			progressDialog.show();

		}


		@Override
		protected String doInBackground(String... command) 
		{

			try
			{
				
				
				/** Create a session **/
				
				Session session = connection.openSession();
						
					if(USER_IS_SUPERUSER)
					{
	
						session.execCommand("sudo -S -p '' "+ command[0]);
							
						OutputStream outputStream = new BufferedOutputStream(session.getStdin(), 8192);
						
						outputStream.write((new PreferenceHolder()
					   		.getUserPassword(ActivityRemoteTerminal.this) + "\n").getBytes());
					   
					    outputStream.flush();
					   
					    outputStream.close();
					   
					}
						
					else
					{
							
						session.execCommand(command[0]);
							
					}
	
						
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
		protected void onPostExecute(String result)
		{
			
			progressDialog.cancel();

			final TextView outputTextView = (TextView)
					findViewById(R.id.outputTextView);
	        
	        outputTextView.setText(result);
	        			
			
		}
		
	}




	
}





