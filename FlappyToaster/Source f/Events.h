// ===========================================================================
//	Events.h
// ===========================================================================


// ASCII values offsets for various characters.
#define kEnterKeyASCII				0x03	// Enter
#define	kReturnKeyASCII				0x0D	// Return

// KeyMap offsets for various keys.
#define	kQKeyMap					11		// Q key
#define	kCommandKeyMap				48		// Command key
#define kSpaceBarMap				54		// Space Bar
#define	kLeftArrowKeyMap			124		// Left Arrow key
#define kRightArrowKeyMap			123		// Right Arrow key
#define	kUpArrowKeyMap				126		// Up Arrow key
#define kDownArrowKeyMap			125		// Down Arrow key


extern Boolean		quitting;


void handleEvents (void);
