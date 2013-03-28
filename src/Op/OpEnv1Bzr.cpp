// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv1Bzr.cpp

#include "OpEnv1Bzr.h"
#ifdef OP_INC_OP_ENV_1_BZR

////////////////////////////////////////////////////////////////////////////////
// COpEnv1Bzr
////////////////////////////////////////////////////////////////////////////////

#define OP_E1B_IN_TRIGGER	0
#define OP_E1B_IN_DUR		1
#define OP_E1B_IN_V0		2
#define OP_E1B_IN_V1		3
#define OP_E1B_IN_SLOPE		4
#define OP_E1B_IN_CLEAR		5
#define OP_E1B_OUT_DST		0
#define OP_E1B_OUT_ON		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_E1B_IN_TRIGGER_INFO	"trigger"
#define OP_E1B_IN_DUR_INFO	"duration"
#define OP_E1B_IN_V0_INFO	"start value"
#define OP_E1B_IN_V1_INFO	"end value"
#define OP_E1B_IN_SLOPE_INFO	"slope"
#define OP_E1B_IN_CLEAR_INFO	"reset"
#define OP_E1B_OUT_DST_INFO	"output"
#define OP_E1B_OUT_ON_INFO	"state"
#define OP_E1B_CLASS_INFO \
\
"1-Stage B�zier Envelope\n" \
"- A new cycle begins when the trigger value is greater than 0.0.\n" \
"- The duration value defines the total length in samples.\n" \
"- Start and end value define the first and last output value.\n" \
"- Once the end of the envelope has been reached, the output value\n" \
"  remains constantly equal to the end value.\n" \
"- The envelope is reset whenever the reset value is greater than 0.0.\n" \
"- The state output value is 1.0 when the envelope is active, otherwise 0.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_E1B_IN_TRIGGER_INFO	"Ausl�ser"
#define OP_E1B_IN_DUR_INFO	"Dauer"
#define OP_E1B_IN_V0_INFO	"Startwert"
#define OP_E1B_IN_V1_INFO	"Endwert"
#define OP_E1B_IN_SLOPE_INFO	"Kr�mmung"
#define OP_E1B_IN_CLEAR_INFO	"Zur�cksetzen"
#define OP_E1B_OUT_DST_INFO	"Ausgang"
#define OP_E1B_OUT_ON_INFO	"Aktiv"
#define OP_E1B_CLASS_INFO \
\
"1-Stufen B�zier-H�llkurve\n" \
"- Ist der Wert am Ausl�ser-Eingang gr�sser als 0.0, so wird eine neuer\n" \
"  Zyklus begonnen.\n" \
"- Die Dauer d gibt die gesamte Dauer der H�llkurve in Samples an.\n" \
"- Start- und Endwert bestimmen Werte ganz am Anfang und Ende der Kurve.\n" \
"- Ist das Ende der H�llkurve erreicht, so bleibt der Ausgang konstant auf\n" \
"  dem Endwert stehen.\n" \
"- Steht der Zur�cksetzen-Eingang auf einem Wert gr�sser als 0.0, so wird\n" \
"  die H�llkurve auf den Startwert zur�ckgesetzt.\n" \
"- Der Aktiv-Ausgang steht vom Anfang bis am Ende auf 1.0, sonst auf 0.0.\n" \
"- Die B�zier-H�llkurve kann das [Startwert,Endwert]-Interval �berschreiten."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpEnv1Bzr::COpEnv1Bzr()
: COp( 6, 2 )
{
	Init();
	OP_SET_NAME_INSTANCE( "e1bzr" );
	m_poIn[OP_E1B_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1B_IN_TRIGGER, "tr", OP_E1B_IN_TRIGGER_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1B_IN_DUR, "d", OP_E1B_IN_DUR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1B_IN_V0, "v0", OP_E1B_IN_V0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1B_IN_V1, "v1", OP_E1B_IN_V1_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1B_IN_SLOPE, "s", OP_E1B_IN_SLOPE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_E1B_IN_CLEAR, "cl", OP_E1B_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_E1B_OUT_DST, "o", OP_E1B_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_E1B_OUT_ON, "on", OP_E1B_OUT_ON_INFO );
	Update();
	Validate();
}

COpEnv1Bzr::~COpEnv1Bzr()
{}

void COpEnv1Bzr::Init()
{
	m_dDur = m_dV0 = m_dV1 = m_tSlope = 0.0;
	OP_SET_COUNT_INTERNAL_UINT( 2 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oEnv.Index() );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_oEnv.State() );
	OP_SET_COUNT_INTERNAL_FLT( 2 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oEnv.Value() );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oEnv.Position() );
	COp::Init();
}

void COpEnv1Bzr::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_E1B_IN_CLEAR].IsValid() )
		if( m_poIn[OP_E1B_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dDur = m_poIn[OP_E1B_IN_DUR].Proc();
	const double dV0 = m_poIn[OP_E1B_IN_V0].Proc();
	const double dV1 = m_poIn[OP_E1B_IN_V1].Proc();
	const double tSlope = m_poIn[OP_E1B_IN_SLOPE].Proc();
	if( dDur != m_dDur || dV0 != m_dV0 || dV1 != m_dV1 || tSlope != m_tSlope )
	{
		m_dDur = dDur;
		m_dV0 = dV0;
		m_dV1 = dV1;
		m_tSlope = tSlope;
		Update();
	}
	if( m_poIn[OP_E1B_IN_TRIGGER].Proc() > 0.0 )
	{
		m_oEnv.Reset();
		m_oEnv.Trigger();
	}
	m_pdOut[OP_E1B_OUT_ON] = ( m_oEnv.IsOn() ? 1.0 : 0.0 );
	m_pdOut[OP_E1B_OUT_DST] = m_oEnv.Proc();
}

void COpEnv1Bzr::Update()
{
	m_oEnv.Set( m_dDur, m_dV0, m_dV1, m_tSlope );
	COp::Update();
}

void COpEnv1Bzr::Reset()
{
	m_oEnv.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpEnv1Bzr, OP_E1B_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpEnv1Bzr )
OP_GENERIC_METHODS_DEF( COpEnv1Bzr )

#endif // OP_INC_OP_ENV_1_BZR
