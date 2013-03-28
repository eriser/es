// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// GLTool.h

#ifndef _GL_TOOL_H_
#define _GL_TOOL_H_

#define GL_TOOL_VERSION		"0.05"

#include "../DEF.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

// Hilfsfunktionen für OpenGL-Programme.
////////////////////////////////////////////////////////////////////////////////
class CGLTool
{
public:
	void Update();
	
	// Fenster-Koordinaten in OpenGL-Koordinaten umwandeln.
	void WndToGLCoord( GLint iX, GLint iY, GLfloat *pfX, GLfloat *pfY, GLfloat *pfZ ) const;
	void WndToGLCoord( GLint iX, GLint iY, GLdouble *pdX, GLdouble *pdY, GLdouble *pdZ ) const;
	
	// OpenGL-Koordinaten in Fenster-Koordinaten umwandeln.
	void GLCoordToWnd( GLfloat fX, GLfloat fY, GLfloat fZ, GLint *piX, GLint *piY, GLint *piZ ) const;
	void GLCoordToWnd( GLdouble dX, GLdouble dY, GLdouble dZ, GLint *piX, GLint *piY, GLint *piZ ) const;

	// Koordinaten-Wert auf Pixel runden.
	inline GLfloat RoundToPixelX( GLfloat fValue );
	inline GLfloat RoundToPixelY( GLfloat fValue );
	inline GLdouble RoundToPixelX( GLdouble dValue );
	inline GLdouble RoundToPixelY( GLdouble dValue );
	
public:
	// Breite/Höhe von einem Pixel in OpenGL-Koordinaten,
	// ohne direkt zu "wissen" wie gross das Fenster ist.
	GLdouble m_dPixelExtX, m_dPixelExtY;
	GLdouble m_dPixelExtX_2, m_dPixelExtY_2;
	// Viewport.
	GLint m_iViewportX, m_iViewportY;
	// Minimale und Maximale Werte des Viewports in OpenGL-Koordinaten.
	GLdouble m_adLeftBottom[3];
	GLdouble m_adRightBottom[3];
	GLdouble m_adRightTop[3];
	GLdouble m_adLeftTop[3];

protected:
	GLint m_aiViewport[4];
	GLdouble m_adModelview[16];
	GLdouble m_adProjection[16];
};

#include "GLTool.inline.h"

#endif // _GL_TOOL_H_
