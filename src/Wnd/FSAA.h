// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FSAA.h

#ifndef _FSAA_H_
#define _FSAA_H_

#include "WndDEF.h"

#if defined( USE_FSAA ) && defined( __WINDOWS__ )

#define FSAA_VERSION		0.05 // 2010-04-15
#define FSAA_USE_NO_DEP		// ACHTUNG: Extension-Support wird nicht mehr geprüft (strstr)!

class CWnd;

// Fullscreen Antialiasing
////////////////////////////////////////////////////////////////////////////////
class CFSAA
{
public:
	static bool InitMultisample_( CWnd * m_poWnd, HDC hDC, unsigned int uiMaxSamples=16 );
	
protected:
#ifndef FSAA_USE_NO_DEP
	static bool WGLisExtensionSupported_( const char *acExtension );
#endif // FSAA_USE_NO_DEP
	
public:
	static bool	m_bIsMultisampleSupported_;
	static int	m_iMultisampleFormat_;
};

#endif // defined( USE_FSAA ) && defined( __WINDOWS__ )
#endif // _FSAA_H_
