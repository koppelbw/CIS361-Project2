#include "proj2.h"

// mutex and condition variables to ensure correct access to shared memory
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reading_done = PTHREAD_COND_INITIALIZER;
pthread_cond_t writing_done = PTHREAD_COND_INITIALIZER;

// memory shared between threads
int current = 7;
char buffer[7];

int main()
{
   	int rc1, rc2;
   	int x = 1, y = 2;
   	pthread_t thread1, thread2;

   	/* create two threads, each runing a different function */

   	if( (rc1=pthread_create( &thread1, NULL, devDriver, &x)) )
   	{
      		printf("Thread creation failed: %d\n", rc1);
   	}

   	if( (rc2=pthread_create( &thread2, NULL, controller, &y)) )
   	{
      		printf("Thread creation failed: %d\n", rc2);
   	}

   	/* wait till each thread is complete before main continues */

   	pthread_join( thread1, NULL);
   	pthread_join( thread2, NULL); 

   	printf("\nthe end\n");

   	return 0;
}

