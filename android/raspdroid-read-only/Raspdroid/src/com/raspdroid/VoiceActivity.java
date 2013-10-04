package com.raspdroid;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;
import java.util.concurrent.ExecutionException;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

import android.app.Dialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.speech.RecognizerIntent;
import android.speech.tts.TextToSpeech;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.util.Base64;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.Toast;

import com.raspdroid.helper.Dialogs;
import com.raspdroid.helper.PreferenceHolder;

public class VoiceActivity extends FragmentActivity implements TextToSpeech.OnInitListener {
	public static final String STRING_WAKELOCKTOAST = "Wake lock has been turned on";
	private static final int VR_REQUEST = 999;
  	public PreferenceHolder preferenceHolder;
  	SectionsPagerAdapter mSectionsPagerAdapter;
  	private TextToSpeech tts;
	ViewPager mViewPager;
	String pin;
	String ipaddress = "";
	String result = "";
	boolean speak;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_voice_main);
		 tts = new TextToSpeech(this, this);
		// Create the adapter that will return a fragment for each of the three
		// primary sections of the app.
		mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());		
		// Set up the ViewPager with the sections adapter.
		mViewPager = (ViewPager) findViewById(R.id.pager);
		mViewPager.setAdapter(mSectionsPagerAdapter);
		preferenceHolder = new PreferenceHolder();
		pin =  preferenceHolder.WEBIOPI_getPin(VoiceActivity.this);
		ipaddress = preferenceHolder.WEBIOPI_getIPAddress(VoiceActivity.this);
	}
	
	@Override
    public void onDestroy() {
        // Don't forget to shutdown tts!
        if (tts != null) {
            tts.stop();
            tts.shutdown();
        }
        super.onDestroy();
    }
	
 	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
 		Boolean goAuthorise;
	      if (requestCode == VR_REQUEST && resultCode == RESULT_OK) 
	      {
	          ArrayList<String> suggestedWords = data.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);
	          TextView tekst = (TextView) findViewById(R.id.section_label);
	          String slowo = (String) suggestedWords.get(0);
	          tekst.setTextSize(30);
	          tekst.setText(getString(R.string.yousaid) + ": " + slowo);
	          
	          String x = preferenceHolder.WEBIOPI_getUsername(VoiceActivity.this);
	          if (x != null) {
	        	  goAuthorise = true;
	          } else {
	        	  goAuthorise = false;
	          }
	        if(ipaddress.equals(""))
	        {
	        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(VoiceActivity.this, 
						"Connection Error", getResources().getString(R.string.error_connection), true);
	        	dialog.show();
	        }
	        else
	        {
	        //authorise, username, password, ipaddress, port, pin, function, out
			if (slowo.equals(getString(R.string.output))) 
			{
	        	  postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(VoiceActivity.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getPort(VoiceActivity.this), 
	        			  pin,
	        			  "function", "out");
	        }
			//authorise, username, password, ipaddress, port, pin, function, in
			else if (slowo.equals(getString(R.string.input))) 
			{
	        	  postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(VoiceActivity.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getPort(VoiceActivity.this), 
	        			  pin,
	        			  "function", "in");
	        }
			//authorise, username, password, ipaddress, port, pin, value, 1
			else if (slowo.equals(getString(R.string.open))) 
			{
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(VoiceActivity.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getPort(VoiceActivity.this), 
	        			  pin,
	        			  "value", "1");
	        }
			//authorise, username, password, ipaddress, port, pin, value, 1
			else if (slowo.equals(getString(R.string.on))) 
			{
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(VoiceActivity.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getPort(VoiceActivity.this), 
	        			  pin,
	        			  "value", "1");
	        }
			//authorise, username, password, ipaddress, port, pin, value, 0
			else if (slowo.equals(getString(R.string.close))) 
			{
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(VoiceActivity.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getPort(VoiceActivity.this), 
	        			  pin,
	        			  "value", "0");
	        }
			//authorise, username, password, ipaddress, port, pin, value, 0
			else if (slowo.equals(getString(R.string.off))) 
			{
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(VoiceActivity.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getPort(VoiceActivity.this), 
	        			  pin,
	        			  "value", "0");
	        }
			//authorise, username, password, ipaddress, port, pin, sequence, 150,10
			else if (slowo.equals(getString(R.string.sequence))) 
			{
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(VoiceActivity.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getPort(VoiceActivity.this), 
	        			  pin,
	        			  "sequence", "150,10");
	        }
			//authorise, username, password, ipaddress, port, pin, sequence, 150,10
			else if (slowo.equals(getString(R.string.magic))) 
			{
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(VoiceActivity.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getPort(VoiceActivity.this), 
	        			  pin,
	        			  "sequence", "150,10");
	        }
			else if(slowo.equals(getString(R.string.zero)))
			{
				pin = "0";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.one)))
			{
				pin = "1";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.four)))
			{
				pin = "4";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.seventeen)))
			{
				pin = "17";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.twentyone)))
			{
				pin = "21";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.twentytwo)))
			{
				pin = "22";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.ten)))
			{
				pin = "10";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.nine)))
			{
				pin = "9";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.eleven)))
			{
				pin = "11";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.eighteen)))
			{
				pin = "18";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.twentythree)))
			{
				pin = "23";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.twentyfour)))
			{
				pin = "24";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.twentyfive)))
			{
				pin = "25";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.eight)))
			{
				pin = "8";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.seven)))
			{
				pin = "7";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
			}
			else if(slowo.equals(getString(R.string.get)))
			{
				new getValueTask().execute();
			}
			else if(slowo.equals(getString(R.string.status)))
			{
				new getValueTask().execute();
			}
			//authorise, username, password, ipaddress, port, pin, pulse, ""
			else if (slowo.equals(getString(R.string.pulse))) 
			{
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(VoiceActivity.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(VoiceActivity.this), 
	        			  preferenceHolder.WEBIOPI_getPort(VoiceActivity.this), 
	        			  pin, 
	        	  "pulse", "");
	        } else 
	        {
	        	  tekst.setText((CharSequence) findViewById(R.string.error_word));
	        };
	      }
	      }
	      super.onActivityResult(requestCode, resultCode, data);
	}	
 	
	public void postData(Boolean authorise, String login, String pass, Boolean ssl, String IP, String port, String GPIO, String mode, String value) {
		// used ONLY to POST data, but it should be no problem to make it able to GET too
		// authorise : true/false (automagically is set to true if username is set in Preferences)
		// login : username configured in Webiopi 
		// pass : password of user configured in Webiopi
		// ssl : true/false
		// IP : ip address or a hostname of RPi (can be public or private address)
		// port : port number that Webiopi is running on
		// GPIO : GPIO pin number to control
		// mode : one of -> function, value, pulse, sequence, pulseRatio, pulseAngle
		// value : if mode =
		//					function 	-> in/out/pwm
		//					value 		-> 0/1
		//					pulse 		-> null
		//					sequence 	-> delay,sequence (numbers)
		//					pulseRatio 	-> ratio (number)
		//					pulseAngle 	-> angle (number)
		String h = null;
		HashMap<String, String> data = new HashMap<String, String>();
        VoiceAsyncPost asyncHttpPost = new VoiceAsyncPost(data);
        if (ssl == true) { h = "https://"; } else { h = "http://"; };
        
        VoiceAsyncPost.AUTH = authorise;
        VoiceAsyncPost.AUTH_DATA = asyncHttpPost.getB64Auth(login, pass);
        try {
			result = asyncHttpPost.execute(h + IP + ":" + port + "/GPIO/" + GPIO + "/" + mode + "/" + value).get();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
	}
    
	public void onClick(View v) {
		if (v.getId() == R.id.speech_btn) {
			//listen for results
			listenToSpeech();
		}
	}
		
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.voice_menu, menu);
		return true;
	}
	public class SectionsPagerAdapter extends FragmentPagerAdapter {

		public SectionsPagerAdapter(FragmentManager fm) {
			super(fm);
		}

		@Override
		public Fragment getItem(int position) {
			// getItem is called to instantiate the fragment for the given page.
			// Return a DummySectionFragment (defined as a static inner class
			// below) with the page number as its lone argument.
			Fragment fragment = new DummySectionFragment();
			Bundle args = new Bundle();
			args.putInt(DummySectionFragment.ARG_SECTION_NUMBER, position + 1);
			fragment.setArguments(args);
			return fragment;
		}

		@Override
		public int getCount() {
			// Show 3 total pages.
			return 1;
		}

		@Override
		public CharSequence getPageTitle(int position) {
			Locale l = Locale.getDefault();
			switch (position) {
			case 0:
				return getString(R.string.title_section1).toUpperCase(l);
			case 1:
				return getString(R.string.title_section2).toUpperCase(l);
			}
			return null;
		}
	}
	public static class DummySectionFragment extends Fragment {
		public static final String ARG_SECTION_NUMBER = "section_number";

		public DummySectionFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.voice_fragment_main_dummy,container, false);
//			TextView dummyTextView = (TextView) rootView.findViewById(R.id.section_label);
//			dummyTextView.setText(Integer.toString(getArguments().getInt(ARG_SECTION_NUMBER)));
			return rootView;
		}
	}
	private void listenToSpeech() {
	  	//start the speech recognition intent passing required data
	  	Intent listenIntent = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
	  	//indicate package
	  	listenIntent.putExtra(RecognizerIntent.EXTRA_CALLING_PACKAGE, getClass().getPackage().getName());
	  	//message to display while listening
	  	listenIntent.putExtra(RecognizerIntent.EXTRA_PROMPT, getString(R.string.sayaword));
	  	//set speech model
	  	listenIntent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);
	  	//specify number of results to retrieve
	  	listenIntent.putExtra(RecognizerIntent.EXTRA_MAX_RESULTS, 1);
	  	//start listening
	    startActivityForResult(listenIntent, VR_REQUEST);
	}

	@Override
	public boolean onOptionsItemSelected(android.view.MenuItem item) {
		switch (item.getItemId()) {
			case R.id.settings_option_item:
				Dialog dialog = new Dialogs()
					.webiopiDialog(this);
				dialog.show();
				break;
			case R.id.list_controls:
				Dialog dialog_control = new Dialogs()
					.show_options(this);
				dialog_control.show();
				break;
			case R.id.useVoice:
				preferenceHolder.saveVoice(VoiceActivity.this, true);
				speak = preferenceHolder.getVoice(VoiceActivity.this);
				break;
			case R.id.muteVoice:
				preferenceHolder.saveVoice(VoiceActivity.this, false);
				speak = preferenceHolder.getVoice(VoiceActivity.this);
				break;
			default:
				return super.onOptionsItemSelected(item);
		}
		return true;
	}
	
	@Override
	protected void onResume()
	{
		super.onResume();
	    handleWakeLock();
	}

	private void handleWakeLock()
	{
		if (preferenceHolder != null)
		{
			final boolean wakelockBoolean = preferenceHolder
					.getWakeLockBoolean(VoiceActivity.this);
			if(wakelockBoolean == true)
			{
				final Window window = getWindow(); 
				window.setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, 0);	
				final boolean wakelocktoastBoolean = preferenceHolder
						.getWakeLockToastBoolean(VoiceActivity.this);
				if(wakelocktoastBoolean == true)
				{
					Toast.makeText(VoiceActivity.this.getApplicationContext(), STRING_WAKELOCKTOAST, 
							Toast.LENGTH_LONG).show();					
				}
			}
		}
	}
	
	//////////////TASK FOR GETTING THE VALUE OF THE PIN/////////////////////////////////////////////
	public class getValueTask extends AsyncTask<String, String, String> {
	
	@Override
	protected String doInBackground(String... params) {
		BufferedReader in = null;
		String data = null;
		boolean ssl = false;
		String h;
		if (ssl == true) { h = "https://"; } else { h = "http://"; };
		String ip = preferenceHolder.WEBIOPI_getIPAddress(VoiceActivity.this);
		String port = preferenceHolder.WEBIOPI_getPort(VoiceActivity.this);
		String user = preferenceHolder.WEBIOPI_getUsername(VoiceActivity.this);
		String password = preferenceHolder.WEBIOPI_getUserPassword(VoiceActivity.this);
		String pin = preferenceHolder.WEBIOPI_getPin(VoiceActivity.this);
		try{
	  	HttpClient client = new DefaultHttpClient();
	  	String getURL = h + ip + ":" + port + "/GPIO/" + pin + "/" + "value";
	    HttpGet get = new HttpGet(getURL);
	    String source=user+":"+password;
	    String ret="Basic "+Base64.encodeToString(source.getBytes(),Base64.URL_SAFE|Base64.NO_WRAP);
	    get.setHeader("Authorization", ret);
	    HttpResponse responseGet = client.execute(get);  
	    HttpEntity resEntityGet = responseGet.getEntity();  
	    if (resEntityGet != null) {  
	        // do something with the response
	        result = EntityUtils.toString(resEntityGet);
	    }
	  	return data;
		} catch (ClientProtocolException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		finally
		{
			if(in != null)
			{
				try
				{
					in.close();
					return data;
				}
				catch(Exception e)
				{
					e.printStackTrace();
				}
			}
		}	    	
		return data;     
	}
	@Override
	protected void onPostExecute(String result) {
		//once the results are gathered, toast them.
		showResult();
		if(speak)
		{
			speak();
		}
	}
}
	
	@Override
    public void onInit(int status) {
 
        if (status == TextToSpeech.SUCCESS) {
 
            int result = tts.setLanguage(Locale.US);
 
            if (result == TextToSpeech.LANG_MISSING_DATA || result == TextToSpeech.LANG_NOT_SUPPORTED) 
            {
            	Toast.makeText(getApplicationContext(), "Language not supported.", Toast.LENGTH_SHORT).show();
            } 
        } 
        else 
        {
        	Toast.makeText(getApplicationContext(), "TTS Error", Toast.LENGTH_SHORT).show();
        }
 
    }
	
	private void speak() {
		if(result.contains("1"))
    	{
    	tts.speak("Pin "+pin+" is on.", TextToSpeech.QUEUE_FLUSH, null);
    	}
    	else if(result.contains("0"))
    	{
    		tts.speak("Pin "+pin+" is off.", TextToSpeech.QUEUE_FLUSH, null);
    	}
    	else
    	{
    		tts.speak("Error getting results.", TextToSpeech.QUEUE_FLUSH, null);
    	}		
	}

	public void showResult()
	{
		if(result.contains("1"))
		{
			Toast.makeText(VoiceActivity.this.getApplicationContext(),"ON", Toast.LENGTH_SHORT).show();
		}
		else if(result.contains("0"))
		{
			Toast.makeText(VoiceActivity.this.getApplicationContext(),"OFF", Toast.LENGTH_SHORT).show();
		}
		else
		{
			Toast.makeText(VoiceActivity.this.getApplicationContext(),result, Toast.LENGTH_SHORT).show();
		}
	}
}