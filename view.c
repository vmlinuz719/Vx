/* Visual Executive - Data Types
 * (C) 2020 vmlinuz719. All rights reserved
 * See COPYING for copyright notice. You may modify and redistribute this file
 * however you like, so long as it is redistributed along with the notice in
 * COPYING.
 
 * This file defines the Motif user interface.
 */

#include <stdio.h>
#include <stdlib.h>

#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/CascadeB.h>

typedef struct {
	Widget topLevel, mainWindow, menuBar, quit, help;
	XtAppContext context;
} View;

void quitCallback() {
	printf("Exiting.\n");
	exit(0);
}

void helpCallback() {
	printf("Not implemented, sorry!\n");
}

#ifdef BUILD
int main(int argc, char *argv[]) {
	View *view = malloc(sizeof(view));
	if (view == NULL) {
		return EXIT_FAILURE;
	}
	
	Arg arg[1];
	
	view->topLevel = XtVaAppInitialize(&(view->context), "Vx", NULL, 0,
			&argc, argv, NULL, NULL);
	
	view->mainWindow = XtVaCreateManagedWidget("mainWindow",
			xmMainWindowWidgetClass, view->topLevel,
			NULL);
	
	view->menuBar = XmCreateMenuBar(view->mainWindow, "mainMenu", NULL, 0);
	XtManageChild(view->menuBar);
	
	view->quit = XtCreateManagedWidget("Quit",
			xmCascadeButtonWidgetClass, view->menuBar,
			XmNmnemonic, 'Q',
			NULL);
	XtAddCallback(view->quit, XmNactivateCallback, quitCallback, NULL);
	
	view->help = XtCreateManagedWidget("Help",
			xmCascadeButtonWidgetClass, view->menuBar,
			XmNmnemonic, 'H',
			NULL);
	XtAddCallback(view->help, XmNactivateCallback, helpCallback, NULL);
	XtSetArg(arg[0], XmNmenuHelpWidget, help);
	XtSetValues(view->menuBar, arg, 1);
	
	XtRealizeWidget(view->topLevel);
	XtAppMainLoop(view->context);
	
	return 0;
}
#endif
