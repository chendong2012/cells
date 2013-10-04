/**    
* Copyright 2013, John Persano
* This project includes software developed by John Persano and Dustin Evans.
* 
* http://code.google.com/p/raspdroid/
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


package com.raspdroid.fragments;


import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.actionbarsherlock.app.SherlockListFragment;
import com.raspdroid.R;
import com.raspdroid.helper.Dialogs;

public class FragmentSlideMenu extends SherlockListFragment
{
	
	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
	  super.onCreate(savedInstanceState);
	  
      /** Populate the List Fragment with String Array **/
	  	  
      ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String> (getActivity(),
              android.R.layout.simple_list_item_1, getResources().getStringArray(R.array.slidemenuoptions));
      
	  setListAdapter(arrayAdapter);
	  
	}

	@Override
	public void onListItemClick(ListView listView, View view, int position, long id) 
	{
		
		switch(position)
		{
		
			case 0:
				
			    /** Send an email to raspdroid@gmail.com **/
				
				final Intent sendsuggestionsIntent  = new Intent(Intent.ACTION_SEND);
				sendsuggestionsIntent.setType("message/rfc822");
				sendsuggestionsIntent.putExtra(Intent.EXTRA_EMAIL, new String[] {"raspdroid@gmail.com"});
				sendsuggestionsIntent.putExtra(Intent.EXTRA_SUBJECT, "Suggestion");
				
					try 
					{
						
					    startActivity(Intent.createChooser(sendsuggestionsIntent, "Send suggestion by..."));
					    
					} 
					
					catch (ActivityNotFoundException activityNotFoundException)
					{
						
					    Toast.makeText(getActivity(), 
					    		"Error finding e-mail client", Toast.LENGTH_SHORT).show();
					    
					}
				
				break;
		
		
			case 1:
				
			    /** Launch an Intent to the main project page **/

                final Intent sourcecodeIntent = new Intent
        			(Intent.ACTION_VIEW, Uri.parse("http://code.google.com/p/raspdroid/"));
                startActivity(sourcecodeIntent);		
                
				break;
				
				
			case 2:
				
			    /** Grab the third party libraries String Array and populate a List Dialog with it **/

				final String[] thirdpartylibrariesStringArray = getActivity()
				.getResources().getStringArray(R.array.thirdpartylibraries);
				
				new Dialogs().librariesused(getActivity(), 
				true, thirdpartylibrariesStringArray, "Third party libraries").show();
				
				break;
				
				
			case 3:
				
			    /** Grab the contributors String Array and populate a List Dialog with it **/

				final String[] contributorsStringArray = getActivity()
					.getResources().getStringArray(R.array.contributors);
				
				new Dialogs().contributors(getActivity(), 
						true, contributorsStringArray, "Contributors").show();
				
				break;
			
			case 4:
				
				/** External Links **/
				
				break;
				
			case 5:
				
			    /** Launch an Intent to the technical help page **/

                final Intent technicalfaqIntent = new Intent
                		(Intent.ACTION_VIEW, Uri.parse("http://code.google.com/p/raspdroid/wiki/Technical_Help"));
                startActivity(technicalfaqIntent);
                
				break;	
				
			case 6:
				
			    /** Launch an Intent to the general faq page **/

                final Intent generalfaqIntent = new Intent
        			(Intent.ACTION_VIEW, Uri.parse("http://code.google.com/p/raspdroid/wiki/General_FAQ"));
                startActivity(generalfaqIntent);	
                
				break;
		}
	}
}
