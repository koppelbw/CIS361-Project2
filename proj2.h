#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void bufferWriter(char buff[]);
char bufferReader();
void * devDriver(void * arg);
void * controller(void * arg);
