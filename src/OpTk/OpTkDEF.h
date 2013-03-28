// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkDEF.h

#ifndef _OP_TK_DEF_H_
#define _OP_TK_DEF_H_

#define OP_TK_VERSION	"0.07" // 2011-01-19

#ifdef OP_VERSION
#include "../Op/OpDEF.h"
#define OP_TK_MEM_ZERO( dst, count )		OP_MEM_ZERO( dst, count )
#define OP_TK_MEM_COPY( dst, src, count )	OP_MEM_COPY( dst, src, count )
#define OP_TK_MEM_SET( dst, val, count )	OP_MEM_SET( dst, val, count )
#define OP_TK_MIN( a, b )			OP_MIN( a, b )
#define OP_TK_MAX( a, b )			OP_MAX( a, b )
#define OP_TK_LOG_N( a, basis )			OP_LOG_N( a, basis ) // new
#define OP_TK_LOGF_N( a, basis )		OP_LOGF_N( a, basis ) // new
#define OP_TK_RANGE_MINMAX( val, min, max )	OP_RANGE_MINMAX( val, min, max )
#define OP_TK_DELETE_ARRAY( arr )		OP_DELETE_ARRAY( arr )
#else
//#include "../DEF.h"
#include <string.h>
#define OP_TK_MEM_ZERO( dst, count )		memset( dst, 0, count )
#define OP_TK_MEM_COPY( dst, src, count )	memmove( dst, src, count )
#define OP_TK_MEM_SET( dst, val, count )	memset( dst, val, count )
#define OP_TK_MIN( a, b )			(((a) < (b)) ? (a) : (b))
#define OP_TK_MAX( a, b )			(((a) > (b)) ? (a) : (b))
#define OP_TK_LOG_N( a, basis )			( log( double( a ) ) / log( double( basis ) ) ) // new
#define OP_TK_LOGF_N( a, basis )		( logf( float( a ) ) / logf( float( basis ) ) ) // new
#define OP_TK_RANGE_MINMAX( val, min, max )	if( (val) > max ) \
							(val) = (max); \
						else if( (val) < min ) \
							(val) = (min);
#define OP_TK_DELETE_ARRAY( arr )		if( arr ) { \
							delete [] arr; \
							arr = 0; \
						}
#endif // OP_MEM_ZERO

#define OP_TK_PI				3.1415926535897932384626433832795
#define OP_TK_PI_2				1.5707963267948966192313216916398
#define OP_TK_PI2				6.283185307179586476925286766559
#define OP_TK_LN_2				0.69314718055994530941723212145818

#include <math.h>
#include <stdlib.h>

#endif // _OP_TK_DEF_H_
