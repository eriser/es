// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpAdd.cpp

#include "OpAdd.h"
#ifdef OP_INC_OP_ADD

////////////////////////////////////////////////////////////////////////////////
// COpAdd
////////////////////////////////////////////////////////////////////////////////

#define OP_ADD_IN_SRC0		0
#define OP_ADD_IN_SRC1		1
#define OP_ADD_OUT_SUM		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_ADD_IN_SRC0_INFO	"first addend"
#define OP_ADD_IN_SRC1_INFO	"second addend"
#define OP_ADD_OUT_SUM_INFO	"sum"
#define OP_ADD_CLASS_INFO \
\
"Addition\n" \
"- Calculates the sum of both addends."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_ADD_IN_SRC0_INFO	"erster Summand"
#define OP_ADD_IN_SRC1_INFO	"zweiter Summand"
#define OP_ADD_OUT_SUM_INFO	"Summe"
#define OP_ADD_CLASS_INFO \
\
"Addition\n" \
"- Die Summanden werden addiert."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpAdd::COpAdd()
: COp( 2, 1 )
{
	OP_SET_NAME_INSTANCE( "add" );
	OP_SET_NAME_AND_INFO_INPUT( OP_ADD_IN_SRC0, "x", OP_ADD_IN_SRC0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_ADD_IN_SRC1, "y", OP_ADD_IN_SRC1_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_ADD_OUT_SUM, "+", OP_ADD_OUT_SUM_INFO );
	SetFlags( OP_FLAG_COMBINATIVE );
	Validate();
}

COpAdd::~COpAdd() 
{}

void COpAdd::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	m_pdOut[OP_ADD_OUT_SUM] =
		  m_poIn[OP_ADD_IN_SRC0].Proc()
		+ m_poIn[OP_ADD_IN_SRC1].Proc();
}

OP_SET_NAME_AND_INFO_CLASS( COpAdd, OP_ADD_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpAdd )
OP_GENERIC_METHODS_DEF( COpAdd )

#endif // OP_INC_OP_ADD

