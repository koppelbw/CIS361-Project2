#include "proj2.h"
#include <ctype.h>
#define TRUE 1
#define FALSE 0
extern pthread_mutex_t mutex;	
extern pthread_cond_t reading_done;
extern pthread_cond_t writing_done;
	
extern int  current;			 
extern char buffer[];

void displayText(char line[], int lettercnt){


printf("\nlc: %d\n",lettercnt);	


//char *tmp;
	//tmp = (char*)calloc(lettercnt, sizeof(char));
	//strncpy(tmp, line, lettercnt);

//printf("lettcnt: %d\n", lettercnt);
	int i;
	for(i = 0; i < lettercnt; i++){
		if(isprint(line[i])){
			printf("%c", line[i]);
		}else{
			printf("\nERROR: NON-DISPLAYABLE CHARACTER.\n");
		}
	}
//	free(tmp);
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

	int i, j, lettercnt;
	char ch = 'a';
	char line[30];
	char *comm;
//	char *tmp;

        while(ch != EOF){
		lettercnt = 0;
                for(i = 0; i < 30; i++){
                        if(ch != EOF){
                                ch = bufferReader();
                                if(ch == '['){
                                        displayText(line, lettercnt);
					i = -1;
					lettercnt = 0;
					j = 0;
					//printf("[");
                                        comm = (char*)calloc(5, sizeof(char));
                                        while(j < 6 && ch != ']' && ch != EOF){
                                                ch = bufferReader();
						if(j == 5 && ch != ']'){
                                                        printf("\nERROR: INVALID COMMAND.\n");
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
//printf("%c\n", ch);
                        }
                }
                	displayText(line, lettercnt);
        }
        return 0;

}
/*
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

*/
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
