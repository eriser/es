// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// GLTxtDEF.h

#ifndef _GL_TXT_DEF_H_
#define _GL_TXT_DEF_H_

#define GLTXT_TEXT_VERSION 		"0.13" // 2009-02-09

//#define GLTXT_USE_FONT_BDF		// enable BDF fonts
//#define GLTXT_USE_FONT_BDF_INCLUDE	// use included font, not a file

//#define GLTXT_USE_LOG
//#define GLTXT_USE_ERROR

#include "../DEF.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
#include <stdarg.h>


#if defined( GLTXT_USE_LOG )
#define GLTXT_LOG(...)			LOG( __VA_ARGS__ )
#else
#define GLTXT_LOG(...)
#endif // GL_TXT_USE_LOG

#if defined( GLTXT_USE_LOG )
#include <stdio.h>
#define GLTXT_ERROR(...)		fprintf( stderr, "GLTXT_ERROR: " __VA_ARGS__ )
#else
#define GLTXT_ERROR(...)
#endif // GLTXT_USE_ERROR


#if defined( GLTXT_USE_FONT_BDF )

#include "../PrsBDF/PrsBDF.h"

#endif // GLTXT_USE_FONT_BDF


#if defined( __WINDOWS__ )


#define	WIN32_LEAN_AND_MEAN
#include <windows.h>

#elif defined( __LINUX__ )

#include <stdlib.h> // atoi
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/Xatom.h>

#endif // platform

#if defined( __WINDOWS__ )
#define GLTXT_STRLEN(s)			lstrlen( s )
#else
#define GLTXT_STRLEN(s)			strlen( s )
#endif // __WINDOWS__

#endif // _GL_TXT_DEF_H_
