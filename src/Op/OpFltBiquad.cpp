// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltBiquad.cpp

#include "OpFltBiquad.h"
#ifdef OP_INC_OP_FLT_BIQUAD

////////////////////////////////////////////////////////////////////////////////
// COpFltBiquad
////////////////////////////////////////////////////////////////////////////////

#define OP_FLM_IN_SRC		0
#define OP_FLM_IN_FREQ		1
#define OP_FLM_IN_BANDW		2
#define OP_FLM_IN_GAIN		3
#define OP_FLM_IN_CLEAR		4
#define OP_FLM_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FLM_IN_SRC_INFO	"input"
#define OP_FLM_IN_FREQ_INFO	"frequency"
#define OP_FLM_IN_BANDW_INFO	"bandwidth"
#define OP_FLM_IN_GAIN_INFO	"gain"
#define OP_FLM_IN_CLEAR_INFO	"reset"
#define OP_FLM_OUT_DST_INFO	"output"
#define OP_FLM_ROUTINE_INFO	"filter type"
#define OP_FLM_CLASS_INFO \
\
"Biquad Filter\n" \
"- Available filter types: LOW_PASS, HIGH_PASS, BAND_PASS,\n" \
"  NOTCH, PEAKING_EQ, LOW_SHELF, HIGH_SHELF.\n" \
"- When the reset input value is greater than 0.0, then all the internal\n" \
"  variables will be reinitialized."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FLM_IN_SRC_INFO	"Eingang"
#define OP_FLM_IN_FREQ_INFO	"Frequenz"
#define OP_FLM_IN_BANDW_INFO	"Bandbreite"
#define OP_FLM_IN_GAIN_INFO	"Verstärkung"
#define OP_FLM_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_FLM_OUT_DST_INFO	"Ausgang"
#define OP_FLM_ROUTINE_INFO	"Filtertyp"
#define OP_FLM_CLASS_INFO \
\
"Biquad-Filter\n" \
"- Es stehen folgende Filter-Typen zur Auswahl: LOW_PASS, HIGH_PASS, BAND_PASS,\n" \
"  NOTCH, PEAKING_EQ, LOW_SHELF, HIGH_SHELF.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der Speicher zurück-\n" \
"  gesetzt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltBiquad::COpFltBiquad()
: COp( 5, 1 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flBiq" );
	m_poIn[OP_FLM_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLM_IN_SRC, "i", OP_FLM_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLM_IN_FREQ, "f", OP_FLM_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLM_IN_BANDW, "bw", OP_FLM_IN_BANDW_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLM_IN_GAIN, "g", OP_FLM_IN_GAIN_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLM_IN_CLEAR, "cl", OP_FLM_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FLM_OUT_DST, "o", OP_FLM_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_FLM_ROUTINE_INFO );
	Validate();
	Update();
}

COpFltBiquad::~COpFltBiquad() 
{}

void COpFltBiquad::Init()
{
	Reset();
	m_dCutoff = m_dGain = m_dBandwidth = 0.0;
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	OP_SET_COUNT_INTERNAL_FLT( 4 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oFlt.m_tIn0 );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oFlt.m_tIn1 );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oFlt.m_tOut0 );
	OP_SET_PTR_INTERNAL_FLT( 3, &m_oFlt.m_tOut1 );
	COp::Init();
}

void COpFltBiquad::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_FLM_IN_CLEAR].IsValid() )
		if( m_poIn[OP_FLM_IN_CLEAR].Proc() > 0.0 )
			m_oFlt.Reset();
	
	const double dCutoff = m_poIn[OP_FLM_IN_FREQ].Proc();
	const double dGain = m_poIn[OP_FLM_IN_GAIN].Proc();
	const double dBandwidth = m_poIn[OP_FLM_IN_BANDW].Proc();
	if( m_dCutoff != dCutoff || m_dGain != dGain || m_dBandwidth != dBandwidth )
	{
		m_dCutoff = dCutoff;
		m_dGain = dGain;
		m_dBandwidth = dBandwidth;
		m_oFlt.Init( m_uiFuncNum, dCutoff, dGain, dBandwidth );
	}
	m_pdOut[OP_FLM_OUT_DST] = m_oFlt.Proc( m_poIn[OP_FLM_IN_SRC].Proc() );
}

void COpFltBiquad::Update()
{
#ifdef OP_USE_RUNTIME_INFO
	static const char *aacFuncName_[] = { "lp", "hp", "bp", "ntc", "peq", "lsh", "hsh" };
	SetNameOutput( 0, aacFuncName_[m_uiFuncNum] );
#endif // OP_USE_RUNTIME_INFO

	m_oFlt.Init( m_uiFuncNum, m_dCutoff, m_dGain, m_dBandwidth );
}

void COpFltBiquad::Reset()
{
	COp::Reset();
	m_oFlt.Reset();
}

#ifdef OP_USE_ROUTINES
void COpFltBiquad::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	if( ++m_uiFuncNum >= COpTkFltBiquad<double, OP_SAMPLE_FREQ>::TYPE_MAX )
		m_uiFuncNum = 0;
	Reset();
	Update();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpFltBiquad, OP_FLM_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltBiquad )
OP_GENERIC_METHODS_DEF( COpFltBiquad )

#endif // OP_INC_OP_FLT_BIQUAD
