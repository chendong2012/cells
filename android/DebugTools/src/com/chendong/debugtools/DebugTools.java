package com.chendong.debugtools;


import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

import org.achartengine.ChartFactory;
import org.achartengine.GraphicalView;
import org.achartengine.chart.PointStyle;
import org.achartengine.model.XYMultipleSeriesDataset;
import org.achartengine.model.XYSeries;
import org.achartengine.renderer.XYMultipleSeriesRenderer;
import org.achartengine.renderer.XYSeriesRenderer;

import android.graphics.Color;
import android.graphics.Point;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.media.AudioManager;
import android.media.ToneGenerator;
import android.os.Bundle;
import android.os.Handler;
import android.os.Vibrator;
import android.R.integer;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class DebugTools extends Activity {

	private GraphicalView mChart;
	private XYSeries psSeries;
	private XYSeries lowSeries;
	private XYSeries highSeries;
	private XYSeries calinoiseSeries;
	private String fileDatas;
	private XYMultipleSeriesDataset dataset;
	
	
	String PS_FILE_PATH = "/sys/devices/platform/als_ps/driver/ps";
	String LOW_FILE_PATH = "/sys/devices/platform/als_ps/driver/proximity_low";
	String HIGH_FILE_PATH = "/sys/devices/platform/als_ps/driver/proximity_high";

	String CALINOISE_FILE_PATH = "/sys/devices/platform/als_ps/driver/cali_noise";
	String CALITABLEINDEX_FILE_PATH = "/sys/devices/platform/als_ps/driver/calitableindex";
	private SensorManager mSensorMgr;//距离传感器管理器
   private Sensor mPSensor;//距离传感器
   private boolean mIsProximityRight;

   TextView psNum;
	TextView low;
	TextView high;
	TextView calinoise;
	TextView caliindex;
	
	Button runButton;
	
   Handler handler=new Handler();
   private Object mToneGeneratorLock = new Object();
   private Vibrator vibrator;
   private ToneGenerator mToneGenerator;
	private XYSeriesRenderer visitsRenderer;
	private XYSeriesRenderer lowRenderer;
	private XYSeriesRenderer highRenderer;
	private XYSeriesRenderer calinoiseRenderer;

	
	
	private XYMultipleSeriesRenderer multiRenderer;
	GraphDatas psPoints = new GraphDatas(200);
	GraphDatas lowPoints = new GraphDatas(200);
	GraphDatas highPoints = new GraphDatas(200);
	GraphDatas calinoisePoints = new GraphDatas(200);		
	boolean runFlag= true;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_debug_tools);
        
        psNum = (TextView)findViewById(R.id.textView1);
        low = (TextView)findViewById(R.id.textView2);
        high = (TextView)findViewById(R.id.textView3);
        calinoise = (TextView)findViewById(R.id.textView4);
        caliindex = (TextView)findViewById(R.id.textView5);
        runButton = (Button)findViewById(R.id.button1);
        runButton.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						
				//	    Intent intent = new Intent("com.chendong.debugtools.Command");
					//    startActivity(intent);
					    Intent intent=new Intent();	
					    intent.setClass(DebugTools.this, Command.class);
					    startActivity(intent);
						
						/*
						if (runFlag)
							runFlag = false;
						else 
							runFlag = true;
						handler.postDelayed(runnable, 300);
						*/
					}
				});
        
        mSensorMgr = (SensorManager) this.getSystemService(SENSOR_SERVICE);

        setupChart();
		//		try {
		//			execCommand("ls /sys/devices/platform/als_ps/driver/");
		//		} catch (IOException e) {
					// TODO Auto-generated catch block
			//		e.printStackTrace();
		//		}
		vibrator = (Vibrator) 
				this.getSystemService(Context.VIBRATOR_SERVICE);
        handler.postDelayed(runnable, 200);
    }//onCreate end

	    Runnable runnable=new Runnable() {
	        @Override
	        public void run() {
	        	try {
	        		fileDatas = ToolsFileOps.getFileCxt(CALITABLEINDEX_FILE_PATH);
			     	String caliindexstr="校准索引:";
			     	caliindex.setText(caliindexstr + fileDatas);
//ps
	        		fileDatas = ToolsFileOps.getFileCxt(PS_FILE_PATH);
			     		String psstr="PS值:";
			     		psNum.setText(psstr + fileDatas);
			     		int ps_rawdata = Integer.parseInt(fileDatas.trim());	     		
			     		
		        	int length = psSeries.getItemCount();  
		        	if (length > 200)
		        			length = 200;  
        	
        		for (int i = 0; i < length; i++)
        			psPoints.setpoint(i, (int) psSeries.getX(i) + 1, (int)psSeries.getY(i));

        		psSeries.clear();
        		psSeries.add(0, ps_rawdata);//(0,y)坐标
	
        		for (int i = 0; i < length; i++)
        			psSeries.add(psPoints.getX(i), psPoints.getY(i));
 //low       		
        		fileDatas = ToolsFileOps.getFileCxt(LOW_FILE_PATH);
		     		String lowstr="远离：";
		     		low.setText(lowstr+fileDatas);
		     		int data_low = Integer.parseInt(fileDatas.trim());	     		
		     		
	        		int length_low = lowSeries.getItemCount();  
	        		if (length_low > 200)
	        			length_low = 200;  

       		for (int i = 0; i < length_low; i++)    
						lowPoints.setpoint(i, (int) lowSeries.getX(i) + 1, (int) lowSeries.getY(i));

       		lowSeries.clear();
       		lowSeries.add(0, data_low);//(0,y)坐标
	
       		for (int i = 0; i < length; i++)
       			lowSeries.add(lowPoints.getX(i), lowPoints.getY(i));
       		
//high       		
       		fileDatas = ToolsFileOps.getFileCxt(HIGH_FILE_PATH);
	     		String highstr="接近：";
	     		high.setText(highstr+fileDatas);
	     		
	     		int data_high = Integer.parseInt(fileDatas.trim());	     		
	     		
       		int length_high = highSeries.getItemCount();  
       		if (length_high > 200)
       			length_high = 200;  

  		for (int i = 0; i < length_high; i++)
				highPoints.setpoint(i, (int) highSeries.getX(i) + 1, (int) highSeries.getY(i));

  		highSeries.clear();
  		highSeries.add(0, data_high);//(0,y)坐标

  		for (int i = 0; i < length_high; i++)
  			highSeries.add(highPoints.getX(i), highPoints.getY(i));
  		
//calinoise       		
   		fileDatas = ToolsFileOps.getFileCxt(CALINOISE_FILE_PATH);
	 		String calinoisestr="校准PS值：";
	 		calinoise.setText(calinoisestr+fileDatas);
     		int data_calinoise = Integer.parseInt(fileDatas.trim());	     		

   		int length_calinoise = calinoiseSeries.getItemCount();  
   		if (length_calinoise > 200)
   			length_calinoise = 200;  

		for (int i = 0; i < length_calinoise; i++)
			calinoisePoints.setpoint(i, (int) calinoiseSeries.getX(i) + 1, (int) calinoiseSeries.getY(i));

		calinoiseSeries.clear();
		calinoiseSeries.add(0, data_calinoise);//(0,y)坐标

		for (int i = 0; i < length_calinoise; i++)
			calinoiseSeries.add(calinoisePoints.getX(i), calinoisePoints.getY(i));

	    }//try end
	     		
	 catch (Exception e) {
	 			// TODO: handle exception
	 	}
	  mChart.repaint();
	   
	   if (runFlag)
		   handler.postDelayed(this, 200);
   }//run end
    };
    
    SensorEventListener mProximityListener = new SensorEventListener() {
        @Override
        public void onSensorChanged(SensorEvent event) {
        	
        	if (event.values[0] > 0) {
        		vibrator.cancel();
        		mToneGenerator.stopTone();
        	} else {
        		//vibrator.vibrate(new long[]{10000,100}, -1);
        		vibrator.vibrate(new long[] {0, 2000}, 0);
        		playTone(ToneGenerator.TONE_DTMF_1);
        		
        	}
        }
        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
        	}
    };//new SensorEventListener() end
    
    private void setupChart(){
    	/*Creating an  XYSeries for Visits*/
    	psSeries = new XYSeries("PS值");
    	lowSeries = new XYSeries("远离");
    	highSeries = new XYSeries("接近");
    	calinoiseSeries = new XYSeries("校准PS值");
    	
    	/*Creating a dataset to hold each seri*/
    	dataset = new XYMultipleSeriesDataset();
    	/*Adding Visits Series to the dataset*/
    	dataset.addSeries(psSeries);    	
    	dataset.addSeries(lowSeries);    
    	dataset.addSeries(highSeries);
    	dataset.addSeries(calinoiseSeries);
    	
    	// Creating XYSeriesRenderer to customize psSeries
    	visitsRenderer = new XYSeriesRenderer();
    	visitsRenderer.setColor(Color.GREEN);
    	visitsRenderer.setPointStyle(PointStyle.POINT);
    	visitsRenderer.setFillPoints(true);
    	visitsRenderer.setLineWidth(2);

    	// Creating XYSeriesRenderer to customize psSeries
    	lowRenderer = new XYSeriesRenderer();
    	lowRenderer.setColor(Color.RED);
    	lowRenderer.setPointStyle(PointStyle.POINT);
    	lowRenderer.setFillPoints(true);
    	lowRenderer.setLineWidth(2);
    	lowRenderer.setDisplayChartValues(true);
    	lowRenderer.setChartValuesTextSize(16);
    	
    	// Creating XYSeriesRenderer to customize psSeries
    	highRenderer = new XYSeriesRenderer();
    	highRenderer.setColor(Color.YELLOW);
    	highRenderer.setPointStyle(PointStyle.POINT);
    	highRenderer.setFillPoints(true);
    	highRenderer.setLineWidth(2);
    	highRenderer.setDisplayChartValues(true);
    	highRenderer.setChartValuesTextSize(16);

    	// Creating XYSeriesRenderer to customize psSeries
    	calinoiseRenderer = new XYSeriesRenderer();
    	calinoiseRenderer.setColor(Color.BLUE);
    	calinoiseRenderer.setPointStyle(PointStyle.POINT);
    	calinoiseRenderer.setFillPoints(true);
    	calinoiseRenderer.setLineWidth(3);
    	calinoiseRenderer.setDisplayChartValues(true);
    	calinoiseRenderer.setChartValuesTextSize(16);
    	// Creating a XYMultipleSeriesRenderer to customize the whole chart
    	multiRenderer = new XYMultipleSeriesRenderer();
    	
    	multiRenderer.setChartTitle("");
    	multiRenderer.setXTitle("Msec");
    	multiRenderer.setYTitle("Value");

    	multiRenderer.setZoomButtonsVisible(true);
    	
    	multiRenderer.setXAxisMin(0);
    	multiRenderer.setXAxisMax(200);
    	
    	multiRenderer.setYAxisMin(-20);
    	multiRenderer.setYAxisMax(1000);

    	multiRenderer.setLabelsTextSize(15);
    	multiRenderer.setLegendTextSize(15);
    	multiRenderer.setXLabels(15);
    	multiRenderer.setYLabels(15);
    	
    	multiRenderer.setMarginsColor(Color.TRANSPARENT);    	
    	
    	multiRenderer.setMargins(new int[] { 20, 25, 15, 20 });    	
    	multiRenderer.setAxesColor(Color.YELLOW);
    	multiRenderer.setShowGrid(true);    	
    	//multiRenderer.setBarSpacing(1);
    	
    	// Adding visitsRenderer to multipleRenderer
    	// Note: The order of adding dataseries to dataset and renderers to multipleRenderer
    	// should be same
    	multiRenderer.addSeriesRenderer(visitsRenderer);
    	multiRenderer.addSeriesRenderer(lowRenderer);
    	multiRenderer.addSeriesRenderer(highRenderer);
    	multiRenderer.addSeriesRenderer(calinoiseRenderer);
    	// Getting a reference to LinearLayout of the MainActivity Layout
    	LinearLayout chartContainer = (LinearLayout) findViewById(R.id.LinearLayout2);
    	
   		
    	mChart = (GraphicalView) ChartFactory.getLineChartView(getBaseContext(), dataset, multiRenderer);
    	//mChart.setHorizontalScrollBarEnabled(true);
   		// Adding the Line Chart to the LinearLayout
    	chartContainer.addView(mChart);
    }

    public void newToneGenerator() {
        // if the mToneGenerator creation fails, just continue without it.  It is
        // a local audio signal, and is not as important as the dtmf tone itself.
        synchronized(mToneGeneratorLock) {
            if (mToneGenerator == null) {
                try {
                    // we want the user to be able to control the volume of the dial tones
                    // outside of a call, so we use the stream type that is also mapped to the
                    // volume control keys for this activity
                        // Gionee zhangxiaowei 2013.6.14 modified for CR00825845 start
                    mToneGenerator = new ToneGenerator(AudioManager.STREAM_DTMF, 240);
                    setVolumeControlStream(AudioManager.STREAM_DTMF);
                    // Gionee zhangxiaowei 2013.6.14 modified for CR00825845 end
                } catch (RuntimeException e) {
                    
                    mToneGenerator = null;
                }
            }
        }
    }
    
    
    void playTone(int tone) {
        AudioManager audioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
        int ringerMode = audioManager.getRingerMode();
        if ((ringerMode == AudioManager.RINGER_MODE_SILENT)
            || (ringerMode == AudioManager.RINGER_MODE_VIBRATE)) {
            return;
        }

        synchronized(mToneGeneratorLock) {
            if (mToneGenerator == null) {
                
                return;
            }
            mToneGenerator.startTone(tone, 1000000);
        }
    }

    
    
    

    private void releaseToneGenerator() {
        synchronized(mToneGeneratorLock) {
            if (mToneGenerator != null) {
                mToneGenerator.release();
                mToneGenerator = null;
            }
        }
        
    }
    
    
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.debug_tools, menu);
        return true;
    }

    @Override
    protected void onResume() {
        super.onResume();
        newToneGenerator();
        mPSensor = mSensorMgr.getSensorList(Sensor.TYPE_PROXIMITY).get(0);
        mIsProximityRight = mSensorMgr.registerListener(mProximityListener, mPSensor, SensorManager.SENSOR_DELAY_UI);
        if (false == mIsProximityRight) {
            try {
                Thread.sleep(300);
                mIsProximityRight = mSensorMgr.registerListener(mProximityListener, mPSensor,
                        SensorManager.SENSOR_DELAY_UI);
            } catch (InterruptedException e) {
            }
         if (false == mIsProximityRight) {
               // mProximityNum.setText(R.string.init_proximity_sensor_fail);
            }
        }
    }
  @Override
  public void onPause() {
        super.onPause();
        releaseToneGenerator();
        if (true == mIsProximityRight) {
            mSensorMgr.unregisterListener(mProximityListener);
        }
    }
  

}
