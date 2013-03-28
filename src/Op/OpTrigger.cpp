// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTrigger.cpp

#include "OpTrigger.h"
#ifdef OP_INC_OP_TRIGGER

////////////////////////////////////////////////////////////////////////////////
// COpTrigger
////////////////////////////////////////////////////////////////////////////////

#define OP_TRG_IN_SRC		0
#define OP_TRG_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_TRG_IN_SRC_INFO	"input"
#define OP_TRG_OUT_DST_INFO	"output"
#define OP_TRG_CLASS_INFO \
\
"Trigger\n" \
"- Whenever the input value has changed, the output value is 1.0,\n" \
"  otherwise always 0.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_TRG_IN_SRC_INFO	"Eingang"
#define OP_TRG_OUT_DST_INFO	"Ausgang"
#define OP_TRG_CLASS_INFO \
\
"Trigger\n" \
"- Wenn der Eingangswert sich geändert hat,\n" \
"  steht der Ausgang auf 1.0, sonst auf 0.0."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpTrigger::COpTrigger()
: COp( 1, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "trigg" );
	OP_SET_NAME_AND_INFO_INPUT( OP_TRG_IN_SRC, "i", OP_TRG_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TRG_OUT_DST, "o", OP_TRG_OUT_DST_INFO );
	Validate();
	Reset();
}

COpTrigger::~COpTrigger()
{}

void COpTrigger::Init()
{
	OP_SET_COUNT_INTERNAL_FLT( 1 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dV );
}

void COpTrigger::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	const double dIn = m_poIn[OP_TRG_IN_SRC].Proc();
	m_pdOut[OP_TRG_OUT_DST] = ( m_dV != dIn ? 1.0 : 0.0 );
	m_dV = dIn;
}

void COpTrigger::Reset()
{
	COp::Reset();
	m_dV = 0.0;
}

OP_SET_NAME_AND_INFO_CLASS( COpTrigger, OP_TRG_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpTrigger )
OP_GENERIC_METHODS_DEF( COpTrigger )

#endif // OP_INC_OP_TRIGGER
