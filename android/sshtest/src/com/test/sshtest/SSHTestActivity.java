package com.test.sshtest;

import java.io.File;

import android.app.Activity;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
import ch.ethz.ssh2.Connection;
import ch.ethz.ssh2.KnownHosts;

import com.test.sshtest.SimpleVerifier.HostDataCallback;

public class SSHTestActivity extends Activity implements HostDataCallback {
	
private String hostnameString, hostkeyalgorithmString, hostverificationString;
private KnownHosts databaseKnownHosts;
private byte[] hostkeyByteArray;
public Connection connection1;
public static final String CONNECTION_ERROR = "connectionError";
public static final String CONNECTED = "connected";
public static final String USER_CANCELLED = "userCancelled";

Button mButton;
void showToast(CharSequence msg) {
	Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mButton=(Button)findViewById(R.id.button1);
		mButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				ConnectToRaspberryPi connectToRaspberryPi = new ConnectToRaspberryPi();
				connectToRaspberryPi.execute();
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	private class ConnectToRaspberryPi extends AsyncTask<Void, Void, String> 
    {
		private ProgressDialog progressDialog;
		
		@Override
		protected void onPreExecute() 
		{
			/*建立对话框一定要基于一个存在视图*/
			progressDialog = new ProgressDialog(SSHTestActivity.this);
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
					(SSHTestActivity.this.getFilesDir(), "known_hosts");
			
				try 
				{
					/** If the file exists load the known hosts into memory **/
					if (knownhostsFile.exists())
					{
						databaseKnownHosts.addHostkeys(knownhostsFile);			
					}
					/** Get the Activity's Context **/

					/** Get the necessary log-in credentials **/
					
					final String username = "xbian";					
					final String password = "raspberry";
					final String ipaddress = "mountain.eicp.net";
						if(this.isCancelled())
						{
							return USER_CANCELLED;
						}
					/** Set-up a new connection passing the host as a parameter **/

					Connection connection = new Connection(ipaddress);
					
						if(this.isCancelled())
						{
							return USER_CANCELLED;
						}
						
					/** Connect to the host verifying the host key **/					
					
					connection.connect(new SimpleVerifier(SSHTestActivity.this, databaseKnownHosts));
				//	connection.connect();
			
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
			
			Toast.makeText(SSHTestActivity.this.getApplicationContext(), 
					"Cancelled", Toast.LENGTH_SHORT).show();
		}
		
		@Override
		protected void onPostExecute(String result)
		{
			progressDialog.cancel();
			if(result.equals(CONNECTED))
			{		
				Toast.makeText(SSHTestActivity.this.getApplicationContext(), 
						CONNECTED, Toast.LENGTH_SHORT).show();
			}
			else
			{
				if(hostverificationString != null && databaseKnownHosts != null && hostnameString != null 
						&& hostkeyalgorithmString != null && hostkeyByteArray != null)
				{
					if(hostverificationString.equals(SimpleVerifier.HOST_NEW))
					{
						
						Dialog dialog = new Dialogs().createHostKeyNewDialog(SSHTestActivity.this, databaseKnownHosts,
								hostnameString, hostkeyalgorithmString, hostkeyByteArray);
						
						dialog.setOnCancelListener(new OnCancelListener()
						{

							@Override
							public void onCancel(DialogInterface dialog) 
							{
								
								//connectToPi(false);
								
							}

						});					
						
						dialog.show();	
						
						
					}
					else if(hostverificationString.equals(SimpleVerifier.HOST_KEY_CHANGED))
					{
						
						Dialog dialog = new Dialogs().createHostKeyChangedDialog(SSHTestActivity.this, databaseKnownHosts,
								hostnameString, hostkeyalgorithmString, hostkeyByteArray);
						dialog.setOnCancelListener(new OnCancelListener()
						{
							@Override
							public void onCancel(DialogInterface dialog) 
							{
								//connectToPi(false);
							}
						});					
						
						dialog.show();	
						
					}
					else
					{
						
						final Dialog dialog = new Dialogs().createInformationConfirmDialog(SSHTestActivity.this, 
								"General error", getResources().getString(R.string.error_general), true);
						dialog.show();
						
					}
				}
				else
				{
					
					final Dialog dialog = new Dialogs().createInformationConfirmDialog(SSHTestActivity.this, 
							"Connection error", getResources().getString(R.string.error_connection), true);
					
					dialog.show();
					
				}
			}
		}
	}

	@Override
	public void setData(final KnownHosts databaseKnownHosts, final String hostnameString, 
			final String hostkeyalgorithmString, final byte[] hostkeyByteArray,
			final String hostverificationString)  {
		this.databaseKnownHosts = databaseKnownHosts;
		this.hostnameString = hostnameString;
		this.hostkeyalgorithmString = hostkeyalgorithmString;
		this.hostkeyByteArray = hostkeyByteArray;
		this.hostverificationString = hostverificationString;
	}
}
