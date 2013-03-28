// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSH.cpp

#include "OpSH.h"
#ifdef OP_INC_OP_SH

////////////////////////////////////////////////////////////////////////////////
// COpSH
////////////////////////////////////////////////////////////////////////////////

#define OP_SAH_IN_SRC		0
#define OP_SAH_IN_PERIOD	1
#define OP_SAH_IN_RESET		2
#define OP_SAH_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_SAH_IN_SRC_INFO	"input"
#define OP_SAH_IN_PERIOD_INFO	"period"
#define OP_SAH_IN_RESET_INFO	"reset"
#define OP_SAH_OUT_DST_INFO	"output"
#define OP_SAH_CLASS_INFO \
\
"Sample and Hold\n" \
"- The output value is hold steady for the specified period of time,\n" \
"  measured in samples.\n" \
"- All inlets are processed every cycle.\n" \
"- When the reset input value is greater than 0.0, all values will be\n" \
"  reinitialized and a new period begins immediately.\n" \
"- When the period input value is not an integer number, then the\n" \
"  actual hold duration will be periodically extended by one sample."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_SAH_IN_SRC_INFO	"Eingang"
#define OP_SAH_IN_PERIOD_INFO	"Dauer"
#define OP_SAH_IN_RESET_INFO	"Reset"
#define OP_SAH_OUT_DST_INFO	"Ausgang"
#define OP_SAH_CLASS_INFO \
\
"Sample-And-Hold\n" \
"- Das Eingangssignal wird nach einer am Dauer-Eingang angegebenen Anzahl\n" \
"  Samples d fixiert und danach während d Samples konstant ausgegeben.\n" \
"- Alle Eingänge werden ständig abgefragt.\n" \
"- Wenn der Wert am Reset-Eingang grösser 0.0 ist, werden die Werte sofort\n" \
"  neu bestimmt und eine neue Periode beginnt.\n" \
"- Bei nicht ganzzahligen Dauer-Werten, wird eine Periode zeitweise um ein\n" \
"  Sample verlängert."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSH::COpSH()
: COp( 3, 1 )
, m_iPeriod( 0 )
, m_dPeriodFrc( 0.0 )
, m_dPeriodLast( 0.0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "s&h" );
	m_poIn[OP_SAH_IN_RESET].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_SAH_IN_SRC, "i", OP_SAH_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SAH_IN_PERIOD, "p", OP_SAH_IN_PERIOD_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SAH_IN_RESET, "r", OP_SAH_IN_RESET_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SAH_OUT_DST, "o", OP_SAH_OUT_DST_INFO );
	Reset();
	Validate();
}

COpSH::~COpSH()
{}

void COpSH::Init()
{
	OP_SET_COUNT_INTERNAL_INT( 2 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_iPeriod );
	OP_SET_PTR_INTERNAL_INT( 1, &m_iCounter );
	OP_SET_COUNT_INTERNAL_FLT( 4 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dPeriodFrc );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_dCounterFrc );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_dSample );
	OP_SET_PTR_INTERNAL_FLT( 3, &m_dPeriodLast );
	COp::Init();
}

void COpSH::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dValue = m_poIn[OP_SAH_IN_SRC].Proc();
	
	if( m_poIn[OP_SAH_IN_RESET].IsValid() )
		if( m_poIn[OP_SAH_IN_RESET].Proc() > 0.0 )
			Reset();
	
	--m_iCounter;
	
	const double dPeriod = m_poIn[OP_SAH_IN_PERIOD].Proc();
	if( dPeriod != m_dPeriodLast )
	{
		m_dPeriodLast = dPeriod;
		double dInt;
		m_dPeriodFrc = modf( fabs( dPeriod ), &dInt );
		m_iPeriod = (unsigned int)dInt;
		m_dSample = dValue;
		Reset();
	}
	else if( m_iCounter <= 0 )
	{
		m_iCounter = m_iPeriod;
		if( m_dCounterFrc >= 1.0 )
		{
			m_dCounterFrc = 0.0;
			++m_iCounter;
		}
		m_dCounterFrc += m_dPeriodFrc;
		m_dSample = dValue;
	}
	
	m_pdOut[OP_SAH_OUT_DST] = m_dSample;
}

void COpSH::Reset()
{
	COp::Reset();
	m_iCounter = m_iPeriod;
	m_dCounterFrc = 0.0;
	m_dSample = 0.0;
}

OP_SET_NAME_AND_INFO_CLASS( COpSH, OP_SAH_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSH )
OP_GENERIC_METHODS_DEF( COpSH )

#endif // OP_INC_OP_SH
