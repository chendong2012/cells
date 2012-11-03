package com.llc24x7lab.console24x7;

import java.util.List;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.preference.PreferenceActivity;
import android.preference.PreferenceManager;

public class SettingsPrefActivity extends PreferenceActivity implements OnSharedPreferenceChangeListener {

    @Override
    public void onBuildHeaders(List<Header> target) {
        loadHeadersFromResource(R.xml.pref_headers, target);
    }

    @Override
    protected void onPause() {
        super.onPause();
        Context ctx = Globals.getContext();
        PreferenceManager.getDefaultSharedPreferences(ctx).unregisterOnSharedPreferenceChangeListener(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
        Context ctx = Globals.getContext();
        PreferenceManager.getDefaultSharedPreferences(ctx).registerOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onHeaderClick(Header header, int position) {
        super.onHeaderClick(header, position);
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sp, String key) {
    }
}