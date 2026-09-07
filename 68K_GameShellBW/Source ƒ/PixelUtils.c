//============================================================================
//	PixelUtils.c
//============================================================================


#include "PixelUtils.h"
#include "Utilities.h"


//========================================================  Functions
//--------------------------------------------------------  createOffScreenBitMap

void createOffScreenBitMap (Rect *theRect, GrafPtr *offScreen) {
	GrafPtr		theBWPort;
	BitMap		theBitMap;	
	short		theRowBytes;
	
	theBWPort = (GrafPtr)(NewPtr(sizeof(GrafPort)));
	OpenPort(theBWPort);
	
	theRowBytes = ((theRect->right - theRect->left + 15) / 16) * 2;
	theBitMap.rowBytes = theRowBytes;
	theBitMap.baseAddr = NewPtr((long)theBitMap.rowBytes * 
		(theRect->bottom - theRect->top));
	
	if (!theBitMap.baseAddr)
		fatalError(kErrNoMemory);
	
	theBitMap.bounds = *theRect;
	if (MemError() != noErr)
		fatalError(kErrNoMemory);
	
	SetPort(theBWPort);
	
	SetPortBits(&theBitMap);
	ClipRect(theRect);
	RectRgn(theBWPort->visRgn, theRect);
	EraseRect(theRect);
	
	*offScreen = theBWPort;
}

//--------------------------------------------------------  drawPictureWithID

void drawPictureWithID (short resID) {
	Rect		bounds;
	PicHandle	thePicture;
	Byte		wasState;
	
	thePicture = GetPicture(resID);
	if (!thePicture) {
		fatalError(kErrFailedResourceLoad);
	}
	
	wasState = HGetState((Handle)thePicture);
	HLock((Handle)thePicture);
	bounds = (*thePicture)->picFrame;
	HSetState((Handle)thePicture, wasState);
	OffsetRect(&bounds, -bounds.left, -bounds.top);
	DrawPicture(thePicture, &bounds);
	
	ReleaseResource((Handle)thePicture);
}

