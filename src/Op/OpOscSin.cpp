// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscSin.cpp

#include "OpOscSin.h"
#ifdef OP_INC_OP_OSC_SIN

////////////////////////////////////////////////////////////////////////////////
// COpOscSin
////////////////////////////////////////////////////////////////////////////////

#define OP_OSI_IN_FREQ		0
#define OP_OSI_IN_PHASE		1
#define OP_OSI_IN_CLEAR		2
#define OP_OSI_OUT_DST		0
#define OP_OSI_OUT_SYNC		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OSI_IN_FREQ_INFO	"frequency"
#define OP_OSI_IN_PHASE_INFO	"phase"
#define OP_OSI_IN_CLEAR_INFO	"reset"
#define OP_OSI_OUT_DST_INFO	"output"
#define OP_OSI_OUT_SYNC_INFO	"sync"
#define OP_OSI_CLASS_INFO \
\
"Sine Oscillator\n" \
"- The oscillator is restarted whenever the reset input value is\n" \
"  greater than 0.0.\n" \
"- When a new period begins, then the sync output value is 1.0,\n" \
"  otherwise always 0.0.\n" \
"- The phase input influences the sync signal."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OSI_IN_FREQ_INFO	"Frequenz"
#define OP_OSI_IN_PHASE_INFO	"Phase"
#define OP_OSI_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_OSI_OUT_DST_INFO	"Ausgang"
#define OP_OSI_OUT_SYNC_INFO	"Synchronisation"
#define OP_OSI_CLASS_INFO \
\
"Sinus-Oszillator\n" \
"- Wenn der Wert am Zurücksetzen-Eingang grösser 0.0 ist, wird der Oszillator\n" \
"  neu gestartet.\n" \
"- Der Synchronisations-Ausgang steht immer dann auf 1.0, wenn eine neue\n" \
"  Periode beginnt. Sonst steht er konstant auf 0.0.\n" \
"- Die Phase beeinflusst das Synchronisations-Signal."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpOscSin::COpOscSin()
: COp( 3, 2 )
{
	Init();
	OP_SET_NAME_INSTANCE( "oSin" );
	m_poIn[OP_OSI_IN_PHASE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSI_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSI_IN_FREQ, "f", OP_OSI_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSI_IN_PHASE, "p", OP_OSI_IN_PHASE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSI_IN_CLEAR, "cl", OP_OSI_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSI_OUT_DST, "o", OP_OSI_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSI_OUT_SYNC, "sy", OP_OSI_OUT_SYNC_INFO );
	Reset();
	Validate();
}

COpOscSin::~COpOscSin()
{}

void COpOscSin::Init()
{
	m_dFreq = 0.0;
	m_oOsc.SetFreq( m_dFreq );
	OP_SET_COUNT_INTERNAL_FLT( 1 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oOsc.Pos() );
	OP_SET_COUNT_INTERNAL_INT( 1 ); 
	OP_SET_PTR_INTERNAL_INT( 0, &m_oOsc.Period() );
	COp::Init();
}

void COpOscSin::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_OSI_IN_CLEAR].IsValid() )
		if( m_poIn[OP_OSI_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dFreq = m_poIn[OP_OSI_IN_FREQ].Proc();
	if( m_dFreq != dFreq )
	{
		m_oOsc.SetFreq( dFreq );
		m_dFreq = dFreq;
	}
	
	if( m_poIn[OP_OSI_IN_PHASE].IsValid() )
		m_oOsc.SetPhase( m_poIn[OP_OSI_IN_PHASE].Proc() * OP_PI2 );
	else
		m_oOsc.SetPhaseZero(); // new
	
	m_pdOut[OP_OSI_OUT_DST] = m_oOsc.Proc();
	m_pdOut[OP_OSI_OUT_SYNC] = ( m_oOsc.Sync() ? 1.0 : 0.0 );
}

void COpOscSin::Reset()
{
	m_oOsc.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpOscSin, OP_OSI_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpOscSin )
OP_GENERIC_METHODS_DEF( COpOscSin )

#endif // OP_INC_OP_OSC_SIN
