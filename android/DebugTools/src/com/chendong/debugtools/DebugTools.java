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
import android.os.Bundle;
import android.os.Handler;
import android.R.integer;
import android.app.Activity;
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
	private String fileDatas;
	private XYMultipleSeriesDataset dataset;
	
	
	String PS_FILE_PATH = "/sys/devices/platform/als_ps/driver/ps";
	String LOW_FILE_PATH = "/sys/devices/platform/als_ps/driver/proximity_low";
	String HIGH_FILE_PATH = "/sys/devices/platform/als_ps/driver/proximity_high";

	private SensorManager mSensorMgr;//距离传感器管理器
   private Sensor mPSensor;//距离传感器
   private boolean mIsProximityRight;

   TextView psNum;
	TextView low;
	TextView high;
	Button runButton;
	
   Handler handler=new Handler();
   
	private XYSeriesRenderer visitsRenderer;
	private XYSeriesRenderer lowRenderer;
	private XYSeriesRenderer highRenderer;
	
	private XYMultipleSeriesRenderer multiRenderer;
	GraphDatas psPoints = new GraphDatas(100);
	GraphDatas lowPoints = new GraphDatas(100);
	GraphDatas highPoints = new GraphDatas(100);
		
	boolean runFlag= true;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_debug_tools);
        
        psNum = (TextView)findViewById(R.id.textView1);
        low = (TextView)findViewById(R.id.textView2);
        high = (TextView)findViewById(R.id.textView3);
        runButton = (Button)findViewById(R.id.button1);
        runButton.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						if (runFlag)
							runFlag = false;
						else 
							runFlag = true;
						handler.postDelayed(runnable, 300);
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
				
        handler.postDelayed(runnable, 200);
    }//onCreate end

	    Runnable runnable=new Runnable() {
	        @Override
	        public void run() {
	        	try {	     
	        		fileDatas = ToolsFileOps.getFileCxt(PS_FILE_PATH);
			     		String psstr="感应值:";
			     		psNum.setText(psstr + fileDatas);
			     		int ps_rawdata = Integer.parseInt(fileDatas.trim());	     		
			     		
		        	int length = psSeries.getItemCount();  
		        	if (length > 100)
		        			length = 100;  
        	
        		for (int i = 0; i < length; i++)
        			psPoints.setpoint(i, (int) psSeries.getX(i) + 1, (int)psSeries.getY(i));

        		psSeries.clear();
        		psSeries.add(0, ps_rawdata);//(0,y)坐标
	
        		for (int i = 0; i < length; i++)
        			psSeries.add(psPoints.getX(i), psPoints.getY(i));
 //low       		
        		fileDatas = ToolsFileOps.getFileCxt(LOW_FILE_PATH);
		     		String lowstr="远离阀值：";
		     		low.setText(lowstr+fileDatas);
		     		int data_low = Integer.parseInt(fileDatas.trim());	     		
		     		
	        		int length_low = lowSeries.getItemCount();  
	        		if (length_low > 100)
	        			length_low = 100;  

       		for (int i = 0; i < length_low; i++)    
						lowPoints.setpoint(i, (int) lowSeries.getX(i) + 1, (int) lowSeries.getY(i));

       		lowSeries.clear();
       		lowSeries.add(0, data_low);//(0,y)坐标
	
       		for (int i = 0; i < length; i++)
       			lowSeries.add(lowPoints.getX(i), lowPoints.getY(i));
       		
//high       		
       		fileDatas = ToolsFileOps.getFileCxt(HIGH_FILE_PATH);
	     		String highstr="接近阀值：";
	     		high.setText(highstr+fileDatas);
	     		
	     		int data_high = Integer.parseInt(fileDatas.trim());	     		
	     		
       		int length_high = highSeries.getItemCount();  
       		if (length_high > 100)
       			length_high = 100;  

  		for (int i = 0; i < length_high; i++)
				highPoints.setpoint(i, (int) highSeries.getX(i) + 1, (int) highSeries.getY(i));

  		highSeries.clear();
  		highSeries.add(0, data_high);//(0,y)坐标

  		for (int i = 0; i < length_high; i++)
  			highSeries.add(highPoints.getX(i), highPoints.getY(i));
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
        	}
        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
        	}
    };//new SensorEventListener() end
    
    private void setupChart(){
    	/*Creating an  XYSeries for Visits*/
    	psSeries = new XYSeries("PS原始值");
    	lowSeries = new XYSeries("远离阀值");
    	highSeries = new XYSeries("接近阀值");
    	
    	/*Creating a dataset to hold each seri*/
    	dataset = new XYMultipleSeriesDataset();
    	/*Adding Visits Series to the dataset*/
    	dataset.addSeries(psSeries);    	

    	dataset.addSeries(lowSeries);    
    	dataset.addSeries(highSeries);
    	
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

    	// Creating XYSeriesRenderer to customize psSeries
    	highRenderer = new XYSeriesRenderer();
    	highRenderer.setColor(Color.YELLOW);
    	highRenderer.setPointStyle(PointStyle.POINT);
    	highRenderer.setFillPoints(true);
    	highRenderer.setLineWidth(2);
    	
    	// Creating a XYMultipleSeriesRenderer to customize the whole chart
    	multiRenderer = new XYMultipleSeriesRenderer();
    	
    	multiRenderer.setChartTitle("");
    	multiRenderer.setXTitle("Msec");
    	multiRenderer.setYTitle("Value");

    	multiRenderer.setZoomButtonsVisible(true);
    	
    	multiRenderer.setXAxisMin(0);
    	multiRenderer.setXAxisMax(100);
    	
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

    	// Getting a reference to LinearLayout of the MainActivity Layout
    	LinearLayout chartContainer = (LinearLayout) findViewById(R.id.LinearLayout2);
    	
   		
    	mChart = (GraphicalView) ChartFactory.getLineChartView(getBaseContext(), dataset, multiRenderer);
    	//mChart.setHorizontalScrollBarEnabled(true);
   		// Adding the Line Chart to the LinearLayout
    	chartContainer.addView(mChart);
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
        if (true == mIsProximityRight) {
            mSensorMgr.unregisterListener(mProximityListener);
        }
    }
}