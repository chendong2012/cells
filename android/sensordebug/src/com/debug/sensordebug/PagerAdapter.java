package com.debug.sensordebug;

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
    			return ("Sensor Info"); 	
    			
    		case 1:
    			return ("ALS Config");
    			
    		case 2:
    			return ("PS Config");

    		case 3: 			
    			return ("Tutorials");
    			
    		case 4:
    			return ("Remote functions");
    	
	    	default:	    		
	    		return "Error";
    	
    	}
    	
    	
    }
    
}
