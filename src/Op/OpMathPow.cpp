// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathPow.cpp

#include "OpMathPow.h"
#ifdef OP_INC_OP_MATH_POW

////////////////////////////////////////////////////////////////////////////////
// COpMathPow
////////////////////////////////////////////////////////////////////////////////

#define OP_MPW_IN_BASE		0
#define OP_MPW_IN_POW		1
#define OP_MPW_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MPW_IN_BASE_INFO	"base"
#define OP_MPW_IN_POW_INFO	"exponent"
#define OP_MPW_OUT_DST_INFO	"ouput"
#define OP_MPW_CLASS_INFO \
\
"Power Function\n" \
"- output = base ^ exponent.\n"

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MPW_IN_BASE_INFO	"Basis"
#define OP_MPW_IN_POW_INFO	"Exponent"
#define OP_MPW_OUT_DST_INFO	"Ausgang"
#define OP_MPW_CLASS_INFO \
\
"Potenz-Funktion\n" \
"- Es gilt: Ausgang = Basis ^ Exponent.\n"

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMathPow::COpMathPow()
: COp( 2, 1 )
{
	OP_SET_NAME_INSTANCE( "pow" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MPW_IN_BASE, "b", OP_MPW_IN_BASE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MPW_IN_POW, "p", OP_MPW_IN_POW_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MPW_OUT_DST, "^", OP_MPW_OUT_DST_INFO );
	Validate();
}

COpMathPow::~COpMathPow() 
{}

void COpMathPow::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	m_pdOut[OP_MPW_OUT_DST] =
		pow( m_poIn[OP_MPW_IN_BASE].Proc(), m_poIn[OP_MPW_IN_POW].Proc() );
}

OP_SET_NAME_AND_INFO_CLASS( COpMathPow, OP_MPW_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMathPow )
OP_GENERIC_METHODS_DEF( COpMathPow )

#endif // OP_INC_OP_MATH_POW
