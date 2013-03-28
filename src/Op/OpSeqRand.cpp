// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqRand.cpp

#include "OpSeqRand.h"
#ifdef OP_INC_OP_SEQ_RAND

////////////////////////////////////////////////////////////////////////////////
// COpSeqRand
////////////////////////////////////////////////////////////////////////////////

#define OP_SRN_IN_TRIGG			0
#define OP_SRN_IN_BALANCE		1
#define OP_SRN_IN_SEED			2
#define OP_SRN_IN_CLEAR			3
#define OP_SRN_OUT_DST			0
#define OP_SRN_ROUTINE_FUNC		0
#define OP_SRN_ROUTINE_PINK		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_SRN_IN_TRIGG_INFO		"trigger"
#define OP_SRN_IN_BALANCE_INFO		"balance"
#define OP_SRN_IN_SEED_INFO		"seed"
#define OP_SRN_IN_CLEAR_INFO		"reset"
#define OP_SRN_OUT_DST_INFO		"output"
#define OP_SRN_ROUTINE_FUNC_INFO	"type"
#define OP_SRN_CLASS_INFO \
\
"Random Number Sequence\n" \
"- When the reset input value is greater than 0.0, the sequence will\n" \
"  be reinitialized.\n" \
"- The same seed value always generates the same sequence.\n" \
"- Available types: WHITE_NOISE, PINK_NOISE.\n" \
"- The balance input controls the amount of feedback.\n" \
"- The start value inlet is only processed when a reset occurs."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_SRN_IN_TRIGG_INFO		"Auslöser"
#define OP_SRN_IN_BALANCE_INFO		"Balance"
#define OP_SRN_IN_SEED_INFO		"Startwert"
#define OP_SRN_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_SRN_OUT_DST_INFO		"Ausgang"
#define OP_SRN_ROUTINE_FUNC_INFO	"Typ"
#define OP_SRN_CLASS_INFO \
\
"Zufallszahlen\n" \
"- Steht der Zurücksetzen-Eingang auf einem Wert grösser als 0.0, so wird\n" \
"  die Folge neu initialisiert.\n" \
"- Bei gleichem Startwert, wird nach dem Zurücksetzen immer die gleiche\n" \
"  Sequenz ausgegeben.\n" \
"- Es kann zwischen weissem und pinkem Rauschen gewählt werden.\n" \
"- Die Balance regelt den Anteil an Feedback, der zum Eingang zurückgeführt\n" \
"  wird.\n" \
"- Der Startwert wird nur beim Zurücksetzen neu eingelesen.\n"

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSeqRand::COpSeqRand()
: COp( 4, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "sqRnd" );
	m_poIn[OP_SRN_IN_BALANCE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_SRN_IN_SEED].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_SRN_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_SRN_IN_TRIGG, "tr", OP_SRN_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SRN_IN_BALANCE, "b", OP_SRN_IN_BALANCE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SRN_IN_SEED, "s", OP_SRN_IN_SEED_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SRN_IN_CLEAR, "cl", OP_SRN_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SRN_OUT_DST, "o", OP_SRN_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_SRN_ROUTINE_FUNC_INFO );
	Update();
	Validate();
}

COpSeqRand::~COpSeqRand()
{}

void COpSeqRand::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_oSeqRand.Type() );
	OP_SET_COUNT_INTERNAL_INT( 3 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_oSeqRand.m_iSeed );
	OP_SET_PTR_INTERNAL_INT( 1, &m_oSeqRand.m_iS0 );
	OP_SET_PTR_INTERNAL_INT( 2, &m_oSeqRand.m_iS1 );
	Reset();
	COp::Init();
}

void COpSeqRand::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_SRN_IN_CLEAR].IsValid() &&
	    m_poIn[OP_SRN_IN_CLEAR].Proc() > 0.0 )
		Reset();
	
	if( m_poIn[OP_SRN_IN_BALANCE].IsValid() )
		m_oSeqRand.SetBalance( m_poIn[OP_SRN_IN_BALANCE].Proc() );
	else
		m_oSeqRand.SetBalance( 0.0 );
	
	if( m_poIn[OP_SRN_IN_TRIGG].Proc() > 0.0 )
		m_pdOut[OP_SRN_OUT_DST] = m_oSeqRand.Proc();
}

void COpSeqRand::Update()
{
	COp::Update();
	
#ifdef OP_USE_RUNTIME_INFO
	SetNameOutput( 0, ((m_oSeqRand.Type() == COpTkSeqRandomEx<double>::PINK)?("pi"):("wh")) );
#endif // OP_USE_RUNTIME_INFO
}

void COpSeqRand::Reset()
{
	if( !m_bIsNotValid && m_poIn[OP_SRN_IN_SEED].IsValid() )
		m_oSeqRand.m_iSeed = (int)rint( m_poIn[OP_SRN_IN_SEED].Proc() );
	m_oSeqRand.Reset();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpSeqRand::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	m_oSeqRand.NextType();
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpSeqRand, OP_SRN_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSeqRand )
OP_GENERIC_METHODS_DEF( COpSeqRand )

#endif // OP_INC_OP_SEQ_RAND
