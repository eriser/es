// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// DEF.h
//
// rem: Bei Verwendung von MinGW muss DEF.o als erste Objektdatei gelinkt
//      werden, da hier die Eintrittsfunktion _Prolog/main definiert ist!
//

#ifndef _DEF_H_
#define _DEF_H_

#ifdef __WINDOWS__
#include <dsound.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#endif // platform

#define APP_NAME			"ES"
#define APP_AUTHOR			"Aurelio Lucchesi"
#define APP_WEBSITE			"http://es.0rel.com/"
#define APP_DATE			__DATE__
#define APP_TIME			__TIME__

#define APP_LANGUAGE_ENGLISH
//#define APP_LANGUAGE_GERMAN

#ifndef _DEBUG
//#define USE_EDIT_PATCH
#endif // _DEBUG
#define USE_ASM
#define COND_NOSTD			( ( !defined( _DEBUG ) || defined( USE_NO_STD ) ) && defined( __GNUC__ ) && defined( __WINDOWS__ ) )
#define COND_SUBSYS_WINDOW		( !defined( _DEBUG ) && !defined( VERSION_RUNTIME ) && defined( __WINDOWS__ ) )

#if !defined( VERSION_RUNTIME ) && defined( _DEBUG )
#define USE_LOG
#endif // VERSION_RUNTIME && _DEBUG

#ifdef VERSION_RUNTIME

// ACHTUNG: Entweder oder...
#define VERSION_RUNTIME_MODE_REALTIME
//#define VERSION_RUNTIME_MODE_RENDER_TO_FILE

#define RUNTIME_FILE_NAME_RENDER	"out.wav"
#define RUNTIME_FILE_NAME_PATCH		"../build/run.patch.txt"
#define RUNTIME_FILE_NAME_DEF		"../../build/run.def.txt"
#define RUNTIME_FILE_NAME_REG		"../../build/run.reg.txt"
#elif defined( USE_EDIT_PATCH )
#define EDIT_FILE_NAME_PATCH		"../build/edit.patch.txt"
#endif // VERSION_RUNTIME


#define PI				3.1415926535897932384626433832795
#define PI2				6.283185307179586476925286766559
#define PI_2				1.5707963267948966192313216916398
#define PI_4				0.78539816339744830961566084581988
#define E				2.7182818284590452353602874713527

#define	AUDIO_SAMPLE_FREQ		44100 // 48000
#ifdef LOW_LATENCY
#define AUDIO_BUFFER_SIZE		2048
#else
#define AUDIO_BUFFER_SIZE		1024 * 8 //4 //16 // 1024 // 2048 //( 1024 * 4 ) // 4096 samples = ~93 ms
#endif // LOW_LATENCY
#define AUDIO_SAMPLE_TIME		( 1.0 / AUDIO_SAMPLE_FREQ )
#define AUDIO_VALUE_MAX			32700 //32767

#define MIN(a, b)			(((a) < (b)) ? (a) : (b))
#define	MAX(a, b)			(((a) > (b)) ? (a) : (b))

// ref: http://www.fefe.de/intof.html
#define __HALF_MAX_SIGNED(type)		((type)1 << (sizeof(type)*8-2))
#define __MAX_SIGNED(type)		(__HALF_MAX_SIGNED(type) - 1 + __HALF_MAX_SIGNED(type))
#define __MIN_SIGNED(type)		(-1 - __MAX_SIGNED(type))

#define __MIN(type)			((type)-1 < 1?__MIN_SIGNED(type):(type)0)
#define __MAX(type)			((type)~__MIN(type))


// ref: https://svn.blender.org/svnroot/bf-blender/trunk/lib/darwin-8.x.i386/sdl/include/SDL_stdinc.h (MOD)

//#ifdef __x86_64__
//typedef ...				tmp_Sint64;
//typedef ...				tmp_Uint64;
//#else
//// This is really just a hack to prevent the compiler from complaining
//typedef struct {
//	unsigned int hi;
//	unsigned int lo;
//} tmp_Uint64, dummy_Sint64;
//#endif

// Make sure the types really have the right sizes
#define ES_COMPILE_TIME_ASSERT(name, x)               \
       typedef int ES_dummy_ ## name[(x) * 2 - 1];

ES_COMPILE_TIME_ASSERT( tmp_uint8, sizeof(char) == 1 )
ES_COMPILE_TIME_ASSERT( tmp_uint8, sizeof(signed char) == 1 )
ES_COMPILE_TIME_ASSERT( tmp_sint8, sizeof(unsigned char) == 1 )
ES_COMPILE_TIME_ASSERT( tmp_uint16, sizeof(short int) == 2 )
ES_COMPILE_TIME_ASSERT( tmp_uint16, sizeof(short signed int) == 2 )
ES_COMPILE_TIME_ASSERT( tmp_sint16, sizeof(short unsigned int) == 2 )
ES_COMPILE_TIME_ASSERT( tmp_uint32, sizeof(int) == 4 )
ES_COMPILE_TIME_ASSERT( tmp_uint32, sizeof(signed int) == 4 )
ES_COMPILE_TIME_ASSERT( tmp_sint32, sizeof(unsigned int) == 4 )
//ES_COMPILE_TIME_ASSERT( uint64, sizeof(tmp_Uint64) == 8 )
//ES_COMPILE_TIME_ASSERT( sint64, sizeof(tmp_Sint64) == 8 )
ES_COMPILE_TIME_ASSERT( tmp_float, sizeof(float) == 4 )
ES_COMPILE_TIME_ASSERT( tmp_double, sizeof(double) == 8 )

// Check to make sure enums are the size of ints, for structure packing.
// For both Watcom C/C++ and Borland C/C++ the compiler option that makes
// enums having the size of an int must be enabled.
// This is "-b" for Borland C/C++ and "-ei" for Watcom C/C++ (v11).

// Enable enums always int in CodeWarrior (for MPW use "-enum int") */
//#ifdef __MWERKS__
//#pragma enumsalwaysint on
//#endif

typedef enum {
	DUMMY_ENUM_VALUE
} ES_DUMMY_ENUM;

ES_COMPILE_TIME_ASSERT( enum, sizeof(ES_DUMMY_ENUM) == sizeof(int) )

//
#include <climits>
#if CHAR_BIT != 8
#error Only supported for 8bit chars
#endif

//

#if defined( __LINUX__ )
#define ISNAN( x )			isnan( x )
#define FINITE( x )			finite( x )
#elif defined( __WINDOWS__ )
#define ISNAN( x )			_isnan( x )
#define FINITE( x )			_finite( x )
#endif // platform

#ifdef USE_LOG
	#include <stdio.h>
#ifdef __LINUX__
	#define LOG(...)		fprintf( stderr, __VA_ARGS__ ) // flush...
#else
	#define LOG(...)		printf( __VA_ARGS__ )
#endif // __LINUX__
#else
#define LOG(...)			{ ; }
#endif // USE_LOG

#ifdef _DEBUG
	#define UNUSED_PARAMETER(x)	x=x
	#define ASSERT(x) 		if( !(x) ) { printf( "ASSERT( %s ) in file %s, line %d\n", #x, __FILE__, __LINE__ ); }
#else
	#define UNUSED_PARAMETER(x)
	#define ASSERT(x)
#endif // _DEBUG


#if !defined( _MSC_VER ) // ANSI C
#define SCANF( buf, fmt, ... )		scanf( fmt, __VA_ARGS__ )
#define SSCANF(	buf, fmt, ... )		sscanf( buf, fmt, __VA_ARGS__ )
#define SPRINTF( buf, size, fmt, ... ) 	sprintf( buf, fmt, __VA_ARGS__ )
#else
#define SCANF( buf, fmt, ... )		scanf( fmt, __VA_ARGS__ )
#define SSCANF(	buf, fmt, ... )		sscanf_s( buf, fmt, __VA_ARGS__ )
#define SPRINTF( buf, size, fmt, ... )	sprintf_s( buf, size, fmt, __VA_ARGS__ )
#endif // !defined( _MSC_VER )

#ifdef __GNUC__
	#define ATTR_ALWAYS_INLINE __attribute__ ((always_inline))
#else
	#define ATTR_ALWAYS_INLINE
#endif

#ifdef USE_ASM
	#ifdef __GNUC__
		#define times3(arg1, arg2) \
			__asm__ ( \
			"leal (%0,%0,2),%0" \
			: "=r" (arg2) \
			: "0" (arg1) );
		#define times5(arg1, arg2) \
			__asm__ ( \
			"leal (%0,%0,4),%0" \
			: "=r" (arg2) \
			: "0" (arg1) );
		#define times9(arg1, arg2) \
			__asm__ ( \
			"leal (%0,%0,8),%0" \
			: "=r" (arg2) \
			: "0" (arg1) );
	#else
		#define times3(arg1, arg2) \
			arg2 = arg1 * 3
		#define times5(arg1, arg2) \
			arg2 = arg1 * 5
		#define times9(arg1, arg2) \
			arg2 = arg1 * 9
	#endif // __GNUC__
#else
	#define times3(arg1, arg2) \
		arg2 = arg1 * 3
	#define times5(arg1, arg2) \
		arg2 = arg1 * 5
	#define times9(arg1, arg2) \
		arg2 = arg1 * 9
#endif // USE_ASM


extern "C" void Main();

#if VERSION_LIB
#include <math.h>
#include <string.h> // new
#include <stdlib.h> // new
#else
#if !COND_NOSTD
#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;

int main( int iArg, char *aaArg[] );
#else // COND_NOSTD

void _Prolog();
void _Entry();
void _Epilog();


extern HANDLE g_hHeap;
#if	defined( __GNUC__ )
#define INIT_DTOR_SIZE	64
typedef void ( *TFuncDTOR )( void );
extern unsigned int g_uiDTORSize;
extern unsigned int g_uiDTORIndex;
extern TFuncDTOR *g_ptDTOR;
#endif // Compiler

void _InitGlobals();

#if	defined( __GNUC__ )

extern "C" void __cxa_pure_virtual( void );
extern "C" int atexit( void (__cdecl *dtor )( void ));

#elif	defined( _MSC_VER )

extern "C" int _fltused;
extern "C" int __cdecl __purecall( void );
extern "C" int __cdecl atexit( void (__cdecl *dtor )( void ));

#else

#error	Compiler not supported with NOSTD.
#endif // Compiler

void * operator new( size_t bytes );
void * operator new[]( size_t bytes );
void operator delete( void *ptr );
void operator delete[]( void *ptr );

#include <math.h>

// Win32 equivalente
#define strcat	lstrcat
#define strcmp	lstrcmp
#define strcpy	lstrcpy
#define strlen	lstrlen

// rem: itoa usw. sind nicht ANSI-konform... sscanf/sprintf!
#endif // COND_NOSTD
#endif // VERSION_LIB

#if defined( _MSC_VER ) || ( !defined( VERSION_LIB ) && !defined( __LINUX__ ) ) // new! (2010-10-02)
inline double __cdecl rint( double x )
{
    if( x > 0.0 )
        return double( (__int64)(x + 0.5) );
    else
	return double( (__int64)(x - 0.5) );
}

inline float __cdecl rintf( float x )
{
    if( x > 0.0f )
        return float( (__int32)(x + 0.5f) );
    else
        return float( (__int32)(x - 0.5f) );
}
#endif // _MSC_VER ...

#if !defined(__cplusplus)
#error C++ compiler required.
#endif

#endif // _DEF_H_
