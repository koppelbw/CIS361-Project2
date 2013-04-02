#include "proj2.h"
#define true 1
#define false 0

extern pthread_mutex_t mutex;		
extern pthread_cond_t reading_done;
extern pthread_cond_t writing_done;

extern int  current;			 
extern char buffer[];

/* This function retrieves text from the specified text file and 
 * sends it to BufferWriter().*/
void *devDriver(void * arg){

	char ch = 'a', tmp[7];
	int end = false, i;

	FILE *ifp;
	ifp = fopen("test3.txt", "r");
	
	if(ifp == NULL){
		printf("Testing file not found!");
		exit(1);
	}

	//sends the entire text file to BufferWriter() 7 chars at a time
	while(end != true){
		for(i = 0; i < 7; i++){
			if(ch == EOF)
				end = true;
			else{
				ch = fgetc(ifp);
				tmp[i] = ch;
			}
		}
		bufferWriter(tmp);
	}
	return 0;
}


/* bufferWriter() stores the seven characters in array data into the 
 * shared buffer*/
void bufferWriter(char data[])
{
   	pthread_mutex_lock(&mutex);

	// wait if the current set of characters is finished yet
	if (current < 7) 
		pthread_cond_wait(&reading_done, &mutex);
 	strncpy(buffer, data, 7);
	current = 0;
	// signal to reader when the next set of characters is ready
	pthread_cond_signal(&writing_done);

  	pthread_mutex_unlock(&mutex);
}
