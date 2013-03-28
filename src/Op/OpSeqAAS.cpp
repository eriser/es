// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqAAS.cpp

#include "OpSeqAAS.h"
#ifdef OP_INC_OP_SEQ_AAS

////////////////////////////////////////////////////////////////////////////////
// COpSeqAAS
////////////////////////////////////////////////////////////////////////////////

#define OP_OSA_IN_TRIGG		0
#define OP_OSA_IN_SIZE		1
#define OP_OSA_IN_START		2
#define OP_OSA_IN_CLEAR		3
#define OP_OSA_OUT_DST		0
#define OP_OSA_OUT_IM		1
#define OP_OSA_OUT_RE		2
#define OP_OSA_OUT_OVER		3

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OSA_IN_TRIGG_INFO	"trigger"
#define OP_OSA_IN_SIZE_INFO	"count"
#define OP_OSA_IN_START_INFO	"start value"
#define OP_OSA_IN_CLEAR_INFO	"reset"
#define OP_OSA_OUT_DST_INFO	"output"
#define OP_OSA_CLASS_INFO \
\
"Audioactive Sequence (Conway)\n" \
"- Whenever the trigger input value is greater than 0.0, the next\n" \
"  number of the sequence will be output.\n" \
"- The absolute value of the start input value will be split up\n" \
"  into a sequence of integer digits, and then fed into the generator.\n" \
"- When the reset input value is greater than 0.0, then the current\n" \
"  value is reset to the first number of the sequence.\n" \
"- The default start value is 1.0.\n" \
"- The count input value specifies the total number of sequence\n" \
"  numbers to be calculated and stored internally.\n" \
"- When the last number has been reached, it will continue at the\n" \
"  the beginning automatically.\n" \
"- The whole sequence will be precalculated in the first cycle, and\n" \
"  after every change on the count and start value inlet.\n" \
"- All inlets are processed every cycle."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OSA_IN_TRIGG_INFO	"Auslöser"
#define OP_OSA_IN_SIZE_INFO	"Anzahl"
#define OP_OSA_IN_START_INFO	"Start-Wert"
#define OP_OSA_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_OSA_OUT_DST_INFO	"Ausgang"
#define OP_OSA_CLASS_INFO \
\
"Conway-Folge\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird die nächste\n" \
"  Zahl der Folge ausgegeben.\n" \
"- Der Absolutbetrag des Start-Werts wird in Ziffern aufgeteilt und dann\n" \
"  in den Generator eingespeist.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der aktuelle Wert\n" \
"  auf die erste Zahl der Folge zurückgesetzt.\n" \
"- Ist das Start-Wert-Eingang nicht verbunden, so wird der Start-Wert\n" \
"  auf 1.0 gesetzt.\n" \
"- Der Anzahl-Eingang bestimmt die gesamte Anzahl der zu berechnenden\n" \
"  Zahlen, die intern zwischengespeichert werden.\n" \
"- Ist der grösste Index erreicht, wird beim Anfang fortgefahren.\n" \
"- Die Zahlen werden im ersten Zyklus, und bei jeder Änderung am\n" \
"  Anzahl- und Start-Wert-Eingang neu vorberechnet.\n" \
"- Alle Eingänge werden immer abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSeqAAS::COpSeqAAS()
: COp( 4, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "sqAAS" );
	m_poIn[OP_OSA_IN_START].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSA_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSA_IN_TRIGG, "tr", OP_OSA_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSA_IN_SIZE, "c", OP_OSA_IN_SIZE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSA_IN_START, "s", OP_OSA_IN_START_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSA_IN_CLEAR, "cl", OP_OSA_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSA_OUT_DST, "o", OP_OSA_OUT_DST_INFO );
	Validate();
}

COpSeqAAS::~COpSeqAAS()
{}

void COpSeqAAS::Init()
{
	UpdateSize( 1.0, 1.0 );
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oSeq.m_uiPos );
	COp::Init();
}

void COpSeqAAS::UpdateSize( double dSize, double dStart )
{
	m_dSize = dSize;
	m_dStart = dStart;
	m_oSeq.Precalc(
		(unsigned int)abs( (int)rint( dSize ) ),
		(unsigned int)abs( (int)rint( dStart ) ) );
}

void COpSeqAAS::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_OSA_IN_CLEAR].IsValid() &&
	    m_poIn[OP_OSA_IN_CLEAR].Proc() > 0.0 )
	    Reset();
	
	const double dSize = m_poIn[OP_OSA_IN_SIZE].Proc();
	double dStart = 1.0;
	if( m_poIn[OP_OSA_IN_START].IsValid() )
	   dStart = m_poIn[OP_OSA_IN_START].Proc();

	if( m_dSize != dSize || m_dStart != dStart )
	{
		UpdateSize( dSize, dStart );
	}
	
	if( m_poIn[OP_OSA_IN_TRIGG].Proc() > 0.0 )
		m_pdOut[OP_OSA_OUT_DST] = double( m_oSeq.Proc() );
}

void COpSeqAAS::Reset()
{
	m_oSeq.Reset();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpSeqAAS, OP_OSA_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSeqAAS )
OP_GENERIC_METHODS_DEF( COpSeqAAS )

#endif // OP_INC_OP_SEQ_AAS
