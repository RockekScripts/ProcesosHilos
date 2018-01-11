#include "barrier.h"
#include <errno.h>

#ifdef POSIX_BARRIER

/* Wrapper functions to use pthread barriers */

int sys_barrier_init(sys_barrier_t* barrier, unsigned int nthreads)
{
	return pthread_barrier_init(barrier,NULL,nthreads);
}

int sys_barrier_destroy(sys_barrier_t* barrier)
{
	return pthread_barrier_destroy(barrier);
}

int sys_barrier_wait(sys_barrier_t *barrier)
{
	return pthread_barrier_wait(barrier);
}
#else



/* Barrier initialization function */
int sys_barrier_init(sys_barrier_t *barrier, unsigned int nr_threads)
{
	/* Initialize fields in sys_barrier_t
	     ... To be completed ....
	*/
	barrier->max_threads = nr_threads;
	barrier->nr_threads_arrived = 0;
	sem_init(&(barrier->cond),0,0);
	sem_init(&(barrier->mutex),0,0);
	return 1;
}

/* Destroy barrier resources */
int sys_barrier_destroy(sys_barrier_t *barrier)
{
	/* Destroy synchronization resources associated with the barrier
	      ... To be completed ....
	*/

	sem_destroy(&barrier->cond);
	sem_destroy(&barrier->mutex);
	return 1;
}

/* Main synchronization operation */
int sys_barrier_wait(sys_barrier_t *barrier)
{
	/* Implementation outline:
	   - Every thread arriving at the barrier acquires the lock and increments the nr_threads_arrived
	    counter atomically
	     * In the event this is not the last thread to arrive at the barrier, the thread
	       must block in the condition variable
	     * Otherwise...
	        1. Reset the barrier state in preparation for the next invocation of sys_barrier_wait() and
	        2. Wake up all threads blocked in the barrier
	   - Don't forget to release the lock before returning from the function

	    ... To be completed ....
	*/
	sem_wait(&barrier->mutex);
	barrier->nr_threads_arrived++;
	sem_post(&barrier->mutex);
	if(barrier->nr_threads_arrived<barrier->max_threads){
		sem_wait(&barrier->cond);
	}
	else{
		int i;
		for( i = 0 ; i < barrier->nr_threads_arrived - 1; i++)
			sem_post(&barrier->cond);
		barrier->nr_threads_arrived = 0;
	}
	return 1;
}

#endif /* POSIX_BARRIER */
