// ===========================================================================
//	Initialize.c
// ===========================================================================


#include "Events.h"
#include "Initialize.h"
#include "MainWindow.h"
#include "Menu.h"
#include "Play.h"
#include "Render.h"
#include "SoundUtils.h"


void _toolBoxInit (void);


// ========================================================  Private Functions
// --------------------------------------------------------  _toolBoxInit

void _toolBoxInit (void) {
	// Boiler-plate initialization of the various Toolbkx managers.
	InitGraf (&thePort);
	InitFonts ();
	FlushEvents (everyEvent, 0);
	InitWindows ();
	InitMenus ();
	TEInit ();
	InitDialogs (NULL);
	InitCursor ();
	
	// Making sure we have adequate memory for our app, enough handle/pointers.
	MaxApplZone ();
	MoreMasters ();
	MoreMasters ();
	MoreMasters ();
	MoreMasters ();
}

// ========================================================  Public Functions
// --------------------------------------------------------  initializeAll

Boolean initializeAll (void) {
	_toolBoxInit ();
	gameInit ();
	openMainWindow ();
	if (!renderInit ()) {
		return FALSE;
	}
	copyScratchToMainWindow ();
	menuBarInit ();
	soundInit ();
	InitCursor ();
	
	return TRUE;
}
