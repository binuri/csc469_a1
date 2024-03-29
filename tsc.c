#include <sys/types.h>
#include <stdio.h>

#include <time.h>
#include <sys/time.h>

#include "tsc.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

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

  return ((((u_int64_t)ncyc_hi << 32) | ncyc_lo) - start) / 2;

}

/*
 * Returns the estimated clock speed of the current system
 */
u_int64_t get_estimated_clock_speed(){
    struct timespec ts = {0,2.5e8};
    u_int64_t startcycle, endcycle, deltacycle, avgcycle = 0;
    for (int i=0; i<10; i++) {
        startcycle = get_counter();
        nanosleep(&ts, NULL);
        endcycle = get_counter();
        deltacycle = (endcycle - startcycle)*4;
        avgcycle += deltacycle;
    }
    avgcycle = avgcycle / 10;
    printf("Cycles/Second : %"PRIu64"\n", avgcycle);
    return avgcycle;
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
        curr = get_counter();
        int temp = 0;    
        for (int i = 0; i < 20; i++){
            temp += i;
        }

        if (curr - prev > threshold){
            samples[ 2 * i] = prev;
            samples[ 2 * i + 1] = curr;
            i++;
        }
        prev = curr;
    }

    return init;
}
