// VSync.h

#ifndef _VSYNC_H_
#define _VSYNC_H_

#include "WndDEF.h"

#ifdef WND_USE_VSYNC

// VSync.
////////////////////////////////////////////////////////////////////////////////
class CVSync
{
public:
	static void Init_();
	static void Enable_();
	static void Disable_();
	static bool IsEnabled_();
};

#endif // USE_VSYNC
#endif // _VSYNC_H_
