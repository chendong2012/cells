#include "Arduino.h"
#include "TimerTask.h"
#include <Task.h>

TimerTask::TimerTask(unsigned long loopTime)
    : Task(loopTime, &(TimerTask::step))
{
	this->_looptime = loopTime;
}

/*会立刻执行调用step,...looptime....step*/
void TimerTask::start()
{
	SoftTimer.add(this);
}

void TimerTask::stop()
{
	SoftTimer.remove(this);
}

void TimerTask::step(Task* task)
{
	TimerTask* dr = (TimerTask*)task;
	dr->_callback(dr->_p);
}

void TimerTask::cb_func(boolean (*callback)(void *p))
{
	this->_callback = callback;
}

void TimerTask::set_cb_para(void *p)
{
	_p = p;
}
