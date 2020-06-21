#include <err.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#define wordSize 10
#define maxArgs 4

void printArray(char * arr) {
	//execl("echo", "echo", "-n", arr, " ", (char*)0);
	for(int i = 0; arr[i] != '\0'; i++) {
		write(1, arr + i, 1);
	}
	
	write(1, " ", 1);
}

void printCommandArgs(char args[][wordSize + 1]) {
	for(int i = 0; args[i][0] != '\0'; i++) {
		printArray(args[i]);
	}
	
	write(1, "\n", 1);
}

void printExecArgs(char * args[maxArgs + 1]) {
	for(int i = 0; args[i] != NULL; i++) {
		printArray(args[i]);
	}
	
	write(1, "\n", 1);
}

//void executeCommandInCurrentProcess(char * args[maxArgs + 1], char * command) {
//	if(args[2][0] == '\0') {
//		execp(command, command, args[0], args[1], (char*)0);
//	} else if(args[1][0] == '\0') {
//		execlp(command, command, args[0], (char*)0);
//	}
//}

void executeCommand(char args[][wordSize + 1], char * command) {
	pid_t pid = fork();
	if(pid == 0) {
		char * execvArgs[maxArgs + 2];
		execvArgs[0] = command;

		int i = 1;
		int j = 0;
		while(args[j][0] != '\0') {
			execvArgs[i] = args[j];
			j++;
 			i++;
		}
	
		execvArgs[i] = NULL;
		if(execvp(command, execvArgs) < 0) {
			err(5, "Exec failed");	
		}
		return;
	}
	
	wait(NULL);
}

int main(int argc, char * argv[]) {
	
	if(argc > 2) {
		errx(1, "Too many commands");
	} 
	
	char command[wordSize + 1];
	if(argc < 2) {
		strcpy(command, "echo");
	} else if(strlen(argv[1]) <= 4) {
		strcpy(command, argv[1]);
	} else {
		errx(2, "Invalid length of command");
	}

	char arg[wordSize + 1];
	int index = 0;
	int row = 0;
	char commandArgs[maxArgs + 1][wordSize + 1];
	int readReturn;
	
	while((readReturn = read(0, arg + index, 1)) > 0 && index <= wordSize) {

		if(row == maxArgs) {
			commandArgs[maxArgs][0] = '\0';
			executeCommand(commandArgs, command);
			row = 0;
		}

	    if(arg[index] == '\n' || arg[index] == ' ') {
			arg[index] = '\0';
			strcpy(commandArgs[row++], arg); 
			index = 0;
		} else {
		  	index++;	
		}
	}

	if(readReturn < 0) {
		err(3, "Error during read");
	}

	if(index > wordSize) {
		errx(4, "Arguments too long");
	}

	if(row != 0) {
		commandArgs[row][0] = '\0';
		executeCommand(commandArgs, command);
	}
	
	return 0;
}
