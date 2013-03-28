// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTime.cpp

#include "OpTime.h"
#ifdef OP_INC_OP_TIME

////////////////////////////////////////////////////////////////////////////////
// COpTime
////////////////////////////////////////////////////////////////////////////////

#define OP_TMR_IN_CLEAR		0
#define OP_TMR_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_TMR_IN_CLEAR_INFO	"reset"
#define OP_TMR_OUT_DST_INFO	"time"
#define OP_TMR_CLASS_INFO \
\
"Time\n" \
"- When the reset input value is greater than 0.0, the measurement\n" \
"  starts and the output value is reset to 0.0.\n" \
"- The time is measured in seconds."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_TMR_IN_CLEAR_INFO	"Reset"
#define OP_TMR_OUT_DST_INFO	"Zeit"
#define OP_TMR_CLASS_INFO \
\
"Zeit\n" \
"- Nach einem Reset wird mit der Zeitmessung begonnen, der Ausgang steht auf 0.0.\n" \
"- Die Einheit ist Sekunden."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpTime::COpTime()
: COp( 1, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "time" );
	m_poIn[OP_TMR_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_TMR_IN_CLEAR, "cl", OP_TMR_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TMR_OUT_DST, "o", OP_TMR_OUT_DST_INFO );
	Reset();
	Validate();
}

COpTime::~COpTime()
{}

void COpTime::Init()
{
	OP_SET_COUNT_INTERNAL_FLT( 1 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dTime );
	COp::Init();
}

void COpTime::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_TMR_IN_CLEAR].IsValid() )
		if( m_poIn[OP_TMR_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	m_pdOut[OP_TMR_OUT_DST] = m_dTime;
	m_dTime += OP_ONE_D_SAMPLE_FREQ;
}

void COpTime::Reset()
{
	m_dTime = 0.0;
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpTime, OP_TMR_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpTime )
OP_GENERIC_METHODS_DEF( COpTime )

#endif // OP_INC_OP_TIME
