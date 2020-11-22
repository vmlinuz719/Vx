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
	
	view->quitDialog = XmCreateQuestionDialog
			(view->quit, "question", NULL, 0);
	XmString quitText = XmStringCreateLocalized
			("Are you sure you want to quit?");
	XtVaSetValues(view->quitDialog,
			XmNmessageString, quitText,
			NULL);
	XmStringFree(quitText);
	
	view->commandNotFoundDialog = XmCreateErrorDialog
			(view->runButton, "error", NULL, 0);
	XmString errorText = XmStringCreateLocalized
			("Could not run command");
	XtVaSetValues(view->commandNotFoundDialog,
			XmNmessageString, errorText,
			NULL);
	XmStringFree(errorText);
	
	view->defaultHelpDialog = XmCreateTemplateDialog
			(view->help, "defaultHelp", NULL, 0);
	XmString defaultHelpText = XmStringCreateLocalized
			("Select a program group on the left.\n"
			"Then, choose a program on the right.\n"
			"Double-click the program's name to start it.\n\n"
			"You may enter or edit any command using the\n"
			"command entry box on the bottom.\n\n"
			"Programs that do not provide a graphical\n"
			"interface will fail silently unless run in a\n"
			"terminal window (e.g. Xterm).");
	XtVaSetValues(view->defaultHelpDialog,
			XmNmessageString, defaultHelpText,
			NULL);
	XmStringFree(defaultHelpText);
	XtVaCreateManagedWidget("Dismiss",
			xmPushButtonWidgetClass, view->defaultHelpDialog,
			NULL);
	
	view->quitHelpDialog = XmCreateTemplateDialog
			(view->quit, "quitHelp", NULL, 0);
	XmString quitHelpText = XmStringCreateLocalized
			("Select \"OK\" to quit.\n"
			"Select \"Cancel\" to dismiss the dialog.");
	XtVaSetValues(view->quitHelpDialog,
			XmNmessageString, quitHelpText,
			NULL);
	XmStringFree(quitHelpText);
	XtVaCreateManagedWidget("Dismiss",
			xmPushButtonWidgetClass, view->quitHelpDialog,
			NULL);
	
	view->runHelpDialog = XmCreateTemplateDialog
			(view->runButton, "runHelp", NULL, 0);
	XmString runHelpText = XmStringCreateLocalized
			("The command could not be executed.\n"
			"Select \"OK\" to revise the command.\n"
			"Select \"Cancel\" to clear the command.");
	XtVaSetValues(view->runHelpDialog,
			XmNmessageString, runHelpText,
			NULL);
	XmStringFree(runHelpText);
	XtVaCreateManagedWidget("Dismiss",
			xmPushButtonWidgetClass, view->runHelpDialog,
			NULL);
	
	XtVaSetValues(resizer,
			XmNwidth, 480,
			XmNheight, 256,
			NULL);
	
	XtSetArg(arg[0], XmNmenuHelpWidget, view->help);
	XtSetValues(view->menuBar, arg, 1);
	
	return view;
}
