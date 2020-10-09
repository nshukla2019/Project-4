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
#include <sys/mman.h>

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
	int total_print_chars=0;
	int percentage;
	// ./proj4 is first argument and file is second


	char *pchFile;
	struct stat sb;

	if(argc < 3) {
		if((fd = open(argv[1], O_RDONLY)) < 0) {
			printf("Error, file could not open");
		}

		while((cnt = read(fd, buf, BUFSIZE)) > 0) {
			fileSize = cnt; //inside or out
			for(i = 0; i < cnt; i++) {
				if(isprint(buf[i]) > 0 || isspace(buf[i]) > 0) {
					total_print_chars = total_print_chars+1;
				}
			}
		}

		percentage = (total_print_chars/fileSize)*100;

		close(fd);
		printf("%d printable characters out of %d bytes, %d%% \n", total_print_chars, fileSize, percentage);

	}

	// mmap(NULL, BUFSIZE, sb.st_size, PROT_READ, MAP_SHARED, fd, 0
	// if input is mmap, use mmap() system call to map contents of srcfile to memory
	// iterate through memory to count printable characters

	else if (argc == 3) {
		if(strcmp(argv[2], "mmap") == 0) {
			byte_chunk = 0;

			if((fd = open(argv[1], O_RDONLY)) < 0) {
				printf("Error, file could not open");
			}

			else {
				int err = fstat(fd, &sb);

				pchFile = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

				if(pchFile == "MAP_FAILED") {
					printf("Mapping Failed");
				}
				else {
					fileSize = sb.st_size;
					for (i = 0; i < sb.st_size; i++) {
						if(isprint(pchFile[i]) > 0 || isspace(pchFile[i]) > 0) {
							total_print_chars = total_print_chars+1;
						}
					}
				}

				percentage = (total_print_chars/fileSize)*100;

				close(fd);

				if(munmap(pchFile, sb.st_size) < 0) {
					printf("Could not unmap memory");
				}

				printf("%d printable characters out of %d bytes, %d%% \n", total_print_chars, fileSize, percentage);

			}
		}

		else if ((byte_chunk = atoi(argv[2])) > 8192){
			printf("Number of bytes of chunk is too large (exceeded 8192)\n");
			printf("Setting to %d bytes\n", BUFSIZE);
			byte_chunk = BUFSIZE;
		}
		else {

			byte_chunk = atoi(argv[2]);

			if((fd = open(argv[1], O_RDONLY)) < 0) {
				printf("Error, file could not open");
			}

			while((cnt = read(fd, buf, byte_chunk)) > 0) {
				fileSize = cnt; //inside or out
				for(i = 0; i < cnt; i++) {
					if(isprint(buf[i]) > 0 || isspace(buf[i]) > 0) {
						total_print_chars = total_print_chars+1;
					}
				}
			}

			percentage = (total_print_chars/fileSize)*100;

			close(fd);
			printf("%d printable characters out of %d bytes, %d%% \n", total_print_chars, fileSize, percentage);


		}
	}


	return EXIT_SUCCESS;
}
