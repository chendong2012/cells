package com.raspdroid.helper;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Base64;
import android.widget.Toast;

public class PreferenceHolder 
{

	public static final String PREFERENCE_HAPTICFEEDBACKENABLED_BOOLEAN = "preferenceHapticFeedbackEnabledBoolean";
	public static final String PREFERENCE_FORMATDECIMALPLACE_STRING = "preferenceFormatDecimalPlaceString";
	public static final String PREFERENCE_WAKELOCK_BOOLEAN = "preferenceWakeLockBoolean";
	public static final String PREFERENCE_WAKELOCKTOAST_BOOLEAN = "preferenceWakeLockToastBoolean";	
	public static final String PREFERENCE_USERNAME_STRING = "preferenceUsernameString";	
	public static final String PREFERENCE_USERPASSWORD_STRING = "preferenceUserPasswordString";	
	public static final String PREFERENCE_IPADDRESS_STRING = "preferenceIPAddressString";	
	public static final String PREFERENCE_PORT_STRING = "preferencePortString";
	public static final String PREFERENCE_HIDE_BOOLEAN1 = "preferenceHideBoolean1";
	public static final String PREFERENCE_HIDE_BOOLEAN2 = "preferenceHideBoolean2";
	public static final String PREFERENCE_HIDE_BOOLEAN3 = "preferenceHideBoolean3";
	public static final String PREFERENCE_HIDE_BOOLEAN4 = "preferenceHideBoolean4";
	public static final String PREFERENCE_HIDE_BOOLEAN5 = "preferenceHideBoolean5";
	public static final String PREFERENCE_HIDE_BOOLEAN6 = "preferenceHideBoolean6";
	public static final String PREFERENCE_HIDE_BOOLEAN7 = "preferenceHideBoolean7";
	public static final String PREFERENCE_HIDE_BOOLEAN8 = "preferenceHideBoolean8";
	public static final String PREFERENCE_HIDE_BOOLEAN9 = "preferenceHideBoolean9";
	public static final String PREFERENCE_HIDE_BOOLEAN10 = "preferenceHideBoolean10";
	public static final String PREFERENCE_HIDE_BOOLEAN11 = "preferenceHideBoolean11";
	public static final String PREFERENCE_HIDE_BOOLEAN12 = "preferenceHideBoolean12";
	public static final String PREFERENCE_HIDE_BOOLEAN13 = "preferenceHideBoolean13";
	public static final String PREFERENCE_HIDE_BOOLEAN14 = "preferenceHideBoolean14";
	public static final String PREFERENCE_HIDE_BOOLEAN15 = "preferenceHideBoolean15";
	public static final String PREFERENCE_USE_VOICE_BOOLEAN = "preferenceUseVoiceBoolean";
	public static String PREFERENCE_PIN_STRING = "preferencePinString";	
	
	public static final String WEBIOPI_PREFERENCE_IPADDRESS_STRING = "WEBIOPI_preferenceIPAddressString";
	public static final String WEBIOPI_PREFERENCE_USERNAME_STRING = "WEBIOPI_preferenceUsernameString";	
	public static final String WEBIOPI_PREFERENCE_USERPASSWORD_STRING = "WEBIOPI_preferenceUserPasswordString";	
	public static final String WEBIOPI_PREFERENCE_PORT_STRING = "WEBIOPI_preferencePortString";
	public static String WEBIOPI_PREFERENCE_PIN_STRING = "WEBIOPI_preferencePinString";	

	public static final String STRING_ERRORFORMATPARSE = "Number parse error, check your decimal format entry";

	public int getVibrateDuration(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean enablehapticfeedbackBoolean = sharedPreferences
				.getBoolean(PREFERENCE_HAPTICFEEDBACKENABLED_BOOLEAN, true);
			if (enablehapticfeedbackBoolean == true) 
			{
			return 0;
			}
			else
			{
			return 25;		
			}		
	}
	
	public int getFormatDecimalPlaces(final Context context)
	{	
		final Context applicationContext = context
				.getApplicationContext();	
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final String formatdecimalplacesString = sharedPreferences
				.getString(PREFERENCE_FORMATDECIMALPLACE_STRING, "2");
		int formatdecimalplacesInteger = 0;
			try 
			{
				formatdecimalplacesInteger = Integer.parseInt(formatdecimalplacesString);				
			}
			catch (NumberFormatException e) 
			{
				Toast.makeText(applicationContext, STRING_ERRORFORMATPARSE, 
						Toast.LENGTH_LONG).show();
			}
		return formatdecimalplacesInteger;	
	}
	
	public boolean getWakeLockBoolean(final Context context)
	{	
		final Context applicationContext = context
				.getApplicationContext();	
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		return sharedPreferences.getBoolean(PREFERENCE_WAKELOCK_BOOLEAN, false);
	}
	
	public boolean getWakeLockToastBoolean(final Context context)
	{	
		final Context applicationContext = context
				.getApplicationContext();	
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		return sharedPreferences.getBoolean(PREFERENCE_WAKELOCKTOAST_BOOLEAN, true);
	}
	
	public void saveUsername(final Context context, final String usernameString)
	{		  
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putString(PREFERENCE_USERNAME_STRING, usernameString);
        sharedPreferencesEditor.commit();
	}
	public String getUsername(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();	
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final String usernameString = sharedPreferences
				.getString(PREFERENCE_USERNAME_STRING, "");
		return usernameString;
	}
	
	public void saveUserPassword(final Context context, final String plaintextpasswordString)
	{		  
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        final String encryptedpasswordString = 
        		Base64.encodeToString(plaintextpasswordString.getBytes(), Base64.DEFAULT);
        sharedPreferencesEditor.putString(PREFERENCE_USERPASSWORD_STRING, encryptedpasswordString);
        sharedPreferencesEditor.commit();
	}
	public String getUserPassword(final Context context)
	{	
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final String encryptedpasswordString = sharedPreferences
				.getString(PREFERENCE_USERPASSWORD_STRING, "");
		final String plaintextpasswordString = new String
				(Base64.decode(encryptedpasswordString, Base64.DEFAULT));
		return plaintextpasswordString;
	}
	
	public void saveIPAddress(final Context context, final String ipaddressString)
	{		  
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putString(PREFERENCE_IPADDRESS_STRING, ipaddressString);
        sharedPreferencesEditor.commit();
	}
	public String getIPAddress(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();	
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final String ipaddressString = sharedPreferences
				.getString(PREFERENCE_IPADDRESS_STRING, "");
		return ipaddressString;
	}
	
	public void savePort(final Context context, final String portString)
	{		  
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putString(PREFERENCE_PORT_STRING, portString);
        sharedPreferencesEditor.commit();
	}
	public String getPort(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();	
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final String portString = sharedPreferences
				.getString(PREFERENCE_PORT_STRING, "22");
		return portString;
	}
	public void savePin(final Context context, final String pinString)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putString(PREFERENCE_PIN_STRING, pinString);
        sharedPreferencesEditor.commit();
	}
	public String getPin(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();	
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final String portString = sharedPreferences
				.getString(PREFERENCE_PIN_STRING, "");
		return portString;
	}
	
	public void saveHide1(final Context context, final boolean b1)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN1, b1);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide1(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button1 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN1, false);
		return button1;
	}
	
	public void saveHide2(final Context context, final boolean b2)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN2, b2);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide2(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button2 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN12, false);
		return button2;
	}
	
	public void saveHide3(final Context context, final boolean b3)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN3, b3);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide3(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button3 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN3, false);
		return button3;
	}
	
	public void saveHide4(final Context context, final boolean b4)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN4, b4);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide4(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button4 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN4, false);
		return button4;
	}
	
	public void saveHide5(final Context context, final boolean b5)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN5, b5);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide5(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button5 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN5, false);
		return button5;
	}
	
	public void saveHide6(final Context context, final boolean b6)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN6, b6);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide6(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button6 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN6, false);
		return button6;
	}
	
	public void saveHide7(final Context context, final boolean b7)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN7, b7);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide7(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button7 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN7, false);
		return button7;
	}
	
	public void saveHide8(final Context context, final boolean b8)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN8, b8);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide8(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button8 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN8, false);
		return button8;
	}
	
	public void saveHide9(final Context context, final boolean b9)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN9, b9);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide9(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button9 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN9, false);
		return button9;
	}
	
	public void saveHide10(final Context context, final boolean b10)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN10, b10);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide10(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button10 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN10, false);
		return button10;
	}
	
	public void saveHide11(final Context context, final boolean b11)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN11, b11);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide11(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button11 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN11, false);
		return button11;
	}
	
	public void saveHide12(final Context context, final boolean b12)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN12, b12);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide12(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button12 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN12, false);
		return button12;
	}
	
	public void saveHide13(final Context context, final boolean b13)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN13, b13);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide13(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button13 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN13, false);
		return button13;
	}
	
	public void saveHide14(final Context context, final boolean b14)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN14, b14);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide14(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button14 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN14, false);
		return button14;
	}
	
	public void saveHide15(final Context context, final boolean b15)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_HIDE_BOOLEAN15, b15);
        sharedPreferencesEditor.commit();
	}
	public boolean getHide15(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean button15 = sharedPreferences.getBoolean(PREFERENCE_HIDE_BOOLEAN15, false);
		return button15;
	}
	
	public void saveVoice(final Context context, final boolean useVoice)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREFERENCE_USE_VOICE_BOOLEAN, useVoice);
        sharedPreferencesEditor.commit();
	}
	public boolean getVoice(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final boolean voice = sharedPreferences.getBoolean(PREFERENCE_USE_VOICE_BOOLEAN, false);
		return voice;
	}
//VOICE CONTROL AND BUTTON CONTROL PREFERENCES FOR WEBIOPI//////////////////////////////////////////////
	public void WEBIOPI_saveIPAddress(final Context context, final String WEBIOPI_ipaddressString)
	{		  
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putString(WEBIOPI_PREFERENCE_IPADDRESS_STRING, WEBIOPI_ipaddressString);
        sharedPreferencesEditor.commit();
	}
	public String WEBIOPI_getIPAddress(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();	
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final String ipaddressString = sharedPreferences
				.getString(WEBIOPI_PREFERENCE_IPADDRESS_STRING, "");
		return ipaddressString;
	}
	public void WEBIOPI_saveUsername(final Context context, final String WEBIOPI_usernameString)
	{		  
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putString(WEBIOPI_PREFERENCE_USERNAME_STRING, WEBIOPI_usernameString);
        sharedPreferencesEditor.commit();
	}
	public String WEBIOPI_getUsername(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();	
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final String usernameString = sharedPreferences
				.getString(WEBIOPI_PREFERENCE_USERNAME_STRING, "");
		return usernameString;
	}
	public void WEBIOPI_saveUserPassword(final Context context, final String plaintextpasswordString)
	{		  
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        final String encryptedpasswordString = 
        		Base64.encodeToString(plaintextpasswordString.getBytes(), Base64.DEFAULT);
        sharedPreferencesEditor.putString(WEBIOPI_PREFERENCE_USERPASSWORD_STRING, encryptedpasswordString);
        sharedPreferencesEditor.commit();
	}
	public String WEBIOPI_getUserPassword(final Context context)
	{	
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final String encryptedpasswordString = sharedPreferences
				.getString(WEBIOPI_PREFERENCE_USERPASSWORD_STRING, "");
		final String plaintextpasswordString = new String
				(Base64.decode(encryptedpasswordString, Base64.DEFAULT));
		return plaintextpasswordString;
	}
	public void WEBIOPI_savePort(final Context context, final String WEBIOPI_portString)
	{		  
		final Context applicationContext = context
				.getApplicationContext();
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putString(WEBIOPI_PREFERENCE_PORT_STRING, WEBIOPI_portString);
        sharedPreferencesEditor.commit();
	}
	public String WEBIOPI_getPort(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();	
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final String portString = sharedPreferences
				.getString(WEBIOPI_PREFERENCE_PORT_STRING, "8000");
		return portString;
	}
	public void WEBIOPI_savePin(final Context context, final String WEBIOPI_pinString)
	{		  
		 Context applicationContext = context
				.getApplicationContext();
		 SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		 SharedPreferences.Editor sharedPreferencesEditor = 
				sharedPreferences.edit();
        sharedPreferencesEditor.putString(WEBIOPI_PREFERENCE_PIN_STRING, WEBIOPI_pinString);
        sharedPreferencesEditor.commit();
	}
	public String WEBIOPI_getPin(final Context context)
	{
		final Context applicationContext = context
				.getApplicationContext();	
		final SharedPreferences sharedPreferences = PreferenceManager
				.getDefaultSharedPreferences(applicationContext);
		final String portString = sharedPreferences
				.getString(WEBIOPI_PREFERENCE_PIN_STRING, "");
		return portString;
	}
}