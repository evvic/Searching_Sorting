#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <time.h>

class Timer{
public:
   Timer();
   double elapsed_time();
   void reset();
private:
   clock_t start_time;
};

#endif