// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// PrsBDF.cpp

#include "PrsBDF.h"
#include <stdlib.h> // TODO: Entfernen, keine Abhängigkeit von anderen Libs!

/*
#ifdef PBDF_DEBUG
#include <iostream>
static void DEBUG_PrintLargeString_( CStr &roStr )
{
	const unsigned int uiChunkSize = 256;
	const unsigned int uiStringSize = roStr.GetSize();
	const unsigned int uiChunkCount = uiStringSize / uiChunkSize;
	unsigned int uiPos = 0;
	
	for( unsigned int uiChunk=0; uiChunk<uiChunkCount; uiChunk++, uiPos += uiChunkSize )
	{
		std::cout << roStr.GetSub( uiPos, uiChunkSize ).GetData();
	}
	std::cout << roStr.GetSub( uiPos, uiStringSize - uiChunkCount * uiChunkSize ).GetData();
	std::cout << std::endl << std::flush;
}
#endif // PBDF_DEBUG
*/

////////////////////////////////////////////////////////////////////////////////
// CPrsBDF::CGlyph
////////////////////////////////////////////////////////////////////////////////

CPrsBDF::CGlyph::CGlyph()
{
	Reset();
}

CPrsBDF::CGlyph::CGlyph( const CPrsBDF::CGlyph &roO )
{
	*this = roO;
}

CPrsBDF::CGlyph::~CGlyph()
{}
	
CPrsBDF::CGlyph & CPrsBDF::CGlyph::operator=( const CPrsBDF::CGlyph &roO )
{
	m_oStrName = roO.m_oStrName;
	m_iEncoding0 = roO.m_iEncoding0, 
	m_iEncoding1 = roO.m_iEncoding1;
	m_dSX = roO.m_dSX;
	m_dSY = roO.m_dSY;
	m_iDX = roO.m_iDX;
	m_iDY = roO.m_iDY;
	m_dSX1 = roO.m_dSX1;
	m_dSY1 = roO.m_dSY1;
	m_iDX1 = roO.m_iDX1;
	m_iDY1 = roO.m_iDY1;
	m_dVVX = roO.m_dVVX;
	m_dVVY = roO.m_dVVY;
	m_iBBX = roO.m_iBBX;
	m_iBBY = roO.m_iBBY;
	m_iBBOffX = roO.m_iBBOffX;
	m_iBBOffY = roO.m_iBBOffY;
	m_oArrBitmap = roO.m_oArrBitmap;
	return *this;
}

void CPrsBDF::CGlyph::Reset()
{
	m_oStrName.Clear();
	m_iEncoding0 = -1;
	m_iEncoding1 = -1;
	m_dSX = m_dSY = 0.0;
	m_iDX = m_iDY = 0;
	m_dSX1 = m_dSY1 = 0.0;
	m_iDX1 = m_iDY1 = 0;
	m_dVVX = m_dVVY = 0.0;
	m_iBBX = m_iBBY = 0;
	m_iBBOffX = m_iBBOffY = 0;
	m_oArrBitmap.Clear();
}

////////////////////////////////////////////////////////////////////////////////
// CPrsBDF
////////////////////////////////////////////////////////////////////////////////


CPrsBDF::CPrsBDF()
{}

CPrsBDF::~CPrsBDF()
{}

bool CPrsBDF::Parse( const char *pcFont )
{
#define TOK(T) TPairTokenNameID( #T, PBDF_TOKEN_##T )
	static const TPairTokenNameID m_aoToken_[] = 
	{
		TOK( STARTFONT ),
		TOK( COMMENT ),
		TOK( CONTENTVERSION ),
		TOK( FONT ),
		TOK( SIZE ),
		TOK( FONTBOUNDINGBOX ),
		TOK( METRICSSET ),
		TOK( SWIDTH ),
		TOK( DWIDTH ),
		TOK( SWIDTH1 ),
		TOK( DWIDTH1 ),
		TOK( VVECTOR ),
		TOK( STARTPROPERTIES ),
		TOK( ENDPROPERTIES ),
		TOK( CHARS ),
		TOK( STARTCHAR ),
		TOK( ENCODING ),
		TOK( BBX ),
		TOK( BITMAP ),
		TOK( ENDCHAR ),
		TOK( ENDFONT )
	};
	static const unsigned int uiTokenNum = sizeof( m_aoToken_ ) / sizeof( TPairTokenNameID );
	CStr oStrFontFile( pcFont );
	const int iFontSize = oStrFontFile.GetSize();
	int iPropertiesStartPos = -1;
	CGlyph oGylphNew;
	CGlyph *poGlyph = &m_oGlyphDefault;
	bool bIsReadingChar = false;
	
	Reset();
	
	PBDF_LOG( "parse: start\n" );
	
	int iPos = 0;
	while( iPos < iFontSize )
	{
		int iEndOfLine = oStrFontFile.Find( iPos, PBDF_NEWLINE );
		if( iEndOfLine < 0 )
			iEndOfLine = iFontSize;
		
		CStr oStrLine = oStrFontFile.GetSub( iPos, iEndOfLine - iPos );
		RemoveLeadingSpace( oStrLine );
		
		//PBDF_LOG( "line: %s", oStrLine.GetData() );
		//PBDF_LOG( "line_size: %s", oStrLine.GetSize() );
		
		unsigned int uiTokenNext = PBDF_TOKEN_NONE;
		
		for( unsigned int t=0; t<uiTokenNum; ++t )
		{
			CStr oStrToken = m_aoToken_[t].GetFirst() ;
			const unsigned int uiTokenSize = oStrToken.GetSize();
			
			//PBDF_LOG( "token: %s", oStrToken.GetData() );
			
			int iPosToken = oStrLine.Find( 0, oStrToken );
			if( iPosToken != 0 ) 
				continue;
			
			// Danach müssen Leerzeichen oder das Ende folgen!
			const unsigned int uiPosNext = iPosToken + uiTokenSize;
			if( uiPosNext < oStrLine.GetSize() )
			{
				const char cNext = oStrLine[ uiPosNext ];
				if( !( cNext == ' ' || cNext == '\t' ) )
					continue;
			}
			
			if( iPosToken >= 0 )
			{
				oStrLine.Del( 0, uiTokenSize );
				RemoveLeadingSpace( oStrLine );
				uiTokenNext = m_aoToken_[t].GetSecond();
				PBDF_LOG( "token: %s\n", oStrToken.GetData() );
				break;
			}
		}
		
		if( uiTokenNext != PBDF_TOKEN_NONE )
		{
			switch( uiTokenNext )
			{
			case PBDF_TOKEN_STARTFONT:
				ReadValue( oStrLine, &m_dVersion );
				PBDF_LOG( "version: %g\n", m_dVersion );
			break;
			case PBDF_TOKEN_COMMENT:
			break;
			case PBDF_TOKEN_CONTENTVERSION: // optional
				ReadValue( oStrLine, &m_iContentVersion );
				PBDF_LOG( "content_version: %d\n", m_iContentVersion );
			break;
			case PBDF_TOKEN_FONT:
				ReadValue( oStrLine, &m_oStrFont );
				PBDF_LOG( "font: %s\n", m_oStrFont.GetData() );
			break;
			case PBDF_TOKEN_SIZE:
				ReadValue( oStrLine, &m_iPointSize );
				PBDF_LOG( "point_size: %d\n", m_iPointSize );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &m_iResX );
				PBDF_LOG( "res_x: %d\n", m_iResX );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &m_iResY );
				PBDF_LOG( "res_y: %d\n", m_iResY );
			break;
			case PBDF_TOKEN_FONTBOUNDINGBOX:
				ReadValue( oStrLine, &m_iBBX );
				PBDF_LOG( "bounding_box_x: %d\n", m_iBBX );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &m_iBBY );
				PBDF_LOG( "bounding_box_y: %d\n", m_iBBY );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &m_iOffX );
				PBDF_LOG( "offset_y: %d\n", m_iOffX );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &m_iOffY );
				PBDF_LOG( "offset_x: %d\n", m_iOffY );
				RemoveNextWord( oStrLine );
			break;
			case PBDF_TOKEN_METRICSSET: // optional
				ReadValue( oStrLine, &m_iMetricsSet );
				PBDF_LOG( "metrics_set: %d\n", m_iMetricsSet );
			break;
			case PBDF_TOKEN_SWIDTH:
				ReadValue( oStrLine, &poGlyph->m_dSX );
				PBDF_LOG( "sx: %g\n", poGlyph->m_dSX );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &poGlyph->m_dSY );
				PBDF_LOG( "sy: %g\n", poGlyph->m_dSY );
			break;
			case PBDF_TOKEN_DWIDTH:
				ReadValue( oStrLine, &poGlyph->m_iDX );
				PBDF_LOG( "dx: %d\n", poGlyph->m_iDX );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &poGlyph->m_iDY );
				PBDF_LOG( "dy: %d\n", poGlyph->m_iDY );
			break;
			case PBDF_TOKEN_SWIDTH1:
				ReadValue( oStrLine, &poGlyph->m_dSX1 );
				PBDF_LOG( "sx1: %g\n", poGlyph->m_dSX1 );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &poGlyph->m_dSY1 );
				PBDF_LOG( "sy1: %g\n", poGlyph->m_dSY1 );
			break;
			case PBDF_TOKEN_DWIDTH1:
				ReadValue( oStrLine, &poGlyph->m_iDX1 );
				PBDF_LOG( "dx1: %d\n", poGlyph->m_iDX1 );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &poGlyph->m_iDY1 );
				PBDF_LOG( "dy1: %d\n", poGlyph->m_iDY1 );
			break;
			case PBDF_TOKEN_VVECTOR: // optional
				ReadValue( oStrLine, &poGlyph->m_dVVX );
				PBDF_LOG( "vvx: %g\n", poGlyph->m_dVVX );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &poGlyph->m_dVVY );
				PBDF_LOG( "vvy: %g\n", poGlyph->m_dVVY );
			break;
			case PBDF_TOKEN_STARTPROPERTIES: // optional
				ReadValue( oStrLine, &m_iProperties );
				PBDF_LOG( "properties: %d\n", m_iProperties );
				iPropertiesStartPos = iEndOfLine + 1;
			break;
			case PBDF_TOKEN_ENDPROPERTIES: // optional
				if( iPropertiesStartPos < 0 )
					break;
				m_oStrProperties = oStrFontFile.GetSub( 
					iPropertiesStartPos, iPos - iPropertiesStartPos );
				PBDF_LOG( "properties:\n%s\n", m_oStrProperties.GetData() );
			break;
			case PBDF_TOKEN_CHARS:
				ReadValue( oStrLine, &m_iChars );
				PBDF_LOG( "chars: %d\n", m_iChars );
				
				poGlyph = &oGylphNew;
			break;
			case PBDF_TOKEN_STARTCHAR:
				if( bIsReadingChar )
				{
					PBDF_ERROR( "glyph %s\n", oStrLine.GetData() );
					return false;
				}
				bIsReadingChar = true;
				
				oGylphNew = m_oGlyphDefault;
				ReadValue( oStrLine, &poGlyph->m_oStrName );
				PBDF_LOG( "char_name: %s\n", poGlyph->m_oStrName.GetData() );
				PBDF_LOG( "char_num: %d\n", m_oArrGlyph.GetSize() );
			break;
			case PBDF_TOKEN_ENCODING:
				ReadValue( oStrLine, &poGlyph->m_iEncoding0  );
				PBDF_LOG( "encoding0: %d\n", poGlyph->m_iEncoding0 );
				RemoveNextWord( oStrLine );
				if( !oStrLine.GetSize() )
					break;
				ReadValue( oStrLine, &poGlyph->m_iEncoding1 );
				PBDF_LOG( "encoding1: %d\n", poGlyph->m_iEncoding1 );
			break;
			case PBDF_TOKEN_BBX:
				ReadValue( oStrLine, &poGlyph->m_iBBX );
				PBDF_LOG( "bbx: %d\n", poGlyph->m_iBBX );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &poGlyph->m_iBBY );
				PBDF_LOG( "bby: %d\n", poGlyph->m_iBBY );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &poGlyph->m_iBBOffX );
				PBDF_LOG( "bb_off_x: %d\n", poGlyph->m_iBBOffX );
				RemoveNextWord( oStrLine );
				ReadValue( oStrLine, &poGlyph->m_iBBOffY );
				PBDF_LOG( "bb_off_y: %d\n", poGlyph->m_iBBOffY );
			break;
			case PBDF_TOKEN_BITMAP:
			{
				CArray<unsigned char> oArrBitmap;
				const int iHeight = poGlyph->m_iBBY;
				
				// Da hier sowieso kein Token folgt, kann ohne weiteres
				// auch über das Zeilenende hinaus gearbeitet werden.
				int iPosBegin = iEndOfLine + 1;
				int l = 0;
				
				poGlyph->m_oArrBitmap.Clear();
				
				while( l < iHeight )
				{
					const int iEOL = oStrFontFile.Find( iPosBegin, PBDF_NEWLINE ); 
					if( iEOL < 0 )
						break;
					CStr oCodeLine = oStrFontFile.GetSub( iPosBegin, iEOL - iPosBegin );
					ReadValue( oCodeLine, &oArrBitmap );
					poGlyph->m_oArrBitmap.Append( oArrBitmap );
					iPosBegin = iEOL + 1;
					++l;
				}		
			}
			break;
			case PBDF_TOKEN_ENDCHAR:
				m_oArrGlyph.Append( oGylphNew );
				bIsReadingChar = false;
			break;
			case PBDF_TOKEN_ENDFONT:
				PBDF_LOG( "glyph_count: %d\n", m_oArrGlyph.GetSize() );
				if( m_oArrGlyph.GetSize() != (unsigned int)m_iChars )
				{
					PBDF_ERROR( "glyph_cout: %d != %d", m_oArrGlyph.GetSize(), m_iChars );
					return false;
				}
			break;
			};
		}
		iPos = iEndOfLine + 1; // falls "\r\n"??
	}
	
	PBDF_LOG( "parse: end\n" );
	return true;
}

void CPrsBDF::Reset()
{
	m_dVersion = 0.0;
	m_iContentVersion = 0;
	m_oStrFont.Clear();
	m_iPointSize = 0;
	m_iResX = m_iResY = 0;
	m_iBBX = m_iBBY = 0;
	m_iOffX = m_iOffY = 0;
	m_iMetricsSet = 0;
	/*
	m_iSX = m_iSY = 0;
	m_iDX = m_iDY = 0;
	m_iSX1 = m_iSY1 = 0;
	m_iDX1 = m_iDY1 = 0;
	m_iVVector = 0;
	*/
	m_oGlyphDefault.Reset();
	m_iProperties = 0;
	m_oStrProperties.Clear();
	m_oArrGlyph.Clear();
}

bool CPrsBDF::RemoveNextWord( CStr &roStr )
{
	if( !roStr.GetSize() )
		return false;
	static const char *acSpace_ = " \t";
	RemoveLeadingSpace( roStr );
	int iPos = roStr.FindVec( 0, acSpace_ );
	if( iPos < 0 )
		iPos = roStr.GetSize();
	roStr.Del( 0, iPos );
	RemoveLeadingSpace( roStr );
	return true;
}

bool CPrsBDF::RemoveLeadingSpace( CStr &roStr )
{
	static const char *acSpace_ = " \t";
	int iPos = roStr.FindVecInv( 0, acSpace_ );
	if( iPos >= 0 )
	{
		roStr.Del( 0, iPos );
		return true;
	}
	return false;
}

void CPrsBDF::ReadValue( CStr &roStr, double *pdNumber )
{
	if( roStr.GetSize() )
		*pdNumber = atof( roStr.GetData() ); // TODO: ersetzen durch eigene Funktion.
}

void CPrsBDF::ReadValue( CStr &roStr, int *piInteger )
{
	if( roStr.GetSize() )
		*piInteger = atoi( roStr.GetData() ); // TODO: ersetzen durch eigene Funktion.
}

void CPrsBDF::ReadValue( CStr &roStr, CStr *poString )
{
	if( roStr.GetSize() )
		*poString = roStr; // TODO...
}

void CPrsBDF::ReadValue( CStr &roStr, CArray<unsigned char> *poArrByte )
{
	poArrByte->Clear();
	CStr oStrHex( roStr );
	const int iSize = roStr.GetSize();
	int iPos = 0;
	char acHex[3] = { 0, 0, 0 };
	const char *pcS = oStrHex.GetData();
	while( iPos < iSize )
	{
		acHex[0] = *( pcS + iPos );
		++iPos;
		acHex[1] = *( pcS + iPos );
		++iPos;
		const unsigned char ucByte = (unsigned char)strtol( acHex, 0, 16 ); // TODO: ersetzen durch eigene Funktion.
		poArrByte->Append( ucByte );
	}
}
