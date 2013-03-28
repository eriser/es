// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// GLTxtPrinter.inline.h

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// CGLTxtPrinter
////////////////////////////////////////////////////////////////////////////////

inline void CGLTxtPrinter::SetPixelWidth( GLfloat fWidth ) {
	m_fPixelWidth = fWidth;
}

inline void CGLTxtPrinter::SetPos( GLfloat fX, GLfloat fY )
{
	m_fX = fX;
	m_fY = fY;
	m_fZ = 0.0f;
}

inline void CGLTxtPrinter::SetPos( GLfloat fX, GLfloat fY, GLfloat fZ )
{
	m_fX = fX;
	m_fY = fY;
	m_fZ = fZ;
}

inline CGLTxtFont * CGLTxtPrinter::GetFont() const {
	return m_poFont;
}

inline void CGLTxtPrinter::SetAlignment( CGLTxtPrinter::EAlignment eAlign ) {
	m_eAlign = eAlign;
}

inline void CGLTxtPrinter::SetLineSingle() {
	m_bLineSingle = true;
}

inline void CGLTxtPrinter::SetLineMulti() {
	m_bLineSingle = false;
}

inline void CGLTxtPrinter::SetLineHeight( GLfloat fHeight ) {
	m_fLineHeight = fHeight;
}

inline GLfloat CGLTxtPrinter::GetLineHeight() const {
	return m_fLineHeight;
}
