// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpPhaser.cpp

#include "OpPhaser.h"
#ifdef OP_INC_OP_PHASER

////////////////////////////////////////////////////////////////////////////////
// COpPhaser
////////////////////////////////////////////////////////////////////////////////

#define OP_PHS_IN_SRC			0
#define OP_PHS_IN_MIX			1
#define OP_PHS_IN_FEEDB			2
#define OP_PHS_IN_STAGES		3
#define OP_PHS_IN_DELAY			4
#define OP_PHS_IN_LFORATIO		5
#define OP_PHS_IN_LFO			6
#define OP_PHS_IN_CLEAR			7
#define OP_PHS_OUT_DST			0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_PHS_IN_SRC_INFO		"input"
#define OP_PHS_IN_MIX_INFO		"mix"
#define OP_PHS_IN_FEEDB_INFO		"feedback"
#define OP_PHS_IN_STAGES_INFO		"number of filters"
#define OP_PHS_IN_DELAY_INFO		"delay"
#define OP_PHS_IN_LFORATIO_INFO		"LFO ratio"
#define OP_PHS_IN_LFO_INFO		"LFO input"
#define OP_PHS_IN_CLEAR_INFO		"reset"
#define OP_PHS_OUT_DST_INFO		"output"
#define OP_PHS_CLASS_INFO \
\
"Phaser\n" \
"- Input values pass through the given number of allpass filters, and are\n" \
"  then mixed with the original input signal according to the mix input value.\n" \
"- The delay inlet controls all the filters.\n" \
"- All filters are parametrized the same way.\n" \
"- The delay of the filters is modulated by an external LFO.\n" \
"- The LFO ratio defines how much the LFO influences the delay. 0.0 stands\n" \
"  for no modulation at all, 1.0 for a modulation of the whole delay duration.\n" \
"  Negative LFO ratio values are also possible.\n" \
"- The LFO input signal should always lie within [-1.0,1.0].\n" \
"- An overflow can occur when the absolute value of the feedback value is\n" \
"  greater or equal to 1.0!\n" \
"- When the reset input value is greater than 0.0, then all filters will be\n" \
"  reinitialized."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_PHS_IN_SRC_INFO		"Eingang"
#define OP_PHS_IN_MIX_INFO		"Mix"
#define OP_PHS_IN_FEEDB_INFO		"Feedback"
#define OP_PHS_IN_STAGES_INFO		"Anzahl Filter"
#define OP_PHS_IN_DELAY_INFO		"Verzögerung"
#define OP_PHS_IN_LFORATIO_INFO		"LFO Verhältnis"
#define OP_PHS_IN_LFO_INFO		"LFO Eingang"
#define OP_PHS_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_PHS_OUT_DST_INFO		"Ausgang"
#define OP_PHS_CLASS_INFO \
\
"Phaser\n" \
"- Eingabewerte werden durch die angegebene Anzahl Allpass-Filter geleitet\n" \
"  und entsprechend dem Mix-Wert mit dem Eingabesignal addiert.\n" \
"- Die Filter lassen sich durch den Verzögerungs-Eingang beeinflussen.\n" \
"- Alle Filter werden gleich parameterisiert.\n" \
"- Die Dauer d der Verzögerung wird durch einen externen LFO moduliert.\n" \
"- Der LFO Verhältnis-Wert lr bestimmt den Anteil, welchen der LFO maximal\n" \
"  von d beeinflussen kann. 0.0 entspricht keiner Beeinflussung, 1.0 einer\n" \
"  einer von max. d Samples. Negative Werte sind erlaubt.\n" \
"- Der Wert am LFO-Eingang sollte sich innerhalb von [-1.0,+1.0] bewegen.\n" \
"- Wenn der Betrag des Feedback-Wertes grösser gleich 1.0 ist, kann es\n" \
"  zu einem Overflow kommen.\n" \
"- Ist der Wert am Zurücksetzen-Eingang grösser als 0.0, so werden alle\n" \
"  Filter neu initialisiert."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpPhaser::COpPhaser()
: COp( 8, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "phsr" );
	m_poIn[OP_PHS_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_PHS_IN_SRC, "i", OP_PHS_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PHS_IN_MIX, "m", OP_PHS_IN_MIX_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PHS_IN_FEEDB, "fb", OP_PHS_IN_FEEDB_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PHS_IN_STAGES, "st", OP_PHS_IN_STAGES_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PHS_IN_DELAY, "d", OP_PHS_IN_DELAY_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PHS_IN_LFORATIO, "lr", OP_PHS_IN_LFORATIO_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PHS_IN_LFO, "lfo", OP_PHS_IN_LFO_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PHS_IN_CLEAR, "cl", OP_PHS_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_PHS_OUT_DST, "o", OP_PHS_OUT_DST_INFO );
	Update();
	Validate();
}

COpPhaser::~COpPhaser()
{}

void COpPhaser::Init()
{
	m_dStages = 0;
	m_uiStages = 0;
	Reset();
	COp::Init();
}

inline void COpPhaser::ClearFeed()
{
	m_dLast = 0.0;
	for( unsigned int i=0; i<m_uiStages; ++i )
		m_oArrFlt[i].Reset();
}

void COpPhaser::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dStages = m_poIn[OP_PHS_IN_STAGES].Proc();
	if( dStages != m_dStages )
	{
		const unsigned int uiStages = (unsigned int)abs( (int)rint( dStages ) );
		if( uiStages > m_uiStages )
			m_oArrFlt.Resize( uiStages );
		m_uiStages = uiStages;
		m_dStages = dStages;
	}
	
	if( m_poIn[OP_PHS_IN_CLEAR].IsValid()
	    && m_poIn[OP_PHS_IN_CLEAR].Proc() > 0.0 )
		ClearFeed();
	
	const double dIn = m_poIn[OP_PHS_IN_SRC].Proc();
	const double dDly = fabs( m_poIn[OP_PHS_IN_DELAY].Proc() ) * 0.001;
	const double dRatio = m_poIn[OP_PHS_IN_LFORATIO].Proc();
	const double dFeed = m_poIn[OP_PHS_IN_FEEDB].Proc();
	double dLFO = dDly + dDly * dRatio * ( m_poIn[OP_PHS_IN_LFO].Proc() * 0.5 - 0.5 );
	double dOut = dIn;
	if( dLFO < 0.0 )
		dLFO = 0.0;
	if( fabs( dFeed ) < 1.0 )
		dOut += m_dLast * dFeed;
	else
		dOut += dFeed;
	
	const double dCoeff = COpTkFltAllpass1<double>::CalcCoeff_( dLFO );
	for( unsigned int i=0; i<m_uiStages; ++i )
		dOut = m_oArrFlt[i].Proc( dOut, dCoeff );
	
	m_dLast = dOut;
	
	const double dMix = m_poIn[OP_PHS_IN_MIX].Proc();
	m_pdOut[OP_PHS_OUT_DST] = dMix * ( dOut - dIn ) + dIn;
}

void COpPhaser::Reset()
{
	m_oArrFlt.Resize( m_uiStages );
	ClearFeed();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpPhaser, OP_PHS_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpPhaser )
OP_GENERIC_METHODS_DEF( COpPhaser )

#endif // OP_INC_OP_PHASER
