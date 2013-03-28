// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDemux.cpp

#include "OpDemux.h"
#ifdef OP_INC_OP_DEMUX

////////////////////////////////////////////////////////////////////////////////
// COpDemux
////////////////////////////////////////////////////////////////////////////////

#define OP_DMP_IN_SRC			0
#define OP_DMP_IN_CLEAR			1
#define OP_DMP_OUT_NUM			0
#define OP_DMP_OUT_POS			1
#define OP_DMP_OUT_SYNC			2
#define OP_DMP_OUT_DST			3

#define OP_DMP_ROUTINE_SHRINK		0
#define OP_DMP_ROUTINE_EXPAND		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_DMP_IN_SRC_INFO		"input"
#define OP_DMP_IN_CLEAR_INFO		"reset"
#define OP_DMP_OUT_NUM_INFO		"number of outputs"
#define OP_DMP_OUT_SYNC_INFO		"sync"
#define OP_DMP_OUT_POS_INFO		"position"
#define OP_DMP_OUT_DST_INFO		"outout"
#define OP_DMP_ROUTINE_EXPAND_INFO	"add output"
#define OP_DMP_ROUTINE_SHRINK_INFO	"remove output"
#define OP_DMP_CLASS_INFO \
\
"Demultiplexer\n" \
"- The incoming series of input values is mapped to an arbitrary number of\n" \
"  outputs, sample by sample. The outputs are used consecutively, one by one.\n" \
"- After the last output has been used, it continues with the first one.\n" \
"  At that moment, the sync output is set to 1.0, otherwise always to 0.0.\n" \
"- When the reset input value is greater than 0.0, then the output position is\n" \
"  reset to the first output outlet."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_DMP_IN_SRC_INFO		"Eingang"
#define OP_DMP_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_DMP_OUT_NUM_INFO		"Anzahl Ausgänge"
#define OP_DMP_OUT_SYNC_INFO		"Synchronisation"
#define OP_DMP_OUT_POS_INFO		"Position"
#define OP_DMP_OUT_DST_INFO		"Ausgang"
#define OP_DMP_ROUTINE_EXPAND_INFO	"Ausgang hinzufügen"
#define OP_DMP_ROUTINE_SHRINK_INFO	"Ausgang entfernen"
#define OP_DMP_CLASS_INFO \
\
"Demultiplexer\n" \
"- Die Sequenz aus Eingabewerten wird auf die Ausgänge so abgebildet, dass der\n" \
"  Wert nach dem aktuellen Wert immer auf dem folgenden Ausgang ausgegeben\n" \
"  wird.\n" \
"- Nachdem der letze Ausgang beschrieben wurde, wird beim ersten forgefahren.\n" \
"  In diesem Moment steht der Synchronisations-Ausgang auf 1.0, sonst auf 0.0.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird die Position auf 0.0\n" \
"  gesetzt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpDemux::COpDemux()
: COp( 2, 4 )
, m_uiPos( OP_DMP_OUT_DST )
{
	Init();
	OP_SET_NAME_INSTANCE( "dmux" );
	m_poIn[OP_DMP_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_DMP_IN_SRC, "i", OP_DMP_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_DMP_IN_CLEAR, "cl", OP_DMP_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_DMP_OUT_NUM, "n", OP_DMP_OUT_NUM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_DMP_OUT_POS, "p", OP_DMP_OUT_POS_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_DMP_OUT_SYNC, "cy", OP_DMP_OUT_SYNC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_DMP_OUT_DST, "o", OP_DMP_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_DMP_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_DMP_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_DMP_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_DMP_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpDemux::~COpDemux()
{}

void COpDemux::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiPos );
	COp::Init();
}

void COpDemux::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_DMP_IN_CLEAR].IsValid() )
		if( m_poIn[OP_DMP_IN_CLEAR].Proc() > 0.0 )
			Reset();

	m_pdOut[OP_DMP_OUT_POS] = double( m_uiPos - OP_DMP_OUT_DST );
	m_pdOut[m_uiPos] = m_poIn[OP_DMP_IN_SRC].Proc();
	
	double dSync = 0.0;
	if( ++m_uiPos >= m_uiCountOut )
	{
		m_uiPos = OP_DMP_OUT_DST;
		dSync = 1.0;
	}
	m_pdOut[OP_DMP_OUT_SYNC] = dSync;
}

void COpDemux::Update()
{
	m_pdOut[OP_DMP_OUT_NUM] = double( m_uiCountOut - OP_DMP_OUT_DST );
	
#ifdef OP_USE_RUNTIME_INFO
	unsigned int o = OP_DMP_OUT_DST;
	while( o < m_uiCountOut )
	{
		SetNameOutput( o, "o" );
		SetInfoOutput( o++, OP_DMP_OUT_DST_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
	COp::Update();
}

void COpDemux::Reset()
{
	m_uiPos = OP_DMP_OUT_DST;
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpDemux::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_DMP_ROUTINE_SHRINK:
		if( m_uiCountOut > OP_DMP_OUT_DST + 1 )
			SetCountOut( m_uiCountOut - 1 );
	break;
	case OP_DMP_ROUTINE_EXPAND:
		SetCountOut( m_uiCountOut + 1 );
	break;
	}
	Reset();
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpDemux, OP_DMP_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpDemux )
OP_GENERIC_METHODS_DEF( COpDemux )

#endif // OP_INC_OP_DEMUX
