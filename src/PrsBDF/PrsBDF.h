// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// PrsBDF.h
//
// TODO: 
// - Speed!
// - Error Checking.
// - Auflösen von Abhängigkeiten. 
//

#ifndef _PRS_BDF_H_
#define _PRS_BDF_H_

#define PBDF_VERSION	"0.03"
#define PBDF_DEBUG
//#define PBDF_USE_LOG
#define PBDF_USE_ERROR

#define PBDF_NEWLINE	"\n"	// ACHTUNG... mac/win/linux...

#ifdef PBDF_USE_LOG
#include <iostream>
#define			PBDF_LOG(...)	LOG( __VA_ARGS__ )
#else
#define			PBDF_LOG(...)
#endif

#ifdef PBDF_USE_ERROR
#include <stdio.h>
#define			PBDF_ERROR(...)	fprintf( stderr, "PBDF_ERROR: " __VA_ARGS__ )
#else
#define			PBDF_ERROR(...)
#endif // PBDF_USE_ERROR

#include "../Str/Str.h"
#include "../Array/Array.h"

// Glyph Bitmap Distribution Format Parser.
// http://partners.adobe.com/public/developer/en/font/5005.BDF_Spec.pdf
////////////////////////////////////////////////////////////////////////////////
class CPrsBDF
{
public:
	// Glyph.
	////////////////////////////////////////////////////////////////////////////////
	class CGlyph
	{
	public:
		CGlyph();
		CGlyph( const CGlyph & roO );
		~CGlyph();
		CGlyph & operator=( const CGlyph & roO );
		void Reset();
	public:
		CStr m_oStrName;
		int m_iEncoding0, m_iEncoding1;
		double m_dSX, m_dSY;
		int m_iDX, m_iDY;
		double m_dSX1, m_dSY1;
		int m_iDX1, m_iDY1;
		double m_dVVX, m_dVVY;
		int m_iBBX, m_iBBY;
		int m_iBBOffX, m_iBBOffY;
		CArray<unsigned char> m_oArrBitmap;
	};
	
public:
	CPrsBDF();
	~CPrsBDF();
	
	bool Parse( const char *pcFont );
	
public:
	double m_dVersion;
	int m_iContentVersion;
	CStr m_oStrFont;
	int m_iPointSize;
	int m_iResX, m_iResY;
	int m_iBBX, m_iBBY;
	int m_iOffX, m_iOffY;
	int m_iMetricsSet;
	CGlyph m_oGlyphDefault;
	int m_iProperties;
	CStr m_oStrProperties;
	int m_iChars;
	
	CArray<CGlyph> m_oArrGlyph;
	
protected:
	void Reset();
	
	bool RemoveNextWord( CStr &roStr );
	bool RemoveLeadingSpace( CStr &roStr );
	
	void ReadValue( CStr &roStr, double *pdNumber );
	void ReadValue( CStr &roStr, int *piInteger );
	void ReadValue( CStr &roStr, CStr *poString );
	void ReadValue( CStr &roStr, CArray<unsigned char> *poArrByte );
	
protected:
	
	enum EToken {
		PBDF_TOKEN_NONE,
		PBDF_TOKEN_STARTFONT,
		PBDF_TOKEN_COMMENT,
		PBDF_TOKEN_CONTENTVERSION,
		PBDF_TOKEN_FONT,
		PBDF_TOKEN_SIZE,
		PBDF_TOKEN_FONTBOUNDINGBOX,
		PBDF_TOKEN_METRICSSET,
		PBDF_TOKEN_SWIDTH,
		PBDF_TOKEN_DWIDTH,
		PBDF_TOKEN_SWIDTH1,
		PBDF_TOKEN_DWIDTH1,
		PBDF_TOKEN_VVECTOR,
		PBDF_TOKEN_STARTPROPERTIES,
		PBDF_TOKEN_ENDPROPERTIES,
		PBDF_TOKEN_CHARS,
		PBDF_TOKEN_STARTCHAR,
		PBDF_TOKEN_ENCODING,
		PBDF_TOKEN_BBX,
		PBDF_TOKEN_BITMAP,
		PBDF_TOKEN_ENDCHAR,
		PBDF_TOKEN_ENDFONT
	};
	
	enum EType {
		PBDF_TYPE_INTEGER,
		PBDF_TYPE_NUMBER,
		PBDF_TYPE_STRING
	};
	
	// Pair.
	////////////////////////////////////////////////////////////////////////////////
	template <class X, class Y>
	class CPair
	{
	public:
		CPair( X tX, Y tY )
		: m_tX( tX )
		, m_tY( tY )
		{}
		X GetFirst() const { return m_tX; }
		Y GetSecond() const { return m_tY; }
	protected:
		X m_tX;
		Y m_tY;
	};
	typedef CPair<CStr, unsigned int> TPairTokenNameID;
};

#endif // _PRS_BDF_H_
