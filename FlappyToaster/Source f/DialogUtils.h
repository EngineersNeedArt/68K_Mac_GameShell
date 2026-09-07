// ===========================================================================
//	DialogUtils.h
// ===========================================================================

/// Visually centers the dialog with dialogID. It is placed horizontally centered, vertically 1/3 of the way down.
void centerDialog (short dialogID);

/// Visually centers the alert with alertID. It is placed horizontally centered, vertically 1/3 of the way down.
void centerAlert (short alertID);

/// Draws a fat, rounded border around itemNumber for theDialog.
void drawDefaultButton (DialogPtr theDialog, short itemNumber);