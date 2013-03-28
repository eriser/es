// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpNop.cpp

#include "OpNop.h"
#ifdef OP_INC_OP_NOP

////////////////////////////////////////////////////////////////////////////////
// COpNop
////////////////////////////////////////////////////////////////////////////////

#define OP_NOP_IN_SRC		0
#define OP_NOP_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_NOP_IN_SRC_INFO	"input"
#define OP_NOP_OUT_DST_INFO	"output"
#define OP_NOP_CLASS_INFO \
\
"No Operation\n" \
"- Input equals output."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_NOP_IN_SRC_INFO	"Eingang"
#define OP_NOP_OUT_DST_INFO	"Ausgang"
#define OP_NOP_CLASS_INFO \
\
"Keine Operation\n" \
"- Eingang gleich Ausgang."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpNop::COpNop()
: COp( 1, 1 )
{
	OP_SET_NAME_INSTANCE( "nop" );
	OP_SET_NAME_AND_INFO_INPUT( OP_NOP_IN_SRC, "i", OP_NOP_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_NOP_OUT_DST, "o", OP_NOP_OUT_DST_INFO );
	Validate();
	//SetFlags( OP_FLAG_SHOW_OUTPUTS );
}

COpNop::~COpNop() 
{}

void COpNop::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	m_pdOut[OP_NOP_OUT_DST] = m_poIn[OP_NOP_IN_SRC].Proc();
}

OP_SET_NAME_AND_INFO_CLASS( COpNop, OP_NOP_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpNop )
OP_GENERIC_METHODS_DEF( COpNop )

#endif // OP_INC_OP_NOP
