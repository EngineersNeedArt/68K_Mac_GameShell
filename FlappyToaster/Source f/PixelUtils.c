// ===========================================================================
//	PixelUtils.c
// ===========================================================================


#include "PixelUtils.h"
#include "Utilities.h"


// ======================================================== Public Functions
// -------------------------------------------------------- createBWOffScreenBitMap

GrafPtr createBWOffScreenBitMap (Rect *theRect) {
	GrafPtr		bwPort = NULL;
	BitMap		bitMap;	
	short		rowBytes;
	
	// Calculate row bytes (round up to the nearest multiple of 16)
	rowBytes = ((theRect->right - theRect->left + 15) / 16) * 2;
	bitMap.rowBytes = rowBytes;
	
	// Allocate storage for pixels: rowBytes * height.
	bitMap.baseAddr = NewPtr ((long) bitMap.rowBytes * 
		(theRect->bottom - theRect->top));
	if (bitMap.baseAddr == NULL) {
		// Not enough memory to allocate the pointer.
		fatalError (kErrNoMemory);
		goto bail;
	}
	
	// This call might be unnecessary.
	if (MemError () != noErr) {
		fatalError (kErrNoMemory);
		goto bail;
	}
	
	// Assign bounds.
	bitMap.bounds = *theRect;
	
	// Create GrafPtr.
	bwPort = (GrafPtr) (NewPtr (sizeof (GrafPort)));
	if (bwPort == NULL) {
		// Not enough memory to allocate pointer.
		fatalError (kErrNoMemory);
		goto bail;
	}
	
	OpenPort (bwPort);
	
	// Assign bitmap, clip to bounds, assign vsible region, clear.
	SetPort (bwPort);
	SetPortBits (&bitMap);
	ClipRect (theRect);
	RectRgn (bwPort->visRgn, theRect);
	EraseRect (theRect);
	
bail:
	
	return bwPort;
}

// --------------------------------------------------------  drawPictureWithID

Boolean drawPictureWithID (short resID) {
	Rect		bounds;
	PicHandle	thePicture;
	Byte		wasState;
	
	thePicture = GetPicture (resID);
	if (!thePicture) {
		fatalError (kErrFailedResourceLoad);
		return FALSE;
	}
	
	wasState = HGetState ((Handle) thePicture);
	HLock ((Handle) thePicture);
	bounds = (*thePicture)->picFrame;
	HSetState ((Handle) thePicture, wasState);
	OffsetRect (&bounds, -bounds.left, -bounds.top);
	DrawPicture (thePicture, &bounds);
	
	ReleaseResource ((Handle) thePicture);
	
	return TRUE;
}
