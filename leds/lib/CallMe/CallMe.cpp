#include "Arduino.h"
#include "CallMe.h"
#include <Task.h>

CallMe::CallMe(unsigned long loopTime)
    : Task(loopTime, &(CallMe::step))
{
	this->_looptime = loopTime;
}

/*会立刻执行调用step,...looptime....step*/
void CallMe::start()
{
	SoftTimer.add(this);
}

void CallMe::stop()
{
	SoftTimer.remove(this);
}

void CallMe::step(Task* task)
{
	CallMe* dr = (CallMe*)task;
	dr->_callback(dr->_p);
}

void CallMe::cb_func(boolean (*callback)(void *p))
{
	this->_callback = callback;
}

void CallMe::set_cb_para(void *p)
{
	_p = p;
}


