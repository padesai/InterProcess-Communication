#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <fcntl.h>
#include <unistd.h> 
#include <sys/stat.h> 
#include <mqueue.h>
#include "library.h" 

#define SERVER_QUEUE_NAME "/file_requests" 
#define QUEUE_PERMISSIONS 0660 
#define MAX_MESSAGES 10  


int compress(char *path, char *path2)
{

key_t key,key2,key3; 
int shmflg=IPC_CREAT | 0666;
size_t size=1024*1024;
int shmid,shmid2; 
int projid=getpid();
printf("\nprojid=%d",projid);
char *shm,*buff=(char*)malloc(size);
int *flagg;
int flag;
int rtrn1, rtrn2, rtrn3;

FILE *inputf;
key=ftok(path,projid);
inputf=fopen(path2,"w");
fclose(inputf);

// storing the file in shared memory
if ((shmid = shmget (key, size, shmflg))<0 )
 {
   perror("shmget: shmget failed"); exit(1); 
 }

if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
{
 perror("shmget: shmat failed"); exit(1);
}

inputf = fopen(path, "r");

if (inputf == NULL) 
{
 perror ("Error opening file ");exit(0);
}

else 
{
	if(fgets(buff, size, inputf) !=NULL)
	{

	memcpy(shm,buff,strlen(buff));
	}
}

fclose(inputf);
shmdt(shm);
free(buff);

key2=ftok(path2,projid);
if ((shmid2 = shmget (key2, size, IPC_CREAT|0666)) <0)
 {
   perror("shmget: shmget failed"); exit(1); 
 }
//3rd shared memory segement
key3=ftok(path2, projid+1); 

if ((flag = shmget (key3, sizeof(int), shmflg))<0)
 {
   perror("shmget: shmget failed"); exit(1); 
 }
if ((flagg = shmat(flag, NULL, 0)) ==(int *) -1)
{
 perror("shmget: shmat failed"); exit(1);
}
if(*flagg==1)
{ goto done; //for same process requesting same file while the shm flag still exists will be given the existing compressed file.
}
*flagg=0;


//message passing the ids to the server
//pass the keys
mqd_t qd_server;

if ((qd_server = mq_open(SERVER_QUEUE_NAME, O_WRONLY)) == -1) {
perror("Client:mq_open(client)");
exit(1);
}

char *in_buffer=(char*)malloc(100);
char *projid_char=(char*)malloc(1);
sprintf(projid_char,"%d\n",projid);
strcpy(in_buffer,path);
strcat(in_buffer, ",");
strcat(in_buffer, path2);
strcat(in_buffer, ",");
strcat(in_buffer, projid_char); 
strcat(in_buffer, "\0");


if ( mq_send(qd_server, in_buffer,256, 0) == -1) 
{ 
perror("Client: Not able to send message to server"); 
exit(0);
}


while(*flagg==0)
{

}



buff=(char*)malloc(size);
inputf=fopen(path2,"w");
if ((shmid2 = shmget (key2, size, 0666)) <0)
 {
   perror("shmget: shmget failed"); exit(1); 
 }
if ((shm = shmat(shmid2, NULL, 0)) == (char *) -1)
{
 perror("shmget: shmat failed"); exit(1);
}
memcpy(buff,shm,strlen(shm));
fputs(buff,inputf);
free(buff);
fclose(inputf);
done:
shmdt(flagg);


shmdt(shm);
if ((rtrn1 = shmctl(shmid, IPC_RMID, NULL)) == -1) {
    perror("shmctl1: shmctl failed");
    exit(1);
   }
if ((rtrn2 = shmctl(shmid2, IPC_RMID,NULL)) == -1) {
    perror("shmctl2: shmctl failed");
    exit(1);
   }
if ((rtrn3 = shmctl(flag, IPC_RMID, NULL)) == -1) {
    perror("shmctl3: shmctl failed");
    exit(1);
   }
return(1);
}


int *asynccompress(char *path, char*path2)
{

key_t key,key2,key3; 
int shmflg=IPC_CREAT | 0666;
size_t size=1024*1024;
int shmid,shmid2; 
int projid=getpid();
printf("\n %d\n",projid);
char *shm,*buff=(char*)malloc(size);
int *flagg;
int flag;
int rtrn1, rtrn2, rtrn3;
FILE *inputf;
key=ftok(path,projid);
inputf=fopen(path2,"w");
fclose(inputf);

// storing the file in shared memory
if ((shmid = shmget (key, size, shmflg)) <0)
 {
   perror("shmget1: shmget failed"); exit(1); 
 }

if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
{
 perror("shmat1: shmat failed"); exit(1);
}

inputf=fopen(path, "r");

if (inputf == NULL) { perror ("Error opening file");exit(0);}
else 
{
	if(fgets(buff, size, inputf) !=NULL)
	{

	memcpy(shm,buff,strlen(buff));
	}
}

fclose(inputf);
shmdt(shm);
free(buff);
//creating 2nd shared memory segment for the compressed file

key2=ftok(path2,projid);
if ((shmid2 = shmget (key2, size, shmflg)) <0)
 {
   perror("shmget2: shmget failed"); exit(1); 
 }

//3rd shared memory segement
key3=ftok(path2, projid+1); 
if ((flag = shmget (key3, sizeof(int), shmflg)) <0)
 {
   perror("shmget3: shmget failed"); exit(1); 
 }
if ((flagg = shmat(flag, NULL, 0)) <0)
{
 perror("shmat3: shmat failed"); exit(1);
}

*flagg=0;

//message passing the ids to the server
//pass the keys
mqd_t qd_server;

if ((qd_server = mq_open(SERVER_QUEUE_NAME, O_WRONLY)) == (-1)) {
perror("Client:mq_open(client)");
exit(1);
}

char *in_buffer=(char*)malloc(100);
char *projid_char=(char*)malloc(1);
sprintf(projid_char,"%d",projid);
strcpy(in_buffer,path);
strcat(in_buffer, ",");
strcat(in_buffer, path2);
strcat(in_buffer, ",");
strcat(in_buffer, projid_char); 
strcat(in_buffer, "\0");
printf("%s",in_buffer);
if ( mq_send(qd_server, in_buffer, 256,0 ) == -1) //see
{ 
perror("Client: Not able to send message to server"); }


printf("message sent returning to main %d",*flagg);
return flagg;

}

void cleanup(char* path, char* path2) {

key_t key,key2,key3; 
int shmflg=0666;
int size=1024*1024;
int shmid,shmid2; 
int projid=getpid();
char *shm,*buff=(char*)malloc(size);
int flag;
int rtrn1, rtrn2, rtrn3;
FILE *inputf;

key=ftok(path,projid);
key2=ftok(path2, projid);
key3=ftok(path2, projid+1);

if ((shmid = shmget (key, size, shmflg)) <0)
 {
   perror("shmget1: shmget failed"); exit(1); 
 }
if ((shmid2 = shmget (key2, size, shmflg)) <0)
 {
   perror("shmget2: shmget failed"); exit(1); 
 }
if ((flag = shmget (key3, sizeof(int), shmflg)) <0)
 {
   perror("shmget3: shmget failed"); exit(1); 
 }

inputf=fopen(path2,"w");
if ((shm = shmat(shmid2, NULL, 0)) == (char *) -1)
{
 perror("shmget2: shmat failed"); exit(1);
}

memcpy(buff,shm,strlen(shm));
fputs(buff,inputf);
fclose(inputf);
free(buff);
shmdt(shm);
if ((rtrn1 = shmctl(shmid, IPC_RMID, (struct shmid_ds *) NULL)) == -1) {
    perror("shmctl1: shmctl failed");
    exit(1);
   }
if ((rtrn2 = shmctl(shmid2, IPC_RMID, (struct shmid_ds *) NULL)) == -1) {
    perror("shmctl2: shmctl failed");
    exit(1);
   }
if ((rtrn3 = shmctl(flag, IPC_RMID, (struct shmid_ds *) NULL) )== -1) {
    perror("shmctl3: shmctl failed");
    exit(1);
   }

}


