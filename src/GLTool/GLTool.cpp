// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// GLTool.cpp

#include "GLTool.h"

////////////////////////////////////////////////////////////////////////////////
// CGLTool
////////////////////////////////////////////////////////////////////////////////

void CGLTool::Update()
{
	glGetDoublev( GL_MODELVIEW_MATRIX, m_adModelview );
	glGetDoublev( GL_PROJECTION_MATRIX, m_adProjection );
	glGetIntegerv( GL_VIEWPORT, m_aiViewport );
	
	GLdouble dWinX0, dWinX1, dWinY0, dWinY1, dWinZ;
	gluProject( 0.0, 0.0, 0.0, m_adModelview, m_adProjection, m_aiViewport, &dWinX0, &dWinY0, &dWinZ );
	gluProject( 1.0, 0.0, 0.0, m_adModelview, m_adProjection, m_aiViewport, &dWinX1, &dWinY0, &dWinZ );
	gluProject( 0.0, 1.0, 0.0, m_adModelview, m_adProjection, m_aiViewport, &dWinX0, &dWinY1, &dWinZ );
	
	m_dPixelExtX = 1.0 / ( dWinX1 - dWinX0 );
	m_dPixelExtY = 1.0 / ( dWinY1 - dWinY0 );
	m_iViewportX = m_aiViewport[2] - m_aiViewport[0];
	m_iViewportY = m_aiViewport[3] - m_aiViewport[1];
	m_dPixelExtX_2 = m_dPixelExtX * 0.5;
	m_dPixelExtY_2 = m_dPixelExtY * 0.5;
	
	// ACHTUNG: Neu: -1.
	WndToGLCoord( 0, 0,                           &m_adLeftTop[0], &m_adLeftTop[1], &m_adLeftTop[2] );
	WndToGLCoord( m_iViewportX-1, 0,              &m_adRightTop[0], &m_adRightTop[1], &m_adRightTop[2] );
	WndToGLCoord( m_iViewportX-1, m_iViewportY-1, &m_adRightBottom[0], &m_adRightBottom[1], &m_adRightBottom[2] );
	WndToGLCoord( 0, m_iViewportY-1,              &m_adLeftBottom[0], &m_adLeftBottom[1], &m_adLeftBottom[2] );
}

void CGLTool::WndToGLCoord( GLint iX, GLint iY, GLfloat *pfX, GLfloat *pfY, GLfloat *pfZ ) const
{
	GLdouble dPosX, dPosY, dPosZ;
	GLfloat fWndZ = 0.0f;
	
	const GLint iYTmp = m_aiViewport[3] - iY;
	if( iY >= m_aiViewport[1] || iY < m_aiViewport[3] )
		glReadPixels( iX, iYTmp, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &fWndZ );
	
	gluUnProject(
		GLdouble( iX ), GLdouble( iYTmp ), GLdouble( fWndZ ), 
		m_adModelview, m_adProjection, m_aiViewport, &dPosX, &dPosY, &dPosZ );
	
	if( pfX )
		*pfX = GLfloat( dPosX );
	if( pfY )
		*pfY = GLfloat( dPosY );
	if( pfZ )
		*pfZ = GLfloat( dPosZ );
}

void CGLTool::WndToGLCoord( GLint iX, GLint iY, GLdouble *pdX, GLdouble *pdY, GLdouble *pdZ ) const
{
	GLdouble dPosX, dPosY, dPosZ;
	GLfloat fWndZ = 0.0f;
	
	const GLint iYTmp = m_aiViewport[3] - iY;
	if( iY >= m_aiViewport[1] || iY < m_aiViewport[3] ) // Achtung: Neu.
		glReadPixels( iX, iYTmp, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &fWndZ );
	
	gluUnProject( 
		GLdouble( iX ), GLdouble( iYTmp ), GLdouble( fWndZ ), 
		m_adModelview, m_adProjection, m_aiViewport, &dPosX, &dPosY, &dPosZ );
	
	if( pdX )
		*pdX = GLfloat( dPosX );
	if( pdY )
		*pdY = GLfloat( dPosY );
	if( pdZ )
		*pdZ = GLfloat( dPosZ );
}

void CGLTool::GLCoordToWnd( GLfloat fX, GLfloat fY, GLfloat fZ, GLint *piX, GLint *piY, GLint *piZ ) const
{
	GLdouble dPosX, dPosY, dPosZ;
	gluProject(
		GLdouble( fX ), GLdouble( fY ), GLdouble( fZ ),
		m_adModelview, m_adProjection, m_aiViewport,
		&dPosX, &dPosY, &dPosZ );
	// Runden?
	if( piX )
		*piX = GLint( dPosX );
	if( piY )
		*piY = GLint( dPosY );
	if( piZ )
		*piZ = GLint( dPosZ );
}

void CGLTool::GLCoordToWnd( GLdouble dX, GLdouble dY, GLdouble dZ, GLint *piX, GLint *piY, GLint *piZ ) const
{
	GLdouble dPosX, dPosY, dPosZ;
	gluProject(
		dX, dY, dZ,
		m_adModelview, m_adProjection, m_aiViewport,
		&dPosX, &dPosY, &dPosZ );
	// Runden?
	if( piX )
		*piX = GLint( dPosX );
	if( piY )
		*piY = GLint( dPosY );
	if( piZ )
		*piZ = GLint( dPosZ );
}
