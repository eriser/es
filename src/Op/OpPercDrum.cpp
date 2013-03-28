// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpPercDrum.cpp

#include "OpPercDrum.h"
#ifdef OP_INC_OP_PERC_DRUM

////////////////////////////////////////////////////////////////////////////////
// COpPercDrum
////////////////////////////////////////////////////////////////////////////////

#define OP_PDR_IN_TRIGG			0
#define OP_PDR_IN_DUR			1
#define OP_PDR_IN_MIX			2
#define OP_PDR_IN_SEED			3
#define OP_PDR_IN_BALANCE		4
#define OP_PDR_IN_FREQ0			5
#define OP_PDR_IN_FREQ1			6
#define OP_PDR_IN_RES			7
#define OP_PDR_IN_FRQ_RATIO		8
#define OP_PDR_IN_FRQ_SLOPE0		9
#define OP_PDR_IN_FRQ_SLOPE1		10
#define OP_PDR_IN_AMP_RATIO		11
#define OP_PDR_IN_AMP_SLOPE0		12
#define OP_PDR_IN_AMP_SLOPE1		13
#define OP_PDR_IN_CLEAR			14
#define OP_PDR_OUT_DST			0
#define OP_PDR_OUT_ENV_AMP		1
#define OP_PDR_OUT_ENV_FRQ		2
#define OP_PDR_ROUTINE_FILTER		0
#define OP_PDR_ROUTINE_NOISE		1
#define OP_PDR_ROUTINE_OSC		2

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_PDR_IN_TRIGG_INFO		"trigger"
#define OP_PDR_IN_DUR_INFO		"duration"
#define OP_PDR_IN_MIX_INFO		"mix"
#define OP_PDR_IN_SEED_INFO		"start value of the noise generator"
#define OP_PDR_IN_BALANCE_INFO		"balance value of the noise generator"
#define OP_PDR_IN_FREQ0_INFO		"base frequency"
#define OP_PDR_IN_FREQ1_INFO		"frequency factor"
#define OP_PDR_IN_RES_INFO		"filter resonance"
#define OP_PDR_IN_FRQ_RATIO_INFO	"ratio (frequency envelope)"
#define OP_PDR_IN_FRQ_SLOPE0_INFO	"slope 0 (frequency envelope)"
#define OP_PDR_IN_FRQ_SLOPE1_INFO	"slope 1 (frequency envelope)"
#define OP_PDR_IN_AMP_RATIO_INFO	"ratio (amplitude envelope)"
#define OP_PDR_IN_AMP_SLOPE0_INFO	"slope 0 (amplitude envelope)"
#define OP_PDR_IN_AMP_SLOPE1_INFO	"slope 1 (amplitude envelope)"
#define OP_PDR_IN_CLEAR_INFO		"reset"
#define OP_PDR_OUT_DST_INFO		"output"
#define OP_PDR_OUT_ENV_AMP_INFO		"amplitude envelope"
#define OP_PDR_OUT_ENV_FRQ_INFO		"frequency envelope"
#define OP_PDR_ROUTINE_FILTER_INFO	"filter type"
#define OP_PDR_ROUTINE_NOISE_INFO	"noise generator type"
#define OP_PDR_ROUTINE_OSC_INFO		"oscillator type"
#define OP_PDR_CLASS_INFO \
\
"Drum\n" \
"- Two synthesis methods are used in parallel. The mix input tells how\n" \
"  much each one contributes to the output signal.\n" \
"- The mix value is clamped to [-1.0,0.0].\n" \
"- For mix values less or equal to -1.0, only the oscillator method comes to use.\n" \
"- For mix values greater or equal to 1.0, only the noise generator method comes to use.\n" \
"- After each hit the noise generator is reset. For equal start values, it\n" \
"  always generates the same sequence.\n" \
"- Either white or pink noise can be selected, via routine.\n" \
"- The balance input controls the amount of feedback used for noise generation.\n" \
"- After each hit, the oscillator will be reset.\n" \
"- The frequency inlets refer to the filter and the oscillator at the same time.\n" \
"- The duration input value defines the duration of both envelopes.\n" \
"- Both, the frequency and amplitude envelope, influence both synthesis methods.\n" \
"- When the reset input value is greater than 0.0, then all the internal components\n" \
"  will be reinitialized.\n" \
"- All utilized envelopes are exponential."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_PDR_IN_TRIGG_INFO		"Auslöser"
#define OP_PDR_IN_DUR_INFO		"Dauer"
#define OP_PDR_IN_MIX_INFO		"Mix"
#define OP_PDR_IN_SEED_INFO		"Startwert des Rausch-Generators"
#define OP_PDR_IN_BALANCE_INFO		"Balance des Rausch-Generators"
#define OP_PDR_IN_FREQ0_INFO		"Basis-Frequenz"
#define OP_PDR_IN_FREQ1_INFO		"Frequenz-Faktor"
#define OP_PDR_IN_RES_INFO		"Filter-Resonanz"
#define OP_PDR_IN_FRQ_RATIO_INFO	"Verhältnis der Frequenz-Hüllkurve"
#define OP_PDR_IN_FRQ_SLOPE0_INFO	"Krümmung 0 der Frequenz-Hüllkurve"
#define OP_PDR_IN_FRQ_SLOPE1_INFO	"Krümmung 1 der Frequenz-Hüllkurve"
#define OP_PDR_IN_AMP_RATIO_INFO	"Verhältnis der Amplituden-Hüllkurve"
#define OP_PDR_IN_AMP_SLOPE0_INFO	"Krümmung 0 der Amplituden-Hüllkurve"
#define OP_PDR_IN_AMP_SLOPE1_INFO	"Krümmung 1 der Amplituden-Hüllkurve"
#define OP_PDR_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_PDR_OUT_DST_INFO		"Ausgang"
#define OP_PDR_OUT_ENV_AMP_INFO		"Amplituden-Hüllkurve"
#define OP_PDR_OUT_ENV_FRQ_INFO		"Frequenz-Hüllkurve"
#define OP_PDR_ROUTINE_FILTER_INFO	"Filter-Typ"
#define OP_PDR_ROUTINE_NOISE_INFO	"Rausch-Generator-Typ"
#define OP_PDR_ROUTINE_OSC_INFO		"Oszillator-Typ"
#define OP_PDR_CLASS_INFO \
\
"Trommel\n" \
"- Zwei Synthese-Methoden kommen parallel zum Einsatz und können über den\n" \
"  Mix-Eingang mit Werten zwischen -1.0 und +1.0 beliebig gemischt werden.\n" \
"- Ist der Mix-Wert kleiner oder gleich -1.0, so kommt nur die Oszillator-\n" \
"  Methode zum Einsatz.\n" \
"- Ist der Mix-Wert grösser oder gleich +1.0, so kommt nur die Rausch-\n" \
"  Generator-/Filter-Methode zum Einsatz.\n" \
"- Nach jedem Zyklus wird der Rausch-Generator zurückgesetzt. Für gleiche Start-\n" \
"  werte generiert er die gleiche Sequenz.\n" \
"- Es kann zwischen weissem und pinkem Rauschen gewählt werden.\n" \
"- Die Balance regelt den Anteil an Feedback der zum Eingang des Rausch-\n" \
"  Generators zurückgeführt wird\n" \
"- Nach jedem Zyklus wird der Oszillator zurückgesetzt.\n" \
"- Die Frequenz-Werte beziehen sich sowohl auf den Filter, als auch den Oszillator.\n" \
"- Die Dauer bezieht sich auf die Frequenz- und Amplituden-Hüllkurve.\n" \
"- Die Frequenz- und Amplituden-Hüllkurve beeinflussen beide Synthese-Methoden\n" \
"  gleichermassen.\n" \
"- Steht der Zurücksetzen-Eingang auf einem Wert grösser als 1.0, so werden\n" \
"  alle Komponenten neu initialisiert.\n" \
"- Alle verwendeten Hüllkurven sind exponentiell."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpPercDrum::COpPercDrum()
: COp( 15, 3 )
, m_uiFuncNum( 0 )
, m_dDur( 0.0 )
, m_dFrqRatio( 0.0 )
, m_dFrqSlope0( 0.0 )
, m_dFrqSlope1( 0.0 )
, m_dAmpRatio( 0.0 )
, m_dAmpSlope0( 0.0 )
, m_dAmpSlope1( 0.0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "prDrm" );
	m_poIn[OP_PDR_IN_SEED].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_PDR_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_TRIGG, "tr", OP_PDR_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_MIX, "m", OP_PDR_IN_MIX_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_DUR, "d", OP_PDR_IN_DUR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_SEED, "s", OP_PDR_IN_SEED_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_BALANCE, "b", OP_PDR_IN_BALANCE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_FREQ0, "f0", OP_PDR_IN_FREQ0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_FREQ1, "f1", OP_PDR_IN_FREQ1_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_RES, "r", OP_PDR_IN_RES_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_FRQ_RATIO, "fr", OP_PDR_IN_FRQ_RATIO_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_FRQ_SLOPE0, "fs0", OP_PDR_IN_FRQ_SLOPE0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_FRQ_SLOPE1, "fs1", OP_PDR_IN_FRQ_SLOPE1_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_AMP_RATIO, "ar", OP_PDR_IN_AMP_RATIO_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_AMP_SLOPE0, "as0", OP_PDR_IN_AMP_SLOPE0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_AMP_SLOPE1, "as1", OP_PDR_IN_AMP_SLOPE1_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_PDR_IN_CLEAR, "cl", OP_PDR_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_PDR_OUT_DST, "o", OP_PDR_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_PDR_OUT_ENV_AMP, "ea", OP_PDR_OUT_ENV_AMP_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_PDR_OUT_ENV_FRQ, "ef", OP_PDR_OUT_ENV_FRQ_INFO );
	OP_SET_COUNT_ROUTINE( 3 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_PDR_ROUTINE_FILTER_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( 1, "n" OP_STR_ROUTINE, OP_PDR_ROUTINE_NOISE_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( 2, "o" OP_STR_ROUTINE, OP_PDR_ROUTINE_OSC_INFO );
	Update();
	Validate();
}

COpPercDrum::~COpPercDrum()
{}

void COpPercDrum::Init()
{
	OP_SET_COUNT_INTERNAL_INT( 4 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_oSeq.m_iSeed );
	OP_SET_PTR_INTERNAL_INT( 1, &m_oSeq.m_iS0 );
	OP_SET_PTR_INTERNAL_INT( 2, &m_oSeq.m_iS1 );
	OP_SET_PTR_INTERNAL_INT( 3, &m_oOsc.Period() );
	OP_SET_COUNT_INTERNAL_UINT( 7 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oEnvFrq.Index() );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_oEnvFrq.State() );
	OP_SET_PTR_INTERNAL_UINT( 2, &m_oEnvAmp.Index() );
	OP_SET_PTR_INTERNAL_UINT( 3, &m_oEnvAmp.State() );
	OP_SET_PTR_INTERNAL_UINT( 4, &m_uiFuncNum );
	OP_SET_PTR_INTERNAL_UINT( 5, &m_oSeq.Type() );
	OP_SET_PTR_INTERNAL_UINT( 6, &m_oOsc.Type() );
	OP_SET_COUNT_INTERNAL_FLT( 16 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oEnvFrq.Value() );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oEnvFrq.Position() );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oEnvAmp.Value() );
	OP_SET_PTR_INTERNAL_FLT( 3, &m_oEnvAmp.Position() );
	OP_SET_PTR_INTERNAL_FLT( 4, &m_oFlt.m_tIn0 );
	OP_SET_PTR_INTERNAL_FLT( 5, &m_oFlt.m_tIn1 );
	OP_SET_PTR_INTERNAL_FLT( 6, &m_oFlt.m_tOut0 );
	OP_SET_PTR_INTERNAL_FLT( 7, &m_oFlt.m_tOut1 );
	OP_SET_PTR_INTERNAL_FLT( 8, &m_oOsc.Pos() );
	OP_SET_PTR_INTERNAL_FLT( 9, &m_dDur );
	OP_SET_PTR_INTERNAL_FLT( 10, &m_dFrqRatio );
	OP_SET_PTR_INTERNAL_FLT( 11, &m_dFrqSlope0 );
	OP_SET_PTR_INTERNAL_FLT( 12, &m_dFrqSlope1 );
	OP_SET_PTR_INTERNAL_FLT( 13, &m_dAmpRatio );
	OP_SET_PTR_INTERNAL_FLT( 14, &m_dAmpSlope0 );
	OP_SET_PTR_INTERNAL_FLT( 15, &m_dAmpSlope1 );
	COp::Init();
}

inline void COpPercDrum::UpdateEnvFrq() {
	m_oEnvFrq.Set( m_dDur, m_dFrqRatio, m_dFrqSlope0, m_dFrqSlope1 );
}

inline void COpPercDrum::UpdateEnvAmp() {
	m_oEnvAmp.Set( m_dDur, m_dAmpRatio, m_dAmpSlope0, m_dAmpSlope1 );
}

void COpPercDrum::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_PDR_IN_CLEAR].IsValid() )
		if( m_poIn[OP_PDR_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dDur = m_poIn[OP_PDR_IN_DUR].Proc();
	const double dFrqRatio = m_poIn[OP_PDR_IN_FRQ_RATIO].Proc();
	const double dFrqSlope0 = m_poIn[OP_PDR_IN_FRQ_SLOPE0].Proc();
	const double dFrqSlope1 = m_poIn[OP_PDR_IN_FRQ_SLOPE1].Proc();
	const double dAmpRatio = m_poIn[OP_PDR_IN_AMP_RATIO].Proc();
	const double dAmpSlope0 = m_poIn[OP_PDR_IN_AMP_SLOPE0].Proc();
	const double dAmpSlope1 = m_poIn[OP_PDR_IN_AMP_SLOPE1].Proc();

	if( m_poIn[OP_PDR_IN_TRIGG].Proc() > 0.0 )
	{
		bool bCondDur = ( dDur != m_dDur );
		m_dDur = dDur;
		
		if( bCondDur
		 || dFrqRatio != m_dFrqRatio || dFrqSlope0 != m_dFrqSlope0 || dFrqSlope1 != m_dFrqSlope1 )
		{
			m_dFrqRatio = dFrqRatio;
			m_dFrqSlope0 = dFrqSlope0;
			m_dFrqSlope1 = dFrqSlope1;
			UpdateEnvFrq();
		}
		if( bCondDur
		 || dAmpRatio != m_dAmpRatio || dAmpSlope0 != m_dAmpSlope0 || dAmpSlope1 != m_dAmpSlope1 )
		{
			m_dAmpRatio = dAmpRatio;
			m_dAmpSlope0 = dAmpSlope0;
			m_dAmpSlope1 = dAmpSlope1;
			UpdateEnvAmp();
		}
		
		m_oEnvFrq.Reset();
		m_oEnvFrq.Trigger();
		m_oEnvAmp.Reset();
		m_oEnvAmp.Trigger();
		m_oFlt.Reset();
		m_oOsc.Reset();
		if( m_poIn[OP_PDR_IN_SEED].IsValid() )
			m_oSeq.m_iSeed = (int)rint( m_poIn[OP_PDR_IN_SEED].Proc() );
		m_oSeq.Reset();
	}
	
	m_oSeq.SetBalance( m_poIn[OP_PDR_IN_BALANCE].Proc() );
	
	const double dEnvAmp = m_oEnvAmp.Proc();
	const double dEnvFrq = m_oEnvFrq.Proc();
	
	const double dFreq = m_poIn[OP_PDR_IN_FREQ0].Proc() + dEnvFrq * m_poIn[OP_PDR_IN_FREQ1].Proc();
	m_oOsc.SetFreq( dFreq );
	m_oFlt.Init( m_uiFuncNum != 0, dFreq, m_poIn[OP_PDR_IN_RES].Proc() );

	const double dOutFlt = m_oFlt.Proc( m_oSeq.Proc() );
	const double dOutOsc = m_oOsc.Proc();

	double dMix = m_poIn[OP_PDR_IN_MIX].Proc();
	dMix += 1.0;
	dMix *= 0.5;
	OP_RANGE_MINMAX( dMix, -1.0, +1.0 );
	double dOut = dMix * ( dOutFlt - dOutOsc ) + dOutOsc;
	dOut *= dEnvAmp;
	
	m_pdOut[OP_PDR_OUT_DST] = dOut;
	m_pdOut[OP_PDR_OUT_ENV_AMP] = dEnvAmp;
	m_pdOut[OP_PDR_OUT_ENV_FRQ] = dEnvFrq;
}

void COpPercDrum::Update()
{
#ifdef OP_USE_RUNTIME_INFO
	static char acName_[] = "  ";
	acName_[0] = ( m_oSeq.Type() == COpTkSeqRandomEx<double>::PINK ? 'p' : 'w' );
	acName_[1] = ( m_uiFuncNum != 0 ? 'h' : 'l' );
	acName_[2] = 'a' + m_oOsc.Type();
	SetNameOutput( OP_PDR_OUT_DST, acName_ );
#endif // OP_USE_RUNTIME_INFO
	COp::Update();
	
	UpdateEnvFrq();
	UpdateEnvAmp();
}

void COpPercDrum::Reset()
{
	m_oEnvFrq.Reset();
	m_oEnvAmp.Reset();
	m_oSeq.Reset();
	m_oOsc.Reset();
	m_oFlt.Reset();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpPercDrum::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_PDR_ROUTINE_FILTER:
		m_uiFuncNum = !m_uiFuncNum;
	break;
	case OP_PDR_ROUTINE_NOISE:
		m_oSeq.NextType();
	break;
	case OP_PDR_ROUTINE_OSC:
		m_oOsc.NextType();
	break;
	};
	Update();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpPercDrum, OP_PDR_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpPercDrum )
OP_GENERIC_METHODS_DEF( COpPercDrum )

#endif // OP_INC_OP_PERC_DRUM
