package com.debug.sensordebug;


import com.debug.sensordebug.fragments.*;
import com.debug.sensordebug.PagerAdapter;
import java.util.List;
import java.util.Vector;

import android.os.Bundle;
import android.support.v4.app.Fragment;

import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.view.View;
import android.view.ViewGroup.LayoutParams;

import com.actionbarsherlock.app.ActionBar;
import com.actionbarsherlock.app.SherlockFragmentActivity;

import com.jeremyfeinstein.slidingmenu.lib.SlidingMenu;
import com.jeremyfeinstein.slidingmenu.lib.app.SlidingActivityBase;
import com.viewpagerindicator.TitlePageIndicator;
import com.viewpagerindicator.TitlePageIndicator.IndicatorStyle;



public class ActivityMainScreen extends SherlockFragmentActivity implements SlidingActivityBase
{
	 @Override
	 protected void onCreate(Bundle savedInstanceState) 
	 {
       super.onCreate(savedInstanceState);
       setContentView(R.layout.activity_main_screen);

       /** Create a list and fill it with Fragments **/

		List<Fragment> fragmentList = new Vector<Fragment>();
		
		fragmentList.add(Fragment.instantiate(this,
				FragmentSensorInfo.class.getName()));	
		
		fragmentList.add(Fragment.instantiate(this,
				FragmentALS.class.getName()));
		
		fragmentList.add(Fragment.instantiate(this,
				FragmentPS.class.getName()));
		
	
		
		
       /** Create a new Pager Adapter and place the fragment list in it **/

	    final PagerAdapter pagerAdapter = new PagerAdapter(super.getSupportFragmentManager(), fragmentList);
	    
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
	    actionBar.setTitle(getResources().getString(R.string.app_name));
		actionBar.setBackgroundDrawable(getResources()
				.getDrawable(R.drawable.background_tiledwhitelines));
		
       /** Get a reference to the TitlePageIndicator and modify it's appearance **/
		
		final TitlePageIndicator titlePageIndicator = (TitlePageIndicator)
				findViewById(R.id.titles);
		
		titlePageIndicator.setViewPager(viewPager);
		titlePageIndicator.setCurrentItem(0);
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
					default:
					break;				
				}
			}   	
       });
	}

	
	@Override
	public void setBehindContentView(View view, LayoutParams layoutParams) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setBehindContentView(View view) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setBehindContentView(int layoutResID) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public SlidingMenu getSlidingMenu() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void toggle() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void showContent() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void showMenu() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void showSecondaryMenu() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setSlidingActionBarEnabled(boolean slidingActionBarEnabled) {
		// TODO Auto-generated method stub
		
	}













}

