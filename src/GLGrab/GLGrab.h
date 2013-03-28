// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// GLGrab.h

#ifndef _GL_GRAB_H_
#define _GL_GRAB_H_

#define GL_GRAB_VERSION		"0.09" // 2009-12-14

#include "../DEF.h"
#include "../Time/Time.h"

// OpenGL Screen Grabber.
////////////////////////////////////////////////////////////////////////////////
class CGLGrab
{
public:
	CGLGrab();
	~CGLGrab();
public:
	bool Grab();
	bool Save();
	
	void SetFileNamePrefix( const char * pcFileNamePrefix );
	const char * GetFileNamePrefix() const;
	const char * GetFileNameCurr() const;
protected:
	void Clear();
public:
	unsigned char *m_pucData;
	unsigned int m_uiExtX, m_uiExtY;
protected:
	const char * m_pcFileNamePrefix;
	const char * m_pcFileName;
	CTime m_oTime;
};

#endif // _GL_GRAB_H_
