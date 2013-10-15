package com.test.sshtest;

import android.content.Context;
import ch.ethz.ssh2.KnownHosts;
import ch.ethz.ssh2.ServerHostKeyVerifier;

/*实现:ch.ethz.ssh2.ServerHostKeyVerifier 接口,然后会交给
 * ch.ethz.ssh2.Connection.connect(ServerHostKeyVerifier)的参数来用;
 * 
 * 
 * */
public class SimpleVerifier implements ServerHostKeyVerifier
{
	public static String HOST_KEY_MATCHES = "hostKeyMatches";
	public static String HOST_NEW = "hostNew";
	public static String HOST_KEY_CHANGED = "hostKeyChanged";	
	
	private HostDataCallback hostDataCallback = dummyHostDataCallback;
	
	private KnownHosts database;
	
	/*定义这个接口的目的是什么？（类里面的接口）,表示我有数据给你，你去实现好先，我会调你的
	 * 实际上，是想让Activity类去实现这个接口的，通过这样的方式可以把这个类里面的变量给到Activity类去
	 * 
	 * 提供一个接口出来，就意味着，我有数据要传给你，这也是一种数据传递的方法
	 * 你这里有数据，你就可以提供一个接口出来,让别人去实现，你调它给你的实现接口就可以了
	 * 
	 * 就比如一个插座，里面有点，我就提供一个插电的座子给你，你就可以取电了
	 * 
	 * 接口是虚的，
	 * 主动者给接口
	 * *********做好接口后，让其它类去实现，并把实现好的接口给我，然后给我调用****同时就完成了数据的传递
	 * */
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
/*这个方法是由SSH＠包里面来实现的*/
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