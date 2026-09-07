//============================================================================
//	Render.h
//============================================================================


#define kGameWidth 512
#define kGameHeight 322


extern Rect		backgroundBounds;
extern Rect		scratchBounds;
extern Rect		mainDisplayDestBounds;
extern GrafPtr	backgroundOffscreen;
extern GrafPtr	scratchOffscreen;
extern GrafPtr	spriteOffscreen;
extern GrafPtr	maskOffscreen;


void copyBackgroundToScratch (void);
void copyScratchToMainWindow (void);
void renderInit (void);
