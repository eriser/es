// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathModf.cpp

#include "OpMathModf.h"
#ifdef OP_INC_OP_MATH_MODF

////////////////////////////////////////////////////////////////////////////////
// COpMathModf
////////////////////////////////////////////////////////////////////////////////

#define OP_MMM_IN_SRC		0
#define OP_MMM_OUT_INT		0
#define OP_MMM_OUT_FRAC		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MMM_IN_SRC_INFO	"input"
#define OP_MMM_OUT_INT_INFO	"integer part"
#define OP_MMM_OUT_FRAC_INFO	"fractional part"
#define OP_MMM_CLASS_INFO \
\
"modf Function\n" \
"- Splits a floating-point value into fractional and integer parts."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MMM_IN_SRC_INFO	"Eingang"
#define OP_MMM_OUT_INT_INFO	"Ganzzahl"
#define OP_MMM_OUT_FRAC_INFO	"Nachkommastellen"
#define OP_MMM_CLASS_INFO \
\
"modf-Funktion\n" \
"- Trennt Ganzzahl und Nachkommastellen."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMathModf::COpMathModf()
: COp( 1, 2 )
{
	OP_SET_NAME_INSTANCE( "modf" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MMM_IN_SRC, "x", OP_MMM_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MMM_OUT_INT, "i", OP_MMM_OUT_INT_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MMM_OUT_FRAC, "f", OP_MMM_OUT_FRAC_INFO );
	Validate();
}

COpMathModf::~COpMathModf() 
{}

void COpMathModf::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	m_pdOut[OP_MMM_OUT_FRAC] = modf( m_poIn[OP_MMM_IN_SRC].Proc(), &m_pdOut[OP_MMM_OUT_INT] );
}

OP_SET_NAME_AND_INFO_CLASS( COpMathModf, OP_MMM_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMathModf )
OP_GENERIC_METHODS_DEF( COpMathModf )

#endif // OP_INC_OP_MATH_MODF
