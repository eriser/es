// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// COpFltAllpass1.cpp

#include "OpFltAllpass1.h"
#ifdef OP_INC_OP_FLT_ALLPASS_1

////////////////////////////////////////////////////////////////////////////////
// COpFltAllpass1
////////////////////////////////////////////////////////////////////////////////

#define OP_FL1_IN_SRC			0
#define OP_FL1_IN_DELAY			1
#define OP_FL1_OUT_DST			0
#define OP_FL1_ROUTINE_SIZE		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FL1_IN_SRC_INFO		"input"
#define OP_FL1_IN_DELAY_INFO		"delay"
#define OP_FL1_OUT_DST_INFO		"output"
#define OP_FL1_CLASS_INFO \
\
"First-Order Allpass Filter\n" \
"- This filter type changes the phase relationship between frequencies."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FL1_IN_SRC_INFO		"Eingang"
#define OP_FL1_IN_DELAY_INFO		"Verzögerung"
#define OP_FL1_OUT_DST_INFO		"Ausgang"
#define OP_FL1_CLASS_INFO \
\
"Allpass-Filter erster Ordnung\n" \
"- Das Signal wird frequenzabhängig verzögert."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltAllpass1::COpFltAllpass1()
: COp( 2, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flAll1" );
	OP_SET_NAME_AND_INFO_INPUT( OP_FL1_IN_SRC, "i", OP_FL1_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FL1_IN_DELAY, "d", OP_FL1_IN_DELAY_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FL1_OUT_DST, "o", OP_FL1_OUT_DST_INFO );
	Update();
	Validate();
}

COpFltAllpass1::~COpFltAllpass1()
{}

void COpFltAllpass1::Init()
{
	m_dDelay = 0.0;
	OP_SET_COUNT_INTERNAL_FLT( 1 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oFlt.m_tOut );
	COp::Init();
}

void COpFltAllpass1::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dIn = m_poIn[OP_FL1_IN_SRC].Proc();
	const double dDelay = m_poIn[OP_FL1_IN_DELAY].Proc();
	if( dDelay != m_dDelay )
	{
		m_dDelay = dDelay;
		m_oFlt.Init( dDelay );
	}
	m_pdOut[OP_FL1_OUT_DST] = m_oFlt.Proc( dIn );
}

void COpFltAllpass1::Update()
{
	m_oFlt.Init( m_dDelay );
	COp::Update();
}

void COpFltAllpass1::Reset()
{
	m_oFlt.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpFltAllpass1, OP_FL1_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltAllpass1 )
OP_GENERIC_METHODS_DEF( COpFltAllpass1 )

#endif // OP_INC_OP_FLT_ALLPASS_1
