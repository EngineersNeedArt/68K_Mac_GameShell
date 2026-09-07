// ===========================================================================
//	B&W Game Shell in C
// ===========================================================================


#include "Events.h"
#include "Play.h"


// ========================================================  Functions
// --------------------------------------------------------  main

void main (void) {
	// Initialize everything.
	if (!initializeAll ()) {
		goto bail;
	}
	
	// Sit in an event loop until the user has chosen to quit.
	while (!quitting) {
		handleEvents ();
		if (playing)
			playGame ();
	}
	
bail:
	
	// Shut down everything before our app quits.
	shutdownGame ();
}
