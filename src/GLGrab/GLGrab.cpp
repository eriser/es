// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// GLGrab.cpp

#include "GLGrab.h"
#include "../File/FileBlockImgBmp.h"
#include <GL/gl.h>

#ifdef _MSC_VER
#define GG_SPRINTF(dst,bytes,format,...) sprintf_s(dst,bytes,format,__VA_ARGS__)
#else
#define GG_SPRINTF(dst,bytes,format,...) sprintf(dst,format,__VA_ARGS__)
#endif // _MSC_VER

////////////////////////////////////////////////////////////////////////////////
// CGLGrab
////////////////////////////////////////////////////////////////////////////////

CGLGrab::CGLGrab()
: m_pucData( 0 )
, m_uiExtX( 0 )
, m_uiExtY( 0 )
{}

CGLGrab::~CGLGrab() {
	Clear();
}

void CGLGrab::SetFileNamePrefix( const char * pcFileNamePrefix )
{
	m_pcFileNamePrefix = pcFileNamePrefix;
}

const char * CGLGrab::GetFileNamePrefix() const {
	return m_pcFileNamePrefix;
}

const char * CGLGrab::GetFileNameCurr() const {
	return m_pcFileName;
}

bool CGLGrab::Grab()
{
	GLint aiViewport[4];
	glGetIntegerv( GL_VIEWPORT, aiViewport );

	Clear();

	m_uiExtX = aiViewport[2];
	m_uiExtY = aiViewport[3];
	m_pucData = new unsigned char[ m_uiExtX * m_uiExtY * 3 ];

	glPushAttrib( GL_PIXEL_MODE_BIT );

	glFinish();
	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, 0 );
	glPixelStorei( GL_PACK_SKIP_ROWS, 0 );
	glPixelStorei( GL_PACK_SKIP_PIXELS, 0 );

	glReadPixels( aiViewport[0], aiViewport[1], m_uiExtX, m_uiExtY, GL_RGB, GL_UNSIGNED_BYTE, m_pucData );

	glPopAttrib();

	return true;
}

bool CGLGrab::Save()
{
	char acTmp[256];
	unsigned int uiNum = 0;
	unsigned int j = 0;
	j  = GG_SPRINTF( acTmp, 256, "%s", m_pcFileNamePrefix );
	m_oTime.Update();
	j += GG_SPRINTF( acTmp + j, 256 - j, "_%s_000.bmp", m_oTime.GetString() );
	while( CFile::Exist_( acTmp ) )
	{
		++uiNum;
		if( uiNum == 1000 )
			return false;
		GG_SPRINTF( acTmp + j - 7, 256 - j, "%.3d.bmp", uiNum );
	}
	CFileBlockImgBmp oFileBmp;
	oFileBmp.m_uiWidth = m_uiExtX;
	oFileBmp.m_uiHeight = m_uiExtY;
	oFileBmp.Encode( m_pucData, m_uiExtX * m_uiExtY * 3 );
	return oFileBmp.Save( acTmp );
}

void CGLGrab::Clear()
{
	if( m_pucData )
		delete [] m_pucData;
	m_pucData = 0;
	m_uiExtX = m_uiExtY = 0;
}
