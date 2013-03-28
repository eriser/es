// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpCombiner.cpp

#include "OpCombiner.h"
#ifdef OP_INC_OP_COMBINER

////////////////////////////////////////////////////////////////////////////////
// COpCombiner
////////////////////////////////////////////////////////////////////////////////

#define OP_CMB_IN_SRC			0
#define OP_CMB_OUT_NUM			0
#define OP_CMB_OUT_DST			1

#define OP_CMB_ROUTINE_FUNC		0
#define OP_CMB_ROUTINE_SHRINK		1
#define OP_CMB_ROUTINE_EXPAND		2

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_CMB_IN_SRC_INFO		"input"
#define OP_CMB_OUT_NUM_INFO		"number of inputs"
#define OP_CMB_OUT_DST_INFO		"output"
#define OP_CMB_ROUTINE_FUNC_INFO	"operation"
#define OP_CMB_ROUTINE_EXPAND_INFO	"add input"
#define OP_CMB_ROUTINE_SHRINK_INFO	"remove input"
#define OP_CMB_CLASS_INFO \
\
"Combiner\n" \
"- An arbitrary number of input values are added or multiplied."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_CMB_IN_SRC_INFO		"Eingang"
#define OP_CMB_OUT_NUM_INFO		"Anzahl Eingänge"
#define OP_CMB_OUT_DST_INFO		"Ausgang"
#define OP_CMB_ROUTINE_FUNC_INFO	"Operation"
#define OP_CMB_ROUTINE_EXPAND_INFO	"Eingang hinzufügen"
#define OP_CMB_ROUTINE_SHRINK_INFO	"Eingang entfernen"
#define OP_CMB_CLASS_INFO \
\
"Kombinations-Operator\n" \
"- Eine beliebige Anzahl Operanden können addiert oder multiplziert werden."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpCombiner::COpCombiner()
: COp( 1, 2 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "cmb" );
	OP_SET_NAME_AND_INFO_INPUT( OP_CMB_IN_SRC, "", OP_CMB_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CMB_OUT_NUM, "n", OP_CMB_OUT_NUM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CMB_OUT_DST, "o", OP_CMB_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 3 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_CMB_ROUTINE_FUNC, "f" OP_STR_ROUTINE, OP_CMB_ROUTINE_FUNC_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_CMB_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_CMB_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_CMB_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_CMB_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS );
	Update();
	Validate();
}

COpCombiner::~COpCombiner() 
{}

void COpCombiner::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	COp::Init();
}

void COpCombiner::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	CLink *poS = m_poIn + OP_CMB_IN_SRC;
	const CLink * const poSEnd = poS + m_uiCountIn;
	double dOut = 0.0;
	
	if( m_uiFuncNum )
	{
		dOut = 1.0;
		while( poS < poSEnd )
		{
			dOut *= poS->Proc();
			++poS;
		}
	}
	else
	{
		while( poS < poSEnd )
		{
			dOut += poS->Proc();
			++poS;
		}
	}
	m_pdOut[OP_CMB_OUT_DST] = dOut;
}

void COpCombiner::Update()
{
	m_pdOut[OP_CMB_OUT_NUM] = double( m_uiCountIn );

#ifdef OP_USE_RUNTIME_INFO
	SetNameOutput( OP_CMB_OUT_DST, ( m_uiFuncNum ? "*" : "+" ) );
	
	unsigned int i = OP_CMB_IN_SRC;
	while( i < m_uiCountIn )
	{
		SetNameInput( i, "i" );
		SetInfoInput( i++, OP_CMB_IN_SRC_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
}

#ifdef OP_USE_ROUTINES
void COpCombiner::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_CMB_ROUTINE_FUNC:
		m_uiFuncNum = !m_uiFuncNum;
	break;
	case OP_CMB_ROUTINE_SHRINK:
		if( m_uiCountIn >= OP_CMB_IN_SRC + 1 )
			SetCountIn( m_uiCountIn - 1 );
	break;
	case OP_CMB_ROUTINE_EXPAND:
		SetCountIn( m_uiCountIn + 1 );
	break;
	}
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpCombiner, OP_CMB_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpCombiner )
OP_GENERIC_METHODS_DEF( COpCombiner )

#endif // OP_INC_OP_COMBINER
