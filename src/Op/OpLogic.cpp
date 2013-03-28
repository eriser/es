// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpLogic.cpp

#include "OpLogic.h"
#ifdef OP_INC_OP_LOGIC

////////////////////////////////////////////////////////////////////////////////
// COpLogic
////////////////////////////////////////////////////////////////////////////////

#define OP_LGC_IN_SRC0			0
#define OP_LGC_IN_SRC1			1
#define OP_LGC_OUT_DST			0
#define OP_LGC_ROUTINE_FUNC		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_LGC_IN_SRC0_INFO		"first operand"
#define OP_LGC_IN_SRC1_INFO		"second operand"
#define OP_LGC_OUT_DST_INFO		"output"
#define OP_LGC_ROUTINE_FUNC_INFO	"operator"
#define OP_LGC_CLASS_INFO \
\
"Logical Conjunction\n" \
"- Available operators: AND, OR, NOT.\n" \
"- NOT, as an unary operator, is only using the first operand.\n" \
"- The operands are considered true when they are greater than 0.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_LGC_IN_SRC0_INFO		"erster Operand"
#define OP_LGC_IN_SRC1_INFO		"zweiter Operand"
#define OP_LGC_OUT_DST_INFO		"Ausgang"
#define OP_LGC_ROUTINE_FUNC_INFO	"Operator"
#define OP_LGC_CLASS_INFO \
\
"Logische Verknüpfung\n" \
"- Die Operanden werden logisch miteinander verknüpft, dabei stehen\n" \
"  die Opertoren AND, OR und NOT zur Verfügung.\n" \
"- NOT, als unärer Operator, wird nur auf den ersten Operand angewendet.\n" \
"- Die Operanden gelten als wahr, wenn sie grösser als 0.0 sind."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpLogic::TOpFuncBool2 COpLogic::m_atFunc_[] = { 
	COpLogic::FuncAND_,
	COpLogic::FuncOR_,
	COpLogic::FuncNOT_ 
};

COpLogic::COpLogic()
: COp( 2, 1 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "logic" );
	OP_SET_NAME_AND_INFO_INPUT( OP_LGC_IN_SRC0, "x", OP_LGC_IN_SRC0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_LGC_IN_SRC1, "y", OP_LGC_IN_SRC1_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_LGC_OUT_DST, "", OP_LGC_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_LGC_ROUTINE_FUNC_INFO );
	Update();
	Validate();
}

COpLogic::~COpLogic()
{}

void COpLogic::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	COp::Init();
}

void COpLogic::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	m_pdOut[OP_LGC_OUT_DST] = 
			( m_tFunc( m_poIn[OP_LGC_IN_SRC0].Proc() > 0.0,
				   m_poIn[OP_LGC_IN_SRC1].Proc() > 0.0 ) )
			?( 1.0 )
			:( 0.0 );
}

void COpLogic::Update()
{
	COp::Update();
	m_tFunc = m_atFunc_[m_uiFuncNum];
	
#ifdef OP_USE_RUNTIME_INFO
	static const char * aacFuncName_[] = { "&&", "||", "!x" };
	SetNameOutput( 0, aacFuncName_[m_uiFuncNum] );
#endif // OP_USE_RUNTIME_INF
}

#ifdef OP_USE_ROUTINES
void COpLogic::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	++m_uiFuncNum;
	if( m_uiFuncNum >= 3 )
		m_uiFuncNum = 0;
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpLogic, OP_LGC_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpLogic )
OP_GENERIC_METHODS_DEF( COpLogic )

#endif // OP_INC_OP_LOGIC
