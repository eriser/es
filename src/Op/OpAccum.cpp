// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpAccum.cpp

#include "OpAccum.h"
#ifdef OP_INC_OP_ACCUM

////////////////////////////////////////////////////////////////////////////////
// COpAccum
////////////////////////////////////////////////////////////////////////////////

#define OP_ACC_IN_SRC			0
#define OP_ACC_IN_CLEAR			1
#define OP_ACC_OUT_DST			0

#define OP_ACC_FUNC_ADD			0
#define OP_ACC_FUNC_MUL			1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_ACC_IN_SRC_INFO		"accumulation value"
#define OP_ACC_IN_CLEAR_INFO		"reset"
#define OP_ACC_OUT_DST_INFO		"output"
#define OP_STR_ROUTINE_INFO		"mode"
#define OP_ACC_CLASS_INFO \
\
"Accumulator\n" \
"- Every cycle, the accumulation value will be added to the current\n" \
"  internal accumulation variable.\n" \
"- When the reset input value is greater than 0.0, then the internal\n" \
"  accumulation variable will be set to 0.0 in additive mode, or to 1.0\n" \
"  in multiplicative mode.\n" \
"- By default, the accumulation value is equals 1.0 in additive mode,\n" \
"  or 2.0 in multiplicative mode.\n" \
"- Process: output current accumulation variable, then the reset inlet will\n" \
"  be processed, and after that the accumulation value inlet."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_ACC_IN_SRC_INFO		"Akkumulationswert"
#define OP_ACC_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_ACC_OUT_DST_INFO		"Ausgang"
#define OP_STR_ROUTINE_INFO		"Modus"
#define OP_ACC_CLASS_INFO \
\
" Akkumulator\n" \
"- In jedem Zyklus wird die aktuelle Statusvariable mit dem Eingabe-\n" \
"  wert addiert oder multipliziert.\n" \
"- Zuerst wird der aktuelle Stand ausgegeben, dann wird der Reset-Eingang\n" \
"  abgefragt und entsprechend reagiert. Erst danach wird akkumuliert.\n" \
"- Wenn der Zurücksetzen-Eingang gleich 1.0 ist, wird ein Reset ausgelöst.\n" \
"- Nach einem Reset wird im Additions-Modus mit 0.0,\n" \
"  oder mit 1.0 im Multiplikations-Modus begonnen.\n" \
"- Standardmässig ist der Akkumulationswert gleich 1.0 im Add.-Modus,\n" \
"  oder auf 2.0 im Multi.-Modus."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpAccum::COpAccum()
: COp( 2, 1 )
, m_dAccum( 0.0 )
, m_uiFuncNum( OP_ACC_FUNC_ADD )
{
	Init();
	OP_SET_NAME_INSTANCE( "acc" );
	m_poIn[OP_ACC_IN_SRC].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_ACC_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_ACC_IN_SRC, "i", OP_ACC_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_ACC_IN_CLEAR, "cl", OP_ACC_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_ACC_OUT_DST, "+", OP_ACC_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_STR_ROUTINE_INFO );
	Validate();
}

COpAccum::~COpAccum()
{}

void COpAccum::Init()
{
	OP_SET_COUNT_INTERNAL_FLT( 1 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dAccum );
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	COp::Init();
} 

void COpAccum::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	m_pdOut[OP_ACC_OUT_DST] = m_dAccum;
	
	if( m_poIn[OP_ACC_IN_CLEAR].IsValid() )
		if( m_poIn[OP_ACC_IN_CLEAR].Proc() > 0.0 ) // Trigger
		{
			Reset();
			m_pdOut[OP_ACC_OUT_DST] = m_dAccum;
		}
	
	if( m_poIn[OP_ACC_IN_SRC].IsValid() )
	{
		if( m_uiFuncNum )
			m_dAccum *= m_poIn[OP_ACC_IN_SRC].Proc();
		else
			m_dAccum += m_poIn[OP_ACC_IN_SRC].Proc();
	}
	else
	{
		if( m_uiFuncNum )
			m_dAccum *= 2.0;
		else
			m_dAccum += 1.0;
	}
}

void COpAccum::Update()
{
	COp::Update();
	
#ifdef OP_USE_RUNTIME_INFO
	static const char * aacName_[] = { "+", "*" };
	SetNameOutput( OP_ACC_OUT_DST, aacName_[m_uiFuncNum] );
#endif // OP_USE_RUNTIME_INF
}

void COpAccum::Reset()
{
	m_dAccum = ( m_uiFuncNum == OP_ACC_FUNC_MUL )?( 1.0 ):( 0.0 );
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpAccum::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	m_uiFuncNum = !m_uiFuncNum;
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpAccum, OP_ACC_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpAccum )
OP_GENERIC_METHODS_DEF( COpAccum )

#endif // OP_INC_OP_ACCUM
