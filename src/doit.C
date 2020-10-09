/*
 * doit.C
 *
 *  Created on: Oct 9, 2020
 *      Author: nupurshukla
 */




/*
 * doit.C
 *
 *  Created on: Sep 7, 2020
 *      Author: nupurs
 *
 * NOTES:
 * - the function getrusage() and the data type struct rusage
 * are used to determine the resource usage of a process
 * - declared in sys/resource.h
 *
 * - the struct timeval structure represents an elapsed time
 * - it's declared in sys/time.h
 *
 * - the function gettimeofday() can get the time as well as a timezone
 * - tv_sec = seconds (whole number)
 * - tv_usec = microseconds (portion of a second)
 * - ru_utime
 * - user time used
 * - ru_stime
 * - system time used
 * - ru_nivcsw
 * - involuntary context switches
 * - ru_nvcsw
 * - voluntary context switches
 * - ru_majflt
 * - page faults
 * - ru_minflt
 * - page reclaims
 * - ru_maxrss
 * - maximum resident set size
 * - waitpid()
 * - will return the status of a child process
 * - pid_t
 * - data type (signed int) which is capable of representing a process ID
 * - token
 * - smallest element of a program that is meaningful to the compiler
 * - keywords, identifiers, constants, strings, special symbols, operators
 */

#include <iostream>
using namespace std;
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#define MAX_CHARS 128
#define MAX_ARGS 32

typedef struct {
	int pid;
	string cmd;
	long startTime;
} process;

char *prompt = (char *)malloc(16 * sizeof(char));
int amp = 0;
vector<process> children;

void printStatistics(long start_ms) {
	struct rusage u; //needed for the resource usage for a process
	struct timeval   // needed for elapsed time
	wallClock_end,
	userTime_end,
	systemTime_end;

	gettimeofday(&wallClock_end, NULL); //needed to get the current time
	long end_ms = (wallClock_end.tv_sec * 1000) + (wallClock_end.tv_usec / 1000);
	getrusage(RUSAGE_CHILDREN, &u); // needed to get the resource usage
	userTime_end = u.ru_utime;
	systemTime_end = u.ru_stime;
	double userTime = ((userTime_end.tv_sec * 1000) + (userTime_end.tv_sec / 1000));
	double systemTime = ((systemTime_end.tv_sec * 1000) + (systemTime_end.tv_sec / 1000));
	double wallClock = end_ms - start_ms;

	cout << "Usage Stats :\n";
	cout << "system time = " << systemTime << " milliseconds\n";
	cout << "user time = " << userTime << " milliseconds\n";
	cout << "wall clock time " << wallClock << " milliseconds\n";
	cout << "involuntary context switches = " << u.ru_nivcsw << endl;
	cout << "voluntary context switches = " << u.ru_nvcsw << endl;
	cout << "page faults requiring I/O = " << u.ru_majflt << endl;
	cout << "page faults serviced w/o I/O = " << u.ru_minflt << endl;
	cout << "maximum resident set size = " << u.ru_maxrss << " KB\n";

}

void exit() {
	if(children.size() > 0) {
		cout << "Currently waiting on " << children.size() << " processes to finish" << endl;
		for(unsigned long i = 0; i < children.size(); i++) {
			int status;
			pid_t r = waitpid(children.at(i).pid, &status, 0);
			if(r > 0) /* child has finished at this point */ {
				cout << "[" << i+1 << " ]" << children.at(i).pid << " completed.\n";
				printStatistics(children.at(i).startTime);
			}
		}
	}
	exit(0);
}


int execute(char ** newArgs){
	int pid;
	long start_ms;
	struct timeval
	wallClock_start;

	gettimeofday(&wallClock_start, NULL);
	start_ms = ((wallClock_start.tv_sec * 1000) + (wallClock_start.tv_usec / 1000));

	pid = fork();
	if (pid < 0) {
		cerr << "Fork error\n";
		exit(1);
	}
	else if (pid == 0){
		if(execvp(newArgs[0], newArgs) < 0) {
			cerr << "execvp error\n";
			exit(1);
		}
		return 0;
	}
	else {
		if(!amp) {
			wait(0);
			printStatistics(start_ms);
			return 0;
		}
		else {
			process child = {pid, newArgs[0], start_ms};
			children.push_back(child);
			cout << "[" << children.size() << " ]" << children.back().pid << endl;
			return 0;
		}
	}
}

int executeLine(char **out) {
	char L[MAX_CHARS];
	char *token;
	int position = 0;

	cin.getline(L, MAX_CHARS);
	token = strtok(L, " ");

	while (token != NULL) {
		out[position] = token;
		token = strtok(NULL, " ");
		position = position +1;
	}

	if (strcmp(out[0], "exit") == 0) {
		exit();
		return 1;
	}
	else if(strcmp(out[0], "cd") == 0 && out[1] != NULL) {
		if(chdir(out[1])!= 0) {
			cerr << "chdir error\n";
		}
	}
	else if(strcmp(out[0], "set") == 0 && strcmp(out[1], "prompt") == 0 && strcmp(out[2], "=") == 0 && out[3] != NULL) {
		strcpy(prompt, out[3]);

	}
	else {
		execute(out);
	}
	return 0;
}

int main(int argc, char *argv[]) {
	char ** newArgs = (char **)malloc(MAX_ARGS * sizeof(char *));
	int i;
	prompt[0] = '=';
	prompt[1] = '=';
	prompt[2] = '>';
	prompt[3] = '\0';

	if(argc > 1) {
		for(i=1; i<argc; i++) {
			newArgs[i-1] = argv[i];
		}
		newArgs[argc - 1] = NULL;
		execute(newArgs);
	}
	else {
		while (1) {
			cout << prompt << " ";

			char L[MAX_CHARS];
			char *token;
			int position = 0;

			cin.getline(L, MAX_CHARS);

			token = strtok(L, " ");
			while(token != NULL) {
				newArgs[position] = token;
				token = strtok(NULL, " ");
				position++;
			}
			if (strcmp(newArgs[0], "exit") == 0) {
				exit();
			}
			else if(strcmp(newArgs[0], "cd") == 0 && newArgs[1] != NULL) {
				if(chdir(newArgs[1]) != 0) {
					cerr << "chdir error\n";
				}
			}
			else if(strcmp(newArgs[0], "set") == 0 && strcmp(newArgs[1], "prompt") == 0 && strcmp(newArgs[2], "=") == 0 && newArgs[3] != NULL) {
				strcpy(prompt, newArgs[3]);

			}
			else {
				execute(newArgs);
			}
		}

	}

}
