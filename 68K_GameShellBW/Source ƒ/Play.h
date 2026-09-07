//============================================================================
//	Play.h
//============================================================================


struct PlayerType {
	Rect	sourceRect;
	Rect	destRect;
	Rect	dirtyRect;
	int		xVelocity;
	int		yVelocity;
	int		direction;
};


extern Boolean	playing;
extern Boolean	pausing;


void beginNewGame (void);
void playGame (void);
void gameInit (void);
void shutdownGame (void);
