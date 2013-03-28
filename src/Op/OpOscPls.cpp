// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscPls.cpp

#include "OpOscPls.h"
#ifdef OP_INC_OP_OSC_PLS

////////////////////////////////////////////////////////////////////////////////
// COpOscPls
////////////////////////////////////////////////////////////////////////////////

#define OP_ONP_IN_FREQ			0
#define OP_ONP_IN_MOD			1
#define OP_ONP_IN_PULSEW		2
#define OP_ONP_IN_CLEAR			3
#define OP_ONP_OUT_DST			0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_ONP_IN_FREQ_INFO		"frequency"
#define OP_ONP_IN_MOD_INFO		"modulation"
#define OP_ONP_IN_PULSEW_INFO		"pulse width"
#define OP_ONP_IN_CLEAR_INFO		"reset"
#define OP_ONP_OUT_DST_INFO		"output"
#define OP_ONP_ROUTINE_FUNC_INFO	"quality"
#define OP_ONP_CLASS_INFO \
\
"Pulse Oscillator\n " \
"- The pulse width has be lie within [-1.0,1.0].\n" \
"     pw = 0.0 produces a square wave,\n" \
"     pw < 0.0 increases the negative side,\n" \
"     pw > 0.0 increases the positive side.\n" \
"- The pulse width is mapped onto the wavelength based on the sine function:\n" \
"     pw_conv = ( ( PI - asin( pw ) ) - asin( pw ) ) / ( 2 * PI )\n" \
"- The oscillator is restarted whenever the reset input value is\n" \
"  greater than 0.0.\n" \
"- When a new period begins, then the sync output value is 1.0,\n" \
"  otherwise always 0.0.\n" \
"- Higher quality reduces anti-aliasing, but it also increases\n" \
"  the computation time."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_ONP_IN_FREQ_INFO		"Frequenz"
#define OP_ONP_IN_MOD_INFO		"Modulation"
#define OP_ONP_IN_PULSEW_INFO		"Pulsweite"
#define OP_ONP_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_ONP_OUT_DST_INFO		"Ausgang"
#define OP_ONP_ROUTINE_FUNC_INFO	"Qualität"
#define OP_ONP_CLASS_INFO \
\
"Rechteck-Oszillator\n " \
"- Die Pulsweite wird zwischen -1.0 und +1.0 angegeben.\n" \
"     pw = 0.0 entspricht einer Square,\n" \
"     pw < 0.0 vergrössert die negative Flake,\n" \
"     pw > 0.0 vergrössert die positive Flake.\n" \
"  Das Verhalten ist nicht linear, sondern sinusförmig.\n" \
"     pw_conv = ( ( PI - asin( pw ) ) - asin( pw ) ) / ( 2 * PI )\n" \
"- Wenn der Wert am Zurücksetzen-Eingang grösser 0.0 ist, wird der Oszillator\n" \
"  neu gestartet.\n" \
"- Der Synchronisations-Ausgang steht immer dann auf 1.0, wenn eine neue\n" \
"  Periode beginnt. Sonst steht er konstant auf 0.0.\n" \
"- Besser Qualität vermindert Anti-Aliasing-Effekte, kann den Rechenaufwand\n" \
"  aber stark erhöhen."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpOscPls::COpOscPls()
: COp( 4, 1 )
, COpTkFltAA44100<double>()
, m_uiFilterIndex( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "oPls" );
	m_poIn[OP_ONP_IN_MOD].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_ONP_IN_PULSEW].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_ONP_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_ONP_IN_FREQ, "f", OP_ONP_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_ONP_IN_MOD, "m", OP_ONP_IN_MOD_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_ONP_IN_PULSEW, "pw", OP_ONP_IN_PULSEW_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_ONP_IN_CLEAR, "cl", OP_ONP_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_ONP_OUT_DST, "o", OP_ONP_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "" OP_STR_ROUTINE, OP_ONP_ROUTINE_FUNC_INFO );
	Reset();
	Update();
	Validate();
	InitFreq( 0.0 );
}

COpOscPls::COpOscPls( const COpOscPls &roOp )
: COp( 0, 0 )
, COpTkFltAA44100<double>()
{
	Init();
	*this = roOp;
	Update();
	Validate();
}

COpOscPls::~COpOscPls()
{}

COpOscPls & COpOscPls::operator = ( const COpOscPls &roOp )
{
	COp::operator = ( roOp );
	COpTkFltAA44100<double>::operator = ( roOp );
	
	m_dFTStp = roOp.m_dFTStp;
	m_dFTStpAA = roOp.m_dFTStpAA;
	m_dFreqLast = roOp.m_dFreqLast;
	//m_dFT; // Internal.
	//m_dSample = roOp.m_dSample; // Internal.
	//m_uiSample // Internal.
	m_uiSampleMax = roOp.m_uiSampleMax;
	//m_uiFilterIndex = roOp.m_uiFilterIndex; // Internal.
	
	m_uiOverSample = roOp.m_uiOverSample;
	m_uiFilterSize = roOp.m_uiFilterSize;
	m_uiBufferInPos = roOp.m_uiBufferInPos;
	//m_adBufferIn; // Nicht nötig.
	
	Update();
	return *this;
}

void COpOscPls::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 2 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiSample );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_uiFilterIndex );
	OP_SET_COUNT_INTERNAL_FLT( 2 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dFT );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_dSample );
	COp::Init();
}

void COpOscPls::InitFreq( double dFreq )
{
	m_dFreqLast = dFreq;
	
	// Anteil von 2*Pi, der pro Sample addiert wird.
	m_dFTStp    = dFreq * OP_PI2_D_SAMPLE_FREQ;
	m_dFTStpAA  = m_dFTStp / m_uiOverSample;
	
	// Anzahl Samples pro halber Periode.
	// Es ist nur bei den paar letzten Samples einer halben Periode
	// nötig, die Winkelfunktion zu berechnen, welche dann den genauen
	// Zeitpunkt bestimmt, wann die nachste Halbperiode beginnt.
	const double dFreqAbs = fabs( dFreq ); 
	m_uiSampleMax = 0;
	if( dFreqAbs )
	{
		const unsigned int uiMax = 
			(unsigned int)( double( m_uiOverSample * OP_SAMPLE_FREQ ) / (2.0 * dFreqAbs) );
		if( uiMax > 12 )
			m_uiSampleMax = uiMax - 12;
	}
	// Bei Einer apprupten Änderung auf 0 Hertz bleibt der aktuelle Wert in m_dSample
	// einfach konstant. Das ist so korrekt, da die Mod ungleich Null ist.
	// else 
	// 	Reset();
}

void COpOscPls::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_ONP_IN_CLEAR].IsValid() )
		if( m_poIn[OP_ONP_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dFreq =  m_poIn[OP_ONP_IN_FREQ].Proc();
	if( dFreq != m_dFreqLast )
		InitFreq( dFreq );	
	
	// Nur einmal hier prüfen, da Oversamplingrate und die Filtergrösse 2er-Potenzen sind!
	// Das geht immer schön auf, und innerhalb der Oversample-Schleife kommt es zu keinem Underrun.
	if( !m_uiBufferInPos )
		m_uiBufferInPos = m_uiFilterSize;
	
	double *pdB = m_atBufferIn + m_uiBufferInPos;
	double dFT = m_dFT;
	unsigned int i = m_uiOverSample;
	
	const bool bIsValidMod = m_poIn[OP_ONP_IN_MOD].IsValid();
	const bool bIsValidPulseWidth =  m_poIn[OP_ONP_IN_PULSEW].IsValid();
	if( bIsValidMod || bIsValidPulseWidth )
	{
		if( bIsValidMod )
			dFT += OP_PI2 * m_poIn[OP_ONP_IN_MOD].Proc();
		
		double dPulseWidth = 0.0;
		if( bIsValidPulseWidth )
			dPulseWidth = fmod( m_poIn[OP_ONP_IN_PULSEW].Proc(), 1.0 );
		
		do
		{
			--i;
			--pdB;
			if( sin( dFT ) < dPulseWidth )
				*pdB = -1.0;
			else
				*pdB = +1.0;
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
			--pdB;
			
			// ACHTUNG: m_dSample ist bei den erste paar Samples
			// nach der Mod-Port-Toggle nicht korrekt, da
			// oben direkt in den Puffer geschrieben wird!
			if( m_uiSample >= m_uiSampleMax )
			{
				if( ( sin( dFT ) < 0.0 ) != ( m_dSample < 0.0 ) )
				{
					m_dSample = -m_dSample;
					//printf( "%d\n", m_uiSample );
					m_uiSample = 0;
				}
			}
			++m_uiSample;
			
			*pdB = m_dSample;
			dFT += m_dFTStpAA;
		}
		while( i );
	}
	m_uiBufferInPos -= m_uiOverSample;
	m_dFT += m_dFTStp;
	
	m_pdOut[OP_ONP_OUT_DST] = COpTkFltAA44100<double>::Filter(); //*pdB
}

void COpOscPls::Update()
{
	m_uiFilterIndex = COpTkFltAA44100<double>::SetFilter( m_uiFilterIndex );
	
#ifdef OP_USE_ROUTINES
	char acName[] = "a";
	acName[0] += m_uiFilterIndex;
	SetNameOutput( OP_ONP_OUT_DST, acName );
#endif
}

void COpOscPls::Reset()
{
	m_dFreqLast = 0.0; // Update in Proc() erzwingen.
	m_dSample = +1.0; // Wichtig!
	m_dFT = 0.0;
	m_uiSample = 0;
	m_uiSampleMax = 0;
	COpTkFltAA44100<double>::ResetFilter(); // m_uiBufferInPos wird auf 0 gesetzt.
}

#ifdef OP_USE_ROUTINES
void COpOscPls::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	++m_uiFilterIndex;
	Update();
	Reset();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpOscPls, OP_ONP_CLASS_INFO )
//OP_GENERIC_COPY_CTOP_DEF( COpOscPls )
OP_GENERIC_METHODS_DEF( COpOscPls )

#endif // OP_INC_OP_OSC_PLS
