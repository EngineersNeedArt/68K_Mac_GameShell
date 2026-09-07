//============================================================================
//	Play.c
//============================================================================


#include "Events.h"
#include "MainWindow.h"
#include "Play.h"
#include "Render.h"
#include "SoundUtils.h"


#define	kCommandKeyMap				48		// key map offset for Command key
#define	kQKeyMap					11		// key map offset for Q key


struct PlayerType	player;
Rect				shipSourceRects[16];
KeyMap				theKeys;
Boolean				pausing;
Boolean				playing;


void endGame (void);
void checkCommandKeys (void);
//void getPlayerInput (PlayerType *who);
void handlePlayer (struct PlayerType *who);
void copyPlayerToScratch (struct PlayerType *who);
void copyPlayerToMainWindow(struct PlayerType *who);
void playerRestoreScratch (struct PlayerType *who);
void playerInit (void);


//========================================================  Functions
//--------------------------------------------------------  beginNewGame

void beginNewGame (void) {
	OSErr		theErr;
	
	playerInit();
	enableSound();
	playing = TRUE;
	HideCursor();
}

//--------------------------------------------------------  endGame

void endGame (void) {
	OSErr		theErr;
	
	disableSound();
	InitCursor();
}

//--------------------------------------------------------  checkCommandKeys

void checkCommandKeys (void) {	
	if (BitTst(&theKeys, kCommandKeyMap)) {
		if (BitTst(&theKeys, kQKeyMap)) {
			playing = FALSE;
			quitting = TRUE;
		}
	}
}

//--------------------------------------------------------  playGame

void playGame (void) {
	long		nextFrameTime;
	
	nextFrameTime = Ticks + 1;
	
	while (playing) {
		GetKeys(theKeys);
		checkCommandKeys();
		
		if (!pausing) {
			handlePlayer(&player);
			
			copyPlayerToScratch(&player);
			copyPlayerToMainWindow(&player);
			playerRestoreScratch(&player);
			
//			prepareOffscreen();
			while (Ticks < nextFrameTime) {
			}
			nextFrameTime = Ticks + 1;
//			displayScene();
		}
	}
	
	endGame();
}

//--------------------------------------------------------  getPlayerInput

void getPlayerInput (struct PlayerType *who) {
//	if (BitTst(&theKeys, kSpaceBarMap)) {
//		who->vVel += kVerticalThrust;
//	}
	
	if (BitTst(&theKeys, kLeftArrowKeyMap)) {
		who->direction += 1;
		if (who->direction > 15) {
			who->direction = 0;
		}
		who->sourceRect = shipSourceRects[who->direction];
	} else if (BitTst(&theKeys, kRightArrowKeyMap)) {
		who->direction -= 1;
		if (who->direction < 0) {
			who->direction = 15;
		}
		who->sourceRect = shipSourceRects[who->direction];
	}
}

//--------------------------------------------------------  handlePlayer

void handlePlayer (struct PlayerType *who) {
	getPlayerInput (who);
	
	who->dirtyRect = who->destRect;
	OffsetRect(&(who->destRect), who->xVelocity, who->yVelocity);
	
	if (who->destRect.left < 0) {
		int delta = -who->destRect.left;
		OffsetRect(&(who->destRect), delta, 0);
		who->xVelocity = -who->xVelocity;
		playSoundWithIndex(0);
	} else if (who->destRect.right > kGameWidth) {
		int delta = kGameWidth - who->destRect.right;
		OffsetRect(&(who->destRect), delta, 0);
		who->xVelocity = -who->xVelocity;
		playSoundWithIndex(0);
	}
	
	if (who->destRect.top < 0) {
		int delta = -who->destRect.top;
		OffsetRect(&(who->destRect), 0, delta);
		who->yVelocity = -who->yVelocity;		
		playSoundWithIndex(0);
	} else if (who->destRect.bottom > kGameHeight) {
		int delta = kGameHeight - who->destRect.bottom;
		OffsetRect(&(who->destRect), 0, delta);
		who->yVelocity = -who->yVelocity;
		playSoundWithIndex(0);
	}
	UnionRect(&who->dirtyRect, &who->destRect, &who->dirtyRect);
}

//--------------------------------------------------------  copyPlayerToScratch

void copyPlayerToScratch (struct PlayerType *who) {
	CopyMask(&((GrafPtr)spriteOffscreen)->portBits, 
			&((GrafPtr)maskOffscreen)->portBits, 
			&((GrafPtr)scratchOffscreen)->portBits, 
			&(who->sourceRect), &(who->sourceRect), &(who->destRect));
}

//--------------------------------------------------------  copyPlayerToMainWindow

void copyPlayerToMainWindow(struct PlayerType *who) {
	Rect mainDestRect = who->dirtyRect;
	
	OffsetRect(&mainDestRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	CopyBits(&((GrafPtr)scratchOffscreen)->portBits, 
			&(((GrafPtr)mainWindow)->portBits), 
			&(who->dirtyRect), &mainDestRect, srcCopy, 
			(RgnHandle)((GrafPtr)mainWindow->visRgn));
}

//--------------------------------------------------------  playerRestoreScratch

void playerRestoreScratch (struct PlayerType *who) {
	CopyBits(&((GrafPtr)backgroundOffscreen)->portBits,
			&((GrafPtr)scratchOffscreen)->portBits,
			&(who->dirtyRect), &(who->dirtyRect), srcCopy, NULL);
}

//--------------------------------------------------------  playerInit

void playerInit (void) {
	player.direction = 0;
	player.sourceRect = shipSourceRects[player.direction];
	SetRect(&(player.destRect), 0, 0, 32, 32);
	player.xVelocity = 1;
	player.yVelocity = 1;
}

//--------------------------------------------------------  gameInit

void gameInit (void) {
	int	i;
	for (i = 0; i < 16; i++) {
		SetRect(&(shipSourceRects[i]), 0, 0, 32, 32);
		OffsetRect(&(shipSourceRects[i]), (i * 33) + 1, 1);
	}
}

//--------------------------------------------------------  shutdownGame

void shutdownGame (void) {
	killSound();
	closeMainWindow ();
	FlushEvents(everyEvent, 0);
}