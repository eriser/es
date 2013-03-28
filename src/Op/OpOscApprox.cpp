// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscApprox.cpp

#include "OpOscApprox.h"
#ifdef OP_INC_OP_OSC_APPROX

////////////////////////////////////////////////////////////////////////////////
// COpOscApprox
////////////////////////////////////////////////////////////////////////////////

#define OP_APX_IN_FREQ			0
#define OP_APX_IN_PHASE			1
#define OP_APX_IN_APPROX		2
#define OP_APX_IN_CLEAR			3
#define OP_APX_OUT_DST			0
#define OP_APX_OUT_SYNC			1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_APX_IN_FREQ_INFO		"frequency"
#define OP_APX_IN_PHASE_INFO		"phase"
#define OP_APX_IN_APPROX_INFO		"approximation"
#define OP_APX_IN_CLEAR_INFO		"reset"
#define OP_APX_OUT_DST_INFO		"output"
#define OP_APX_ROUTINE_FUNC_INFO	"target function"
#define OP_APX_OUT_SYNC_INFO		"sync"
#define OP_APX_CLASS_INFO \
\
"Approximative Oscillator\n" \
"- Available target functions: PULSE, TRIANGLE, SAW.\n" \
"- The target functions is approximated by a fourier series.\n" \
"- 12 iterations by default.\n" \
"- The oscillator is restarted whenever the reset input value is\n" \
"  greater than 0.0.\n" \
"- When a new period begins, then the sync output value is 1.0,\n" \
"  otherwise always 0.0.\n" \
"- The phase input influences the sync signal."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_APX_IN_FREQ_INFO		"Frequenz"
#define OP_APX_IN_PHASE_INFO		"Phase"
#define OP_APX_IN_APPROX_INFO		"Approximation"
#define OP_APX_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_APX_OUT_DST_INFO		"Ausgang"
#define OP_APX_ROUTINE_FUNC_INFO	"Zielfunktion"
#define OP_APX_OUT_SYNC_INFO		"Synchronisation"
#define OP_APX_CLASS_INFO \
\
"Approximativer Oszillator\n" \
"- Es stehen folgende Zielfunktionen zur Verfügung:\n" \
"  PULSE, TRIANGLE, SAW.\n" \
"- Die Ziefunktion wird jeweils durch eine Fourierreihe angenähert.\n" \
"- Standardmässig werden 12 Iterationen gemacht.\n" \
"- Wenn der Wert am Zurücksetzen-Eingang grösser 0.0 ist, wird der Oszillator\n" \
"  neu gestartet.\n" \
"- Der Synchronisations-Ausgang steht immer dann auf 1.0, wenn eine neue\n" \
"  Periode beginnt. Sonst steht er konstant auf 0.0.\n" \
"- Die Phase beeinflusst das Synchronisations-Signal."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpOscApprox::COpOscApprox()
: COp( 4, 2 )
{
	Init();
	OP_SET_NAME_INSTANCE( "oApp" );
	m_poIn[OP_APX_IN_PHASE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_APX_IN_APPROX].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_APX_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_APX_IN_FREQ, "f", OP_APX_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_APX_IN_PHASE, "p", OP_APX_IN_PHASE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_APX_IN_APPROX, "i", OP_APX_IN_APPROX_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_APX_IN_CLEAR, "cl", OP_APX_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_APX_OUT_DST, "o", OP_APX_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_APX_OUT_SYNC, "sy", OP_APX_OUT_SYNC_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_APX_ROUTINE_FUNC_INFO );
	Update();
	Validate();
}

void COpOscApprox::Init()
{
	m_dHarmon = 12.0;
	m_oOsc.SetHarmon( 12 );
	Reset();
	OP_SET_COUNT_INTERNAL_FLT( 1 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oOsc.Pos() );
	OP_SET_COUNT_INTERNAL_UINT( 2 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oOsc.Type() );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_oOsc.Harmon() );
	OP_SET_COUNT_INTERNAL_INT( 1 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_oOsc.Period() );
	COp::Init();
}

COpOscApprox::~COpOscApprox()
{}

void COpOscApprox::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_APX_IN_CLEAR].IsValid() )
		if( m_poIn[OP_APX_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	m_oOsc.SetFreq( m_poIn[OP_APX_IN_FREQ].Proc() );
	
	if( m_poIn[OP_APX_IN_PHASE].IsValid() )
		m_oOsc.SetPhase( m_poIn[OP_APX_IN_PHASE].Proc() * OP_PI2 );
	else
		m_oOsc.SetPhaseZero(); // new
	
	if( m_poIn[OP_APX_IN_APPROX].IsValid() )
	{
		double dHarmon = m_poIn[OP_APX_IN_APPROX].Proc();
		if( m_dHarmon != dHarmon )
		{
			m_dHarmon = dHarmon;
			m_oOsc.SetHarmon( (unsigned int)abs( (int)rint( dHarmon )) );
		}
	}
	
	m_pdOut[OP_APX_OUT_DST] = m_oOsc.Proc();
	m_pdOut[OP_APX_OUT_SYNC] = ( m_oOsc.Sync() ? 1.0 : 0.0 );
}

void COpOscApprox::Update()
{
	COp::Update();
#ifdef OP_USE_RUNTIME_INFO
	const char * aacFuncName_[] = { "p", "t", "s" };
	SetNameOutput( OP_APX_OUT_DST, aacFuncName_[m_oOsc.Type()] );
#endif // OP_USE_RUNTIME_INFO
}

void COpOscApprox::Reset()
{
	m_oOsc.Reset();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpOscApprox::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	m_oOsc.NextType();
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpOscApprox, OP_APX_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpOscApprox )
OP_GENERIC_METHODS_DEF( COpOscApprox )

#endif // OP_INC_OP_OSC_APPROX
