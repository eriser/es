// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathTrigo.cpp

#include "OpMathTrigo.h"
#ifdef OP_INC_OP_MATH_TRIGO

////////////////////////////////////////////////////////////////////////////////
// COpMathTrigo
////////////////////////////////////////////////////////////////////////////////

#define OP_MTR_IN_SRC			0
#define OP_MTR_OUT_DST			0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MTR_IN_SRC_INFO		"input"
#define OP_MTR_OUT_DST_INFO		"output"
#define OP_MTR_ROUTINE_FUNC_INFO	"function"
#define OP_MTR_CLASS_INFO \
\
"Trigonometric Function\n" \
"- Available functions: SIN, COS, TAN."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MTR_IN_SRC_INFO		"Eingang"
#define OP_MTR_OUT_DST_INFO		"Ausgang"
#define OP_MTR_ROUTINE_FUNC_INFO	"Funktion"
#define OP_MTR_CLASS_INFO \
\
"Trigonometrische Funktion\n" \
"- Es stehen die trigonometrischen Funktionen SIN, COS und TAN zur Verfügung."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COp::TOpFuncFlt1 COpMathTrigo::m_atFunc_[] = { sin, cos, tan };

COpMathTrigo::COpMathTrigo()
: COp( 1, 1 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "trigo" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MTR_IN_SRC, "x", OP_MTR_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MTR_OUT_DST, "", OP_MTR_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_MTR_ROUTINE_FUNC_INFO );
	Update();
	Validate();
}

COpMathTrigo::~COpMathTrigo()
{}

void COpMathTrigo::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	COp::Init();
}

void COpMathTrigo::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	m_pdOut[OP_MTR_OUT_DST] = m_tFunc( m_poIn[OP_MTR_IN_SRC].Proc() );
}

void COpMathTrigo::Update()
{
	COp::Update();
	m_tFunc = m_atFunc_[m_uiFuncNum];
	
#ifdef OP_USE_RUNTIME_INFO
	const char * aacFuncName[] = { "sin", "cos", "tan" };
	SetNameOutput( 0, aacFuncName[m_uiFuncNum] );
#endif // OP_USE_RUNTIME_INF
}

#ifdef OP_USE_ROUTINES
void COpMathTrigo::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	if( ++m_uiFuncNum >= 3 )
		m_uiFuncNum = 0;
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpMathTrigo, OP_MTR_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMathTrigo )
OP_GENERIC_METHODS_DEF( COpMathTrigo )

#endif // OP_INC_OP_MATH_TRIGO
