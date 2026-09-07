//============================================================================
//	Utilities.h
//============================================================================


#define	kErrUnnaccounted			1
#define	kErrNeedNewerSystem			2
#define kErrWrongBitDepth			3
#define	kErrNoMemory				4
#define kErrFailedResourceLoad		5
#define kErrFailedFileLoad			6


short randomShort (short range);
void getPICTBounds (short pictID, Rect *theBounds);
void fatalError (short errorNumber);