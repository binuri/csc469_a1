#include <sys/types.h>
#include "tsc.h"

/* initialize the cycle counter */
u_int64_t start = 0;

extern inline void access_counter(unsigned *hi, unsigned *lo);

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
