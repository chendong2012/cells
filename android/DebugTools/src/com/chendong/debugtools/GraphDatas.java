package com.chendong.debugtools;
import java.util.ArrayList;
import android.graphics.Point;

public class GraphDatas {
	 ArrayList <Point> mypoints = new ArrayList<Point>();
	 public GraphDatas(int num) {
		 for (int i = 0; i < num; i++)
			 mypoints.add(new Point(0, 0));
	 }
	 
	 void setpoint(int index, int x, int y) {		 
		 mypoints.set(index, new Point(x,y));
	 }
	 
	 Point getpoint(int index) {
		return mypoints.get(index);		 
	 }
	 
	 int getX(int index) {
		 return mypoints.get(index).x;
	 }
	 
	 int getY(int index) {
		 return mypoints.get(index).y;
	 }
	 
	 void setX(int index, int x) {
		 Point point = mypoints.get(index);
		 point.x = x;
	 }
	 
	 void setY(int index, int y) {
		 Point point = mypoints.get(index);
		 point.y = y;		 
	 }
}
