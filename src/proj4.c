/*
 ============================================================================
 Name        : proj4.c
 Author      : Nupur Shukla
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFSIZE 1024


int main(int argc, char* argv[]) {

	// takes a file name as a command-line argument and computes the
	// percentage of printable characters in the file
	// use two Linux routines isprint() and isspace()
	// isprint() which determines if a byte value is a printable character
	// isspace() which determines if a byte is a space
	// if either returns true, the character is printable
	// output from program:
		// total number of printable characters
		// total number of bytes
		// percentage printed AS AN INT
	// default uses the read system call

	size_t byte_chunk; // this is to be used when user enters a num
	int fd; // file descriptor
	char buf[BUFSIZE]; //character array where the read content will be stored
	int i;
	int cnt;
	int fileSize;
	int total_print_chars;
	// ./proj4 is first argument and file is second

	if(argc < 3) {
		if((fd = open(argv[1], O_RDONLY)) < 0) {
			printf("Error, file could not open");
		}

		else {
			printf("File is open.");
		}

		while((cnt = read(fd, buf, BUFSIZE)) > 0) {
			fileSize = cnt; //inside or out
			for(i = 0; i < BUFSIZE; i++) {
				if(isprint(buf[i]) > 0 || isspace(buf[i]) > 0) {
					total_print_chars = total_print_chars + 1;
				}
			}
		}
		printf("%d printable characters out of %d bytes/n", total_print_chars, fileSize);

	}



		/*int fd = open(argv[1], O_RDONLY);

		char buff[BUFSIZE];
		int count = 0;

		 for (count = 0; read(fd, &buff[count], 1) > 0; count++) {
			 file_size += count;
			 if(isprint(buff[count]) > 0 || isspace(buff[count]) > 0) {
				 total_print_chars++;
			 }
		}*/
/*
	if(argc > 2) {
		if(strcmp(argv[3], "mmap") == 0) {
			byte_chunk = 0;
		}
		else {
			byte_chunk = atoi(argv[3]);
		}
	}


	puts("!!!Hello World!!!")*/; /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
