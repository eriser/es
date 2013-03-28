// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpLadder.cpp

#include "OpLadder.h"
#ifdef OP_INC_OP_LADDER

////////////////////////////////////////////////////////////////////////////////
// COpLadder
////////////////////////////////////////////////////////////////////////////////

#define OP_LDR_IN_START			0
#define OP_LDR_IN_STEP			1
#define OP_LDR_OUT_DST			0
#define OP_LDR_FUNC_ADD			0
#define OP_LDR_FUNC_MUL			1
#define OP_LDR_FUNC_MAX			2
#define OP_LDR_ROUTINE_SHRINK		0
#define OP_LDR_ROUTINE_EXPAND		1
#define OP_LDR_ROUTINE_FUNC		2

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_LDR_IN_START_INFO		"start value"
#define OP_LDR_IN_STEP_INFO		"step width"
#define OP_LDR_OUT_NUM_INFO		"number of outputs"
#define OP_LDR_OUT_DST_INFO		"output"
#define OP_LDR_ROUTINE_EXPAND_INFO	"add output"
#define OP_LDR_ROUTINE_SHRINK_INFO	"remove output"
#define OP_LDR_ROUTINE_FUNC_INFO	"function"
#define OP_LDR_CLASS_INFO \
\
"Ladder\n" \
"- A number sequence starting with the start value is written to the\n" \
"  available number of outputs.\n" \
"- Available step functions: ADD, MULTIPLY.\n" \
"- The output in the middle is always equal to the start value. The values\n" \
"  of subsequent outputs are calculated with the step function, previous\n" \
"  output values are calculated with the inverse step function.\n" \
"- The output values are only refreshed when at least one input has changed.\n" \
"- All inlets are processed every cycle."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_LDR_IN_START_INFO		"Startwert"
#define OP_LDR_IN_STEP_INFO		"Schrittweite"
#define OP_LDR_OUT_NUM_INFO		"Anzahl Ausgänge"
#define OP_LDR_OUT_DST_INFO		"Ausgang"
#define OP_LDR_ROUTINE_EXPAND_INFO	"Ausgang hinzufügen"
#define OP_LDR_ROUTINE_SHRINK_INFO	"Ausgang entfernen"
#define OP_LDR_ROUTINE_FUNC_INFO	"Funktion"
#define OP_LDR_CLASS_INFO \
\
"Leiter\n" \
"- Vom Start-Wert ausgehend, wird eine Zahlenfolge berechnet, welche an die\n" \
"  Ausgänge ausgegeben wird.\n" \
"- Als Schrittfunktion stehen die Addition und Multiplikation zur Verfügung.\n" \
"- Der mittlere Ausgang zeigt den Startwert. Die Werte nachfolgender Ausgänge\n" \
"  werden mit der Schrittfunktionen berechnet, vorhergehende mit der umgekehrten\n" \
"  Schrittfunktion.\n" \
"- Die Ausgänge werden nur erneurt, wenn sich min. einer der Eingänge geändert\n" \
"  hat.\n" \
"- Die Eingänge werden trotzdem immer abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpLadder::COpLadder()
: COp( 2, 1 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "lddr" );
	OP_SET_NAME_AND_INFO_INPUT( OP_LDR_IN_START, "i", OP_LDR_IN_START_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_LDR_IN_STEP, "s", OP_LDR_IN_STEP_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_LDR_OUT_DST, "", OP_LDR_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 3 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_LDR_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_LDR_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_LDR_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_LDR_ROUTINE_SHRINK_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_LDR_ROUTINE_FUNC, "f" OP_STR_ROUTINE, OP_LDR_ROUTINE_FUNC_INFO );
	SetFlags( OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpLadder::~COpLadder()
{}

void COpLadder::Init()
{
	m_dIn = m_dStp = 0.0;
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	COp::Init();
}

void COpLadder::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dIn = m_poIn[OP_LDR_IN_START].Proc();
	const double dStp = m_poIn[OP_LDR_IN_STEP].Proc();
	if( dIn != m_dIn || dStp != m_dStp )
	{
		m_dIn = dIn;
		m_dStp = dStp;
		Update();
	}
}

void COpLadder::Update()
{
	const unsigned int uiCount = m_uiCountOut - OP_LDR_OUT_DST;
	const unsigned int uiMid = OP_LDR_OUT_DST + ( uiCount >> 1 );
	int i = uiMid;
	double dV = m_dIn;
	
	m_pdOut[i] = m_dIn;
	while( i > OP_LDR_OUT_DST )
	{
		--i;
		if( m_uiFuncNum == OP_LDR_FUNC_ADD )
			dV -= m_dStp;
		else if( m_dStp )
			dV *= 1.0 / m_dStp;
		else
			dV = 0.0;
		m_pdOut[i] = dV;
	}
	dV = m_dIn;
	i = uiMid + 1;
	while( i < (int)m_uiCountOut )
	{
		if( m_uiFuncNum == OP_LDR_FUNC_ADD )
			dV += m_dStp;
		else
			dV *= m_dStp;
		m_pdOut[i] = dV;
		++i;
	}
	
#ifdef OP_USE_RUNTIME_INFO
	unsigned int o = OP_LDR_OUT_DST;
	while( o < m_uiCountOut )
	{
		SetNameOutput( o, 
			( o == uiMid ? "o"
			: ( m_uiFuncNum == OP_LDR_FUNC_ADD 
				? ( o < uiMid ? "-" : "+" )
				: ( o < uiMid ? "/" : "*" ) ) ) );
		SetInfoOutput( o++, OP_LDR_OUT_DST_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
	COp::Update();
}

#ifdef OP_USE_ROUTINES
void COpLadder::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_LDR_ROUTINE_SHRINK:
		if( m_uiCountOut > OP_LDR_OUT_DST + 1 )
			SetCountOut( m_uiCountOut - 2 );
	break;
	case OP_LDR_ROUTINE_EXPAND:
		SetCountOut( m_uiCountOut + 2 );
	break;
	case OP_LDR_ROUTINE_FUNC:
		if( ++m_uiFuncNum >= OP_LDR_FUNC_MAX )
			m_uiFuncNum = 0;
	break;
	}
	Reset();
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpLadder, OP_LDR_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpLadder )
OP_GENERIC_METHODS_DEF( COpLadder )

#endif // OP_INC_OP_LADDER
