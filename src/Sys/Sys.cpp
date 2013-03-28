// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Sys.cpp

#include "Sys.h"
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// CSys
////////////////////////////////////////////////////////////////////////////////

#ifdef __WINDOWS__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>
#include <mmsystem.h>

bool CSys::Init_()
{
	::timeBeginPeriod( 1 ); // rem: Für Sleep().
	//::timeEndPeriod(1); // Wird hier ausgelassen...
	return true;
}

bool CSys::GetCharNonBlock_( char cChar ) {
	return ( GetAsyncKeyState( LOWORD( VkKeyScan( cChar ) ) ) < 0 );
}

bool CSys::CreateDirectory_( const char* acName )
{
	if( !acName )
		return false;
	return ( ::CreateDirectory( LPCTSTR( acName ), NULL ) > 0 );
}

bool CSys::GetFilesInDir_( const char* pcDir, const char *pcFile, CArray<CStr> *poArray )
{
	WIN32_FIND_DATA oFD;
	CStr oDir( pcDir );
	const unsigned int uiLen( oDir.GetSize() );
	if( !uiLen )
		return false;
	if( oDir[uiLen-1] != '/' )
		oDir += '/';
	CStr oMask( oDir + pcFile );
	HANDLE hFind = FindFirstFile( oMask.GetData(), &oFD );
	if( hFind == INVALID_HANDLE_VALUE )
		return false;
	do
	{
		poArray->Append( oDir + CStr( oFD.cFileName ) );
	}
	while( FindNextFile( hFind, &oFD ) != 0 );
	const DWORD dwError = GetLastError();
	FindClose( hFind );
	return ( dwError == ERROR_NO_MORE_FILES );
}

#ifdef SYS_USE_TICK_COUNT
long unsigned int CSys::GetTickCount_()
{
	return ::GetTickCount();
}
#endif // SYS_USE_TICK_COUNT

void CSys::SetThreadPriority_( unsigned int uiPrio )
{
	switch( uiPrio )
	{
	case THREAD_PRIO_LOW:
		::SetThreadPriority( GetCurrentThread(),  THREAD_PRIORITY_LOWEST ); // THREAD_PRIORITY_BELOW_NORMAL
	break;
	case THREAD_PRIO_NORMAL:
		::SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
	break;
	case THREAD_PRIO_HIGH:
		::SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST ); // THREAD_PRIORITY_ABOVE_NORMAL
	break;
	}
}

unsigned long int CSys::GetTime1970_01_01_()
{
	return (unsigned long int)::time( 0 );
}

void CSys::Sleep_( unsigned int uiMilliseconds )
{
	::Sleep( uiMilliseconds );
}

bool CSys::Exectue( const char *pcCmd )
{
	CStr oCmd( pcCmd );
	oCmd.Replace( '/', '\\' );
	const int iStatus = ::system( oCmd.GetData() );
	return ( !iStatus );
}

#ifdef SYS_USE_NET

#ifdef _MSC_VER
#pragma comment( lib, "wininet.lib" )
#endif // _MS_VER

#include <wininet.h>

#ifndef FLAG_ICC_FORCE_CONNECTION
#define FLAG_ICC_FORCE_CONNECTION       0x00000001
#endif // FLAG_ICC_FORCE_CONNECTION

bool CSys::CheckURL_( const char *pcURL )
{
	return ( InternetCheckConnection( pcURL, FLAG_ICC_FORCE_CONNECTION, 0 ) == TRUE );
}

bool CSys::ReadURL_( const char *pcURL, CStr *poContent )
{
	HINTERNET hINet, hFile;
	hINet = InternetOpen( "InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
	if ( !hINet )
		return false;
	hFile = InternetOpenUrl( hINet, pcURL, NULL, 0, INTERNET_FLAG_RELOAD, 0 ); // new: INTERNET_FLAG_RELOAD
	if ( hFile )
	{
		CHAR buffer[1025];
		DWORD dwRead;
		poContent->Clear();
		while( InternetReadFile( hFile, buffer, sizeof(buffer)-1, &dwRead ) )
		{
			if( !dwRead  )
				break;
			buffer[dwRead] = 0;
			poContent->Append( buffer );
		}
		InternetCloseHandle( hFile );
	}
	InternetCloseHandle( hINet );
	return true;
}

#endif // SYS_USE_NET

#elif __LINUX__

//#include "../Util/Util.h" // CUtil::WildcardMatch_()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef SYS_USE_TICK_COUNT
#include <sys/sysinfo.h>
#endif // SYS_USE_TICK_COUNT
#include <sys/select.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

static int kbhit_()
{
	struct timeval tv;
	fd_set read_fd;

	// Do not wait at all, not even a microsecond
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	// Must be done first to initialize read_fd
	FD_ZERO( &read_fd );

	// Makes select() ask if input is ready:
	// 0 is the file descriptor for stdin
	FD_SET( 0, &read_fd );

	// The first parameter is the number of the
	// largest file descriptor to check + 1.
	if( select( 1, &read_fd, NULL, NULL, &tv ) == -1 )
		return 0; // An error occured

	// read_fd now holds a bit map of files that are
	// readable. We test the entry for the standard
	// input (file 0).
	if(FD_ISSET(0,&read_fd))
		// Character pending on stdin
		return 1;

	// no characters were pending
	return 0;
}

// http://www.ale.org/archive/ale/ale-2000-07/msg00442.html
static int set_raw_mode_(void)
{
	int fd = STDIN_FILENO;
	struct termios t;

	if (tcgetattr(fd, &t) < 0)
	{
		//perror("tcgetattr");
		return -1;
	}

	t.c_lflag &= ~ICANON;

	if (tcsetattr(fd, TCSANOW, &t) < 0)
	{
		//perror("tcsetattr");
		return -1;
	}

	setbuf(stdin, NULL);

	return 0;
}

bool CSys::Init_() {
	return ( set_raw_mode_() == 0 );
}

bool CSys::GetCharNonBlock_( char cChar )
{
	if( kbhit_() )
	{
		int ch = getchar();
		return ( ch != EOF && ch == cChar );
	}
	return false;
}

bool CSys::CreateDirectory_( const char* acName )
{
	if( !acName )
		return false;
	return ( ::mkdir( acName, 0 ) > 0 );
}

// http://www.gamedev.net/community/forums/topic.asp?topic_id=132523
// http://ubuntuforums.org/archive/index.php/t-659718.html
bool CSys::GetFilesInDir_( const char* pcDir, const char *pcFile, CArray<CStr> *poArray )
{
	CStr oDir( pcDir );
	CArray<CStr> oArrToken;
	
	if( oDir[oDir.GetSize() - 1] != '/' )
		oDir += '/';
	
	DIR * poDir = ::opendir( pcDir );
	if( !poDir )
	{
		::fprintf( stderr, "Unable to open the directory.\n" );
		return false;
	}
	
	struct dirent *d = 0;
	while( ( d = ::readdir( poDir ) ) != 0 )
	{
		CStr oName( d->d_name );
		if ( oName == "." || oName == ".." ) 
			continue;
		if ( d->d_type == 4 ) // skip directories
			continue;
		
		const char *wild = pcFile;
		const char *string = oName.GetData();
		//static int wild_test( const char *wild, const char *string )
		{
			const char *cp = NULL, *mp = NULL;

			while( (*string) && (*wild != '*'))
			{
				if( ( *wild != *string ) && (*wild != '?') )
				{
					return 0;
				}
				++wild;
				++string;
			}
			while (*string)
			{
				if (*wild == '*') {
					if (!*++wild) {
						return 1;
				}
				mp = wild;
				cp = string+1;
				} else if ((*wild == *string) || (*wild == '?')) {
					wild++;
				string++;
				} else {
					wild = mp;
					string = cp++;
				}
			}

			while (*wild == '*') {
				++wild;
			}
			
			//return !*wild;
			if( !*wild )
				poArray->Append( oDir + oName );
		}
		
		/*
		if( CUtil::WildcardMatch_( pcFile, oName.GetData() ) )
			poArray->Append( oDir + oName );
		*/
	}
	
	::closedir( poDir );
	return true;
}

/*
// more polite version, but only with * wildcars
bool CSys::GetFilesInDir_( const char* pcDir, const char *pcFile, CArray<CStr> *poArray )
{
	CStr oDir( pcDir );
	CStr oFileNameMask( pcFile );
	CArray<CStr> oArrToken;
	
	if( oDir[oDir.GetSize() - 1] != '/' )
		oDir += '/';
	
	// getWildCardTokens
	char *pcTok = strtok( (char *)oFileNameMask.GetData(), "*" );
	while ( pcTok != NULL )
	{
		//fprintf( stderr, "tok: %s\n", pcTok );
		oArrToken.Append( pcTok );
		pcTok = strtok( NULL, "*" );
	}
	
	// getAllFiles
	DIR * poDir = ::opendir( pcDir );

	// If the directory specified in opendir cannot be opened, 
	// it will return NULL.
	if( !poDir )
	{
		::fprintf( stderr, "Unable to open the directory.\n" );
		return false;
	}

	// Now, we'll read in all the files and directories in this 
	// current directory and then print them to stdout.
	struct dirent *d = 0;
	while( ( d = ::readdir( poDir ) ) != 0 )
	{
		CStr oName( d->d_name );
		if ( oName == "." || oName == ".." ) // skip these
			continue;
		if ( d->d_type == 4 )
		{
			// found a directory; recurse into it.
			//const CStr oPath( oDir + oName );
			//GetAllFiles( poArray, oPath, oFileNameMask );
			continue;
		}
		
		bool matches = true;

		// found a file; check if all of the wildcard tokens exist in its name.
		// search linearly thru the file name (i.e. continue where the last token
		// is found).
		int iPos = 0;
		for( unsigned int i = 0; i < oArrToken.GetSize(); ++i )
		{
			if( ( iPos = oName.Find( iPos, oArrToken[i] ) ) < 0 )
			{
				matches = false;
				break;
			}
		}
		if ( matches )
		{
			poArray->Append( oDir + oName );
		}
	}
	
	::closedir( poDir );
	
	return true;
}
*/

#ifdef SYS_USE_TICK_COUNT
long unsigned int CSys::GetTickCount_() // totest
{
	struct sysinfo si;
	if( sysinfo(&si) == 0 )
		return si.uptime;
	else
		return 0L;
}
#endif // SYS_USE_TICK_COUNT

void CSys::SetThreadPriority_( unsigned int uiPrio )
{
	// todo
	switch( uiPrio )
	{
	case THREAD_PRIO_LOW:
	break;
	case THREAD_PRIO_NORMAL:
	break;
	case THREAD_PRIO_HIGH:
	break;
	}
}

unsigned long int CSys::GetTime1970_01_01_()
{
	return time( 0 );
}

void CSys::Sleep_( unsigned int uiMilliseconds )
{
	usleep( uiMilliseconds * 1000 ); // totest
}

bool CSys::Exectue( const char *pcCmd )
{
	const int iStatus = ::system( pcCmd );
	return ( !iStatus );
}

#else
#error platform not supported
#endif // platform
