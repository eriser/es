// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltMoog.cpp

#include "OpFltMoog.h"
#ifdef OP_INC_OP_FLT_MOOG

////////////////////////////////////////////////////////////////////////////////
// COpFltMoog
////////////////////////////////////////////////////////////////////////////////

#define OP_FLM_IN_SRC		0
#define OP_FLM_IN_FREQ		1
#define OP_FLM_IN_RES		2
#define OP_FLM_IN_CLEAR		3
#define OP_FLM_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FLM_IN_SRC_INFO	"input"
#define OP_FLM_IN_FREQ_INFO	"frequency"
#define OP_FLM_IN_RES_INFO	"resonance"
#define OP_FLM_IN_CLEAR_INFO	"reset"
#define OP_FLM_OUT_DST_INFO	"output"
#define OP_FLM_CLASS_INFO \
\
"Moog Filter\n" \
"- When the frequency value change too fast, the filter may become unstable!\n" \
"- When the reset input value is greater than 0.0, then all the internal\n" \
"  variables will be reinitialized."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FLM_IN_SRC_INFO	"Eingang"
#define OP_FLM_IN_FREQ_INFO	"Frequenz"
#define OP_FLM_IN_RES_INFO	"Resonanz"
#define OP_FLM_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_FLM_OUT_DST_INFO	"Ausgang"
#define OP_FLM_CLASS_INFO \
\
"Moog-Filter\n" \
"- Bei zu raschen Änderungen des Frequenz Wertes kann das Filter instabil werden.\n" \
"- Ist der Reset-Eingang grösser als 0.0, so wird der Speicher zurückgesetzt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltMoog::COpFltMoog()
: COp( 4, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flMoog" );
	m_poIn[OP_FLM_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLM_IN_SRC, "i", OP_FLM_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLM_IN_FREQ, "f", OP_FLM_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLM_IN_RES, "r", OP_FLM_IN_RES_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLM_IN_CLEAR, "cl", OP_FLM_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FLM_OUT_DST, "o", OP_FLM_OUT_DST_INFO );
	Validate();
	Update();
}

COpFltMoog::~COpFltMoog() 
{}

void COpFltMoog::Init()
{
	Reset();
	m_dCutoff = m_dRes = 0.0;
	OP_SET_COUNT_INTERNAL_FLT( 8 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oFlt.m_tIn0 );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oFlt.m_tIn1 );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oFlt.m_tIn2 );
	OP_SET_PTR_INTERNAL_FLT( 3, &m_oFlt.m_tIn3 );
	OP_SET_PTR_INTERNAL_FLT( 4, &m_oFlt.m_tOut0 );
	OP_SET_PTR_INTERNAL_FLT( 5, &m_oFlt.m_tOut1 );
	OP_SET_PTR_INTERNAL_FLT( 6, &m_oFlt.m_tOut2 );
	OP_SET_PTR_INTERNAL_FLT( 7, &m_oFlt.m_tOut3 );
	COp::Init();
}

void COpFltMoog::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_FLM_IN_CLEAR].IsValid() )
		if( m_poIn[OP_FLM_IN_CLEAR].Proc() > 0.0 )
			m_oFlt.Reset();
	
	const double dCutoff = m_poIn[OP_FLM_IN_FREQ].Proc();
	const double dRes = m_poIn[OP_FLM_IN_RES].Proc();
	if( m_dCutoff != dCutoff || m_dRes != dRes )
	{
		m_dCutoff = dCutoff;
		m_dRes = dRes;
		m_oFlt.Init( dCutoff, dRes );
	}
	m_pdOut[OP_FLM_OUT_DST] = m_oFlt.Proc( m_poIn[OP_FLM_IN_SRC].Proc() );
}

void COpFltMoog::Update()
{
	m_oFlt.Init( m_dCutoff, m_dRes );
}

void COpFltMoog::Reset()
{
	COp::Reset();
	m_oFlt.Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpFltMoog, OP_FLM_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltMoog )
OP_GENERIC_METHODS_DEF( COpFltMoog )

#endif // OP_INC_OP_FLT_MOOG
