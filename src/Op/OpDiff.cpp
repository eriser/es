// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDiff.cpp

#include "OpDiff.h"
#ifdef OP_INC_OP_DIFF

////////////////////////////////////////////////////////////////////////////////
// COpDiff
////////////////////////////////////////////////////////////////////////////////

#define OP_DFF_IN_SRC		0
#define OP_DFF_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_DFF_IN_SRC_INFO	"input"
#define OP_DFF_OUT_DST_INFO	"output"
#define OP_DFF_CLASS_INFO \
\
"Difference\n" \
"- Calculates the difference between the current and the previous input value."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_DFF_IN_SRC_INFO	"Eingang"
#define OP_DFF_OUT_DST_INFO	"Ausgang"
#define OP_DFF_CLASS_INFO \
\
"Differenz\n" \
"- Die Differenz des aktuellen Eingangs-Werts und dem zuletzt eingegebenen wird\n" \
"  ausgegeben."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpDiff::COpDiff()
: COp( 1, 1 )
, m_dLast( 0.0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "diff" );
	OP_SET_NAME_AND_INFO_INPUT( OP_DFF_IN_SRC, "i", OP_DFF_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_DFF_OUT_DST, "o", OP_DFF_OUT_DST_INFO );
	SetFlags( OP_FLAG_COMBINATIVE );
	Validate();
}

COpDiff::~COpDiff() 
{}

void COpDiff::Init()
{
	OP_SET_COUNT_INTERNAL_FLT( 1 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dLast );
	COp::Init();
}

void COpDiff::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	const double dIn = m_poIn[OP_DFF_IN_SRC].Proc();
	m_pdOut[OP_DFF_OUT_DST] = dIn - m_dLast;
	m_dLast = dIn;
}

void COpDiff::Reset()
{
	m_dLast = 0.0;
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpDiff, OP_DFF_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpDiff )
OP_GENERIC_METHODS_DEF( COpDiff )

#endif // OP_INC_OP_DIFF

