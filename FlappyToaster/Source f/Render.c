// ===========================================================================
//	Render.c
// ===========================================================================


#include "Render.h"
#include "MainWindow.h"
#include "PixelUtils.h"
#include "Utilities.h"


#define kBackgroundPICTID	128
#define kSpritePICTID		129
#define kMaskPICTID			130


Rect		backgroundBounds;
Rect		scratchBounds;
Rect		mainDisplayDestBounds;
GrafPtr		backgroundOffscreen = NULL;
GrafPtr		scratchOffscreen = NULL;
GrafPtr		spriteOffscreen = NULL;
GrafPtr		maskOffscreen = NULL;
RgnHandle	mainDisplayRenderRgn = NULL;


// ========================================================  Public Functions
// --------------------------------------------------------  copyBackgroundToScratch

void copyBackgroundToScratch (void) {
	CopyBits (&((GrafPtr)backgroundOffscreen)->portBits, &((GrafPtr) scratchOffscreen)->portBits, 
			&scratchBounds, &scratchBounds, srcCopy, NULL);
}

// --------------------------------------------------------  copyScratchToMainWindow

void copyScratchToMainWindow (void) {
	CopyBits (&((GrafPtr) scratchOffscreen)->portBits,  &(((GrafPtr) mainWindow)->portBits), 
			&scratchBounds, &mainDisplayDestBounds, srcCopy, 
			(RgnHandle) ((GrafPtr) mainWindow->visRgn));
}

// --------------------------------------------------------  renderInit

Boolean renderInit (void) {
	Boolean	success = FALSE;
	short	screenWidth = screenBits.bounds.right - screenBits.bounds.left;
	short	screenHeight = screenBits.bounds.bottom - screenBits.bounds.top - MBarHeight;
	Rect	spriteBounds;
	
	SetRect (&backgroundBounds, 0, 0, kGameWidth, kGameHeight);
	SetRect (&scratchBounds, 0, 0, kGameWidth, kGameHeight);
	SetRect (&mainDisplayDestBounds, 0, 0, kGameWidth, kGameHeight);
	
	// Compute a destination rectangle that will center the game on the main display.
	SetRect (&mainDisplayDestBounds, 0, 0, kGameWidth, kGameHeight);
	OffsetRect (&mainDisplayDestBounds, (screenWidth - kGameWidth) / 2, (screenHeight - kGameHeight) / 2);
	
	// Create a bitmap to hold the unadulterated background. Load background.
	SetRect (&backgroundBounds, 0, 0, kGameWidth, kGameHeight);
	backgroundOffscreen = createBWOffScreenBitMap (&backgroundBounds);
	if (!drawPictureWithID (kBackgroundPICTID)) {
		goto bail;
	}
	
	// Create a full (game) size scratch bitmap for our compositing.
	SetRect (&scratchBounds, 0, 0, kGameWidth, kGameHeight);
	scratchOffscreen = createBWOffScreenBitMap (&scratchBounds);
	copyBackgroundToScratch();
	
	// Create sprite and mask offscreen bitmaps. Assumed to be the same size.
	getPICTBounds(kSpritePICTID, &spriteBounds);
	spriteOffscreen = createBWOffScreenBitMap (&spriteBounds);
	if (!drawPictureWithID (kSpritePICTID)) {
		goto bail;
	}
	
	maskOffscreen = createBWOffScreenBitMap( &spriteBounds);
	if (!drawPictureWithID (kMaskPICTID)) {
		goto bail;
	}
	
	// Create a clipping region for th emain window.
	mainDisplayRenderRgn = NewRgn ();
	RectRgn (mainDisplayRenderRgn, &mainDisplayDestBounds);
	
	success = TRUE;
	
bail:
	
	return success;
}
