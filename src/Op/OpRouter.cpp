// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpRouter.cpp

#include "OpRouter.h"
#ifdef OP_INC_OP_ROUTER

////////////////////////////////////////////////////////////////////////////////
// COpRouter
////////////////////////////////////////////////////////////////////////////////

#define OP_RTR_IN_POS			0
#define OP_RTR_IN_SRC			1
#define OP_RTR_OUT_NUM			0
#define OP_RTR_OUT_SYNC			1
#define OP_RTR_OUT_DST			2
#define OP_RTR_ROUTINE_SHRINK		0
#define OP_RTR_ROUTINE_EXPAND		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_RTR_IN_POS_INFO		"position"
#define OP_RTR_IN_SRC_INFO		"input"
#define OP_RTR_OUT_NUM_INFO		"number of outputs"
#define OP_RTR_OUT_SYNC_INFO		"sync"
#define OP_RTR_OUT_DST_INFO		"output"
#define OP_RTR_ROUTINE_EXPAND_INFO	"add output"
#define OP_RTR_ROUTINE_SHRINK_INFO	"remove output"
#define OP_RTR_CLASS_INFO \
\
"Router\n" \
"- The position parameter defines the current outlet which incoming\n" \
"  input values are written to. Positions out of range are wrapped.\n" \
"- When the position is not an integer, then the input value is\n" \
"  distributed among two outlets, so that the sum of both output values is\n" \
"  always equal to the input value.\n" \
"- Inactive outlets always output 0.0 by default.\n" \
"- When the current position changes, the sync output value is 1.0,\n" \
"  otherwise always 0.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_RTR_IN_POS_INFO		"Postion"
#define OP_RTR_IN_SRC_INFO		"Eingang"
#define OP_RTR_OUT_NUM_INFO		"Anzahl Ausgänge"
#define OP_RTR_OUT_SYNC_INFO		"Synchronisation"
#define OP_RTR_OUT_DST_INFO		"Ausgang"
#define OP_RTR_ROUTINE_EXPAND_INFO	"Ausgang hinzufügen"
#define OP_RTR_ROUTINE_SHRINK_INFO	"Ausgang entfernen"
#define OP_RTR_CLASS_INFO \
\
"Router\n" \
"- Der Positions-Wert bezeichnet den aktuellen Ausgang, an welchen der\n" \
"  Eingabewert ausgegeben wird. Er verhält sich zyklisch.\n" \
"- Ist der Positions-Wert nicht ganzzahlig, so wird der Eingabewert so auf\n" \
"  den aktuellen Ausgang und dessen Nachfolger bzw. Vorgänger verteilt, dass\n" \
"  die Summe der beiden Ausgabewerte gleich dem Eingabewert ist.\n" \
"- Alle nicht aktiven Ausgänge stehen immer auf 0.0.\n" \
"- Der Synchronisationsausgang steht auf 1.0, wenn eine neue Position erreicht\n" \
"  wird, andernfalls auf 0.0."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpRouter::COpRouter()
: COp( 2, 3 )
, m_iPosPrev( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "router" );
	OP_SET_NAME_AND_INFO_INPUT( OP_RTR_IN_POS, "p", OP_RTR_IN_POS_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_RTR_IN_SRC, "i", OP_RTR_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_RTR_OUT_NUM, "n", OP_RTR_OUT_NUM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_RTR_OUT_SYNC, "sy", OP_RTR_OUT_SYNC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_RTR_OUT_DST, "o", OP_RTR_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_RTR_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_RTR_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_RTR_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_RTR_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpRouter::~COpRouter()
{}

void COpRouter::Init()
{
	OP_SET_COUNT_INTERNAL_INT( 1 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_iPosPrev );
	COp::Init();
}

void COpRouter::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dIn = m_poIn[OP_RTR_IN_SRC].Proc();
	const int iNum = (int)m_uiCountOut - OP_RTR_OUT_DST;
	const double dPos = m_poIn[OP_RTR_IN_POS].Proc();
	double dPosInt;
	const double dPosFrc = modf( dPos, &dPosInt );
	int iPos = (int)dPosInt % iNum;
	if( iPos < 0 )
		iPos += iNum;
	
	iPos += OP_RTR_OUT_DST;
	OP_MEM_ZERO( &m_pdOut[OP_RTR_OUT_DST], iNum * sizeof(double) );
	int iPos1 = iPos;
	if( dPosFrc == 0.0 )
	{
		m_pdOut[iPos] = dIn;
	}
	else if( dPosFrc > 0.0 )
	{
		if( ++iPos1 == (int)m_uiCountOut )
			iPos1 = OP_RTR_OUT_DST;
		m_pdOut[iPos] = dIn * ( 1.0 - dPosFrc );
		m_pdOut[iPos1] = dIn * dPosFrc;
	}
	else
	{
		if( iPos == OP_RTR_OUT_DST )
			iPos1 = (int)m_uiCountOut - 1;
		else
			--iPos1;
		m_pdOut[iPos] = dIn * ( 1.0 + dPosFrc );
		m_pdOut[iPos1] = dIn * ( - dPosFrc );
	}
	
	m_pdOut[OP_RTR_OUT_SYNC] = ( iPos == m_iPosPrev )?( 0.0 ):( +1.0 );
	m_iPosPrev = iPos;
}

void COpRouter::Update()
{
	m_pdOut[OP_RTR_OUT_NUM] = double( m_uiCountOut - OP_RTR_OUT_DST );
	
#ifdef OP_USE_RUNTIME_INFO
	unsigned int o = OP_RTR_OUT_DST;
	while( o < m_uiCountOut )
	{
		SetNameOutput( o, "o" );
		SetInfoOutput( o++, OP_RTR_OUT_DST_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
	COp::Update();
}

#ifdef OP_USE_ROUTINES
void COpRouter::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_RTR_ROUTINE_SHRINK:
		if( m_uiCountOut > OP_RTR_OUT_DST + 1 )
			SetCountOut( m_uiCountOut - 1 );
	break;
	case OP_RTR_ROUTINE_EXPAND:
		SetCountOut( m_uiCountOut + 1 );
	break;
	}
	Reset();
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpRouter, OP_RTR_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpRouter )
OP_GENERIC_METHODS_DEF( COpRouter )

#endif // OP_INC_OP_ROUTER
