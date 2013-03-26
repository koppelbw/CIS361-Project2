#include "proj2.h"
#include <ctype.h>

extern pthread_mutex_t mutex;	
extern pthread_cond_t reading_done;
extern pthread_cond_t writing_done;
	
extern int  current;			 
extern char buffer[];

void displayText(char line[]){
printf("--");
	int i;
	for(i = 0; i < strlen(line); i++){
		if(isprint(line[i])){
			printf("%c", line[i]);
		}else{
			printf("\nERROR: NON-DISPLAYABLE CHARACTER.\n");
		}
	}
}

void execComm(char comm[]){
	printf("\nExecute Command: %s\n", comm);
	//use if/else ladder	
}

void * controller(void * arg)
{
	//ignore the parameter arg which is not useful in this project	

	//use bufferReader() to receive one character from the buffer
	//shared between threads and process it; do this repeatedly
	//until EOF is encountered. also define functions displayText() 
	//and execComm() to perform the required actions.

	int i, j;
	char ch = 'a';
	char *line;
	char *comm;
	while(ch != EOF){
		for(i = 0; i < 30; i++){
			line = (char*)calloc(30, sizeof(char));
		        if(ch != EOF){
				ch = bufferReader();
				if(ch == '['){
					printf("[");
				        displayText(line);
					comm = (char*)calloc(5, sizeof(char));
					for(j = 0; j < 5; j++){
						ch = bufferReader();
						if(ch == ']'){     
							execComm(comm);
						}else if(j == 4 && ch != ']')
							printf("\nERROR: UNRECOGNIZABLE COMMAND.\n");
						      else
							comm[j] = ch;
					}	
					free(comm);
				}else{
					line[i] = ch;
				}
//printf("%c\n", ch);	
			}
		}
		displayText(line);
		free(line);
	}
	return 0;
}

// bufferReader() returns one character from the shared buffer 

char bufferReader()
{
   	char ch;
   	pthread_mutex_lock( &mutex );

	// wait if the next set of characters is not ready yet
	if (current == 7) 
		pthread_cond_wait( &writing_done, &mutex );
	
	 ch = buffer[current++];

	// signal to writer if the current set of characters is finished  
	if (current == 7)
		pthread_cond_signal(&reading_done);
	
	pthread_mutex_unlock( &mutex );

	return ch;
}
