// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFlanger.cpp

#include "OpFlanger.h"
#ifdef OP_INC_OP_FLANGER

////////////////////////////////////////////////////////////////////////////////
// COpFlanger
////////////////////////////////////////////////////////////////////////////////

#define OP_FLG_IN_SRC		0
#define OP_FLG_IN_MIX		1
#define OP_FLG_IN_FEEDB		2
#define OP_FLG_IN_DELAY		3
#define OP_FLG_IN_AMP		4
#define OP_FLG_IN_LFO		5
#define OP_FLG_IN_CLEAR		6
#define OP_FLG_OUT_DST		0
#define OP_FLG_ROUTINE_SIZE	0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FLG_IN_SRC_INFO	"input"
#define OP_FLG_IN_MIX_INFO	"mix"
#define OP_FLG_IN_FEEDB_INFO	"feedback"
#define OP_FLG_IN_DELAY_INFO	"delay"
#define OP_FLG_IN_AMP_INFO	"LFO amplitude"
#define OP_FLG_IN_LFO_INFO	"LFO input"
#define OP_FLG_IN_CLEAR_INFO	"reset"
#define OP_FLG_OUT_DST_INFO	"output"
#define OP_FLG_CLASS_INFO \
\
"Flanger\n" \
"- The delay of input samples changes according to the LFO.\n" \
"- Delayed input values are scaled and mixed with the original signal,\n" \
"  according to the mix parameter, which may vary between 0.0 and 1.0.\n" \
"- The delay inlet defines the delay in samples, without alterations.\n" \
"- The LFO amplitude defines the count of samples the delay can change.\n" \
"- The LFO input value drives the delay variation. It should always lie\n" \
"  inside [-1.0,1.0].\n" \
"- The minimal delay is 1 Sample.\n" \
"- When the reset input value is greater than 0.0, then the whole buffer\n" \
"  will be reset to 0.0. Its current size remains the same.\n" \
"- An overflow can occur when the feedback value is greater than 1.0!"

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FLG_IN_SRC_INFO	"Eingang"
#define OP_FLG_IN_MIX_INFO	"Mix"
#define OP_FLG_IN_FEEDB_INFO	"Feedback"
#define OP_FLG_IN_DELAY_INFO	"Verzögerung"
#define OP_FLG_IN_AMP_INFO	"LFO Amplitude"
#define OP_FLG_IN_LFO_INFO	"LFO Eingang"
#define OP_FLG_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_FLG_OUT_DST_INFO	"Ausgang"
#define OP_FLG_CLASS_INFO \
\
"Flanger\n" \
"- Eingabewerte und Ausgabewerte werden entsprechend skaliert und\n" \
"  unterschiedlich verzögert mit aktuellen Eingabesignal addiert.\n" \
"- Die Dauer d der Verzögerung wird durch einen externen LFO moduliert.\n" \
"- Der Amplituden-Wert a bestimmt die maximale Veränderung von d in Samples.\n" \
"- Der Wert am LFO-Eingang sollte sich innerhalb von [-1.0,1.0] bewegen.\n" \
"- Die minimale Verzögerung beträgt 1.\n" \
"- Ist der Wert am Zurücksetzen-Eingang grösser als 0.0, so wird der gesamte\n" \
"  Puffer auf 0.0 gesetzt. An seine Grösse ändert sich nichts.\n" \
"- Wenn der Betrag des Feedback-Wertes grösser gleich 1.0 ist kann es\n" \
"  zu einem Overflow kommen."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFlanger::COpFlanger()
: COp( 7, 1 )
{
	OP_SET_NAME_INSTANCE( "flanger" );
	m_poIn[OP_FLG_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLG_IN_SRC, "i", OP_FLG_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLG_IN_MIX, "m", OP_FLG_IN_MIX_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLG_IN_FEEDB, "f", OP_FLG_IN_FEEDB_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLG_IN_DELAY, "d", OP_FLG_IN_DELAY_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLG_IN_AMP, "a", OP_FLG_IN_AMP_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLG_IN_LFO, "lfo", OP_FLG_IN_LFO_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLG_IN_CLEAR, "cl", OP_FLG_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FLG_OUT_DST, "o", OP_FLG_OUT_DST_INFO );
	Update();
	Validate();
}

COpFlanger::~COpFlanger()
{}

void COpFlanger::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_FLG_IN_CLEAR].IsValid()
	    && m_poIn[OP_FLG_IN_CLEAR].Proc() > 0.0 )
			m_oDelay.Reset();

	const double dIn = m_poIn[OP_FLG_IN_SRC].Proc();
	//const double dDly = m_poIn[OP_FLG_IN_DELAY].Proc();
	//m_oDelay.SetDelay( 
	//	dDly + dDly * m_poIn[OP_FLG_IN_AMP].Proc() * (
	//	m_poIn[OP_FLG_IN_LFO].Proc()* 0.5 - 0.5 ) );
	m_oDelay.SetDelay(
		m_poIn[OP_FLG_IN_DELAY].Proc() 
		+ m_poIn[OP_FLG_IN_LFO].Proc() * m_poIn[OP_FLG_IN_AMP].Proc() );
	const double dOut = m_oDelay.ReadDelay();
	m_oDelay.WriteDelay( dIn + dOut * m_poIn[OP_FLG_IN_FEEDB].Proc() );
	m_pdOut[OP_FLG_OUT_DST] = m_poIn[OP_FLG_IN_MIX].Proc() * ( dOut - dIn ) + dIn;
}
 
void COpFlanger::Update()
{
	m_oDelay.UpdateSize();
	COp::Update();
}

void COpFlanger::Reset()
{
	m_oDelay.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpFlanger, OP_FLG_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFlanger )
OP_GENERIC_METHODS_DEF( COpFlanger )

#endif // OP_INC_OP_FLANGER
