//============================================================================
//	MainWindow.c
//============================================================================


#include "MainWindow.h"
#include "Render.h"


#define kMainWindowID			128


Rect		mainWindowRect;
Rect		viewRect;
WindowPtr	mainWindow = NULL;


//========================================================  Functions
//--------------------------------------------------------  openMainWindow

void openMainWindow (void) {
	RGBColor	isBlack, isWhite;
	
	mainWindowRect = screenBits.bounds;
	mainWindow = NULL;
	
//	if (isColor) {
//		mainWindow = GetNewCWindow(kMainWindowID, NULL, (WindowPtr)-1L);
//	} else {
		mainWindow = GetNewWindow(kMainWindowID, NULL, (WindowPtr)-1L);
//	}
	
	SizeWindow((GrafPtr)mainWindow, mainWindowRect.right - mainWindowRect.left, 
			mainWindowRect.bottom - mainWindowRect.top, TRUE);
	
	ShowWindow((GrafPtr)mainWindow);
	SetPort((GrafPtr)mainWindow);
	
//	if (isColor) {
//		isBlack.red = 0x0000;
//		isBlack.green = 0x0000;
//		isBlack.blue = 0x0000;
//		RGBForeColor(&isBlack);
		
//		isWhite.red = 0xFFFF;
//		isWhite.green = 0xFFFF;
//		isWhite.blue = 0xFFFF;
//		RGBBackColor(&isWhite);
//	} else {
		FillRect(&mainWindowRect, black);
//	}
}

//--------------------------------------------------------  updateMainWindow

void updateMainWindow (void) {
	SetPort((GrafPtr)mainWindow);
	copyScratchToMainWindow();
}

//--------------------------------------------------------  closeMainWindow

void closeMainWindow (void) {
	if (mainWindow)
		DisposeWindow(mainWindow);
	mainWindow = NULL;
}

