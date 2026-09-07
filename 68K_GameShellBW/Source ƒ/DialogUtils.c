//============================================================================
//	DialogUtils.c
//============================================================================


#include "DialogUtils.h"


#define kActive						0
#define kInactive					255


//========================================================  Functions
//--------------------------------------------------------  centerDialog

void centerDialog (short dialogID) {
	DialogTHndl	dialogHandle;
	
	dialogHandle = (DialogTHndl)GetResource('DLOG', dialogID);
	if (dialogHandle) {
		Byte	wasState;
		Rect	screenBounds;
		Rect	dialogBounds;
		short	hPos, vPos;
		
		// Lock dialog handle.
		wasState = HGetState((Handle)dialogHandle);
		HLock((Handle)dialogHandle);
		
		// Get the bounds of the display, subtract menu bar height.
		screenBounds = screenBits.bounds;
		screenBounds.top += MBarHeight;
		
		// Get the dialog bounds and 'zero' it.
		dialogBounds = (*dialogHandle)->boundsRect;
		OffsetRect(&dialogBounds, -dialogBounds.left, -dialogBounds.top);
		
		// Calculate offset to center horizontally, 1/3 of the way down vertically. 
		hPos = ((screenBounds.right - screenBounds.left) - dialogBounds.right) / 2;
		vPos = ((screenBounds.bottom - screenBounds.top) - dialogBounds.bottom) / 3;
		
		// Offset bounds and assign.
		OffsetRect(&dialogBounds, hPos, vPos + MBarHeight);
		(*dialogHandle)->boundsRect = dialogBounds;
		
		// Restore handle state.
		HSetState((Handle)dialogHandle, wasState);
	}
}

//--------------------------------------------------------  centerAlert

void centerAlert (short alertID) {
	AlertTHndl		alertHandle;
	Rect			scrnRect, alertRect;
	short			horiOff, vertOff;
	Byte			wasState;
	
	scrnRect = screenBits.bounds;
	scrnRect.top += MBarHeight;
	
	alertHandle = (AlertTHndl)GetResource('ALRT', alertID);
	if (alertHandle) {
		wasState = HGetState((Handle)alertHandle);
		HLock((Handle)alertHandle);
		
		alertRect = (**alertHandle).boundsRect;
		OffsetRect(&alertRect, -alertRect.left, -alertRect.top);
		
		horiOff = ((scrnRect.right - scrnRect.left) - alertRect.right) / 2;	
		vertOff = ((scrnRect.bottom - scrnRect.top) - alertRect.bottom) / 3;
		
		OffsetRect(&alertRect, horiOff, vertOff + MBarHeight);
		
		(**alertHandle).boundsRect = alertRect;
		HSetState((Handle)alertHandle, wasState);
	}
}

//--------------------------------------------------------  drawDefaultButton

void drawDefaultButton (DialogPtr theDialog, short itemNumber) {
	Rect			itemRect;
	Handle			itemHandle;
	short			itemType;
	
	GetDItem(theDialog, itemNumber, &itemType, &itemHandle, &itemRect);
	InsetRect(&itemRect, -4, -4);
	PenSize(3, 3);
	FrameRoundRect(&itemRect, 16, 16);
	PenNormal();
}

