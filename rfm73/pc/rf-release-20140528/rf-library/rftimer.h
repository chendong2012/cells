#ifndef __RFTIMER__
#define __RFTIMER__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>    // for printf()
#include <unistd.h>   // for pause()
#include <signal.h>   // for signal()
#include <string.h>   // for memset()
#include <sys/time.h> // struct itimeral. setitimer()
#include <errno.h>
class user_activity;
struct bb {
	timer_t timer_id;
	int a;
	void *b;
};

class rftimer {
public:
	rftimer(user_activity *activity);
	void init(void (*cb)(union sigval si));
	void begin(float delay, float interval);
	void stop();
	void clear_count(void);
	void add_count(void);
	void handler(int event);

	unsigned char get_count(void);
	unsigned char is_timeout(void);
	void settimeout(user_activity *activity);
	struct bb timer_datas;
	user_activity *m_activity;
	int test;
private:
	unsigned char count;
	struct itimerval tick;
};
#endif
