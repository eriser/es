// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpRetrigger.cpp

#include "OpRetrigger.h"
#ifdef OP_INC_OP_RETRIGGER

////////////////////////////////////////////////////////////////////////////////
// COpRetrigger
////////////////////////////////////////////////////////////////////////////////

#define OP_DST_IN_TRIGG		0
#define OP_DST_IN_DELAY		1
#define OP_DST_IN_CLEAR		2
#define OP_DST_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_DST_IN_TRIGG_INFO	"trigger"
#define OP_DST_IN_DELAY_INFO	"delay"
#define OP_DST_IN_CLEAR_INFO	"reset"
#define OP_DST_OUT_DST_INFO	"output"
#define OP_DST_CLASS_INFO \
\
"Retrigger\n" \
"- Once the trigger input value is greater than 0.0, a new trigger event will\n" \
"  be activated.\n" \
"- The delay input value defines the number of samples until an incoming\n" \
"  trigger event will be output again as a value of 1.0.\n" \
"- Incoming trigger events always overwrite the current one! So, to prevent\n" \
"  accidental data loss, a normal delay operator may be more suitable in\n" \
"  certain cases.\n" \
"- When the reset input value is greater than 0.0, the current trigger event\n" \
"  will always be canceled."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_DST_IN_TRIGG_INFO	"Auslöser"
#define OP_DST_IN_DELAY_INFO	"Verzögerung"
#define OP_DST_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_DST_OUT_DST_INFO	"Ausgang"
#define OP_DST_CLASS_INFO \
\
"Retrigger\n" \
"- Nachdem der Auslöser-Wert einmal grösser als 0.0 gewesen ist, steht der\n" \
"  Ausgangswert genau nach der an dem Verzögerungs-Eingang angegebenen Anzahl\n" \
"  Samples Verzögerung auf 1.0, sonst immer auf 0.0.\n" \
"- Neu einkommende Auslöser überschreiben noch nicht ausgebene! Um also keine\n" \
"  Daten zu verlieren kann ein ganz normaler Delay-Operator besser sein.\n" \
"- Ist der Wert am Zurücksetzen-Eingang grösser als 0.0, so wird ein allfällig\n" \
"  anstehender Auslöser-Wert gelöscht."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpRetrigger::COpRetrigger()
: COp( 3, 1 )
, m_iPos( -1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "retrg" );
	m_poIn[OP_DST_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_DST_IN_TRIGG, "tr", OP_DST_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_DST_IN_DELAY, "d", OP_DST_IN_DELAY_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_DST_IN_CLEAR, "cl", OP_DST_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_DST_OUT_DST, "o", OP_DST_OUT_DST_INFO );
	Validate();
}

COpRetrigger::~COpRetrigger() 
{}

void COpRetrigger::Init() 
{
	OP_SET_COUNT_INTERNAL_INT( 1 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_iPos );
	COp::Init();
}

void COpRetrigger::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dDelay = m_poIn[OP_DST_IN_DELAY].Proc();
	if( m_poIn[OP_DST_IN_TRIGG].Proc() > 0.0 )
		m_iPos = abs( (int)rint( dDelay ) );
	
	if( m_poIn[OP_DST_IN_CLEAR].IsValid()
	 && m_poIn[OP_DST_IN_CLEAR].Proc() > 0.0 )
		Reset();
	
	m_pdOut[OP_DST_OUT_DST] = ( !m_iPos ? 1.0 : 0.0 );
	--m_iPos;
}

void COpRetrigger::Reset()
{
	m_iPos = -1;
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpRetrigger, OP_DST_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpRetrigger )
OP_GENERIC_METHODS_DEF( COpRetrigger )

#endif // OP_INC_OP_RETRIGGER
