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

import java.util.List;
import java.util.Vector;

import ch.ethz.ssh2.KnownHosts;

import com.actionbarsherlock.app.ActionBar;
import com.actionbarsherlock.app.SherlockFragmentActivity;
import com.actionbarsherlock.view.Menu;
import com.actionbarsherlock.view.MenuInflater;
import com.actionbarsherlock.view.MenuItem;
import com.raspdroid.fragments.FragmentCalculations;
import com.raspdroid.fragments.FragmentGPIO;
import com.raspdroid.fragments.FragmentRemote;
import com.raspdroid.fragments.FragmentSlideMenu;
import com.raspdroid.fragments.FragmentSoftware;
import com.raspdroid.fragments.FragmentTutorials;
import com.raspdroid.helper.Dialogs;
import com.raspdroid.helper.PagerAdapter;
import com.raspdroid.helper.PreferenceHolder;
import com.raspdroid.helper.SimpleVerifier.HostDataCallback;
import com.jeremyfeinstein.slidingmenu.lib.SlidingMenu;
import com.jeremyfeinstein.slidingmenu.lib.app.SlidingActivityBase;
import com.viewpagerindicator.TitlePageIndicator;
import com.viewpagerindicator.TitlePageIndicator.IndicatorStyle;

import android.app.Dialog;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;


public class ActivityMainScreen extends SherlockFragmentActivity implements SlidingActivityBase, HostDataCallback
{ 
	 private SlidingMenu slidingMenu;	 
	 private KnownHosts databaseKnownHosts;
	 private String hostnameString, hostkeyalgorithmString, hostverificationString;
	 private byte[] hostkeyByteArray;
	 @Override
	 protected void onCreate(Bundle savedInstanceState) 
	 {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_screen);
        
        /** Create a new Sliding Menu object **/

		slidingMenu = new SlidingMenu
				(ActivityMainScreen.this);

        /** Check if R.id.slidingmenu_frame exists (it will only exist when the user has a large screen and is in landscape orientation 
         *  as specified in layout-large-land). If it does not exist than the user's device has a regular size screen or is in portrait mode
         *  so we will create a dummy View to inflate for the Sliding Menu and disable it because the menu will already be showing **/

			if (findViewById(R.id.slidingmenu_frame) == null) 
			{
	        	/** Here there is no View with an ID of (R.id.slidingmenu_frame) so we can conclude
	        	 *  that we are not using a large screen device in landscape mode because the layout in layout-large-land is the only version 
	        	 *  of ActivityMainScreen that has this View by default. Since (R.id.slidingmenu_frame) doesn't exist we will point the Sliding Menu
	        	 *  to set the Menu to a different layout that does have that ID (R.layout.menu_frame)  **/

				slidingMenu.setMenu(R.layout.menu_frame);
				slidingMenu.setSlidingEnabled(true);
				slidingMenu.setTouchModeAbove(SlidingMenu.TOUCHMODE_MARGIN);
			} 
			
			else 
			{
		        /** Here there IS a View with the ID of (R.id.slidingmenu_frame) since it is NOT null so we ARE using a large screen 
		         *  device in landscape orientation. We will give the Sliding Menu a dummy (empty) view to inflate since the Menu 
		         *  will already be showing in the layout via the next command where we attach the Fragment **/

				View dummyView = new View(this);
				slidingMenu.setMenu(dummyView);			
				slidingMenu.setSlidingEnabled(false);
				slidingMenu.setTouchModeAbove(SlidingMenu.TOUCHMODE_NONE);
			}
		
	    /** Place the menu Fragment in the frame provided by the above code or XML **/

		getSupportFragmentManager().beginTransaction().replace
			(R.id.slidingmenu_frame, new FragmentSlideMenu()).commit();
		
        /** Modify the Sliding Menu to make it look nice **/

		slidingMenu.attachToActivity(this, SlidingMenu.SLIDING_CONTENT);
	    slidingMenu.setShadowDrawable(R.drawable.shadow);
		slidingMenu.setShadowWidthRes(R.dimen.slidingmenu_shadowwidth);
		slidingMenu.setBehindOffsetRes(R.dimen.slidingmenu_offset);
		slidingMenu.setFadeDegree(0.35f);

        /** Check if Preferences are set to keep the device's screen on **/

        handleWakeLock();
		
        /** Create a list and fill it with Fragments **/

		List<Fragment> fragmentList = new Vector<Fragment>();

		fragmentList.add(Fragment.instantiate(this,
			FragmentCalculations.class.getName()));

		fragmentList.add(Fragment.instantiate(this,
			FragmentGPIO.class.getName()));

		fragmentList.add(Fragment.instantiate(this,
			FragmentSoftware.class.getName()));
		
		fragmentList.add(Fragment.instantiate(this,
				FragmentTutorials.class.getName()));
		
		fragmentList.add(Fragment.instantiate(this,
				FragmentRemote.class.getName()));
		
        /** Create a new Pager Adapter and place the fragment list in it **/

	    final PagerAdapter pagerAdapter = new PagerAdapter
	    	(super.getSupportFragmentManager(), fragmentList);
	    
        /** Get the density multiplier of the current screen  **/
        
        final float density = getResources()
        		.getDisplayMetrics().density;
        
        /** Initialize the ViewPager **/
        
		final ViewPager viewPager = (ViewPager)
				findViewById(R.id.pager);
		
        /** Set the Adapter of the ViewPager to the Adapter we created earlier **/
		
		viewPager.setAdapter(pagerAdapter);
		
        /** Get a reference to the ActionBar, change the title and background **/
		
	    final ActionBar actionBar = getSupportActionBar();
	    actionBar.setTitle(getResources().getString(R.string.application_name));
		actionBar.setBackgroundDrawable(getResources()
				.getDrawable(R.drawable.background_tiledwhitelines));
		
        /** Get a reference to the TitlePageIndicator and modify it's appearance **/
		
		final TitlePageIndicator titlePageIndicator = (TitlePageIndicator)
				findViewById(R.id.titles);
		
		titlePageIndicator.setViewPager(viewPager);
		titlePageIndicator.setCurrentItem(2);
        titlePageIndicator.setFooterColor(0xFF33B5E5);
        titlePageIndicator.setFooterLineHeight(1 * density);
        titlePageIndicator.setFooterIndicatorHeight(3 * density); 
        titlePageIndicator.setFooterIndicatorStyle(IndicatorStyle.Triangle);
        titlePageIndicator.setSelectedColor(0xFF33B5E5);
        titlePageIndicator.setTextColor(0xFF000000);
        titlePageIndicator.setSelectedBold(false);
		
        /** Set an OnPageListener to the TitlePageIndicator to listen for page changes **/

        titlePageIndicator.setOnPageChangeListener(new OnPageChangeListener()
        {
        	
			@Override
			public void onPageScrollStateChanged(int arg0) 
			{
				// Do nothing
			}
			
			@Override
			public void onPageScrolled(int arg0, float arg1, int arg2)
			{
				
				// Do nothing
				
			}
			
			@Override
			public void onPageSelected(int pagenumberInteger) 
			{
				
		        /** When the page changes we will check and see if it is the first (0) page, if it is than
		         *  we will make it easier to access the Sliding Menu by setting it's touchmode to fullscreen
		         *  instead of the very edge of the screen **/

				switch(pagenumberInteger)
				{
					case 0:
						
				        /** Sliding finger anywhere on the page opens the Sliding Menu, not a good idea unless we are
				         *  on the page that is all the way to the left **/

						slidingMenu.setTouchModeAbove(SlidingMenu.TOUCHMODE_FULLSCREEN);
						break;
						
					default:
						
				        /** Sliding finger from the left side of the screen will open the Sliding Menu, this is good for everywhere 
				         *  except the leftmost page where we want the Menu easier to open **/

						slidingMenu.setTouchModeAbove(SlidingMenu.TOUCHMODE_MARGIN);
						break;				
				}
			}   	
        });
	}
	 
	 @Override
	 public boolean onCreateOptionsMenu(Menu menu) 
	 {	 
	    /** Inflate the Menu in the Action Bar **/
		 
	    MenuInflater inflater = getSupportMenuInflater();
	    inflater.inflate(R.menu.menu_main, menu);
	    
	    return true;
	 }

	@Override
	public boolean onOptionsItemSelected(MenuItem item) 
	{
        /** Listen for selections in the ActionBar menu**/
		switch (item.getItemId()) 
		{
        	/** Opens the Preferences screen with an Intent  **/
			case R.id.preferences:
				
				//vibrator.vibrate(getVibrateDuration());
				
				Intent intentPreferenceFragment = new Intent(ActivityMainScreen.this.getApplicationContext(), 
						ActivityPreferences.class);
				startActivity(intentPreferenceFragment);
				return true;
				
			case R.id.piinformation:
				
			    /** Show a dialog with editable fields for the desired connection **/
				Dialog dialog = new Dialogs()
					.createRaspberryPiInformationDialog(ActivityMainScreen.this);
				dialog.show();
				return true;
				
			default:
				return super.onOptionsItemSelected(item);
		}
	} 
	 
	@Override
	protected void onResume()
	{
		super.onResume();
        /** Check if Preferences are set to keep the device's screen on **/
	    handleWakeLock();
	}
	
	private void handleWakeLock()
	{
    	/** Get a reference to the Window object  **/

		final Window window = getWindow();

    	/** If Preferences are set to keep screen on than add the screen on flag to the current window  **/
		
		if(new PreferenceHolder().getWakeLockBoolean(ActivityMainScreen.this) == true)
		{
			window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
			
	    	/** If Preferences are set to notify the user that the screen will stay on than send the Toast **/

			if(new PreferenceHolder().getWakeLockToastBoolean(ActivityMainScreen.this) == true)
			{
				Toast.makeText(ActivityMainScreen.this.getApplicationContext(), getResources().getString(R.string.toast_wakelock), 
						Toast.LENGTH_LONG).show();
			}
		}
		
		else
		{
	    	/** If not than clear the possibly existing flag  **/

			window.clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);	
		}
	}
	
	/** Below are methods that are necessary for implementing SlidingActivityBase, we will not use any of them  **/

	@Override
	public void setBehindContentView(View view, LayoutParams layoutParams) 
	{

		// Do nothing
		
	}


	@Override
	public void setBehindContentView(View view)
	{

		// Do nothing
		
	}


	@Override
	public void setBehindContentView(int layoutResID) 
	{
		
		// Do nothing
		
	}


	@Override
	public SlidingMenu getSlidingMenu() 
	{
		
		// Do nothing

		return null;
		
	}


	@Override
	public void toggle() 
	{

		// Do nothing
		
	}


	@Override
	public void showContent() 
	{

		// Do nothing
		
	}


	@Override
	public void showMenu()
	{

		// Do nothing
		
	}


	@Override
	public void showSecondaryMenu() 
	{

		// Do nothing
		
	}


	@Override
	public void setSlidingActionBarEnabled(boolean slidingActionBarEnabled)
	{

		// Do nothing
		
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
	
	
	public KnownHosts getKnownHostsDatabase()
	{
		
		if(databaseKnownHosts != null)
		{
			
			return databaseKnownHosts;
			
		}
		
		else
		{
			
			return null;
			
		}
		
	}
	
	public String getHostNameString()
	{
		
		if(hostnameString != null)
		{
			
			if(!hostnameString.equals(""))
			{
				
				return hostnameString;
				
			}
			
			else
			{
				
				return null;
				
			}
			
		}
		
		else
		{
			
			return null;
			
		}
				
	}
	
	
	public String getHostKeyAlgorithmString()
	{
		
		if(hostkeyalgorithmString != null)
		{
			
			if(!hostkeyalgorithmString.equals(""))
			{
				
				return hostkeyalgorithmString;
				
			}
			
			else
			{
				
				return null;
				
			}
			
		}
		
		else
		{
			
			return null;
			
		}
				
	}
	
	
	public String getHostVerificationString()
	{
		
		if(hostverificationString != null)
		{
			
			if(!hostverificationString.equals(""))
			{
				
				return hostverificationString;
				
			}
			
			else
			{
				
				return null;
				
			}
			
		}
		
		else
		{
			
			return null;
			
		}
				
	}
	
	
	public byte[] getHostKeyByteArray()
	{
		
		if(hostkeyByteArray != null)
		{

			return hostkeyByteArray;
			
		}
		
		else
		{
			
			return null;	
		}			
	}	
}