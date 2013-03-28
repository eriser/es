// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// GLTxtPrinter.h

#ifndef _GLTXT_PRINTER_H_
#define _GLTXT_PRINTER_H_

#include "GLTxtDEF.h"
#include "GLTxtFont.h"

// Druckt Text auf den Schirm.
////////////////////////////////////////////////////////////////////////////////
class CGLTxtPrinter
{
public:
	enum EAlignment {
		ALIGN_LEFT,
		ALIGN_RIGHT
	};

public:
	CGLTxtPrinter();
	~CGLTxtPrinter();
	
public:
	void Init( CGLTxtFont *pFont );
	void Clear();
	
public:
	inline void SetPixelWidth( GLfloat fWidth );
	inline void SetPos( GLfloat fX, GLfloat fY );
	inline void SetPos( GLfloat fX, GLfloat fY, GLfloat fZ );
	
	inline CGLTxtFont * GetFont() const;
	
	inline void SetAlignment( EAlignment eAlign );
	inline void SetLineSingle();
	inline void SetLineMulti();
	//inline void SetBlockText( GLfloat fWidth );
	inline void SetLineHeight( GLfloat fHeight ); // Da Höhe dyn. nur in Pixeln bestimmt werden kann.
	
	unsigned int GetLineCount( const char *pcStr ) const;
	GLfloat GetLineHeight() const;
	unsigned int Print( const char *pcStr ) const;
	unsigned int Print( const char cChar ) const;
	//unsigned int PrintFormat( const char *pcStr, ... ) const;

public:
	CGLTxtFont *m_poFont;
	
protected:
	GLfloat m_fX;
	GLfloat m_fY;
	GLfloat m_fZ;
	GLfloat m_fLineHeight;
	GLfloat m_fPixelWidth;
	EAlignment m_eAlign;
	bool m_bLineSingle;
};


#include "GLTxtPrinter.inline.h"

#endif // _GLTXT_PRINTER_H_
