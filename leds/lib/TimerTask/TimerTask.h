#ifndef CALLME_H
#define CALLME_H

#include <SoftTimer.h>
#include <Task.h>
#include <Arduino.h>
class TimerTask : public Task
{
public:
	TimerTask(unsigned long loopTime);
	void start();
	void stop();
	void set_cb_para(void *p);
	void cb_func(boolean (*callback)(void *p));
private:
	boolean (*_callback)(void *p);
	static void step(Task* me);
	unsigned long _looptime;
	void *_p;
};
#endif
