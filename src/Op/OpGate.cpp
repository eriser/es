// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpGate.cpp

#include "OpGate.h"
#ifdef OP_INC_OP_GATE

////////////////////////////////////////////////////////////////////////////////
// COpGate
////////////////////////////////////////////////////////////////////////////////

#define OP_GTE_IN_SRC			0
#define OP_GTE_IN_THRESHOLD		1
#define OP_GTE_IN_ATTACK		2
#define OP_GTE_IN_RELEASE		3
#define OP_GTE_IN_SLOPE0		4
#define OP_GTE_IN_SLOPE1		5
#define OP_GTE_IN_DELAY			6
#define OP_GTE_OUT_DST			0
#define OP_GTE_OUT_ENV			1
#define OP_GTE_OUT_RMS			2
#define OP_GTE_OUT_SYNCA		3
#define OP_GTE_OUT_SYNCR		4

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_GTE_IN_SRC_INFO		"input"
#define OP_GTE_IN_THRESHOLD_INFO	"threshold"
#define OP_GTE_IN_ATTACK_INFO		"attack"
#define OP_GTE_IN_RELEASE_INFO		"release"
#define OP_GTE_IN_SLOPE0_INFO		"slope 0"
#define OP_GTE_IN_SLOPE1_INFO		"slope 1"
#define OP_GTE_IN_DELAY_INFO		"delay"
#define OP_GTE_OUT_DST_INFO		"output"
#define OP_GTE_OUT_ENV_INFO		"envelope"
#define OP_GTE_OUT_SYNCA_INFO		"attack sync"
#define OP_GTE_OUT_SYNCR_INFO		"release sync"
#define OP_GTE_OUT_RMS_INFO		"energy"
#define OP_GTE_CLASS_INFO \
\
"Gate\n" \
"- The attack envelope is triggered whenever the current energy value\n" \
"  exceeds the threshold.\n" \
"- The release envelope is triggered when the energy value falls under the\n" \
"  threshold again.\n" \
"- The attack envelope raises from 0.0 up to 1.0, as long as it doesn't get\n" \
"  interrupted by the release envelope.\n" \
"- The release envelope drops from 1.0 down to 0.0, as long as it doesn't get\n" \
"  interrupted by the attack envelope.\n" \
"- The attack and release parameters define the maximal duration in samples\n" \
"  of the envelopes.\n" \
"- The slope values define the shape of the exponential envelope curves.\n" \
"- The envelope output is equal to the current state of the overall gate envelope.\n" \
"- The energy output value is a measure for the relative dynamics of the\n" \
"  input signal, and is directly related to the size of the internal delay buffer.\n" \
"- The sync outputs values are equal to 1.0 when the corresponding envelope\n" \
"  is triggered. Otherwise they are always 0.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_GTE_IN_SRC_INFO		"Eingang"
#define OP_GTE_IN_THRESHOLD_INFO	"Threshold"
#define OP_GTE_IN_ATTACK_INFO		"Attack"
#define OP_GTE_IN_RELEASE_INFO		"Release"
#define OP_GTE_IN_SLOPE0_INFO		"Krümmung 0"
#define OP_GTE_IN_SLOPE1_INFO		"Krümmung 1"
#define OP_GTE_IN_DELAY_INFO		"Verzögerung"
#define OP_GTE_OUT_DST_INFO		"Ausgang"
#define OP_GTE_OUT_ENV_INFO		"Hüllkurve"
#define OP_GTE_OUT_SYNCA_INFO		"Attack-Synchronisation"
#define OP_GTE_OUT_SYNCR_INFO		"Release-Synchronisation"
#define OP_GTE_OUT_RMS_INFO		"Energie"
#define OP_GTE_CLASS_INFO \
\
"Gate\n" \
"- Sobald der Energie-Wert den Threshold-Wert übersteigt, wird die Attack-\n" \
"  Hüllkurve ausgelöst.\n" \
"- Fällt der Energie-Wert wieder unter den Threshold-Wert, wird die Release-\n" \
"  Hüllkurve ausgelöst.\n" \
"- Die Attack-Hüllkurve steigt von 0.0 auf 1.0, vorausgesetzt, sie wird nicht\n" \
"  von der Release-Hüllkurve unterbrochen.\n" \
"- Die Release-Hüllkurve sinkt von max. 1.0 auf 0.0, vorausgesetzt, sie wird\n" \
"  nicht von der Attack-Hüllkurve unterbrochen.\n" \
"- Die Werte am Attack- und Release-Eingang bestimmen die max. Dauer der Hüll-\n" \
"  kurven in Samples.\n" \
"- Die beiden Krümmungswerte beschreiben die Form des exponentiellen Kurven.\n" \
"- Der Hüllkurven-Ausgang gibt mit Werten zwischen 0.0 und 1.0 den aktuellen\n" \
"  Ausschlag der Gate-Hüllkurve an.\n" \
"- Die Energie ist ein Mass für die relative Dynamik des Signals und steht\n" \
"  in direkter Abhängikeit von der Grösse des Verzögerungspuffers.\n" \
"- Ein Synchronisations-Ausgang stehen immer dann auf 1.0, wenn die ensprechende\n" \
"  Hüllkurve gestartet wird. Sonst steht er konstant auf 0.0."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpGate::COpGate()
: COp( 7, 5 )
{
	Init();
	OP_SET_NAME_INSTANCE( "gate" );
	OP_SET_NAME_AND_INFO_INPUT( OP_GTE_IN_SRC, "i", OP_GTE_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_GTE_IN_THRESHOLD, "th", OP_GTE_IN_THRESHOLD_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_GTE_IN_ATTACK, "a", OP_GTE_IN_ATTACK_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_GTE_IN_RELEASE, "r", OP_GTE_IN_RELEASE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_GTE_IN_SLOPE0, "s0", OP_GTE_IN_SLOPE0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_GTE_IN_SLOPE1, "s1", OP_GTE_IN_SLOPE1_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_GTE_IN_DELAY, "d", OP_GTE_IN_DELAY_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_GTE_OUT_DST, "o", OP_GTE_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_GTE_OUT_ENV, "ev", OP_GTE_OUT_ENV_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_GTE_OUT_RMS, "e", OP_GTE_OUT_RMS_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_GTE_OUT_SYNCA, "sa", OP_GTE_OUT_SYNCA_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_GTE_OUT_SYNCR, "sr", OP_GTE_OUT_SYNCR_INFO );
	Update();
	Validate();
}

COpGate::~COpGate()
{}

void COpGate::Init()
{
	m_dDelay = 1.0;
	m_oDelay.SetDelay( 1u );
	Reset();
	OP_SET_COUNT_INTERNAL_UINT( 5 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiEnvPos );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_oEnvAtt.Index() );
	OP_SET_PTR_INTERNAL_UINT( 2, &m_oEnvAtt.State() );
	OP_SET_PTR_INTERNAL_UINT( 3, &m_oEnvRel.Index() );
	OP_SET_PTR_INTERNAL_UINT( 4, &m_oEnvRel.State() );
	OP_SET_COUNT_INTERNAL_FLT( 4 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oEnvAtt.Value() );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oEnvAtt.Position() );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oEnvRel.Value() );
	OP_SET_PTR_INTERNAL_FLT( 3, &m_oEnvRel.Position() );
	COp::Init();
}

void COpGate::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();

	const double dDly = m_poIn[OP_GTE_IN_DELAY].Proc();
	if( dDly != m_dDelay )
	{
		m_dDelay = dDly;
		m_oDelay.SetDelay( dDly );
		Reset();
	}
	
	double dVal = m_oDelay.ReadDelay();
	m_dSqSum -= dVal * dVal;
	
	const double dValNew = m_poIn[OP_GTE_IN_SRC].Proc();
	m_dSqSum += dValNew * dValNew;
	m_oDelay.WriteDelay( dValNew );
	
	double dRMS = sqrt( m_dSqSum / ( dDly + 1.0 ) );
	
	m_pdOut[OP_GTE_OUT_DST] = dVal * m_dEnv;
	m_pdOut[OP_GTE_OUT_ENV] = m_dEnv;
	m_pdOut[OP_GTE_OUT_RMS] = dRMS;
	
	const double dAttack = m_poIn[OP_GTE_IN_ATTACK].Proc();
	const double dRelease = m_poIn[OP_GTE_IN_RELEASE].Proc();
	const double dSlope0 = m_poIn[OP_GTE_IN_SLOPE0].Proc();
	const double dSlope1 = m_poIn[OP_GTE_IN_SLOPE1].Proc();
	
	if( dRMS >= m_poIn[OP_GTE_IN_THRESHOLD].Proc() )
	{
		if( !m_uiEnvPos || m_uiEnvPos == 3 )
		{
			m_oEnvAtt.Set( dAttack, ( m_uiEnvPos ? m_dEnv : 0.0 ), 1.0, dSlope0 );
			m_oEnvAtt.Reset();
			m_oEnvAtt.Trigger();
			m_uiEnvPos = 1;
			m_pdOut[OP_GTE_OUT_SYNCA] = 1.0;
		}
	}
	else if( m_uiEnvPos == 1 || m_uiEnvPos == 2 ) // ( Sustain || Attack ) -> Release.
	{
		m_oEnvRel.Set( dRelease, ( m_uiEnvPos == 1 ? m_dEnv : 1.0 ), 0.0, - dSlope1 );
		m_oEnvRel.Reset();
		m_oEnvRel.Trigger();
		m_uiEnvPos = 3;
		m_pdOut[OP_GTE_OUT_SYNCR] = 1.0;
	}
	else {
		m_pdOut[OP_GTE_OUT_SYNCR] = m_pdOut[OP_GTE_OUT_SYNCA] = 0.0;
	}
	
	if( m_uiEnvPos == 1 ) // Attack.
	{
		m_dEnv = m_oEnvAtt.Proc();
		if( !m_oEnvAtt.IsOn() )
		{
			m_dEnv = 1.0;
			++m_uiEnvPos;
		}
	}
	//else if( m_uiEnvPos == 2 ) // Sustain.
	//	m_dEnv = 1.0;
	else if( m_uiEnvPos == 3 ) // Release.
	{
		m_dEnv = m_oEnvRel.Proc();
		if( !m_oEnvRel.IsOn() )
		{
			m_dEnv = 0.0;
			m_uiEnvPos = 0;
		}
	}
}

void COpGate::Update()
{
	COp::Update();
	m_oDelay.UpdateSize();
}

void COpGate::Reset()
{
	COp::Reset();
	m_oDelay.Reset();
	m_oEnvAtt.Reset();
	m_oEnvRel.Reset();
	m_dEnv = 0.0;
	m_dSqSum = 0.0;
	m_uiEnvPos = 0;
}

OP_SET_NAME_AND_INFO_CLASS( COpGate, OP_GTE_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpGate )
OP_GENERIC_METHODS_DEF( COpGate )

#endif // OP_INC_OP_GATE
