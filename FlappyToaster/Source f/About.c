// ===========================================================================
//	About.c
// ===========================================================================


#include "About.h"
#include "DialogUtils.h"
#include "Events.h"
#include "MainWindow.h"
#include "Utilities.h"


#define kAboutDialogID		100		// resourceID of 'About' DLOG
#define kOkayButtonItem		1


// ========================================================  Public Functions
// --------------------------------------------------------  doAbout

void doAbout (void) {
	DialogPtr aboutDialog;
	short itemHit = 0;
	GrafPtr		oldPort;
	
	// Center and load the dialog.
	centerDialog (kAboutDialogID);
	aboutDialog = GetNewDialog (kAboutDialogID, NULL, (WindowPtr) -1);
	if (!aboutDialog) {
		fatalError (kErrFailedResourceLoad);
		return;
	}
	
	ParamText ("\pGameShellC", "\pNemo", "\pv. 1.0", "\p");
	
	// Show the dialog, make it the current graphics port.
	GetPort (&oldPort);
	ShowWindow ((GrafPtr) aboutDialog);
	SetPort ((GrafPtr) aboutDialog);
	
	drawDefaultButton (aboutDialog, kOkayButtonItem);
	
	// Loop until a button is hit.
	while (itemHit == 0) {
		ModalDialog (NULL, &itemHit);
	}
	
	// Finished.
	DisposeDialog (aboutDialog);
	SetPort (oldPort);
}
