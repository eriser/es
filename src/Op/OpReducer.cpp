// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpReducer.cpp

#include "OpReducer.h"
#ifdef OP_INC_OP_REDUCER

////////////////////////////////////////////////////////////////////////////////
// COpReducer
////////////////////////////////////////////////////////////////////////////////

#define OP_RED_IN_SRC		0
#define OP_RED_IN_STEP		1
#define OP_RED_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_RED_IN_SRC_INFO	"input"
#define OP_RED_IN_RES_INFO	"step width"
#define OP_RED_OUT_DST_INFO	"output"
#define OP_RED_CLASS_INFO \
\
"Reducer\n" \
"- The input signal will be output with a reduced sample rate.\n" \
"- The step width defines the smallest possible difference between\n" \
"  two successive input values.\n" \
"- The reduction happens towards zero, so that the resulting sample\n" \
"  value always has a smaller absolute value than the original one."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_RED_IN_SRC_INFO	"Eingang"
#define OP_RED_IN_RES_INFO	"Schrittweite"
#define OP_RED_OUT_DST_INFO	"Ausgang"
#define OP_RED_CLASS_INFO \
\
"Reduktion\n" \
"- Das Eingangssignal wird mit einer verminderten Sample-Auflösung\n" \
"  ausgegebnen.\n" \
"- Die Schrittweite bestimmt den kleinst möglichen Unterschied zwischen\n" \
"  zwei Samples.\n" \
"- Sample-Werte werden immer auf die betragsmässig niederere Stufe hin\n" \
"  reduziert."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpReducer::COpReducer()
: COp( 2, 1 )
{
	OP_SET_NAME_INSTANCE( "reduc" );
	OP_SET_NAME_AND_INFO_INPUT( OP_RED_IN_SRC, "i", OP_RED_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_RED_IN_STEP, "s", OP_RED_IN_RES_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_RED_OUT_DST, "o", OP_RED_OUT_DST_INFO );
	SetFlags( OP_FLAG_COMBINATIVE );
	Validate();
}

COpReducer::~COpReducer()
{}

void COpReducer::Proc()
{	
	OP_GENERIC_CODE_PROC_PROLOG();
	
	double dValue = m_poIn[OP_RED_IN_SRC].Proc();
	const double dStep = m_poIn[OP_RED_IN_STEP].Proc();
	if( dStep )
		dValue -= fmod( dValue, dStep );
	m_pdOut[OP_RED_OUT_DST] = dValue;
}

OP_SET_NAME_AND_INFO_CLASS( COpReducer, OP_RED_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpReducer )
OP_GENERIC_METHODS_DEF( COpReducer )

#endif // OP_INC_OP_REDUCER
