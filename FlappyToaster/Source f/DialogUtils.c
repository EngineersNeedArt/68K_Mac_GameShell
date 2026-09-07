// ===========================================================================
//	DialogUtils.c
// ===========================================================================


#include "DialogUtils.h"


// ========================================================  Public Functions
// --------------------------------------------------------  centerDialog

void centerDialog (short dialogID) {
	DialogTHndl	dialogHandle;
	
	// Get a handle to the dialog.
	dialogHandle = (DialogTHndl) GetResource ('DLOG', dialogID);
	if (dialogHandle) {
		Byte	wasState;
		Rect	screenBounds;
		Rect	dialogBounds;
		short	hPos, vPos;
		
		// Save handle state, lock it.
		wasState = HGetState ((Handle) dialogHandle);
		HLock ((Handle) dialogHandle);
		
		// Get the bounds of the display, subtract menu bar height.
		screenBounds = screenBits.bounds;
		screenBounds.top += MBarHeight;
		
		// Get the dialog bounds and set the origin to zero.
		dialogBounds = (*dialogHandle)->boundsRect;
		OffsetRect (&dialogBounds, -dialogBounds.left, -dialogBounds.top);
		
		// Calculate offset to center horizontally, 1/3 of the way down vertically. 
		hPos = ((screenBounds.right - screenBounds.left) - dialogBounds.right) / 2;
		vPos = ((screenBounds.bottom - screenBounds.top) - dialogBounds.bottom) / 3;
		
		// Offset bounds and assign.
		OffsetRect (&dialogBounds, hPos, vPos + MBarHeight);
		(*dialogHandle)->boundsRect = dialogBounds;
		
		// Restore handle state.
		HSetState ((Handle) dialogHandle, wasState);
	}
}

// --------------------------------------------------------  centerAlert

void centerAlert (short alertID) {
	AlertTHndl	alertHandle;
	Rect		scrnRect, alertRect;
	short		horiOff, vertOff;
	Byte		wasState;
	
	// Get the screen dimensions (minus menu bar height).
	scrnRect = screenBits.bounds;
	scrnRect.top += MBarHeight;
	
	// Get a handle to the alert.
	alertHandle = (AlertTHndl) GetResource ('ALRT', alertID);
	if (alertHandle) {
		// Save handle state, lock it.
		wasState = HGetState ((Handle) alertHandle);
		HLock ((Handle) alertHandle);
		
		// Get alert bounds, zero the origin.
		alertRect = (**alertHandle).boundsRect;
		OffsetRect (&alertRect, -alertRect.left, -alertRect.top);
		
		// Compute horizontal center and 1/3 of the way down vertically.
		horiOff = ((scrnRect.right - scrnRect.left) - alertRect.right) / 2;	
		vertOff = ((scrnRect.bottom - scrnRect.top) - alertRect.bottom) / 3;
		
		// Offset bounds and assign.
		OffsetRect (&alertRect, horiOff, vertOff + MBarHeight);
		(**alertHandle).boundsRect = alertRect;
		
		// Restore handle state.
		HSetState ((Handle)alertHandle, wasState);
	}
}

// --------------------------------------------------------  drawDefaultButton

void drawDefaultButton (DialogPtr theDialog, short itemNumber) {
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	if (theDialog) {
		// Get the item bounds, 'outset' it.
		GetDItem (theDialog, itemNumber, &itemType, &itemHandle, &itemRect);
		InsetRect (&itemRect, -4, -4);
		
		// Stroke a 3 pixel border around item bounds.
		PenSize (3, 3);
		FrameRoundRect (&itemRect, 16, 16);
		
		// Reset drawing state.
		PenNormal ();
	}
}
