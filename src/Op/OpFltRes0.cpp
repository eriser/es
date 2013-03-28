// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltRes0.cpp

#include "OpFltRes0.h"
#ifdef OP_INC_OP_FLT_RES_0

////////////////////////////////////////////////////////////////////////////////
// COpFltRes0
////////////////////////////////////////////////////////////////////////////////

#define OP_FLR_IN_SRC		0
#define OP_FLR_IN_FREQ		1
#define OP_FLR_IN_RES		2
#define OP_FLR_IN_CLEAR		3
#define OP_FLR_OUT_LP		0
#define OP_FLR_OUT_BP		1
#define OP_FLR_OUT_HP		2

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FLR_IN_SRC_INFO	"input"
#define OP_FLR_IN_FREQ_INFO	"frequency"
#define OP_FLR_IN_RES_INFO	"resonance"
#define OP_FLR_IN_CLEAR_INFO	"reset"
#define OP_FLR_OUT_LP_INFO	"low-pass"
#define OP_FLR_OUT_BP_INFO	"band-pass"
#define OP_FLR_OUT_HP_INFO	"high-pass"
#define OP_FLR_CLASS_INFO \
\
"Resonant Filter\n" \
"- When the reset input value is greater than 0.0, then all the internal\n" \
"  variables will be reinitialized."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FLR_IN_SRC_INFO	"Eingang"
#define OP_FLR_IN_FREQ_INFO	"Frequenz"
#define OP_FLR_IN_RES_INFO	"Resonanz"
#define OP_FLR_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_FLR_OUT_LP_INFO	"Tiefpass"
#define OP_FLR_OUT_BP_INFO	"Bandpass"
#define OP_FLR_OUT_HP_INFO	"Hochpass"
#define OP_FLR_CLASS_INFO \
\
"Resonantes Filter\n" \
"- Ist der Reset-Eingang grösser als 0.0, so wird der Speicher zurückgesetzt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltRes0::COpFltRes0()
: COp( 4, 3 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flRes0" );
	m_poIn[OP_FLR_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLR_IN_SRC, "i", OP_FLR_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLR_IN_FREQ, "f", OP_FLR_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLR_IN_RES, "r", OP_FLR_IN_RES_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLR_IN_CLEAR, "cl", OP_FLR_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FLR_OUT_LP, "lp", OP_FLR_OUT_LP_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FLR_OUT_BP, "bp", OP_FLR_OUT_BP_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FLR_OUT_HP, "hp", OP_FLR_OUT_HP_INFO );
	Validate();
	Update();
}

COpFltRes0::~COpFltRes0() 
{}

void COpFltRes0::Init()
{
	Reset();
	m_dCutoff = m_dRes = 0.0;
	OP_SET_COUNT_INTERNAL_FLT( 2 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oFlt.m_tOut0 );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oFlt.m_tOut1 );
	COp::Init();
}

void COpFltRes0::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_FLR_IN_CLEAR].IsValid() )
		if( m_poIn[OP_FLR_IN_CLEAR].Proc() > 0.0 )
			m_oFlt.Reset();
	
	const double dCutoff = m_poIn[OP_FLR_IN_FREQ].Proc();
	const double dRes = m_poIn[OP_FLR_IN_RES].Proc();
	if( m_dCutoff != dCutoff || m_dRes != dRes )
	{
		m_dCutoff = dCutoff;
		m_dRes = dRes;
		m_oFlt.Init( dCutoff, dRes );
	}
	m_oFlt.Proc( m_poIn[OP_FLR_IN_SRC].Proc(), 
			&m_pdOut[OP_FLR_OUT_LP],
			&m_pdOut[OP_FLR_OUT_BP],
			&m_pdOut[OP_FLR_OUT_HP] );
}

void COpFltRes0::Update()
{
	m_oFlt.Init( m_dCutoff, m_dRes );
}

void COpFltRes0::Reset()
{
	COp::Reset();
	m_oFlt.Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpFltRes0, OP_FLR_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltRes0 )
OP_GENERIC_METHODS_DEF( COpFltRes0 )

#endif // OP_INC_OP_FLT_RES_0
