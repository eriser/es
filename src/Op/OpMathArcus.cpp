// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathArcus.cpp

#include "OpMathArcus.h"
#ifdef OP_INC_OP_MATH_ARCUS

////////////////////////////////////////////////////////////////////////////////
// COpMathArcus
////////////////////////////////////////////////////////////////////////////////

#define OP_MAR_IN_SRC			0
#define OP_MAR_OUT_DST			0
#define OP_MAR_ROUTINE_FUNC		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MAR_IN_SRC_INFO		"input"
#define OP_MAR_OUT_DST_INFO		"output"
#define OP_MAR_ROUTINE_FUNC_INFO	"function"
#define OP_MAR_CLASS_INFO \
\
"Arcus Functions\n" \
"- Available functions: ASIN, ACOS, ATAN.\n" \
"- Domain: ASIN/ACOS [-1,+1], ATAN (-inf,+inf).\n" \
"- Range: ASIN [-PI/2,+PI/2], ACOS [0,+PI], ATAN (-PI/2,+PI/2).\n" \
"- The output value is only calculated for valid arguments."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MAR_IN_SRC_INFO		"Eingang"
#define OP_MAR_OUT_DST_INFO		"Ausgang"
#define OP_MAR_ROUTINE_FUNC_INFO	"Funktion"
#define OP_MAR_CLASS_INFO \
\
"Arcus-Funktionen\n" \
"- Es stehen folgende Arcus-Funktionen zur Verfügung: ASIN, ACOS, ATAN.\n" \
"- ASIN/ACOS haben den Definitionsbereich [-1,+1] und ATAN (-inf,+inf).\n" \
"- Der Wertebereich ist für ASIN [-PI/2,+PI/2], für ACOS [0,+PI] und\n" \
"  für ATAN (-PI/2,+PI/2).\n" \
"- Der Funktionswert wird nur bei gültigem Argument berechnet."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COp::TOpFuncFlt1 COpMathArcus::m_atFunc_[] = { asin, acos, atan };

COpMathArcus::COpMathArcus()
: COp( 1, 1 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "arcus" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MAR_IN_SRC, "i", OP_MAR_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MAR_OUT_DST, "", OP_MAR_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_MAR_ROUTINE_FUNC_INFO );
	Update();
	Validate();
}

COpMathArcus::~COpMathArcus()
{}

void COpMathArcus::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	COp::Init();
}

void COpMathArcus::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dIn = m_poIn[OP_MAR_IN_SRC].Proc();
	if( m_uiFuncNum != 2 && ( dIn < -1.0 || dIn > +1.0 ) )
		return;
	m_pdOut[OP_MAR_OUT_DST] = m_tFunc( dIn );
}

void COpMathArcus::Update()
{
	COp::Update();
	m_tFunc = m_atFunc_[m_uiFuncNum];
	
#ifdef OP_USE_RUNTIME_INFO
	static const char * aacFuncName_[] = { "as", "ac", "at" };
	SetNameOutput( 0, aacFuncName_[m_uiFuncNum] );
#endif // OP_USE_RUNTIME_INF
}

#ifdef OP_USE_ROUTINES
void COpMathArcus::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	if( ++m_uiFuncNum >= 3 )
		m_uiFuncNum = 0;
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpMathArcus, OP_MAR_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMathArcus )
OP_GENERIC_METHODS_DEF( COpMathArcus )

#endif // OP_INC_OP_MATH_ARCUS
