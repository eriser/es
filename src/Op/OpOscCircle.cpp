// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscCircle.cpp

#include "OpOscCircle.h"
#ifdef OP_INC_OP_OSC_CIRCLE

////////////////////////////////////////////////////////////////////////////////
// COpOscCircle
////////////////////////////////////////////////////////////////////////////////

#define OP_OCR_IN_FREQ		0
#define OP_OCR_IN_PHASE		1
#define OP_OCR_IN_CLEAR		2
#define OP_OCR_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OCR_IN_FREQ_INFO	"frequency"
#define OP_OCR_IN_PHASE_INFO	"phase"
#define OP_OCR_IN_CLEAR_INFO	"reset"
#define OP_OCR_OUT_DST_INFO	"output"
#define OP_OCR_ROUTINE_AA_INFO	"quality"
#define OP_OCR_CLASS_INFO \
\
"Circle Oscillator\n" \
"- The oscillator is restarted whenever the reset input value is\n" \
"  greater than 0.0.\n" \
"- When a new period begins, then the sync output value is 1.0,\n" \
"  otherwise always 0.0.\n" \
"- Higher quality reduces anti-aliasing, but it also increases\n" \
"  the computation time."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OCR_IN_FREQ_INFO	"Frequenz"
#define OP_OCR_IN_PHASE_INFO	"Phase"
#define OP_OCR_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_OCR_OUT_DST_INFO	"Ausgang"
#define OP_OCR_ROUTINE_AA_INFO	"Qualität"
#define OP_OCR_CLASS_INFO \
\
"Kreis-Oszillator\n" \
"- Wenn der Wert am Zurücksetzen-Eingang grösser 0.0 ist, wird der Oszillator\n" \
"  neu gestartet.\n" \
"- Der Synchronisations-Ausgang steht immer dann auf 1.0, wenn eine neue\n" \
"  Periode beginnt. Sonst steht er konstant auf 0.0.\n" \
"- Besser Qualität vermindert Anti-Aliasing-Effekte, kann den Rechenaufwand\n" \
"  aber stark erhöhen."

#else
#error language not specified
#endif // OP_LANGUAGE_*


COpOscCircle::COpOscCircle()
: COp( 3, 1 )
, COpTkFltAA44100<double>()
, m_uiFilterIndex( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "oCir" );
	m_poIn[OP_OCR_IN_PHASE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OCR_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OCR_IN_FREQ, "f", OP_OCR_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OCR_IN_PHASE, "p", OP_OCR_IN_PHASE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OCR_IN_CLEAR, "cl", OP_OCR_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OCR_OUT_DST, "o", OP_OCR_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "o" OP_STR_ROUTINE, OP_OCR_ROUTINE_AA_INFO );
	Reset();
	Update();
	Validate();
	InitFreq( 0.0 );
}

COpOscCircle::COpOscCircle( const COpOscCircle &roOp )
: COp( 0, 0 )
, COpTkFltAA44100<double>()
{
	Init();
	*this = roOp;
	Update();
	Validate();
}

COpOscCircle::~COpOscCircle()
{}

COpOscCircle & COpOscCircle::operator = ( const COpOscCircle &roO )
{
	COp::operator =( roO );
	COpTkFltAA44100<double>::operator = ( roO );
	
	//m_dFT; // Internal.
	m_dFTStp = roO.m_dFTStp;
	m_dFTStpAA = roO.m_dFTStpAA;
	m_dFreqLast = roO.m_dFreqLast;
	//m_uiFilterIndex; // Internal.
	
	m_uiOverSample = roO.m_uiOverSample;
	m_uiFilterSize = roO.m_uiFilterSize;
	m_uiBufferInPos = roO.m_uiBufferInPos;
	//m_adBufferIn; // Nicht nötig.
	
	Update();
	return *this;
}

void COpOscCircle::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFilterIndex );
	OP_SET_COUNT_INTERNAL_FLT( 1 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dFT );
	COp::Init();
}

void COpOscCircle::InitFreq( double dFreq )
{
	m_dFreqLast = dFreq;
	
	// Anteil von 1, der pro Sample addiert wird.
	m_dFTStp    = 4.0 * fabs( dFreq ) * OP_ONE_D_SAMPLE_FREQ;
	m_dFTStpAA  = m_dFTStp / m_uiOverSample;
}

void COpOscCircle::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_OCR_IN_CLEAR].IsValid() )
		if( m_poIn[OP_OCR_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dFreq =  m_poIn[OP_OCR_IN_FREQ].Proc();
	if( dFreq != m_dFreqLast )
		InitFreq( dFreq );
	
	// Nur einmal hier prüfen, da Oversamplingrate und die Filtergrösse 2er-Potenzen sind!
	// Das geht immer schön auf, und innerhalb der Oversample-Schleife kommt es zu keinem Underrun.
	if( !m_uiBufferInPos )
		m_uiBufferInPos = m_uiFilterSize;	
	double *pdB = m_atBufferIn + m_uiBufferInPos;
	double dFT = m_dFT;
	unsigned int i = m_uiOverSample;
	
	if( m_poIn[OP_OCR_IN_PHASE].IsValid() )
	{
		dFT += m_poIn[OP_OCR_IN_PHASE].Proc();
		
		do
		{
			--i;
			--pdB;
			const double dT = fmod( dFT, 2.0 ) - 1.0;
			*pdB = ( ( fmod( dFT, 4.0 ) <= 2.0 ) ? +1.0 : -1.0 ) * sqrt( 1.0 - dT * dT );
			dFT += m_dFTStpAA;
		}
		while( i );
	}	
	else
	{
		do
		{
			--i;
			--pdB;
			const double dT = fmod( dFT, 2.0 ) - 1.0;
			*pdB = ( ( fmod( dFT, 4.0 ) <= 2.0 ) ? +1.0 : -1.0 ) * sqrt( 1.0 - dT * dT );
			dFT += m_dFTStpAA;
		}
		while( i );
	}
	
	m_uiBufferInPos -= m_uiOverSample;
	m_dFT += m_dFTStp;
	
	m_pdOut[OP_OCR_OUT_DST] = COpTkFltAA44100<double>::Filter(); //*pdB
}

void COpOscCircle::Update()
{
	m_uiFilterIndex = COpTkFltAA44100<double>::SetFilter( m_uiFilterIndex );
	
#ifdef OP_USE_ROUTINES
	char acName[] = "a";
	acName[0] += m_uiFilterIndex;
	SetNameOutput( OP_OCR_OUT_DST, acName );
#endif
}

void COpOscCircle::Reset()
{
	m_dFreqLast = 0.0; // Update in Proc() erzwingen.
	m_dFT = 0.0;
	COpTkFltAA44100<double>::ResetFilter();
}

#ifdef OP_USE_ROUTINES
void COpOscCircle::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	++m_uiFilterIndex;
	Update();
	Reset();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpOscCircle, OP_OCR_CLASS_INFO )
//OP_GENERIC_COPY_CTOR_DEF( COpOscCircle )
OP_GENERIC_METHODS_DEF( COpOscCircle )

#endif // OP_INC_OP_OSC_CIRCLE
