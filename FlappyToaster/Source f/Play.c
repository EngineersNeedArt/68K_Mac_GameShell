// ===========================================================================
//	Play.c
// ===========================================================================


#include "Events.h"
#include "MainWindow.h"
#include "Play.h"
#include "Render.h"
#include "SoundUtils.h"


#define kMaxToastCount	4

#define kToastInitalH		512 + 32
#define kToastHRange		200
#define kToastMinimumH		40
#define kToastShadowInitalV	397

#define kCeiling			240
#define kCrashDelay			200
#define kAheadToastPadding	25
#define kBehindToastPadding	-28
#define kBelowToastPadding	16


struct PlayerType	playerOne;
Rect				toasterSrcRects[4];
struct ToastType	toast[kMaxToastCount];
Rect				toastSrcRect;
Rect				toastShadowSrcRect;
Rect				sidewalkCrackSrcRect;
Rect				sidewalkCrackDestRect;
Rect				digitSrcRects[10];
Rect				gameOverSrcRect;
Rect				gameOverDestRect;
short				numToasts = 0;
short				numLives = 5;
short				score = 0;
KeyMap				theKeys;
long				gameTick = 0L;
long				toastCounter = 0L;
Boolean				pausing = FALSE;
Boolean				playing = FALSE;
Boolean 			scoreboardInvalid = FALSE;


void _endGame (void);
void _checkCommandKeys (void);
void _getPlayerInput (struct PlayerType *thePlayer);
void _handlePlayer (struct PlayerType *thePlayer);
void _copyPlayerToScratch (struct PlayerType *thePlayer);
void _copyPlayerToMainWindow(struct PlayerType *thePlayer);
void _playerRestoreScratch (struct PlayerType *thePlayer);
void _playerInit (void);
void _toastInit (void);


// ========================================================  Private Functions
// --------------------------------------------------------  _endGame

void _endGame (void) {
	OSErr		theErr;
	
	disableSound ();
	InitCursor ();
}

// --------------------------------------------------------  _hPosVPos2XY

void _hPosVPos2XY (short h, short v, short *x, short *y) {
	*x = h;
	*y = (144 - v) + (h / 2);
}

// --------------------------------------------------------  _toastCounterReset

long _toastCounterReset (short points) {
	short levelEase = 5 - (points / 5);
	
	if (levelEase < 0) {
		levelEase = 0;
	}
	
	return (levelEase * 25);
}

// --------------------------------------------------------  _calculatePlayerRects

void _calculatePlayerRects (struct PlayerType *thePlayer) {
	short	x;
	short	y;
	
	thePlayer->destRect = toasterSrcRects[thePlayer->wingState];
	OffsetRect (&(thePlayer->destRect), 
			-thePlayer->destRect.left, 
			-thePlayer->destRect.top);
	_hPosVPos2XY (thePlayer->hPos, thePlayer->vPos, &x, &y);
	OffsetRect (&(thePlayer->destRect), x - 36, y - 50);
	
	thePlayer->shadowDestRect = thePlayer->shadowSrcRect;
	OffsetRect (&(thePlayer->shadowDestRect), 
			-thePlayer->shadowDestRect.left, 
			-thePlayer->shadowDestRect.top);
	_hPosVPos2XY (thePlayer->hPos, 0, &x, &y);
	OffsetRect (&(thePlayer->shadowDestRect), x - 36, y - 12);
	
	SetRect (&(thePlayer->collisionRect), thePlayer->hPos - 26, 300 - (thePlayer->vPos + 20), 
			thePlayer->hPos + 26, 300 - (thePlayer->vPos - 20));

}

// --------------------------------------------------------  _calculateToastRects

void _calculateToastRects (struct ToastType *theToast) {
	short	x;
	short	y;
	
	theToast->destRect = toastSrcRect;
	OffsetRect (&(theToast->destRect), -toastSrcRect.left, -toastSrcRect.top);
	_hPosVPos2XY (theToast->hPos, theToast->vPos, &x, &y);
	OffsetRect (&(theToast->destRect), x - 32, y - 25);
	
	theToast->shadowDestRect = toastShadowSrcRect;
	OffsetRect (&(theToast->shadowDestRect), -toastShadowSrcRect.left, -toastShadowSrcRect.top);
	_hPosVPos2XY (theToast->hPos, 0, &x, &y);
	OffsetRect (&(theToast->shadowDestRect), x - 32, y - 20);
	
	SetRect (&(theToast->collisionRect), theToast->hPos - 26, 300 - (theToast->vPos + 8), 
			theToast->hPos + 26, 300 - (theToast->vPos - 8));
}

// --------------------------------------------------------  _resetSideWalkCrackDestRect

void _resetSideWalkCrackDestRect (void) {
	SetRect (&sidewalkCrackDestRect, 366, 322, 366 + 48, 322 + 24);
}

// --------------------------------------------------------  _checkCommandKeys

void _checkCommandKeys (void) {	
	if (BitTst (&theKeys, kCommandKeyMap)) {
		if (BitTst (&theKeys, kQKeyMap)) {
			playing = FALSE;
			quitting = TRUE;
		}
	}
}

// --------------------------------------------------------  _getPlayerInput

void _getPlayerInput (struct PlayerType *thePlayer) {
	thePlayer->flap = Button ();
}

// --------------------------------------------------------  _addToast

void _addToast (short index) {
	toast[index].hPos = kToastInitalH;
	toast[index].vPos = randomShort (kToastHRange) + kToastMinimumH;
	toast[index].vVelocity = 0;
	_calculateToastRects (&(toast[index]));
	toast[index].state = TOASTSTATE_NORMAL;
}

// --------------------------------------------------------  _handleToast

void _handleToast () {
	short	i;
	short	emptyToastSlot = -1;
	short	activeToastCount = 0;
	
	for (i = 0; i < kMaxToastCount; i++) {
		// Hide any toast that has passed off the left edge of the screen.
		if (toast[i].destRect.right < 0) {
			toast[i].state = TOASTSTATE_INVISIBLE;
		}
		
		// See if toast is visible, skip if not.
		// Make a note though of an empty slot to possible be used later.
		if (toast[i].state == TOASTSTATE_INVISIBLE) {
			emptyToastSlot = i;
			continue;
		}
		
		// Handle a visible piece of toast.
		activeToastCount += 1;
		
		// Initial dirty rect (last destination).
		toast[i].dirtyRect = toast[i].destRect;

		// Handle falling toast.
		if (toast[i].state == TOASTSTATE_CRASHING) {
			toast[i].vVelocity += 1;

			// Advance toast to right.
			toast[i].hPos += 1;
			
			// Move the toast (we scale the velocity down by 8).
			toast[i].vPos = toast[i].vPos - toast[i].vVelocity / 8;
			if (toast[i].vPos <= 0) {
				toast[i].vPos = 0;
				toast[i].state = TOASTSTATE_CRASHED;
			}
		} else {
			// Advance toast to left.
			toast[i].hPos -= 2;
		}
		
		_calculateToastRects (&toast[i]);
		UnionRect (&toast[i].dirtyRect, &toast[i].destRect, &toast[i].dirtyRect);
		
		toast[i].shadowDirtyRect = toast[i].shadowDestRect;
		OffsetRect (&toast[i].shadowDestRect, -2, -1);
		UnionRect (&toast[i].shadowDirtyRect, &toast[i].shadowDestRect, &toast[i].shadowDirtyRect);
	}
	
	// If the counter has counted down to zero and there is a slot available, add toast.
	if ((toastCounter < 0) && (emptyToastSlot >= 0)) {
		_addToast (emptyToastSlot);
		
		// Reset counter.
		toastCounter = _toastCounterReset (score);
	}
}

// --------------------------------------------------------  _checkPlayerToastCollision

void _checkPlayerToastCollision (struct PlayerType *thePlayer) {
	short	i;
	short 	hitIndex = -1;
	Rect	intersection;
	short	playerTrailingEdge = thePlayer->collisionRect.left;
	short	toastTrailingEdge;
	
	for (i = 0; i < kMaxToastCount; i++) {
		// See if toast is visible, skip if not.
		if (toast[i].state != TOASTSTATE_NORMAL) {
			continue;
		}
		
		if (SectRect (&(thePlayer->collisionRect), &(toast[i].collisionRect), &intersection)) {
			playSoundWithIndex (kSoundIndexToastCollide);
			thePlayer->state = PLAYERSTATE_CRASHING;
			toast[i].state = TOASTSTATE_CRASHING;
			numLives = numLives - 1;
			scoreboardInvalid = TRUE;
		}
		
		if (toast[i].state == TOASTSTATE_NORMAL) {
			toastTrailingEdge = toast[i].collisionRect.right;
			if (toastTrailingEdge < playerTrailingEdge) {
				toast[i].state = TOASTSTATE_BEHIND;
				score = score + 1;
				playSoundWithIndex (kSoundIndexScorePoint);
				scoreboardInvalid = TRUE;
			}
		}
	}
}

// --------------------------------------------------------  _playerReset

void _playerReset (void) {
	playerOne.hPos = 200;
	playerOne.vPos = 200;
	_calculatePlayerRects (&playerOne);
	
	SetRect (&(playerOne.shadowSrcRect), 0, 256, 64, 272);
	
	playerOne.vVelocity = 0;
	playerOne.wingState = 1;
	playerOne.flap = FALSE;
	playerOne.state = PLAYERSTATE_NORMAL;
}

// --------------------------------------------------------  _handlePlayer

void _handlePlayer (struct PlayerType *thePlayer) {
	Boolean wasFlap = thePlayer->flap;
	
	if (thePlayer->state == PLAYERSTATE_CRASHED) {
		// Take the (previous) destination rectangle as the current dirty rectangle.
		thePlayer->dirtyRect = thePlayer->destRect;
		thePlayer->shadowDirtyRect = thePlayer->shadowDestRect;
		
		OffsetRect (&(thePlayer->destRect), -2, -1);
		OffsetRect (&(thePlayer->shadowDestRect), -2, -1);
		
		if (thePlayer->destRect.right <= 0) {
			if (numLives == 0) {
				playing = FALSE;
			} else {
				_playerReset ();
			}
		}
	} else {
		if (thePlayer->state == PLAYERSTATE_CRASHING) {
			thePlayer->flap = FALSE;
		} else {
			// See if the player has hit a piece of toast.
			_checkPlayerToastCollision (thePlayer);
			
			// Get the player's input (if any).
			_getPlayerInput (thePlayer);
		}
		
		// Handle flapping, wing state, falling.
		if (thePlayer->wingState == 0) {
			thePlayer->wingState = 1;
		}
		if (thePlayer->flap) {
			if (thePlayer->wingState != 0) {
				thePlayer->wingState = 0;
				thePlayer->vVelocity -= 8;
			}
			
			if (!wasFlap) {
				if (randomShort (2) == 0) {
					playSoundWithIndex (kSoundIndexFlap0);
				} else {
					playSoundWithIndex (kSoundIndexFlap1);
				}
			}
		} else {		
			if ((thePlayer->vVelocity > 0) && (thePlayer->wingState < 3)) {
				thePlayer->wingState += 1;
			}
		}
		
		thePlayer->srcRect = toasterSrcRects[thePlayer->wingState];
		thePlayer->vVelocity += 1;
		
		// Take the (previous) destination rectangle as the current dirty rectangle.
		thePlayer->dirtyRect = thePlayer->destRect;
		
		// Move the player (we scale the velocity down by 8).
		thePlayer->vPos = thePlayer->vPos - thePlayer->vVelocity / 8;
		_calculatePlayerRects (thePlayer);
		
		// Simple collision tests for the edges of the screen to keep the player from leaving.
		if (thePlayer->vPos > kCeiling) {
			// Handle the player bouncing off the top wall.
			thePlayer->vPos = kCeiling;
			_calculatePlayerRects (thePlayer);
			thePlayer->vVelocity = (thePlayer->vVelocity / -2);	
			playSoundWithIndex (kSoundIndexCeilingCollide);
		} else if (thePlayer->vPos < 0) {
			// Player crashed.
			thePlayer->vPos = 0;
			_calculatePlayerRects (thePlayer);
			thePlayer->vVelocity = 0;
			thePlayer->counter = kCrashDelay;
			thePlayer->state = PLAYERSTATE_CRASHED;
			thePlayer->wingState = 0;
			playSoundWithIndex (kSoundIndexGroundSkid);
		}
	}
	
	// The union of the old player destination and the new one gives us the update (dirty) rectangle.
	UnionRect (&thePlayer->dirtyRect, &thePlayer->destRect, &thePlayer->dirtyRect);
	UnionRect (&thePlayer->shadowDirtyRect, &thePlayer->shadowDestRect, &thePlayer->shadowDirtyRect);
}

// --------------------------------------------------------  _prepareSidewalkCrack

void _prepareSidewalkCrack (void) {
	OffsetRect (&sidewalkCrackDestRect, -2, -1);
	if (sidewalkCrackDestRect.right < 0) {
		_resetSideWalkCrackDestRect ();
	}
	
	CopyBits (&((GrafPtr) spriteOffscreen)->portBits, 
			&((GrafPtr) scratchOffscreen)->portBits, 
			&sidewalkCrackSrcRect, &sidewalkCrackDestRect, srcCopy, 
			NULL);
}

// --------------------------------------------------------  _copyToastToScratch

void _copyToastToScratch (struct ToastType *theToast) {
	// Toast shadow.	
	CopyMask (&((GrafPtr) spriteOffscreen)->portBits, 
			&((GrafPtr) maskOffscreen)->portBits, 
			&((GrafPtr) scratchOffscreen)->portBits, 
			&toastShadowSrcRect, &toastShadowSrcRect, &(theToast->shadowDestRect));
	
	// Toast sprite.
	CopyMask (&((GrafPtr) spriteOffscreen)->portBits, 
			&((GrafPtr) maskOffscreen)->portBits, 
			&((GrafPtr) scratchOffscreen)->portBits, 
			&toastSrcRect, &toastSrcRect, &(theToast->destRect));
}

// --------------------------------------------------------  _copyPlayerToScratch

void _copyPlayerToScratch (struct PlayerType *thePlayer) {
	// Copy pixels from the sprite bitmap to the 'scratch' bitmap using thePlayer's destRect.
	CopyMask (&((GrafPtr) spriteOffscreen)->portBits, 
			&((GrafPtr) maskOffscreen)->portBits, 
			&((GrafPtr) scratchOffscreen)->portBits, 
			&(thePlayer->shadowSrcRect), &(thePlayer->shadowSrcRect), &(thePlayer->shadowDestRect));
	
	CopyMask (&((GrafPtr) spriteOffscreen)->portBits, 
			&((GrafPtr) maskOffscreen)->portBits, 
			&((GrafPtr) scratchOffscreen)->portBits, 
			&(thePlayer->srcRect), &(thePlayer->srcRect), &(thePlayer->destRect));
}

// --------------------------------------------------------  _copySidewalkCrackToMainWindow

void _copySidewalkCrackToMainWindow (void) {
	Rect destRect = sidewalkCrackDestRect;
	OffsetRect (&destRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	CopyBits (&((GrafPtr) scratchOffscreen)->portBits, 
			&(((GrafPtr) mainWindow)->portBits), 
			&sidewalkCrackDestRect, &destRect, srcCopy, 
			mainDisplayRenderRgn);
}

// --------------------------------------------------------  _renderScoreboard

void _renderScoreboard (void) {
	Rect	destRect;
	short	digit;
	
	SetRect (&destRect, 0, 0, 21, 22);
	OffsetRect (&destRect, 385, 34);
	OffsetRect (&destRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	
	digit = numLives % 10;
	CopyBits (&((GrafPtr) spriteOffscreen)->portBits, 
			&(((GrafPtr) mainWindow)->portBits), 
			&digitSrcRects[digit], &destRect, srcCopy, 
			mainDisplayRenderRgn);
	
	SetRect (&destRect, 0, 0, 21, 22);	
	OffsetRect (&destRect, 444, 34);
	OffsetRect (&destRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	
	digit = (score / 10) % 10;
	CopyBits (&((GrafPtr) spriteOffscreen)->portBits, 
			&(((GrafPtr) mainWindow)->portBits), 
			&digitSrcRects[digit], &destRect, srcCopy, 
			mainDisplayRenderRgn);
	
	SetRect (&destRect, 0, 0, 21, 22);	
	OffsetRect (&destRect, 466, 34);	
	OffsetRect (&destRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	
	digit = score % 10;
	CopyBits (&((GrafPtr) spriteOffscreen)->portBits, 
			&(((GrafPtr) mainWindow)->portBits), 
			&digitSrcRects[digit], &destRect, srcCopy, 
			mainDisplayRenderRgn);	
}

// --------------------------------------------------------  _copyToastToMainWindow

void _copyToastToMainWindow (struct ToastType *theToast) {
	Rect dirtyRect;
	
	dirtyRect = theToast->shadowDirtyRect;
	OffsetRect (&dirtyRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	CopyBits (&((GrafPtr) scratchOffscreen)->portBits, 
			&(((GrafPtr) mainWindow)->portBits), 
			&(theToast->shadowDirtyRect), &dirtyRect, srcCopy, 
			mainDisplayRenderRgn);
	
	// Copy pixels from the 'scratch' bitmap to the main window using toast's 'dirty' rectangle.
	dirtyRect = theToast->dirtyRect;
	OffsetRect (&dirtyRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	CopyBits (&((GrafPtr) scratchOffscreen)->portBits, 
			&(((GrafPtr) mainWindow)->portBits), 
			&(theToast->dirtyRect), &dirtyRect, srcCopy, 
			mainDisplayRenderRgn);
}

// --------------------------------------------------------  _copyPlayerToMainWindow

void _copyPlayerToMainWindow (struct PlayerType *thePlayer) {
	Rect dirtyRect = thePlayer->shadowDestRect;
	OffsetRect (&dirtyRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	CopyBits (&((GrafPtr) scratchOffscreen)->portBits, 
			&(((GrafPtr) mainWindow)->portBits), 
			&(thePlayer->shadowDestRect), &dirtyRect, srcCopy, 
			mainDisplayRenderRgn);
	
	// Copy pixels from the 'scratch' bitmap to the main window using thePlayer's 'dirty' rectangle.
	dirtyRect = thePlayer->dirtyRect;
	OffsetRect (&dirtyRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	CopyBits (&((GrafPtr) scratchOffscreen)->portBits, 
			&(((GrafPtr) mainWindow)->portBits), 
			&(thePlayer->dirtyRect), &dirtyRect, srcCopy, 
			mainDisplayRenderRgn);
}

// --------------------------------------------------------  _copyGameOverToMainWindow

void _copyGameOverToMainWindow (void) {
	Rect destRect = gameOverDestRect;
	OffsetRect (&destRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	
	CopyMask (&((GrafPtr) spriteOffscreen)->portBits, 
			&((GrafPtr) maskOffscreen)->portBits, 
			&(((GrafPtr) mainWindow)->portBits), 
			&gameOverSrcRect, &gameOverSrcRect, &destRect);
}

// --------------------------------------------------------  _toastRestoreScratch

void _toastRestoreScratch (struct ToastType *theToast) {
	// Copy pixels from the background bitmap to the scratch bitmap.
	// toast's dirtyRects indicate the bounds that need restoration.
	
	CopyBits (&((GrafPtr) backgroundOffscreen)->portBits,
			&((GrafPtr) scratchOffscreen)->portBits,
			&(theToast->shadowDirtyRect), &(theToast->shadowDirtyRect), srcCopy, NULL);
	
	CopyBits (&((GrafPtr) backgroundOffscreen)->portBits,
			&((GrafPtr) scratchOffscreen)->portBits,
			&(theToast->dirtyRect), &(theToast->dirtyRect), srcCopy, NULL);
}

// --------------------------------------------------------  _playerRestoreScratch

void _playerRestoreScratch (struct PlayerType *thePlayer) {
	// Copy pixels from the background bitmap to the scratch bitmap.
	// thePlayer's dirtyRect indicates the bounds that need restoration.
	CopyBits (&((GrafPtr) backgroundOffscreen)->portBits,
			&((GrafPtr) scratchOffscreen)->portBits,
			&(thePlayer->dirtyRect), &(thePlayer->dirtyRect), srcCopy, NULL);
	
	CopyBits (&((GrafPtr) backgroundOffscreen)->portBits,
			&((GrafPtr) scratchOffscreen)->portBits,
			&(thePlayer->shadowDirtyRect), &(thePlayer->shadowDirtyRect), srcCopy, NULL);
}

// --------------------------------------------------------  _toastInit

void _toastInit (void) {
	short	i;
	
	for (i = 0; i < kMaxToastCount; i++) {
		toast[i].state = TOASTSTATE_INVISIBLE;
	}
}

// --------------------------------------------------------  _debugRestoreMainWindow

void _debugRestoreMainWindow (void) {
	Rect	tempRect;
	short	i;
	
	tempRect = playerOne.collisionRect;
	OffsetRect (&tempRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	CopyBits (&((GrafPtr) scratchOffscreen)->portBits, 
			&(((GrafPtr) mainWindow)->portBits), 
			&(playerOne.collisionRect), &tempRect, srcCopy, 
			(RgnHandle) ((GrafPtr) mainWindow->visRgn));
			
	for (i = 0; i < kMaxToastCount; i++) {
		if (toast[i].state == TOASTSTATE_INVISIBLE) {
			continue;
		}
		tempRect = toast[i].collisionRect;
		OffsetRect (&tempRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
		CopyBits (&((GrafPtr) scratchOffscreen)->portBits, 
				&(((GrafPtr) mainWindow)->portBits), 
				&(toast[i].collisionRect), &tempRect, srcCopy, 
				(RgnHandle) ((GrafPtr) mainWindow->visRgn));
	}
}

// --------------------------------------------------------  _debugDisplayCollisionRects

void _debugDisplayCollisionRects (void) {
	Rect	tempRect;
	short	i;
	
	SetPort (mainWindow);
	
	tempRect = playerOne.collisionRect;
	OffsetRect (&tempRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	FillRect (&tempRect, white);

	for (i = 0; i < kMaxToastCount; i++) {
		if (toast[i].state == TOASTSTATE_INVISIBLE) {
			continue;
		}
		tempRect = toast[i].collisionRect;
		OffsetRect (&tempRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
		FillRect (&tempRect, white);
	}
	
	SetRect (&tempRect, 0, 300, 512, 302);
	OffsetRect (&tempRect, mainDisplayDestBounds.left, mainDisplayDestBounds.top);
	FillRect (&tempRect, white);
}

// ========================================================  Public Functions
// --------------------------------------------------------  beginNewGame

void beginNewGame (void) {
	gameTick = 0L;
	_playerReset ();
	_toastInit ();
	enableSound ();
	
	numToasts = 0;
	numLives = 5;
	score = 0;
	scoreboardInvalid = TRUE;
	toastCounter = _toastCounterReset (score);
	
	playing = TRUE;
	HideCursor ();
	
	// Reset scratch offscreen..
	CopyBits (&((GrafPtr) backgroundOffscreen)->portBits,
			&((GrafPtr) scratchOffscreen)->portBits,
			&backgroundBounds, &backgroundBounds, srcCopy, NULL);
	
	// Refresh main window.
	CopyBits (&((GrafPtr) scratchOffscreen)->portBits, 
			&(((GrafPtr) mainWindow)->portBits), 
			&scratchBounds, &mainDisplayDestBounds, srcCopy, 
			mainDisplayRenderRgn);
	
	_renderScoreboard ();
}

// --------------------------------------------------------  playGame

void playGame (void) {
	long	nextFrameTime;
	
	nextFrameTime = Ticks + 1;
	
	while (playing) {
		gameTick++;
		
		GetKeys (theKeys);
		_checkCommandKeys ();
		
		if (!pausing) {
			short	i;
			
			// BOGUS TEMP
//			_debugRestoreMainWindow ();
			
			_handleToast ();
			_handlePlayer (&playerOne);
			
			// Count down to next toast.
			toastCounter = toastCounter - 1;
			
			_prepareSidewalkCrack ();
			
			// Render the scoreboard if needed.
			if (scoreboardInvalid) {
				_renderScoreboard ();
				scoreboardInvalid = FALSE;
			}
			
			for (i = 0; i < kMaxToastCount; i++) {
				if (toast[i].state == TOASTSTATE_INVISIBLE) {
					continue;
				}
				_copyToastToScratch (&(toast[i]));
			}
			_copyPlayerToScratch (&playerOne);
			
			for (i = 0; i < kMaxToastCount; i++) {
				if (toast[i].state == TOASTSTATE_INVISIBLE) {
					continue;
				}
				_copyToastToMainWindow (&(toast[i]));
			}
			
			_copySidewalkCrackToMainWindow ();
			_copyPlayerToMainWindow (&playerOne);
			
			if ((playerOne.state == PLAYERSTATE_CRASHED) && (numLives == 0)) {
				_copyGameOverToMainWindow ();
			}
			
			// BOGUS: TEMP
//			_debugDisplayCollisionRects ();
			
			for (i = 0; i < kMaxToastCount; i++) {
				if (toast[i].state == TOASTSTATE_INVISIBLE) {
					continue;
				}
				_toastRestoreScratch (&(toast[i]));
			}
			_playerRestoreScratch (&playerOne);
			
			while (Ticks < nextFrameTime) {
			}
			nextFrameTime = Ticks + 1;
		}
	}
	
	_endGame ();
}

// --------------------------------------------------------  gameInit

void gameInit (void) {
	short 	i;
	
	// Set up the toaster source rectangles.
	for (i = 0; i < 4; i++) {
		SetRect (&(toasterSrcRects[i]), 0, 0, 64, 64);
		OffsetRect (&(toasterSrcRects[i]), 0, i * 64);
	}
	
	// Set up the digit source rectangles.
	for (i = 0; i < 10; i++) {
		SetRect (&(digitSrcRects[i]), 0, 0, 21, 22);
		OffsetRect (&(digitSrcRects[i]), 64, i * 22);
	}
	
	// Set up toast source rectangles.
	SetRect (&toastSrcRect, 0, 272, 0 + 64, 272 + 40);
	SetRect (&toastShadowSrcRect, 0, 312, 0 + 64, 312 + 32);
	
	// Set up sidewalk crack bounds.
	SetRect (&sidewalkCrackSrcRect, 0, 344, 0 + 48, 344 + 24);
	_resetSideWalkCrackDestRect ();
	
	// Set up 'Game Over' sprite bounds.
	SetRect (&gameOverSrcRect, 0, 368, 0 + 151, 368 + 43);
	SetRect (&gameOverDestRect, 0, 0, 151, 43);
	OffsetRect (&gameOverDestRect, (kGameWidth - 151) / 2, (kGameHeight - 43) / 3);
	
	numToasts = 0;
	numLives = 5;
	score = 0;
	scoreboardInvalid = TRUE;
	toastCounter = _toastCounterReset (score);
}

// --------------------------------------------------------  shutdownGame

void shutdownGame (void) {
	// Tear everything down.
	killSound ();
	closeMainWindow ();
	FlushEvents (everyEvent, 0);
}