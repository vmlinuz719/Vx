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

void quitCallback() {
	printf("Exiting.\n");
	exit(0);
}

void helpCallback() {
	printf("Not implemented, sorry!\n");
}

Widget topLevel, mainWindow, menuBar, quit, help;

#ifdef BUILD
int main(int argc, char *argv[]) {
	return 0;
}
#endif
