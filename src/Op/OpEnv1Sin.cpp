// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv1Sin.cpp

#include "OpEnv1Sin.h"
#ifdef OP_INC_OP_ENV_1_SIN

////////////////////////////////////////////////////////////////////////////////
// COpEnv1Sin
////////////////////////////////////////////////////////////////////////////////

#define OP_E1S_IN_TRIGGER	0
#define OP_E1S_IN_DUR		1
#define OP_E1S_IN_V0		2
#define OP_E1S_IN_V1		3
#define OP_E1S_IN_SLOPE		4
#define OP_E1S_IN_CLEAR		5
#define OP_E1S_OUT_DST		0
#define OP_E1S_OUT_ON		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_E1S_IN_TRIGGER_INFO	"trigger"
#define OP_E1S_IN_DUR_INFO	"duration"
#define OP_E1S_IN_V0_INFO	"start value"
#define OP_E1S_IN_V1_INFO	"end value"
#define OP_E1S_IN_SLOPE_INFO	"slope"
#define OP_E1S_IN_CLEAR_INFO	"reset"
#define OP_E1S_OUT_DST_INFO	"output"
#define OP_E1S_OUT_ON_INFO	"state"
#define OP_E1S_CLASS_INFO \
\
"1-Stage Sine Envelope\n" \
"- A new cycle begins when the trigger value is greater than 0.0.\n" \
"- The duration value defines the total length in samples.\n" \
"- Start and end value define the extreme values of the curve.\n" \
"- Once the end of the envelope has been reached, the output value\n" \
"  remains constantly equal to the end value.\n" \
"- The envelope is reset whenever the reset value is greater than 0.0.\n" \
"- The state output value is 1.0 when the envelope is active, otherwise 0.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_E1S_IN_TRIGGER_INFO	"Auslöser"
#define OP_E1S_IN_DUR_INFO	"Dauer"
#define OP_E1S_IN_V0_INFO	"Startwert"
#define OP_E1S_IN_V1_INFO	"Endwert"
#define OP_E1S_IN_SLOPE_INFO	"Krümmung"
#define OP_E1S_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_E1S_OUT_DST_INFO	"Ausgang"
#define OP_E1S_OUT_ON_INFO	"Aktiv"
#define OP_E1S_CLASS_INFO \
\
"1-Stufen Sinus-Hüllkurve\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird eine neuer\n" \
"  Zyklus begonnen.\n" \
"- Die Dauer d gibt die gesamte Dauer der Hüllkurve in Samples an.\n" \
"- Start- und Endwert bestimmen die Extremwerte der Kurve.\n" \
"- Ist das Ende der Hüllkurve erreicht, so bleibt der Ausgang konstant auf\n" \
"  dem Endwert stehen.\n" \
"- Steht der Zurücksetzen-Eingang auf einem Wert grösser als 1.0, so wird\n" \
"  die Hüllkurve auf den Startwert zurückgesetzt.\n" \
"- Der Aktiv-Ausgang steht vom Anfang bis am Ende auf 1.0, sonst auf 0.0."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpEnv1Sin::COpEnv1Sin()
: COp( 6, 2 )
{
	Init();
	OP_SET_NAME_INSTANCE( "e1sin" );
	m_poIn[OP_E1S_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1S_IN_TRIGGER, "tr", OP_E1S_IN_TRIGGER_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1S_IN_DUR, "d", OP_E1S_IN_DUR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1S_IN_V0, "v0", OP_E1S_IN_V0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1S_IN_V1, "v1", OP_E1S_IN_V1_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1S_IN_SLOPE, "s", OP_E1S_IN_SLOPE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1S_IN_CLEAR, "cl", OP_E1S_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_E1S_OUT_DST, "o", OP_E1S_OUT_DST_INFO );
	Validate();
}

COpEnv1Sin::~COpEnv1Sin() 
{}

void COpEnv1Sin::Init()
{
	m_dDur = m_dV0 = m_dV1 = m_dSlope = 0.0;
	OP_SET_COUNT_INTERNAL_UINT( 2 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oEnv.Index() );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_oEnv.State() );
	OP_SET_COUNT_INTERNAL_FLT( 3 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oEnv.Value() );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oEnv.Position() );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oEnv.Exponent() );
	COp::Init();
}

void COpEnv1Sin::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_E1S_IN_CLEAR].IsValid() )
		if( m_poIn[OP_E1S_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dDur = m_poIn[OP_E1S_IN_DUR].Proc();
	const double dV0 = m_poIn[OP_E1S_IN_V0].Proc();
	const double dV1 = m_poIn[OP_E1S_IN_V1].Proc();
	const double dSlope = m_poIn[OP_E1S_IN_SLOPE].Proc();
	if( dDur != m_dDur || dV0 != m_dV0 || dV1 != m_dV1 || dSlope != m_dSlope  )
	{
		m_dDur = dDur;
		m_dV0 = dV0;
		m_dV1 = dV1;
		m_dSlope = dSlope;
		Update();
	}
	if( m_poIn[OP_E1S_IN_TRIGGER].Proc() > 0.0 )
	{
		m_oEnv.Reset();
		m_oEnv.Trigger();
	}
	m_pdOut[OP_E1S_OUT_ON] = ( m_oEnv.IsOn() ? 1.0 : 0.0 );
	m_pdOut[OP_E1S_OUT_DST] = m_oEnv.Proc();
}

void COpEnv1Sin::Update()
{
	m_oEnv.Set( m_dDur, m_dV0, m_dV1, m_dSlope );
	COp::Update();
}

void COpEnv1Sin::Reset()
{
	m_oEnv.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpEnv1Sin, OP_E1S_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpEnv1Sin )
OP_GENERIC_METHODS_DEF( COpEnv1Sin )

#endif // OP_INC_OP_ENV_1_SIN
