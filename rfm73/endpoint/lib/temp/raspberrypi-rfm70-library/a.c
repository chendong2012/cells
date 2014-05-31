#include <stdio.h>
#include <time.h>
#include <unistd.h>
int fun(char *buf)
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
	return sprintf(buf,"%02d:%02d",h,n);
}
int main(void)
{
	int ret;
	char buf[30];
	for(;;) {
		fun(buf);
		printf("%s\n",buf);
		ret = strcmp(buf,"06:20");
		if (ret == 0) {
			printf("alarm 13:00!!........................\n");
		} else {
		}
		sleep(60);
	}	

	return 0;
}
