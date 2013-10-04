package com.raspdroid.helper;

import java.util.List;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

public class PagerAdapter extends FragmentPagerAdapter {

	private List<Fragment> fragments;

	public PagerAdapter(FragmentManager fm, List<Fragment> fragments) {

		super(fm);
		this.fragments = fragments;

	}

	@Override
	public Fragment getItem(int position) {

		return this.fragments.get(position);

	}

	@Override
	public int getCount() {

		return this.fragments.size();

	}
	
    @Override
    public CharSequence getPageTitle(int position)
    {
    	
    	switch(position)
    	{
    	
    		case 0:
    			
    			return ("Calculations");
    			
    			
    		case 1:
    			
    			return ("GPIO");
    			
    			
    		case 2:
    			
    			return ("Software");
    			
    			
    		case 3:
    			
    			return ("Tutorials");
    			
    			
    		case 4:
    			
    			return ("Remote functions");
    			

	    	default:
	    		
	    		return "Error";
    	
    	}
    	
    	
    }
    
}
