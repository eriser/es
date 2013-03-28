// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// GLTxtPrinter.cpp

#include "GLTxtPrinter.h"

#if defined( __WINDOWS__ ) && defined( __GNUC__ )
#	define vsnprintf _vsnprintf
#endif


////////////////////////////////////////////////////////////////////////////////
// CGLTxtPrinter
////////////////////////////////////////////////////////////////////////////////

CGLTxtPrinter::CGLTxtPrinter()
: m_poFont( 0 )
, m_fX( 0.0f )
, m_fY( 0.0f )
, m_fZ( 0.0f )
, m_fLineHeight( 0.1f )
, m_fPixelWidth( 0.0f )
, m_eAlign( ALIGN_LEFT )
, m_bLineSingle( false )
{}

CGLTxtPrinter::~CGLTxtPrinter()
{
	Clear();
}

void CGLTxtPrinter::Init( CGLTxtFont *pFont )
{
	m_poFont = pFont;
}

void CGLTxtPrinter::Clear()
{}

unsigned int CGLTxtPrinter::GetLineCount( const char *pcStr ) const
{
	if( m_poFont )
	{
		// SingleLine.
		if( m_bLineSingle )
		{
			return 1;
		}
		// MultiLine.
		else
		{
			const char *pcFound = pcStr;
			unsigned int uiLines = 0;
			do
			{
				pcFound = strchr( pcFound + 1, '\n' );
				++uiLines;
			}
			while( pcFound );
			return uiLines;
		}
	}
	return 0;
}

unsigned int CGLTxtPrinter::Print( const char cChar ) const
{
	static char acTmp[] = { 0, 0 };
	acTmp[0] = cChar;
	return Print( acTmp );
}

unsigned int CGLTxtPrinter::Print( const char *pcStr ) const
{
	unsigned int uiLines = 0;
	
	//glPushAttrib( GL_ALL_ATTRIB_BITS );
	//glPushAttrib( GL_CURRENT_BIT | GL_LIST_BIT );
	
	glListBase( m_poFont->m_uiLstBase );
	
	if( m_poFont )
	{
		GLfloat fX = m_fX, fY = m_fY, fZ = m_fZ;
		
		// SingleLine.
		if( m_bLineSingle )
		{
			const unsigned int uiLength = (unsigned int)( GLTXT_STRLEN( pcStr ) );
			if( m_eAlign == ALIGN_RIGHT )
				fX = m_fX - m_poFont->GetWidth( pcStr, uiLength ) * m_fPixelWidth;

			glRasterPos3f( fX, fY, fZ );
			glCallLists( uiLength, GL_UNSIGNED_BYTE, pcStr );
			uiLines = 1;
		}
		// MultiLine.
		else
		{
 			const char *pcFirst = pcStr;
			const char *pcFound = pcStr;

			do
			{
				unsigned int uiLength;

				pcFound = strchr( pcFirst, '\n' );
				if( pcFound )
					uiLength = (unsigned int)(pcFound - pcFirst);
				else
					uiLength = (unsigned int)( GLTXT_STRLEN( pcFirst ) );

				if( m_eAlign == ALIGN_RIGHT )
					fX = m_fX - m_poFont->GetWidth( pcFirst, uiLength ) * m_fPixelWidth;

				glRasterPos3f( fX, fY, fZ );
				glCallLists( uiLength, GL_UNSIGNED_BYTE, pcFirst );
				fY -= m_fLineHeight;
				
				pcFirst = pcFound + 1;
				
				++uiLines;
			}
			while( pcFound );
		}
	}
	
	//glPopAttrib();
	
	return uiLines;
}

/*
unsigned int CGLTxtPrinter::PrintFormat( const char *acFormat, ... ) const
{
	const int iFormatStrLen = int( GLTXT_STRLEN( acFormat ) );
	int iSize = iFormatStrLen * 3; // 3 mal die Länge des Meta-Strings. rem: Schätzung!!!
	int iWritten = 0;
	char *acText = new char[ iSize ];
	va_list args; // Pointer auf List Of Arguments.

	if( !iFormatStrLen )
		return 0;

	// ACHTUNG: Wirklich wüste Art einen Buffer Overrun zu verhindern! (s.u.)
	va_start( args, acFormat ); // Parses The String For Variables

LWrite:
#ifdef _MSC_VER
	iWritten = vsnprintf_s( acText, iSize, iSize, acFormat, args ); // Konvertiren des Meta-Strings in String.
#else
	iWritten = vsnprintf( acText, iSize, acFormat, args ); // Konvertiren des Meta-Strings in String.
#endif // _MSC_VER

	if( iWritten < 0 || iWritten == iSize ) // ...falls der geschrieben String nicht in acText passt, oder kein Platz mehr bleibt für Nullterminator
	{
		if( acText )
			delete [] acText;
		iSize += iSize; // doppelt soviel Platz reservieren, und probieren...
		acText = new char[iSize];
		goto LWrite;
	}
	acText[iWritten] = '\0';
	va_end( args ); // Reslultat gespeichert in acText.

	const unsigned int uiLines = Print( acText );

	if( acText )
		delete []  acText;

	return uiLines;
}
*/
