// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpStep.cpp

#include "OpStep.h"
#ifdef OP_INC_OP_STEP

////////////////////////////////////////////////////////////////////////////////
// COpStep
////////////////////////////////////////////////////////////////////////////////

#define OP_STP_IN_DLY			0
#define OP_STP_IN_V0			1
#define OP_STP_IN_V1			2
#define OP_STP_IN_CLEAR			3
#define OP_STP_OUT_DST			0
#define OP_STP_OUT_SYNC			1
#define OP_STP_OUT_CYCLE		2
#define OP_STP_OUT_DUR			3

#if defined( OP_LANGUAGE_ENGLISH )
#define OP_STP_IN_DLY_INFO		"duration"
#define OP_STP_IN_SRC_START_INFO	"start"
#define OP_STP_IN_SRC_END_INFO		"end"
#define OP_STP_IN_SRC_CLEAR_INFO	"reset"
#define OP_STP_OUT_DST_INFO		"current stage"
#define OP_STP_OUT_SYNC_INFO		"sync"
#define OP_STP_OUT_CYCLE_INFO		"cycle sync"
#define OP_STP_OUT_DUR_INFO		"current duration"
#define OP_STP_CLASS_INFO \
\
"Step\n" \
"- The duration input value defines the length of one stage in samples.\n" \
"- The current stage output value increases incrementally and is wrapped\n" \
"  to [start, end).\n" \
"- The start input value is 0.0 by default.\n" \
"- The end input value is 1.0 by default.\n" \
"- The duration inlet is processed every cycle, although it only influences\n" \
"  the operator when the current stage changes.\n" \
"- When the current stage changes, then the sync output value is 1.0,\n" \
"  otherwise always 0.0.\n" \
"- When all stages have been completed, the cycle sync output value will\n" \
"  be 1.0, otherwise always 0.0. The next stage is the start stage.\n" \
"- When the duration value is not an integer number, two different step\n" \
"  operators will still never get out of sync! The actual stage\n" \
"  duration is periodically corrected.\n" \
"- When the reset input value is greater than 0.0, then start and end\n" \
"  values will be reinitialized and the current stage will be reset\n" \
"  to the given start value."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_STP_IN_DLY_INFO		"Dauer"
#define OP_STP_IN_SRC_START_INFO	"Startwert"
#define OP_STP_IN_SRC_END_INFO		"Endwert"
#define OP_STP_IN_SRC_CLEAR_INFO	"Zurücksetzen"
#define OP_STP_OUT_DST_INFO		"Stufe"
#define OP_STP_OUT_SYNC_INFO		"Synchronisation"
#define OP_STP_OUT_CYCLE_INFO		"Zyklus"
#define OP_STP_OUT_DUR_INFO		"Aktuelle Dauer"
#define OP_STP_CLASS_INFO \
\
"Stepper\n" \
"- Die Dauer gibt die Anzahl Samples vor, die zwischen jedem Stufenwechsel-\n" \
"  vergehen.\n" \
"- Bei Stufenwechsel, wird die Stufe immer um eins erhöt.\n" \
"- Der Wert am Stufenausgang ist Element von [Startwert,Endwert),\n" \
"  und verhält sich zyklisch.\n" \
"- Der Startwert ist standardmässig gleich 0.0.\n" \
"- Der Endwert ist standardmässig gleich 1.0.\n" \
"- Der Dauer-Eingang wird ständig abgefragt, hat aber nur bei Stufenwechsel.\n" \
"  Einfluss.\n" \
"- Bei Stufenwechsel oder beim Zurücksetzen steht der Synchronisations-Ausgang\n" \
"  auf 1.0, sonst auf 0.0.\n" \
"- Beim Antritt eines neuen Durchgangs steht der Zyklus-Ausgang auf 1.0,\n" \
"  sonst auf 0.0.\n" \
"- Auch bei nicht ganzzahligen Dauer-Werten sollten zwei Stepper nicht aus-\n" \
"  einander laufen. Dabei wird die Dauer zeitweise um ein Sample verlängert.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so werden Start- und Endwert\n" \
"  neu bestimmt und die aktuelle Stufe auf den Startwert zurückgesetzt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpStep::COpStep()
: COp( 4, 4 )
, m_uiAccum( 1 )
, m_uiDly( 0 )
, m_iStp( 0 )
, m_iInc( 0 )
, m_dAccumFrc( 0.0 )
, m_dDlyFrc( 0.0 )
, m_dDlyLast( 0.0 )
, m_bUpdateDelay( false )
{
	Init();
	OP_SET_NAME_INSTANCE( "stp" );
	m_poIn[OP_STP_IN_V0].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_STP_IN_V1].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_STP_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_STP_IN_DLY, "i", OP_STP_IN_DLY_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_STP_IN_V0, "v0", OP_STP_IN_SRC_START_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_STP_IN_V1, "v1", OP_STP_IN_SRC_END_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_STP_IN_CLEAR, "cl", OP_STP_IN_SRC_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_STP_OUT_DST, "o", OP_STP_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_STP_OUT_SYNC, "sy", OP_STP_OUT_SYNC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_STP_OUT_CYCLE, "cy", OP_STP_OUT_CYCLE_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_STP_OUT_DUR, "d", OP_STP_OUT_DUR_INFO );
	Update();
	Validate();
	//Reset();
}

COpStep::~COpStep()
{}

void COpStep::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 2 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiAccum );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_uiDly );
	OP_SET_COUNT_INTERNAL_INT( 2 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_iStp );
	OP_SET_PTR_INTERNAL_INT( 1, &m_iInc );
	OP_SET_COUNT_INTERNAL_FLT( 3 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dAccumFrc );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_dDlyLast );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_dDlyFrc );
	COp::Init();
}

void COpStep::GetV0V1( int *piV0, int *piV1 )
{
	*piV0 = ( m_poIn[OP_STP_IN_V0].IsValid() )
		? ( (int)rint( m_poIn[OP_STP_IN_V0].Proc() ) )
		: ( 0 );
	*piV1 = ( m_poIn[OP_STP_IN_V1].IsValid() )
		? ( (int)rint( m_poIn[OP_STP_IN_V1].Proc() ) )
		: ( 1 );
}

double COpStep::UpdateDelay()
{
	double dDly = m_poIn[OP_STP_IN_DLY].Proc();
	if( dDly != m_dDlyLast )
	{
		m_dDlyLast = dDly;
		if( dDly < 0.0 )
			dDly = -dDly;
		m_uiDly = (unsigned int)( dDly );
		m_dAccumFrc = m_dDlyFrc = dDly - double(m_uiDly);
	}
	return dDly;
}

void COpStep::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	m_pdOut[OP_STP_OUT_CYCLE] = m_pdOut[OP_STP_OUT_SYNC] = 0.0;
	
	
	if( m_poIn[OP_STP_IN_CLEAR].IsValid() )
		if( m_poIn[OP_STP_IN_CLEAR].Proc() > 0.0 )
		{
			Reset();
			Update(); // Setzt m_bUpdateDelay.
		}
	
	if( m_bUpdateDelay )
	{
		m_bUpdateDelay = false;
		m_pdOut[OP_STP_OUT_SYNC] = 1.0;
		m_pdOut[OP_STP_OUT_DUR] = UpdateDelay();
	}

	if( m_uiAccum > m_uiDly )
	{
		int iV0, iV1;
		GetV0V1( &iV0, &iV1 );
		const bool bInv = ( iV0 > iV1 );
		m_iInc = ( bInv ? -1 : +1 );
		
		m_uiAccum = 1;
		
		m_dAccumFrc += m_dDlyFrc;
		if( m_dAccumFrc >= 1.0 )
		{
			m_uiAccum = 0; // Im nächsten Zyklus ein Sample Korrektur!
			m_dAccumFrc = m_dAccumFrc - 1.0;
		}
		
		m_iStp += m_iInc;
		m_pdOut[OP_STP_OUT_DST] = (double)m_iStp;
		
		if( ( iV0 <= iV1 && m_iStp >= iV1 ) || ( bInv && m_iStp <= iV1 ) )
		{
			m_iStp = iV0;
			m_pdOut[OP_STP_OUT_CYCLE] = 1.0;
			m_pdOut[OP_STP_OUT_DST] = (double)m_iStp;
		}
		
		m_pdOut[OP_STP_OUT_SYNC] = 1.0;
		m_pdOut[OP_STP_OUT_DUR] = UpdateDelay(); // Abhängig von OP_STP_OUT_SYNC.
	}
	else
	{
		m_poIn[OP_STP_IN_DLY].Proc();
	}
	++m_uiAccum;
}

void COpStep::Update()
{
	if( !m_bIsNotValid )
	{
		int iV0, iV1;
		GetV0V1( &iV0, &iV1 );
		m_iInc = ( iV0 <= iV1 ? +1 : -1 );
		m_iStp = iV0;
	}
	m_pdOut[OP_STP_OUT_DST] = (double)m_iStp;
	m_bUpdateDelay = true;
	COp::Update();
}

void COpStep::Reset()
{
	m_uiAccum = 1;
	m_uiDly = m_iStp = 0;
	m_iInc = 1;
	m_dAccumFrc = m_dDlyFrc = m_dDlyLast = 0.0;
	m_bUpdateDelay = false;
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpStep, OP_STP_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpStep )
OP_GENERIC_METHODS_DEF( COpStep )

#endif // OP_INC_OP_STEP
