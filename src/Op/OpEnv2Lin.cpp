// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv2Lin.cpp

#include "OpEnv2Lin.h"
#ifdef OP_INC_OP_ENV_2_LIN

////////////////////////////////////////////////////////////////////////////////
// COpEnv2Lin
////////////////////////////////////////////////////////////////////////////////

#define OP_E2L_IN_TRIGGER	0
#define OP_E2L_IN_DUR		1
#define OP_E2L_IN_RATIO		2
#define OP_E2L_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_E2L_IN_TRIGGER_INFO	"trigger"
#define OP_E2L_IN_DUR_INFO	"duration"
#define OP_E2L_IN_RATIO_INFO	"ratio"
#define OP_E2L_OUT_DST_INFO	"output"
#define OP_E2L_CLASS_INFO \
\
"Linear 2-Stage Envelope\n" \
"- A new cycle begins when the trigger value is greater than 0.0.\n" \
"- The duration value defines the total length in samples.\n" \
"- The ratio defines the proportional duration of the first stage.\n" \
"- Once the end of the envelope has been reached, the output value\n" \
"  will be 0.0.\n" \
"- Changes on the duration, ratio and slope inlets only influence\n" \
"  the curve in the next cycle.\n" \
"- All inlets are processed every cycle."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_E2L_IN_TRIGGER_INFO	"Auslöser"
#define OP_E2L_IN_DUR_INFO	"Dauer"
#define OP_E2L_IN_RATIO_INFO	"Verhältnis"
#define OP_E2L_OUT_DST_INFO	"Ausgang"
#define OP_E2L_CLASS_INFO \
\
"Lineare 2-Stufen-Hüllkurve\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird eine neuer\n" \
"  Zyklus begonnen.\n" \
"- Die Dauer d gibt die gesamte Dauer der Hüllkurve in Samples an.\n" \
"- Das Verhältnis verhält sich zu 1.0, wie die Dauer der ersten Stufe zur\n" \
"  gesamten Dauer d.\n" \
"- Ist das Ende der Hüllkurve erreicht, so steht der Ausgang konstant auf 0.0.\n" \
"- Änderungen am Dauer-/Verhältnis-Eingang beeinflussen die Kurve erst in ihrem\n" \
"  nächsten Zyklus.\n" \
"- Alle Eingänge werden immer abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpEnv2Lin::COpEnv2Lin()
: COp( 3, 1 )
, m_dDur( 0.0 )
, m_dRatio( 0.0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "e2lin" );
	OP_SET_NAME_AND_INFO_INPUT( OP_E2L_IN_TRIGGER, "tr", OP_E2L_IN_TRIGGER_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E2L_IN_DUR, "d", OP_E2L_IN_DUR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E2L_IN_RATIO, "r", OP_E2L_IN_RATIO_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_E2L_OUT_DST, "o", OP_E2L_OUT_DST_INFO );
	Update();
	Validate();
}

COpEnv2Lin::~COpEnv2Lin() 
{}

void COpEnv2Lin::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 2 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oEnv.Index() );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_oEnv.State() );
	OP_SET_COUNT_INTERNAL_FLT( 3 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oEnv.Value() );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_dDur );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_dRatio );
	COp::Init();
}

void COpEnv2Lin::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();

	const double dDur = m_poIn[OP_E2L_IN_DUR].Proc();
	const double dRatio = m_poIn[OP_E2L_IN_RATIO].Proc();
	if( m_poIn[OP_E2L_IN_TRIGGER].Proc() > 0.0 )
	{
		if( dDur != m_dDur || dRatio != m_dRatio )
		{
			m_dDur = dDur;
			m_dRatio = dRatio;
			Update();
		}
		m_oEnv.Reset();
		m_oEnv.Trigger();
	}
	m_pdOut[OP_E2L_OUT_DST] = m_oEnv.Proc();
}

void COpEnv2Lin::Update()
{
	m_oEnv.Set( m_dDur, m_dRatio );
	COp::Update();
}

void COpEnv2Lin::Reset()
{
	m_oEnv.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpEnv2Lin, OP_E2L_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpEnv2Lin )
OP_GENERIC_METHODS_DEF( COpEnv2Lin )

#endif // OP_INC_OP_ENV_2_LIN
