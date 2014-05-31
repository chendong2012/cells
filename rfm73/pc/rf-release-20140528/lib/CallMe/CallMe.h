#ifndef CALLME_H
#define CALLME_H

#include <SoftTimer.h>
#include <Task.h>
#include <Arduino.h>
//#include <ISend.h>
class ISend;
class CallMe : public Task
{
  public:
    CallMe(unsigned long loopTime, boolean (*callback)(ISend *), ISend *is);
    void start();
    void stop();
  private:
    boolean (*_callback)(ISend *);
    static void step(Task* me);
    unsigned long _looptime;
    ISend *_is;
};

#endif

