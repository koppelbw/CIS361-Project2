#include "proj2.h"
#include <ctype.h>
#include <unistd.h>
#define TRUE 1
#define FALSE 0

extern pthread_mutex_t mutex;	
extern pthread_cond_t reading_done;
extern pthread_cond_t writing_done;
	
extern int  current;			 
extern char buffer[];

/* displays all printable characters*/
void displayText(char line[], int lettercnt){

	int i;
	for(i = 0; i < lettercnt; i++){
		if(isprint(line[i])){
			if(line[i] != ']')
				printf("%c", line[i]);
		}else{
			printf("--ERROR: NON-DISPLAYABLE CHARACTER.--");
		}
	}
}

/* executes all supported commands*/ 
void execComm(char comm[]){

	//this block of code contains all supported commands
	if(strcmp(comm, "NEWLN") == 0){
		printf("\n");
	}else if(strcmp(comm, "TAB") == 0){
		printf("\t");
	}else if(strcmp(comm, "DELAY") == 0){
		sleep(2);
	}else if(strcmp(comm, "ERASE") == 0){
		printf("\b \b");
	}else if(strcmp(comm, "OVER") == 0){
		printf("\n");
		exit(0);
	}else{
		char *error = "--ERROR: COMMAND NOT FOUND.--";
		displayText(error, strlen(error));
	}
}

/* This function reads in characters one at a time from BufferReader().
 * Then it processes the text.*/
void * controller(void * arg){

	int i, j, lettercnt;
	char ch = 'a';
	char line[30], *comm;

        while(ch != EOF){
		lettercnt = 0;

		//creates an array of chars, at most 30 chars.
                for(i = 0; i < 30; i++){
                        if(ch != EOF){
                                ch = bufferReader();
                                if(ch == '['){
                                        displayText(line, lettercnt);
					i = -1;
					lettercnt = 0;
					j = 0;
                                        comm = (char*)calloc(5, sizeof(char));

					//determines if string is a command
                                        while(j < 6 && ch != ']' && ch != EOF){
                                                ch = bufferReader();
						if(j == 5 && ch != ']'){
							char *error = "--ERROR"
							": INVALID COMMAND--";
							displayText(error,
							       strlen(error));
						j++;
						}else if(ch != ']'){
						        comm[j++] = ch;
						      }else{
							j = 6;
							execComm(comm);
							}
					}
                                        free(comm);
                                }else{
                                        line[i] = ch;
					lettercnt++;
                                }
                        }
                }
                	displayText(line, lettercnt);
        }
        return 0;
}

/*bufferReader() returns one character from the shared buffer.*/
char bufferReader(){

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
