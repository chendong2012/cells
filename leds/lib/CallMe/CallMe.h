#ifndef CALLME_H
#define CALLME_H

#include <SoftTimer.h>
#include <Task.h>
#include <Arduino.h>
class CallMe : public Task
{
public:
    CallMe(unsigned long loopTime, boolean (*callback)(void));
    void start();
    void stop();
private:
    boolean (*_callback)(void);
    static void step(Task* me);
    unsigned long _looptime;
};
#endif
