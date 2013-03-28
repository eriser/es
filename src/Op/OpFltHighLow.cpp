// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltHighLow.cpp

#include "OpFltHighLow.h"
#ifdef OP_INC_OP_FLT_HIGH_LOW

////////////////////////////////////////////////////////////////////////////////
// COpFltHighLow
////////////////////////////////////////////////////////////////////////////////

#define OP_FLH_IN_SRC		0
#define OP_FLH_IN_FREQ		1
#define OP_FLH_IN_RES		2
#define OP_FLH_IN_CLEAR		3
#define OP_FLH_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FLH_IN_SRC_INFO	"input"
#define OP_FLH_IN_FREQ_INFO	"frequency"
#define OP_FLH_IN_RES_INFO	"resonance"
#define OP_FLH_IN_CLEAR_INFO	"reset"
#define OP_FLH_OUT_DST_INFO	"output"
#define OP_FLH_ROUTINE_INFO	"filter type"
#define OP_FLH_CLASS_INFO \
\
"High-/Low-Pass Filter\n" \
"- Available filter types: LOW_PASS, HIGH_PASS\n" \
"- The filter gets unstable for resonance values over 0.9.\n" \
"- When the reset input value is greater than 0.0, then all the internal\n" \
"  variables will be reinitialized."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FLH_IN_SRC_INFO	"Eingang"
#define OP_FLH_IN_FREQ_INFO	"Frequenz"
#define OP_FLH_IN_RES_INFO	"Resonanz"
#define OP_FLH_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_FLH_OUT_DST_INFO	"Ausgang"
#define OP_FLH_ROUTINE_INFO	"Filtertyp"
#define OP_FLH_CLASS_INFO \
\
"Hoch-/Tiefpass-Filter\n" \
"- Mittels Routine kann zwischen Hoch- und Tief-Pass Charakter gewählt werden.\n" \
"- Das Filter wird instabil bei Resonanzwerten über 0.9.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der Speicher zurückgesetzt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltHighLow::COpFltHighLow()
: COp( 4, 1 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flHiLo" );
	m_poIn[OP_FLH_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLH_IN_SRC, "i", OP_FLH_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLH_IN_FREQ, "f", OP_FLH_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLH_IN_RES, "r", OP_FLH_IN_RES_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLH_IN_CLEAR, "cl", OP_FLH_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FLH_OUT_DST, "o", OP_FLH_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_FLH_ROUTINE_INFO );
	Validate();
	Update();
}

COpFltHighLow::~COpFltHighLow() 
{}

void COpFltHighLow::Init()
{
	Reset();
	m_dCutoff = m_dRes = 0.0;
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	OP_SET_COUNT_INTERNAL_FLT( 4 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oFlt.m_tIn0 );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oFlt.m_tIn1 );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oFlt.m_tOut0 );
	OP_SET_PTR_INTERNAL_FLT( 3, &m_oFlt.m_tOut1 );
	COp::Init();
}

void COpFltHighLow::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_FLH_IN_CLEAR].IsValid() )
		if( m_poIn[OP_FLH_IN_CLEAR].Proc() > 0.0 )
			m_oFlt.Reset();
	
	const double dCutoff = m_poIn[OP_FLH_IN_FREQ].Proc();
	const double dRes = m_poIn[OP_FLH_IN_RES].Proc();
	if( m_dCutoff != dCutoff || m_dRes != dRes )
	{
		m_dCutoff = dCutoff;
		m_dRes = dRes;
		m_oFlt.Init( m_uiFuncNum != 0, dCutoff, dRes );
	}
	
	m_pdOut[OP_FLH_OUT_DST] = m_oFlt.Proc( m_poIn[OP_FLH_IN_SRC].Proc() );
}

void COpFltHighLow::Update()
{
#ifdef OP_USE_RUNTIME_INFO
	SetNameOutput( OP_FLH_OUT_DST, ( m_uiFuncNum != 0 ? "hp" : "lp" ) );
#endif // OP_USE_RUNTIME_INFO

	m_oFlt.Init( m_uiFuncNum != 0, m_dCutoff, m_dRes );
	COp::Update();
}

void COpFltHighLow::Reset()
{
	COp::Reset();
	m_oFlt.Reset();
}

#ifdef OP_USE_ROUTINES
void COpFltHighLow::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	m_uiFuncNum = !m_uiFuncNum;
	Reset();
	Update();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpFltHighLow, OP_FLH_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltHighLow )
OP_GENERIC_METHODS_DEF( COpFltHighLow )

#endif // OP_INC_OP_FLT_HIGH_LOW
