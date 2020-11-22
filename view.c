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
#include <Xm/MessageB.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/List.h>
#include <Xm/Text.h>

#include "vx.h"

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
			XmNmarginWidth, 5,
			XmNmarginHeight, 5,
			NULL);
	
	Widget pane = XtVaCreateManagedWidget("pane",
			xmFormWidgetClass, resizer,
			XmNorientation, XmHORIZONTAL,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			/* XmNbottomAttachment, XmATTACH_FORM, */
			NULL);
	
	Widget left = XtVaCreateManagedWidget("left",
			xmFormWidgetClass, pane,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 50,
			XmNmarginWidth, 5,
			XmNmarginHeight, 5,
			NULL);
	
	Widget right = XtVaCreateManagedWidget("right",
			xmFormWidgetClass, pane,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftPosition, 50,
			XmNmarginWidth, 5,
			XmNmarginHeight, 5,
			NULL);
	
	Widget groupLabel = XtVaCreateManagedWidget("Groups",
			xmLabelWidgetClass, left,
			NULL);
	
	view->groupList = XmCreateScrolledList(left, "groupList", NULL, 0);
	XtVaSetValues(XtParent(view->groupList),
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, groupLabel,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL);
	XtVaSetValues(view->groupList, XmNscrollBarDisplayPolicy, XmSTATIC,
			NULL);
	XtManageChild(view->groupList);
	
	Widget programLabel = XtVaCreateManagedWidget("Programs",
			xmLabelWidgetClass, right,
			NULL);
	/*
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
	*/
	view->programList = XmCreateScrolledList(right, "programList", NULL, 0);
	XtVaSetValues(XtParent(view->programList),
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, programLabel,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			/*
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, view->favButton,
			*/
			XmNbottomAttachment, XmATTACH_FORM,
			NULL);
	XtVaSetValues(view->programList, XmNscrollBarDisplayPolicy, XmSTATIC,
			NULL);
	XtManageChild(view->programList);
	
	Widget commandArea = XtVaCreateManagedWidget("commandArea",
			xmFormWidgetClass, resizer,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNmarginWidth, 5,
			NULL);
	XtVaSetValues(pane, XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, commandArea,
			NULL);
	Widget commandLabel = XtVaCreateManagedWidget("Command:",
			xmLabelWidgetClass, commandArea,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL);
	view->runButton = XtVaCreateManagedWidget("Run",
			xmPushButtonWidgetClass, commandArea,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);
	view->commandBox = XtVaCreateManagedWidget("commandBox",
			xmTextWidgetClass, commandArea,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrightWidget, view->runButton,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, commandLabel,
			NULL);
	
	Arg quitArg[5];
	int quitn = 0;
	XmString quitText = XmStringCreateLocalized
			("Are you sure you want to quit?");
	XtSetArg(quitArg[quitn], XmNmessageString, quitText); quitn++;
	view->quitDialog = XmCreateQuestionDialog
			(view->quit, "question", quitArg, quitn);
	XmStringFree(quitText);
	
	Arg errorArg[5];
	int errorn = 0;
	XmString errorText = XmStringCreateLocalized
			("Could not run command");
	XtSetArg(errorArg[errorn], XmNmessageString, errorText); errorn++;
	view->commandNotFoundDialog = XmCreateErrorDialog
			(view->quit, "error", errorArg, errorn);
	XmStringFree(errorText);
	
	XtSetArg(arg[0], XmNmenuHelpWidget, view->help);
	XtSetValues(view->menuBar, arg, 1);
	
	return view;
}
