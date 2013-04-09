#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>

char* SERIALPORT = "/dev/ttyUSB0";
int fd;

int open_port()
{
	fd = open(SERIALPORT, O_RDWR | O_NOCTTY | O_NDELAY);
	if(fd == -1)
		perror("open_port: Unable to open serial port\n");
	else
		fcntl(fd, F_SETFL, 0);
	return fd;
}

int read_port()
{
	char buffer[255];  /* Input buffer */
	char *bufptr;      /* Current char in buffer */
	int  nbytes;       /* Number of bytes read */
	int  tries;        /* Number of tries so far */

	while(1)
	{
	/* read characters into our string buffer until we get a CR or NL */
		bufptr = buffer;
		while ((nbytes = read(fd, bufptr, buffer + sizeof(buffer) - bufptr - 1)) > 0)
		{
//printf("nbyte: %d\n", nbytes);
			bufptr += nbytes;
			if (bufptr[-1] == '\n' || bufptr[-1] == '\r')
				break;
		}

		/* nul terminate the string and see if we got an OK response */
		*bufptr = '\0';
printf("Read: %s\n", buffer);
		//if (strncmp(buffer, "OK", 2) == 0)
		//return nbytes;
	}
}

void signal_callback_handler(int signum)
{
	if(signum == SIGINT)
	{
		close(fd);
		exit(-1);
	}
}

int main(int argc, char** argv)
{
	if(argc > 1)
		SERIALPORT = argv[1];
	printf("Beginning to read from %s\n", SERIALPORT);
	signal(SIGINT, signal_callback_handler);
	int fd = open_port();
	read_port();
	close(fd);
	return 0;
}
