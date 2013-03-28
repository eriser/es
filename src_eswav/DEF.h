// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// DEF.h

#ifndef _DEF_H_
#define _DEF_H_

#define APP_NAME			"eswav"
#define APP_VERSION			"0.03" // 2010-12-10
#define APP_AUTHOR			"alucch"
#define APP_DATE			__DATE__

#define AUDIO_SAMPLE_FREQ		44100
#define AUDIO_BUFFER_SIZE		32768 //4096 // 2048
#define AUDIO_VALUE_MAX			32700 //32767

#define MIN(a, b)			(((a) < (b)) ? (a) : (b))
#define	MAX(a, b)			(((a) > (b)) ? (a) : (b)) 
#define ABS(v)				(  (v) < 0   ? -(v):+(v) )

#define SQRT2				( 1.4142135623730950488016887242097 )
#define SQRT3				( 1.7320508075688772935274463415059 )

#if 1 // defined( _DEBUG )
	#include <stdio.h>
	#define LOG(...)		{ printf( __VA_ARGS__ ); }
	#define ERR(...)		{ fprintf( stderr, "error: " __VA_ARGS__ ); }
#else
	#define LOG(...)		{ ; }
	#define ERR(...)		{ ; }
#endif // USE_LOG

#ifdef _DEBUG
	#define UNUSED_PARAMETER(x)	x=x
	#define ASSERT(x) 		if( !(x) ) { printf( "ASSERT( %s ) in file %s, line %d\n", #x, __FILE__, __LINE__ ); }
#else
	#define UNUSED_PARAMETER(x)
	#define ASSERT(x)
#endif // _DEBUG

#define SWAP(t,a,b) \
	{ \
		t p = (a); \
		(a) = (b); \
		(b) = p; \
	}

#define WRAP_ZERO(v,max) \
	if( (v) >= (max) ) \
		(v) %= (max); \
	else if( (v) < 0 ) \
		(v) = (max) + ( (v) % (max) );

#define CLAMP(v,min,max) \
	if( (v) > (max) ) \
		(v) = (max); \
	else if( (v) < (min) ) \
		(v) = (min);

#define DELETE_INSTANCE( var )		{ if( var ) { delete var; var = 0; } }
#define DELETE_ARRAY( var )		{ if( var ) { delete [] var; var = 0; } }

#define MEM_ZERO( dst, size )		ZeroMemory( dst, size )
#define MEM_COPY( dst, src, size )	CopyMemory( dst, src, size )
#define MEM_SET( dst, val, size )	FillMemory( dst, val, size )

#ifdef _MSC_VER
#define SPRINTF(dst,bytes,format,...)	sprintf_s(dst,bytes,format,__VA_ARGS__)
#define SSCANF(dst,format,...)		sscanf(dst,format,__VA_ARGS__)
#else
#define SPRINTF(dst,bytes,format,...)	sprintf(dst,format,__VA_ARGS__)
#define SSCANF(dst,format,...)		sscanf(dst,format,__VA_ARGS__)
#endif // _MSC_VER

#if defined( __UNIX__ )
#define ISNAN( x )			isnan( x )
#define FINITE( x )			finite( x )
#elif defined( __WINDOWS__ )
#define ISNAN( x )			_isnan( x )
#define FINITE( x )			_finite( x )
#endif // platform

#endif // _DEF_H_
