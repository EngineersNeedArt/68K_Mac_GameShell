// ============================================================================
//	SoundUtils.h
// ============================================================================


#define kMaxSoundPriority			99
#define kNoSoundPlaying				-1

#define kSoundIndexFlap0			0
#define kSoundIndexFlap1			1
#define kSoundIndexCeilingCollide	2
#define kSoundIndexScorePoint		3
#define kSoundIndexToastCollide		4
#define kSoundIndexGroundSkid		5


/// Call to allow sound (create a sound channel).
void enableSound (void);

/// Call to disallow sound (destroys the sound channel).
void disableSound (void);

/// Play a sound at a specific index. If index is out of range, nothing happens.
void playSoundWithIndex (short index);

/// Call once to load up all sounds ('snd ' resources).
void soundInit (void);

/// Call when the game is exiting to free up memory, close the sound channel.
void killSound (void);
