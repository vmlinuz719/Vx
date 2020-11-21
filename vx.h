/* Visual Executive - Data Types
 * (C) 2020 vmlinuz719. All rights reserved
 * See COPYING for copyright notice. You may modify and redistribute this file
 * however you like, so long as it is redistributed along with the notice in
 * COPYING.
 
 * This file contains data types used by the Visual Executive to represent
 * program groups. A Java-like style is used throughout the source code to
 * improve readability even though it is not idiomatic for C.
 */

typedef struct pgm {
	/* Program: Data structure to represent a program or program list.
	 * Fields:
	 *	char *visualName:	Display name of the program
	 *	char *command:		Command to execute when selected
	 *	struct pgm *next:	Next Program in the list or NULL
	 */
	
	char *visualName;
	/* TODO: add icon support */	
	char *command;
	
	struct pgm *next;
} Program;

typedef struct grp {
	/* ProgramGroup: A container for a list of programs.
	 * Fields:
	 *	char *visualName:	Display name of the group
	 *	Program *programs:	List of programs
	 *	struct grp *next:	Next ProgramGroup in the list or NULL
	 */
	
	char *visualName;
	/* TODO: add icon support */
	Program *programs;
	
	struct grp *next;
} ProgramGroup;

/* ProgramGroup *readConfigFile(char *fileName)
 * Parameters:
 *	char *fileName:	Path of the config file to read
 *	Format:
 *		grp Group1
 *			pgm visualName command
 *			...
 *		...
 * Returns: a ProgramGroup list containing all programs in the config file
 */

ProgramGroup *readConfigFile(char *fileName);

/* int execProgram(Program *program)
 * Parameters:
 *	Program *program:	Program to run using fork() and exec()
 * Returns:
 *	0 on success, non-zero on error
 */

int execProgram(Program *program, char *args);

/* void deleteProgram(Program *program) - free()s a program */

void deleteProgram(Program *program);

/* void deleteProgramGroup(ProgramGroup *programGroup) -
 * free()s a program group
 */

void deleteProgramGroup(ProgramGroup *programGroup);

/* Preprocessor config options */

#define TEST_GETLINE
#define TYPE_LEN 3
#define VISUALNAME_LEN 16
#define COMMAND_LEN 128
#define FULL_COMMAND_LEN 256
#define MAX_ARGS 256
