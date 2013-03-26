#include "proj2.h"
#define true 1
#define false 0
extern pthread_mutex_t mutex;		
extern pthread_cond_t reading_done;
extern pthread_cond_t writing_done;

extern int  current;			 
extern char buffer[];

void * devDriver(void * arg)
{
	//igore the parameter arg which is not useful in this project	

	//read seven characters from the input file into an array
	//and use bufferWritter() with that array as the parameter
	//to store those characters in the buffer shared between 
	//threads; do this repeatedly until EOF is reached.  
	// note that EOF needs to be stored in the buffer as well. 

	FILE *ifp;
	ifp = fopen("test.txt", "r");
	
	if(ifp == NULL){
		printf("Testing file not found!");
		exit(1);
	}

char ch = 'a';
int end = false;
char tmp[7];

	while(end != true){
		int i;
		for(i = 0; i < 7; i++){
			if(ch == EOF)
				end = true;
			else{
				ch = fgetc(ifp);
				tmp[i] = ch;
//	printf("%c", tmp[i]);
			}
		}
		bufferWriter(tmp);
	}
	return 0;
}


// bufferWriter() stores the seven characters in array data into the  
// shared buffer

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
