// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// File.h

#ifndef _FILE_H_
#define _FILE_H_

#define FILE_USE_ANSI
#define FILE_VERSION				"0.22" // 2010-12-30
//#define FILE_USE_POSIX_STAT

#ifdef __LINUX__
#define FILE_USE_ANSI
#endif // __LINUX__

#if defined( FILE_USE_ANSI )
#	include <stdio.h>
#	include <string.h>
#	ifdef FILE_USE_POSIX_STAT
//#		include <sys/types.h>
#		include <sys/stat.h>
#	 endif // FILE_USE_POSIX_STAT
#elif defined( __WINDOWS__ )
#	define FILE_USE_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#else
#	error platform not supported
#endif // platform

#if defined( FILE_USE_ANSI )
#define FILE_MEM_COPY(dst,src,bytes)		::memcpy(dst,src,bytes)
#define FILE_MEM_SET(dst,val,bytes)		::memset(dst,val,bytes)
#define FILE_MEM_ZERO(dst,bytes)		::memset(dst,0,bytes)
#elif defined( FILE_USE_WINDOWS )
#define FILE_MEM_COPY(dst,src,bytes)		::CopyMemory(dst,src,bytes)
#define FILE_MEM_SET(dst,val,bytes)		::FillMemory(dst,bytes,val)
#define FILE_MEM_ZERO(dst,bytes)		::ZeroMemory(dst,bytes)
#endif // platform

#ifdef _MSC_VER
#define FILE_FOPEN( file, ... )			::fopen_s( &file, __VA_ARGS__ )
#else
#define FILE_FOPEN( file, ... )			file = ::fopen( __VA_ARGS__ )
#endif // _MSVC


// Datei.
//
// - Wird der Pointer verschoben, und dann geschrieben, werden wird der
//   Inhalt überschrieben, und nicht eingefügt.
// - Datei ist kann für Binär- als auch für Text-Daten verwendet werden.
// - CFile ist selbst eine generische Datei, und kann z.B. für reinen Text
//   verwendet werden.
// - Datei-Pointer sind unsigned int. Maximale Grösse: 4 GB.
////////////////////////////////////////////////////////////////////////////////
class CFile
{
public:
	enum { FLAG_READ=1, FLAG_WRITE=2, FLAG_TRUNC=4 };

#if defined( FILE_USE_ANSI )
	enum { PTR_BEG =	SEEK_SET,
		PTR_END =	SEEK_END,
		PTR_CURR =	SEEK_CUR };
#elif defined( FILE_USE_WINDOWS )
	enum {	PTR_BEG =	FILE_BEGIN,
		PTR_END =	FILE_END,
		PTR_CURR =	FILE_CURRENT };
#endif // platform

public:
	CFile();
	CFile( const CFile &roO );
	~CFile();
	CFile & operator=( const CFile &roO );
public:
	bool Open( const char * pcFileName, unsigned int uiFlags );
	unsigned int GetSize();
	unsigned int Read( void *pvData, unsigned int uiBytes );
	unsigned int Write( void *pvData, unsigned int uiBytes );
	bool MovePointer( unsigned int uiOffset, unsigned int uiOrigin );
	bool Close();
	bool IsOpen() const;
public:
	static bool Exist_( const char *pcFileName );
	static bool IsEmpty_( const char *pcFileName );
	static bool IsNewerThan_( const char *pcFileName0, const char *pcFileName1 );
	static bool CreateEmpty_( const char *pcFileName ); // Gibt false zurück, wenn Datei bereits existiert.
	
	// <mod date="2010-12-09">
	template<class T>
	inline T ForceLittleEndian_( T i )
	{
		if( m_bIsLittleEndian_ )
			return i;
		SwapBytes_( &i );
		return i;
	}
	template<class T>
	inline T ForceBigEndian_( T i )
	{
		if( m_bIsLittleEndian_ )
		{
			SwapBytes_( &i );
			return i;
		}
		return i;
	}
	// </mod>
protected:
	static void SwapBytes_( signed short int *v );
	static void SwapBytes_( unsigned short int *v );
	static void SwapBytes_( signed int *v );
	static void SwapBytes_( unsigned int *v );
	static void SwapBytes_( signed long int *v );
	static void SwapBytes_( unsigned long int *v );
	static unsigned int VarLen4Read_( unsigned long int *puliDst, unsigned char *pucSrc );
	static void VarLen4Write_( unsigned char *pucDst, unsigned long int uliVal );
private:
	static void SwapBytesN_( unsigned char *pucDst, unsigned char *pucSrc, unsigned int uiN );
protected:
	bool m_bIsRead;
	bool m_bIsWrite;
	bool m_bIsTrunc;
	// <mod date="2010-12-09">
	static bool m_bIsLittleEndian_;
	// </mod>
private:
#if defined( FILE_USE_ANSI )
	FILE *m_pFile;
#elif defined( FILE_USE_WINDOWS )
	HANDLE m_pFile;
#endif // platform
};

#endif // _FILE_H_
