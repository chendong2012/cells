#include "Arduino.h"
#include "CallMe.h"
#include <Task.h>
#include <ISend.h>

CallMe::CallMe(unsigned long loopTime, boolean (*callback)(ISend *), ISend *is)
    : Task(loopTime, &(CallMe::step))
{
	this->_callback = callback;
	this->_looptime = loopTime;
	this->_is = is;
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
	dr->_callback(dr->_is);
}
