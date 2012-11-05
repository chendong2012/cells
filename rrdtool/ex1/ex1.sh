#!/bin/bash
# create rrd
rm -f test.rrd
rm -f *.png
rrdtool create test.rrd --start 920804400 \
DS:speed:COUNTER:600:U:U \
RRA:AVERAGE:0.5:1:24 \
RRA:AVERAGE:0.5:6:10
rrdtool update test.rrd 920804700:12345 920805000:12357 920805300:12363
rrdtool update test.rrd 920805600:12363 920805900:12363 920806200:12373
rrdtool update test.rrd 920806500:12383 920806800:12393 920807100:12399
rrdtool update test.rrd 920807400:12405 920807700:12411 920808000:12415
rrdtool update test.rrd 920808300:12420 920808600:12422 920808900:12423


rrdtool fetch test.rrd AVERAGE --start 920804400 --end 920809200

rrdtool graph speed.png \
	--start 920804400 --end 920808000 \
	DEF:myspeed=test.rrd:speed:AVERAGE:step=300 \
	LINE1:myspeed#FF0000


rrdtool graph speed2.png \
	--start 920804400 --end 920808000 \
	--vertical-label m/s \
	DEF:myspeed=test.rrd:speed:AVERAGE CDEF:realspeed=myspeed,100,\* \
	LINE2:realspeed#FFFF00


rrdtool graph speed3.png \
     --start 920804400 --end 920808000 \
     --vertical-label km/h \
     DEF:myspeed=test.rrd:speed:AVERAGE \
     "CDEF:kmh=myspeed,3600,*" \
     CDEF:fast=kmh,100,GT,kmh,0,IF \
     CDEF:good=kmh,100,GT,0,kmh,IF \
     HRULE:100#0000FF:"Maximum allowed" \
     AREA:good#00FF00:"Good speed" \
     AREA:fast#FF0000:"Too fast"
