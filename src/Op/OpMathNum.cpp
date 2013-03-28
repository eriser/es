// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathNum.cpp

#include "OpMathNum.h"
#ifdef OP_INC_OP_MATH_NUM

////////////////////////////////////////////////////////////////////////////////
// COpMathNum
////////////////////////////////////////////////////////////////////////////////

#define OP_MNM_IN_SRC			0
#define OP_MNM_OUT_DST			0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MNM_IN_SRC_INFO		"input"
#define OP_MNM_OUT_DST_INFO		"output"
#define OP_MNM_ROUTINE_FUNC_INFO	"function"
#define OP_MNM_CLASS_INFO \
\
"Numeric Function\n" \
"- Available functions: ABS, RINT, CEIL, FLOOR.\n" \
"- ABS calculates the absolute value of the input value.\n" \
"- RINT returns the closest integer to the input value.\n" \
"- CEIL returns a value representing the smallest integer\n" \
"  that is greater than or equal to the input value.\n" \
"- FLOOR returns a value representing the largest integer\n" \
"  that is less than or equal to the input value."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MNM_IN_SRC_INFO		"Eingang"
#define OP_MNM_OUT_DST_INFO		"Ausgang"
#define OP_MNM_ROUTINE_FUNC_INFO	"Funktion"
#define OP_MNM_CLASS_INFO \
\
"Nummerische Funktionen\n" \
"- Es stehen folgende Operationen zur Verfügung: ABS, RINT, CEIL, FLOOR.\n" \
"- ABS gibt den Betrag des Arguments zurück.\n" \
"- RINT rundet auf ganze Zahlen.\n" \
"- CEIL gibt die kleinste ganze Zahl zurück, die grösser als oder gleich dem\n" \
"  Argument ist.\n" \
"- FLOOR gibt die grösste ganze Zahl zurück, die nicht grösser als das\n" \
"  Argument ist."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COp::TOpFuncFlt1 COpMathNum::m_atFunc_[] = { fabs, rint, ceil, floor };

COpMathNum::COpMathNum()
: COp( 1, 1 )
, m_uiNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "num" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MNM_IN_SRC, "x", OP_MNM_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MNM_OUT_DST, "", OP_MNM_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_MNM_ROUTINE_FUNC_INFO );
	Update();
	Validate();
}

COpMathNum::~COpMathNum()
{}

void COpMathNum::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiNum );
	COp::Init();
}

void COpMathNum::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	m_pdOut[OP_MNM_OUT_DST] = m_tFunc( m_poIn[OP_MNM_IN_SRC].Proc() );
}

void COpMathNum::Update()
{
	COp::Update();
	m_tFunc = m_atFunc_[m_uiNum];
	
#ifdef OP_USE_RUNTIME_INFO
	static const char * aacFuncName_[] = { "|x|", "r", "c", "f" };
	SetNameOutput( 0, aacFuncName_[m_uiNum] );
#endif // OP_USE_RUNTIME_INF
}

#ifdef OP_USE_ROUTINES
void COpMathNum::Routine( unsigned int uiIndex )
{	
	UNUSED_PARAMETER( uiIndex );
	if( ++m_uiNum >= 4 )
		m_uiNum = 0;
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpMathNum, OP_MNM_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMathNum )
OP_GENERIC_METHODS_DEF( COpMathNum )

#endif // OP_INC_OP_MATH_NUM
