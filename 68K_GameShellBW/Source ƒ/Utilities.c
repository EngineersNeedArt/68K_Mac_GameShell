//============================================================================
//	Utilities.c
//============================================================================


#include "DialogUtils.h"
#include "Utilities.h"


#define kFatalAlertID				1001
#define kFatalTitleStringsID		1001
#define kFatalDescriptionStringsID	1002


//========================================================  Functions
//--------------------------------------------------------  randomShort

short randomShort (short range) {
	register long	rawResult;
	
	rawResult = Random();
	if (rawResult < 0)
		rawResult *= -1;
	return (rawResult * (long)range / 32768);
}

//--------------------------------------------------------  getPICTBounds

void getPICTBounds (short pictID, Rect *theBounds) {
	PicHandle	thePicture;
	Byte		wasState;
	
	thePicture = GetPicture(pictID);
	if (!thePicture) {
		fatalError(kErrFailedResourceLoad);
	}
	
	wasState = HGetState((Handle)thePicture);
	HLock((Handle)thePicture);
	*theBounds = (*thePicture)->picFrame;
	HSetState((Handle)thePicture, wasState);
	
	ReleaseResource((Handle)thePicture);
}

//--------------------------------------------------------  fatalError

void fatalError (short errorNumber) {
	Str255	errTitle;
	Str255	errMessage;
	Str255	errNumberString;
	
	InitCursor();
	
	// Set appropriate error message.
	if (errorNumber > 1) {
		GetIndString(errTitle, kFatalTitleStringsID, errorNumber);
		GetIndString(errMessage, kFatalDescriptionStringsID, errorNumber);
	} else {
		GetIndString(errTitle, kFatalTitleStringsID, 1);
		GetIndString(errMessage, kFatalDescriptionStringsID, 1);
	}
	NumToString((long)errorNumber, errNumberString);
	ParamText(errTitle, errMessage, errNumberString, "\p");
	
	// Center and display error alert.
	centerAlert(kFatalAlertID);
	Alert(kFatalAlertID, NULL);
	
	// Quit application.
	ExitToShell();
}

