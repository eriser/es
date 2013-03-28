// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscSaw.cpp

#include "OpOscSaw.h"
#ifdef OP_INC_OP_OSC_SAW

////////////////////////////////////////////////////////////////////////////////
// COpOscSaw
////////////////////////////////////////////////////////////////////////////////

#define OP_OSA_IN_FREQ		0
#define OP_OSA_IN_MOD		1
#define OP_OSA_IN_CLEAR		2
#define OP_OSA_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OSA_IN_FREQ_INFO	"frequency"
#define OP_OSA_IN_MOD_INFO	"modulation"
#define OP_OSA_IN_CLEAR_INFO	"reset"
#define OP_OSA_OUT_DST_INFO	"output"
#define OP_OSA_ROUTINE_AA_INFO	"quality"
#define OP_OSA_CLASS_INFO \
\
"Rising Sawtooth Oscillator\n" \
"- The oscillator is restarted whenever the reset input value is\n" \
"  greater than 0.0.\n" \
"- When a new period begins, then the sync output value is 1.0,\n" \
"  otherwise always 0.0.\n" \
"- Higher quality reduces anti-aliasing, but it also increases\n" \
"  the computation time."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OSA_IN_FREQ_INFO	"Frequenz"
#define OP_OSA_IN_MOD_INFO	"Modulation"
#define OP_OSA_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_OSA_OUT_DST_INFO	"Ausgang"
#define OP_OSA_ROUTINE_AA_INFO	"Qualität"
#define OP_OSA_CLASS_INFO \
\
"Steigender Sägezahn-Oszillator\n" \
"- Wenn der Wert am Zurücksetzen-Eingang grösser 0.0 ist, wird der Oszillator\n" \
"  neu gestartet.\n" \
"- Der Synchronisations-Ausgang steht immer dann auf 1.0, wenn eine neue\n" \
"  Periode beginnt. Sonst steht er konstant auf 0.0.\n" \
"- Besser Qualität vermindert Anti-Aliasing-Effekte, kann den Rechenaufwand\n" \
"  aber stark erhöhen."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpOscSaw::COpOscSaw()
: COp( 3, 1 )
, COpTkFltAA44100<double>()
, m_uiFilterIndex( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "oSaw" );
	m_poIn[OP_OSA_IN_MOD].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSA_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSA_IN_FREQ, "f", OP_OSA_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSA_IN_MOD, "m", OP_OSA_IN_MOD_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSA_IN_CLEAR, "cl", OP_OSA_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSA_OUT_DST, "o", OP_OSA_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "o" OP_STR_ROUTINE, OP_OSA_ROUTINE_AA_INFO );
	Reset();
	Update();
	Validate();
	InitFreq( 0.0 );
}

COpOscSaw::COpOscSaw( const COpOscSaw &roOp )
: COp( 0, 0 )
, COpTkFltAA44100<double>()
{
	Init();
	*this = roOp;
	Update();
	Validate();
}

COpOscSaw::~COpOscSaw()
{}

COpOscSaw & COpOscSaw::operator=( const COpOscSaw &roOp )
{
	COp::operator = ( roOp );
	COpTkFltAA44100<double>::operator = ( roOp );

	//m_dFT; // Internal.
	m_dSStp = roOp.m_dSStp;
	m_dFTStp = roOp.m_dFTStp;
	m_dFTStpAA = roOp.m_dFTStpAA;
	m_dFreqLast = roOp.m_dFreqLast;
	//m_dSample; // Internal.
	//m_uiSgnLast; // Internal.
	//m_uiSample; // Internal.
	m_uiSampleMax = roOp.m_uiSampleMax; 
	//m_uiFilterIndex = roOp.m_uiFilterIndex; // Internal.
	//m_bIsValidInputMod; // In Validate().
	//m_bIsValidInputPulseWidth; // In Validate().
	//m_bIsValidInputReset; // In Validate().
	
	m_uiOverSample = roOp.m_uiOverSample;
	m_uiFilterSize = roOp.m_uiFilterSize;
	m_uiBufferInPos = roOp.m_uiBufferInPos;
	//m_atBufferIn; // Nicht nötig.

	Update();
	return *this;
}

void COpOscSaw::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 3 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiSgnLast );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_uiSample );
	OP_SET_PTR_INTERNAL_UINT( 2, &m_uiFilterIndex );
	OP_SET_COUNT_INTERNAL_FLT( 2 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dSample );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_dFT );
	COp::Init();
}

void COpOscSaw::InitFreq( double dFreq )
{
	m_dFreqLast = dFreq;
	
	// Anteil von 2*Pi, der pro Sample addiert wird.
	// ACHTUNG: Nicht OP_PI2! 
	m_dFTStp    = dFreq * OP_PI_D_SAMPLE_FREQ;
	m_dFTStpAA  = m_dFTStp / m_uiOverSample;
	
	//const double dFreqAbs2 = 2.0 * fabs( dFreq );
	const double dFreqAbs = fabs( dFreq );
	
	// Amplituden-Schritt, um in einer Periode 2.0 aufzusummieren.
	m_dSStp = ( /*( m_uiSgnLast ? -2.0 : +2.0 ) * */ dFreqAbs * 2.0 ) 
		/ double( m_uiOverSample * OP_SAMPLE_FREQ );
	
	// Anzahl Samples pro Periode.
	// Es ist nur bei den paar letzten Samples einer Periode
	// nötig, die Winkelfunktion zu berechnen, welche dann den genauen
	// Zeitpunkt bestimmt, wann die nachste Zyklus beginnt.
	m_uiSampleMax = 0;
	if( dFreqAbs  )
	{
		const unsigned int uiMax = 
			(unsigned int)( double( m_uiOverSample * OP_SAMPLE_FREQ ) / dFreqAbs );
		if( uiMax > 12 )
			m_uiSampleMax = uiMax - 12;
		
		// Nach Frequenz-Änderung immer alles berechnen, 
		// denn es wird nicht an Extrempunkt, sondern bei 0 begonnen!
		m_uiSample = uiMax;
	}
	// Bei Einer abrupten Änderung auf 0 Hertz bleibt der aktuelle Wert in m_dSample
	// einfach konstant. Das ist so korrekt, da die Mod ungleich Null ist.
	// else 
	// 	Reset();
}

void COpOscSaw::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_OSA_IN_CLEAR].IsValid() )
		if( m_poIn[OP_OSA_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dFreq =  m_poIn[OP_OSA_IN_FREQ].Proc();
	if( dFreq != m_dFreqLast )
		InitFreq( dFreq );
	
	// Nur einmal hier prüfen, da Oversamplingrate und die Filtergrösse 2er-Potenzen sind!
	// Das geht immer schön auf, und innerhalb der Oversample-Schleife kommt es zu keinem Underrun.
	if( !m_uiBufferInPos )
		m_uiBufferInPos = m_uiFilterSize;	
	double *pdB = m_atBufferIn + m_uiBufferInPos;
	double dFT = m_dFT;
	unsigned int i = m_uiOverSample;
	
	if( m_poIn[OP_OSA_IN_MOD].IsValid() )
	{
		dFT += OP_PI2 * m_poIn[OP_OSA_IN_MOD].Proc(); // dFT += dMod;
		
		do
		{
			--i;
			if( m_uiSgnLast != ( cos( dFT ) >= 0.0 ) )
			{		
				//m_dSStp = -m_dSStp;
				m_uiSgnLast = !m_uiSgnLast;
				//m_dSample = ( m_uiSgnLast )?( +1.0 ):( -1.0 );
				m_dSample = -1.0;
			}
			
			--pdB;
			*pdB = m_dSample;
			dFT += m_dFTStpAA;
			m_dSample += m_dSStp;
		}
		while( i );
		m_uiSample += m_uiOverSample;
	}	
	else
	{
		do
		{
			--i;
			if( m_uiSample >= m_uiSampleMax && 
			    m_uiSgnLast != (unsigned int)( cos( dFT ) >= 0.0 ) )
			{				
				m_uiSample = 0;
				
				//m_dSStp = -m_dSStp;
				m_uiSgnLast = !m_uiSgnLast;
				//m_dSample = ( m_uiSgnLast )?( +1.0 ):( -1.0 );
				m_dSample = -1.0;
			}
			++m_uiSample;
			
			--pdB;
			*pdB = m_dSample;
			dFT += m_dFTStpAA;
			m_dSample += m_dSStp;
		}
		while( i );
	}
	
	m_uiBufferInPos -= m_uiOverSample;
	m_dFT += m_dFTStp;
	
	m_pdOut[OP_OSA_OUT_DST] = COpTkFltAA44100<double>::Filter(); //*pdB
}

void COpOscSaw::Update()
{
	m_uiFilterIndex = COpTkFltAA44100<double>::SetFilter( m_uiFilterIndex );
	
#ifdef OP_USE_ROUTINES
	char acName[] = "a";
	acName[0] += m_uiFilterIndex;
	SetNameOutput( OP_OSA_OUT_DST, acName );
#endif
}

void COpOscSaw::Reset()
{
	m_dFreqLast = 0.0; // Update in Proc() erzwingen.
	m_dSample = 0.0;
	m_dFT = 0.0;
	m_uiSgnLast = 1;
	m_uiSample = 0;
	m_uiSampleMax = 0;
	COpTkFltAA44100<double>::ResetFilter();
}

#ifdef OP_USE_ROUTINES
void COpOscSaw::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	++m_uiFilterIndex;
	Update();
	Reset();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpOscSaw, OP_OSA_CLASS_INFO )
//OP_GENERIC_COPY_CTOR_DEF( COpOscSaw )
OP_GENERIC_METHODS_DEF( COpOscSaw )

#endif // OP_INC_OP_OSC_SAW 
