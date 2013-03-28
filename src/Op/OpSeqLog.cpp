// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqLog.cpp

#include "OpSeqLog.h"
#ifdef OP_INC_OP_SEQ_LOG

////////////////////////////////////////////////////////////////////////////////
// COpSeqLog
////////////////////////////////////////////////////////////////////////////////

#define OP_OSL_IN_TRIGG			0
#define OP_OSL_IN_COEFF			1
#define OP_OSL_IN_START			2
#define OP_OSL_IN_CLEAR			3
#define OP_OSL_OUT_DST			0
#define OP_OSL_OUT_OVER			1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OSL_IN_TRIGG_INFO		"trigger"
#define OP_OSL_IN_COEFF_INFO		"coefficient"
#define OP_OSL_IN_START_INFO		"start value"
#define OP_OSL_IN_CLEAR_INFO		"reset"
#define OP_OSL_OUT_DST_INFO		"output"
#define OP_OSL_OUT_OVER_INFO		"overflow trigger"
#define OP_OSL_CLASS_INFO \
\
"Logistic Sequence\n" \
"- Whenever the trigger input value is greater than 0.0, a new\n" \
"  sequence value will be calculated.\n" \
"- When the reset input value is greater than 0.0, then the internal\n" \
"  sequence variable will be reset to the given start value.\n" \
"- The start value is 0.1 by default."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OSL_IN_TRIGG_INFO		"Auslöser"
#define OP_OSL_IN_COEFF_INFO		"Koeffizient"
#define OP_OSL_IN_START_INFO		"Startwert"
#define OP_OSL_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_OSL_OUT_DST_INFO		"Ausgang"
#define OP_OSL_OUT_OVER_INFO		"Überlaufs-Auslöser"
#define OP_OSL_CLASS_INFO \
\
"Logistische Folge\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird eine neuer\n" \
"  Ausgangs-Wert berechnet.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der aktuelle Wert\n" \
"  auf den Startwert gesetzt.\n" \
"- Der Startwert ist standardmässig gleich 0.1."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSeqLog::COpSeqLog()
: COp( 4, 2 )
{
	Init();
	OP_SET_NAME_INSTANCE( "sqLog" );
	m_poIn[OP_OSL_IN_START].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSL_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSL_IN_TRIGG, "tr", OP_OSL_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSL_IN_COEFF, "c", OP_OSL_IN_COEFF_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSL_IN_START, "s", OP_OSL_IN_START_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSL_IN_CLEAR, "cl", OP_OSL_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSL_OUT_DST, "o", OP_OSL_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSL_OUT_OVER, "tr", OP_OSL_OUT_OVER_INFO );
	Validate();
}

COpSeqLog::~COpSeqLog()
{}

void COpSeqLog::Init()
{
	OP_SET_COUNT_INTERNAL_FLT( 3 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oSeq.m_tC );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oSeq.m_tV );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oSeq.m_tV0 );
	Reset();
	COp::Init();
}

void COpSeqLog::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	m_oSeq.m_tC = m_poIn[OP_OSL_IN_COEFF].Proc();

	double dS = 0.1;
	if( m_poIn[OP_OSL_IN_START].IsValid() )
		dS = m_poIn[OP_OSL_IN_START].Proc();
	m_oSeq.m_tV0 = dS;

	if( m_poIn[OP_OSL_IN_CLEAR].IsValid() &&
	    m_poIn[OP_OSL_IN_CLEAR].Proc() > 0.0 )
	    Reset();
	
	double dOver = 0.0;
	if( m_poIn[OP_OSL_IN_TRIGG].Proc() > 0.0 )
	{
		double dOut = double( m_oSeq.Proc() );
		if( OP_IS_VALUE_INVALID( dOut ) )
			dOut = dOver = 1.0;
		m_pdOut[OP_OSL_OUT_DST] = dOut;
	}
	m_pdOut[OP_OSL_OUT_OVER] = dOver;
}

void COpSeqLog::Reset()
{
	COp::Reset();
	/*
	if( !m_bIsNotValid )
	{
		if( m_poIn[OP_OSL_IN_START].IsValid() )
			dS = m_poIn[OP_OSL_IN_START].Proc();
		m_oSeq.m_tV0 = dS;
	}
	*/
	m_oSeq.Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpSeqLog, OP_OSL_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSeqLog )
OP_GENERIC_METHODS_DEF( COpSeqLog )

#endif // OP_INC_OP_SEQ_LOG
