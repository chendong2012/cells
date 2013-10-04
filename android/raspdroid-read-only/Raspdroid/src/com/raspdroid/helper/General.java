package com.raspdroid.helper;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import android.content.Context;

public class General 
{
	
	public List<Option> getDeviceExternalStorageDirectoryList(final File directoryFile)
	{
		
		final File[] fileArray = directoryFile
				.listFiles();	
		
	    List<Option> directoryList = new ArrayList<Option>();
	        
		List<Option> filesList = new ArrayList<Option>(); 	 
		
			for(File file: fileArray)
		 	{
		 			
				if(file.isDirectory())
				{
				 		  
					directoryList.add(new Option
							(file.getName(),"Folder",  file.getAbsolutePath()));
				 		
				}
				 	  
		        else
		        {
		            	  
		        	filesList.add(new Option
		        			(file.getName(),"File Size: "+ file.length(), file.getAbsolutePath()));
		                	
		        }
			 	  
		 	}
		    
	 	Collections.sort(directoryList);
	 	Collections.sort(filesList);
	 	
	 	directoryList.addAll(filesList);
	 	
		 	if(!directoryFile.getName().equalsIgnoreCase("sdcard"))
		 	{
		 		
		 		directoryList.add(0,new Option
		 				("...", "Parent Directory", directoryFile.getParent()));
	
		 	}
		 	
		 	
		return directoryList;    

	}
	
	
	public boolean areLogonCredentialsFilled(final Context context)
	{
		
		final PreferenceHolder preferenceHolder = 
				new PreferenceHolder();
		

		final String usernameString = preferenceHolder
				.getUsername(context);
		
		final String passwordString = preferenceHolder
				.getUserPassword(context);
		
		final String ipaddressString = preferenceHolder
				.getIPAddress(context);
		
		final String portString = preferenceHolder
				.getPort(context);

			
			if(!usernameString.equals("") && !passwordString.equals("") && 
					!ipaddressString.equals("") && !portString.equals(""))
			{
				
				return true;
								
			}
			
			else
			{
				
				return false;
				
			}

	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

}
