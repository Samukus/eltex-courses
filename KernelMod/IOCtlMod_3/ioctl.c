#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "ioctl.h"

int main( int argc, char *argv[] ) {
   int dfd;                  // дескриптор устройства 
   if( ( dfd = open( DEVPATH, O_RDWR ) ) < 0 ) {
	printf("Error opening device\n");
	return -1;
   }
   printf("IOCtl return: %d\n", ioctl( dfd, IOCTL_GET_RANDOM) );
   close( dfd );
   return EXIT_SUCCESS;
};


