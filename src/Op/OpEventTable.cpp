// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEventTable.cpp

#include "OpEventTable.h"
#ifdef OP_INC_OP_EVENT_TABLE

////////////////////////////////////////////////////////////////////////////////
// COpEventTable
////////////////////////////////////////////////////////////////////////////////

#define OP_EVT_IN_SRC			0
#define OP_EVT_IN_TRIGG			1
#define OP_EVT_IN_VAL			2
#define OP_EVT_OUT_NUM			0
#define OP_EVT_OUT_TRIGG		1
#define OP_EVT_OUT_DST			2

#define OP_EVT_ROUTINE_SHRINK		0
#define OP_EVT_ROUTINE_EXPAND		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_EVT_IN_SRC_INFO		"test"
#define OP_EVT_IN_TRIGG_INFO		"trigger"
#define OP_EVT_IN_VAL_INFO		"probe"
#define OP_EVT_OUT_NUM_INFO		"number of probes"
#define OP_EVT_OUT_TRIGG_INFO		"trigger"
#define OP_EVT_OUT_DST_INFO		"output"
#define OP_EVT_ROUTINE_EXPAND_INFO	"add probe"
#define OP_EVT_ROUTINE_SHRINK_INFO	"remove probe"
#define OP_EVT_CLASS_INFO \
\
"Event Table\n" \
"- A new test sequence starts when the trigger input is greater than 0.0.\n" \
"- During a test sequence, each probe value will be compared with the current\n" \
"  test value.\n" \
"- When a probe value is equal to the test value, then the corresponding output\n" \
"  value is set to 1.0, otherwise to 0.0.\n" \
"- The global trigger output is 1.0, when at least one comparison is true,\n" \
"  otherwise 0.0.\n" \
"- All output values are 0.0, when no test sequence is executed.\n" \
"- All inlets are processed every cycle."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_EVT_IN_SRC_INFO		"Test-Wert"
#define OP_EVT_IN_TRIGG_INFO		"Auslöser"
#define OP_EVT_IN_VAL_INFO		"Wert"
#define OP_EVT_OUT_NUM_INFO		"Anzahl Werte"
#define OP_EVT_OUT_TRIGG_INFO		"Auslöser"
#define OP_EVT_OUT_DST_INFO		"Ausgang"
#define OP_EVT_ROUTINE_EXPAND_INFO	"Wert hinzufügen"
#define OP_EVT_ROUTINE_SHRINK_INFO	"Wert entfernen"
#define OP_EVT_CLASS_INFO \
\
"Ereignis-Tabelle\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird ein Test-Durchlauf\n" \
"  begonnen.\n" \
"- In einem Test-Durchlauf wird der Test-Wert mit allen Eingabe-Werte vergleichen.\n" \
"  Sind beide gleich, so wird der zugehörige Ausgang und der global Auslöser-\n" \
"  Ausgang auf 1.0 gesetzt, sonst auf 0.0.\n" \
"- Wird kein Test-Durchlauf durchgeführt, so stehen alle Ausgänge auf 0.0.\n" \
"- Alle Eingänge werden immer abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpEventTable::COpEventTable()
: COp( 3, 3 )
{
	OP_SET_NAME_INSTANCE( "etTab" );
	m_poIn[OP_EVT_IN_VAL].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_EVT_IN_SRC, "i", OP_EVT_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EVT_IN_TRIGG, "tr", OP_EVT_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EVT_IN_VAL, "", OP_EVT_IN_VAL_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_EVT_OUT_NUM, "n", OP_EVT_OUT_NUM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_EVT_OUT_TRIGG, "tr", OP_EVT_OUT_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_EVT_OUT_DST, "", OP_EVT_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_EVT_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_EVT_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_EVT_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_EVT_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS | OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpEventTable::~COpEventTable()
{}

void COpEventTable::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dIn = m_poIn[OP_EVT_IN_SRC].Proc();
	const double *pdDEnd = m_pdOut + m_uiCountOut;
	double *pdD = &m_pdOut[OP_EVT_OUT_DST];
	CLink *poS = &m_poIn[OP_EVT_IN_VAL];
	double dTrigg = 0.0;
	
	if( m_poIn[OP_EVT_IN_TRIGG].Proc() > 0.0 )
	{
		const int i = int( rint( dIn ) );
		while( pdD < pdDEnd )
		{
			if( poS->IsValid() )
			{
				*pdD = ( i == int( rint( poS->Proc() ) ) ? 1.0 : 0.0 );
				dTrigg = 1.0;
			}
			else
				*pdD = 0.0;
			++pdD;
			++poS;
		}
	}
	else
	{
		while( pdD < pdDEnd )
		{
			*pdD = 0.0;
			if( poS->IsValid() )
				poS->Proc();
			++pdD;
			++poS;
		}
	}
	m_pdOut[OP_EVT_OUT_TRIGG] = dTrigg;
}

void COpEventTable::Update()
{
	m_pdOut[OP_EVT_OUT_NUM] = double( m_uiCountIn - OP_EVT_IN_VAL );
	
#ifdef OP_USE_RUNTIME_INFO
	unsigned int i = OP_EVT_IN_VAL;
	while( i < m_uiCountIn )
	{
		SetNameInput( i, "v" );
		SetInfoInput( i++, OP_EVT_IN_VAL_INFO );
	}
	i = OP_EVT_OUT_DST;
	while( i < m_uiCountOut )
	{
		SetNameOutput( i, "o" );
		SetInfoOutput( i++, OP_EVT_OUT_DST_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
	COp::Update();
}

#ifdef OP_USE_ROUTINES
void COpEventTable::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_EVT_ROUTINE_SHRINK:
		if( m_uiCountIn > OP_EVT_IN_VAL + 1 )
		{
			SetCountIn( m_uiCountIn - 1 );
			SetCountOut( m_uiCountOut - 1 );
		}
	break;
	case OP_EVT_ROUTINE_EXPAND:
		SetCountIn( m_uiCountIn + 1 );
		SetCountOut( m_uiCountOut + 1 );
		m_poIn[m_uiCountIn - 1].SetFlags( OP_LINK_FLAG_OPTIONAL );
	break;
	}
	Reset();
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpEventTable, OP_EVT_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpEventTable )
OP_GENERIC_METHODS_DEF( COpEventTable )

#endif // OP_INC_OP_EVENT_TABLE
