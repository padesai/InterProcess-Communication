#include <stdio.h>
#include <stdlib.h>
#include "library.h"
#include "snappy.h"
#include <unistd.h>
#include <string.h>

void main(int argc, char* argv[3])
{

if (argc != 4) {
	fprintf(stderr, "Usage: object_file choice(1 or 2) for sync of asynch respectively, path1, filename2");
}

else {
int choice=atoi(argv[1]);
char *path1 = argv[2];
char *path2 = argv[3];
//strcat(path2,temp);
strcat(path2,".snp");

char *buff=(char*)malloc(1024*1024);
char *uncommp=(char*)malloc(1024*1024);
int *flag,count=0;

if (choice==1)
{
//sync 

	if(compress(path1,path2)!=1)
	{
	printf("error with compression");
	exit(0);
	}

	else
	{
	printf("file compressed and stored at current location with filename by sync calling ");
	}
 
}

else if(choice==2)
{

	flag=asynccompress(path1,path2);
	
	while(*flag==0)
	{
	printf("\n sleeping for 10 sec while w8ing for compression to be done\n");
	sleep(10);
	}
	cleanup(path1,path2);
	printf("file compressed and stored at location 2 async");
	//continue process
	}

printf("\n file compressed in requested method.");
}
}
