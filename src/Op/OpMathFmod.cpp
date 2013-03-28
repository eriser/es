// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathFmod.cpp

#include "OpMathFmod.h"
#ifdef OP_INC_OP_MATH_FMOD

////////////////////////////////////////////////////////////////////////////////
// COpMathFmod
////////////////////////////////////////////////////////////////////////////////

#define OP_MFM_IN_SRC0		0
#define OP_MFM_IN_SRC1		1
#define OP_MFM_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MFM_IN_SRC0_INFO	"dividend"
#define OP_MFM_IN_SRC1_INFO	"divisor"
#define OP_MFM_OUT_DST_INFO	"remainder"
#define OP_MFM_CLASS_INFO \
\
"Modulo Function\n" \
"- The dividend is divided by the divisor. The output is the remainder.\n" \
"- If the divisor is equals 0.0, no operation will be executed."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MFM_IN_SRC0_INFO	"Dividend"
#define OP_MFM_IN_SRC1_INFO	"Divisor"
#define OP_MFM_OUT_DST_INFO	"Rest"
#define OP_MFM_CLASS_INFO \
\
"Modulo-Funktion\n" \
"- Der Dividend wird durch den Divisor geteilt und dann der Rest der Division\n" \
"  ausgegeben.\n" \
"- Ist der Divisor gleich 0.0, so wird die Operation nicht ausgeführt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMathFmod::COpMathFmod()
: COp( 2, 1 )
{
	OP_SET_NAME_INSTANCE( "fmod" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MFM_IN_SRC0, "x", OP_MFM_IN_SRC0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MFM_IN_SRC1, "y", OP_MFM_IN_SRC1_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MFM_OUT_DST, "o", OP_MFM_OUT_DST_INFO );
	Validate();
}

COpMathFmod::~COpMathFmod() 
{}

void COpMathFmod::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dSrc1 = m_poIn[OP_MFM_IN_SRC1].Proc();
	if( dSrc1 )
		m_pdOut[OP_MFM_OUT_DST] = fmod( 
			m_poIn[OP_MFM_IN_SRC0].Proc(),
			dSrc1 );
}

OP_SET_NAME_AND_INFO_CLASS( COpMathFmod, OP_MFM_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMathFmod )
OP_GENERIC_METHODS_DEF( COpMathFmod )

#endif // OP_INC_OP_MATH_FMOD
