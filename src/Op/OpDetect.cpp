// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDetect.cpp

#include "OpDetect.h"
#ifdef OP_INC_OP_DETECT

////////////////////////////////////////////////////////////////////////////////
// COpDetect
////////////////////////////////////////////////////////////////////////////////

#define OP_DTC_IN_SRC		0
#define OP_DTC_IN_THRES		1
#define OP_DTC_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_DTC_IN_SRC_INFO	"input"
#define OP_DTC_IN_THRES_INFO	"threshold"
#define OP_DTC_OUT_DST_INFO	"output"
#define OP_DTC_CLASS_INFO \
\
"Detector\n" \
"- When the input value crosses the threshold, then the output will be 1.0,\n" \
"  otherwise always 0.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_DTC_IN_SRC_INFO	"Eingang"
#define OP_DTC_IN_THRES_INFO	"Schranke"
#define OP_DTC_OUT_DST_INFO	"Ausgang"
#define OP_DTC_CLASS_INFO \
\
"Detektor\n" \
"- Wenn der Eingangswert den Schranken-Wert über- oder unterschreitet,\n" \
"  steht der Ausgang auf 1.0, sonst auf 0.0."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpDetect::COpDetect()
: COp( 2, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "detect" );
	m_poIn[OP_DTC_IN_THRES].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_DTC_IN_SRC, "i", OP_DTC_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_DTC_IN_THRES, "th", OP_DTC_IN_THRES_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_DTC_OUT_DST, "o", OP_DTC_OUT_DST_INFO );
	Validate();
	Reset();
}

COpDetect::~COpDetect()
{}

void COpDetect::Init()
{
	OP_SET_COUNT_INTERNAL_FLT( 2 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dL0 );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_dL1 );
}

void COpDetect::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	double dCurr = m_poIn[OP_DTC_IN_SRC].Proc();
	
	if( m_poIn[OP_DTC_IN_THRES].IsValid() )
		dCurr -= m_poIn[OP_DTC_IN_THRES].Proc();
	
	if( ( dCurr <= 0.0 && m_dL0 >= 0.0 ) || 
	    ( dCurr >= 0.0 && m_dL1 <= 0.0 ) ) {
		m_pdOut[OP_DTC_OUT_DST] = +1.0;
	}
	else {
		m_pdOut[OP_DTC_OUT_DST] = 0.0;
	}
	
	m_dL1 = m_dL0;
	m_dL0 = dCurr;
}

void COpDetect::Reset()
{
	COp::Reset();
	m_dL0 = m_dL1 = 0.0;
}

OP_SET_NAME_AND_INFO_CLASS( COpDetect, OP_DTC_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpDetect )
OP_GENERIC_METHODS_DEF( COpDetect )

#endif // OP_INC_OP_DETECT
