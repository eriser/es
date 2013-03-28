// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqReca.cpp

#include "OpSeqReca.h"
#ifdef OP_INC_OP_SEQ_RECA

////////////////////////////////////////////////////////////////////////////////
// COpSeqReca
////////////////////////////////////////////////////////////////////////////////

#define OP_OSR_IN_TRIGG			0
#define OP_OSR_IN_SIZE			1
#define OP_OSR_IN_INDEX			2
#define OP_OSR_IN_VAL			3
#define OP_OSR_IN_CLEAR			4
#define OP_OSR_OUT_DST			0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OSR_IN_TRIGG_INFO		"trigger"
#define OP_OSR_IN_SIZE_INFO		"count"
#define OP_OSR_IN_INDEX_INFO		"index"
#define OP_OSR_IN_VAL_INFO		"start value"
#define OP_OSR_IN_CLEAR_INFO		"reset"
#define OP_OSR_OUT_DST_INFO		"output"
#define OP_OSR_CLASS_INFO \
\
"Recamán Sequence\n" \
"- Whenever the trigger input value is greater than 0.0, a new sequence\n" \
"  value will be calculated.\n" \
"- Start and index values are 0.0 by default.\n" \
"- The count parameter defines the number of elements after which the\n" \
"  sequence repeats itself.\n" \
"- The start and index value inlets are only processed when a reset or\n" \
"  trigger event occurs."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OSR_IN_TRIGG_INFO		"Auslöser"
#define OP_OSR_IN_SIZE_INFO		"Anzahl"
#define OP_OSR_IN_INDEX_INFO		"Index"
#define OP_OSR_IN_VAL_INFO		"Startwert"
#define OP_OSR_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_OSR_OUT_DST_INFO		"Ausgang"
#define OP_OSR_CLASS_INFO \
\
"Recamán-Folge\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird eine neuer\n" \
"  Ausgangs-Wert berechnet.\n" \
"- Startwert und Index sind standardmässig gleich 0.0.\n" \
"- Die Folge wiederholt sich nach der am Anzahl-Eingang angegebenen Anzahl\n" \
"  Gliedern.\n" \
"- Startwert- und Index-Eingang werden nur bei Auslöser-Aktivierung und beim\n" \
"  Zurücksetzen abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSeqReca::COpSeqReca()
: COp( 5, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "sqRca" );
	m_poIn[OP_OSR_IN_INDEX].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSR_IN_VAL].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSR_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSR_IN_TRIGG, "tr", OP_OSR_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSR_IN_SIZE, "s", OP_OSR_IN_SIZE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSR_IN_INDEX, "i", OP_OSR_IN_INDEX_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSR_IN_VAL, "v", OP_OSR_IN_VAL_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSR_IN_CLEAR, "cl", OP_OSR_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSR_OUT_DST, "o", OP_OSR_OUT_DST_INFO );
	Validate();
}

COpSeqReca::~COpSeqReca()
{}

void COpSeqReca::Init()
{
	UpdateSize( 1.0 );
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oSeq.m_uiPos );
	COp::Init();
}

void COpSeqReca::UpdateSize( double dSize )
{
	m_dSize = dSize;
	m_oSeq.SetSize( (unsigned int)abs( (int)rint( dSize ) ) );
	m_oSeq.Precalc();
}

void COpSeqReca::UpdateInit()
{
	int iInitV = 0, iInitIndex = 0;
	if( m_poIn[OP_OSR_IN_VAL].IsValid() )
		iInitV = int( rint( m_poIn[OP_OSR_IN_VAL].Proc() ) );
	if( m_poIn[OP_OSR_IN_INDEX].IsValid() )
		iInitIndex = int( rint( m_poIn[OP_OSR_IN_INDEX].Proc() ) );
	m_oSeq.m_tInitV = iInitV;
	m_oSeq.m_iInitIndex = iInitIndex;
}

void COpSeqReca::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_OSR_IN_CLEAR].IsValid() &&
	    m_poIn[OP_OSR_IN_CLEAR].Proc() > 0.0 )
	    Reset();
	
	const double dSize = m_poIn[OP_OSR_IN_SIZE].Proc();
	if( m_dSize != dSize )
	{
		UpdateInit();
		m_oSeq.Init();
		UpdateSize( dSize );
	}
	if( m_poIn[OP_OSR_IN_TRIGG].IsValid() &&
	    m_poIn[OP_OSR_IN_TRIGG].Proc() > 0.0 )
	{
		UpdateInit();
		m_pdOut[OP_OSR_OUT_DST] = double( m_oSeq.Proc() );
	}
}

void COpSeqReca::Update()
{
	UpdateInit();
	COp::Update();
}

void COpSeqReca::Reset()
{
	COp::Reset();
	m_oSeq.Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpSeqReca, OP_OSR_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSeqReca )
OP_GENERIC_METHODS_DEF( COpSeqReca )

#endif // OP_INC_OP_SEQ_RECA
