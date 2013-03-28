// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltAllpass.cpp

#include "OpFltAllpass.h"
#ifdef OP_INC_OP_FLT_ALLPASS

////////////////////////////////////////////////////////////////////////////////
// COpFltAllpass
////////////////////////////////////////////////////////////////////////////////

#define OP_FLA_IN_SRC			0
#define OP_FLA_IN_COEFF			1
#define OP_FLA_IN_DELAY			2
#define OP_FLA_OUT_DST			0
#define OP_FLA_ROUTINE_SIZE		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FLA_IN_SRC_INFO		"input"
#define OP_FLA_IN_COEFF_INFO		"coefficient"
#define OP_FLA_IN_DELAY_INFO		"delay"
#define OP_FLA_OUT_DST_INFO		"output"
#define OP_FLA_CLASS_INFO \
\
"Allpass Filter\n" \
"- This filter type changes the phase relationship between frequencies.\n" \
"- The delay value defines the overall delay of the input signal.\n" \
"- The minimal delay is 1 sample.\n" \
"- An overflow can occur when the coefficient value is too large or too small!"

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FLA_IN_SRC_INFO		"Eingang"
#define OP_FLA_IN_COEFF_INFO		"Koeffizient"
#define OP_FLA_IN_DELAY_INFO		"Verzögerung"
#define OP_FLA_OUT_DST_INFO		"Ausgang"
#define OP_FLA_CLASS_INFO \
\
"Allpass-Filter\n" \
"- Das Signal wird frequenzabhängig verzögert.\n" \
"- Der Verzögerungs-Eingang gibt die Anzahl Samples der Verzögerung des\n" \
"  Eingangssignals an.\n" \
"- Die minimale Verzögerung beträgt 1.\n" \
"- Wenn der Koeffizient zu gross gewählt wird, d.h.: sein Betrag\n" \
"  grösser als 1.0 ist, kann zu einem Overflow kommen (starkes Feedback). Dann\n" \
"  sollten der Koeffizient neu gewählt und danach ein Reset ausgeführt werden."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltAllpass::COpFltAllpass()
: COp( 3, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flAllp" );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLA_IN_SRC, "i", OP_FLA_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLA_IN_COEFF, "c", OP_FLA_IN_COEFF_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLA_IN_DELAY, "d", OP_FLA_IN_DELAY_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FLA_OUT_DST, "o", OP_FLA_OUT_DST_INFO );
	Update();
	Validate();
}

COpFltAllpass::~COpFltAllpass()
{}

void COpFltAllpass::Init()
{
	m_dDelay = 1.0;
	m_oDelay.SetDelay( 1u );
	COp::Init();
}

void COpFltAllpass::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dIn = m_poIn[OP_FLA_IN_SRC].Proc();
	const double dCoeff = m_poIn[OP_FLA_IN_COEFF].Proc();
	const double dDly = m_poIn[OP_FLA_IN_DELAY].Proc();
	if( dDly != m_dDelay )
	{
		m_dDelay = dDly;
		m_oDelay.SetDelay( dDly );
	}
	
	const double dOut = m_oDelay.ReadDelay();
	const double dT = dIn + dOut * dCoeff;
	m_oDelay.WriteDelay( dT );
	m_pdOut[OP_FLA_OUT_DST] = dOut - dT * dCoeff;
}

void COpFltAllpass::Update()
{
	m_oDelay.UpdateSize();
	COp::Update();
}

void COpFltAllpass::Reset()
{
	m_oDelay.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpFltAllpass, OP_FLA_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltAllpass )
OP_GENERIC_METHODS_DEF( COpFltAllpass )

#endif // OP_INC_OP_FLT_ALLPASS
