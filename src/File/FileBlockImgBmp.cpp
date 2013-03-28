// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FileBlockImgBmp.h

#include "FileBlockImgBmp.h"
#include <math.h>

#ifndef WINVER

#ifndef _MSVC
#	define __ATTRIBUTE_PACKED__
#	pragma pack(push,2)
#else // __GNUC__
#	define __ATTRIBUTE_PACKED__	__attribute__ ((__packed__))
#endif

typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

typedef struct __ATTRIBUTE_PACKED__ tagBITMAPFILEHEADER
{
	WORD    bfType; 
	DWORD   bfSize; 
	WORD    bfReserved1; 
	WORD    bfReserved2; 
	DWORD   bfOffBits; 
} BITMAPFILEHEADER, *PBITMAPFILEHEADER ; 

typedef struct __ATTRIBUTE_PACKED__ tagBITMAPINFOHEADER
{
	DWORD  biSize; 
	LONG   biWidth; 
	LONG   biHeight; 
	WORD   biPlanes; 
	WORD   biBitCount; 
	DWORD  biCompression; 
	DWORD  biSizeImage; 
	LONG   biXPelsPerMeter; 
	LONG   biYPelsPerMeter; 
	DWORD  biClrUsed; 
	DWORD  biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER; 

#define BI_RGB           0

#ifndef _MSVC
#	pragma pack(pop)
#endif

#endif // WINVER

////////////////////////////////////////////////////////////////////////////////
// CFileBlockImgBmp
////////////////////////////////////////////////////////////////////////////////

#define SFH	sizeof(BITMAPFILEHEADER)
#define SIH	sizeof(BITMAPINFOHEADER)

CFileBlockImgBmp::CFileBlockImgBmp()
: CFileBlockImg()
{}

CFileBlockImgBmp::~CFileBlockImgBmp()
{}

bool CFileBlockImgBmp::Load( const char *pcFileName )
{
	if( !CFileBlockImg::Load( pcFileName ) )
		return false;
	if( !m_pucData || ( m_uiSize < SFH + SIH ) )
		return false;
	
	// rem: Nur ungefähr.
	PBITMAPFILEHEADER poHFile = PBITMAPFILEHEADER(m_pucData);
	PBITMAPINFOHEADER poHInfo = PBITMAPINFOHEADER(m_pucData + SFH);
	if(    poHFile->bfType		== 0x4D42
  	//    && poHFile->bfSize		>  SFH + SIH
	    && poHFile->bfReserved1	== 0
	    && poHFile->bfReserved2	== 0
	    && poHFile->bfOffBits	== SFH + SIH
	    && poHInfo->biSize		== SIH
	    && poHInfo->biWidth		!= 0
	    && poHInfo->biHeight	!= 0
	    && poHInfo->biPlanes	== 1
	    && ( poHInfo->biBitCount	== 24 || poHInfo->biBitCount == 32 )
	    && poHInfo->biCompression	== BI_RGB
	    && poHInfo->biSizeImage	>  0
	    //&& poHInfo->biXPelsPerMeter == 2952
	    //&& poHInfo->biYPelsPerMeter == 2952
	    && poHInfo->biClrUsed	== 0
	    && poHInfo->biClrImportant	== 0
	    && m_uiSize >= poHInfo->biWidth * poHInfo->biHeight * 3 + SFH + SIH )
	{
		m_uiWidth = poHInfo->biWidth;
		m_uiHeight = poHInfo->biHeight;
		m_uiChannels = ( poHInfo->biBitCount / 8 );
		return true;
	}
	Clear();
	return false;
}

void CFileBlockImgBmp::Clear()
{
	CFileBlockImg::Clear();
}

bool CFileBlockImgBmp::Decode( void *pvData, unsigned int uiBytes )
{
	if( !pvData || !uiBytes || !m_pucData || !m_uiSize || 
	    !( m_uiChannels == 3 || m_uiChannels == 4 ) || ( uiBytes > m_uiSize - SFH - SIH ) )
		return false;
	unsigned char *pucDst = (unsigned char *)pvData;
	unsigned char *pucSrc = m_pucData + SFH + SIH;
	if( m_uiChannels == 3 )
	{
		const unsigned int uiWidth3New = ((((m_uiWidth * 24) + 31) & ~31) >> 3);
		const unsigned int uiWidth3 = m_uiWidth * 3;
		unsigned int uiHeight = uiBytes / uiWidth3;
		if( uiHeight * uiWidth3 < uiBytes )
			++uiHeight;
		for( unsigned int y=0; y<uiHeight; ++y )
		{
			for( unsigned int x=0; x<uiWidth3; x+=3 )
			{
				*pucDst++ = pucSrc[x+2];
				*pucDst++ = pucSrc[x+1];
				*pucDst++ = pucSrc[x  ];
			}
			pucSrc += uiWidth3New;
		}
	}
	if( m_uiChannels == 4 )
	{
		// ACHTUNG: Einfach adaptiert...
		const unsigned int uiWidth4New = ((((m_uiWidth * 32) + 31) & ~31) >> 3);
		const unsigned int uiWidth4 = m_uiWidth * 4;
		unsigned int uiHeight = uiBytes / uiWidth4;
		if( uiHeight * uiWidth4 < uiBytes )
			++uiHeight;
		for( unsigned int y=0; y<uiHeight; ++y )
		{
			for( unsigned int x=0; x<uiWidth4; x+=4 )
			{
				*pucDst++ = pucSrc[x+2];
				*pucDst++ = pucSrc[x+1];
				*pucDst++ = pucSrc[x  ];
				*pucDst++ = pucSrc[x+3];
			}
			pucSrc += uiWidth4New;
		}
	}
	return true;
}

bool CFileBlockImgBmp::Encode( void *pvData, unsigned int uiBytes )
{
	if( !pvData || !uiBytes )
		return false;
	
	if( m_uiChannels != 4 ) // backward compatibility
	{
		unsigned int uiPixels = uiBytes / 3;
		if( uiBytes % 3 )
			++uiPixels;
		const unsigned int uiWH3 = m_uiWidth * m_uiHeight * 3;
		const bool bWZ = !m_uiWidth;
		const bool bWH = !m_uiHeight;
		if( bWZ || bWH ) 
		{
			if( bWZ && bWH )
			{
				m_uiWidth = (unsigned int)( sqrt( (double)uiPixels ) ); // Abrunden!
				m_uiHeight = m_uiWidth;
				while( m_uiWidth * m_uiHeight < uiPixels )
					++m_uiHeight;
			}
			else if( bWZ )
			{
				m_uiWidth = uiPixels / m_uiHeight;
				if( uiPixels != m_uiWidth * m_uiHeight )
					++m_uiWidth;
			}
			else if( bWH )
			{
				m_uiHeight = uiPixels / m_uiWidth;
				if( uiPixels != m_uiWidth * m_uiHeight )
					++m_uiHeight;
			}
		}
		else if( uiWH3 > uiBytes )
			return false;
		else if( uiWH3 < uiBytes )
			uiBytes = uiWH3;
		
		unsigned int uiWidth3 = m_uiWidth * 3;
		unsigned int uiSizeInBytes = m_uiHeight * uiWidth3;
		
		unsigned char *pucIn = (unsigned char *)pvData;
		unsigned char *pucIntern = new unsigned char[uiBytes];
		for( unsigned int i=0; i<uiBytes; i+=3)
		{
			pucIntern[i+2] = *pucIn++;
			pucIntern[i+1] = *pucIn++;
			pucIntern[i  ] = *pucIn++;
		}
		
		// rem: Padding: Breite mit DWORD-Grenze! Byte-Zahl durch 4 teilbar!
		//
		//       2^5 - 1	= 31	= 11111b
		//
		// Bsp.: 4 Pixel
		//	 4 * 24		= 96	= 1100000b
		//	 96 + 31	= 127	= 1111111b
		//	 127 & ~31	= 01111111b
		//			& 11100000b 
		//			= 01100000b = 96 => 96 / 8 = 12 bytes
		//
		// Bsp.: 6 Pixel
		//	 6 * 24		= 144	= 10010000b
		//	 144 + 31	= 175	= 10101111b
		//	 175 & ~31	= 10101111b
		//			& 11100000b
		//			= 10100000b = 160 => 160 / 8 = 20 bytes
		//
		unsigned int uiWidth3New = ((((m_uiWidth * 24) + 31) & ~31) >> 3);
		
		// rem: Verständlichere Version:
		//
		//unsigned int uiPadding = uiWidth3 % 4;
		//if( uiPadding )
		//	uiPadding = 4 - uiPadding;
		//const unsigned int uiWidth3New = uiWidth3 + uiPadding;
		
		uiSizeInBytes = m_uiHeight * uiWidth3New;
		unsigned char *pucD = new unsigned char[ uiSizeInBytes ];
		FILE_MEM_ZERO( pucD, sizeof( unsigned char ) * uiSizeInBytes );
		unsigned char *pucSrc = pucIntern;
		unsigned char *pucDst = pucD;
		for( unsigned int y=0; y<m_uiHeight-1; ++y )
		{
			FILE_MEM_COPY( pucDst, pucSrc, uiWidth3 * sizeof( unsigned char ) );
			pucDst += uiWidth3New;
			pucSrc += uiWidth3;
		}
		FILE_MEM_COPY( pucDst, pucSrc, uiBytes - uiWidth3 * (m_uiHeight-1) );
		
		BITMAPFILEHEADER oHeaderFile;
		BITMAPINFOHEADER oHeaderInfo;
		const unsigned int uiSizeTotal = uiSizeInBytes + SFH + SIH;
		
		FILE_MEM_ZERO( &oHeaderFile, SFH );
		oHeaderFile.bfType		= 0x4D42; 
		oHeaderFile.bfSize		= uiSizeTotal;
		oHeaderFile.bfReserved1		= 0;
		oHeaderFile.bfReserved2		= 0;
		oHeaderFile.bfOffBits		= SFH + SIH;
		
		FILE_MEM_ZERO( &oHeaderInfo, SIH );
		oHeaderInfo.biSize		= SIH;
		oHeaderInfo.biWidth		= m_uiWidth;
		oHeaderInfo.biHeight		= m_uiHeight;
		oHeaderInfo.biPlanes		= 1;
		oHeaderInfo.biBitCount		= 24;
		oHeaderInfo.biCompression	= BI_RGB;
		oHeaderInfo.biSizeImage		= uiSizeInBytes; // ACHTUNG: Hat Einfluss wenn != 0.
		oHeaderInfo.biXPelsPerMeter	= 2952;
		oHeaderInfo.biYPelsPerMeter	= 2952;
		oHeaderInfo.biClrUsed		= 0;
		oHeaderInfo.biClrImportant	= 0;
		
		CFileBlockImg::Init( uiSizeTotal );
		FILE_MEM_COPY( (void *)(m_pucData), (void *)&oHeaderFile, SFH );
		FILE_MEM_COPY( (void *)(m_pucData + SFH), (void *)&oHeaderInfo, SIH );
		FILE_MEM_COPY( (void *)(m_pucData + SFH + SIH), (void *)pucD, uiSizeInBytes );
		delete [] pucD;
		delete [] pucIntern;
		return true;
	}
	else if( m_uiChannels == 4 )
	{
		// ACHTUNG: Einfach adaptiert...
		unsigned int uiPixels = uiBytes / 4;
		if( uiBytes % 4 )
			++uiPixels;
		const unsigned int uiWH4 = m_uiWidth * m_uiHeight * 4;
		const bool bWZ = !m_uiWidth;
		const bool bWH = !m_uiHeight;
		if( bWZ || bWH ) 
		{
			if( bWZ && bWH )
			{
				m_uiWidth = (unsigned int)( sqrt( (double)uiPixels ) ); // Abrunden!
				m_uiHeight = m_uiWidth;
				while( m_uiWidth * m_uiHeight < uiPixels )
					++m_uiHeight;
			}
			else if( bWZ )
			{
				m_uiWidth = uiPixels / m_uiHeight;
				if( uiPixels != m_uiWidth * m_uiHeight )
					++m_uiWidth;
			}
			else if( bWH )
			{
				m_uiHeight = uiPixels / m_uiWidth;
				if( uiPixels != m_uiWidth * m_uiHeight )
					++m_uiHeight;
			}
		}
		else if( uiWH4 > uiBytes )
			return false;
		else if( uiWH4 < uiBytes )
			uiBytes = uiWH4;
		
		unsigned int uiWidth4 = m_uiWidth * 4;
		unsigned int uiSizeInBytes = m_uiHeight * uiWidth4;
		
		unsigned char *pucIn = (unsigned char *)pvData;
		unsigned char *pucIntern = new unsigned char[uiBytes];
		/*
		for( unsigned int i=0; i<uiBytes; i+=4 )
		{
			pucIntern[i+3] = *pucIn++;
			pucIntern[i+2] = *pucIn++;
			pucIntern[i+1] = *pucIn++;
			pucIntern[i  ] = *pucIn++;
		}
		*/
		FILE_MEM_COPY( pucIntern, pucIn, uiBytes );

		unsigned int uiWidth4New = ((((m_uiWidth * 32) + 31) & ~31) >> 3);
		
		uiSizeInBytes = m_uiHeight * uiWidth4New;
		unsigned char *pucD = new unsigned char[ uiSizeInBytes ];
		FILE_MEM_ZERO( pucD, sizeof( unsigned char ) * uiSizeInBytes );
		unsigned char *pucSrc = pucIntern;
		unsigned char *pucDst = pucD;
		for( unsigned int y=0; y<m_uiHeight-1; ++y )
		{
			FILE_MEM_COPY( pucDst, pucSrc, uiWidth4 * sizeof( unsigned char ) );
			pucDst += uiWidth4New;
			pucSrc += uiWidth4;
		}
		FILE_MEM_COPY( pucDst, pucSrc, uiBytes - uiWidth4 * (m_uiHeight-1) );
		
		BITMAPFILEHEADER oHeaderFile;
		BITMAPINFOHEADER oHeaderInfo;
		const unsigned int uiSizeTotal = uiSizeInBytes + SFH + SIH;
		
		FILE_MEM_ZERO( &oHeaderFile, SFH );
		oHeaderFile.bfType		= 0x4D42; 
		oHeaderFile.bfSize		= uiSizeTotal;
		oHeaderFile.bfReserved1		= 0;
		oHeaderFile.bfReserved2		= 0;
		oHeaderFile.bfOffBits		= SFH + SIH;
		
		FILE_MEM_ZERO( &oHeaderInfo, SIH );
		oHeaderInfo.biSize		= SIH;
		oHeaderInfo.biWidth		= m_uiWidth;
		oHeaderInfo.biHeight		= m_uiHeight;
		oHeaderInfo.biPlanes		= 1;
		oHeaderInfo.biBitCount		= 32;
		oHeaderInfo.biCompression	= BI_RGB;
		oHeaderInfo.biSizeImage		= uiSizeInBytes; // ACHTUNG: Hat Einfluss wenn != 0.
		oHeaderInfo.biXPelsPerMeter	= 2952;
		oHeaderInfo.biYPelsPerMeter	= 2952;
		oHeaderInfo.biClrUsed		= 0;
		oHeaderInfo.biClrImportant	= 0;
		
		CFileBlockImg::Init( uiSizeTotal );
		FILE_MEM_COPY( (void *)(m_pucData), (void *)&oHeaderFile, SFH );
		FILE_MEM_COPY( (void *)(m_pucData + SFH), (void *)&oHeaderInfo, SIH );
		FILE_MEM_COPY( (void *)(m_pucData + SFH + SIH), (void *)pucD, uiSizeInBytes );
		delete [] pucD;
		delete [] pucIntern;
		return true;
	}
	return false;
}
