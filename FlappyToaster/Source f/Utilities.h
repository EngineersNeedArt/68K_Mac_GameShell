// ============================================================================
//	Utilities.h
// ============================================================================


// Some standard errors. You can define/add others.
#define	kErrUnnaccounted			1
#define	kErrNeedNewerSystem			2
#define kErrWrongBitDepth			3
#define	kErrNoMemory				4
#define kErrFailedResourceLoad		5
#define kErrFailedFileLoad			6


/// Return a random integer within range.
short randomShort (short range);

/// Get the bounding rectangle for a 'PICT' resources with pictID.
/// theBounds will be a zero-rectangle in case of error.
void getPICTBounds (short pictID, Rect *theBounds);

/// Presents a fatal error alert and then quits our app straightaway.
void fatalError (short errorNumber);
