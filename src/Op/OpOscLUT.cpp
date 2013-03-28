// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscLUT.cpp

#include "OpOscLUT.h"
#ifdef OP_INC_OP_OSC_LUT

////////////////////////////////////////////////////////////////////////////////
// COpOscLUT
////////////////////////////////////////////////////////////////////////////////

#define OP_OSL_IN_FREQ		0
#define OP_OSL_IN_PHASE		1
#define OP_OSL_IN_CLEAR		2
#define OP_OSL_OUT_DST		0
#define OP_OSL_OUT_SYNC		1
#define OP_BFF_ROUTINE_WAV	0
#define OP_BFF_ROUTINE_RES	1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OSL_IN_FREQ_INFO	"frequency"
#define OP_OSL_IN_PHASE_INFO	"phase"
#define OP_OSL_IN_CLEAR_INFO	"reset"
#define OP_OSL_OUT_DST_INFO	"output"
#define OP_OSL_OUT_SYNC_INFO	"sync"
#define OP_OSL_ROUTINE_WAV_INFO	"waveform"
#define OP_OSL_ROUTINE_RES_INFO	"resolution"
#define OP_OSL_CLASS_INFO \
\
"Lookup Table Oscillator\n" \
"- The oscillator is restarted whenever the reset input value is\n" \
"  greater than 0.0.\n" \
"- When a new period begins, then the sync output value is 1.0,\n" \
"  otherwise always 0.0.\n" \
"- The phase input influences the sync signal.\n" \
"- Available waveforms: SINE, SQUARE, SAW, TRIANGLE, RANDOM."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OSL_IN_FREQ_INFO	"Frequenz"
#define OP_OSL_IN_PHASE_INFO	"Phase"
#define OP_OSL_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_OSL_OUT_DST_INFO	"Ausgang"
#define OP_OSL_OUT_SYNC_INFO	"Synchronisation"
#define OP_OSL_ROUTINE_WAV_INFO	"Wellenform"
#define OP_OSL_ROUTINE_RES_INFO	"Auflösung"
#define OP_OSL_CLASS_INFO \
\
"Lookup-Tabellen-Oszillator\n" \
"- Wenn der Wert am Zurücksetzen-Eingang grösser 0.0 ist, wird der Oszillator\n" \
"  neu gestartet.\n" \
"- Der Synchronisations-Ausgang steht immer dann auf 1.0, wenn eine neue\n" \
"  Periode beginnt.\n" \
"- Die Phase beeinflusst das Synchronisations-Signal.\n" \
"- Es kann zwischen verschienen Wennformen gewählt werden."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpOscLUT::COpOscLUT()
: COp( 3, 2 )
{
	Init();
	OP_SET_NAME_INSTANCE( "oLUT" );
	m_poIn[OP_OSL_IN_PHASE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSL_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSL_IN_FREQ, "f", OP_OSL_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSL_IN_PHASE, "p", OP_OSL_IN_PHASE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSL_IN_CLEAR, "cl", OP_OSL_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSL_OUT_DST, "o", OP_OSL_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSL_OUT_SYNC, "sy", OP_OSL_OUT_SYNC_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_BFF_ROUTINE_WAV, "w" OP_STR_ROUTINE, OP_OSL_ROUTINE_WAV_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_BFF_ROUTINE_RES, "r" OP_STR_ROUTINE, OP_OSL_ROUTINE_RES_INFO );
	Reset();
	Validate();
}

COpOscLUT::~COpOscLUT()
{}

void COpOscLUT::Init()
{
	m_dFreq = 0.0;
	m_oOsc.SetFreq( m_dFreq );
	OP_SET_COUNT_INTERNAL_FLT( 1 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oOsc.Pos() );
	OP_SET_COUNT_INTERNAL_UINT( 2 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oOsc.Type() );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_oOsc.Size() );
	OP_SET_COUNT_INTERNAL_INT( 1 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_oOsc.Period() );
	COp::Init();
}

void COpOscLUT::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_OSL_IN_CLEAR].IsValid() )
		if( m_poIn[OP_OSL_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dFreq = m_poIn[OP_OSL_IN_FREQ].Proc();
	if( m_dFreq != dFreq )
	{
		m_oOsc.SetFreq( dFreq );
		m_dFreq = dFreq;
	}
	
	if( m_poIn[OP_OSL_IN_PHASE].IsValid() )
	{
		m_oOsc.SetPhaseDirect( m_poIn[OP_OSL_IN_PHASE].Proc() ); // CAUTION: PI2 is missing, intentionally!
		//m_oOsc.SetPhase( m_poIn[OP_OSL_IN_PHASE].Proc() * OP_PI2 );
	}
	else
		m_oOsc.SetPhaseZero(); // new
	
	m_pdOut[OP_OSL_OUT_DST] = m_oOsc.Proc();
	m_pdOut[OP_OSL_OUT_SYNC] = ( m_oOsc.Sync() ? 1.0 : 0.0 );
}

void COpOscLUT::Update()
{
	m_oOsc.Init();
#ifdef OP_USE_ROUTINES
	const unsigned int uiRes = (unsigned int)( OP_LOG_N( m_oOsc.Size() / m_oOsc.LUT_SIZE_MIN, 2.0 ) );
	const unsigned int uiRes1 = ( uiRes / 10 );
	const unsigned int uiRes0 = ( uiRes - uiRes1 * 10 );
	char acName[] = " 0a";
	if( uiRes1 )
		acName[0] = '0' + uiRes1;
	acName[1] += uiRes0;
	acName[2] += m_oOsc.Type();
	SetNameOutput( OP_OSL_OUT_DST, acName );
#endif // OP_USE_ROUTINES
	COp::Update();
}

void COpOscLUT::Reset()
{
	m_oOsc.Reset();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpOscLUT::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_BFF_ROUTINE_WAV:
		m_oOsc.NextType();
	break;
	case OP_BFF_ROUTINE_RES:
		m_oOsc.NextSize();
		m_dFreq = 0.0; // Frequency and phase must be updated after every change in size! And Init() must be called (-> Update()) before.
	break;
	}
	Update();
	//Reset();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpOscLUT, OP_OSL_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpOscLUT )
OP_GENERIC_METHODS_DEF( COpOscLUT )

#endif // OP_INC_OP_OSC_LUT
