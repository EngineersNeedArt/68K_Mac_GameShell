// ===========================================================================
//	PixelUtils.h
// ===========================================================================


/// Creates a B&W offscreen bitmap of the specified size. Returns NULL on failure.
GrafPtr createBWOffScreenBitMap (Rect *theRect);

/// Draw a picture to the current port with resourceID. Returns TRUE on success.
Boolean drawPictureWithID (short resourceID);
