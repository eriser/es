// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Sys.h

#ifndef _SYS_H_
#define _SYS_H_

#define SYS_VERSION	"0.12" // 2009-06-13
//#define SYS_USE_NET
//#define SYS_USE_TICK_COUNT

#include "../Str/Str.h"
#include "../Array/Array.h"

// System-Wrapper.
////////////////////////////////////////////////////////////////////////////////
class CSys
{
public:
	enum { THREAD_PRIO_NORMAL, THREAD_PRIO_LOW, THREAD_PRIO_HIGH };
	static bool Init_();
	static bool GetCharNonBlock_( char cChar );
	static bool CreateDirectory_( const char* acName );
	static bool GetFilesInDir_( 
		const char* pcDir, const char *pcFile, CArray<CStr> *poArray );
#ifdef SYS_USE_TICK_COUNT
	static long unsigned int GetTickCount_();
#endif // SYS_USE_TICK_COUNT
	static void SetThreadPriority_( unsigned int uiPrio );
	static unsigned long int GetTime1970_01_01_();
	static void Sleep_( unsigned int uiMilliseconds );
	static bool Exectue( const char *pcCmd );
#ifdef SYS_USE_NET
	static bool CheckURL_( const char *pcURL );
	static bool ReadURL_( const char *pcURL, CStr *poContent );
#endif // SYS_USE_NET
};

#endif // _SYS_H_
