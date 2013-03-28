// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSelector.cpp

#include "OpSelector.h"
#ifdef OP_INC_OP_SELECTOR

////////////////////////////////////////////////////////////////////////////////
// COpSelector
////////////////////////////////////////////////////////////////////////////////

#define OP_SEL_IN_POS			0
#define OP_SEL_IN_SRC			1
#define OP_SEL_OUT_NUM			0
#define OP_SEL_OUT_VAL			1
#define OP_SEL_ROUTINE_EXPAND_OUT	0
#define OP_SEL_ROUTINE_SHRINK_OUT	1
#define OP_SEL_ROUTINE_EXPAND_IN	2
#define OP_SEL_ROUTINE_SHRINK_IN	3

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_SEL_IN_POS_INFO		"position"
#define OP_SEL_IN_SRC_INFO		"input"
#define OP_SEL_OUT_NUM_INFO		"number of inputs"
#define OP_SEL_OUT_VAL_INFO		"output"
#define OP_SEL_ROUTINE_EXPAND_OUT_INFO	"add output"
#define OP_SEL_ROUTINE_SHRINK_OUT_INFO	"remove output"
#define OP_SEL_ROUTINE_EXPAND_IN_INFO	"add input"
#define OP_SEL_ROUTINE_SHRINK_IN_INFO	"remove input"
#define OP_SEL_CLASS_INFO \
\
"Selector\n" \
"- The position parameter selects the input values which are written to\n" \
"  the output.\n" \
"- The position value is wrapped automatically.\n" \
"- When the position value is not an integer number, the two nearest\n" \
"  input values are linearly interpolated.\n" \
"- The sync output value is 1.0 whenever the current position changes,\n" \
"  otherwise always 0.0.\n" \
"- Unconnected inlets are set to 0.0 by default.\n" \
"- All inlets are processed every cycle."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_SEL_IN_POS_INFO		"Position"
#define OP_SEL_IN_SRC_INFO		"Eingang"
#define OP_SEL_OUT_NUM_INFO		"Anzahl Eingänge"
#define OP_SEL_OUT_VAL_INFO		"Ausgang"
#define OP_SEL_ROUTINE_EXPAND_OUT_INFO	"Ausgang hinzufügen"
#define OP_SEL_ROUTINE_SHRINK_OUT_INFO	"Ausgang entfernen"
#define OP_SEL_ROUTINE_EXPAND_IN_INFO	"Eingang hinzufügen"
#define OP_SEL_ROUTINE_SHRINK_IN_INFO	"Eingang entfernen"
#define OP_SEL_CLASS_INFO \
\
"Selektor\n" \
"- Die Position bestimmt, welche Eingangswerte aktuell zum Ausgang geführt werden.\n" \
"- Der Positions-Wert verhält sich zyklisch.\n" \
"- Ist die Position nicht ganzzahlig wird linear interpoliert.\n" \
"- Der Synchronisationsausgang steht auf 1.0, wenn eine neue Position erreicht\n" \
"  wird, andernfalls auf 0.0.\n" \
"- Unbesetze Eingänge werden als 0.0 interpretiert.\n" \
"- Es werden immer alle Eingänge abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSelector::COpSelector()
: COp( 2, 2 )
, m_uiCountPos( 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "select" );
	m_poIn[OP_SEL_IN_SRC].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_SEL_IN_POS, "", OP_SEL_IN_POS_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SEL_IN_SRC, "", OP_SEL_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SEL_OUT_NUM, "n", OP_SEL_OUT_NUM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SEL_OUT_VAL, "", OP_SEL_OUT_VAL_INFO );
	OP_SET_COUNT_ROUTINE( 4 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SEL_ROUTINE_EXPAND_OUT, "+o" OP_STR_ROUTINE, OP_SEL_ROUTINE_EXPAND_OUT_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SEL_ROUTINE_SHRINK_OUT, "-o" OP_STR_ROUTINE, OP_SEL_ROUTINE_SHRINK_OUT_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SEL_ROUTINE_EXPAND_IN, "+i" OP_STR_ROUTINE, OP_SEL_ROUTINE_EXPAND_IN_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SEL_ROUTINE_SHRINK_IN, "-i" OP_STR_ROUTINE, OP_SEL_ROUTINE_SHRINK_IN_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS | OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpSelector::~COpSelector() 
{}

void COpSelector::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiCountPos );
	COp::Init();
}

void COpSelector::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const int iCountIn = int( m_uiCountIn - m_uiCountPos );

	unsigned int i = 0; // OP_SEL_IN_POS
	while( i < m_uiCountPos )
	{
		const double dPos = m_poIn[i].Proc();
		double dPosInt;
		const double dPosFrc = modf( dPos, &dPosInt );
		int iPos = int( dPosInt ) % iCountIn;
		if( iPos < 0 )
			iPos += iCountIn;
		iPos += (int)m_uiCountPos;
		
		double dVal = 0.0;
		if( m_poIn[iPos].IsValid() )
			dVal = m_poIn[iPos].Proc();
		if( dPosFrc )
		{
			if( dPosFrc > 0.0 )
			{
				if( ++iPos == (int)m_uiCountIn )
					iPos = (int)m_uiCountPos;
				if( m_poIn[iPos].IsValid() )
				{
					const double dVal1 = m_poIn[iPos].Proc();
					dVal = dVal + dPosFrc * ( dVal1 - dVal );
				}
			}
			else
			{
				if( --iPos == 0 )
					iPos = (int)m_uiCountIn - 1;
				if( m_poIn[iPos].IsValid() )
				{
					const double dVal1 = m_poIn[iPos].Proc();
					dVal = dVal1 + dPosFrc * ( dVal1 - dVal );
				}
			}
		}
		m_pdOut[ i + OP_SEL_OUT_VAL ] = dVal;
		++i;
	}
	
	CLink *poI = m_poIn + m_uiCountPos;
	const CLink * const poIEnd = m_poIn + m_uiCountIn;
	while( poI < poIEnd )
	{
		if( poI->IsValid() )
			poI->Proc();
		++poI;
	}
}

void COpSelector::Update()
{
	COp::Update();
	
	m_pdOut[OP_SEL_OUT_NUM] = double( m_uiCountIn - m_uiCountPos );
	
#ifdef OP_USE_RUNTIME_INFO
	unsigned int i = OP_SEL_IN_POS;
	while( i < m_uiCountPos )
	{
		SetNameInput( i, "ps" );
		SetInfoInput( i, OP_SEL_IN_POS_INFO );
		++i;
	}
	while( i < m_uiCountIn )
	{
		SetNameInput( i, "i" );
		SetInfoInput( i, OP_SEL_IN_SRC_INFO );
		++i;
	}
	for( unsigned int o=OP_SEL_OUT_VAL; o<m_uiCountOut; ++o )
	{
		SetNameOutput( o, "o" );
		SetInfoOutput( o, OP_SEL_OUT_VAL_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
}

#ifdef OP_USE_ROUTINES
void COpSelector::Routine( unsigned int uiIndex )
{	
	COp::Routine( uiIndex );
	
	switch( uiIndex )
	{
	case OP_SEL_ROUTINE_SHRINK_OUT:
	{
		if( m_uiCountPos <= 1 )
			break;
		--m_uiCountPos;
		unsigned int i = m_uiCountPos;
		do
			m_poIn[i] = m_poIn[i+1];
		while( ++i < m_uiCountIn - 1 );
		SetCountOut( m_uiCountOut - 1 );
		SetCountIn( m_uiCountIn - 1 );
	}
	break;
	case OP_SEL_ROUTINE_EXPAND_OUT:
	{
		SetCountOut( m_uiCountOut + 1 );
		SetCountIn( m_uiCountIn + 1 );
		m_poIn[m_uiCountIn - 1].SetFlags( OP_LINK_FLAG_OPTIONAL );
		unsigned int i = m_uiCountIn;
		while( --i >= m_uiCountPos )
			m_poIn[i] = m_poIn[i-1];
		++m_uiCountPos;
		m_poIn[m_uiCountPos-1].Clear();
	}
	break;
	case OP_SEL_ROUTINE_SHRINK_IN:
		if( m_uiCountIn > m_uiCountPos + 1 )
			SetCountIn( m_uiCountIn - 1 );
	break;
	case OP_SEL_ROUTINE_EXPAND_IN:
		SetCountIn( m_uiCountIn + 1 );
		m_poIn[m_uiCountIn - 1].SetFlags( OP_LINK_FLAG_OPTIONAL );
	break;
	}
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpSelector, OP_SEL_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSelector )
OP_GENERIC_METHODS_DEF( COpSelector )

#endif // OP_INC_OP_SELECTOR
