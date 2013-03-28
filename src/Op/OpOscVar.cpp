// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscVar.cpp

#include "OpOscVar.h"
#ifdef OP_INC_OP_OSC_VAR

////////////////////////////////////////////////////////////////////////////////
// COpOscVar
////////////////////////////////////////////////////////////////////////////////

#define OP_OSV_IN_FREQ		0
#define OP_OSV_IN_PHASE		1
#define OP_OSV_IN_CLEAR		2
#define OP_OSV_OUT_DST		0
#define OP_OSV_OUT_SYNC		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OSV_IN_FREQ_INFO	"frequency"
#define OP_OSV_IN_PHASE_INFO	"phase"
#define OP_OSV_IN_CLEAR_INFO	"reset"
#define OP_OSV_OUT_DST_INFO	"output"
#define OP_OSV_OUT_SYNC_INFO	"sync"
#define OP_OSV_ROUTINE_WAV_INFO	"waveform"
#define OP_OSV_CLASS_INFO \
\
"Variable Oscillator\n" \
"- The oscillator is restarted whenever the reset input value is\n" \
"  greater than 0.0.\n" \
"- When a new period begins, then the sync output value is 1.0,\n" \
"  otherwise always 0.0.\n" \
"- The phase input influences the sync signal.\n" \
"- Available waveforms: SINE, SQUARE, SAW."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OSV_IN_FREQ_INFO	"Frequenz"
#define OP_OSV_IN_PHASE_INFO	"Phase"
#define OP_OSV_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_OSV_OUT_DST_INFO	"Ausgang"
#define OP_OSV_OUT_SYNC_INFO	"Synchronisation"
#define OP_OSV_ROUTINE_WAV_INFO	"Wellenform"
#define OP_OSV_CLASS_INFO \
\
"Variabler Oszillator\n" \
"- Wenn der Wert am Zurücksetzen-Eingang grösser 0.0 ist, wird der Oszillator\n" \
"  neu gestartet.\n" \
"- Der Synchronisations-Ausgang steht immer dann auf 1.0, wenn eine neue\n" \
"  Periode beginnt.\n" \
"- Die Phase beeinflusst das Synchronisations-Signal.\n" \
"- Es kann zwischen verschienen Wennformen gewählt werden."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpOscVar::COpOscVar()
: COp( 3, 2 )
{
	Init();
	OP_SET_NAME_INSTANCE( "oVar" );
	m_poIn[OP_OSV_IN_PHASE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSV_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSV_IN_FREQ, "f", OP_OSV_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSV_IN_PHASE, "p", OP_OSV_IN_PHASE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSV_IN_CLEAR, "cl", OP_OSV_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSV_OUT_DST, "o", OP_OSV_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSV_OUT_SYNC, "sy", OP_OSV_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "w" OP_STR_ROUTINE, OP_OSV_ROUTINE_WAV_INFO );
	Reset();
	Validate();
}

COpOscVar::~COpOscVar()
{}

void COpOscVar::Init()
{
	m_dFreq = 0.0;
	m_oOsc.SetFreq( m_dFreq );
	OP_SET_COUNT_INTERNAL_FLT( 1 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oOsc.Pos() );
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oOsc.Type() );
	OP_SET_COUNT_INTERNAL_INT( 1 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_oOsc.Period() );
	COp::Init();
}

void COpOscVar::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_OSV_IN_CLEAR].IsValid() )
		if( m_poIn[OP_OSV_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dFreq = m_poIn[OP_OSV_IN_FREQ].Proc();
	if( m_dFreq != dFreq )
	{
		m_oOsc.SetFreq( dFreq );
		m_dFreq = dFreq;
	}
	
	if( m_poIn[OP_OSV_IN_PHASE].IsValid() )
		m_oOsc.SetPhase( m_poIn[OP_OSV_IN_PHASE].Proc() * OP_PI2 );
	else
		m_oOsc.SetPhaseZero(); // new
	
	m_pdOut[OP_OSV_OUT_DST] = m_oOsc.Proc();
	m_pdOut[OP_OSV_OUT_SYNC] = ( m_oOsc.Sync() ? 1.0 : 0.0 );
}

void COpOscVar::Update()
{
#ifdef OP_USE_ROUTINES
	char acName[] = "a";
	acName[0] += m_oOsc.Type();
	SetNameOutput( OP_OSV_OUT_DST, acName );
#endif // OP_USE_ROUTINES
	COp::Update();
}

void COpOscVar::Reset()
{
	m_oOsc.Reset();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpOscVar::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	m_oOsc.NextType();
	Update();
	Reset();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpOscVar, OP_OSV_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpOscVar )
OP_GENERIC_METHODS_DEF( COpOscVar )

#endif // OP_INC_OP_OSC_VAR
