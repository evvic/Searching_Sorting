#include "TIMER.H"
#include <time.h>

Timer::Timer()
{
   start_time = clock();
}
 
double Timer::elapsed_time()
{
   clock_t end_time = clock();
   return ((double) ((double)end_time - (double)start_time)) / ((double) CLK_TCK);
}
 
void Timer::reset()
{
   start_time = clock();
}
