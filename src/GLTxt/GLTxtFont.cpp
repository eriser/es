// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// GLTxtFont.cpp

#include "GLTxtFont.h"

////////////////////////////////////////////////////////////////////////////////
// CGLTxtFont
////////////////////////////////////////////////////////////////////////////////

CGLTxtFont::CGLTxtFont()
: m_tListCount( 0 )
, m_uiLstBase( 0 )
{}

CGLTxtFont::~CGLTxtFont()
{
	//ClearDisplayLists(); // ...depends
}

void CGLTxtFont::InitDisplayLists( GLsizei tSize )
{
	ClearDisplayLists();
	m_tListCount = tSize;
	m_uiLstBase = glGenLists( m_tListCount );
}

void CGLTxtFont::ClearDisplayLists()
{
	if( m_uiLstBase )
	{
		glDeleteLists( m_uiLstBase, m_tListCount );
		m_uiLstBase = 0;
		m_tListCount = 0;
	}
}

bool CGLTxtFont::Destroy()
{
	ClearDisplayLists();
	return true;
}


#if defined( GLTXT_USE_FONT_BDF )

////////////////////////////////////////////////////////////////////////////////
// CGLTxtFontBDF
////////////////////////////////////////////////////////////////////////////////

CGLTxtFontBDF::CGLTxtFontBDF()
: CGLTxtFont()
, m_bUnderline( false )
, m_bIsValid( false )
{}

CGLTxtFontBDF::~CGLTxtFontBDF()
{}

void CGLTxtFontBDF::Setup( const char *pcFont, bool bUnderline )
{
	m_bIsValid = m_oParser.Parse( pcFont );
	m_bUnderline = bUnderline;
	memset( m_afGlyphWidth, 0, sizeof(m_afGlyphWidth) / sizeof(GLfloat) );
}

bool CGLTxtFontBDF::Create()
{
	if( !m_bIsValid )
		return false;

	Destroy();

	int iGlyphCount = m_oParser.m_oArrGlyph.GetSize();

	// max 256 characters (lists)
	if( iGlyphCount > 256 )
		iGlyphCount = 256;
	InitDisplayLists( iGlyphCount );

	//UpdatePixelExt();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*
	GLTXT_LOG( "ParserBDF: GLOBAL INFORMATION\n" );
	GLTXT_LOG( "////////////////////////////////////////////////////////////////////////////////\n" );
	GLTXT_LOG( "ParserBDF.m_dVersion:        %g\n", m_oParser.m_dVersion );
	GLTXT_LOG( "ParserBDF.m_iContentVersion: %d\n", m_oParser.m_iContentVersion );
	GLTXT_LOG( "ParserBDF.m_oStrFont:        %s\n", m_oParser.m_oStrFont.GetData() );
	GLTXT_LOG( "ParserBDF.m_iPointSize:      %d\n", m_oParser.m_iPointSize );
	GLTXT_LOG( "ParserBDF.m_iResX/Y:         %d, %d\n", m_oParser.m_iResX, m_oParser.m_iResY );
	GLTXT_LOG( "ParserBDF.m_iBBX/Y:          %d, %d\n", m_oParser.m_iBBX, m_oParser.m_iBBY );
	GLTXT_LOG( "ParserBDF.m_iOffX/Y:         %d, %d\n", m_oParser.m_iOffX, m_oParser.m_iOffY );
	GLTXT_LOG( "ParserBDF.m_iMetricsSet:     %d\n", m_oParser.m_iMetricsSet );
	//m_oGlyphDefault
	GLTXT_LOG( "ParserBDF.m_iProperties:     %d\n", m_oParser.m_iProperties );
	GLTXT_LOG( "ParserBDF.m_oStrProperties:  \n%s\n", m_oParser.m_oStrProperties.GetData() );
	GLTXT_LOG( "ParserBDF.m_iChars:          %d\n", m_oParser.m_iChars );

	GLTXT_LOG( "ParserBDF: GLYPH INFORMATION\n" );
	GLTXT_LOG( "////////////////////////////////////////////////////////////////////////////////\n" );
	GLTXT_LOG( "glyph_count:                 %d\n", iGlyphCount );
	for( int i=0; i<iGlyphCount; ++i )
	{
		CPrsBDF::CGlyph &roGlyph = m_oParser.m_oArrGlyph[i];
		GLTXT_LOG( "glyph_num:                  %d\n", i );
		GLTXT_LOG( "---------------------------------------------------------\n" );
		GLTXT_LOG( "GlyphBDF.m_oStrName:        %s\n",     roGlyph.m_oStrName.GetData() );
		GLTXT_LOG( "GlyphBDF.m_iEncoding0/1     %d, %d\n", roGlyph.m_iEncoding0, roGlyph.m_iEncoding1 );
		GLTXT_LOG( "GlyphBDF.m_dSX/Y:           %g, %g\n", roGlyph.m_dSX, roGlyph.m_dSY );
		GLTXT_LOG( "GlyphBDF.m_iDX/Y:           %d, %d\n", roGlyph.m_iDX, roGlyph.m_iDY );
		GLTXT_LOG( "GlyphBDF.m_dSX1/Y1:         %g, %g\n", roGlyph.m_dSX1, roGlyph.m_dSY1 );
		GLTXT_LOG( "GlyphBDF.m_iDX1/Y1:         %d, %d\n", roGlyph.m_iDX1, roGlyph.m_iDY1 );
		GLTXT_LOG( "GlyphBDF.m_dVVX/Y:          %g, %g\n", roGlyph.m_dVVX, roGlyph.m_dVVY );
		GLTXT_LOG( "GlyphBDF.m_iBBX/Y:          %d, %d\n", roGlyph.m_iBBX, roGlyph.m_iBBY );
		GLTXT_LOG( "GlyphBDF.m_iBBOffX/Y:       %d, %d\n", roGlyph.m_iBBOffX, roGlyph.m_iBBOffY );
		GLTXT_LOG( "GlyphBDF.m_oArrBitmap.Size: %d\n",     roGlyph.m_oArrBitmap.GetSize() );
		GLTXT_LOG( "\n" );

		//CArray<unsigned char> m_oArrBitmap;
	}
	*/

	// Umrechungsfaktoren: fRX/Y entsprechen Anzahl Punkten.
	// rem: 1 Point = 1/72 inch
	//const GLfloat fRX = m_oParser.m_iResX / 72.27f; // z.B.  96 dpi / 72.27 dpi ~= 1.333 pxl...
	//const GLfloat fRY = m_oParser.m_iResY / 72.27f;
	//const GLfloat fFacP = m_oParser.m_iPointSize / 1000.0f; // m_dSX/Y sind in 1000steln der Grösse des Glyphs.

	unsigned int uiAllocSize = 0;
	GLubyte *ptBitmap = 0;
	for( int i=0; i<m_tListCount; ++i )
	{
		CPrsBDF::CGlyph &roGlyph = m_oParser.m_oArrGlyph[i];
		const unsigned int uiBitmapSize = roGlyph.m_oArrBitmap.GetSize();
		const unsigned int uiByteWidth = ( roGlyph.m_iBBX >> 3 ) + (( roGlyph.m_iBBX % 8 )?( 1 ): ( 0 ));
		const unsigned int uiBytesCheck = uiByteWidth * roGlyph.m_iBBY;
		if( uiBitmapSize > uiAllocSize )
		{
			uiAllocSize = uiBitmapSize;
			if( ptBitmap )
				delete [] ptBitmap;
			ptBitmap =  new GLubyte[ uiAllocSize ];
		}
		if( !uiBitmapSize )
		{
			GLTXT_LOG( "glyph %s (%d): no bitmap data found (%dx%d)\n",
				roGlyph.m_oStrName.GetData(),
				roGlyph.m_iEncoding0,
				roGlyph.m_iBBX, roGlyph.m_iBBY );
		}
		if( uiBytesCheck != uiBitmapSize )
		{
			GLTXT_ERROR( "glyph %s: bitmap size does not correspond to glyph size\n", roGlyph.m_oStrName.GetData() );
			return false;
		}

		if( uiBitmapSize )
		{
			memset( ptBitmap, 0, uiBitmapSize );

			unsigned int c = 0;
			for( int iLine=roGlyph.m_iBBY-1; iLine>=0; --iLine )
			{
				unsigned int uiIndex = (unsigned int)iLine * uiByteWidth;
				for( unsigned int uiByte=0; uiByte<uiByteWidth; ++uiByte )
				{
					ptBitmap[c] = roGlyph.m_oArrBitmap[uiIndex];
					++uiIndex;
					++c;
				}
			}
		}

		//GLTXT_LOG( "glyph: %s (%d)\n", roGlyph.m_oStrName.GetData(), roGlyph.m_iEncoding0 );
		//GLTXT_LOG( "glyph: %dx%d bytes\n", uiByteWidth, roGlyph.m_iBBY );

		// expermiental
		if( m_bUnderline )
			memset( ptBitmap, 0xFF, uiByteWidth );

		//memset( ptBitmap, 255, uiBitmapSize ); // block

		// rem: Leider meist nicht korrekt implementiert in den BDF-Fonts!
		// -> Direkt device-spezifisch, pixel-orientiert arbeiten.
		//GLfloat fMoveX = roGlyph.m_dSX * fFacP * fRX;
		//GLfloat fMoveY = roGlyph.m_dSY * fFacP * fRY;
		//GLTXT_LOG( "fMove: %g, %g\n", fMoveX, fMoveY );

		const unsigned int uiCode = roGlyph.m_iEncoding0;
		if( uiCode >= (unsigned int)m_tListCount )
		{
			//GLTXT_LOG( "skip glyph\n" );
			continue;
		}

		m_afGlyphWidth[uiCode] = GLfloat( roGlyph.m_iDX );

		glNewList( m_uiLstBase + uiCode, GL_COMPILE );
		glBitmap(
			roGlyph.m_iBBX,
			roGlyph.m_iBBY,
			GLfloat( -roGlyph.m_iBBOffX ),
			GLfloat( -roGlyph.m_iBBOffY ),
			GLfloat( roGlyph.m_iDX ), //fMoveX,
			GLfloat( roGlyph.m_iDY ), //fMoveY,
			(GLubyte *)&ptBitmap[0] );
		glEndList();

	}
	if( ptBitmap )
		delete [] ptBitmap;

	return true;
}

GLfloat CGLTxtFontBDF::GetWidth( const char *pcStr, unsigned int uiLen ) const
{
	GLfloat fWidth = 0.0f;
	while( uiLen )
	{
		fWidth += m_afGlyphWidth[ (unsigned int)*pcStr++ ];
		--uiLen;
	}
	return fWidth; // * m_fPixelExtX;
}

GLfloat CGLTxtFontBDF::GetWidth( char cChar ) const {
	return m_afGlyphWidth[ (unsigned int)cChar ]; // * m_fPixelExtX;
}

GLfloat CGLTxtFontBDF::GetSizePxl() const {
	return GLfloat( m_oParser.m_iBBY );
}

#endif // GLTXT_USE_FONT_BDF


#if defined( __WINDOWS__ )

////////////////////////////////////////////////////////////////////////////////
// CGLTxtFontMS
////////////////////////////////////////////////////////////////////////////////

CGLTxtFontMS::CGLTxtFontMS()
: CGLTxtFont()
, m_hDC( 0 )
, m_pcFont( 0 )
, m_uiSize( 0 )
, m_bBold( false )
, m_bItalic( false )
, m_bUnderline( false )
, m_bStrikeout( false )
/*, m_bUse3DFont( false )
, m_f3DThicknessZ( 1.0f )
, m_f3DDeviation( 0.0f )*/
{
	memset( m_afCharWidth, 0, 256 * sizeof( GLfloat ) );
}

CGLTxtFontMS::~CGLTxtFontMS()
{
	//Destroy(); // Scheint  nicht nötig/erwünscht zu sein (-> GlIntercept). wgl löscht die Display-Listen wahrscheinlich automatisch...
}

void CGLTxtFontMS::Setup(
	  HDC hDC,
	  const char *pcFont,
	  unsigned int uiSize,
	  bool bBold,
	  bool bItalic,
	  bool bUnderline,
	  bool bStrikeout//,
	  //bool bUse3DFont,
	  //float f3DThicknessZ,
	  //float f3DDeviation
	 )
{
	m_hDC = hDC;
	m_pcFont = pcFont;
	m_uiSize = uiSize;
	m_bBold = bBold;
	m_bItalic = bItalic;
	m_bUnderline = bUnderline;
	m_bStrikeout = bStrikeout;
	//m_bUse3DFont = bUse3DFont;
	//m_f3DThicknessZ = f3DThicknessZ;
	//m_f3DDeviation = f3DDeviation;
}

bool CGLTxtFontMS::Create()
{
	HFONT hOldFont = 0;

	if( !m_hDC )
		return false;

	Destroy();
	InitDisplayLists( 256 );

	HFONT hFont = ::CreateFont(			// Windows Font ID
		-(int)(m_uiSize),			// Height Of Font
		0,					// Width Of Font
		0,					// Angle Of Escapement
		0,					// Orientation Angle
		(m_bBold)?(FW_BOLD):(FW_NORMAL),	// Font Weight (FW_BOLD)
		m_bItalic,				// Italic
		m_bUnderline,				// Underline
		m_bStrikeout,				// Strikeout
		ANSI_CHARSET,				// Character Set Identifier // z.B. Für "Windings" hier SYMBOL_CHARSET
		OUT_TT_ONLY_PRECIS,			// Output Precision (only TrueType)
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
		m_pcFont );				// Font Name

	hOldFont = (HFONT)( ::SelectObject( m_hDC, hFont ) );

/*
	// Outline Font
	if( m_bUse3DFont )
	{
		GLYPHMETRICSFLOAT aoGMF[256];
		if( m_f3DDeviation < 0.0f )
			m_f3DDeviation = 0.0f;
		
		::wglUseFontOutlines(
			m_hDC,			// Select The Current DC
			0,			// Starting Character
			256,			// Number Of Display Lists To Build
			m_uiLstBase,		// Starting Display Lists
			m_f3DDeviation,		// Deviation From The True Outlines
			m_f3DThicknessZ,	// Font Thickness In The Z Direction
			WGL_FONT_POLYGONS,	// Use Polygons, Not Lines
			aoGMF );		// Address Of Buffer To Recieve Data
		
		for( unsigned int i=0; i<256; ++i )
		{
			m_afCharWidth[i] = aoGMF[i].gmfCellIncX;
		}
	}
	// Bitmap Font
	else
*/
	{
		::wglUseFontBitmaps( m_hDC, 0, 256, m_uiLstBase ); // Builds 96 Characters Starting At Character 32
		
		// Breite aller Buchstaben ermitteln (API ist völlig wirr...)
		/*
		// ACHTUNG: Klappt nicht!
		GLYPHMETRICS oGMF;
		//MAT2 oMAT;
		//DWORD dwBufferSize = ::GetGlyphOutline( m_hDC, 0, GGO_METRICS, &oGMF, 0, 0, &oMAT );
		//unsigned char *pucBuffer = new unsigned char[dwBufferSize];
		GLdouble dPxlConv = GetPixelObjExtX_();

		for( UINT uChar=0; uChar<256; ++uChar )
		{
			//GetTextMetric
			//GetTextExtentPoint
			if( GDI_ERROR == ::GetGlyphOutline( m_hDC, uChar, GGO_METRICS, &oGMF, 0, 0, 0 ) )
				::MessageBox( 0, "ERROR", "GetGlyphOutline", MB_OK );
			m_aoGMF[uChar].gmfBlackBoxX = dPxlConv * oGMF.gmBlackBoxX;
			m_aoGMF[uChar].gmfBlackBoxY = dPxlConv * oGMF.gmBlackBoxY;
			m_aoGMF[uChar].gmfptGlyphOrigin.x = dPxlConv * oGMF.gmptGlyphOrigin.x;
			m_aoGMF[uChar].gmfptGlyphOrigin.y = dPxlConv * oGMF.gmptGlyphOrigin.y;
			m_aoGMF[uChar].gmfCellIncX = dPxlConv * oGMF.gmCellIncX;
			m_aoGMF[uChar].gmfCellIncY = dPxlConv * oGMF.gmCellIncY;
			GLTXT_LOG( "%c:\t%f, %f\n", (char)uChar, m_aoGMF[uChar].gmfCellIncX, m_aoGMF[uChar].gmfBlackBoxX );
		}*/
		
		
		TEXTMETRIC oTM;

		//GLdouble dPxlConv = GetPixelObjExtX_();
		ABCFLOAT aoWidth[256];
		GLfloat fExt;
		//INT aiWidth[256];
		// GetOutlineTextMetrics/GetGlyphOutline/...???
		// GetTabbedTextExtent
		// GetTextExtentPoint32

		::GetTextMetrics( m_hDC, &oTM );
		if( !(oTM.tmPitchAndFamily & TMPF_TRUETYPE) )
		{
			::MessageBox( 0, "Kein TrueType-Font!", "ERROR", MB_OK );
		}

		// Each logical unit is mapped to one device pixel.
		// Positive x is to the right; positive y is down.
		//const int iMapModeOld = ::SetMapMode( m_hDC, MM_TEXT );

		//m_iExtra = ::GetTextCharacterExtra( m_hDC );

		//::GetCharABCWidths für TrueType.
		//::GetCharWidth32( m_hDC, 0, 255, aiWidth );
		//::GetCharWidthFloat( m_hDC, 0, 255, m_afCharWidth );
		::GetCharABCWidthsFloat( m_hDC, 0, 255, aoWidth );
		for( unsigned int i=0; i<256; ++i )
		{
			//POINT oPnt = { aiWidth[i], 0 };
			//m_afCharWidth[i] = oPnt.x * dPxlConv;
			fExt  = aoWidth[i].abcfA;
			fExt += aoWidth[i].abcfB;
			fExt += aoWidth[i].abcfC;
			//::LPtoDP( m_hDC, &oPnt, 1 ); // K.A. ob nötig!
			m_afCharWidth[i] = fExt; // * GLfloat( dPxlConv );
		}

		//::SetMapMode( m_hDC, iMapModeOld );
	}
	
	::SelectObject( m_hDC, hOldFont );
	::DeleteObject( hFont );

	return true;
}

GLfloat CGLTxtFontMS::GetWidth( const char *pcStr, unsigned int uiLen ) const
{
	/*
	SIZE oSize = { 0, 0 };
	POINT oPnt;
	TEXTMETRIC oTM;

	::GetTextExtentPoint32( m_hDC, pcStr, lstrlen(pcStr), &oSize );

	// Retrieve the overhang value from the TEXTMETRIC
	// structure and subtract it from the x-increment.
	// (This is only necessary for non-TrueType raster
	// fonts.)
	::GetTextMetrics( m_hDC, &oTM );
	oPnt.x = oSize.cx + oTM.tmOverhang;


	const int iMapModeOld = ::SetMapMode( m_hDC, MM_TEXT );
	LPtoDP( m_hDC, &oPnt, 1 );
	::SetMapMode( m_hDC, iMapModeOld );

	return oPnt.x * GetPixelObjExtX_();
	*/

	GLfloat fWidth = 0.0f;
	while( uiLen )
	{
		fWidth += m_afCharWidth[(unsigned int)(*pcStr++)];
		--uiLen;
	}

	return fWidth;
}

GLfloat CGLTxtFontMS::GetWidth( char cChar ) const {
	// TODO:
	return m_afCharWidth[(unsigned int)(cChar)];
}

GLfloat CGLTxtFontMS::GetSizePxl() const {
	return GLfloat( m_uiSize ); // ???: point -> pixel conversion (96dpi) ( fac = device-resolution / 72.27 )
}

/*
GLfloat CGLTxtFontMS::GetHeight( const char *pcStr ) const
{
	SIZE oSize;
	if( GetTextExtentPoint32( m_hDC, pcStr, lstrlen(pcStr), &oSize ) )
	{

	}
}

GLfloat CGLTxtFontMS::GetHeight( unsigned char ucChar ) const
{

}
*/

#endif // __WINDOWS__


#if defined( __LINUX__ )

////////////////////////////////////////////////////////////////////////////////
// CGLTxtFontX11
////////////////////////////////////////////////////////////////////////////////

CGLTxtFontX11::CGLTxtFontX11()
: CGLTxtFont()
, m_poDisplay( 0 )
, m_pcFont( 0 )
, m_uiSize( 0 )
{

}

CGLTxtFontX11::~CGLTxtFontX11()
{}

void CGLTxtFontX11::Setup( Display *poDisplay, const char *pcFont )
{
	m_poDisplay = poDisplay;
	m_pcFont = pcFont;
}

/*
Display *Dpy;
XFontStruct *m_poFont;

Dpy = XOpenDisplay(NULL);
m_poFont = XLoadQueryFont(Dpy, "-*-helvetica-*-*-*-*-*-180-*-*-*-*-*-*");
//Sollte man noch auf NULL testen

glXUseXFont(m_poFont->fid, 32, 96, FontList);

XFreeFont(Dpy, m_poFont);
XCloseDisplay(Dpy);
*/

// http://www.koders.com/c/fidEF333BE5086CB6E82ED17DEA9F5308B118BFCB97.aspx
/*
XFontStruct *m_poFont;
Font id;
unsigned int first, last;

m_poFont = XLoadQueryFont(dpy, "6x12");
if (m_poFont == NULL) {
	*fwidth = 0;
	*fheight = 0;
	return;
}

id = m_poFont->fid;
first = m_poFont->min_char_or_byte2;
last = m_poFont->max_char_or_byte2;

fontbase = glGenLists((GLuint) last+1);
if (fontbase == 0) {
	*fwidth = 0;
	*fheight = 0;
	return;
}
glXUseXFont(id, first, last-first+1, fontbase+first);

*fheight = m_poFont -> ascent + m_poFont -> descent;
*fwidth  = m_poFont -> max_bounds.width;
*/


/*
// http://web.mit.edu/answers/xwindows/xwindows_font_fields.html
Most X fonts have a 14-field name in the form:

	-foundry-family-weight-slant-setwidth-style-pixelSize-pointSize-\
	  Xresolution-Yresolution-spacing-averageWidth-registry-encoding

The 14 fields are as follows:

00 -
01 foundry         the name or identifier of the digital type foundry
                   that digitized and supplied the font data.
02 family          a string that identifies the basic typographic style
                   (e.g., "helvetica", "courier", etc.).
03 weight          a string that identifies the nominal blackness of the
                   font, according to the FOUNDRY's judgment (e.g.,
                   "medium", "bold", etc.).
04 slant           a code string that indicates the overall posture of
                   the typeface design used in the font -- one of the
                   following:
                     R       Roman           upright design
                     I       Italic          italic design, slanted clockwise
                     O       Oblique         obliqued upright design,
                                               slanted clockwise
05 setwidth        a string that gives the nominal width per horizontal
		   unit of the font, according to the FOUNDRY's judgment
		   (e.g., "Normal", "Narrow", "Double Wide").
06 style           a string that identifies additional style information
		   not captured by other fields, but needed to uniquely
		   identify the font in the FOUNDRY's judgment (e.g.,
		   "Serif", "Sans Serif").
07 pixelSize       an unsigned integer-string typographic metric in
		   device pixels which gives the body size of the font.
08 pointSize       an unsigned integer-string typographic metric in
		   device-independent units which gives the body size for
		   which the font was designed.
09 Xresolution     an unsigned integer-string in pixels/dots per inch
		   (dpi) which gives the horizontal resolution for which
		   the font was designed.
10 Yresolution     an unsigned integer-string in pixels/dots per inch
		   (dpi) which gives the vertical resolution for which
		   the font was designed.
11 spacing         a code-string that indicates the escapement class of
		   the font -- one of the following:
		     P       Proportional    proportional font
		     M       Monospace       a font whose logical character
					       widths are constant, but no
  					       other restrictions apply
	    	     C       CharCell        a monospaced font in which the
					       glyphs can be modeled as boxes
					       of identical width and height
					       with no "ink" outside the box.
12 averageWidth    an unsigned integer-string value giving the unweighted
		   arithmetic mean width of all glyphs in the font,
		   measured in 1/10th pixels.
13 registry        a registered name that identifies the registration
		   authority (i.e. international standards body) that
		   owns the specified encoding.
14 encoding        a registered name that identifies the coded character
		   set as defined by the specified registry.
*/

bool CGLTxtFontX11::Create()
{
	if( !m_poDisplay )
		return false;

	Destroy();

	//UpdatePixelExt();

	Font id;
	int first, last, firstbitmap, i;
	int firstrow, lastrow;
	int maxchars;


	m_poFont = XLoadQueryFont( m_poDisplay, m_pcFont );
	if( !m_poFont )
		return false;

	long unsigned int luiValue;
	//if( XGetFontProperty( m_poFont, XA_X_HEIGHT, &luiValue ) ) {
	//	m_uiSize = luiValue;
	//	GLTXT_LOG( "font size: %li\n", luiValue );
	//}

	if( XGetFontProperty( m_poFont, XA_FONT, &luiValue ) )
	{
		char *pcFontName = XGetAtomName( m_poDisplay, (Atom)luiValue );
		GLTXT_LOG( "font name: %s\n", pcFontName );

		const char *pcStartSize = 0;
		unsigned int c = 0;
		while( *pcFontName )
		{
			if( *pcFontName == '-' )
			{
				++c;
				if( c == 7 )
				{
					pcStartSize = pcFontName + 1;
				}
				else if( c == 8 )
				{
					*pcFontName = '\0';
					m_uiSize = atoi( pcStartSize );
					GLTXT_LOG( "font pixel size: %d\n", m_uiSize );
					break;
				}
			}
			pcFontName++;
		}
	}
	id = m_poFont->fid;

	// First and Last char in a row of chars.
	first = (int)m_poFont->min_char_or_byte2;
	last = (int)m_poFont->max_char_or_byte2;

	// First and Last row of chars, important for multibyte charset's.
	firstrow = (int)m_poFont->min_byte1;
	lastrow = (int)m_poFont->max_byte1;

	// How many chars in the charset.
	maxchars = 256 * lastrow + last;
	InitDisplayLists( glGenLists(maxchars+1) );

	// Get offset to first char in the charset.
	firstbitmap = 256 * firstrow + first;

	// for each row of chars, call glXUseXFont to build the bitmaps.
	for( i=firstrow; i<=lastrow; i++ )
	{
		glXUseXFont( id, firstbitmap, last-first+1, m_uiLstBase+firstbitmap );
		firstbitmap += 256;
	}

	return true;
}


GLfloat CGLTxtFontX11::GetWidth( char cChar ) const
{
	// TODO:
	// hack...
	// XTextExtents(...
	return XTextWidth( m_poFont, &cChar, 1 ); // * m_fPixelExtX;
}

GLfloat CGLTxtFontX11::GetWidth( const char *pcStr, unsigned int uiLen ) const {
	return XTextWidth( m_poFont, pcStr, uiLen ); // * m_fPixelExtX;
}

GLfloat CGLTxtFontX11::GetSizePxl() const {
	return GLfloat( m_uiSize );
}

#endif // __LINUX__
