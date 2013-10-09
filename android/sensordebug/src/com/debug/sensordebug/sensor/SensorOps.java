package com.debug.sensordebug.sensor;

import android.app.Activity;
import android.hardware.Sensor;
import android.hardware.SensorManager;


import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;


public class SensorOps {

	private SensorManager mSensorMgr;	// 距离传感器管理器
	
	private Sensor mPSensor;				// 距离传感器
	private boolean mIsProximityRight;
	
	private Sensor mALSensor;				// LIGHT传感器	
	private boolean mIsLightRight;
	
	
	private Activity mActivity;
	
	

	public SensorOps(Activity activity) {
		// TODO Auto-generated constructor stub
		mActivity = activity;
	}

	SensorEventListener mProximityListener = new SensorEventListener() {
        @Override
        public void onSensorChanged(SensorEvent event) {
        	
        	}
        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
        	}
    };//new SensorEventListener() end

	SensorEventListener mLightListener = new SensorEventListener() {
        @Override
        public void onSensorChanged(SensorEvent event) {
        	
        	}
        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
        	}
    };//new SensorEventListener() end

    
	public void init() {
		mSensorMgr = (SensorManager) mActivity.getSystemService("sensor");
	}
	
	public void uninit()
	{
		
	}
	
	private void register_als()
	{
		  mALSensor = mSensorMgr.getSensorList(Sensor.TYPE_LIGHT).get(0);
        mIsLightRight = mSensorMgr.registerListener(mLightListener, mALSensor, SensorManager.SENSOR_DELAY_UI);
        if (false == mIsLightRight) {
            try {
                Thread.sleep(300);
                mIsLightRight = mSensorMgr.registerListener(mLightListener, mALSensor,
                        SensorManager.SENSOR_DELAY_UI);
            } catch (InterruptedException e) {
            }

        }			
	}
	
	private void register_ps()
	{
        mPSensor = mSensorMgr.getSensorList(Sensor.TYPE_PROXIMITY).get(0);
        mIsProximityRight = mSensorMgr.registerListener(mProximityListener, mPSensor, SensorManager.SENSOR_DELAY_UI);
        if (false == mIsProximityRight) {
            try {
                Thread.sleep(300);
                mIsProximityRight = mSensorMgr.registerListener(mProximityListener, mPSensor,
                        SensorManager.SENSOR_DELAY_UI);
            } catch (InterruptedException e) {
            }

        }		
	}
	
	
	/*注册*/
	public void register()
	{
		register_ps();
		register_als();
	}
	
	/////////////////////////////////////////////
	private void unregister_ps() {
        if (true == mIsProximityRight) {
            mSensorMgr.unregisterListener(mProximityListener);
        }
	}
	private void unregister_als() {
        if (true == mIsLightRight) {
            mSensorMgr.unregisterListener(mLightListener);
        }		
	}
	
	/*注销*/
	public void unregister()
	{
		unregister_ps();
		unregister_als();
	}
}
