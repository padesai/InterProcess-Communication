#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include "snappy.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <fcntl.h> 
#include <mqueue.h> 

#define SERVER_QUEUE_NAME "/file_requests" 
#define QUEUE_PERMISSIONS 0660 
#define MAX_MESSAGES 10 
#define MAX_MSG_SIZE 256 
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

void main()
{

mqd_t qd_server, qd_client;

struct mq_attr attr;

attr.mq_flags = 0;
attr.mq_maxmsg = MAX_MESSAGES;
attr.mq_msgsize = MAX_MSG_SIZE;
attr.mq_curmsgs = 0;

if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
	perror ("Server: mq_open (server)");
	exit(1);
}

char in_buffer [MAX_MSG_SIZE];
	
while (1) {

	if (mq_receive (qd_server, in_buffer, 256, NULL) == -1) {

	perror ("Server: mq_receive");
	exit(1);
	}


//printf("Server: message keys recieved.\n");

   const char s[2] = ",";
   char *paths[4];
   char *token;
   
   token = strtok(in_buffer, s);

   
  
   
   int i = 0;
	paths[i] = token;
   while( token != NULL ) 
   {
	i++;
        token = strtok(NULL , s);
	paths[i] = token;

	

   }
char* path = paths[0];
char* path2 = paths[1];
char* projid_char = paths[2];

int shmid,shmid2,flag;
key_t key1, key2, key3;
int projid = atoi(projid_char); 

key1 = ftok(path, projid);
key2 = ftok(path2, projid);
key3 = ftok(path2, projid+1);

printf("\nProject id= %s",projid_char);


int *flagg;
char *shm,*buff=(char*)malloc(1024*1024),*op;
int shmflg=0666;
size_t size=1024*1024,opsize;

if ((shmid = shmget (key1, size, SHM_RDONLY)) <0)
 {
   perror("shmget: shmget failed"); exit(1); 
 }
if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
{
 perror("shmget: shmat failed"); exit(1);
}
//2nd memory segment
if ((shmid2 = shmget (key2, size, 0666)) <0)
 {
   perror("shmget: shmget failed"); exit(1); 
 }
if ((op = shmat(shmid2,NULL, 0)) == (char *) -1)
{
 perror("shmget: shmat failed"); exit(1);
}

struct snappy_env env;
memcpy(buff,shm,strlen(shm));
snappy_init_env(&env);
//snappy compression and write that to 2nd shared momory segment i.e. op
if(snappy_compress(&env, buff,strlen(buff), op,&opsize )!=0)
{
 perror("snappy compress failed");exit(0);
}

//set *flagg to 1
if ((flag = shmget (key3, sizeof(int), shmflg)) <0)
{
 perror("shmget: shmget failed"); exit(1); 
}

if ((flagg = shmat(flag, NULL, 0)) ==(int *) -1)
{
 perror("shmget: shmat failed"); exit(1);
}


*flagg=1;//setting that compression is done

shmdt(flagg);
shmdt(op);
shmdt(shm);
free(buff);



}

}
