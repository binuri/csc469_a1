#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <sys/wait.h>

#include "tsc.h"

int verbose;

void print_sample_results(int process_num, int num_samples, u_int64_t start_time, 
                                 u_int64_t threshold, u_int64_t *samples){
    
    int i = 0;
    while (i < num_samples){
        
        printf("[Child %d]Active %d: start at %"PRIu64
                        " duration %"PRIu64" cycles (%f ms)\n",
            process_num,
            i,
            start_time,
            samples[i * 2] - start_time,
            (samples[i * 2] - start_time) / (threshold * 1.0) * 10e3);

        // Start time for the inactive period
        start_time = samples[i * 2];
        
        printf("[Child %d]Inactive %d: start at %"PRIu64
                        " duration %"PRIu64" cycles (%f ms)\n",
            process_num,
            i,
            start_time,
            samples[i * 2 + 1] - start_time,
            (samples[i * 2 + 1] - start_time) / (threshold * 1.0) * 10e3);
            
        // Start time for active period is end of the inactive period
        start_time = samples[i * 2 + 1];
        
        i++;
    }

}

int main(int argc, char *argv[])
{
    if(argc < 2) {
        printf ("ERROR: Invalid number of arguments. Please use the following format:\n");
        printf ("context_tracker <num_samples> <num_child_procs>\n");
        return EXIT_FAILURE;
    }

    char *err;
    int num_samples = strtoll(argv[1], &err, 10);
    if (*err) {
        printf ("ERROR: Invalid integer value given for 'num_samples'\n");
        return EXIT_FAILURE;
    }    

    // Check if verbose flag is found
    int num_child_procs = strtoll(argv[2], &err, 10);
    if (*err) {
        printf ("ERROR: Invalid integer value given for 'num_child_procs'\n");
        return EXIT_FAILURE;
    }
    
    // Pinning the process to 0th CPU
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set)) {
        perror("Error when pinning the process to a single CPU\n");
        exit(EXIT_FAILURE);
    }

    u_int64_t clock_speed = get_estimated_clock_speed();

    start_counter();
    for (int i = 0; i < num_child_procs ; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Error when forking");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
        
            u_int64_t *samples = malloc(num_samples * sizeof(u_int64_t) * 2);

            u_int64_t start_time = inactive_periods(num_samples, 1100, samples);

            print_sample_results(i, num_samples, start_time, clock_speed, samples);
            fflush(stdout);
            free(samples);

            return EXIT_SUCCESS;
        }
    }

    
    for (int i = 0; i < num_child_procs; i++) {
        int status;
        pid_t pid = wait(&status);
        if (pid == -1) {
            perror("Error in parent process when waiting for child process to end\n");
            exit(EXIT_FAILURE);
        } else if (!WIFEXITED(status)) {
            fprintf(stderr,
                "ERROR: Error occured in a child process.\n");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}


    
    
