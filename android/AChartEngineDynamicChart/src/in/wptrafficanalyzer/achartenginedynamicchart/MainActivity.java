package in.wptrafficanalyzer.achartenginedynamicchart;

import java.io.FileOutputStream;
import java.util.Random;

import org.achartengine.ChartFactory;
import org.achartengine.GraphicalView;
import org.achartengine.chart.BarChart.Type;
import org.achartengine.chart.PointStyle;
import org.achartengine.model.XYMultipleSeriesDataset;
import org.achartengine.model.XYSeries;
import org.achartengine.renderer.XYMultipleSeriesRenderer;
import org.achartengine.renderer.XYSeriesRenderer;
import org.w3c.dom.Text;

import android.app.Activity;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Menu;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MainActivity extends Activity implements SensorEventListener {
	FileOutputStream fout;	


	private GraphicalView mChart;
	
	private XYSeries visitsSeries ;
	private XYSeries yaxisSeries ;
	private XYSeries zaxisSeries ;
	
	
	private XYMultipleSeriesDataset dataset;

    private SensorManager mSensorMgr;
    private Sensor mSensor;	
	 TextView disp_x;
	 TextView disp_y;
	 TextView disp_z;
	private XYSeriesRenderer visitsRenderer;
	
	private XYSeriesRenderer yaxisRenderer;
	private XYSeriesRenderer zaxisRenderer;
	
	private XYMultipleSeriesRenderer multiRenderer;

//x aixs	
	float [] xv = new float[100];
	float [] yv = new float[100];
//y aixs
	float [] yyxv = new float[100];
	float [] yyyv = new float[100];
//z aixs
	float [] zzxv = new float[100];
	float [] zzyv = new float[100];

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
       // openfile(Environment.getExternalStorageDirectory().getPath() +"/gsensor.csv");
        disp_x = (TextView)findViewById(R.id.x);
        disp_y = (TextView)findViewById(R.id.y);
        disp_z = (TextView)findViewById(R.id.z);

        mSensorMgr = (SensorManager) this.getSystemService(SENSOR_SERVICE);
        mSensor = mSensorMgr.getSensorList(Sensor.TYPE_ACCELEROMETER).get(0);
        mSensorMgr.registerListener(this, mSensor, SensorManager.SENSOR_DELAY_NORMAL);       
        // Setting up chart
        setupChart();

        
        // Start plotting chart
   //     new ChartTask().execute();
        
    }
    
    private void setupChart(){
    	
    	// Creating an  XYSeries for Visits
    	visitsSeries = new XYSeries("x axis");
    	yaxisSeries = new XYSeries("y axis");
    	zaxisSeries = new XYSeries("z axis");
   	
    	// Creating a dataset to hold each series
    	dataset = new XYMultipleSeriesDataset();
    	// Adding Visits Series to the dataset
    	dataset.addSeries(visitsSeries);    	
    	
    	dataset.addSeries(yaxisSeries);    	
    	dataset.addSeries(zaxisSeries);    	
    	
    	// Creating XYSeriesRenderer to customize visitsSeries
    	visitsRenderer = new XYSeriesRenderer();
    	visitsRenderer.setColor(Color.RED);
    	visitsRenderer.setPointStyle(PointStyle.POINT);
    	visitsRenderer.setFillPoints(true);
    	visitsRenderer.setLineWidth(2);
//    	visitsRenderer.setDisplayChartValues(true);

//Y axis    	
    	yaxisRenderer = new XYSeriesRenderer();
    	yaxisRenderer.setColor(Color.BLUE);
    	yaxisRenderer.setPointStyle(PointStyle.POINT);
    	yaxisRenderer.setFillPoints(true);
    	yaxisRenderer.setLineWidth(2);    	

//Z axis    	
    	zaxisRenderer = new XYSeriesRenderer();
    	zaxisRenderer.setColor(Color.GREEN);
    	zaxisRenderer.setPointStyle(PointStyle.POINT);
    	zaxisRenderer.setFillPoints(true);
    	zaxisRenderer.setLineWidth(2);    	
    	
    	
    	// Creating a XYMultipleSeriesRenderer to customize the whole chart
    	multiRenderer = new XYMultipleSeriesRenderer();
    	
    	multiRenderer.setChartTitle("");
    	multiRenderer.setXTitle("Msec");
    	multiRenderer.setYTitle("Value");

    	multiRenderer.setZoomButtonsVisible(true);
    	
    	multiRenderer.setXAxisMin(0);
    	multiRenderer.setXAxisMax(100);
    	
    	multiRenderer.setYAxisMin(-20);
    	multiRenderer.setYAxisMax(20);

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

    	multiRenderer.addSeriesRenderer(yaxisRenderer);
    	multiRenderer.addSeriesRenderer(zaxisRenderer);
    	
    	// Getting a reference to LinearLayout of the MainActivity Layout
    	LinearLayout chartContainer = (LinearLayout) findViewById(R.id.chart_container);
    	
   		
    	mChart = (GraphicalView) ChartFactory.getLineChartView(getBaseContext(), dataset, multiRenderer);
    	//mChart.setHorizontalScrollBarEnabled(true);
    	//mChart.
   		// Adding the Line Chart to the LinearLayout
    	chartContainer.addView(mChart);
    	
    }
    
/*    
    private class ChartTask extends AsyncTask<Void, String, Void> {

    	// Generates dummy data in a non-ui thread
		@Override
		protected Void doInBackground(Void... params) {
			int i = 0;
			try{
				do{
					String [] values = new String[20];
					Random r = new Random();
					int visits = r.nextInt(10);
					
					values[0] = Integer.toString(i);
					values[1] = Integer.toString(visits);
					publishProgress(values);					
					Thread.sleep(50);
					i++;
				}while(i<=10000);
			}catch(Exception e){ }
			return null;
		}
		
		// Plotting generated data in the graph
		@Override
		protected void onProgressUpdate(String... values) {
			
		int length = visitsSeries.getItemCount();  
		if (length > 10) {  
		length = 10;  
		}  

	//将旧的点集中x和y的数值取出来放入backup中，并且将x的值加1，造成曲线向右平移的效果  
		for (int i = 0; i < length; i++) {  
			xv[i] = (float) visitsSeries.getX(i) + 1;
			yv[i] = (float) visitsSeries.getY(i);
		}

			visitsSeries.clear();
			visitsSeries.add(0, Integer.parseInt(values[1]));
			
			for (int i = 0; i < length; i++) {  
				visitsSeries.add(xv[i], yv[i]);
			}
			mChart.repaint();
		}    	
    }    
*/
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		// TODO Auto-generated method stub
		/*
//        StringBuffer sb = new StringBuffer();
//        sb.append("x = " + event.values[SensorManager.DATA_X] + "\n");
        //sb.append("y = " + event.values[SensorManager.DATA_Y] + "\n");
      // sb.append("z = " + event.values[SensorManager.DATA_Z] + "\n");
//        mContentTv.setText(sb.toString());
//        mValues = event.values;
        */
		
		int length = visitsSeries.getItemCount();  
		if (length > 100) {  
		length = 100;  
		}  

	//将旧的点集中x和y的数值取出来放入backup中，并且将x的值加1，造成曲线向右平移的效果  
		for (int i = 0; i < length; i++) {  
			xv[i] = (float) visitsSeries.getX(i) + 1;
			yv[i] = (float) visitsSeries.getY(i);
			
			yyxv[i] = (float) yaxisSeries.getX(i) + 1;
			yyyv[i] = (float) yaxisSeries.getY(i);

			
			zzxv[i] = (float) zaxisSeries.getX(i) + 1;
			zzyv[i] = (float) zaxisSeries.getY(i);
			
			
		}

			visitsSeries.clear();
			visitsSeries.add(0, event.values[SensorManager.DATA_X]);

			yaxisSeries.clear();
			yaxisSeries.add(0, event.values[SensorManager.DATA_Y]);

			zaxisSeries.clear();
			zaxisSeries.add(0, event.values[SensorManager.DATA_Z]);

						
			for (int i = 0; i < length; i++) {  
				visitsSeries.add(xv[i], yv[i]);

				yaxisSeries.add(yyxv[i], yyyv[i]);

				zaxisSeries.add(zzxv[i], zzyv[i]);
			}
			mChart.repaint();
//			Float.toString(yv[1]);
			Log.d("chend",Float.toString(yv[1]));
			
			/*
	        StringBuffer sb = new StringBuffer();
	        sb.append("x:" + event.values[SensorManager.DATA_X] + " ");
	        sb.append("y:" + event.values[SensorManager.DATA_Y] + " ");
	        sb.append("z:" + event.values[SensorManager.DATA_Z] + " ");
			*/
			
			disp_x.setText("x:"+Float.toString(yv[1]));
			disp_y.setText("y:"+Float.toString(yyyv[1]));
			disp_z.setText("z:"+Float.toString(zzyv[1]));
			//writefile("12345");


			
		}        

		public void openfile(String filefullname) {
			try {
				fout = new FileOutputStream(filefullname);
			} catch(Exception e) {
				e.printStackTrace();
			}			
		}
		
		public void closefile() {
			try {
			fout.close();
			} catch(Exception e) {
				e.printStackTrace();
			}
		}
		

	  public void writefile(String message){ 
		   try{	
		        byte [] bytes = message.getBytes(); 
		        fout.write(bytes); 
		         //fout.close(); 
		        } catch(Exception e){ 
		        	e.printStackTrace();
		       }
		   }
}