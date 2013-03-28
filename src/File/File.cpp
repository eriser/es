// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// File.cpp

#ifndef _FILE_CPP_
#define _FILE_CPP_

#include "File.h"

////////////////////////////////////////////////////////////////////////////////
// CFile
////////////////////////////////////////////////////////////////////////////////

// <mod date="2010-12-09">
static const unsigned short l_usWord_( 0x1234 );
bool CFile::m_bIsLittleEndian_( ((unsigned char *)&l_usWord_)[1] == 0x12 );

//bool CFile::m_bIsLittleEndian_ =
//#if ( defined(BIG_ENDIAN) && !defined(LITTLE_ENDIAN) )
//	false;
//#elif ( !defined(BIG_ENDIAN) && defined(LITTLE_ENDIAN) )
//	true;
//#else
//#error endianess is not defined
//#endif // LITTLE BIG ENDIAN
// </mod>

CFile::CFile()
: m_bIsRead( false )
, m_bIsWrite( false )
, m_bIsTrunc( false )
, m_pFile( 0 )
{
	// <mod date="2010-12-09">
	//const unsigned short usWord = 0x1234;
	//unsigned char *p = (unsigned char *) &usWord;
	//m_bIsLittleEndian = ( p[1] == 0x12 );
	// </mod>
}

CFile::CFile( const CFile &roO )
: m_bIsRead( false )
, m_bIsWrite( false )
, m_bIsTrunc( false )
, m_pFile( 0 )
{
	*this = roO;
}

CFile::~CFile()
{
	Close();
}

CFile & CFile::operator=( const CFile &roO )
{
	// Datei-Deskriptoren auf offene Streams liessen sich kopieren (dup()...)
	// Doch das wird jetzt noch nicht unterstützt.
	m_bIsRead = false;
	m_bIsWrite = false;
	m_bIsWrite = false;
	m_pFile = 0;
	return *this;
}

bool CFile::Open( const char * pcFileName, unsigned int uiFlags )
{
	if( !uiFlags )
		return false;
	
	if( m_pFile )
		Close();
	
	m_bIsRead = ( ( uiFlags & FLAG_READ ) != 0 );
	m_bIsWrite = ( ( uiFlags & FLAG_WRITE ) != 0 );
	m_bIsTrunc = ( ( uiFlags & FLAG_TRUNC ) != 0 );
	
	if( !( m_bIsRead || m_bIsWrite ) )
		return false;
	
	
#if defined( FILE_USE_ANSI )
	static const char acFlagsR_[] = "rb"; // Opens for reading. If the file does not exist or cannot be found, the fopen call fails.
	static const char acFlagsW_[] = "wb"; // Opens an empty file for writing. If the given file exists, its contents are destroyed.
	//static const char acFlagsRp_[] = "r+b"; // Opens for both reading and writing. (The file must exist.)
	static const char acFlagsWp_[] = "w+b"; // Opens an empty file for both reading and writing. If the given file exists, its contents are destroyed.
	static const char acFlagsA_[] = "ab";  // Opens for writing at the end of the file (appending) without removing the EOF marker before writing new data to the file; creates the file first if it doesn't exist.
	
	const char *pcFlags = 0;
	if( m_bIsWrite )
	{
		if( m_bIsTrunc )
		{
			if( m_bIsRead )
				pcFlags = acFlagsWp_;
			else
				pcFlags = acFlagsW_;
		}
		else
		{
			if( CFile::Exist_( pcFileName ) )
				pcFlags = acFlagsA_;
			else
				pcFlags = acFlagsWp_;
		}
	}
	else if( m_bIsRead )
	{
		if( m_bIsTrunc )
			pcFlags = acFlagsWp_;
		else
			pcFlags = acFlagsR_;
	}
	
	FILE_FOPEN( m_pFile, pcFileName, pcFlags );
	return ( m_pFile != 0 );
#elif defined( FILE_USE_WINDOWS )
	DWORD dwDesiredAccess = 0;
	DWORD dwCreationDisposition = 0;
	
	// ACHTUNG: Nicht sehr allgemein!
	if( m_bIsRead )
	{
		dwDesiredAccess |= GENERIC_READ;
		dwCreationDisposition = OPEN_EXISTING;
	}
	if( m_bIsWrite )
	{
		dwDesiredAccess |= GENERIC_WRITE;
		dwCreationDisposition = OPEN_ALWAYS;
	}
	if( m_bIsTrunc )
	{
		dwCreationDisposition = CREATE_ALWAYS;
	}
	
	m_pFile = ::CreateFile( 
		pcFileName, 
		dwDesiredAccess,
		FILE_SHARE_READ, 
		NULL, 
		dwCreationDisposition, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL );
	
	return ( m_pFile != INVALID_HANDLE_VALUE );
#endif // platform
}

unsigned int CFile::GetSize()
{
	if( m_pFile )
#if defined( FILE_USE_ANSI )
	{
		const long lPos = ::ftell( m_pFile );
		::fseek( m_pFile, 0L, SEEK_END );
		const long lEnd = ::ftell( m_pFile );
		::fseek( m_pFile, lPos, SEEK_SET );
		return lEnd;
	}
#elif defined( FILE_USE_WINDOWS )
		return ::GetFileSize( m_pFile, 0 ); // rem: "high-order word" wird ignoriert!
#endif // platform
	return 0;
}

unsigned int CFile::Read( void *pvData, unsigned int uiBytes )
{
	if( !( pvData && uiBytes ) )
		return 0;
	if( m_pFile )
#if defined( FILE_USE_ANSI )
		return (unsigned int)( ::fread( pvData, 1, uiBytes, m_pFile ) );
#elif defined( FILE_USE_WINDOWS )
	{
		DWORD dwBytesRead;
		::ReadFile( m_pFile, pvData, uiBytes, &dwBytesRead, 0 );
		return dwBytesRead;
	}
#endif // platform
	return 0;
}

unsigned int CFile::Write( void *pvData, unsigned int uiBytes )
{
	if( m_pFile )
#if defined( FILE_USE_ANSI )
		return (unsigned int)( ::fwrite( pvData, 1, uiBytes, m_pFile ) );
#elif defined( FILE_USE_WINDOWS )
	{
		DWORD dwBytesWritten = 0;
		::WriteFile( m_pFile, (LPVOID)(pvData), uiBytes, &dwBytesWritten, 0 );
		return dwBytesWritten;
	}
#endif // platform
	return 0;
}

bool CFile::MovePointer( unsigned int uiOffset, unsigned int uiOrigin )
{
	if( m_pFile )
#if defined( FILE_USE_ANSI )
		return ( ::fseek( m_pFile, (long )uiOffset, (int)uiOrigin ) == 0 );
#elif defined( FILE_USE_WINDOWS )
		return ( ::SetFilePointer( m_pFile, uiOffset, 0, uiOrigin ) != INVALID_SET_FILE_POINTER );
#endif // platform
	return false;
}

bool CFile::IsOpen() const
{
	return ( m_pFile != 0 );
}

bool CFile::Close()
{
	if( m_pFile )
	{
#if defined( FILE_USE_ANSI )
		//::fflush( m_pFile );
		::fclose( m_pFile );
		m_pFile = 0;
#elif defined( FILE_USE_WINDOWS )
		::CloseHandle( m_pFile );
		m_pFile = 0;
#endif // platform
		m_bIsRead = m_bIsWrite = m_bIsTrunc = 0;
		return true; 
	}
	return false;
}

bool CFile::Exist_( const char *pcFileName )
{
#if defined( FILE_USE_ANSI )
	FILE *pTmp;
	FILE_FOPEN( pTmp, pcFileName, "rb" );
	if( pTmp )
	{
		::fclose( pTmp );
		return true;
	}
#elif defined( FILE_USE_WINDOWS )
	HANDLE hTmp = ::CreateFile(
		pcFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL );
	if( hTmp && hTmp != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( hTmp );
		return true;
	}
#endif // platform
	return false;
}

bool CFile::IsEmpty_( const char *pcFileName )
{
	if( Exist_( pcFileName ) )
	{
		CFile oFile;
		if( oFile.Open( pcFileName, FLAG_READ ) )
		{
			const bool bIsEmpty( !oFile.GetSize() );
			oFile.Close();
			return bIsEmpty;
		}
		return false;
	}
	return true;
}

bool CFile::IsNewerThan_( const char *pcFileName0, const char *pcFileName1 )
{
#ifdef FILE_USE_ANSI
#ifdef FILE_USE_POSIX_STAT
	struct stat oFileInfo0, oFileInfo1;
	if( stat( pcFileName0, &oFileInfo0 ) != 0
	 || stat( pcFileName1, &oFileInfo1 ) != 0 )
	{
		return false;
	}
	return ( oFileInfo0.st_mtime > oFileInfo1.st_mtime );
#else
	// todo
	return false;
#endif // FILE_USE_POSIX_STAT
#else
	// todo
	return false;
#endif // FILE_USE_ANSI
}

bool CFile::CreateEmpty_( const char *pcFileName )
{
#if defined( FILE_USE_ANSI )
	if( !CFile::Exist_( pcFileName ) )
	{
		FILE *pTmp;
		FILE_FOPEN( pTmp, pcFileName, "w" );
		if( !pTmp )
			return false;
		::fclose( pTmp );
		return true;
	}
	return false;
#elif defined( FILE_USE_WINDOWS )
	SetLastError( 0 );
	HANDLE hTmp = ::CreateFile(
		pcFileName, 
		FILE_ALL_ACCESS,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL );
	const DWORD dwRet = ::GetLastError();
	if( !hTmp || hTmp == INVALID_HANDLE_VALUE ) 
		return false;
	::CloseHandle( hTmp );
	return ( dwRet != ERROR_ALREADY_EXISTS );
#endif // platform
}

void CFile::SwapBytes_( signed short int *v )
{
	unsigned short int r;
	SwapBytesN_( (unsigned char *)&r, (unsigned char *)v, sizeof(signed short int) );
	*v = r;
	//unsigned char *pucV = (unsigned char *)v;
	//*v = ( pucV[0] << 8 ) | ( pucV[1] );
}

void CFile::SwapBytes_( unsigned short int *v )
{
	unsigned short int r;
	SwapBytesN_( (unsigned char *)&r, (unsigned char *)v, sizeof(unsigned short int) );
	*v = r;
	//unsigned char *pucV = (unsigned char *)v;
	//*v = ( pucV[0] << 8 ) | ( pucV[1] );
}

void CFile::SwapBytes_( signed int *v )
{
	signed int r;
	SwapBytesN_( (unsigned char *)&r, (unsigned char *)v, sizeof(signed int) );
	*v = r;
	//unsigned char *pucV = (unsigned char *)v;
	//*v = ( pucV[0] << 24 ) | ( pucV[1] << 16 ) | ( pucV[2] << 8 ) | ( pucV[3] );
}

void CFile::SwapBytes_( unsigned int *v )
{
	unsigned int r;
	SwapBytesN_( (unsigned char *)&r, (unsigned char *)v, sizeof(unsigned int) );
	*v = r; 
	//unsigned char *pucV = (unsigned char *)v;
	//*v = ( pucV[0] << 24 ) | ( pucV[1] << 16 ) | ( pucV[2] << 8 ) | ( pucV[3] );
}

void CFile::SwapBytes_( signed long int *v )
{
	signed long int r;
	SwapBytesN_( (unsigned char *)&r, (unsigned char *)v, sizeof(signed long int) );
	*v = r;
	//unsigned char *pucV = (unsigned char *)v;
	//*v = ( pucV[0] << 24 ) | ( pucV[1] << 16 ) | ( pucV[2] << 8 ) | ( pucV[3] );
	
	/*
	unsigned char *pucV = (unsigned char *)psliVal;
	unsigned int uiByte = sizeof(signed long int);
	unsigned int uiShift = 0;
	signed long int sliRet = 0;
	while( uiByte )
	{
		--uiByte;
		sliRet |= ( pucV[uiByte] << uiShift );
		uiShift += 8;
	}
	*psliVal = sliRet;
	*/
}

void CFile::SwapBytes_( unsigned long int *v )
{
	unsigned long int r;
	SwapBytesN_( (unsigned char *)&r, (unsigned char *)v, sizeof(unsigned long int) );
	*v = r;
	//unsigned char *pucV = (unsigned char *)v;
	//*v = ( pucV[0] << 24 ) | ( pucV[1] << 16 ) | ( pucV[2] << 8 ) | ( pucV[3] );
	
	/*
	unsigned char *pucV = (unsigned char *)v;
	unsigned int uiByte = sizeof(signed long int);
	unsigned int uiShift = 0;
	unsigned long int uliRet = 0;
	while( uiByte )
	{
		--uiByte;
		uliRet |= ( pucV[uiByte] << uiShift );
		uiShift += 8;
	}
	*v = uliRet;
	*/
}

void CFile::SwapBytesN_( unsigned char *pucDst, unsigned char *pucSrc, unsigned int uiN )
{
	while( uiN )
	{
		--uiN;
		*pucDst = pucSrc[uiN];
		++pucDst;
	}
}

// TODO: Test!
unsigned int CFile::VarLen4Read_( unsigned long int *puliDst, unsigned char *pucSrc )
{
	unsigned long int v = 0;
	unsigned int c = 0;
	do
	{
		const unsigned char b = pucSrc[c];
		++c;
		v |= ( b & 0x7F );
		if( !( b & 0x80 ) )
			break;
		v <<= 7;
	}
	while( c < 4 );
	*puliDst = v;
	return c;
}

// TODO: Test!
void CFile::VarLen4Write_( unsigned char *pucDst, unsigned long int uliVal )
{
	unsigned int c = 0;
	do
	{
		pucDst[c] = (unsigned char)( uliVal & 0x7F );
		uliVal >>= 7;
		if( !uliVal )
			break;
		pucDst[c] |= 0x80;
		++c;
	}
	while( c < 4 );
	
	// rem: Overheadish.
	if( c == 4 )
	{
		// Wenn alle vier Bytes beschrieben wurden, das letzte als
		// End-Byte markieren...
		pucDst[3] &= 0x7F;
	}
	else
	{
		// ...andernfalls die verbliebenen Bytes auf 0 setzen.
		while( c < 4 )
		{
			++c; // Zuerst, da oben mit break abgebrochen.
			pucDst[c] = 0;
		}
	}
}

#endif // _FILE_CPP_
