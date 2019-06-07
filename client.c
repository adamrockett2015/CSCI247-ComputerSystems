// Adam Rockett, CSCI 247, Assignment 3
// March 15, 2018

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
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

  // <Use the POSIX "shm_open" API to open file descriptor with
  //   "O_RDWR" options and the "0666" permissions>
  fd = shm_open(memname, O_CREAT | O_RDWR, 0666);

  // <Use the "mmap" API to memory map the file descriptor>
  shmPtr = mmap(0, len, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

  printf("[Client]: Waiting for valid data ...\n");


  while(shmPtr->status != VALID)
    {
      sleep(1);
    }

  printf("[Client]: Received %d\n",shmPtr->data);

  shmPtr->status = CONSUMED;

  //  <use the "munmap" API to unmap the pointer>
  munmap(shmPtr, len);

  printf("[Client]: Client exiting...\n");

  return(retVal);

}
