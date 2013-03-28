// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqFibo.cpp

#include "OpSeqFibo.h"
#ifdef OP_INC_OP_SEQ_FIBO

////////////////////////////////////////////////////////////////////////////////
// COpSeqFibo
////////////////////////////////////////////////////////////////////////////////

#define OP_OSF_IN_TRIGG			0
#define OP_OSF_IN_VAL0			1
#define OP_OSF_IN_VAL1			2
#define OP_OSF_IN_CLEAR			3
#define OP_OSF_OUT_DST			0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OSF_IN_TRIGG_INFO		"trigger"
#define OP_OSF_IN_VAL0_INFO		"start value 0"
#define OP_OSF_IN_VAL1_INFO		"start value 1"
#define OP_OSF_IN_CLEAR_INFO		"reset"
#define OP_OSF_OUT_DST_INFO		"output"
#define OP_OSF_CLASS_INFO \
\
"Fibonacci Sequence\n" \
"- Whenever the trigger input value is greater than 0.0, a new\n" \
"  sequence value will be calculated.\n" \
"- When the reset input value is greater than 0.0, then the internal\n" \
"  sequence variables will be reset to the given start values.\n" \
"- Start value 0 is 0.0 by default.\n" \
"- Start value 1 is 1.0 by default.\n" \
"- Both start value inlets are only processed when a reset occurs."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OSF_IN_TRIGG_INFO		"Auslöser"
#define OP_OSF_IN_VAL0_INFO		"Startwert 0"
#define OP_OSF_IN_VAL1_INFO		"Startwert 1"
#define OP_OSF_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_OSF_OUT_DST_INFO		"Ausgang"
#define OP_OSF_CLASS_INFO \
\
"Fibonacci-Folge\n	" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird eine neuer\n" \
"  Ausgangs-Wert berechnet.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so werden die beiden Start-\n" \
"  werte neu gesetzt.\n" \
"- Der Startwert 0 ist standardmässig gleich 0.0.\n" \
"- Der Startwert 1 ist standardmässig gleich 1.0.\n" \
"- Die Startwert-Eingänge werden nur beim Zurücksetzen abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSeqFibo::COpSeqFibo()
: COp( 4, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "sqFibo" );
	m_poIn[OP_OSF_IN_VAL0].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSF_IN_VAL1].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSF_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSF_IN_TRIGG, "tr", OP_OSF_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSF_IN_VAL0, "v0", OP_OSF_IN_VAL0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSF_IN_VAL1, "v1", OP_OSF_IN_VAL1_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSF_IN_CLEAR, "cl", OP_OSF_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSF_OUT_DST, "o", OP_OSF_OUT_DST_INFO );

	Validate();
}

COpSeqFibo::~COpSeqFibo()
{}

void COpSeqFibo::Init()
{
	OP_SET_COUNT_INTERNAL_INT( 2 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_oSeq.m_tV0 );
	OP_SET_PTR_INTERNAL_INT( 1, &m_oSeq.m_tV1 );
	Reset();
	COp::Init();
}

void COpSeqFibo::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_OSF_IN_CLEAR].IsValid() &&
	    m_poIn[OP_OSF_IN_CLEAR].Proc() > 0.0 )
	    Reset();

	if( m_poIn[OP_OSF_IN_TRIGG].IsValid() &&
	    m_poIn[OP_OSF_IN_TRIGG].Proc() > 0.0 )
	{
		double dOut = double( m_oSeq.Proc() );
		if( OP_IS_NAN( dOut ) )
			dOut = 0.0;
		m_pdOut[OP_OSF_OUT_DST] = dOut;
	}
}

void COpSeqFibo::Reset()
{
	COp::Reset();
	if( !m_bIsNotValid )
	{
		int iV0 = 0;
		if( m_poIn[OP_OSF_IN_VAL0].IsValid() )
			iV0 = int( rint( m_poIn[OP_OSF_IN_VAL0].Proc() ) );
		int iV1 = 1;
		if( m_poIn[OP_OSF_IN_VAL1].IsValid() )
			iV1 = int( rint( m_poIn[OP_OSF_IN_VAL1].Proc() ) );
		m_oSeq.m_tV0 = iV0;
		m_oSeq.m_tV1 = iV1;
	}
}

OP_SET_NAME_AND_INFO_CLASS( COpSeqFibo, OP_OSF_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSeqFibo )
OP_GENERIC_METHODS_DEF( COpSeqFibo )

#endif // OP_INC_OP_SEQ_FIBO
