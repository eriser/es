// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FSAA.h

#include "FSAA.h"

#if defined( USE_FSAA ) && defined( __WINDOWS__ )

// Quelle: http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=46
//
//	Author: Colt "MainRoach" McAnlis 
//	Date:   4/29/04
//	Desc:   This file contains the context to load a WGL extension from a string
//		As well as collect the sample format available based upon the graphics card.
//

#include <windows.h>
#include <GL/gl.h>
#include <GL/wglext.h>
#include <GL/glext.h>
#include "Wnd.h"

#define WGL_SAMPLE_BUFFERS_ARB	0x2041
#define WGL_SAMPLES_ARB		0x2042

//////////////////////////////////////////////////////////////////////
// CFSAA
//////////////////////////////////////////////////////////////////////

bool CFSAA::m_bIsMultisampleSupported_ = false;
int CFSAA::m_iMultisampleFormat_ = false;

#ifndef FSAA_USE_NO_DEP
// WGLisExtensionSupported: This Is A Form Of The Extension For WGL
bool CFSAA::WGLisExtensionSupported_( const char *acExtension )
{
	const size_t extlen = lstrlen(acExtension);
	const char *supported = NULL;
	
	// Try To Use wglGetExtensionStringARB On Current DC, If Possible
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
	
	if (wglGetExtString)
		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

	// If That Failed, Try Standard Opengl Extensions String
	if (supported == NULL)
		supported = (char*)glGetString(GL_EXTENSIONS);

	// If That Failed Too, Must Be No Extensions Supported
	if (supported == NULL)
		return false;

	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = supported; ; p++)
	{
		// Advance p Up To The Next Possible Match
		p = strstr(p, acExtension);
		
		if (p == NULL)
			return false;															// No Match
		
		// Make Sure That Match Is At The Start Of The String Or That
		// The Previous Char Is A Space, Or Else We Could Accidentally
		// Match "wglFunkywglExtension" With "wglExtension"
		
		// Also, Make Sure That The Following Character Is Space Or NULL
		// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
			return true;															// Match
	}
}
#endif // FSAA_USE_NO_DEP

// InitMultisample: Used To Query The Multisample Frequencies
bool CFSAA::InitMultisample_( CWnd * m_poWnd, HDC hDC, unsigned int uiMaxSamples )
{ 
#ifndef FSAA_USE_NO_DEP
	 // See If The String Exists In WGL!
	if( !WGLisExtensionSupported_("WGL_ARB_multisample") )
	{
		m_bIsMultisampleSupported_ = false;
		return false;
	}
#endif // FSAA_USE_NO_DEP
	
	// Get Our Pixel Format
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");	
	if (!wglChoosePixelFormatARB) 
	{
		m_bIsMultisampleSupported_ = false;
		return false;
	}
	
	int pixelFormat;
	int valid;
	UINT numFormats;
	float fAttributes[] = { 0.0f, 0.0f };
	
	// These Attributes Are The Bits We Want To Test For In Our Sample
	// Everything Is Pretty Standard, The Only One We Want To 
	// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
	// These Two Are Going To Do The Main Testing For Whether Or Not
	// We Support Multisampling On This Hardware.
	
	// rem: Um Abhänigkeiten zu vermeiden, wird bei FSAA_USE_NO_DEP eine lokale Kopie eines 
	//      konstanten statischen Arrays gebildet, da bei direkter Definition eines INITIALISIERTEN
	//      lokalen Arrays, implizit memcpy aufgerufen würde!
#ifdef FSAA_USE_NO_DEP
	static const int aiAttr_[] = 
#else
	int aiAttributes[] = 
#endif // FSAA_USE_NO_DEP
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, m_poWnd->m_uiColorBits,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, m_poWnd->m_uiDepthBits,
		WGL_STENCIL_BITS_ARB, m_poWnd->m_uiStencilBits,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, uiMaxSamples,
		0, 0
	};
	
#ifdef FSAA_USE_NO_DEP
	int aiAttributes[ sizeof(aiAttr_) / sizeof(aiAttr_[0]) ];
	CopyMemory( aiAttributes, aiAttr_, sizeof(aiAttr_) );
#endif // FSAA_USE_NO_DEP
	
	// Test for FSAA_MAX_SAMPLES x FSAA_MAX_SAMPLES, FSAA_MAX_SAMPLES/2 x FSAA_MAX_SAMPLES/2, ... , 2x2
	while( 1 )
	{
		valid = wglChoosePixelFormatARB( hDC, aiAttributes, fAttributes, 1, &pixelFormat, &numFormats );
		
		if (valid && numFormats >= 1)
		{
			m_bIsMultisampleSupported_ = true;
			m_iMultisampleFormat_ = pixelFormat;
			return m_bIsMultisampleSupported_;
		}
		
		if( aiAttributes[19] <= 2 )
			break;
		
		aiAttributes[19] <<= 1;
	}
	 
	return  m_bIsMultisampleSupported_;
}

#endif // defined( USE_FSAA ) && defined( __WINDOWS )
