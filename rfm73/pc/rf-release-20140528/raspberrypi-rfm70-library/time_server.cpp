#include <stdio.h>
#include <stdlib.h>
#include <IReceive.h>
#include "user_activity.h"
#include "comm.h"
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "public.h"

/*send:time[index]
  send back:time2014,3,14,12,01,59

*/
extern user_activity *myu11;

static int gettime(char *buf);
static void cb_get_server_timer(unsigned char *dat, unsigned char len);
IReceive irec_time_server(TIME_REMOTE_KEYWORD, cb_get_server_timer);

static char time_buf[32];
static void cb_get_server_timer(unsigned char *dat, unsigned char len)
{
	int time_cnt;
	/*第５个字节开始才是内容*/
        if(irec_time_server.isNewPackage(dat)) {
		time_cnt = gettime(time_buf);
                myu11->m_comm->send(time_buf, time_cnt);
                irec_time_server.saveAckBuf((unsigned char *)time_buf, time_cnt);
		printf("send back time to remote\n");
        } else {
                myu11->m_comm->send((const char *)irec_time_server.getAckBuf(), irec_time_server.getAckBufLen());
		printf("send back time to remote-1\n");
        }
}

static int gettime(char *buf)
{
	struct tm  *ptm;
	long   ts;
	int    y,m,d,h,n,s;
	ts = time(NULL);
	ptm = localtime(&ts);

	y = ptm->tm_year+1900;  //年
	m = ptm->tm_mon+1;      //月
	d = ptm->tm_mday;       //日
	h = ptm->tm_hour;       //时
	n = ptm->tm_min;        //分
	s = ptm->tm_sec;        //秒 
	return sprintf(buf,"%s%04d,%02d,%02d,%02d,%02d,%02d","time",y,m,d,h,n,s);
}
