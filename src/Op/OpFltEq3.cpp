// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltEq3.cpp

#include "OpFltEq3.h"
#ifdef OP_INC_OP_FLT_EQ_3

////////////////////////////////////////////////////////////////////////////////
// COpFltEq3
////////////////////////////////////////////////////////////////////////////////

#define OP_EQ3_IN_SRC			0
#define OP_EQ3_IN_FREQ0			1
#define OP_EQ3_IN_FREQ1			2
#define OP_EQ3_IN_LOW			3
#define OP_EQ3_IN_MID			4
#define OP_EQ3_IN_HIGH			5
#define OP_EQ3_IN_CLEAR			6
#define OP_EQ3_OUT_DST			0
#define OP_EQ3_ROUTINE_SIZE		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_EQ3_IN_SRC_INFO		"input"
#define OP_EQ3_IN_FREQ0_INFO		"frequency 0"
#define OP_EQ3_IN_FREQ1_INFO		"frequency 1"
#define OP_EQ3_IN_LOW_INFO		"bass"
#define OP_EQ3_IN_MID_INFO		"mid"
#define OP_EQ3_IN_HIGH_INFO		"treble"
#define OP_EQ3_IN_CLEAR_INFO		"reset"
#define OP_EQ3_OUT_DST_INFO		"output"
#define OP_EQ3_CLASS_INFO \
\
"3-Band Equalizer\n" \
"- The whole spectrum is split into three frequency bands.\n" \
"- The first band goes from 0 Hz up to frequency 0.\n" \
"- The second band goes from frequency 0 up to frequency 1.\n" \
"- The third band goes from frequency 1 up to half the sampling frequency.\n" \
"- The bass, mid and treble inlets define how much influence each band has.\n" \
"- When the reset input value is greater than 0.0, then all the internal\n" \
"  variables will be reinitialized."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_EQ3_IN_SRC_INFO		"Eingang"
#define OP_EQ3_IN_FREQ0_INFO		"Frequenz 0"
#define OP_EQ3_IN_FREQ1_INFO		"Frequenz 1"
#define OP_EQ3_IN_LOW_INFO		"Tiefen"
#define OP_EQ3_IN_MID_INFO		"Mitten"
#define OP_EQ3_IN_HIGH_INFO		"Höhen"
#define OP_EQ3_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_EQ3_OUT_DST_INFO		"Ausgang"
#define OP_EQ3_CLASS_INFO \
\
"3-Band Equalizer\n" \
"- Das Spektrum wird in 3 Bänder aufgeteilt.\n" \
"- Das erste Band geht von 0 Hz bis Frequenz 0.\n" \
"- Das zweite Band geht von Frequenz 0 bis Frequenz 1.\n" \
"- Das dritte Band geht von Frequenz 1 bis zur halben Sampling-Frequenz.\n" \
"- Der Tiefen-/Mitten- und Höhen-Eingang bestimmt die jeweilige Verstärkung.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der Speicher zurück-\n" \
"  gesetzt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltEq3::COpFltEq3()
: COp( 7, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flEq3" );
	m_poIn[OP_EQ3_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_EQ3_IN_SRC, "i", OP_EQ3_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EQ3_IN_FREQ0, "f0", OP_EQ3_IN_FREQ0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EQ3_IN_FREQ1, "f1", OP_EQ3_IN_FREQ1_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EQ3_IN_LOW, "l", OP_EQ3_IN_LOW_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EQ3_IN_MID, "m", OP_EQ3_IN_MID_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EQ3_IN_HIGH, "h", OP_EQ3_IN_HIGH_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EQ3_IN_CLEAR, "cl", OP_EQ3_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_EQ3_OUT_DST, "o", OP_EQ3_OUT_DST_INFO );
	Update();
	Validate();
}

COpFltEq3::~COpFltEq3()
{}

void COpFltEq3::Init()
{
	m_dF0 = m_dF1 = m_dF2 = 0.0;
	OP_SET_COUNT_INTERNAL_FLT( 3 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oFlt.m_tSample0 );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oFlt.m_tSample1 );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oFlt.m_tSample2 );
	COp::Init();
}

void COpFltEq3::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_EQ3_IN_CLEAR].IsValid() )
		if( m_poIn[OP_EQ3_IN_CLEAR].Proc() > 0.0 )
			m_oFlt.Reset();
	
	const double dF0 = m_poIn[OP_EQ3_IN_FREQ0].Proc();
	const double dF1 = m_poIn[OP_EQ3_IN_FREQ1].Proc();
	if( dF0 != m_dF0 || dF1 != m_dF1 )
	{
		m_dF0 = dF0;
		m_dF1 = dF1;
		Update();
	}
	m_oFlt.InitGain(
		m_poIn[OP_EQ3_IN_LOW].Proc(),
		m_poIn[OP_EQ3_IN_MID].Proc(),
		m_poIn[OP_EQ3_IN_HIGH].Proc() );
	
	m_pdOut[OP_EQ3_OUT_DST] = m_oFlt.Proc( m_poIn[OP_EQ3_IN_SRC].Proc() );
}

void COpFltEq3::Update()
{
	m_oFlt.InitFreq( fabs( m_dF0 ), fabs( m_dF1 ) );
	COp::Update();
}

void COpFltEq3::Reset()
{
	m_oFlt.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpFltEq3, OP_EQ3_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltEq3 )
OP_GENERIC_METHODS_DEF( COpFltEq3 )

#endif // OP_INC_OP_FLT_EQ_3
