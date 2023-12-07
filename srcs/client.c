#include "mytest.h"
#include <stdio.h>

void clientOrientedIo() {
#ifdef TIMES
  struct timeval stime, etime;
  struct timeval time_result;
#endif

  /* Server_oriented_io. Measure IO time, communication time, and time for the
   * rest. */

#ifdef TIMES
  gettimeofday(&stime, NULL);
#endif

  coRun();

#ifdef TIMES
  gettimeofday(&etime, NULL);
  time_result.tv_sec = etime.tv_sec - stime.tv_sec;
  time_result.tv_usec = etime.tv_usec - stime.tv_usec;

  // Handle case where end microseconds is less than start microseconds
  if (etime.tv_usec < stime.tv_usec) {
    time_result.tv_sec--;
    time_result.tv_usec += 1000000;
  }

  // Convert total time to milliseconds
  long int elapsed_time =
      time_result.tv_sec * 1000 + time_result.tv_usec / 1000;

  printf("Client_oriented_io elapsed time: %ld ms\n", elapsed_time);
#endif
}
