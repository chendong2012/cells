package com.chendong.debugtools;


import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import org.achartengine.ChartFactory;
import org.achartengine.GraphicalView;
import org.achartengine.chart.PointStyle;
import org.achartengine.model.XYMultipleSeriesDataset;
import org.achartengine.model.XYSeries;
import org.achartengine.renderer.XYMultipleSeriesRenderer;
import org.achartengine.renderer.XYSeriesRenderer;

import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.widget.LinearLayout;
import android.widget.TextView;

public class DebugTools extends Activity {
	private GraphicalView mChart;
	private XYSeries visitsSeries;
	private XYSeries lowSeries;
	private XYSeries highSeries;
	
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

   Handler handler=new Handler();
   
	private XYSeriesRenderer visitsRenderer;
	private XYSeriesRenderer lowRenderer;
	private XYSeriesRenderer highRenderer;
	
	private XYMultipleSeriesRenderer multiRenderer;

//x aixs
	float [] xv = new float[100];
	float [] yv = new float[100];

//far_away aixs
		float [] low_x = new float[100];
		float [] low_y = new float[100];
		
//close aixs
		float [] high_x = new float[100];
		float [] high_y = new float[100];	
		
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_debug_tools);
        
        psNum = (TextView)findViewById(R.id.textView1);
        low = (TextView)findViewById(R.id.textView2);
        high = (TextView)findViewById(R.id.textView3);
        
        mSensorMgr = (SensorManager) this.getSystemService(SENSOR_SERVICE);

      // Setting up chart
        setupChart();
        
			try {
				execCommand("ls /sys/devices/platform/als_ps/driver/");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

        
        
			
        
        
        
        handler.postDelayed(runnable, 200);
    }//onCreate end

    public void execCommand(String command) throws IOException {
		Runtime runtime = Runtime.getRuntime();
		Process proc = runtime.exec(command);
		try {
			if (proc.waitFor() != 0) {
				System.err.println("exit value = " + proc.exitValue());
			}
			BufferedReader in = new BufferedReader(new InputStreamReader(
					proc.getInputStream()));
			StringBuffer stringBuffer = new StringBuffer();
			String line = null;
			while ((line = in.readLine()) != null) {
				stringBuffer.append(line+"\n");
			}
			System.out.println(stringBuffer.toString());

		} catch (InterruptedException e) {
			System.err.println(e);
		}
	}    
    
    
	    Runnable runnable=new Runnable() {
	        @Override
	        public void run() {
	            // TODO Auto-generated method stub
	        	try {	 
//ps	        		
	             FileInputStream fis = new FileInputStream(PS_FILE_PATH);
		     		int len = fis.available();
		     		byte[] bytes = new byte[len];
		     		fis.read(bytes);
		     		fis.close();
		     		String res = new String(bytes);
		     		String psstr="感应值:";
		     		psNum.setText(psstr+res);
		     		int ps_rawdata = Integer.parseInt(res.trim());	     		
		     		
	        		int length = visitsSeries.getItemCount();  
	        		if (length > 100) {  
	        			length = 100;  
	        		}
	        			

        	//将旧的点集中x和y的数值取出来放入backup中，并且将x的值加1，造成曲线向右平移的效果 
        	//第一步是把点往右边平移一个单位	
        		for (int i = 0; i < length; i++) {  
        			xv[i] = (float) visitsSeries.getX(i) + 1;
        			yv[i] = (float) visitsSeries.getY(i);
        		}
        		//清所有坐标数据
        		visitsSeries.clear();
        		visitsSeries.add(0, ps_rawdata);//(0,y)坐标
	
        		for (int i = 0; i < length; i++) {  
        			visitsSeries.add(xv[i], yv[i]);
        		}
        		///*
 //low       		
	          FileInputStream stream_low = new FileInputStream(LOW_FILE_PATH);
		     		int len_low = stream_low.available();
		     		byte[] bytes_low = new byte[len_low];
		     		stream_low.read(bytes_low);
		     		stream_low.close();
		     		String res_low = new String(bytes_low);
		     		String lowstr="远离阀值：";
		     		low.setText(lowstr+res_low);
		     		int data_low = Integer.parseInt(res_low.trim());	     		
		     		
	        		int length_low = lowSeries.getItemCount();  
	        		if (length_low > 100) {  
	        			length_low = 100;  
       		}

       	//将旧的点集中x和y的数值取出来放入backup中，并且将x的值加1，造成曲线向右平移的效果 
       	//第一步是把点往右边平移一个单位	
       		for (int i = 0; i < length_low; i++) {  
       			low_x[i] = (float) lowSeries.getX(i) + 1;
       			low_y[i] = (float) lowSeries.getY(i);
       		}
       		//清所有坐标数据
       		lowSeries.clear();
       		lowSeries.add(0, data_low);//(0,y)坐标
	
       		for (int i = 0; i < length; i++) {  
       			lowSeries.add(low_x[i], low_y[i]);
       		    }
       		
//high       		
            FileInputStream stream_high = new FileInputStream(HIGH_FILE_PATH);
	     		int len_high = stream_high.available();
	     		byte[] bytes_high = new byte[len_high];
	     		stream_high.read(bytes_high);
	     		stream_high.close();
	     		String res_high = new String(bytes_high);
	     		String highstr="接近阀值：";
	     		high.setText(highstr+res_high);
	     		
	     		int data_high = Integer.parseInt(res_high.trim());	     		
	     		
       		int length_high = highSeries.getItemCount();  
       		if (length_high > 100) {  
       			length_high = 100;  
  		}

  	//将旧的点集中x和y的数值取出来放入backup中，并且将x的值加1，造成曲线向右平移的效果 
  	//第一步是把点往右边平移一个单位	
  		for (int i = 0; i < length_high; i++) {  
  			high_x[i] = (float) highSeries.getX(i) + 1;
  			high_y[i] = (float) highSeries.getY(i);
  		}
  		//清所有坐标数据
  		highSeries.clear();
  		highSeries.add(0, data_high);//(0,y)坐标

  		for (int i = 0; i < length_high; i++) {  
  			highSeries.add(high_x[i], high_y[i]);
  		}
  	//	*/
	    }//try end
  		//===============================
	        		
	        		
     			
	     		
	 catch (Exception e) {
	 			// TODO: handle exception
	 	}
	   mChart.repaint();  
	   handler.postDelayed(this, 200);
        }//run end
    };
    
    SensorEventListener mProximityListener = new SensorEventListener() {
        @Override
        public void onSensorChanged(SensorEvent event) {

        	}

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
            // TODO Auto-generated method stub

        	}

    };//new SensorEventListener() end
    
    private void setupChart(){
    	/*Creating an  XYSeries for Visits*/
    	visitsSeries = new XYSeries("PS原始值");
    	
    	lowSeries = new XYSeries("远离阀值");
    	highSeries = new XYSeries("接近阀值");
    	

    	/*Creating a dataset to hold each seri*/
    	dataset = new XYMultipleSeriesDataset();
    	/*Adding Visits Series to the dataset*/
    	dataset.addSeries(visitsSeries);    	

    	dataset.addSeries(lowSeries);    
    	dataset.addSeries(highSeries);
    	
    	// Creating XYSeriesRenderer to customize visitsSeries
    	visitsRenderer = new XYSeriesRenderer();
    	visitsRenderer.setColor(Color.GREEN);
    	visitsRenderer.setPointStyle(PointStyle.POINT);
    	visitsRenderer.setFillPoints(true);
    	visitsRenderer.setLineWidth(2);

    	// Creating XYSeriesRenderer to customize visitsSeries
    	lowRenderer = new XYSeriesRenderer();
    	lowRenderer.setColor(Color.RED);
    	lowRenderer.setPointStyle(PointStyle.POINT);
    	lowRenderer.setFillPoints(true);
    	lowRenderer.setLineWidth(2);

    	// Creating XYSeriesRenderer to customize visitsSeries
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
//    	multiRenderer.set
    	
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
    	//mChart.
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