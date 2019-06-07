// Adam Rockett, CSCI 247, Assignment 3
// March 15, 2018

/*
	A situation where the server is a producer of data and the client is a consumer
	of that data is streaming a video. The client could be watching it, but only need
	to buffer the video every so often, so the server could give the data in
	segments while the viewer is watching the video.
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "shm.h"


int main(int argc, char* argv[])
{
	int retVal = 0;
	int fd;
	shmData *shmPtr;
	int len = sizeof(shmPtr);
	const char *memname = "/Assignment3";

  // <Confirm argc is 2 and if not print a usage string.>
	if (argc != 2) {
		printf("Error: Not 2 arguments passed\n");
	}

  // <Use the POSIX "shm_open" API to open file descriptor with
  //   "O_CREAT | O_RDWR" options and the "0666" permissions>
	fd = shm_open(memname, O_CREAT | O_RDWR, 0666);

  // <Use the "ftruncate" API to set the size to the size of your
  //   structure shm.h>
	ftruncate(fd, len);

  // <Use the "mmap" API to memory map the file descriptor>
	shmPtr = mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  // <Set the "status" field to INVALID>
	shmPtr->status = INVALID;
  // <Set the "data" field to atoi(argv[1])>
	shmPtr->data = atoi(argv[1]);
  // <Set the "status" field to VALID>
	shmPtr->status = VALID;


  printf("[Server]: Server data Valid... waiting for client\n");

  while(shmPtr->status != CONSUMED)
    {
      sleep(1);
    }

  printf("[Server]: Server Data consumed!\n");

  // <use the "munmap" API to unmap the pointer>
	munmap(shmPtr, len);

  // <use the "close" API to close the file Descriptor>
	close(fd);

  // <use the "shm_unlink" API to revert the shm_open call above>
	shm_unlink(memname);

  printf("[Server]: Server exiting...\n");


  return(retVal);

}
