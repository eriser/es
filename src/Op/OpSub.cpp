// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSub.cpp

#include "OpSub.h"
#ifdef OP_INC_OP_SUB

////////////////////////////////////////////////////////////////////////////////
// COpSub
////////////////////////////////////////////////////////////////////////////////

#define OP_SUB_IN_SRC0		0
#define OP_SUB_IN_SRC1		1
#define OP_SUB_OUT_SUM		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_SUB_IN_SRC0_INFO	"minuend"
#define OP_SUB_IN_SRC1_INFO	"subtrahend"
#define OP_SUB_OUT_SUM_INFO	"difference"
#define OP_SUB_CLASS_INFO \
\
"Subtraction\n" \
"- Calculates the difference between the minuend and the subtrahend."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_SUB_IN_SRC0_INFO	"Minuend"
#define OP_SUB_IN_SRC1_INFO	"Subtrahend"
#define OP_SUB_OUT_SUM_INFO	"Differenz"
#define OP_SUB_CLASS_INFO \
\
"Subtraktion\n" \
"- Der Subtrahend wird vom Minuend abgezogen."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSub::COpSub()
: COp( 2, 1 )
{
	OP_SET_NAME_INSTANCE( "sub" );
	OP_SET_NAME_AND_INFO_INPUT( OP_SUB_IN_SRC0, "x", OP_SUB_IN_SRC0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SUB_IN_SRC1, "y", OP_SUB_IN_SRC1_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SUB_OUT_SUM, "-", OP_SUB_OUT_SUM_INFO );
	SetFlags( OP_FLAG_COMBINATIVE );
	Validate();
}

COpSub::~COpSub() 
{}

void COpSub::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	m_pdOut[OP_SUB_OUT_SUM] =
		  m_poIn[OP_SUB_IN_SRC0].Proc()
		- m_poIn[OP_SUB_IN_SRC1].Proc();
}

OP_SET_NAME_AND_INFO_CLASS( COpSub, OP_SUB_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSub )
OP_GENERIC_METHODS_DEF( COpSub )

#endif // OP_INC_OP_SUB
