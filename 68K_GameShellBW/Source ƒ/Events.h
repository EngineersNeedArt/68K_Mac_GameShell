//============================================================================
//	Events.h
//============================================================================



#define kEnterKeyASCII				0x03	// ASCII code for the Enter key
#define	kReturnKeyASCII				0x0D	// ASCII code for the Return key

#define kSpaceBarMap				54		// key map offset for Space Bar
#define	kLeftArrowKeyMap			124		// key map offset for the Left Arrow key
#define kRightArrowKeyMap			123		// key map offset for the Right Arrow key
#define	kUpArrowKeyMap				126		// key map offset for the Up Arrow key
#define kDownArrowKeyMap			125		// key map offset for the Down Arrow key


extern Boolean		quitting;


void handleEvents (void);
