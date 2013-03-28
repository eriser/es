// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDEF.h

#ifndef _OP_DEF_H_
#define _OP_DEF_H_

#define OP_VERSION				"0.03" // 2011-01-03

#if !defined( VERSION_RUNTIME ) && !defined( VERSION_LIB )

//#define OP_USE_DEBUG0
#define OP_USE_USER_DATA			// m_pvData-Zeiger in COp
#define OP_USE_RUNTIME_INFO
#define OP_USE_ROUTINES

#if defined( _DEBUG ) || !defined( VERSION_RUNTIME )
#define OP_USE_LOG

#define OP_LOG_LINES				50
#define OP_LOG_CHARS				256
void _Op_LogClear();
const char * _Op_LogGet(  unsigned int uiLine  );
void _Op_LogFlt( const char *pcMsg, double dFlt );
#endif // _DEBUG || !VERSION_RUNTIME

#endif // VERSION_RUNTIME // VERSION_LIB

//#if !defined( _DEBUG )
//#if !defined( VERSION_RUNTIME )
//#ifndef __LINUX__ // tmp
#define OP_USE_MIDI 					// TMP. UNCOMMENT TO ENABLE MIDI UNDER LINUX!
//#cendif // __LINUX__
//#endif // VERSION_RUNTIME
//#endif // _DEBUG

#define OP_USE_INTERNALS

#include "../DEF.h"
#ifdef OP_USE_RUNTIME_INFO
#include "../Array/Array.h"
#include "../Str/Str.h"
#endif // OP_USE_RUNTIME_INFO

#if defined( APP_LANGUAGE_GERMAN )
#define OP_LANGUAGE_GERMAN
#else
#define OP_LANGUAGE_ENGLISH
#endif

#define OP_PI					PI
#define OP_PI2					PI2
#define OP_PI_2					PI_2
#define OP_PI_4					PI_4
#define OP_E					E
#define OP_LN_2					( 0.69314718055994530941723212145818 )
#define OP_SQRT_2_12				( 1.0594630943592952645618252949463 )
#define OP_EPSILON				1e-12

#define	OP_SAMPLE_FREQ				AUDIO_SAMPLE_FREQ
#define OP_SAMPLE_PERIOD			( 1.0 / AUDIO_SAMPLE_FREQ )
#define OP_BUFFER_SIZE				AUDIO_BUFFER_SIZE
#define OP_SAMPLE_TIME				AUDIO_SAMPLE_TIME
#define OP_VALUE_MAX				AUDIO_VALUE_MAX
#define OP_PI2_D_SAMPLE_FREQ			( OP_PI2 / AUDIO_SAMPLE_FREQ )
#define OP_PI_D_SAMPLE_FREQ			( OP_PI / AUDIO_SAMPLE_FREQ )
#define OP_ONE_D_SAMPLE_FREQ			( 1.0 / AUDIO_SAMPLE_FREQ )

#define OP_FREQ_TO_BPM( freq )			( freq * 60.0 )
#define OP_BPM_TO_FREQ( bpm )			( bpm * (1.0 / 60.0) )

#define OP_IS_NAN( val )			ISNAN( (val) )
#define OP_IS_FINITE( val )			FINITE( (val) )
#define OP_IS_VALUE_INVALID( val )		( OP_IS_NAN( val ) || !OP_IS_FINITE( val ) )

#if !defined( VERSION_RUNTIME ) && defined( OP_USE_LOG )
#define		OP_LOG( msg )			LOG( msg )
#define		OP_LOG1( msg, x )		LOG( msg, x )
#define		OP_LOG2( msg, x, y )		LOG( msg, x, y )
#define		OP_LOG_FLT( msg, flt )		_Op_LogFlt( msg, flt )
#else
#define		OP_LOG( msg )
#define		OP_LOG1( msg, x )
#define		OP_LOG2( msg, x, y )
#define		OP_LOG_FLT( msg, flt )
#endif // VERSION_RUNTIME

#define OP_MEM_ZERO( dst, count )		memset( dst, 0, count )
#define OP_MEM_COPY( dst, src, count )		memmove( dst, src, count )
#define OP_MEM_SET( dst, val, count )		memset( dst, val, count )

#define	OP_MAX( a, b )				MAX( a, b )
#define OP_MIN( a, b )				MIN( a, b )

#define OP_LOG_N( a, basis )			( log( double( a ) ) / log( double( basis ) ) ) // new
#define OP_LOGF_N( a, basis )			( logf( float( a ) ) / logf( float( basis ) ) ) // new

#define OP_RANGE_MOD( val, range )		if( val >= range ) \
							val %= range;

#define OP_RANGE_MINMAX( val, min, max )	if( (val) > max ) \
							(val) = (max); \
						else if( (val) < min ) \
							(val) = (min);

#define OP_RANGE_MINMAX_EX( val, min, max, s )	if( (val) >= max ) \
							(val) = (max) - s; \
						else if( (val) <= min ) \
							(val) = (min) + s;

#define OP_SWAP( v0, v1, type )			{ const type tmp( v0 ); v0 = v1; v1 = tmp; }

#define OP_DELETE_ARRAY( arr )			if( arr )  { delete [] arr; arr = 0; }

#ifdef OP_USE_RUNTIME_INFO
#define OP_SET_NAME_INSTANCE( name ) \
	m_oStrNameInstance = name

#define OP_SET_NAME_AND_INFO_INPUT( idx, name, info ) \
	SetNameInput( idx, name ); \
	SetInfoInput( idx, info )

#define OP_SET_NAME_AND_INFO_OUTPUT( idx, name, info ) \
	SetNameOutput( idx, name ); \
	SetInfoOutput( idx, info )

#define OP_SET_NAME_AND_INFO_CLASS( class, info ) \
	const char class::m_acNameClass_[] = #class; \
	const char class::m_acInfoClass_[] = info;

#ifdef OP_USE_ROUTINES
#define OP_SET_COUNT_ROUTINE( size ) \
	SetCountRoutine( size )

#define OP_SET_NAME_AND_INFO_ROUTINE( idx, name, info ) \
	SetNameRoutine( idx, name );  \
	SetInfoRoutine( idx, info )
#endif // OP_USE_ROUTINES
#else
#define OP_SET_NAME_INSTANCE( name )
#define OP_SET_NAME_AND_INFO_INPUT( idx, name, info )
#define OP_SET_NAME_AND_INFO_OUTPUT( idx, name, info )
#define	OP_SET_NAME_AND_INFO_CLASS( class, name )
#define OP_SET_COUNT_ROUTINE( size )
#define OP_SET_NAME_AND_INFO_ROUTINE( idx, name, info )
#endif // OP_USE_RUNTIME_INFO

#ifdef OP_USE_INTERNALS
#define OP_SET_COUNT_INTERNAL_INT( size )	SetCountInternalInt( size )
#define OP_SET_PTR_INTERNAL_INT( idx, val )	SetPtrInternalInt( idx, val )
#define OP_SET_COUNT_INTERNAL_UINT( size )	SetCountInternalUInt( size )
#define OP_SET_PTR_INTERNAL_UINT( idx, val )	SetPtrInternalUInt( idx, val )
#define OP_SET_COUNT_INTERNAL_FLT( size )	SetCountInternalFlt( size )
#define OP_SET_PTR_INTERNAL_FLT( idx, val )	SetPtrInternalFlt( idx, val )
#else
#define OP_SET_COUNT_INTERNAL_INT( size )
#define OP_SET_PTR_INTERNAL_INT( idx, val, info )
#define OP_SET_COUNT_INTERNAL_UINT( size )
#define OP_SET_PTR_INTERNAL_UINT( idx, val, info )
#define OP_SET_COUNT_INTERNAL_FLT( size )
#define OP_SET_PTR_INTERNAL_FLT( idx, val, info )
#endif // OP_USE_INTERNALS


#define OP_FLAG_NONE				0
#define OP_FLAG_DYNAMIC_INPUTS			1	// Die Anzahl Inputs kann sich zu Laufzeit ändern.
#define OP_FLAG_DYNAMIC_OUTPUTS			2	// Die Anzahl Outputs kann sich zu Laufzeit ändern.
#define OP_FLAG_SHOW_OUTPUTS			4	// Anzeige der Outputs erlauben.
#define OP_FLAG_COMPACT				8	// Kompakte Anzeige für spezielle Operatoren.
#define OP_FLAG_COMBINATIVE			16	// Der Operator dient oft dazu, andere Signale zu
							// kombinieren und verlangt deshalb nicht nach Konstanten
							// auf Input-Seite.
#define OP_FLAG_SINGLETON			32	// Es gibt nur einen.

#define OP_LINK_FLAG_NONE			0
#define OP_LINK_FLAG_OPTIONAL			1

#define OP_STR_OPTIONAL_INPUT			"'"
#define OP_STR_ROUTINE				"()"
#define OP_STR_ROUTINE_DUMMY			"..."

// Compilation Control
////////////////////////////////////////////////////////////////////////////////
#if !defined( VERSION_RUNTIME ) && !defined( VERSION_LITE )

#define OP_INC_OP_ACCUM
#define OP_INC_OP_ADD
#define OP_INC_OP_AMU
#define OP_INC_OP_BIT
#define OP_INC_OP_BIT_SPLIT
#define OP_INC_OP_BUFFER
#define OP_INC_OP_CHORUS
#define OP_INC_OP_CLIP
#define OP_INC_OP_CMP
#define OP_INC_OP_COMBINER
#define OP_INC_OP_COMPRESS
#define OP_INC_OP_COMPRESS_EX
#define OP_INC_OP_CONVERTER
#define OP_INC_OP_DELAY
#define OP_INC_OP_DEMUX
#define OP_INC_OP_DETECT
#define OP_INC_OP_DIFF
#define OP_INC_OP_DISTORT
#define OP_INC_OP_DIV
#define OP_INC_OP_ENV_1_BZR
#define OP_INC_OP_ENV_1_LIN
#define OP_INC_OP_ENV_1_EXP
#define OP_INC_OP_ENV_1_SIN
#define OP_INC_OP_ENV_2_BZR
#define OP_INC_OP_ENV_2_EXP
#define OP_INC_OP_ENV_2_LIN
#define OP_INC_OP_ENV_2_SIN
#define OP_INC_OP_ENV_ADSR
#define OP_INC_OP_ENV_GLIDER
#define OP_INC_OP_EVENT_TABLE
#define OP_INC_OP_FADER
#define OP_INC_OP_FILE_WAV_IN
#define OP_INC_OP_FILE_WAV_OUT
#define OP_INC_OP_FLANGER
#define OP_INC_OP_FLT_ALLPASS
#define OP_INC_OP_FLT_ALLPASS_1
#define OP_INC_OP_FLT_ALLPASS_2
#define OP_INC_OP_FLT_BIQUAD
#define OP_INC_OP_FLT_COMB
#define OP_INC_OP_FLT_EQ_3
#define OP_INC_OP_FLT_F_BACK
#define OP_INC_OP_FLT_F_FORW
#define OP_INC_OP_FLT_FFT
#define OP_INC_OP_FLT_HIGH_LOW
#define OP_INC_OP_FLT_LOW_0
#define OP_INC_OP_FLT_MOOG
#define OP_INC_OP_FLT_RES_0
#define OP_INC_OP_FLT_TB303
#define OP_INC_OP_GATE
#define OP_INC_OP_IF
#define OP_INC_OP_INTER
#define OP_INC_OP_LADDER
#define OP_INC_OP_LOGIC
#define OP_INC_OP_LOOP
#define OP_INC_OP_MAD
#define OP_INC_OP_MATH_ARCUS
#define OP_INC_OP_MATH_EXP
#define OP_INC_OP_MATH_FMOD
#define OP_INC_OP_MATH_LOG
#define OP_INC_OP_MATH_MODF
#define OP_INC_OP_MATH_NUM
#define OP_INC_OP_MATH_POW
#define OP_INC_OP_MATH_SQRT
#define OP_INC_OP_MATH_TRIGO
#define OP_INC_OP_MUL
#define OP_INC_OP_MIX
#define OP_INC_OP_NOP
#define OP_INC_OP_OSC_APPROX
#define OP_INC_OP_OSC_CIRCLE
#define OP_INC_OP_OSC_LUT
#define OP_INC_OP_OSC_PLS
#define OP_INC_OP_OSC_SAW
#define OP_INC_OP_OSC_SIN
#define OP_INC_OP_OSC_TRI
#define OP_INC_OP_OSC_VAR
#define OP_INC_OP_PERC_DRUM
#define OP_INC_OP_PHASER
#define OP_INC_OP_REDUCER
#define OP_INC_OP_RETRIGGER
#define OP_INC_OP_REVERB
#define OP_INC_OP_REVERB_AL
#define OP_INC_OP_ROUTER
#define OP_INC_OP_SAMPLER
#define OP_INC_OP_SCALE
#define OP_INC_OP_SELECTOR
#define OP_INC_OP_SEQ_AAS
#define OP_INC_OP_SEQ_CELL_1D
#define OP_INC_OP_SEQ_COLL_1D
#define OP_INC_OP_SEQ_FIBO
#define OP_INC_OP_SEQ_LOG
#define OP_INC_OP_SEQ_MANDEL
#define OP_INC_OP_SEQ_PRIME
#define OP_INC_OP_SEQ_RAND
#define OP_INC_OP_SEQ_RECA
#define OP_INC_OP_SH
#define OP_INC_OP_SHAPER
#define OP_INC_OP_STEP
#define OP_INC_OP_STO
#define OP_INC_OP_SUB
#define OP_INC_OP_SWITCH
#define OP_INC_OP_SYS
#define OP_INC_OP_TIME
#define OP_INC_OP_TRANS_FFT
#define OP_INC_OP_TRIGGER
#define OP_INC_OP_VALUE

#define OP_INC_OP_MIDI_FILE_IN
#define OP_INC_OP_MIDI_IN
#define OP_INC_OP_MIDI_IN_KEY

#else // VERSION_RUNTIME / VERSION_LITE

#include RUNTIME_FILE_NAME_DEF // ... ptch.exe

#endif // VERSION_RUNTIME / VERSION_LITE

#if defined( OP_USE_MIDI )
#define OP_INC_OP_MIDI_DEV_IN
#define OP_INC_OP_MIDI_DEV_OUT
#define OP_INC_OP_MIDI_OUT
#endif // OP_USE_MIDI

//#if defined( OP_INC_OP_MIDI_IN ) || defined( OP_INC_OP_MIDI_IN_KEY )
//#define OP_INC_OP_MIDI_DEV_IN
//#endif // MIDI_IN Classes

////////////////////////////////////////////////////////////////////////////////

#endif // _OP_DEF_H_
