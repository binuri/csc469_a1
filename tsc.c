#include <sys/types.h>
#include <stdio.h>

#include <time.h>
#include <sys/time.h>

#include "tsc.h"

/* initialize the cycle counter */
u_int64_t start = 0;

static inline void access_counter(unsigned *hi, unsigned *lo);

void start_counter()
{
  unsigned hi, lo;
  access_counter(&hi, &lo);
  start = ((u_int64_t)hi << 32) | lo;
}

u_int64_t get_counter()
{
  unsigned ncyc_hi, ncyc_lo;

  /* Get the cycle counter */
  access_counter(&ncyc_hi, &ncyc_lo);

  return (((u_int64_t)ncyc_hi << 32) | ncyc_lo) - start;

}

/*
 * Returns the estimated clock speed of the current system
 */
u_int64_t get_estimated_clock_speed(){
    struct timespec ts = {1,0};
    //struct timeval tvstart,tvend = {0,0};
    u_int64_t startcycle, endcycle, deltacycle;

    //gettimeofday(&tvstart, NULL);
    startcycle = get_counter();
    
    nanosleep(&ts, NULL);
    
    endcycle = get_counter();
    //gettimeofday(&tvend, NULL);

    //tsecs = (tvend.tv_sec - tvstart.tv_sec) + (tvend.tv_usec - tvstart.tv_usec)/1e6;

    deltacycle = (endcycle - startcycle)/2;
    return deltacycle;
}

/*
 * Continually checks the cycle counter and detects when two successive readings
 * differ by more than threshold cycles, which indicates that the process has
 * been inactive. Num samples of such recordings are saved in to the given
 * samples array and the inital reading is returned.
 */
u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples){

    u_int64_t init, curr, prev;
    init = curr = prev = get_counter();

    for (int i = 0; i < num;){
    	//TODO : Random function to sleep or something 
        curr = get_counter();
        if (curr - prev > threshold){
            samples[ 2 * i] = prev;
            samples[ 2 * i + 1] = curr;
            i++;
        }
        prev = curr;
    }

    return init;
}

