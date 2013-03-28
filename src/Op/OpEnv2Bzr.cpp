// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv2Bzr.cpp

#include "OpEnv2Bzr.h"
#ifdef OP_INC_OP_ENV_2_BZR

////////////////////////////////////////////////////////////////////////////////
// COpEnv2Bzr
////////////////////////////////////////////////////////////////////////////////

#define OP_E2B_IN_TRIGGER	0
#define OP_E2B_IN_DUR		1
#define OP_E2B_IN_RATIO		2
#define OP_E2B_IN_SLOPE0	3 	 
#define OP_E2B_IN_SLOPE1	4
#define OP_E2B_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_E2B_IN_TRIGGER_INFO	"trigger"
#define OP_E2B_IN_DUR_INFO	"duration"
#define OP_E2B_IN_RATIO_INFO	"ratio"
#define OP_E2B_IN_SLOPE0_INFO	"slope 0"
#define OP_E2B_IN_SLOPE1_INFO	"slope 1"
#define OP_E2B_OUT_DST_INFO	"output"
#define OP_E2B_CLASS_INFO \
\
"2-Stage Bézier Envelope\n" \
"- A new cycle begins when the trigger value is greater than 0.0.\n" \
"- The duration value defines the total length in samples.\n" \
"- The ratio defines the proportional duration of the first stage.\n" \
"- Once the end of the envelope has been reached, the output value\n" \
"  will be 0.0.\n" \
"- Changes on the duration, ratio and slope inlets only influence\n" \
"  the curve in the next cycle.\n" \
"- All inlets are processed every cycle."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_E2B_IN_TRIGGER_INFO	"Auslöser"
#define OP_E2B_IN_DUR_INFO	"Dauer"
#define OP_E2B_IN_RATIO_INFO	"Verhältnis"
#define OP_E2B_IN_SLOPE0_INFO	"Krümmung 0"
#define OP_E2B_IN_SLOPE1_INFO	"Krümmung 1"
#define OP_E2B_OUT_DST_INFO	"Ausgang"
#define OP_E2B_CLASS_INFO \
\
"2-Stufen Bézier-Hüllkurve\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird eine neuer\n" \
"  Zyklus begonnen.\n" \
"- Die Dauer d gibt die gesamte Dauer der Hüllkurve in Samples an.\n" \
"- Das Verhältnis verhält sich zu 1.0, wie die Dauer der ersten Stufe zur\n" \
"  gesamten Dauer d.\n" \
"- Ist das Ende der Hüllkurve erreicht, so steht der Ausgang konstant auf 0.0.\n" \
"- Änderungen am Dauer-/Verhältnis-Eingang und an den Krümmungs-Eingängen\n" \
"  beeinflussen die Kurve erst in ihrem nächsten Zyklus.\n" \
"- Alle Eingänge werden immer abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpEnv2Bzr::COpEnv2Bzr()
: COp( 5, 1 )
, m_dDur( 0.0 )
, m_dRatio( 0.0 )
, m_dSlope0( 0.0 )
, m_dSlope1( 0.0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "e2bzr" );
	OP_SET_NAME_AND_INFO_INPUT( OP_E2B_IN_TRIGGER, "tr", OP_E2B_IN_TRIGGER_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E2B_IN_DUR, "d", OP_E2B_IN_DUR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E2B_IN_RATIO, "r", OP_E2B_IN_RATIO_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E2B_IN_SLOPE0, "s0", OP_E2B_IN_SLOPE0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E2B_IN_SLOPE1, "s1", OP_E2B_IN_SLOPE1_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_E2B_OUT_DST, "o", OP_E2B_OUT_DST_INFO );
	Update();
	Validate();
}

COpEnv2Bzr::~COpEnv2Bzr() 
{}

void COpEnv2Bzr::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 2 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oEnv.Index() );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_oEnv.State() );
	OP_SET_COUNT_INTERNAL_FLT( 6 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oEnv.Value() );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oEnv.Position() );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_dDur );
	OP_SET_PTR_INTERNAL_FLT( 3, &m_dRatio );
	OP_SET_PTR_INTERNAL_FLT( 4, &m_dSlope0 );
	OP_SET_PTR_INTERNAL_FLT( 5, &m_dSlope1 );
	COp::Init();
}

void COpEnv2Bzr::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dDur = m_poIn[OP_E2B_IN_DUR].Proc();
	const double dRatio = m_poIn[OP_E2B_IN_RATIO].Proc();
	const double dSlope0 = m_poIn[OP_E2B_IN_SLOPE0].Proc();
	const double dSlope1 = m_poIn[OP_E2B_IN_SLOPE1].Proc();
	
	if( m_poIn[OP_E2B_IN_TRIGGER].Proc() > 0.0 )
	{
		if( dDur != m_dDur || dRatio != m_dRatio || dSlope0 != m_dSlope0 || dSlope1 != m_dSlope1 )
		{
			m_dDur = dDur;
			m_dRatio = dRatio;
			m_dSlope0 = dSlope0;
			m_dSlope1 = dSlope1;
			Update();
		}
		m_oEnv.Reset();
		m_oEnv.Trigger();
	}
	m_pdOut[OP_E2B_OUT_DST] = m_oEnv.Proc();
}

void COpEnv2Bzr::Update()
{
	m_oEnv.Set( m_dDur, m_dRatio, m_dSlope0, m_dSlope1 );
	COp::Update();
}

void COpEnv2Bzr::Reset()
{
	m_oEnv.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpEnv2Bzr, OP_E2B_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpEnv2Bzr )
OP_GENERIC_METHODS_DEF( COpEnv2Bzr )

#endif // OP_INC_OP_ENV_2_BZR
