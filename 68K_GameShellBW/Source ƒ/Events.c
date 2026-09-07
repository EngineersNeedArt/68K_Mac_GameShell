//============================================================================
//	Events.c
//============================================================================


#include "Events.h"
#include "MainWindow.h"
#include "Menu.h"


Rect		dragRect;
Rect		sizeRect;
Boolean		quitting;


void handleMouseEvent (EventRecord *);
void handleKeyEvent (EventRecord *);


//========================================================  Functions
//--------------------------------------------------------  handleMouseEvent

void handleMouseEvent (EventRecord *theEvent) {
	WindowPtr	whichWindow;
	long		growth, menuChoice;
	short		thePart;
	
	thePart = FindWindow(theEvent->where, &whichWindow);
	
	switch (thePart) {
		case inSysWindow:
			SystemClick(theEvent, whichWindow);
			break;
		
		case inMenuBar:
			menuChoice = MenuSelect(theEvent->where);
			doMenuChoice(menuChoice);
			break;
		
		case inDrag:
			DragWindow(whichWindow, theEvent->where, &dragRect);
			break;
		
		case inGoAway:
			if (TrackGoAway(whichWindow,theEvent->where)) {
			}
			break;
		
		case inGrow:
			growth = GrowWindow(whichWindow, theEvent->where, &sizeRect);
			if (growth)
			{
				SetPort(whichWindow);
				SizeWindow(whichWindow, LoWord(growth), HiWord(growth), TRUE);
				InvalRect(&thePort->portRect);
			}
			break;
		
		case inZoomIn:
		case inZoomOut:
			if (TrackBox(whichWindow, theEvent->where, thePart))
				ZoomWindow(whichWindow, thePart, TRUE);
			break;
				
		case inContent:
			break;
			
		default:
			break;
	}
}

//--------------------------------------------------------  handleKeyEvent

void handleKeyEvent (EventRecord *theEvent) {
	register char	theChar;
	
	theChar = theEvent->message & charCodeMask;
	if ((theEvent->modifiers & cmdKey) != 0) {
		doMenuChoice(MenuKey(theChar));
	} else  {
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

//--------------------------------------------------------  handleEvents

void handleEvents (void){
	EventRecord	theEvent;
	long		dummyLong, sleep = 0;
	char		theKey;
	Boolean		itHappened;
	
	itHappened = WaitNextEvent(everyEvent, &theEvent, sleep, NULL);
	
	if (itHappened) {
		switch (theEvent.what) {
			case mouseDown:
				handleMouseEvent(&theEvent);
				break;
			
			case keyDown:
			case autoKey:
				handleKeyEvent(&theEvent);
				break;
			
			case updateEvt:
				if ((WindowPtr)theEvent.message == mainWindow) {
					SetPort((GrafPtr)mainWindow);
					BeginUpdate((GrafPtr)mainWindow);
					updateMainWindow();
					EndUpdate((GrafPtr)mainWindow);
				}
				break;
			
			default:
				break;
		}
	}
}