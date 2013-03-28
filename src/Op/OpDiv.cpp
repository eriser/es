// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDiv.cpp

#include "OpDiv.h"
#ifdef OP_INC_OP_DIV

////////////////////////////////////////////////////////////////////////////////
// COpDiv
////////////////////////////////////////////////////////////////////////////////

#define OP_DIV_IN_SRC0		0
#define OP_DIV_IN_SRC1		1
#define OP_DIV_OUT_PROD		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_DIV_IN_SRC0_INFO	"dividend"
#define OP_DIV_IN_SRC1_INFO	"divisor"
#define OP_DIV_OUT_PROD_INFO	"quotient"
#define OP_DIV_CLASS_INFO \
\
"Division\n" \
"- The dividend is divided by the divisor.\n" \
"- If the divisor is equals 0.0, no operation will be executed."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_DIV_IN_SRC0_INFO	"Dividend"
#define OP_DIV_IN_SRC1_INFO	"Divisor"
#define OP_DIV_OUT_PROD_INFO	"Quotient"
#define OP_DIV_CLASS_INFO \
\
"Division\n" \
"- Der Dividend wird durch den Divisor geteilt.\n" \
"- Wenn der Divisor gleich 0.0 ist, wird die Operation nicht ausgeführt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpDiv::COpDiv()
: COp( 2, 1 )
{
	OP_SET_NAME_INSTANCE( "div" );
	OP_SET_NAME_AND_INFO_INPUT( OP_DIV_IN_SRC0, "x", OP_DIV_IN_SRC0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_DIV_IN_SRC1, "y", OP_DIV_IN_SRC1_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_DIV_OUT_PROD, "/", OP_DIV_OUT_PROD_INFO );
	SetFlags( OP_FLAG_COMBINATIVE );
	Validate();
}

COpDiv::~COpDiv() 
{}

void COpDiv::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dDivisor = m_poIn[OP_DIV_IN_SRC1].Proc();
	if( dDivisor )
		m_pdOut[OP_DIV_OUT_PROD] =
			m_poIn[OP_DIV_IN_SRC0].Proc() / dDivisor;
}

OP_SET_NAME_AND_INFO_CLASS( COpDiv, OP_DIV_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpDiv )
OP_GENERIC_METHODS_DEF( COpDiv )

#endif // OP_INC_OP_DIV
