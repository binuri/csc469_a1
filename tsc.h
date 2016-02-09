#ifndef TSC_H
#define TSC_H

/* Set *hi and *lo to the high and low order bits of the cycle counter.
 * Implementation requires assembly code to use the rdtsc instruction.
 */
static inline void access_counter(unsigned *hi, unsigned *lo)
{
  asm volatile("rdtsc; movl %%edx, %0; movl %%eax, %1" /* Read cycle counter */
      : "=r" (*hi), "=r" (*lo)                /* and move results to */
      : /* No input */                        /* the two outputs */
      : "%edx", "%eax");
}

extern void start_counter();
extern u_int64_t get_counter();

// Common functions between part A and part B
extern u_int64_t get_estimated_clock_speed();
extern u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t *samples);

#endif
