//============================================================================
//	SoundUtils.c
//============================================================================


#include "SoundUtils.h"
#include <Sound.h>


#define kMaxSounds				16


Handle			theSounds[kMaxSounds];
SndChannelPtr	theChannel;
short			numberOfSoundsLoaded;
Boolean			soundLoaded[kMaxSounds];
Boolean			soundReady;
Boolean			soundOn = TRUE;


void loadAllSounds (void);


//========================================================  Functions
//--------------------------------------------------------  enableSound

void enableSound (void) {
	OSErr		theErr;
	
	if (soundReady) {
		return;
	}
	
	theErr = SndNewChannel(&theChannel, 0, initMono + initNoInterp, NULL);
	if (theErr != noErr) {
		theChannel = NULL;
		return;
	}
	
	soundReady = TRUE;
}

//--------------------------------------------------------  disableSound

void disableSound (void) {
	OSErr		theErr;
	
	if (!soundReady)
		return;
	
	theErr = SndDisposeChannel(theChannel, TRUE);
	theChannel = NULL;
	
	soundReady = FALSE;
}

//--------------------------------------------------------  playSoundWithIndex

void playSoundWithIndex (short index) {
	OSErr	theErr;
	
	if ((!soundReady) || (!soundOn) || (index < 0) || (index >= kMaxSounds)) {
		return;
	}
	
	if (!soundLoaded[index]) {
		return;
	}
	
	theErr = SndPlay(theChannel, theSounds[index], TRUE);
}

//--------------------------------------------------------  loadAllSounds

void loadAllSounds (void) {
	Handle		theSound;
	short		numberOfSounds, i;
	
	numberOfSounds = Count1Resources('snd ');
	for (i = 0; i < numberOfSounds; i++) {
		theSounds[i] = Get1IndResource('snd ', i + 1);
		if (theSounds[i]) {
			DetachResource(theSounds[i]);
			HNoPurge(theSounds[i]);
			soundLoaded[i] = TRUE;
			numberOfSoundsLoaded++;
		}
	}
}

//--------------------------------------------------------  soundInit

void soundInit (void) {
	short		i;
	
	soundReady = FALSE;
	theChannel = NULL;
	
	numberOfSoundsLoaded = 0;
	for (i = 0; i < kMaxSounds; i++) {
		soundLoaded[i] = FALSE;
		theSounds[i] = NULL;
	}
	
	loadAllSounds();
}

//--------------------------------------------------------  killSound

void killSound (void) {
	short		i;
	
	disableSound();
	
	for (i = 0; i < kMaxSounds; i++) {
		soundLoaded[i] = FALSE;
		if (theSounds[i]) {
			DisposHandle(theSounds[i]);
			theSounds[i] = NULL;
		}
	}
	
	numberOfSoundsLoaded = 0;
}

