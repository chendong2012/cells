package com.raspdroid.helper;

import android.content.Context;
import ch.ethz.ssh2.KnownHosts;
import ch.ethz.ssh2.ServerHostKeyVerifier;

public class SimpleVerifier implements ServerHostKeyVerifier
{

	public static String HOST_KEY_MATCHES = "hostKeyMatches";
	public static String HOST_NEW = "hostNew";
	public static String HOST_KEY_CHANGED = "hostKeyChanged";	
	
	private HostDataCallback hostDataCallback = dummyHostDataCallback;
	
	private KnownHosts database;
	
	
	public interface HostDataCallback
	{

		public void setData(KnownHosts database, String hostname, String serverHostKeyAlgorithm, byte[] serverHostKey,
				String hostverification);
		
	}
	
	
	public SimpleVerifier(final Context context, final  KnownHosts database)
	{
		
		if (database == null)
		{
			
			throw new IllegalArgumentException();
			
		}

		this.database = database;	
		
		hostDataCallback = (HostDataCallback) context;
		
	}

	public boolean verifyServerHostKey(String hostname, int port, String serverHostKeyAlgorithm, byte[] serverHostKey)
			throws Exception
	{
		int result = database.verifyHostkey(hostname, serverHostKeyAlgorithm, serverHostKey);

		switch (result)
		{
		
			case KnownHosts.HOSTKEY_IS_OK:
				
					try
					{
						
						hostDataCallback.setData(database, hostname, serverHostKeyAlgorithm, serverHostKey, HOST_KEY_MATCHES);
						
					}
					
					catch(Exception exception)
					{
						
						// Do nothing
						
					}
				
				return true; 

				
			case KnownHosts.HOSTKEY_IS_NEW:
				
					try
					{
												
						hostDataCallback.setData(database, hostname, serverHostKeyAlgorithm, serverHostKey, HOST_NEW);	
												
					}
					
					catch(Exception exception)
					{
						
						// Do nothing
						
					}
				
				return false;
				

			case KnownHosts.HOSTKEY_HAS_CHANGED:
	
				hostDataCallback.setData(database, hostname, serverHostKeyAlgorithm, serverHostKey, HOST_KEY_CHANGED);		

				return false;

		default:
			throw new IllegalStateException();
		}
	}
	
	
	private static HostDataCallback dummyHostDataCallback= new HostDataCallback() 
	{

		@Override
		public void setData(KnownHosts database, String hostname,
				String serverHostKeyAlgorithm, byte[] serverHostKey, String hostverification) {
			// TODO Auto-generated method stub
			
		}
	};
	
}