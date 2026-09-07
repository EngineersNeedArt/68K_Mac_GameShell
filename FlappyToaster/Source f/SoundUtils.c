// ===========================================================================
//	SoundUtils.c
// ===========================================================================


#include "SoundUtils.h"
#include "Utilities.h"
#include <Sound.h>


#define kNumSoundChannels	4

Handle			*theSounds;							// Array of handles.
SndChannelPtr	channels[kNumSoundChannels];
short			channelPriority[kNumSoundChannels];
short			numberOfSounds;
Boolean			soundReady;
Boolean			soundOn = TRUE;


extern pascal long SetUpA4 (void);
extern pascal void RestoreA4 (long oldA4);


void _loadAllSounds (void);

// ========================================================  Provate Functions
// --------------------------------------------------------  _soundChannelCallback

pascal void _soundChannelCallback (SndChannelPtr chan, SndCommand cmd) {
	long	oldA5;
	short	index = cmd.param2;
	
	// Set A5 to point to our globals.
	oldA5 = SetA5 (chan->userInfo);
	
	// Channel is free.
	channelPriority[index] = kNoSoundPlaying;
	
	// Restore A5.
	SetA5 (oldA5);
}

// --------------------------------------------------------  _availableSoundChannel
/// Returns -1 if there is no available sound channel with a lower priotiy.

short _availableSoundChannel (short priority) {
	short	i;
	short	lowestPrioity = kMaxSoundPriority;
	short	lowestIndex = -1;
	
	// Find lowest priority.
	for (i = 0; i < kNumSoundChannels; i++) {
		if (channelPriority[i] < lowestPrioity) {
			lowestPrioity = channelPriority[i];
			lowestIndex = i;
			
			// If there is no sound playing on the channel, we're done.
			if (lowestPrioity == kNoSoundPlaying) {
				break;
			}
		}
	}
	
	if (priority < lowestPrioity) {
		lowestIndex = -1;
	}
	
	return lowestIndex;
}

// --------------------------------------------------------  _loadAllSounds

void _loadAllSounds (void) {
	short 		i;
	
	// Get the number of sound ()'snd ') resources.
	numberOfSounds = Count1Resources ('snd ');
	
	// Allocate an array of handles to hold the sounds.
	theSounds = (Handle *) NewPtrClear (sizeof (Handle) * numberOfSounds);
	if ((theSounds == NULL) || (MemError () != noErr)) {
		fatalError (kErrNoMemory);
		goto bail;		
	}
	
	// Load the sounds.
	// We assume 'snd ' resource IDs begin at 128 and are contiguous to 128 + n
	// (where n = number of 'snd ' resources).
	for (i = 0; i < numberOfSounds; i++) {
		theSounds[i] = GetResource ('snd ', i + 128);
		if (theSounds[i]) {
			DetachResource (theSounds[i]);
			HNoPurge (theSounds[i]);
		}
	}
	
bail:
	
	return;
}

// ========================================================  Public Functions
// --------------------------------------------------------  enableSound

void enableSound (void) {
	short	i;
	OSErr	theErr;
	
	if (soundReady) {
		return;
	}
	
	for (i = 0; i < kNumSoundChannels; i++) {
		theErr = SndNewChannel (&(channels[i]), 0, initMono + initNoInterp, _soundChannelCallback);
		if (theErr != noErr) {
			channels[i] = NULL;
			return;
		}
		channels[i]->userInfo = SetCurrentA5 ();
	}
	
	soundReady = TRUE;
}

// --------------------------------------------------------  disableSound

void disableSound (void) {
	short	i;
	OSErr	theErr;
	
	if (!soundReady)
		return;
	
	for (i = 0; i < kNumSoundChannels; i++) {
		theErr = SndDisposeChannel (channels[i], TRUE);
		channels[i] = NULL;
		channelPriority[i] = kNoSoundPlaying;
	}
	
	soundReady = FALSE;
}

// --------------------------------------------------------  playSoundWithIndex

void playSoundWithIndex (short index) {
	OSErr		theErr;
	SndCommand	command;
	short		channelIndex = 0;
	
	if ((!soundReady) || (!soundOn) || (index < 0) || (index >= numberOfSounds)) {
		return;
	}
	
	if (theSounds[index] == NULL) {
		return;
	}
	
	// Find an open saound channel.
	// index also acts as 'priority' so that if all channels are busy, a channel 
	// with the lowest priority (index) is returned and will be overwritten.
	channelIndex = _availableSoundChannel (index);
	if (channelIndex < 0) {
		return;
	}
	
	// Kill any sound playing on channel.
	command.cmd = quietCmd;
	command.param1 = 0;
	command.param2 = quietCmd;
	theErr = SndDoImmediate (channels[channelIndex], &command);
	
	// Play sound.
	theErr = SndPlay (channels[channelIndex], theSounds[index], TRUE);
	
	// Enqueue a callback.
	command.cmd = callBackCmd;
	command.param1 = 0;
	command.param2 = (long) channelIndex;
	theErr = SndDoCommand (channels[channelIndex], &command, FALSE);
	
	// Indicate 'priority' of sound on channel.
	channelPriority[channelIndex] = index;
}

// --------------------------------------------------------  soundInit

void soundInit (void) {
	short	i;
	
	soundReady = FALSE;
	for (i = 0; i < kNumSoundChannels; i++) {
		channels[i] = NULL;
		channelPriority[i] = kNoSoundPlaying;
	}
	numberOfSounds = 0;
	
	_loadAllSounds ();
}

// --------------------------------------------------------  killSound

void killSound (void) {
	short		i;
	
	// Turn off sound.
	disableSound ();
	
	// Release sound resources.
	for (i = 0; i < numberOfSounds; i++) {
    	if (theSounds[i] != NULL) {
        	DisposeHandle (theSounds[i]);
        	theSounds[i] = NULL;
    	}
	}
	
	// Release storage for sounds, zero numberOfSounds
	DisposePtr ((Ptr) theSounds);
	theSounds = NULL;
	numberOfSounds = 0;
}
