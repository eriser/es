// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathExp.cpp

#include "OpMathExp.h"
#ifdef OP_INC_OP_MATH_EXP

////////////////////////////////////////////////////////////////////////////////
// COpMathExp
////////////////////////////////////////////////////////////////////////////////

#define OP_MEX_IN_SRC		0
#define OP_MEX_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MEX_IN_SRC_INFO	"input"
#define OP_MEX_OUT_DST_INFO	"output"
#define OP_MEX_CLASS_INFO \
\
"Exponential Function\n" \
"- Domain: (-inf,+inf).\n" \
"- Range: (0,+inf)."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MEX_IN_SRC_INFO	"Eingang"
#define OP_MEX_OUT_DST_INFO	"Ausgang"
#define OP_MEX_CLASS_INFO \
\
"Exponentialfunktion\n" \
"- Definitionsbereich: (-inf,+inf).\n" \
"- Wertebereich: (0,+inf)."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMathExp::COpMathExp()
: COp( 1, 1 )
{
	OP_SET_NAME_INSTANCE( "exp" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MEX_IN_SRC, "i", OP_MEX_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MEX_OUT_DST, "o", OP_MEX_OUT_DST_INFO );
	Validate();
}

COpMathExp::~COpMathExp() 
{}

void COpMathExp::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	m_pdOut[OP_MEX_OUT_DST] = exp( m_poIn[OP_MEX_IN_SRC].Proc() );
}

OP_SET_NAME_AND_INFO_CLASS( COpMathExp, OP_MEX_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMathExp )
OP_GENERIC_METHODS_DEF( COpMathExp )

#endif // OP_INC_OP_MATH_EXP
