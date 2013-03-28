// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqPrime.cpp

#include "OpSeqPrime.h"
#ifdef OP_INC_OP_SEQ_PRIME

////////////////////////////////////////////////////////////////////////////////
// COpSeqPrime
////////////////////////////////////////////////////////////////////////////////

#define OP_OSP_IN_TRIGG		0
#define OP_OSP_IN_MIN		1
#define OP_OSP_IN_SIZE		2
#define OP_OSP_IN_CLEAR		3
#define OP_OSP_OUT_DST		0
#define OP_OSP_OUT_IM		1
#define OP_OSP_OUT_RE		2
#define OP_OSP_OUT_OVER		3

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OSP_IN_TRIGG_INFO	"trigger"
#define OP_OSP_IN_MIN_INFO	"start index"
#define OP_OSP_IN_SIZE_INFO	"count"
#define OP_OSP_IN_CLEAR_INFO	"reset"
#define OP_OSP_OUT_DST_INFO	"output"
#define OP_OSP_CLASS_INFO \
\
"Prime Number Sequence\n" \
"- Whenever the trigger input value is greater than 0.0, the next\n" \
"  prime number will be output.\n" \
"- The smallest prime number is 2.0, which has an index of 0.0.\n" \
"- When the reset input value is greater than 0.0, then the current\n" \
"  value is reset to the prime number with the given start index.\n" \
"- The count input value specifies the total number of prime numbers\n" \
"  to be calculated, inclusively those with smaller index than the\n" \
"  start index.\n" \
"- When the last number has been reached, it will continue at the\n" \
"  start index.\n" \
"- The whole sequence will be precalculated in the first cycle.\n" \
"- All inlets are processed every cycle."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OSP_IN_TRIGG_INFO	"Auslöser"
#define OP_OSP_IN_MIN_INFO	"Start-Index"
#define OP_OSP_IN_SIZE_INFO	"Anzahl"
#define OP_OSP_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_OSP_OUT_DST_INFO	"Ausgang"
#define OP_OSP_CLASS_INFO \
\
"Primzahlen-Folge\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird die nächst-\n" \
"  grössere Primzahl ausgegeben.\n" \
"- Die kleinste Primzahl ist gleich 2.0 und hat den Index 0.0.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der aktuelle Wert\n" \
"  auf die Primzahl mit dem Start-Index zurückgesetzt.\n" \
"- Der Anzahl-Eingang bestimmt die gesamte Anzahl der zu berechnenden\n" \
"  Primzahlen, einschliesslich derer mit einem kleineren Index als der\n" \
"  Start-Index.\n" \
"- Ist der grösste Index erreicht, wird beim Start-Index fortgefahren.\n" \
"- Die Primzahlen werden im ersten Zyklus berechnet und dann gespeichert.\n" \
"- Alle Eingänge werden immer abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSeqPrime::COpSeqPrime()
: COp( 4, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "sqPrm" );
	m_poIn[OP_OSP_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSP_IN_TRIGG, "tr", OP_OSP_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSP_IN_MIN, "m", OP_OSP_IN_MIN_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSP_IN_SIZE, "s", OP_OSP_IN_SIZE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSP_IN_CLEAR, "cl", OP_OSP_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSP_OUT_DST, "o", OP_OSP_OUT_DST_INFO );
	Validate();
}

COpSeqPrime::~COpSeqPrime()
{}

void COpSeqPrime::Init()
{
	UpdateSize( 1.0 );
	UpdateMin( 0.0 );
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oSeq.m_uiPos );
	COp::Init();
}

void COpSeqPrime::UpdateSize( double dSize )
{
	m_dSize = dSize;
	m_oSeq.SetSize( (unsigned int)abs( (int)rint( dSize ) ) );
}

void COpSeqPrime::UpdateMin( double dMin )
{
	m_dMin = dMin;
	m_oSeq.SetMin( (unsigned int)abs( (int)rint( dMin ) ) );
	m_oSeq.Precalc();
}

void COpSeqPrime::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_OSP_IN_CLEAR].IsValid() &&
	    m_poIn[OP_OSP_IN_CLEAR].Proc() > 0.0 )
	    Reset();
	
	const double dSize = m_poIn[OP_OSP_IN_SIZE].Proc();
	const double dMin = m_poIn[OP_OSP_IN_MIN].Proc();
	if( m_dSize != dSize )
	{
		UpdateSize( dSize );
		UpdateMin( dMin );
	}
	if( m_dMin != dMin )
		UpdateMin( dMin );
	
	if( m_poIn[OP_OSP_IN_TRIGG].Proc() > 0.0 )
		m_pdOut[OP_OSP_OUT_DST] = double( m_oSeq.Proc() );
}

void COpSeqPrime::Reset()
{
	m_oSeq.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpSeqPrime, OP_OSP_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSeqPrime )
OP_GENERIC_METHODS_DEF( COpSeqPrime )

#endif // OP_INC_OP_SEQ_PRIME
