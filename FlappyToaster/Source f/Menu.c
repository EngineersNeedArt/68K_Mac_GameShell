// ===========================================================================
//	Menu.c
// ===========================================================================


#include "About.h"
#include "Events.h"
#include "Menu.h"
#include "Play.h"


#define kAppleMenuID		128
#define iAboutItem			1

#define kGameMenuID			129
#define iNewGameItem		1
#define iPauseItem			2
#define iEndGameItem		3
#define iQuitItem			5


MenuHandle	appleMenu;
MenuHandle	gameMenu;


void _doAppleMenu (short);
void _doGameMenu (short);


// ========================================================  Private Functions
// --------------------------------------------------------  _doAppleMenu

void _doAppleMenu (short theItem) {
	Str255		daName;
	GrafPtr		wasPort;
	short		daNumber;
	
	switch (theItem) {
		case iAboutItem:
			// Handle our 'about box'.
			doAbout ();
			break;
		
		default:
			// Handle a Desk Accessory.
			GetItem (appleMenu, theItem, daName);
			GetPort (&wasPort);
			daNumber = OpenDeskAcc (daName);
			SetPort ((GrafPtr) wasPort);
			break;
	}
}

// --------------------------------------------------------  _doGameMenu

void _doGameMenu (short theItem) {
	switch (theItem) {
		case iNewGameItem:
			// Start a new game.
			beginNewGame ();
			break;
		
		case iPauseItem:
			// Toggle pause state.
			pausing = !pausing;
			break;
		
		case iEndGameItem:
			// To be handled.
			break;
		
		case iQuitItem:
			// Quit the application.
			quitting = TRUE;
			break;
		
		default:
			break;
	}
}

// ========================================================  Public Functions
// --------------------------------------------------------  doMenuChoice

void doMenuChoice (long menuChoice) {
	if (menuChoice == 0) {
		return;
	}
	
	switch (HiWord (menuChoice)) {
		case kAppleMenuID:
			_doAppleMenu (LoWord (menuChoice));
			break;
		
		case kGameMenuID:
			_doGameMenu (LoWord (menuChoice));
			break;
		
		default:
			break;
	}
	
	HiliteMenu (0);
}

// --------------------------------------------------------  menuBarInit

void menuBarInit (void) {
	// Create the Apple menu, populate with Desk Asccessories.
	appleMenu = GetMenu (kAppleMenuID);
	AddResMenu (appleMenu, 'DRVR');
	InsertMenu (appleMenu, 0);
	
	// Add our game menu. 
	gameMenu = GetMenu (kGameMenuID);
	InsertMenu (gameMenu, 0);
	
	DrawMenuBar ();
}
