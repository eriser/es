// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDistort.cpp

#include "OpDistort.h"
#ifdef OP_INC_OP_DISTORT

////////////////////////////////////////////////////////////////////////////////
// COpDistort
////////////////////////////////////////////////////////////////////////////////

#define OP_DST_IN_SRC		0
#define OP_DST_IN_COEFF		1
#define OP_DST_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_DST_IN_SRC_INFO	"input"
#define OP_DST_IN_COEFF_INFO	"coefficient"
#define OP_DST_OUT_DST_INFO	"output"
#define OP_DST_CLASS_INFO \
\
"Distortion\n" \
"- The signal becomes distorted."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_DST_IN_SRC_INFO	"Eingang"
#define OP_DST_IN_COEFF_INFO	"Koeffizient"
#define OP_DST_OUT_DST_INFO	"Ausgang"
#define OP_DST_CLASS_INFO \
\
"Verzerrer\n" \
"- Das Signal wird verzerrt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpDistort::COpDistort()
: COp( 2, 1 )
{
	OP_SET_NAME_INSTANCE( "distr" );
	OP_SET_NAME_AND_INFO_INPUT( OP_DST_IN_SRC, "i", OP_DST_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_DST_IN_COEFF, "c", OP_DST_IN_COEFF_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_DST_OUT_DST, "o", OP_DST_OUT_DST_INFO );
	Validate();
}

COpDistort::~COpDistort() 
{}

void COpDistort::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	const double a = m_poIn[OP_DST_IN_COEFF].Proc();
	const double x = m_poIn[OP_DST_IN_SRC].Proc();
	m_pdOut[OP_DST_OUT_DST] = x * ( fabs(x) + a ) / ( x * x + ( a - 1.0 )* fabs(x) + 1.0 );
}

OP_SET_NAME_AND_INFO_CLASS( COpDistort, OP_DST_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpDistort )
OP_GENERIC_METHODS_DEF( COpDistort )

#endif // OP_INC_OP_DISTORT
