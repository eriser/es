// VSync.cpp

#include "VSync.h"

#ifdef WND_USE_VSYNC

#if defined( __WINDOWS__ )

//////////////////////////////////////////////////////////////////////
// CVSync (Windows)
//////////////////////////////////////////////////////////////////////

//function pointer typdefs
typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC) (int);
typedef int (*PFNWGLEXTGETSWAPINTERVALPROC) (void);
 
//declare functions
static PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT = NULL;
static PFNWGLEXTGETSWAPINTERVALPROC wglGetSwapIntervalEXT = NULL;

void CVSync::Init_()
{
	//get extensions of graphics card
	char* acExtensions = (char*)glGetString(GL_EXTENSIONS);
	
	//is WGL_EXT_swap_control in the string? VSync switch possible?
	if( strstr( acExtensions, "WGL_EXT_swap_control" ) )
	{
		//get address's of both functions and save them
		wglSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC)
			wglGetProcAddress("wglSwapIntervalEXT");
		wglGetSwapIntervalEXT = (PFNWGLEXTGETSWAPINTERVALPROC)
			wglGetProcAddress("wglGetSwapIntervalEXT");
	}
}

void CVSync::Enable_()
{
	if( wglSwapIntervalEXT )
	{
		wglSwapIntervalEXT( 1 );
	}
}

void CVSync::Disable_()
{
	if( wglSwapIntervalEXT )
	{
		wglSwapIntervalEXT( 0 );
	}
}

bool CVSync::IsEnabled_()
{
	return ( wglGetSwapIntervalEXT != NULL ) && ( wglGetSwapIntervalEXT() != 0 );
}


#elif __LINUX__

//////////////////////////////////////////////////////////////////////
// CVSync (Linux)
//////////////////////////////////////////////////////////////////////

// todo
#error platform not supported yet

#else
#error platform not supported
#endif // platform

#endif // WND_USE_VSYNC
