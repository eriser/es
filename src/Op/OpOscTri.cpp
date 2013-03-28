// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscTri.cpp

#include "OpOscTri.h"
#ifdef OP_INC_OP_OSC_TRI

////////////////////////////////////////////////////////////////////////////////
// COpOscTri
////////////////////////////////////////////////////////////////////////////////

#define OP_OST_IN_FREQ		0
#define OP_OST_IN_MOD		1
#define OP_OST_IN_CLEAR		2
#define OP_OST_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OST_IN_FREQ_INFO	"frequency"
#define OP_OST_IN_MOD_INFO	"modulation"
#define OP_OST_IN_CLEAR_INFO	"reset"
#define OP_OST_OUT_DST_INFO	"output"
#define OP_OST_ROUTINE_AA_INFO	"quality"
#define OP_OST_CLASS_INFO \
\
"Triangle Oscillator\n" \
"- The oscillator is restarted whenever the reset input value is\n" \
"  greater than 0.0.\n" \
"- When a new period begins, then the sync output value is 1.0,\n" \
"  otherwise always 0.0.\n" \
"- Higher quality reduces anti-aliasing, but it also increases\n" \
"  the computation time."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OST_IN_FREQ_INFO	"frequency"
#define OP_OST_IN_MOD_INFO	"modulation"
#define OP_OST_IN_CLEAR_INFO	"reset"
#define OP_OST_OUT_DST_INFO	"output"
#define OP_OST_ROUTINE_AA_INFO	"quality"
#define OP_OST_CLASS_INFO \
\
"Dreieck-Oszillator\n" \
"- Wenn der Wert am Zurücksetzen-Eingang grösser 0.0 ist, wird der Oszillator\n" \
"  neu gestartet.\n" \
"- Der Synchronisations-Ausgang steht immer dann auf 1.0, wenn eine neue\n" \
"  Periode beginnt. Sonst steht er konstant auf 0.0.\n" \
"- Besser Qualität vermindert Anti-Aliasing-Effekte, kann den Rechenaufwand\n" \
"  aber stark erhöhen."

#else
#error language not specified
#endif // OP_LANGUAGE_*


COpOscTri::COpOscTri()
: COp( 3, 1 )
, COpTkFltAA44100<double>()
, m_uiFilterIndex( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "oTri" );
	m_poIn[OP_OST_IN_MOD].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OST_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OST_IN_FREQ, "f", OP_OST_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OST_IN_MOD, "m", OP_OST_IN_MOD_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OST_IN_CLEAR, "cl", OP_OST_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OST_OUT_DST, "o", OP_OST_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "o" OP_STR_ROUTINE, OP_OST_ROUTINE_AA_INFO );
	Reset();
	Update();
	Validate();
	InitFreq( 0.0 );
}

COpOscTri::COpOscTri( const COpOscTri &roOp )
: COp( 0, 0 )
, COpTkFltAA44100<double>()
{
	Init();
	*this = roOp;
	Validate();
}

COpOscTri::~COpOscTri()
{}

COpOscTri & COpOscTri::operator = ( const COpOscTri &roOp )
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
	
	m_uiOverSample = roOp.m_uiOverSample;
	m_uiFilterSize = roOp.m_uiFilterSize;
	m_uiBufferInPos = roOp.m_uiBufferInPos;
	//m_adBufferIn; // Nicht nötig.
	
	Update();
	return *this;
}

void COpOscTri::Init()
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

void COpOscTri::InitFreq( double dFreq )
{
	m_dFreqLast = dFreq;
	
	// Anteil von 2*Pi, der pro Sample addiert wird.
	m_dFTStp    = dFreq * OP_PI2_D_SAMPLE_FREQ;
	m_dFTStpAA  = m_dFTStp / m_uiOverSample;
	
	const double dFreqAbs2 = 2.0 * fabs( dFreq );
	
	// Amplituden-Schritt, um in einer halben Periode 2.0 aufzusummieren 
	// (von -1.0 bis +1.0 oder umgekehrt).
	m_dSStp = ( ( m_uiSgnLast ? -2.0 : +2.0 ) * dFreqAbs2 ) 
		/ double( m_uiOverSample * OP_SAMPLE_FREQ );
	
	// Anzahl Samples pro halber Periode.
	// Es ist nur bei den paar letzten Samples einer halben Periode
	// nötig, die Winkelfunktion zu berechnen, welche dann den genauen
	// Zeitpunkt bestimmt, wann die nachste Halbperiode beginnt.
	m_uiSampleMax = 0;
	if( dFreqAbs2 )
	{
		const unsigned int uiMax = 
			(unsigned int)( double( m_uiOverSample * OP_SAMPLE_FREQ ) / dFreqAbs2 );
		if( uiMax > 12 )
			m_uiSampleMax = uiMax - 12;
		
		// Nach Frequenz-Änderung immer alles berechnen, 
		// denn es wird nicht an Extrempunkt, sondern bei 0 begonnen!
		m_uiSample = uiMax;
	}
	// Bei Einer apprupten Änderung auf 0 Hertz bleibt der aktuelle Wert in m_dSample
	// einfach konstant. Das ist so korrekt, da die Mod ungleich Null ist.
	// else 
	// 	Reset();
}

void COpOscTri::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_OST_IN_CLEAR].IsValid() )
		if( m_poIn[OP_OST_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dFreq =  m_poIn[OP_OST_IN_FREQ].Proc();
	if( dFreq != m_dFreqLast )
		InitFreq( dFreq );
	
	// Nur einmal hier prüfen, da Oversamplingrate und die Filtergrösse 2er-Potenzen sind!
	// Das geht immer schön auf, und innerhalb der Oversample-Schleife kommt es zu keinem Underrun.
	if( !m_uiBufferInPos )
		m_uiBufferInPos = m_uiFilterSize;	
	double *pdB = m_atBufferIn + m_uiBufferInPos;
	double dFT = m_dFT;
	unsigned int i = m_uiOverSample;
	
	if( m_poIn[OP_OST_IN_MOD].IsValid() )
	{
		dFT += OP_PI2 * m_poIn[OP_OST_IN_MOD].Proc();
		
		const double dQ( dFreq != 0.0 ? ( m_dFT / dFreq ) : 0.0 ); // hack
		do
		{
			--i;
			if( m_uiSgnLast != ( cos( dFT ) >= 0.0 ) )
			{		
				m_dSStp = -m_dSStp;
				m_uiSgnLast = !m_uiSgnLast;
				m_dSample = ( m_uiSgnLast ? +1.0 : -1.0 );
			}
			
			--pdB;
			*pdB = m_dSample;
			if( dQ )
			{
				m_dSample += ( ( m_uiSgnLast ? -2.0 : +2.0 ) * 2.0 * fabs( dFT / dQ ) ) 
						/ double( m_uiOverSample * OP_SAMPLE_FREQ );
			}
			dFT += m_dFTStpAA;
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
				
				m_dSStp = -m_dSStp;
				m_uiSgnLast = !m_uiSgnLast;
				m_dSample = ( m_uiSgnLast )?( +1.0 ):( -1.0 );
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
	
	m_pdOut[OP_OST_OUT_DST] = -COpTkFltAA44100<double>::Filter(); //*pdB // hack: invert!
}

void COpOscTri::Update()
{
	m_uiFilterIndex = COpTkFltAA44100<double>::SetFilter( m_uiFilterIndex );
	
#ifdef OP_USE_ROUTINES
	char acName[] = "a";
	acName[0] += m_uiFilterIndex;
	SetNameOutput( OP_OST_OUT_DST, acName );
#endif
}

void COpOscTri::Reset()
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
void COpOscTri::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	++m_uiFilterIndex;
	Update();
	Reset();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpOscTri, OP_OST_CLASS_INFO )
//OP_GENERIC_COPY_CTOR_DEF( COpOscTri )
OP_GENERIC_METHODS_DEF( COpOscTri )

#endif // OP_INC_OP_OSC_TRI
