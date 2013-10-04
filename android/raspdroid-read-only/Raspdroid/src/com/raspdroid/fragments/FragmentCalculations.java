package com.raspdroid.fragments;



import com.actionbarsherlock.app.SherlockFragment;

import com.raspdroid.helper.PreferenceHolder;

import com.raspdroid.*;
import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Bundle;
import android.os.Vibrator;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.Toast;

public class FragmentCalculations extends SherlockFragment
{
	
	public static final String STRING_ERRORPARSE = "Number parse error, check your entries";
	public static final String STRING_ERROREDITTEXTS = "Please check your entries";

	
	private int vibratedurationInteger, formatdecimalplaceInteger;
	
	private String voltsString, ampsString, ohmsString;
	
	private PreferenceHolder preferenceHolder;
	
	
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) 
	{
		
		final View view = inflater.inflate(R.layout.fragment_calculations,
				container, false);	
		
		final Vibrator vibrator = (Vibrator) 
				getActivity().getSystemService(Context.VIBRATOR_SERVICE);

		
		preferenceHolder = new PreferenceHolder();
		
			if (preferenceHolder != null)
			{
				
				vibratedurationInteger = preferenceHolder
						.getVibrateDuration(getActivity());
				
				formatdecimalplaceInteger = preferenceHolder
						.getFormatDecimalPlaces(getActivity());
			}

		
		final EditText ohmslawvoltsEditText = (EditText)
				view.findViewById(R.id.ohmslawvoltsEditText);
		
		final EditText ohmslawampsEditText = (EditText)
				view.findViewById(R.id.ohmslawampsEditText);
		
		final EditText ohmslawohmsEditText = (EditText)
				view.findViewById(R.id.ohmslawohmsEditText);
		

		View.OnClickListener clickListener = new View.OnClickListener() 
		{
			
			public void onClick(View view) 
			{
				
				vibrator.vibrate(vibratedurationInteger);

				switch (view.getId()) 
				{
				
					case R.id.ohmslawcalculateButton:
						
						
						voltsString = ohmslawvoltsEditText.getText().toString();
						ampsString = ohmslawampsEditText.getText().toString();
						ohmsString = ohmslawohmsEditText.getText().toString();
						
						calculateMissingNumber(ohmslawvoltsEditText, ohmslawampsEditText, ohmslawohmsEditText);
						
						break;

				}
				
			}
			
		};

		view.findViewById(R.id.ohmslawcalculateButton)
				.setOnClickListener(clickListener);
		
		return view;
		
	}
	
	
	@Override
	public void onResume() 
	{
		super.onResume();
		
		if (preferenceHolder != null)
		{
			
			vibratedurationInteger = preferenceHolder
					.getVibrateDuration(getActivity());
			
			formatdecimalplaceInteger = preferenceHolder
					.getFormatDecimalPlaces(getActivity());
		}
		
	}


	private void calculateMissingNumber(EditText ohmslawvoltsEditText, EditText ohmslawampsEditText, EditText ohmslawohmsEditText)
	{

		if(!voltsString.equals("") && !ampsString.equals("") && ohmsString.equals(""))
		{
			
			ohmslawohmsEditText.setText(getOhmsValueString());
			
		}
		
		else if(!ampsString.equals("") && !ohmsString.equals("") && voltsString.equals(""))
		{
			
			ohmslawvoltsEditText.setText(getVoltageValueString());
			
		}
		
		else if(!voltsString.equals("") && !ohmsString.equals("") && ampsString.equals(""))
		{
			
			ohmslawampsEditText.setText(getAmpsValueString());
			
		}
		
		else
		{
			
			Toast.makeText(getActivity().getApplicationContext(), STRING_ERROREDITTEXTS, 
					Toast.LENGTH_LONG).show();
			
		}
		
	}
	
	
	private String getOhmsValueString()
	{
		
		double voltsDouble = 0, ampsDouble = 0, ohmsDouble = 0;
		
			try 
			{
			
				voltsDouble = Double.parseDouble(voltsString);
				
				ampsDouble = Double.parseDouble(ampsString);
			
			}
			
			catch (NumberFormatException e) 
			{
		
				Toast.makeText(getActivity().getApplicationContext(), STRING_ERRORPARSE, 
						Toast.LENGTH_LONG).show();
			
			}
				
		if(voltsDouble > 0 && ampsDouble > 0)
		{
			
			ohmsDouble = (voltsDouble / ampsDouble);
			
		}			
		
		return getFormattedString(ohmsDouble);
	}
	
	private String getVoltageValueString()
	{
		
		double voltsDouble = 0, ampsDouble = 0, ohmsDouble = 0;
		
			try 
			{
			
				ampsDouble = Double.parseDouble(ampsString);
				
				ohmsDouble = Double.parseDouble(ohmsString);
			
			}
			
			catch (NumberFormatException e) 
			{
		
				Toast.makeText(getActivity().getApplicationContext(), STRING_ERRORPARSE, 
						Toast.LENGTH_LONG).show();
			
			}
				
		if(ampsDouble > 0 && ohmsDouble > 0)
		{
			
			voltsDouble = (ampsDouble * ohmsDouble);
			
		}			
		
		return getFormattedString(voltsDouble);
		
	}
	
	private String getAmpsValueString()
	{
		
		double voltsDouble = 0, ampsDouble = 0, ohmsDouble = 0;
		
			try 
			{
			
				voltsDouble = Double.parseDouble(voltsString);
				
				ohmsDouble = Double.parseDouble(ohmsString);
			
			}
			
			catch (NumberFormatException e) 
			{
		
				Toast.makeText(getActivity().getApplicationContext(), STRING_ERRORPARSE, 
						Toast.LENGTH_LONG).show();
			
			}
				
		if(voltsDouble > 0 && ohmsDouble > 0)
		{
			
			ampsDouble = (voltsDouble * ohmsDouble);
			
		}			
		
		return getFormattedString(ampsDouble);
		
	}
	
	@SuppressLint("DefaultLocale")
	private String getFormattedString(double number)
	{
		
		switch(formatdecimalplaceInteger)
		{
		
			case 0:
				
				int roundedInteger = (int) Math.round(number);
				
				return String.valueOf(roundedInteger);
				
				
			case 1:
								
				return String.format("%.01f", number);
				
				
			case 2:
				
				return String.format("%.02f", number);
				
				
			case 3:
				
				return String.format("%.03f", number);
				
				
			case 4:
				
				return String.format("%.04f", number);
				
				
			case 5:
				
				return String.format("%.05f", number);
				
				
			default:
				
				return String.valueOf(number);

		}
			
	}
	
	
	

	
	
	
}
