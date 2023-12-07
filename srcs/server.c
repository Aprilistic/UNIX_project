#include "mytest.h"
void serverOrientedIo() {
	#ifdef TIMES
        struct timeval stime,etime;
        long int time_result;
	#endif
	/* Server_oriented_io. Measure IO time, communication time, and time for the rest.
        */

	#ifdef TIMES
        gettimeofday(&stime, NULL);
	#endif
           
        soRun();

	#ifdef TIMES
        gettimeofday(&etime, NULL);
        time_result = etime.tv_usec - stime.tv_usec;
        printf("Server_oriented_io TIMES == %d %d %ld\n", etime.tv_usec, stime.tv_usec, time_result);
	#endif

}
