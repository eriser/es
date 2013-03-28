// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMul.cpp

#include "OpMul.h"
#ifdef OP_INC_OP_MUL

////////////////////////////////////////////////////////////////////////////////
// COpMul
////////////////////////////////////////////////////////////////////////////////

#define OP_MUL_IN_SRC0		0
#define OP_MUL_IN_SRC1		1
#define OP_MUL_OUT_PROD		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MUL_IN_SRC0_INFO	"first factor"
#define OP_MUL_IN_SRC1_INFO	"second factor"
#define OP_MUL_OUT_PROD_INFO	"product"
#define OP_MUL_CLASS_INFO \
\
"Multiplication\n" \
"- Both factors are multiplied by each other."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MUL_IN_SRC0_INFO	"erster Faktor"
#define OP_MUL_IN_SRC1_INFO	"zweiter Faktor"
#define OP_MUL_OUT_PROD_INFO	"Produkt"
#define OP_MUL_CLASS_INFO \
\
"Multiplikation\n" \
"- Der beiden Faktoren werden miteinander multipliziert."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMul::COpMul()
: COp( 2, 1 )
{
	OP_SET_NAME_INSTANCE( "mul" );
	
	OP_SET_NAME_AND_INFO_INPUT( OP_MUL_IN_SRC0, "x", OP_MUL_IN_SRC0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MUL_IN_SRC1, "y", OP_MUL_IN_SRC1_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MUL_OUT_PROD, "*", OP_MUL_OUT_PROD_INFO );
	SetFlags( OP_FLAG_COMBINATIVE );
	Validate();
}

COpMul::~COpMul() 
{}

void COpMul::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	m_pdOut[OP_MUL_OUT_PROD] =
		  m_poIn[OP_MUL_IN_SRC0].Proc()
		* m_poIn[OP_MUL_IN_SRC1].Proc();
}

OP_SET_NAME_AND_INFO_CLASS( COpMul, OP_MUL_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMul )
OP_GENERIC_METHODS_DEF( COpMul )

#endif // OP_INC_OP_MUL
