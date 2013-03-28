// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnvAdsr.cpp

#include "OpEnvAdsr.h"
#ifdef OP_INC_OP_ENV_ADSR

////////////////////////////////////////////////////////////////////////////////
// COpEnvAdsr
////////////////////////////////////////////////////////////////////////////////

#define OP_EAS_IN_TRIGGERA	0
#define OP_EAS_IN_TRIGGERR	1
#define OP_EAS_IN_S		2
#define OP_EAS_IN_A		3
#define OP_EAS_IN_D		4
#define OP_EAS_IN_R		5
#define OP_EAS_IN_SLOPEA	6
#define OP_EAS_IN_SLOPED	7
#define OP_EAS_IN_SLOPER	8
#define OP_EAS_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_EAS_IN_TRIGGERA_INFO	"A-trigger"
#define OP_EAS_IN_TRIGGERR_INFO	"R-trigger"
#define OP_EAS_IN_S_INFO	"S-value"
#define OP_EAS_IN_A_INFO	"A-duration"
#define OP_EAS_IN_D_INFO	"D-duration"
#define OP_EAS_IN_R_INFO	"R-duration"
#define OP_EAS_IN_SLOPEA_INFO	"A-slope"
#define OP_EAS_IN_SLOPED_INFO	"D-slope"
#define OP_EAS_IN_SLOPER_INFO	"R-slope"
#define OP_EAS_OUT_DST_INFO	"output"
#define OP_EAS_CLASS_INFO \
\
"ADSR Envelope\n" \
"- A new cycle begins when the A-trigger value is greater than 0.0.\n" \
"- When the R-trigger is greater than 0.0, the release stage will be started.\n" \
"- The A-/D-/R-duration values define how long each stage is, in samples.\n" \
"- The S-value defines the output value during the constant sustain stage.\n" \
"- Once the end of the envelope has been reached, the output value\n" \
"  will be 0.0.\n" \
"- Changes on the duration, ratio and slope inlets only influence\n" \
"  the curve in the next cycle.\n" \
"- An incoming A-trigger, while the envelope is already active,\n" \
"  always lets the output value increases until it reaches 1.0.\n" \
"- An incoming R-trigger before the S-stage has begun, lets the curve\n" \
"  directly fall to 0.0.\n" \
"- Negative slope values create an inverse curve and behave completely\n" \
"  symmetrically compared to positive slopes.\n" \
"- The slope value 0.0 defines a linear curve."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_EAS_IN_TRIGGERA_INFO	"A-Auslöser"
#define OP_EAS_IN_TRIGGERR_INFO	"R-Auslöser"
#define OP_EAS_IN_S_INFO	"S-Wert"
#define OP_EAS_IN_A_INFO	"A-Dauer"
#define OP_EAS_IN_D_INFO	"D-Dauer"
#define OP_EAS_IN_R_INFO	"R-Dauer"
#define OP_EAS_IN_SLOPEA_INFO	"A-Krümmung"
#define OP_EAS_IN_SLOPED_INFO	"D-Krümmung"
#define OP_EAS_IN_SLOPER_INFO	"R-Krümmung"
#define OP_EAS_OUT_DST_INFO	"Ausgang"
#define OP_EAS_CLASS_INFO \
\
"ADSR-Hüllkurve\n" \
"- Ist der Wert am A-Auslöser-Eingang grösser als 0.0, so wird eine neuer\n" \
"  Zyklus begonnen.\n" \
"- Ist der Wert am R-Auslöser-Eingang grösser als 0.0, so wird die End-\n" \
"  Stufe gestartet.\n" \
"- Die A-/D-/R-Dauer-Werte geben die Dauer der betreffenden Stufe in Samples\n" \
"  an.\n" \
"- Der S-Wert bestimmt den Ausgabewert der Hüllkurve während der konstanten\n" \
"- S-Stufe.\n" \
"- Ist das Ende der Hüllkurve erreicht, so steht der Ausgang konstant auf 0.0.\n" \
"- Änderungen an Dauer-/Krümmungs-Eingängen beeinflussen die Kurve erst in ihrem\n" \
"  nächsten Zyklus.\n" \
"- Wird während eines Zyklus' der A-Auslöser aktiviert, so steigt die Kurve\n" \
"  aktuellen Wert auf 1.0.\n" \
"- Wird vor der S-Stufe der R-Auslöser aktiviert, so sinkt die Kurve vom\n" \
"  aktuellen Wert auf 0.0.\n" \
"- Negative Krümmungswerte erzeugen umgekehrt gekrümmte Kurven und\n" \
"  verhalten sich vollkommen symmetrisch zum positiven Fall.\n" \
"- Der Krümmungswert 0.0 entspricht etwa einer linearen Kurve."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpEnvAdsr::COpEnvAdsr()
: COp( 9, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "eAdsr" );
	OP_SET_NAME_AND_INFO_INPUT( OP_EAS_IN_TRIGGERA, "ta", OP_EAS_IN_TRIGGERA_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EAS_IN_TRIGGERR, "tr", OP_EAS_IN_TRIGGERR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EAS_IN_S, "s", OP_EAS_IN_S_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EAS_IN_A, "a", OP_EAS_IN_A_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EAS_IN_D, "d", OP_EAS_IN_D_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EAS_IN_R, "r", OP_EAS_IN_R_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EAS_IN_SLOPEA, "sa", OP_EAS_IN_SLOPEA_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EAS_IN_SLOPED, "sd", OP_EAS_IN_SLOPED_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EAS_IN_SLOPER, "sr", OP_EAS_IN_SLOPER_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_EAS_OUT_DST, "o", OP_EAS_OUT_DST_INFO );
	Validate();
}

COpEnvAdsr::~COpEnvAdsr() 
{}

void COpEnvAdsr::Init()
{
	m_dA = m_dD = m_dS = m_dR = m_dSlopeA = m_dSlopeD = m_dSlopeR = 0.0;
	m_dL0 = m_dL1 = 0.0;
	m_oEnvA.Set( 0.0, 0.0, 0.0, 0.0 );
	m_oEnvD.Set( 0.0, 0.0, 0.0, 0.0 );
	m_oEnvR.Set( 0.0, 0.0, 0.0, 0.0 );
	Reset();
	// Speziell wegen: Release-Trigger...
	/*
	OP_SET_COUNT_INTERNAL_UINT( 7 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oEnvA.Index() );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_oEnvA.State() );
	OP_SET_PTR_INTERNAL_UINT( 2, &m_oEnvD.Index() );
	OP_SET_PTR_INTERNAL_UINT( 3, &m_oEnvD.State() );
	OP_SET_PTR_INTERNAL_UINT( 4, &m_oEnvR.Index() );
	OP_SET_PTR_INTERNAL_UINT( 5, &m_oEnvR.State() );
	OP_SET_PTR_INTERNAL_UINT( 6, &m_iPos );
	OP_SET_COUNT_INTERNAL_FLT( 6 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oEnvA.Value() );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oEnvA.Position() );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oEnvD.Value() );
	OP_SET_PTR_INTERNAL_FLT( 3, &m_oEnvD.Position() );
	OP_SET_PTR_INTERNAL_FLT( 4, &m_oEnvR.Value() );
	OP_SET_PTR_INTERNAL_FLT( 5, &m_oEnvR.Position() );
	*/
	COp::Init();
}

void COpEnvAdsr::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dA = m_poIn[OP_EAS_IN_A].Proc();
	const double dD = m_poIn[OP_EAS_IN_D].Proc();
	const double dS = m_poIn[OP_EAS_IN_S].Proc();
	const double dR = m_poIn[OP_EAS_IN_R].Proc();
	const double dSlopeA = m_poIn[OP_EAS_IN_SLOPEA].Proc();
	const double dSlopeD = m_poIn[OP_EAS_IN_SLOPED].Proc();
	const double dSlopeR = m_poIn[OP_EAS_IN_SLOPER].Proc();
	const double dL0 = m_pdOut[OP_EAS_OUT_DST];
	const double dL1 = m_pdOut[OP_EAS_OUT_DST];

	if( m_poIn[OP_EAS_IN_TRIGGERA].Proc() > 0.0 )
	{
		if( dL0 != m_dL0 || dA != m_dA || dSlopeA != m_dSlopeA )
		{
			m_dL0 = dL0;
			m_dA = dA;
			m_dSlopeA = dSlopeA;
			m_oEnvA.Set( m_dA, m_dL0, 1.0, m_dSlopeA );
		}
		if( dS != m_dS || dD != m_dD || dSlopeD != m_dSlopeD )
		{
			m_dS = dS;
			m_dD = dD;
			m_dSlopeD = dSlopeD;
			m_oEnvD.Set( m_dD, 1.0, m_dS, m_dSlopeD );
		}
		m_oEnvA.Reset();
		m_oEnvA.Trigger();
		m_iPos = 0;
	}
	if( m_poIn[OP_EAS_IN_TRIGGERR].Proc() > 0.0 )
	{
		if( dL1 != m_dL1 || dR != m_dR || dSlopeR != m_dSlopeR )
		{
			m_dL1 = dL1;
			m_dR = dR;
			m_dSlopeR = dSlopeR;
			m_oEnvR.Set( m_dR, m_dL1, 0.0, m_dSlopeR );
		}
		m_oEnvR.Reset();
		m_oEnvR.Trigger();
		m_iPos = 3;
	}
	
	double dOut = 0.0;
	if( !m_iPos )
	{
		dOut = m_oEnvA.Proc();
		if( !m_oEnvA.IsOn() )
		{
			dOut = 1.0;
			m_oEnvD.Reset();
			m_oEnvD.Trigger();
			++m_iPos;
		}
	}
	else if( m_iPos == 1 )
	{
		dOut = m_oEnvD.Proc();
		if( !m_oEnvD.IsOn() )
		{
			dOut = m_dS;
			++m_iPos;
		}
	}
	else if( m_iPos == 2 )
		dOut = m_dS;
	else if( m_iPos == 3 )
	{
		dOut = m_oEnvR.Proc();
		if( !m_oEnvR.IsOn() )
			++m_iPos;
	}
	m_pdOut[OP_EAS_OUT_DST] = dOut;
}

void COpEnvAdsr::Reset()
{
	COp::Reset();
	m_oEnvA.Reset();
	m_oEnvD.Reset();
	m_oEnvR.Reset();
	m_iPos = -1;
}

OP_SET_NAME_AND_INFO_CLASS( COpEnvAdsr, OP_EAS_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpEnvAdsr )
OP_GENERIC_METHODS_DEF( COpEnvAdsr )

#endif // OP_INC_OP_ENV_ADSR
