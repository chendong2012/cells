package com.raspdroid;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Locale;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

import android.app.Dialog;
import android.os.AsyncTask;
import android.os.Bundle;
import android.speech.tts.TextToSpeech;
import android.support.v4.app.FragmentActivity;
import android.support.v4.view.ViewPager;
import android.util.Base64;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.Toast;

import com.raspdroid.VoiceActivity.SectionsPagerAdapter;
import com.raspdroid.helper.Dialogs;
import com.raspdroid.helper.PreferenceHolder;

public class GpioButtons extends FragmentActivity implements TextToSpeech.OnInitListener {
	public static final String STRING_WAKELOCKTOAST = "Wake lock has been turned on";
  	public PreferenceHolder preferenceHolder;
  	SectionsPagerAdapter mSectionsPagerAdapter;
  	private TextToSpeech tts;
	ViewPager mViewPager;
	String pin;
	String hideString;
	String getValue = "";
	String message = "";
	int button;
	int resultSpeech;
	Locale language;
	boolean goAuthorise;
	boolean speak;
	boolean status;
	String ipaddress;
	boolean isHidden1, isHidden2, isHidden3, isHidden4, isHidden5, isHidden6, isHidden7, isHidden8,
	isHidden9, isHidden10, isHidden11, isHidden12, isHidden13, isHidden14, isHidden15;
	public Button button1, button2, button3, button4, button5, button6, button7, button8, button9, 
			button10, button11, button12, button13, button14, button15;	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_gpiobuttons);
		 tts = new TextToSpeech(this, this);
		preferenceHolder = new PreferenceHolder();
		pin =  preferenceHolder.WEBIOPI_getPin(GpioButtons.this);
		String x = preferenceHolder.WEBIOPI_getUsername(GpioButtons.this);
        if (x != null) {
      	  goAuthorise = true;
        } else {
      	  goAuthorise = false;
        }
		
		button1 = (Button) findViewById(R.id.button1);
		button2 = (Button) findViewById(R.id.button2);
		button3 = (Button) findViewById(R.id.button3);
		button4 = (Button) findViewById(R.id.button4);
		button5 = (Button) findViewById(R.id.button5);
		button6 = (Button) findViewById(R.id.button6);
		button7 = (Button) findViewById(R.id.button7);
		button8 = (Button) findViewById(R.id.button8);
		button9 = (Button) findViewById(R.id.button9);
		button10 = (Button) findViewById(R.id.button10);
		button11 = (Button) findViewById(R.id.button11);
		button12 = (Button) findViewById(R.id.button12);
		button13 = (Button) findViewById(R.id.button13);
		button14 = (Button) findViewById(R.id.button14);
		button15 = (Button) findViewById(R.id.button15);
		
		registerForContextMenu(findViewById(R.id.button1));
		registerForContextMenu(findViewById(R.id.button2));
		registerForContextMenu(findViewById(R.id.button3));
		registerForContextMenu(findViewById(R.id.button4));
		registerForContextMenu(findViewById(R.id.button5));
		registerForContextMenu(findViewById(R.id.button6));
		registerForContextMenu(findViewById(R.id.button7));
		registerForContextMenu(findViewById(R.id.button8));
		registerForContextMenu(findViewById(R.id.button9));
		registerForContextMenu(findViewById(R.id.button10));
		registerForContextMenu(findViewById(R.id.button11));
		registerForContextMenu(findViewById(R.id.button12));
		registerForContextMenu(findViewById(R.id.button13));
		registerForContextMenu(findViewById(R.id.button14));
		registerForContextMenu(findViewById(R.id.button15));
		
		checkHidden();
		
		final Button button1 = (Button) findViewById(R.id.button1);
		button1.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 1;
				pin = "0";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 1;
					pin = "0";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button1.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 1;
				pin = "0";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button2 = (Button) findViewById(R.id.button2);
		button2.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status 
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 2;
				pin = "1";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 2;
					pin = "1";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button2.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 2;
				pin = "1";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button3 = (Button) findViewById(R.id.button3);
		button3.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status 
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 3;
				pin = "4";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 3;
					pin = "4";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status 
				new getValueTask().execute();
			}
			}
		});
		
		button3.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 3;
				pin = "4";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button4 = (Button) findViewById(R.id.button4);
		button4.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status 
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 4;
				pin = "17";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 4;
					pin = "17";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button4.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 4;
				pin = "17";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button5 = (Button) findViewById(R.id.button5);
		button5.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 5;
				pin = "21";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 5;
					pin = "21";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button5.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 5;
				pin = "21";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button6 = (Button) findViewById(R.id.button6);
		button6.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 6;
				pin = "22";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 6;
					pin = "22";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button6.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 6;
				pin = "22";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button7 = (Button) findViewById(R.id.button7);
		button7.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 7;
				pin = "10";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 7;
					pin = "10";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button7.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 7;
				pin = "10";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button8 = (Button) findViewById(R.id.button8);
		button8.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 8;
				pin = "9";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 8;
					pin = "9";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button8.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 8;
				pin = "9";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button9 = (Button) findViewById(R.id.button9);
		button9.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 9;
				pin = "11";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 9;
					pin = "11";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button9.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 9;
				pin = "11";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button10 = (Button) findViewById(R.id.button10);
		button10.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 10;
				pin = "18";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 10;
					pin = "18";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button10.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 10;
				pin = "18";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button11 = (Button) findViewById(R.id.button11);
		button11.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 11;
				pin = "23";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 11;
					pin = "23";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button11.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 11;
				pin = "23";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button12 = (Button) findViewById(R.id.button12);
		button12.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 12;
				pin = "24";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 12;
					pin = "24";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button12.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 12;
				pin = "24";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button13 = (Button) findViewById(R.id.button13);
		button13.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 13;
				pin = "25";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 13;
					pin = "25";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button13.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 13;
				pin = "25";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button14 = (Button) findViewById(R.id.button14);
		button14.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status 
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 14;
				pin = "8";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 14;
					pin = "8";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button14.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 14;
				pin = "8";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
		
		final Button button15 = (Button) findViewById(R.id.button15);
		button15.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View view) 
			{
				ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
				if(ipaddress.equals(""))
		        {
		        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
							"Connection Error", getResources().getString(R.string.error_connection), true);
		        	dialog.show();
		        }
				else
				{
				//get status
				new getValueTask().execute();
				//if off... turn on
				if(getValue.contains("0"))
				{
				button = 15;
				pin = "7";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				postData(goAuthorise, 
	        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	        			  false, 
	        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	        			  pin,
	        			  "value", "1");
				}
				// if on... turn off
				else
				{
					button = 15;
					pin = "7";
					preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
					postData(goAuthorise, 
		        			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
		        			  false, 
		        			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
		        			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
		        			  pin,
		        			  "value", "0");
				}
				//show updated status
				new getValueTask().execute();
			}
			}
		});
		
		button15.setOnLongClickListener(new View.OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				button = 15;
				pin = "7";
				preferenceHolder.WEBIOPI_savePin(getApplicationContext(), pin);
				v.showContextMenu();
				return true;
			}
		});
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
	
	public void checkHidden() {
		
		isHidden1 = preferenceHolder.getHide1(GpioButtons.this);
		isHidden2 = preferenceHolder.getHide2(GpioButtons.this);
		isHidden3 = preferenceHolder.getHide3(GpioButtons.this);
		isHidden4 = preferenceHolder.getHide4(GpioButtons.this);
		isHidden5 = preferenceHolder.getHide5(GpioButtons.this);
		isHidden6 = preferenceHolder.getHide6(GpioButtons.this);
		isHidden7 = preferenceHolder.getHide7(GpioButtons.this);
		isHidden8 = preferenceHolder.getHide8(GpioButtons.this);
		isHidden9 = preferenceHolder.getHide9(GpioButtons.this);
		isHidden10 = preferenceHolder.getHide10(GpioButtons.this);
		isHidden11 = preferenceHolder.getHide11(GpioButtons.this);
		isHidden12 = preferenceHolder.getHide12(GpioButtons.this);
		isHidden13 = preferenceHolder.getHide13(GpioButtons.this);
		isHidden14 = preferenceHolder.getHide14(GpioButtons.this);
		isHidden15 = preferenceHolder.getHide15(GpioButtons.this);
		
		if(isHidden1)
		{
			button1.setVisibility(View.GONE);
		}
		if(!isHidden1)
		{
			button1.setVisibility(View.VISIBLE);
		}
		
		if(isHidden2)
		{
			button2.setVisibility(View.GONE);
		}
		if(!isHidden2)
		{
			button2.setVisibility(View.VISIBLE);
		}
		
		if(isHidden3)
		{
			button3.setVisibility(View.GONE);
		}
		if(!isHidden3)
		{
			button3.setVisibility(View.VISIBLE);
		}
		
		if(isHidden4)
		{
			button4.setVisibility(View.GONE);
		}
		if(!isHidden4)
		{
			button4.setVisibility(View.VISIBLE);
		}
		
		if(isHidden5)
		{
			button5.setVisibility(View.GONE);
		}
		if(!isHidden5)
		{
			button5.setVisibility(View.VISIBLE);
		}
		
		if(isHidden6)
		{
			button6.setVisibility(View.GONE);
		}
		if(!isHidden6)
		{
			button6.setVisibility(View.VISIBLE);
		}
		
		if(isHidden7)
		{
			button7.setVisibility(View.GONE);
		}
		if(!isHidden7)
		{
			button7.setVisibility(View.VISIBLE);
		}
		
		if(isHidden8)
		{
			button8.setVisibility(View.GONE);
		}
		if(!isHidden8)
		{
			button8.setVisibility(View.VISIBLE);
		}
		
		if(isHidden9)
		{
			button9.setVisibility(View.GONE);
		}
		if(!isHidden9)
		{
			button9.setVisibility(View.VISIBLE);
		}
		
		if(isHidden10)
		{
			button10.setVisibility(View.GONE);
		}
		if(!isHidden10)
		{
			button10.setVisibility(View.VISIBLE);
		}
		
		if(isHidden11)
		{
			button11.setVisibility(View.GONE);
		}
		if(!isHidden11)
		{
			button11.setVisibility(View.VISIBLE);
		}
		
		if(isHidden12)
		{
			button12.setVisibility(View.GONE);
		}
		if(!isHidden12)
		{
			button12.setVisibility(View.VISIBLE);
		}
		
		if(isHidden13)
		{
			button13.setVisibility(View.GONE);
		}
		if(!isHidden13)
		{
			button13.setVisibility(View.VISIBLE);
		}
		
		if(isHidden14)
		{
			button14.setVisibility(View.GONE);
		}
		if(!isHidden14)
		{
			button14.setVisibility(View.VISIBLE);
		}
		
		if(isHidden15)
		{
			button15.setVisibility(View.GONE);
		}
		if(!isHidden15)
		{
			button15.setVisibility(View.VISIBLE);
		}
	}

	@Override
	public void onCreateContextMenu(ContextMenu menu, View v, ContextMenuInfo menuInfo) {
	super.onCreateContextMenu(menu, v, menuInfo);

	switch(v.getId()){
	
	case R.id.button1:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button2:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button3:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button4:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button5:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button6:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button7:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button8:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button9:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button10:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button11:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button12:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button13:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button14:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
	
	case R.id.button15:
		menu.setHeaderTitle("Options:");
		menu.add(Menu.NONE, 1, Menu.NONE, "Hide Button");
		menu.add(Menu.NONE, 2, Menu.NONE, "Input");
		menu.add(Menu.NONE, 3, Menu.NONE, "Output");
		menu.add(Menu.NONE, 4, Menu.NONE, "Get Status");
	break;
		}
	}
	
	@Override
	public boolean onContextItemSelected(MenuItem item){
	switch(item.getItemId()){
	case 1:
		hideButton(button);
	break;
	case 2:
		ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
		if(ipaddress.equals(""))
        {
        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
					"Connection Error", getResources().getString(R.string.error_connection), true);
        	dialog.show();
        }
		else
		{
		setInput();
		}
	break;
	case 3:
		ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
		if(ipaddress.equals(""))
        {
        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
					"Connection Error", getResources().getString(R.string.error_connection), true);
        	dialog.show();
        }
		else
		{
		setOutput();
		}
	break;
	case 4:
		ipaddress = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
		if(ipaddress.equals(""))
        {
        	final Dialog dialog = new Dialogs().createInformationConfirmDialog(GpioButtons.this, 
					"Connection Error", getResources().getString(R.string.error_connection), true);
        	dialog.show();
        }
		else
		{
		speak = preferenceHolder.getVoice(GpioButtons.this);
		status = true;
		new getValueTask().execute();
		}
	break;
	default:
	return super.onContextItemSelected(item);
	}
	return true;
	}
	
	private void setOutput() {
		postData(goAuthorise, 
	  			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
	  			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
	  			  false, 
	  			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
	  			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
	  			  pin,
	  			  "function", "out");
	}

	private void setInput() {
		postData(goAuthorise, 
  			  preferenceHolder.WEBIOPI_getUsername(GpioButtons.this), 
  			  preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this), 
  			  false, 
  			  preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this), 
  			  preferenceHolder.WEBIOPI_getPort(GpioButtons.this), 
  			  pin,
  			  "function", "in");
	}

	public void hideButton(int button) {
		switch(button)
		{
		case 1:
			button1.setVisibility(View.GONE);
			preferenceHolder.saveHide1(GpioButtons.this, true);
			break;
		case 2:
			button2.setVisibility(View.GONE);
			preferenceHolder.saveHide2(GpioButtons.this, true);
			break;
		case 3:
			button3.setVisibility(View.GONE);
			preferenceHolder.saveHide3(GpioButtons.this, true);
			break;
		case 4:
			button4.setVisibility(View.GONE);
			preferenceHolder.saveHide4(GpioButtons.this, true);
			break;
		case 5:
			button5.setVisibility(View.GONE);
			preferenceHolder.saveHide5(GpioButtons.this, true);
			break;
		case 6:
			button6.setVisibility(View.GONE);
			preferenceHolder.saveHide6(GpioButtons.this, true);
			break;
		case 7:
			button7.setVisibility(View.GONE);
			preferenceHolder.saveHide7(GpioButtons.this, true);
			break;
		case 8:
			button8.setVisibility(View.GONE);
			preferenceHolder.saveHide8(GpioButtons.this, true);
			break;
		case 9:
			button9.setVisibility(View.GONE);
			preferenceHolder.saveHide9(GpioButtons.this, true);
			break;
		case 10:
			button10.setVisibility(View.GONE);
			preferenceHolder.saveHide10(GpioButtons.this, true);
			break;
		case 11:
			button11.setVisibility(View.GONE);
			preferenceHolder.saveHide11(GpioButtons.this, true);
			break;
		case 12:
			button12.setVisibility(View.GONE);
			preferenceHolder.saveHide12(GpioButtons.this, true);
			break;
		case 13:
			button13.setVisibility(View.GONE);
			preferenceHolder.saveHide13(GpioButtons.this, true);
			break;
		case 14:
			button14.setVisibility(View.GONE);
			preferenceHolder.saveHide14(GpioButtons.this, true);
			break;
		case 15:
			button15.setVisibility(View.GONE);
			preferenceHolder.saveHide15(GpioButtons.this, true);
			break;
		}
	}
	
	private void unhide() 
	{
		button1.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide1(GpioButtons.this, false);
		button2.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide2(GpioButtons.this, false);
		button3.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide3(GpioButtons.this, false);
		button4.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide4(GpioButtons.this, false);
		button5.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide5(GpioButtons.this, false);
		button6.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide6(GpioButtons.this, false);
		button7.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide7(GpioButtons.this, false);
		button8.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide8(GpioButtons.this, false);
		button9.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide9(GpioButtons.this, false);
		button10.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide10(GpioButtons.this, false);
		button11.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide11(GpioButtons.this, false);
		button12.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide12(GpioButtons.this, false);
		button13.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide13(GpioButtons.this, false);
		button14.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide14(GpioButtons.this, false);
		button15.setVisibility(View.VISIBLE);
		preferenceHolder.saveHide15(GpioButtons.this, false);
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
        asyncHttpPost.execute(h + IP + ":" + port + "/GPIO/" + GPIO + "/" + mode + "/" + value);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.menu_buttons, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(android.view.MenuItem item) {
		switch (item.getItemId()) {
			case R.id.settings_option_item:
				Dialog dialog = new Dialogs()
				.webiopiDialog(this);
				dialog.show();
			break;
			case R.id.unhide:
				unhide();
				break;
			case R.id.useVoice:
				preferenceHolder.saveVoice(GpioButtons.this, true);
				speak = preferenceHolder.getVoice(GpioButtons.this);
				break;
			case R.id.muteVoice:
				preferenceHolder.saveVoice(GpioButtons.this, false);
				speak = preferenceHolder.getVoice(GpioButtons.this);
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
					.getWakeLockBoolean(GpioButtons.this);
			
			if(wakelockBoolean == true)
			{	
				final Window window = getWindow(); 
				window.setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, 0);
				
				final boolean wakelocktoastBoolean = preferenceHolder
						.getWakeLockToastBoolean(GpioButtons.this);
				
				if(wakelocktoastBoolean == true)
				{	
					Toast.makeText(GpioButtons.this.getApplicationContext(), STRING_WAKELOCKTOAST, 
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
		String ip = preferenceHolder.WEBIOPI_getIPAddress(GpioButtons.this);
		String port = preferenceHolder.WEBIOPI_getPort(GpioButtons.this);
		String user = preferenceHolder.WEBIOPI_getUsername(GpioButtons.this);
		String password = preferenceHolder.WEBIOPI_getUserPassword(GpioButtons.this);
		String pin = preferenceHolder.WEBIOPI_getPin(GpioButtons.this);
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
	    	getValue = EntityUtils.toString(resEntityGet);
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
		setcolor();
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
		if(getValue.contains("1"))
    	{
    	tts.speak("Pin "+pin+" is on.", TextToSpeech.QUEUE_FLUSH, null);
    	}
    	else if(getValue.contains("0"))
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
		if(getValue.contains("1"))
		{
			Toast.makeText(GpioButtons.this.getApplicationContext(),"ON", Toast.LENGTH_SHORT).show();
		}
		else if(getValue.contains("0"))
		{
			Toast.makeText(GpioButtons.this.getApplicationContext(),"OFF", Toast.LENGTH_SHORT).show();
		}
		else
		{
			Toast.makeText(GpioButtons.this.getApplicationContext(),getValue, Toast.LENGTH_SHORT).show();
		}
	}
	
public void setcolor()
{
		if((pin == "0") && (getValue.contains("0")))
		{
			button1.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "0") && (getValue.contains("1")))
		{
			button1.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "1") && (getValue.contains("0")))
		{
			button2.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "1") && (getValue.contains("1")))
		{
			button2.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "4") && (getValue.contains("0")))
		{
			button3.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "4") && (getValue.contains("1")))
		{
			button3.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "17") && (getValue.contains("0")))
		{
			button4.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "17") && (getValue.contains("1")))
		{
			button4.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "21") && (getValue.contains("0")))
		{
			button5.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "21") && (getValue.contains("1")))
		{
			button5.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "22") && (getValue.contains("0")))
		{
			button6.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "22") && (getValue.contains("1")))
		{
			button6.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "10") && (getValue.contains("0")))
		{
			button7.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "10") && (getValue.contains("1")))
		{
			button7.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "9") && (getValue.contains("0")))
		{
			button8.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "9") && (getValue.contains("1")))
		{
			button8.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "11") && (getValue.contains("0")))
		{
			button9.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "11") && (getValue.contains("1")))
		{
			button9.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "18") && (getValue.contains("0")))
		{
			button10.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "18") && (getValue.contains("1")))
		{
			button10.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "23") && (getValue.contains("0")))
		{
			button11.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "23") && (getValue.contains("1")))
		{
			button11.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "24") && (getValue.contains("0")))
		{
			button12.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "24") && (getValue.contains("1")))
		{
			button12.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "25") && (getValue.contains("0")))
		{
			button13.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "25") && (getValue.contains("1")))
		{
			button13.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "8") && (getValue.contains("0")))
		{
			button14.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "8") && (getValue.contains("1")))
		{
			button14.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
		
		if((pin == "7") && (getValue.contains("0")))
		{
			button15.setBackgroundColor(getResources().getColor(R.color.standard_black));
		}
		else if((pin == "7") && (getValue.contains("1")))
		{
			button15.setBackgroundColor(getResources().getColor(R.color.holo_blue));
		}
	}
}