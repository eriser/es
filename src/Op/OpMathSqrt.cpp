// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathSqrt.cpp

#include "OpMathSqrt.h"
#ifdef OP_INC_OP_MATH_SQRT

////////////////////////////////////////////////////////////////////////////////
// COpMathSqrt
////////////////////////////////////////////////////////////////////////////////

#define OP_MSQ_IN_SRC		0
#define OP_MSQ_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MSQ_IN_SRC_INFO	"input"
#define OP_MSQ_OUT_DST_INFO	"output"
#define OP_MSQ_CLASS_INFO \
\
"Square Root Function\n" \
"- The function is not executed for negative input values."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MSQ_IN_SRC_INFO	"Eingang"
#define OP_MSQ_OUT_DST_INFO	"Ausgang"
#define OP_MSQ_CLASS_INFO \
\
"Quadratwurzel Funktion\n" \
"- Ist der Eingangswert negativ, so wird die Funktion nicht ausgeführt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMathSqrt::COpMathSqrt()
: COp( 1, 1 )
{
	OP_SET_NAME_INSTANCE( "sqrt" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MSQ_IN_SRC, "i", OP_MSQ_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MSQ_OUT_DST, "o", OP_MSQ_OUT_DST_INFO );
	Validate();
}

COpMathSqrt::~COpMathSqrt()
{}

void COpMathSqrt::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dSrc = m_poIn[OP_MSQ_IN_SRC].Proc();
	if( dSrc >= 0.0 )
		m_pdOut[OP_MSQ_OUT_DST] = sqrt( dSrc );
}

OP_SET_NAME_AND_INFO_CLASS( COpMathSqrt, OP_MSQ_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMathSqrt )
OP_GENERIC_METHODS_DEF( COpMathSqrt )

#endif // OP_INC_OP_MATH_SQRT
