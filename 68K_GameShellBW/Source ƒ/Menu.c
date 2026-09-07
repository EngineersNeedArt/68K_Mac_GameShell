//============================================================================
//	Menu.c
//============================================================================


#include "About.h"
#include "Events.h"
#include "Menu.h"
#include "Play.h"


#define kAppleMenuID				128
#define iAboutItem					1

#define kGameMenuID					129
#define iNewGameItem				1
#define iPauseItem					2
#define iEndGameItem				3
#define iQuitItem					5


MenuHandle	appleMenu;
MenuHandle	gameMenu;


void doAppleMenu (short);
void doGameMenu (short);


//========================================================  Functions
//--------------------------------------------------------  doAppleMenu

void doAppleMenu (short theItem) {
	Str255		daName;
	GrafPtr		wasPort;
	short		daNumber;
	
	switch (theItem) {
		case iAboutItem:
			doAbout(FALSE);
			break;
		
		default:
			GetItem(appleMenu, theItem, daName);
			GetPort(&wasPort);
			daNumber = OpenDeskAcc(daName);
			SetPort((GrafPtr)wasPort);
			break;
	}
}

//--------------------------------------------------------  doGameMenu

void doGameMenu (short theItem) {
	switch (theItem) {
		case iNewGameItem:
			beginNewGame();
			break;
		
		case iPauseItem:
			pausing = !pausing;
			break;
		
		case iEndGameItem:
			break;
		
		case iQuitItem:
			quitting = TRUE;
			break;
		
		default:
			break;
	}
}

//--------------------------------------------------------  doMenuChoice

void doMenuChoice (long menuChoice) {
	if (menuChoice == 0)
		return;
	
	switch (HiWord(menuChoice)) {
		case kAppleMenuID:
			doAppleMenu(LoWord(menuChoice));
			break;
		
		case kGameMenuID:
			doGameMenu(LoWord(menuChoice));
			break;
		
		default:
			break;
	}
	
	HiliteMenu(0);
}

//--------------------------------------------------------  menuBarInit

void menuBarInit (void) {
	appleMenu = GetMenu(kAppleMenuID);
	AddResMenu(appleMenu, 'DRVR');
	InsertMenu(appleMenu, 0);
	
	gameMenu = GetMenu(kGameMenuID);
	InsertMenu(gameMenu, 0);
	
	DrawMenuBar();
}

