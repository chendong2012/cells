package com.raspdroid.helper;

import android.app.Application;

public class RaspdroidApplication extends Application
{
	
	private int GPIO_IMAGE_INDEX;
	
	
	public void setGPIOImageIndex(final int imageindexInteger)
	{
		
		GPIO_IMAGE_INDEX = imageindexInteger;
		
	}
	
	
	public int getGPIOImageIndex()
	{
		
		return GPIO_IMAGE_INDEX;
		
	}
	
	
	
	

}
