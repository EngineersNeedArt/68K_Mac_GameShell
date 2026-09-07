// ===========================================================================
//	MainWindow.h
// ===========================================================================


extern WindowPtr	mainWindow;


/// Called to open our main window, full-screen.
void openMainWindow (void);

/// Called in response to an update event. Redraws the window contents.
void updateMainWindow (void);

/// Called when the game quits, to close our window.
void closeMainWindow (void);
