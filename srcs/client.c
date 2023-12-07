#include "mytest.h"

void clientOrientedIo() {

	#ifdef TIMES
		struct timeval stime, etime;
		long int time_result;
	#endif
		/* Client_oriented_io. Measure io time, communication time, and time for the rest.
			*/

	#ifdef TIMES
		gettimeofday(&stime, NULL);
	#endif

	coRun();

	#ifdef TIMES
		gettimeofday(&etime, NULL);
		time_result = etime.tv_usec - stime.tv_usec;
		printf("Client_oriented_io TIMES == %ld %ld %ld\n", etime.tv_usec, stime.tv_usec, time_result);
	#endif

}