package com.llc24x7lab.console24x7;

import android.content.Context;
import android.os.Bundle;
import android.preference.EditTextPreference;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceFragment;
import android.widget.Toast;

public class SettingsVNCFragment extends PreferenceFragment {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // load the preferences from an XML resource
        addPreferencesFromResource(R.xml.pref_settings_vnc);

        // validate vncpassword
        EditTextPreference passwordPref = (EditTextPreference) findPreference("vncpassword");
        passwordPref.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
            @Override
            public boolean onPreferenceChange(Preference preference, Object newValue) {
                String passwordNew = (String) newValue;
                if (!passwordNew.matches("[a-zA-Z0-9]+")) {
                    Context ctx = Globals.getContext();
                    Toast.makeText(ctx, R.string.password_validation_error, Toast.LENGTH_LONG).show();
                    return false;
                }
                return true;
            }
        });

        // validate vncportnum
        EditTextPreference portnumPref = (EditTextPreference) findPreference("vncportnum");
        portnumPref.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
            @Override
            public boolean onPreferenceChange(Preference preference, Object newValue) {
                int portnumNew = Integer.parseInt((String) newValue);
                if ((portnumNew < 1) || (portnumNew > 65535)) {
                    Context ctx = Globals.getContext();
                    Toast.makeText(ctx, R.string.portnum_validation_error, Toast.LENGTH_LONG).show();
                    return false;
                }
                return true;
            }
        });

        // validate vncreversehost
        EditTextPreference rhostPref = (EditTextPreference) findPreference("vncreversehost");
        rhostPref.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
            @Override
            public boolean onPreferenceChange(Preference preference, Object newValue) {
                String rhostNew = (String) newValue;
                if (rhostNew.matches("127.0.0.1")) {
                    Context ctx = Globals.getContext();
                    Toast.makeText(ctx, R.string.hostname_validation_error, Toast.LENGTH_LONG).show();
                    return false;
                }
                return true;
            }
        });

        // validate vncreverseport
        EditTextPreference rportPref = (EditTextPreference) findPreference("vncreverseport");
        rportPref.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
            @Override
            public boolean onPreferenceChange(Preference preference, Object newValue) {
                int rportNew = Integer.parseInt((String) newValue);
                if ((rportNew < 1) || (rportNew > 65535)) {
                    Context ctx = Globals.getContext();
                    Toast.makeText(ctx, R.string.portnum_validation_error, Toast.LENGTH_LONG).show();
                    return false;
                }
                return true;
            }
        });
    }
}