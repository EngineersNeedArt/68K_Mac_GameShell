// ===========================================================================
//	Events.c
// ===========================================================================


#include "Events.h"
#include "MainWindow.h"
#include "Menu.h"


Rect		dragRect;
Rect		sizeRect;
Boolean		quitting = FALSE;


void _handleMouseEvent (EventRecord *);
void _handleKeyEvent (EventRecord *);


// ========================================================  Private Functions
// --------------------------------------------------------  _handleMouseEvent

void _handleMouseEvent (EventRecord *theEvent) {
	WindowPtr	whichWindow;
	long		growth, menuChoice;
	short		thePart;
	
	// Handle mouse events.
	// Get the window and the part of the window from the event.
	thePart = FindWindow (theEvent->where, &whichWindow);
	
	// Most of the window parts have system calls to handle those events (dragging, closing, etc.).
	// We do handle the menu bar events (could handle the events inContent as well).
	
	switch (thePart) {
		case inSysWindow:
			SystemClick (theEvent, whichWindow);
			break;
			
		case inMenuBar:
			// The event is a menu selection, pass to out handler.
			menuChoice = MenuSelect (theEvent->where);
			doMenuChoice (menuChoice);
			break;
		
		case inDrag:
			DragWindow (whichWindow, theEvent->where, &dragRect);
			break;
		
		case inGoAway:
			if (TrackGoAway (whichWindow,theEvent->where)) {
			}
			break;
		
		case inGrow:
			growth = GrowWindow (whichWindow, theEvent->where, &sizeRect);
			if (growth) {
				SetPort (whichWindow);
				SizeWindow (whichWindow, LoWord (growth), HiWord (growth), TRUE);
				InvalRect (&thePort->portRect);
			}
			break;
		
		case inZoomIn:
		case inZoomOut:
			if (TrackBox (whichWindow, theEvent->where, thePart))
				ZoomWindow (whichWindow, thePart, TRUE);
			break;
		
		case inContent:
			// Here we would handle mouse events within the content of our window.
			break;
		
		default:
			break;
	}
}

// --------------------------------------------------------  _handleKeyEvent

void _handleKeyEvent (EventRecord *theEvent) {
	register char	theChar;
	
	// Handle key(board) events.
	
	// Get the character and modifiers (shift, command, etc.) from the event.
	theChar = theEvent->message & charCodeMask;
	if ((theEvent->modifiers & cmdKey) != 0) {
		// If it was a Command-key (shortcut) call the menu handling routine.
		doMenuChoice (MenuKey (theChar));
	} else  {
		// Here we could handle key events that are not Command-key events.
		// This is not always a desirable way to handle the keyboard in 
		// action games however. (Why they're disabled here.)
		switch (theChar) {
//			case kLeftArrowKeyASCII:
//				break;
			
//			case kRightArrowKeyASCII:
//				break;
			
//			case kUpArrowKeyASCII:
//				break;
			
//			case kDownArrowKeyASCII:
//				break;
			
			default:
				break;
		}
	}
}

// ========================================================  Public Functions
// --------------------------------------------------------  handleEvents

void handleEvents (void) {
	EventRecord	theEvent;
	long		sleep = 0;
	
	// We handle a single event (if there is one).
	if (WaitNextEvent (everyEvent, &theEvent, sleep, NULL)) {
		switch (theEvent.what) {
			case mouseDown:
				_handleMouseEvent (&theEvent);
				break;
			
			case keyDown:
			case autoKey:
				_handleKeyEvent (&theEvent);
				break;
			
			case updateEvt:
				// Handle updating (redrawing) our main window.
				if ((WindowPtr)theEvent.message == mainWindow) {
					SetPort ((GrafPtr) mainWindow);
					BeginUpdate ((GrafPtr) mainWindow);
					updateMainWindow ();
					EndUpdate ((GrafPtr) mainWindow);
				}
				break;
			
			default:
				break;
		}
	}
}
