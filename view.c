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
#include <Xm/PanedW.h>
#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/List.h>
#include <Xm/Text.h>

#include "vx.h"

/* The view class */

typedef struct {
	Widget topLevel, mainWindow, menuBar, quit, help;
	Widget groupList, programList;
	Widget commandBox;
	Widget favButton, unfavButton;
	XtAppContext context;
} View;

void quitCallback() {
	printf("Exiting.\n");
	exit(0);
}

void helpCallback() {
	printf("Not implemented, sorry!\n");
}

View *createView(int argc, char *argv[]) {
	View *view = malloc(sizeof(View));
	if (view == NULL) {
		return NULL;
	}
	
	Arg arg[1];
	
	view->topLevel = XtVaAppInitialize(&(view->context), "Vx", NULL, 0,
			&argc, argv, NULL, NULL);
	
	view->mainWindow = XtVaCreateManagedWidget("mainWindow",
			xmMainWindowWidgetClass, view->topLevel,
			NULL);
	
	view->menuBar = XmCreateMenuBar(view->mainWindow, "mainMenu", NULL, 0);
	XtManageChild(view->menuBar);
	
	view->quit = XtVaCreateManagedWidget("Quit",
			xmCascadeButtonWidgetClass, view->menuBar,
			XmNmnemonic, 'Q',
			NULL);
	
	view->help = XtVaCreateManagedWidget("Help",
			xmCascadeButtonWidgetClass, view->menuBar,
			XmNmnemonic, 'H',
			NULL);
	
	Widget resizer = XtVaCreateManagedWidget("resizer",
			xmFormWidgetClass, view->mainWindow,
			XmNmarginWidth, 3,
			XmNmarginHeight, 3,
			NULL);
	
	Widget pane = XtVaCreateManagedWidget("pane",
			xmFormWidgetClass, resizer,
			XmNorientation, XmHORIZONTAL,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL);
	
	Widget left = XtVaCreateManagedWidget("left",
			xmFormWidgetClass, pane,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 50,
			XmNmarginWidth, 3,
			XmNmarginHeight, 3,
			NULL);
	
	Widget right = XtVaCreateManagedWidget("right",
			xmFormWidgetClass, pane,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftPosition, 50,
			XmNmarginWidth, 3,
			XmNmarginHeight, 3,
			NULL);
	
	Widget groupLabel = XtVaCreateManagedWidget("Groups",
			xmLabelWidgetClass, left,
			NULL);
	
	view->groupList = XtVaCreateManagedWidget("groupList",
			xmListWidgetClass, left,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, groupLabel,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL);
	
	Widget programLabel = XtVaCreateManagedWidget("Programs",
			xmLabelWidgetClass, right,
			NULL);
	view->favButton = XtVaCreateManagedWidget("Fav",
			xmPushButtonWidgetClass, right,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 50,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL);
	view->unfavButton = XtVaCreateManagedWidget("Unfav",
			xmPushButtonWidgetClass, right,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftPosition, 50,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL);
	view->programList = XtVaCreateManagedWidget("programList",
			xmListWidgetClass, right,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, programLabel,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, view->favButton,
			NULL);
	
	XtSetArg(arg[0], XmNmenuHelpWidget, view->help);
	XtSetValues(view->menuBar, arg, 1);
	
	/* TODO: move to controller */
	XtAddCallback(view->quit, XmNactivateCallback, quitCallback, NULL);
	XtAddCallback(view->help, XmNactivateCallback, helpCallback, NULL);
	
	return view;
}

int main(int argc, char *argv[]) {
	View *view = createView(argc, argv);
	
	XtRealizeWidget(view->topLevel);
	XtAppMainLoop(view->context);
	
	return 0;
}
