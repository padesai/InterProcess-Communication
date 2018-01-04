#ifndef LIBRARY_H
#define LIBRARY_H

#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <mqueue.h>

#define SERVER_QUEUE_NAME "/file_requests" 
#define QUEUE_PERMISSIONS 0660 
#define MAX_MESSAGES 10  


int compress(char *path, char*path2);
int *asynccompress(char *path, char*path2);
void cleanup(char* path, char* path2);



#endif
