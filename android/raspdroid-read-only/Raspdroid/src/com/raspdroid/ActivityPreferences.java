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


import com.actionbarsherlock.app.ActionBar;
import com.actionbarsherlock.app.SherlockPreferenceActivity;
import com.actionbarsherlock.view.MenuItem;
import com.raspdroid.R;
import android.os.Bundle;


public class ActivityPreferences extends SherlockPreferenceActivity
{

	/** Here we suppress depreciation warnings because the new Preference Fragment that took over is not compatible below 4.0 **/

	
	@SuppressWarnings("deprecation")
	@Override
    public void onCreate(Bundle savedInstanceState) 
	{
        super.onCreate(savedInstanceState);
        
    	/** Change the background to a Drawable **/

        getWindow().setBackgroundDrawableResource
		        	(R.drawable.background_tiledwhitelines); 
        
        
    	/** Get a reference to the Action Bar and set the Home icon as a back button **/

	    final ActionBar actionBar = getSupportActionBar();
	    actionBar.setDisplayHomeAsUpEnabled(true);
	    
	    
    	/** Add preferences from a file specified in XML **/
        
        addPreferencesFromResource(R.xml.preferences_main);    
        
    }
	
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		
    	/** Check if the option selected was the Home icon, if yes than exit the Activity **/
		
	    if (item.getItemId() == android.R.id.home) 
	    {
	    	
	    	ActivityPreferences.this.finish();
	        
	        return true;
	    }
	    
	    return super.onOptionsItemSelected(item);
	    
	}
	

}

