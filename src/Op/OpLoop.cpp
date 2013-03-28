// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpLoop.cpp

#include "OpLoop.h"
#ifdef OP_INC_OP_LOOP

////////////////////////////////////////////////////////////////////////////////
// COpLoop
////////////////////////////////////////////////////////////////////////////////

#define OP_LOO_IN_TRIGG			0
#define OP_LOO_IN_STEP			1
#define OP_LOO_IN_CLEAR			2
#define OP_LOO_IN_SRC			3
#define OP_LOO_IN_REP			4
#define OP_LOO_OUT_NUM			0
#define OP_LOO_OUT_POS			1
#define OP_LOO_OUT_REPT			2
#define OP_LOO_OUT_REPM			3
#define OP_LOO_OUT_REP			4
#define OP_LOO_OUT_SYNC			5
#define OP_LOO_OUT_CYCLE		6
#define OP_LOO_OUT_DST			7
#define OP_LOO_OUT_TRIGG		8
#define OP_LOO_ROUTINE_EXPAND_STA	0
#define OP_LOO_ROUTINE_SHRINK_STA	1
#define OP_LOO_ROUTINE_EXPAND_VAL	2
#define OP_LOO_ROUTINE_SHRINK_VAL	3

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_LOO_IN_TRIGG_INFO		"trigger"
#define OP_LOO_IN_STEP_INFO		"step width"
#define OP_LOO_IN_CLEAR_INFO		"reset"
#define OP_LOO_IN_SRC_INFO		"value"
#define OP_LOO_IN_REP_INFO		"repeats"
#define OP_LOO_OUT_NUM_INFO		"number of stages in total"
#define OP_LOO_OUT_POS_INFO		"current stage"
#define OP_LOO_OUT_REPT_INFO		"number of repeats in total"
#define OP_LOO_OUT_REPM_INFO		"number of repeats of the current stage"
#define OP_LOO_OUT_REP_INFO		"current repeat"
#define OP_LOO_OUT_SYNC_INFO		"sync"
#define OP_LOO_OUT_CYCLE_INFO		"cycle"
#define OP_LOO_OUT_DST_INFO		"output"
#define OP_LOO_OUT_TRIGG_INFO		"trigger"
#define OP_LOO_ROUTINE_EXPAND_STA_INFO	"add stages"
#define OP_LOO_ROUTINE_SHRINK_STA_INFO	"remove stages"
#define OP_LOO_ROUTINE_EXPAND_VAL_INFO	"add value"
#define OP_LOO_ROUTINE_SHRINK_VAL_INFO	"remove value"
#define OP_LOO_CLASS_INFO \
\
"Loop\n" \
"- A new stage starts whenever the trigger value is greater than 0.0.\n" \
"- The step width defines the number of transitions per step.\n" \
"- Steps are always integer numbers.\n" \
"- A transition happens when the current stage is repeated, or when the\n" \
"  current stage changes.\n" \
"- Grouped value inlets including repeats inlet are called a stage.\n" \
"- There can be an arbitrary number of stages.\n" \
"- Next to the last stage, the first one follows, and vice versa. During such\n" \
"  a transition the cycle output is 1.0, otherwise 0.0.\n" \
"- All inlets are processed every cycle.\n" \
"- Unconnected value inlets are interpreted as 0.0.\n" \
"- Unconnected repeats inlets are interpreted as 1.0.\n" \
"- A stage will be skipped when its repeats input value is equal to 0.0.\n" \
"- Position and repeat outlets show the current indices.\n" \
"- When the reset input value is greater than 0.0, then all the position and\n" \
"  repeat variables will be reset to 0.0.\n" \
"- The sync output is 1.0 when the current position changes or directly\n" \
"  after a reset happened.\n" \
"- Trigger outputs are exactly then equal 1.0, when the corresponding\n" \
"  stage is reached, and before the related inlets will be processed."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_LOO_IN_TRIGG_INFO		"Auslöser"
#define OP_LOO_IN_STEP_INFO		"Schrittweite"
#define OP_LOO_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_LOO_IN_SRC_INFO		"Wert"
#define OP_LOO_IN_REP_INFO		"Wiederholung"
#define OP_LOO_OUT_NUM_INFO		"Anzahl Stufen insgesamt"
#define OP_LOO_OUT_POS_INFO		"Aktuelle Stufe"
#define OP_LOO_OUT_REPT_INFO		"Wiederholungen insgesamt"
#define OP_LOO_OUT_REPM_INFO		"Anzahl Wiederholungen der aktuellen Stufe"
#define OP_LOO_OUT_REP_INFO		"Aktuelle Wiederholung"
#define OP_LOO_OUT_SYNC_INFO		"Synchronisation"
#define OP_LOO_OUT_CYCLE_INFO		"Zyklus"
#define OP_LOO_OUT_DST_INFO		"Ausgang"
#define OP_LOO_OUT_TRIGG_INFO		"Auslöser"
#define OP_LOO_ROUTINE_EXPAND_STA_INFO	"Stufe hinzufügen"
#define OP_LOO_ROUTINE_SHRINK_STA_INFO	"Stufe entfernen"
#define OP_LOO_ROUTINE_EXPAND_VAL_INFO	"Wert hinzufügen"
#define OP_LOO_ROUTINE_SHRINK_VAL_INFO	"Wert entfernen"
#define OP_LOO_CLASS_INFO \
\
"Loop\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird ein Schritt gemacht.\n" \
"- Die Schrittweite bestimmt die Anzahl Übergänge pro Schritt.\n" \
"- Es sind nur ganze Schritte möglich.\n" \
"- Als Übergang wird entweder eine Wiederholung einer Stufe oder ein Wechsel\n" \
"  zwischen zwei aufeinander folgende Stufen bezeichnet.\n" \
"- Zusammengehörige Wert-Eingänge inklusive Wiederholungs-Eingang werden als Stufe\n" \
"  bezeichnet.\n" \
"- Es kann eine beliebige Anzahl Stufen definiert werden.\n" \
"- Nach der letzen Stufe folgt die erste und umgekehrt. Wird ein solcher Übergang\n" \
"  gemacht, steht der Zyklus-Ausgang auf 1.0, sonst auf 0.0.\n" \
"- Es werden immer alle Eingänge abgefragt.\n" \
"- Unbesetze Werte-Eingänge werden als 0.0 interpretiert.\n" \
"- Unbesetze Wiederholungs-Eingänge werden als 1.0 interpretiert.\n" \
"- Ist eine Wiederholungs-Wert gleich 0.0, wird diese Stufe übersprungen.\n" \
"- Positions- und Wiederholungs-Ausgang zeigen die aktuellen Indizies.\n" \
"- Ist der Wert am Zurücksetzen-Eingang grösser als 0.0, so wird Positions-\n" \
"  und Wiederholungs-Index auf 0.0 gesetzt.\n" \
"- Der Synchronisations-Ausgang steht genau dann auf 1.0, wenn die Position ge-\n" \
"  wechselt wurde oder wenn der Operator gerade zurückgesetzt wurde.\n" \
"- Ein Auslöser-Ausgang wird jeweils vor dem Aufruf der verbundenen Operatoren\n" \
"  der entsprechenen Stufe auf 1.0 gesetzt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpLoop::COpLoop()
: COp( 5, 9 )
, m_iPos( 0 )
, m_iRep( 0 )
, m_uiCountVal( 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "loop" );
	m_poIn[OP_LOO_IN_STEP].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_LOO_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_LOO_IN_TRIGG, "tr", OP_LOO_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_LOO_IN_STEP, "s", OP_LOO_IN_STEP_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_LOO_IN_CLEAR, "cl", OP_LOO_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_LOO_IN_SRC, "", OP_LOO_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_LOO_IN_REP, "", OP_LOO_IN_REP_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_LOO_OUT_NUM, "pt", OP_LOO_OUT_NUM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_LOO_OUT_POS, "p", OP_LOO_OUT_POS_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_LOO_OUT_REPT, "rt", OP_LOO_OUT_REPT_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_LOO_OUT_REPM, "rm", OP_LOO_OUT_REPM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_LOO_OUT_REP, "r", OP_LOO_OUT_REP_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_LOO_OUT_SYNC, "sy", OP_LOO_OUT_SYNC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_LOO_OUT_CYCLE, "cy", OP_LOO_OUT_CYCLE_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_LOO_OUT_DST, "", OP_LOO_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_LOO_OUT_TRIGG, "", OP_LOO_OUT_TRIGG_INFO );
	OP_SET_COUNT_ROUTINE( 4 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_LOO_ROUTINE_EXPAND_STA, "+s" OP_STR_ROUTINE, OP_LOO_ROUTINE_EXPAND_STA_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_LOO_ROUTINE_SHRINK_STA, "-s" OP_STR_ROUTINE, OP_LOO_ROUTINE_SHRINK_STA_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_LOO_ROUTINE_EXPAND_VAL, "+i" OP_STR_ROUTINE, OP_LOO_ROUTINE_EXPAND_VAL_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_LOO_ROUTINE_SHRINK_VAL, "-i" OP_STR_ROUTINE, OP_LOO_ROUTINE_SHRINK_VAL_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS | OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpLoop::~COpLoop() 
{}

void COpLoop::Init()
{
	OP_SET_COUNT_INTERNAL_INT( 2 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_iPos );
	OP_SET_PTR_INTERNAL_INT( 1, &m_iRep );
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiCountVal );
	COp::Init();
}

int COpLoop::GetRep( unsigned int iPos )
{
	const unsigned int r = iPos + m_uiCountVal;
	if( m_poIn[r].IsValid() )
		return (int)abs( (int)rint( m_poIn[r].Proc() ) );
	return 1;
}

inline unsigned int COpLoop::GetCountGroup() const {
	return ( m_uiCountIn - OP_LOO_IN_SRC ) / ( m_uiCountVal + 1 );
}

void COpLoop::GetStep( int *piStep, int *piInc )
{
	int iStep = 1;
	int iInc = +1;
	if( m_poIn[OP_LOO_IN_STEP].IsValid() )
	{
		iStep = (int)rint( m_poIn[OP_LOO_IN_STEP].Proc() );
		if( iStep < 0 )
		{
			iStep = -iStep;
			iInc = -1;
		}
	}
	*piStep = iStep;
	*piInc = iInc;
}

bool COpLoop::IncPos( int iInc, int *piPos )
{
	m_iPos += iInc;
	*piPos += ( m_uiCountVal + 1 ) * iInc;
	bool bCond0 = ( *piPos < OP_LOO_IN_SRC );
	bool bCond1 = ( *piPos >= (int)m_uiCountIn );
	if( bCond0 || bCond1 )
	{
		if( bCond0 )
		{
			*piPos = (int)m_uiCountIn - ( m_uiCountVal + 1 );
			m_iPos = (int)( ( m_uiCountIn - OP_LOO_IN_SRC ) / ( m_uiCountVal + 1 ) ) - 1;
		}
		else // bCond1
		{
			*piPos = OP_LOO_IN_SRC;
			m_iPos = 0;
		}
		return true;
	}
	return false;
}

/*
void COpLoop::UpdateRepTotal()
{
	int iRepTotal = 0;
	unsigned int r = OP_LOO_IN_SRC + m_uiCountVal;
	while( r < m_uiCountIn )
	{
		if( m_poIn[r].IsValid() )
			iRepTotal += (int)abs( (int)rint( m_poIn[r].Proc() ) );
		else
			++iRepTotal;
		r += ( m_uiCountVal + 1 );
	}
	m_pdOut[OP_LOO_OUT_REPT] = double( iRepTotal );
}
*/

void COpLoop::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const int iGroupCount = GetCountGroup();
	int iPosPrev = m_iPos;
	double dCycle = 0.0;
	bool bDoStep = true;
	if( m_poIn[OP_LOO_IN_CLEAR].IsValid() )
		if( m_poIn[OP_LOO_IN_CLEAR].Proc() > 0.0 )
		{
			Reset();

			int iStep, iInc;
			GetStep( &iStep, &iInc );
			int i = OP_LOO_IN_SRC;
			int iMax = iGroupCount;
			while( !GetRep( i ) && iMax )
			{
				--iMax;
				IncPos( iInc, &i );
			}
			bDoStep = false;
			iPosPrev = m_iPos + 1; // Nach Reset() immer SYNC auslösen.
		}
	
	int iPos = OP_LOO_IN_SRC + m_iPos * ( m_uiCountVal + 1 );
	
	m_pdOut[OP_LOO_OUT_POS] = (double)m_iPos;
	m_pdOut[OP_LOO_OUT_REP] = (double)m_iRep;
	
	// Alle Auslöser zurücksetzen.
	double *pdTr = m_pdOut + OP_LOO_OUT_TRIGG;
	double *pdEnd = m_pdOut + m_uiCountOut;
	while( pdTr < pdEnd )
		*pdTr++ = 0;
	
	const double dTrigg = m_poIn[OP_LOO_IN_TRIGG].Proc();
	
	// Wenn alle Rep-Eingänge gleich 0.0.
	if( m_pdOut[OP_LOO_OUT_REPT] == 0.0 )
	{
 		Reset();
	}
	else
	{
		if( bDoStep && dTrigg > 0.0 )
		{
			int iStep, iInc;
			GetStep( &iStep, &iInc );
			
			unsigned int iMax = iGroupCount;
			int iRep = GetRep( iPos );
			double *pdTrigg = m_pdOut + OP_LOO_OUT_DST + m_uiCountVal;
			while( iMax-- )
			{
				if( !iStep )
					break;
				
				if( ++m_iRep >= iRep )
				{
					m_iRep = 0;
					dCycle = ( IncPos( iInc, &iPos ) ? 1.0 : 0.0 );
					*( pdTrigg + m_iPos ) = 1.0;
				}
				iRep = GetRep( iPos );
				if( iRep )
					--iStep;
			}
		}
		if( iPosPrev != m_iPos )
		{
			m_pdOut[OP_LOO_OUT_REPM] = double( GetRep( iPos ) );
			m_pdOut[OP_LOO_OUT_SYNC] = 1.0;
		}
		else
		{
			m_pdOut[OP_LOO_OUT_SYNC] = 0.0;
		}
		m_pdOut[OP_LOO_OUT_CYCLE] = dCycle;
		
		CLink *poI = m_poIn + iPos;
		double *pdO = m_pdOut + OP_LOO_OUT_DST;
		const double * const pdOEnd = pdO + m_uiCountVal;
		while( pdO < pdOEnd )
		{
			double dOut = 0.0;
			if( poI->IsValid() )
			{
				dOut = poI->Proc();
			}
			*pdO = dOut;
			++poI;
			++pdO;
		}
	}
	CLink *poIn = m_poIn + OP_LOO_IN_SRC;
	int iRepTotal = 0;
	const CLink * const poIEnd = m_poIn + m_uiCountIn;
	while( poIn < poIEnd )
	{
		unsigned int c = m_uiCountVal;
		do
		{
			if( poIn->IsValid() )
				poIn->Proc();
			++poIn;
		}
		while( --c );
		
		if( poIn->IsValid() )
			iRepTotal += (int)abs( (int)rint( poIn->Proc() ) );
		else
			++iRepTotal;
		++poIn;
	}
	m_pdOut[OP_LOO_OUT_REPT] = double( iRepTotal );
}

void COpLoop::Update()
{
	COp::Update();
	
	m_pdOut[OP_LOO_OUT_NUM] = double( GetCountGroup() );
	
#ifdef OP_USE_RUNTIME_INFO
	unsigned int i = OP_LOO_IN_SRC;
	while( i < m_uiCountIn )
	{
		for( unsigned int j=0; j<m_uiCountVal; ++j )
		{
			m_poIn[i].SetFlags( OP_LINK_FLAG_OPTIONAL );
			SetNameInput( i, "i" );
			SetInfoInput( i++, OP_LOO_IN_SRC_INFO );
		}
		m_poIn[i].SetFlags( OP_LINK_FLAG_OPTIONAL );
		SetNameInput( i, "rp" );
		SetInfoInput( i++, OP_LOO_IN_REP_INFO );
	}
	i = OP_LOO_OUT_DST;
	while( i < OP_LOO_OUT_DST + m_uiCountVal )
	{
		SetNameOutput( i, "o" );
		SetInfoOutput( i++, OP_LOO_OUT_DST_INFO );
	}
	while( i < m_uiCountOut )
	{
		SetNameOutput( i, "tr" );
		SetInfoOutput( i++, OP_LOO_OUT_TRIGG_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
}

void COpLoop::Reset()
{
 	m_iPos = m_iRep = 0;
	const double dRepT = m_pdOut[OP_LOO_OUT_REPT];
	const double dNum = m_pdOut[OP_LOO_OUT_NUM];
	COp::Reset();
	m_pdOut[OP_LOO_OUT_REPT] = dRepT;
	m_pdOut[OP_LOO_OUT_NUM] = dNum;
}

#ifdef OP_USE_ROUTINES
void COpLoop::Routine( unsigned int uiIndex )
{	
	COp::Routine( uiIndex );
	
	switch( uiIndex )
	{
	case OP_LOO_ROUTINE_SHRINK_STA:
		if( m_uiCountIn > OP_LOO_IN_SRC + ( m_uiCountVal + 1 ) )
		{
			SetCountIn( m_uiCountIn - ( m_uiCountVal + 1 ) );
			SetCountOut( m_uiCountOut - 1 );
		}
	break;
	case OP_LOO_ROUTINE_EXPAND_STA:
		SetCountIn( m_uiCountIn + ( m_uiCountVal + 1 ) );
		SetCountOut( m_uiCountOut + 1 );
	break;
	case OP_LOO_ROUTINE_SHRINK_VAL:
	case OP_LOO_ROUTINE_EXPAND_VAL:
	{
		CLink *poInPrev = new CLink[ m_uiCountIn ];
		OP_MEM_COPY( poInPrev, m_poIn, m_uiCountIn * sizeof(CLink) );
		
		const unsigned int uiCountGroupInPrev = GetCountGroup();
		
		if( uiIndex == OP_LOO_ROUTINE_SHRINK_VAL )
		{
			if( m_uiCountVal <= 1 )
				break;
			--m_uiCountVal;
		}
		else
			++m_uiCountVal;
		
		SetCountOut( OP_LOO_OUT_DST + m_uiCountVal + uiCountGroupInPrev );
		SetCountIn( OP_LOO_IN_SRC + uiCountGroupInPrev * ( m_uiCountVal + 1 ) );
		
		const CLink *poDstMax = m_poIn + m_uiCountIn;
		if( uiIndex == OP_LOO_ROUTINE_SHRINK_VAL )
		{
			CLink *poDst = m_poIn + OP_LOO_IN_SRC;
			CLink *poSrc = poInPrev + OP_LOO_IN_SRC;
			while( poDst < poDstMax )
			{
				for( unsigned int k=0; k<m_uiCountVal; k++ )
					*poDst++ = *poSrc++;
				++poSrc;
				*poDst++ = *poSrc++; // IN_REP
			}

		}
		else // OP_LOO_ROUTINE_EXPAND_VAL
		{
			CLink *poDst = m_poIn + OP_LOO_IN_SRC;
			CLink *poSrc = poInPrev + OP_LOO_IN_SRC;
			while( poDst < poDstMax )
			{
				for( unsigned int k=0; k<m_uiCountVal-1; k++ )
					*poDst++ = *poSrc++;
				poDst->Reset();
				++poDst;
				*poDst++ = *poSrc++; // IN_REP
			}
		}
		
		OP_DELETE_ARRAY( poInPrev );
	}
	break;
	}
	Update();
	Reset();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpLoop, OP_LOO_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpLoop )
OP_GENERIC_METHODS_DEF( COpLoop )

#endif // OP_INC_OP_LOOP
