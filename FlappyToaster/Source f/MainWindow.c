// ===========================================================================
//	MainWindow.c
// ===========================================================================


#include "MainWindow.h"
#include "Render.h"


#define kMainWindowID			128


WindowPtr	mainWindow = NULL;


// ========================================================  Public Functions
// --------------------------------------------------------  openMainWindow

void openMainWindow (void) {
	Rect	mainWindowRect = screenBits.bounds;
	
	// Create the window using our resource fork.
	mainWindow = GetNewWindow (kMainWindowID, NULL, (WindowPtr) -1L);
	
	// Size the window to fill the entire screen.
	SizeWindow ((GrafPtr) mainWindow, mainWindowRect.right - mainWindowRect.left, 
			mainWindowRect.bottom - mainWindowRect.top, TRUE);
	
	// Show the window, initially fill it black.
	ShowWindow ((GrafPtr) mainWindow);
	SetPort ((GrafPtr) mainWindow);
	FillRect (&mainWindowRect, black);
}

// --------------------------------------------------------  updateMainWindow

void updateMainWindow (void) {
	// When called to update our window, we just set it to be the current port 
	// and render from our 'scratch' offscreen-bitmap to the screen.
	SetPort ((GrafPtr) mainWindow);
	copyScratchToMainWindow ();
}

// --------------------------------------------------------  closeMainWindow

void closeMainWindow (void) {
	if (mainWindow) {
		DisposeWindow (mainWindow);
	}
	mainWindow = NULL;
}
