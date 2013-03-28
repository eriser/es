// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDelay.cpp

#include "OpDelay.h"
#ifdef OP_INC_OP_DELAY

////////////////////////////////////////////////////////////////////////////////
// COpDelay
////////////////////////////////////////////////////////////////////////////////

#define OP_DLY_IN_SRC			0
#define OP_DLY_IN_DELAY			1
#define OP_DLY_OUT_DST			0
#define OP_DLY_ROUTINE_MODE		0

#define OP_DLY_MODE_DISCRETE		0
#define OP_DLY_MODE_INTERPOL		1
#define OP_DLY_MODE_MAX			2

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_DLY_IN_SRC_INFO		"input"
#define OP_DLY_IN_DELAY_INFO		"delay"
#define OP_DLY_OUT_DST_INFO		"output"
#define OP_DLY_ROUTINE_MODE_INFO	"mode"
#define OP_DLY_CLASS_INFO \
\
"Delay\n" \
"- Input values are written to the output with an arbitrary delay.\n" \
"- The minimal delay is 1 sample.\n" \
"- Internally, the samples are stored in a cyclic buffer.\n" \
"- The mode routine switches between a discrete and linear interpolation.\n" \
"- Linear interpolation requires a little more resources, but is better\n" \
"  suited for dynamically changing delays."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_DLY_IN_SRC_INFO		"Eingang"
#define OP_DLY_IN_DELAY_INFO		"Verzögerung"
#define OP_DLY_OUT_DST_INFO		"Ausgang"
#define OP_DLY_ROUTINE_MODE_INFO	"Modus"
#define OP_DLY_CLASS_INFO \
\
"Verzögerung\n" \
"- Die Eingabe wird um die angegebene Anzahl Samples verzögert ausgegeben.\n" \
"- Die minimale Verzögerung beträgt 1 Sample.\n" \
"- Intern werden die Samples in einem zyklischen Puffer gespeichert." \
"- Die Modus-Routine wechselt zwischen einem diskreten und einem interpolierten\n" \
"  Verzögerungs-Puffer, wobei letzterer mehr Ressourcen benötigt, aber besser für\n" \
"  dynamische Änderungen geeignet ist."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpDelay::COpDelay()
: COp( 2, 1 )
, m_uiMode( OP_DLY_MODE_DISCRETE )
{
	Init();
	OP_SET_NAME_INSTANCE( "delay" );
	OP_SET_NAME_AND_INFO_INPUT( OP_DLY_IN_SRC, "i", OP_DLY_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_DLY_IN_DELAY, "d", OP_DLY_IN_DELAY_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_DLY_OUT_DST, "o", OP_DLY_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_DLY_ROUTINE_MODE, "m" OP_STR_ROUTINE, OP_DLY_ROUTINE_MODE_INFO );
	Update();
	Validate();
}

COpDelay::~COpDelay()
{}

void COpDelay::Init()
{
	m_dDelay = 1.0;
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiMode );
	COp::Init();
}

void COpDelay::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dDly = m_poIn[OP_DLY_IN_DELAY].Proc();
	if( dDly && dDly != m_dDelay )
	{
		m_dDelay = dDly;
		if( m_uiMode )
			m_oDelayInterpol.SetDelay( dDly );
		else
			m_oDelay.SetDelay( dDly );
	}
	
	if( m_uiMode )
	{
		m_pdOut[OP_DLY_OUT_DST] = m_oDelayInterpol.ReadDelay();
		m_oDelayInterpol.WriteDelay( m_poIn[OP_DLY_IN_SRC].Proc() );
	}
	else
	{
		m_pdOut[OP_DLY_OUT_DST] = m_oDelay.ReadDelay();
		m_oDelay.WriteDelay( m_poIn[OP_DLY_IN_SRC].Proc() );
	}
}

void COpDelay::Update()
{
	COp::Update();
	
	m_oDelay.SetDelay( m_dDelay );
	m_oDelayInterpol.SetDelay( m_dDelay );
	//m_oDelay.UpdateSize();
	//m_oDelayInterpol.UpdateSize();
	
#ifdef OP_USE_RUNTIME_INFO
	static const char * aacName_[] = { "/", "~" };
	SetNameOutput( OP_DLY_OUT_DST, aacName_[m_uiMode] );
#endif // OP_USE_RUNTIME_INFO
}

void COpDelay::Reset()
{
	m_oDelay.Reset();
	m_oDelayInterpol.Reset();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpDelay::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	m_uiMode = !m_uiMode;
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpDelay, OP_DLY_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpDelay )
OP_GENERIC_METHODS_DEF( COpDelay )

#endif // OP_INC_OP_DELAY
