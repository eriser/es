// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltComb.cpp

#include "OpFltComb.h"
#ifdef OP_INC_OP_FLT_COMB

////////////////////////////////////////////////////////////////////////////////
// COpFltComb
////////////////////////////////////////////////////////////////////////////////

#define OP_FLC_IN_SRC			0
#define OP_FLC_IN_F			1
#define OP_FLC_IN_G			2
#define OP_FLC_IN_M			3
#define OP_FLC_IN_N			4
#define OP_FLC_IN_CLEAR			5
#define OP_FLC_OUT_DST			0
#define OP_FLC_ROUTINE_SIZE		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FLC_IN_SRC_INFO		"input"
#define OP_FLC_IN_F_INFO		"feedforward coefficient"
#define OP_FLC_IN_G_INFO		"feedback coefficient"
#define OP_FLC_IN_M_INFO		"feedforward buffer size"
#define OP_FLC_IN_N_INFO		"feedback buffer size"
#define OP_FLC_IN_CLEAR_INFO		"reset"
#define OP_FLC_OUT_DST_INFO		"output"
#define OP_FLC_CLASS_INFO \
\
"Comb Filter\n" \
"- The two interpolated delay buffers can be parameterized in size and\n" \
"  influence over the corresponding inlets.\n" \
"- When the reset input value is greater than 0.0, then both internal\n" \
"  buffers will be reset to 0.0.\n" \
"- An overflow can occur when the absolute value of the feedback\n" \
"  coefficient is greater than 1.0!"

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FLC_IN_SRC_INFO		"Eingang"
#define OP_FLC_IN_F_INFO		"Feedforward-Koeffizient"
#define OP_FLC_IN_G_INFO		"Feedback-Koeffizient"
#define OP_FLC_IN_M_INFO		"Feedforward-Puffer-Grösse"
#define OP_FLC_IN_N_INFO		"Feedback-Puffer-Grösse"
#define OP_FLC_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_FLC_OUT_DST_INFO		"Ausgang"
#define OP_FLC_CLASS_INFO \
\
"Comb-Filter\n" \
"- Die beiden interpolierte Verzögerungspuffer können in ihrer Einfluss-Stärke\n" \
"  und Verzögerungs-Dauer über die enstprechenden Eingänge parametrisiert werden.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der Speicher zurück-\n" \
"  gesetzt.\n" \
"- Wenn der Feedback-Koeffizient zu gross gewählt wird, d.h.: sein Betrag\n" \
"  grösser als 1.0 ist, kann es zu einem Overflow kommen."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltComb::COpFltComb()
: COp( 6, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flCmb" );
	m_poIn[OP_FLC_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLC_IN_SRC, "i", OP_FLC_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLC_IN_F, "f", OP_FLC_IN_F_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLC_IN_G, "g", OP_FLC_IN_G_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLC_IN_M, "m", OP_FLC_IN_M_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLC_IN_N, "n", OP_FLC_IN_N_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLC_IN_CLEAR, "cl", OP_FLC_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FLC_OUT_DST, "o", OP_FLC_OUT_DST_INFO );
	Update();
	Validate();
}

COpFltComb::~COpFltComb()
{}

void COpFltComb::Init()
{
	m_dDelayFor = 1.0;
	m_dDelayBack = 1.0;
	COp::Init();
}

void COpFltComb::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_FLC_IN_CLEAR].IsValid() )
		if( m_poIn[OP_FLC_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dDlyF = m_poIn[OP_FLC_IN_M].Proc();
	if( dDlyF != m_dDelayFor )
	{
		m_dDelayFor = dDlyF;
		m_oDelayFor.SetDelay( dDlyF );
	}
	const double dDlyB = fabs( m_poIn[OP_FLC_IN_N].Proc() );
	if( dDlyB != m_dDelayBack )
	{
		m_dDelayBack = dDlyB;
		m_oDelayBack.SetDelay( dDlyB );
	}
	
	const double dIn = m_poIn[OP_FLC_IN_SRC].Proc();
	double dOut = dIn;
	
	dOut += m_poIn[OP_FLC_IN_F].Proc() * m_oDelayFor.ReadDelay();
	dOut -= m_poIn[OP_FLC_IN_G].Proc() * m_oDelayBack.ReadDelay();
	
	m_oDelayFor.WriteDelay( dIn );
	m_oDelayBack.WriteDelay( dOut );
	
	m_pdOut[OP_FLC_OUT_DST] = dOut;
}

void COpFltComb::Update()
{
	m_oDelayFor.SetDelay( m_dDelayFor );
	m_oDelayBack.SetDelay( m_dDelayBack );
	//m_oDelayFor.UpdateSize();
	//m_oDelayBack.UpdateSize();
	COp::Update();
}

void COpFltComb::Reset()
{
	m_oDelayFor.Reset();
	m_oDelayBack.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpFltComb, OP_FLC_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltComb )
OP_GENERIC_METHODS_DEF( COpFltComb )


#endif // OP_INC_OP_FLT_COMB
