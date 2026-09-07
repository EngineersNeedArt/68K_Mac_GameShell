//============================================================================
//	Render.c
//============================================================================


#include "Render.h"
#include "MainWindow.h"
#include "PixelUtils.h"
#include "Utilities.h"


#define kBackgroundPICTID	128
#define kSpritePICTID		129
#define kMaskPICTID			130


Rect	backgroundBounds = {0, 0, kGameWidth, kGameHeight};
Rect	scratchBounds = {0, 0, kGameWidth, kGameHeight};
Rect	mainDisplayDestBounds = {0, 0, kGameWidth, kGameHeight};
GrafPtr	backgroundOffscreen = NULL;
GrafPtr	scratchOffscreen = NULL;
GrafPtr	spriteOffscreen = NULL;
GrafPtr	maskOffscreen = NULL;


//========================================================  Functions
//--------------------------------------------------------  copyBackgroundToScratch

void copyBackgroundToScratch (void) {
	CopyBits(&((GrafPtr)backgroundOffscreen)->portBits, &((GrafPtr)scratchOffscreen)->portBits, 
			&scratchBounds, &scratchBounds, srcCopy, NULL);
}

//--------------------------------------------------------  copyScratchToMainWindow

void copyScratchToMainWindow (void) {
	CopyBits(&((GrafPtr)scratchOffscreen)->portBits,  &(((GrafPtr)mainWindow)->portBits), 
			&scratchBounds, &mainDisplayDestBounds, srcCopy, 
			(RgnHandle)((GrafPtr)mainWindow->visRgn));
}

//--------------------------------------------------------  renderInit

void renderInit (void) {
	int screenWidth = screenBits.bounds.right - screenBits.bounds.left;
	int screenHeight = screenBits.bounds.bottom - screenBits.bounds.top - MBarHeight;
	Rect spriteBounds;
	
	// Compute a destination rectangle that will center the game on the main display.
	SetRect (&mainDisplayDestBounds, 0, 0, kGameWidth, kGameHeight);
	OffsetRect(&mainDisplayDestBounds, (screenWidth - kGameWidth) / 2, (screenHeight - kGameHeight) / 2);
	
	// Create a bitmap to hold the unadulterated background. Load background.
	SetRect (&backgroundBounds, 0, 0, kGameWidth, kGameHeight);
	createOffScreenBitMap(&backgroundBounds, &backgroundOffscreen);
	drawPictureWithID (kBackgroundPICTID);
	
	// Create a full (game) size scratch bitmap for our compositing.
	SetRect (&scratchBounds, 0, 0, kGameWidth, kGameHeight);
	createOffScreenBitMap(&scratchBounds, &scratchOffscreen);
	copyBackgroundToScratch();
	
	// Create sprite and mask offscreen bitmaps. Assumed to be the same size.
	getPICTBounds(kSpritePICTID, &spriteBounds);
	createOffScreenBitMap(&spriteBounds, &spriteOffscreen);
	drawPictureWithID (kSpritePICTID);
	
	createOffScreenBitMap(&spriteBounds, &maskOffscreen);
	drawPictureWithID (kMaskPICTID);
}
