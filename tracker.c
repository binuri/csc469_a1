#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tsc.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
int verbose;
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
        if (curr - prev > threshold){
            samples[ 2 * i] = prev;
            samples[ 2 * i + 1] = curr;
            i++;
        }
        prev = curr;
    }

    return init;

}

u_int64_t get_estimated_clock_speed(){
    return 0;
}

void print_sample_results(int num_samples, u_int64_t start_time, 
                                 u_int64_t threshold, u_int64_t *samples){
    
    for (int i = 0; i < num_samples; i++){
        if (verbose) {
            printf ("\n__print_results start_time: %"PRIu64"\n", start_time);
            printf ("__print_results %d: %"PRIu64" to %"PRIu64"\n",
                        i, samples[i * 2], samples[2 * i + 1]);
        }
        printf("Active %d: start at %"PRIu64" duration %"PRIu64" cycles (%f ms)\n",
            i,
            start_time,
            samples[i * 2] - start_time,
            (samples[i * 2] - start_time) / (threshold * 10e6) * 10e3);

        start_time = samples[i * 2];
        printf("Inactive %d: start at %"PRIu64" duration %"PRIu64" cycles (%f ms)\n",
            i,
            start_time,
            samples[i * 2 + 1] - start_time,
            (samples[i * 2 + 1] - start_time) / (threshold * 10e6) * 10e3);
        start_time = samples[i * 2 + 1];
    }

}

int main (int argc, char** argv) {

    verbose = 0;

    if(argc < 2) {
        printf ("ERROR: Invalid number of arguments. Please use the following format:");
        printf ("tracker <num_samples>");
        return EXIT_FAILURE;
    }
    
    char *err;
    int num_samples = strtoll(argv[1], &err, 10);
    if (*err) {
        printf ("ERROR: Invalid input for 'num_samples'");
        return EXIT_FAILURE;
    }    

    // Check if verbose flag is found
    if (argc == 3 && strcmp(argv[2], "-v") == 0) {
       	verbose = 1;
    }

    u_int64_t threshold = get_estimated_clock_speed();
    
    u_int64_t *samples = malloc(num_samples * sizeof(u_int64_t) * 2);
    u_int64_t start_time = inactive_periods(num_samples, 1400, samples);
    
    print_sample_results(num_samples, start_time, threshold, samples);
    free(samples);
   	 
	return EXIT_SUCCESS;
} 
