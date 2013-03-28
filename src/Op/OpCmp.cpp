// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpCmp.cpp

#include "OpCmp.h"
#ifdef OP_INC_OP_CMP

////////////////////////////////////////////////////////////////////////////////
// COpCmp
////////////////////////////////////////////////////////////////////////////////

#define OP_CMP_IN_SRC0		0
#define OP_CMP_IN_SRC1		1
#define OP_CMP_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_CMP_IN_SRC0_INFO		"first operand"
#define OP_CMP_IN_SRC1_INFO		"second operand"
#define OP_CMP_OUT_DST_INFO		"output"
#define OP_CMP_ROUTINE_FUNC_INFO	"operation"
#define OP_CMP_CLASS_INFO \
\
"Compare\n" \
"- Both operands are compared with each other.\n" \
"- Available operators: >, >=, <, <=, ==, !=.\n" \
"- If the comparison is true, then the output value is 1.0, otherwise 0.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_CMP_IN_SRC0_INFO		"erster Operand"
#define OP_CMP_IN_SRC1_INFO		"zweiter Operand"
#define OP_CMP_OUT_DST_INFO		"Ausgang"
#define OP_CMP_ROUTINE_FUNC_INFO	"Operation"
#define OP_CMP_CLASS_INFO \
\
"Vergleich\n" \
"- Die beiden Operanden werden miteinander verglichen,\n" \
"  dabei stehen die Operatoren >, >=, <, <=, ==, != zur Verfügung.\n" \
"- Ist der Vergleich wahr, steht der Ausgang auf 1.0, sonst auf 0.0."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpCmp::TOpFuncFlt2 COpCmp::m_atFunc_[] = { 
	COpCmp::FuncGreater_,
	COpCmp::FuncGreaterOrEqual_,
	COpCmp::FuncLess_,
	COpCmp::FuncLessOrEqual_,
	COpCmp::FuncEqual_,
	COpCmp::FuncNotEqual_ };

COpCmp::COpCmp()
: COp( 2, 1 )
, m_uiNum( 0 )
, m_tFunc( m_atFunc_[0] )
{
	Init();
	OP_SET_NAME_INSTANCE( "cmp" );
	OP_SET_NAME_AND_INFO_INPUT( OP_CMP_IN_SRC0, "x", OP_CMP_IN_SRC0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CMP_IN_SRC1, "y", OP_CMP_IN_SRC1_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CMP_OUT_DST, "", OP_CMP_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_CMP_ROUTINE_FUNC_INFO );
	Validate();
}

COpCmp::~COpCmp()
{}

void COpCmp::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiNum );
	COp::Init();
}

void COpCmp::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	m_pdOut[OP_CMP_OUT_DST] = 
		m_tFunc( m_poIn[OP_CMP_IN_SRC0].Proc(), 
			 m_poIn[OP_CMP_IN_SRC1].Proc() );
}

void COpCmp::Update()
{
	COp::Update();
	
	m_tFunc = m_atFunc_[m_uiNum];
	
#ifdef OP_USE_RUNTIME_INFO
	static const char * aacFuncName_[] = { ">", ">=", "<", "<=", "==", "!=" };
	SetNameOutput( OP_CMP_OUT_DST, aacFuncName_[m_uiNum] );
#endif // OP_USE_RUNTIME_INFO
}

#ifdef OP_USE_ROUTINES
void COpCmp::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	if( ++m_uiNum >= 6 )
		m_uiNum = 0;
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpCmp, OP_CMP_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpCmp )
OP_GENERIC_METHODS_DEF( COpCmp )

#endif // OP_INC_OP_CMP
