// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltAllpass2.cpp

#include "OpFltAllpass2.h"
#ifdef OP_INC_OP_FLT_ALLPASS_2

////////////////////////////////////////////////////////////////////////////////
// COpFltAllpass2
////////////////////////////////////////////////////////////////////////////////

#define OP_FL2_IN_SRC			0
#define OP_FL2_IN_FREQ			1
#define OP_FL2_IN_RADIUS		2
#define OP_FL2_OUT_DST			0
#define OP_FL2_ROUTINE_SIZE		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FL2_IN_SRC_INFO		"input"
#define OP_FL2_IN_FREQ_INFO		"frequency"
#define OP_FL2_IN_RADIUS_INFO		"radius"
#define OP_FL2_OUT_DST_INFO		"output"
#define OP_FL2_CLASS_INFO \
\
"Second-Order Allpass Filter\n" \
"- This filter type changes the phase relationship between frequencies.\n" \
"- The radius should always be less than 1.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FL2_IN_SRC_INFO		"Eingang"
#define OP_FL2_IN_FREQ_INFO		"Frequenz"
#define OP_FL2_IN_RADIUS_INFO		"Radius"
#define OP_FL2_OUT_DST_INFO		"Ausgang"
#define OP_FL2_CLASS_INFO \
\
"Allpass-Filter zweiter Ordnung\n" \
"- Das Signal wird frequenzabhängig verzögert.\n" \
"- Der Radius sollte kleiner 1.0 sein."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltAllpass2::COpFltAllpass2()
: COp( 3, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flAll2" );
	OP_SET_NAME_AND_INFO_INPUT( OP_FL2_IN_SRC, "i", OP_FL2_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FL2_IN_FREQ, "f", OP_FL2_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FL2_IN_RADIUS, "r", OP_FL2_IN_RADIUS_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FL2_OUT_DST, "o", OP_FL2_OUT_DST_INFO );
	Update();
	Validate();
}

COpFltAllpass2::~COpFltAllpass2()
{}

void COpFltAllpass2::Init()
{
	m_dRadius = 0.0;
	m_dFreq = 0.0;
	OP_SET_COUNT_INTERNAL_FLT( 4 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oFlt.m_tIn0 );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oFlt.m_tIn1 );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oFlt.m_tOut0 );
	OP_SET_PTR_INTERNAL_FLT( 3, &m_oFlt.m_tOut1 );
	COp::Init();
}

void COpFltAllpass2::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dIn = m_poIn[OP_FL2_IN_SRC].Proc();
	const double dFreq = m_poIn[OP_FL2_IN_FREQ].Proc();
	const double dRadius = m_poIn[OP_FL2_IN_RADIUS].Proc();
	if( dRadius != m_dRadius )
	{
		m_dRadius = dRadius;
		m_oFlt.InitRadius( dRadius );
	}
	if( dFreq != m_dFreq )
	{
		m_dFreq = dFreq;
		m_oFlt.InitFreq( dFreq );
	}
	m_pdOut[OP_FL2_OUT_DST] = m_oFlt.Proc( dIn );
}

void COpFltAllpass2::Update()
{
	m_oFlt.InitRadius( m_dRadius );
	m_oFlt.InitFreq( m_dFreq );
	COp::Update();
}

void COpFltAllpass2::Reset()
{
	m_oFlt.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpFltAllpass2, OP_FL2_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltAllpass2 )
OP_GENERIC_METHODS_DEF( COpFltAllpass2 )

#endif // OP_INC_OP_FLT_ALLPASS_2
