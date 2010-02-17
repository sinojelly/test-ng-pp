
#ifndef __TESTNGPP_STUPID_TIMER_H
#define __TESTNGPP_STUPID_TIMER_H

#include <sys/time.h>

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct StupidTimer
{
   StupidTimer();

   void start();

   timeval stop();

private:

   timeval startTime;
};

TESTNGPP_NS_END

#endif

