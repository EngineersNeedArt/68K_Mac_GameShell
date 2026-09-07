// ===========================================================================
//	Play.h
// ===========================================================================


typedef enum {
	PLAYERSTATE_NORMAL		= 0,
	PLAYERSTATE_CRASHING	= 1,
	PLAYERSTATE_CRASHED		= 2
} PlayerState;

typedef enum {
	TOASTSTATE_INVISIBLE	= 0,
	TOASTSTATE_NORMAL		= 1,
	TOASTSTATE_BEHIND		= 2,
	TOASTSTATE_CRASHING		= 3,
	TOASTSTATE_CRASHED		= 4
} ToastState;


struct PlayerType {
	Rect		srcRect;
	Rect		destRect;
	Rect		dirtyRect;
	Rect		shadowSrcRect;
	Rect		shadowDestRect;
	Rect		shadowDirtyRect;
	Rect		collisionRect;
	int			hPos;
	int			vPos;
	int			vVelocity;
	int			wingState;
	PlayerState	state;
	long 		counter;
	Boolean		flap;
};

struct ToastType {
	Rect		destRect;
	Rect		dirtyRect;
	Rect		shadowDestRect;
	Rect		shadowDirtyRect;
	Rect		collisionRect;
	int			hPos;
	int			vPos;
	int			vVelocity;
	ToastState	state;
};


extern Boolean	playing;
extern Boolean	pausing;


/// Call to prepare a new game. Initializes player, sound, etc.
void beginNewGame (void);

/// Call to play the game. Returns when the user quits (or the game ends).
void playGame (void);

/// Sets up source rectangles. Called once when the game is just launched.
void gameInit (void);

/// Shuts down the sound, closes the main window. Call just before the game quits.
void shutdownGame (void);
