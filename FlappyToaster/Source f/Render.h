// ===========================================================================
//	Render.h
// ===========================================================================


#define kGameWidth		512
#define kGameHeight		322


extern Rect			backgroundBounds;
extern Rect			scratchBounds;
extern Rect			mainDisplayDestBounds;
extern GrafPtr		backgroundOffscreen;
extern GrafPtr		scratchOffscreen;
extern GrafPtr		spriteOffscreen;
extern GrafPtr		maskOffscreen;
extern RgnHandle	mainDisplayRenderRgn;


/// Refreshes the entire 'scratch' offscreen bitmap by copying pixels from the 
/// background offscreen bitmap.
void copyBackgroundToScratch (void);

/// Redraws the entire main window by copying the pixels from the 'scratch' 
/// offscreen bitmap.
void copyScratchToMainWindow (void);

/// Set up all the offscreen bitmaps, etc.
Boolean renderInit (void);
