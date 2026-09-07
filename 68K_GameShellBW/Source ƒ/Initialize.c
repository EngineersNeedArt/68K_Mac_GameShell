//============================================================================
//	Initialize.c
//============================================================================


#include "Events.h"
#include "Initialize.h"
#include "MainWindow.h"
#include "Menu.h"
#include "Play.h"
#include "Render.h"
#include "SoundUtils.h"


void toolBoxInit (void);
void varInit (void);


//========================================================  Functions
//--------------------------------------------------------  toolBoxInit

void toolBoxInit (void) {
	short		count;
	
	InitGraf(&thePort);
	InitFonts();
	FlushEvents(everyEvent, 0);
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(NULL);
	InitCursor();
	
	MaxApplZone();
	
	MoreMasters();
	MoreMasters();
	MoreMasters();
	MoreMasters();
}

//--------------------------------------------------------  varInit

void varInit (void) {
	quitting = FALSE;
	playing = FALSE;
}

//--------------------------------------------------------  initializeAll

void initializeAll (void) {
	toolBoxInit();
//	environInit();
//	VarInit();
	gameInit();
	openMainWindow();
	renderInit();
	copyScratchToMainWindow();
	menuBarInit();
	soundInit();
	InitCursor();
}