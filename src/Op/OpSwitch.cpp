// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSwitch.cpp

#include "OpSwitch.h"
#ifdef OP_INC_OP_SWITCH


////////////////////////////////////////////////////////////////////////////////
// COpSwitch::SVoice
////////////////////////////////////////////////////////////////////////////////

COpSwitch::SVoice::SVoice()
: m_uiStamp( 0 )
, m_iIndex( -1 )
{}

void COpSwitch::SVoice::Reset( unsigned int uiStamp )
{
	m_uiStamp = uiStamp;
	m_iIndex = -1;
}


////////////////////////////////////////////////////////////////////////////////
// COpSwitch
////////////////////////////////////////////////////////////////////////////////

#define OP_SWT_IN_TRIGG			0
#define OP_SWT_IN_SRC			1
#define OP_SWT_OUT_NUM			0
#define OP_SWT_OUT_TRIGG		1
#define OP_SWT_OUT_DST			2

#define OP_SWT_ROUTINE_EXPAND_IN	0
#define OP_SWT_ROUTINE_SHRINK_IN	1
#define OP_SWT_ROUTINE_EXPAND_OUT	2
#define OP_SWT_ROUTINE_SHRINK_OUT	3
#define OP_SWT_ROUTINE_EXPAND_VAL	4
#define OP_SWT_ROUTINE_SHRINK_VAL	5

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_SWT_IN_TRIGG_INFO		"trigger"
#define OP_SWT_IN_SRC_INFO		"value"
#define OP_SWT_OUT_NUM_INFO		"number of outputs"
#define OP_SWT_OUT_TRIGG_INFO		"output"
#define OP_SWT_OUT_DST_INFO		"value"  
#define OP_SWT_ROUTINE_EXPAND_OUT_INFO	"add output"
#define OP_SWT_ROUTINE_SHRINK_OUT_INFO	"remove output"
#define OP_SWT_ROUTINE_EXPAND_IN_INFO	"add input"
#define OP_SWT_ROUTINE_SHRINK_IN_INFO	"remove input"
#define OP_SWT_ROUTINE_EXPAND_VAL_INFO	"add value"
#define OP_SWT_ROUTINE_SHRINK_VAL_INFO	"remove value"
#define OP_SWT_CLASS_INFO \
\
"Switch\n" \
"- A group is defined by an arbitrary number of values and an trigger.\n" \
"- An arbitrary number of input groups is mapped onto an arbitrary\n" \
"  number of output groups.\n" \
"- When a trigger input value of an input group is greater than 0.0,\n" \
"  the oldest output group will be associated with it. All associated\n" \
"  values will be updated and remain constant on the outlets.\n" \
"- All inlets are processed every cycle."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_SWT_IN_TRIGG_INFO		"Auslöser"
#define OP_SWT_IN_SRC_INFO		"Wert"
#define OP_SWT_OUT_NUM_INFO		"Anzahl Ausgänge"
#define OP_SWT_OUT_TRIGG_INFO		"Auslöser"
#define OP_SWT_OUT_DST_INFO		"Wert"  
#define OP_SWT_ROUTINE_EXPAND_OUT_INFO	"Ausgang hinzufügen"
#define OP_SWT_ROUTINE_SHRINK_OUT_INFO	"Ausgang entfernen"
#define OP_SWT_ROUTINE_EXPAND_IN_INFO	"Eingang hinzufügen"
#define OP_SWT_ROUTINE_SHRINK_IN_INFO	"Eingang entfernen"
#define OP_SWT_ROUTINE_EXPAND_VAL_INFO	"Wert hinzufügen"
#define OP_SWT_ROUTINE_SHRINK_VAL_INFO	"Wert entfernen"
#define OP_SWT_CLASS_INFO \
\
"Switch\n" \
"- Eine Gruppe definiert sich aus einer frei wählbaren Anzahl Werte und\n" \
"  einem zugehörigen Auslöser.\n" \
"- Eine beliebige Anzahl Gruppen auf Eingangsseite wird auf eine\n" \
"  eine beliebige Anzahl Gruppen auf Ausgangsseite abgebildet.\n" \
"- Steht ein Auslöser-Eingang auf einem Wert grösser als 0.0,\n" \
"  so wird die älteste Ausgabe-Gruppe dieser Eingabe-Gruppe zugewiesen.\n" \
"  Dann werden alle zugehörigen Werte erneuert und bleiben danach an\n" \
"  den Ausgängen konstant gehalten.\n" \
"- Alle Eingänge werden immer abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSwitch::COpSwitch()
: COp( 2, 3 )
, m_uiCountVal( 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "switch" );
	OP_SET_NAME_AND_INFO_INPUT( OP_SWT_IN_TRIGG, "", OP_SWT_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SWT_IN_SRC, "", OP_SWT_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SWT_OUT_NUM, "n", OP_SWT_OUT_NUM_INFO ); 
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SWT_OUT_TRIGG, "", OP_SWT_OUT_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SWT_OUT_DST, "", OP_SWT_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 6 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SWT_ROUTINE_EXPAND_IN, "+i" OP_STR_ROUTINE, OP_SWT_ROUTINE_EXPAND_IN_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SWT_ROUTINE_SHRINK_IN, "-i" OP_STR_ROUTINE, OP_SWT_ROUTINE_SHRINK_IN_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SWT_ROUTINE_EXPAND_OUT, "+o" OP_STR_ROUTINE, OP_SWT_ROUTINE_EXPAND_OUT_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SWT_ROUTINE_SHRINK_OUT, "-o" OP_STR_ROUTINE, OP_SWT_ROUTINE_SHRINK_OUT_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SWT_ROUTINE_EXPAND_VAL, "+v" OP_STR_ROUTINE, OP_SWT_ROUTINE_EXPAND_VAL_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SWT_ROUTINE_SHRINK_VAL, "-v" OP_STR_ROUTINE, OP_SWT_ROUTINE_SHRINK_VAL_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS | OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpSwitch::~COpSwitch() 
{}

void COpSwitch::Init()
{
	Reset();
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiCountVal );
	COp::Init();
}

unsigned int COpSwitch::GetVoice()
{
	int iIndex = 0;
	unsigned int s = m_oArrVoice[0].m_uiStamp;
	unsigned int i = 1;
	while( i < m_oArrVoice.GetSize() )
	{
		const unsigned int sn = m_oArrVoice[i].m_uiStamp;
		if( sn < s )
		{
			s = sn;
			iIndex = i;
		}
		++i;
	}	
	return (unsigned int)iIndex;
}

void COpSwitch::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	// Alle Ausgabe-Auslöser auf 0.0 setzen.
	unsigned int o = OP_SWT_OUT_TRIGG;
	while( o < m_uiCountOut )
	{
		m_pdOut[o++] = 0.0; // Trigger.
		o += m_uiCountVal; // Achtung: Neu!
	}
	
	unsigned int i = OP_SWT_IN_TRIGG;
	while( i < m_uiCountIn )
	{
		const bool bTrigg = bool( m_poIn[i + OP_SWT_IN_TRIGG].Proc() > 0.0 );
		if( bTrigg )
		{
			const unsigned int v = GetVoice();
			m_oArrVoice[v].m_uiStamp = m_uiState;
			m_oArrVoice[v].m_iIndex = (int)i;
			unsigned int j = OP_SWT_OUT_TRIGG + v * ( m_uiCountVal + 1 );
			m_pdOut[j++] = 1.0; // Trigger.
			for( unsigned int k=0; k<m_uiCountVal; ++k )
				m_pdOut[j++] = m_poIn[i + OP_SWT_IN_SRC + k].Proc();
		}
		else 
		{			
			for( unsigned int k=0; k<m_uiCountVal; ++k )
				m_poIn[i + OP_SWT_IN_SRC + k].Proc();
		}
		i += ( m_uiCountVal + 1 );
	}
}

void COpSwitch::Update()
{
	COp::Update();
	
	const unsigned int uiCountVoice = ( ( m_uiCountOut - OP_SWT_OUT_TRIGG ) / ( m_uiCountVal + 1 ) );
	if( m_oArrVoice.GetSize() != uiCountVoice )
		m_oArrVoice.Resize( uiCountVoice );
	
	m_pdOut[OP_SWT_OUT_NUM] = (double)uiCountVoice;

#ifdef OP_USE_RUNTIME_INFO
	unsigned int i = OP_SWT_IN_TRIGG;
	while( i < m_uiCountIn )
	{
		SetNameInput( i, "t" );
		SetInfoInput( i++, OP_SWT_IN_TRIGG_INFO );
		for( unsigned int k=0; k<m_uiCountVal; ++k )
		{
			SetNameInput( i, "v" );
			SetInfoInput( i++, OP_SWT_IN_SRC_INFO );
		}
	}
	unsigned int o = OP_SWT_OUT_TRIGG;
	while( o < m_uiCountOut )
	{
		SetNameOutput( o, "t" );
		SetInfoOutput( o++, OP_SWT_OUT_TRIGG_INFO );
		for( unsigned int k=0; k<m_uiCountVal; ++k )
		{
			SetNameOutput( o, "v" );
			SetInfoOutput( o++, OP_SWT_OUT_DST_INFO );
		}
	}
#endif // OP_USE_RUNTIME_INFO
}

void COpSwitch::Reset()
{
	for( unsigned int i=0; i<m_oArrVoice.GetSize(); ++i )
		m_oArrVoice[i].Reset( m_uiState );
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpSwitch::Routine( unsigned int uiIndex )
{	
	switch( uiIndex )
	{
	case OP_SWT_ROUTINE_SHRINK_IN:
		if( m_uiCountIn > ( OP_SWT_IN_TRIGG + m_uiCountVal + 1 ) )
			SetCountIn( m_uiCountIn - m_uiCountVal - 1 );
	break;
	case OP_SWT_ROUTINE_EXPAND_IN:
		SetCountIn( m_uiCountIn + m_uiCountVal + 1 );
	break;
	case OP_SWT_ROUTINE_SHRINK_OUT:
		if( m_uiCountOut > ( OP_SWT_OUT_TRIGG + m_uiCountVal + 1 ) )
			SetCountOut( m_uiCountOut - m_uiCountVal - 1 );
	break;
	case OP_SWT_ROUTINE_EXPAND_OUT:
		SetCountOut( m_uiCountOut + m_uiCountVal + 1 );
	break;
	case OP_SWT_ROUTINE_SHRINK_VAL:
	case OP_SWT_ROUTINE_EXPAND_VAL:
	{
		CLink *poInPrev = new CLink[ m_uiCountIn ];
		OP_MEM_COPY( poInPrev, m_poIn, m_uiCountIn * sizeof(CLink) );
		
		const unsigned int uiCountVoiceInPrev = 
			( m_uiCountIn - OP_SWT_IN_TRIGG ) / ( m_uiCountVal + 1 );
		
		if( uiIndex == OP_SWT_ROUTINE_SHRINK_VAL )
		{
			if( m_uiCountVal <= 1 )
				break;
			--m_uiCountVal;
		}
		else
			++m_uiCountVal;
		
		const unsigned uiCountVoice = m_oArrVoice.GetSize();
		SetCountOut( OP_SWT_OUT_TRIGG + uiCountVoice * ( m_uiCountVal + 1 ) );
		SetCountIn( OP_SWT_IN_TRIGG + uiCountVoiceInPrev * ( m_uiCountVal + 1 ) );
		
		CLink *poDst = m_poIn, *poSrc = poInPrev;
		if( uiIndex == OP_SWT_ROUTINE_SHRINK_VAL )
		{
			const CLink *poDstMax = m_poIn + m_uiCountIn;
			while( poDst < poDstMax )
			{
				*poDst++ = *poSrc++; // OUT_TRIGG
				for( unsigned int k=0; k<m_uiCountVal; k++ )
					*poDst++ = *poSrc++;
				++poSrc;
			}
		}
		else
		{
			const CLink *poDstMax = m_poIn + m_uiCountIn;
			while( poDst < poDstMax )
			{
				*poDst++ = *poSrc++; // OUT_TRIGG
				for( unsigned int k=0; k<m_uiCountVal-1; k++ )
					*poDst++ = *poSrc++;
				poDst->Reset();
				++poDst;
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
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpSwitch, OP_SWT_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSwitch )
OP_GENERIC_METHODS_DEF( COpSwitch )

#endif // OP_INC_OP_SWITCH
