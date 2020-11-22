/* Visual Executive - Data Types
 * (C) 2020 vmlinuz719. All rights reserved
 * See COPYING for copyright notice. You may modify and redistribute this file
 * however you like, so long as it is redistributed along with the notice in
 * COPYING.
 
 * This file contains callbacks that are used to update the view. It is also
 * responsible for initializing the view and communicating with the model.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <Xm/Xm.h>
#include <Xm/MessageB.h>
#include <Xm/Text.h>

#include "vx.h"

ProgramGroup *groups;
Program *currentGroup;

void setListContents(Widget widget, int n, char *contents[], int maxSize) {
	XmStringTable items = (XmStringTable)XtMalloc(n * sizeof(XmString));
	for (int i = 0; i < n; i++)
		items[i] = XmStringCreateLocalized(contents[i]);
	XtVaSetValues(widget,
			XmNvisibleItemCount, n > maxSize ? maxSize : n,
			XmNitemCount, n,
			XmNitems, items,
			NULL);
	for (int i = 0; i < n; i++)
		XmStringFree(items[i]);
	XtFree((char *)items);
}

void destroyCallback(Widget widget, XtPointer clientData, XtPointer callData) {
	XtDestroyWidget(XtParent(widget));
}

void helpCallback(Widget widget, XtPointer clientData, XtPointer callData) {
	View *view = (View *)clientData;
	
	if (widget == view->help) {
		XtManageChild(view->defaultHelpDialog);
		XtPopup(XtParent(view->defaultHelpDialog), XtGrabNone);
	} else if(widget == view->quitDialog) {
		XtManageChild(view->quitHelpDialog);
		XtPopup(XtParent(view->quitHelpDialog), XtGrabNone);
	} else if(widget == view->commandNotFoundDialog) {
		XtManageChild(view->runHelpDialog);
		XtPopup(XtParent(view->runHelpDialog), XtGrabNone);
	} else {
		fprintf(stderr, "Not implemented, sorry.\n");
	}
}

void exitProgram() {
	/* TODO: clean up */
	if (groups != NULL) {
		deleteProgramGroup(groups);
	}
	
	exit(EXIT_SUCCESS);
}

void quitCallback(Widget widget, XtPointer clientData, XtPointer callData) {
	View *view = (View *)clientData;
	
	XtManageChild(view->quitDialog);
	XtPopup(XtParent(view->quitDialog), XtGrabNone);
}

void groupListCallback(Widget w, XtPointer clientData,
		XtPointer callData) {
	View *view = (View *)clientData;
	
	XmListCallbackStruct *callbackStruct =
			(XmListCallbackStruct *)callData;
	
	int selection = callbackStruct->item_position - 1;
	ProgramGroup *current = groups;
	int i = 0;
	while (i != selection) {
		i += 1;
		current = current->next;
	}
	
	currentGroup = current->programs;
	Program *currentProgram = currentGroup;
	i = 0;
	while (currentProgram != NULL) {
		currentProgram = currentProgram->next;
		i += 1;
	}
	
	/* i now has the size of the program list */
	
	char **programList = malloc(i * sizeof(char *));
	
	i = 0;
	currentProgram = currentGroup;
	while (currentProgram != NULL) {
		programList[i] = currentProgram->visualName;
		currentProgram = currentProgram->next;
		i += 1;
	}
	
	setListContents(view->programList, i, programList, 10);
	
	free(programList);
}

void programListSingleClickCallback(Widget w, XtPointer clientData,
		XtPointer callData) {
	View *view = (View *)clientData;
	
	XmListCallbackStruct *callbackStruct =
			(XmListCallbackStruct *)callData;
	
	int selection = callbackStruct->item_position - 1;
	Program *current = currentGroup;
	int i = 0;
	while (i != selection) {
		i += 1;
		current = current->next;
	}
	
	XmTextSetString(view->commandBox, current->command);
	XmTextSetInsertionPosition(view->commandBox, strlen(current->command));
}

void programListDoubleClickCallback(Widget w, XtPointer clientData,
		XtPointer callData) {
	View *view = (View *)clientData;
	
	XmListCallbackStruct *callbackStruct =
			(XmListCallbackStruct *)callData;
	
	int selection = callbackStruct->item_position - 1;
	Program *current = currentGroup;
	int i = 0;
	while (i != selection) {
		i += 1;
		current = current->next;
	}
	
	execProgram(current, "");
}

void runCallback(Widget w, XtPointer clientData,
		XtPointer callData) {
	View *view = (View *)clientData;
	
	char *command = XmTextGetString(view->commandBox);
	
	Program program;
	program.command = command;
	program.next = NULL;
	
	if (execProgram(&program, "")) {
		XtManageChild(view->commandNotFoundDialog);
		XtPopup(XtParent(view->commandNotFoundDialog), XtGrabNone);
	}
	
	XtFree(command);
}

void cancelCallback(Widget w, XtPointer clientData,
		XtPointer callData) {
	View *view = (View *)clientData;
	
	XmTextSetString(view->commandBox, "");
}

int main(int argc, char *argv[]) {
	fclose(stdout);
	fclose(stdin);
	
	/* mop off child processes */
	
	signal(SIGCHLD, SIG_IGN);
	
	View *view = NULL;
	
	/* read config file specified on command line
	 * default is config.vx
	 */
	
	groups = readConfigFile(argc > 1 ? argv[1] : "config.vx");
	if (groups == NULL) {
		fprintf(stderr, "Error opening file\n");
		exit(EXIT_FAILURE);
	}
	
	/* generate array of program groups */
	
	int n = 0;
	
	ProgramGroup *current = groups;
	while(current != NULL) {
		n += 1;
		current = current->next;
	}
	
	char **groupList = malloc(n * sizeof(char *));
	if (groupList == NULL) {
		fprintf(stderr, "Memory error\n");
		exitProgram();
	}
	
	current = groups;
	for (int i = 0; i < n; i++) {
		groupList[i] = current->visualName;
		
		/* get rid of underscores, replace with spaces */
		
		for (int j = 0; j <= strlen(groupList[i]); j++) {
			if (groupList[i][j] == '_')
				groupList[i][j] = ' ';
		}
		
		Program *program = current->programs;
		while (program != NULL) {
			for (int j = 0; j <= strlen(program->visualName); j++) {
				if (program->visualName[j] == '_')
					program->visualName[j] = ' ';
			}
			
			program = program->next;
		}
		
		current = current->next;
	}
	
	view = createView(argc, argv);
	
	XtAddCallback(view->quit, XmNactivateCallback, quitCallback,
			(XtPointer)view);
	XtAddCallback(view->help, XmNactivateCallback, helpCallback,
			(XtPointer)view);
	
	XtAddCallback(view->groupList, XmNbrowseSelectionCallback,
			groupListCallback, (XtPointer)view);
	XtAddCallback(view->programList, XmNbrowseSelectionCallback,
			programListSingleClickCallback, (XtPointer)view);
	XtAddCallback(view->programList, XmNdefaultActionCallback,
			programListDoubleClickCallback, (XtPointer)view);
	
	XtAddCallback(view->runButton, XmNactivateCallback,
			runCallback, (XtPointer)view);
	XtAddCallback(view->commandBox, XmNactivateCallback,
			runCallback, (XtPointer)view);
	
	XtAddCallback(view->commandNotFoundDialog,
			XmNcancelCallback, cancelCallback,
			view);
	XtAddCallback(view->commandNotFoundDialog,
			XmNhelpCallback, helpCallback,
			(XtPointer)view);
	
	XtAddCallback(view->quitDialog, XmNokCallback, exitProgram,
			NULL);
	XtAddCallback(view->quitDialog, XmNhelpCallback, helpCallback,
			(XtPointer)view);
	
	setListContents(view->groupList, n, groupList, 10);

	free(groupList);
	
	XtRealizeWidget(view->topLevel);
	XtAppMainLoop(view->context);
	
	return 0;
}
