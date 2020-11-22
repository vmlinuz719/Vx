/* Visual Executive - Model
 * (C) 2020 vmlinuz719. All rights reserved
 * See COPYING for copyright notice. You may modify and redistribute this file
 * however you like, so long as it is redistributed along with the notice in
 * COPYING.
 
 * This file contains code used to read .vx configuration files. The format is
 * specified in vx.h. Code to work with the internal representation of program
 * groups is also present.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <Xm/Xm.h>

#include "vx.h"

int execProgram(Program *program, char *args) {
	char fullCommandBuffer[FULL_COMMAND_LEN];
	char *argv[MAX_ARGS];
	argv[0] = NULL;
	
	/* first, copy the basename
	 * then, concatenate any additional args
	 * check if it fits
	 */
	
	if (strlcpy(fullCommandBuffer, program->command, FULL_COMMAND_LEN)
			>= FULL_COMMAND_LEN ||
			strlcat(fullCommandBuffer, " ", FULL_COMMAND_LEN)
			>= FULL_COMMAND_LEN ||
			strlcat(fullCommandBuffer, args, FULL_COMMAND_LEN)
			>= FULL_COMMAND_LEN) {
		/* string was too long */
		
		return EXIT_FAILURE;
	}
	
	char *token = NULL, *last = NULL;
	int i = 0;
	
	for (token = strtok_r(fullCommandBuffer, " \t\n", &last);
			token;
			token = strtok_r(NULL, " \t\n", &last)) {
		if (i < MAX_ARGS - 1) {
			argv[i++] = token;
		}
	}
	argv[i] = NULL;
	
	/* generate pipe to get exit code of execvp */
	
	int execPipe[2];
	if (pipe(execPipe) < 0) {
		printf("Pipe failure\n");
		exit(EXIT_FAILURE);
	}
	
	/* run the program! */
	
	pid_t parent = fork();
	
	if (parent == -1) {
		close(execPipe[0]);
		close(execPipe[1]);
		return EXIT_FAILURE;
	} else if (!parent) {
		close(execPipe[0]);
		fcntl(execPipe[1], F_SETFD, FD_CLOEXEC);
		
		int errorCode = execvp(argv[0], argv);
		
		write(execPipe[1], &errorCode, sizeof(int));
		close(execPipe[1]);
		exit(EXIT_FAILURE);
	}
	
	close(execPipe[1]);
	
	int execResult = 0;
	read(execPipe[0], &execResult, sizeof(int));
	
	close(execPipe[0]);
	return execResult;	
}

void deleteProgram(Program *program) {
	free(program->visualName);
	free(program->command);
	
	if (program->next != NULL) {
		deleteProgram(program->next);
	}
	
	free(program);
}

void deleteProgramGroup(ProgramGroup *programGroup) {
	free(programGroup->visualName);
	
	if (programGroup->next != NULL) {
		deleteProgramGroup(programGroup->next);
	}
	
	if (programGroup->programs != NULL) {
		deleteProgram(programGroup->programs);
	}
	
	free(programGroup);
}

ProgramGroup
*readConfigFileFailed
(char *line, FILE *config, ProgramGroup *root) {
	free(line);
	fclose(config);
	deleteProgramGroup(root);
	return NULL;
}

ProgramGroup *readConfigFile(char *fileName) {
	FILE *config = fopen(fileName, "r");
	if (config == NULL) {
		return NULL;
	}
	
	ProgramGroup *root = malloc(sizeof(ProgramGroup));
	ProgramGroup *current = root;
	
	current->visualName = malloc(5 * sizeof(char));
	strcpy(current->visualName, "Main");
	current->next = NULL;
	
	current->programs = NULL;
	Program **currentProgram = &(current->programs);
	
	char *line = NULL;
	size_t lineBufferLength = 0;
	ssize_t lineLength = 0;
	
	lineLength = getline(&line, &lineBufferLength, config);
	
	while (lineLength >= 0) {
		
		/* Tokenize the line using strtok_r */
		
		char *strtokContext = NULL;
		
		char *typeToken = NULL;
		char *nameToken = NULL;
		
		typeToken = strtok_r(line, " \t\n", &strtokContext);
		
		if (typeToken == NULL) {
			/* ignore blank line */
		}
		else if (strcmp(typeToken, "grp") &&
				strcmp(typeToken, "pgm")) {
			return readConfigFileFailed(line, config, root);
		}
		
		else if (!strcmp(typeToken, "grp")) {
			nameToken = strtok_r(NULL, " \t\n", &strtokContext);
			
			if (nameToken == NULL) {
				return readConfigFileFailed(line, config, root);
			} else {
				if (strtok_r(NULL, " \t\n", &strtokContext) !=
						NULL) {
					return readConfigFileFailed
							(line, config, root);
				}
				
				/* add new group node */
				
				char *visualName = malloc(
						(strlen(nameToken) + 1) *
						sizeof(char));
				if (visualName == NULL) {
					return readConfigFileFailed
							(line, config, root);
				}
				
				strcpy(visualName, nameToken);
				
				ProgramGroup *newGroup = malloc(
						sizeof(ProgramGroup));
				if (newGroup == NULL) {
					return readConfigFileFailed
							(line, config, root);
				}
				
				newGroup->visualName = visualName;
				newGroup->programs = NULL;
				newGroup->next = NULL;
				
				current->next = newGroup;
				current = current->next;
				currentProgram = &(current->programs);
			}
		}
		
		else if (!strcmp(typeToken, "pgm")) {
			nameToken = strtok_r(NULL, " \t\n", &strtokContext);
			
			if (nameToken == NULL) {
				return readConfigFileFailed(line, config, root);
			} else {
				strtokContext[strlen(strtokContext) - 1] = 0;
				
				while(isspace(strtokContext[0])) {
					strtokContext += 1;
				}
				
				/* add new program node */
				
				char *visualName = malloc(
						(strlen(nameToken) + 1) *
						sizeof(char));
				if (visualName == NULL) {
					return readConfigFileFailed
							(line, config, root);
				}
				
				strcpy(visualName, nameToken);
				
				char *command = malloc(
						(strlen(strtokContext) + 1) *
						sizeof(char));
				if (command == NULL) {
					return readConfigFileFailed
							(line, config, root);
				}
				
				strcpy(command, strtokContext);
				
				*currentProgram = malloc(sizeof(Program));
				if (*currentProgram == NULL) {
					return readConfigFileFailed
							(line, config, root);
				}
				
				(*currentProgram)->visualName = visualName;
				(*currentProgram)->command = command;
				(*currentProgram)->next = NULL;
				
				currentProgram = &((*currentProgram)->next);
			}
		}
		
		lineLength = getline(&line, &lineBufferLength, config);
	}
	
	free(line);
	line = NULL;
	
	fclose(config);
	config = NULL;
	
	return root;
}

#ifdef TEST_GETLINE
int main(int argc, char *argv[]) {
	ProgramGroup *group = readConfigFile("config.vx");
	
	if (group != NULL) {
		ProgramGroup *current = group;
		while (current != NULL) {
			printf("Group %s\n", current->visualName);
			
			if (current->programs != NULL) {
				Program *currentProgram = current->programs;
				while (currentProgram != NULL) {
					printf("Program %s\n",
							currentProgram->
							visualName);
					currentProgram = currentProgram->next;
				}
			}
			
			current = current->next;
		}
		
		deleteProgramGroup(group);
	}
	
	return 0;
}
#endif
