#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>    // for printf()
#include <unistd.h>   // for pause()
#include <signal.h>   // for signal()
#include <string.h>   // for memset()
#include <sys/time.h> // struct itimeral. setitimer()
#include <errno.h>
#include "rftimer.h"
#include "user_activity.h"
#include <pthread.h>

rftimer::rftimer(user_activity *activity)
{
	m_activity = activity;
}

void rftimer::handler(int event)
{
	count++;
	printf("handle init time++\n");
	if (count >= 2) {
		count = 2;
		printf("time out\n");
		m_activity->ack_timeout();
	}
}

void rftimer::settimeout(user_activity *activity)
{
//	m_activity = activity;
}

unsigned char rftimer::get_count(void)
{
	return count;
}

void rftimer::clear_count(void)
{
	count = 0;
}

void rftimer::add_count(void)
{
	count++;
}
/*cb 里面再去调对像的指定函数就可以了*/
void rftimer::init(void (*cb)(union sigval si))
{
	int status = 0;
	struct itimerspec ts;
	struct sigevent se;
	struct bb *bbp = &timer_datas;
	timer_datas.a = 1;

	se.sigev_notify = SIGEV_THREAD;
	se.sigev_value.sival_ptr =this;
	se.sigev_notify_function = cb;
	se.sigev_notify_attributes = NULL;
	status = timer_create(CLOCK_REALTIME, &se, &bbp->timer_id);
	printf("timer init..............\n");
}

void rftimer::begin(float delay, float interval)
{
	int status =0;
	struct itimerspec ts;

	struct bb *bbp = &timer_datas;


	ts.it_value.tv_sec = abs(delay);
	ts.it_value.tv_nsec = (delay-abs(delay)) * 1e09;

	ts.it_interval.tv_sec = abs(interval);
	ts.it_interval.tv_nsec = (interval-abs(interval)) * 1e09;


	status = timer_settime(bbp->timer_id, 0, &ts, 0);
	printf("timer begin..............\n");
}

void rftimer::stop(void)
{
	struct bb *bbp = &timer_datas;
	printf("timer stop..............\n");
	timer_delete (bbp->timer_id);
}

unsigned char rftimer::is_timeout(void)
{
	if (count >= 2) {
		return 1;
	}
	return 0;

}
