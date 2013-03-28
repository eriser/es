// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltLow0.cpp

#include "OpFltLow0.h"
#ifdef OP_INC_OP_FLT_LOW_0

////////////////////////////////////////////////////////////////////////////////
// COpFltLow0
////////////////////////////////////////////////////////////////////////////////

#define OP_FLL_IN_SRC		0
#define OP_FLL_IN_FREQ		1
#define OP_FLL_IN_CLEAR		2
#define OP_FLL_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FLL_IN_SRC_INFO	"input"
#define OP_FLL_IN_FREQ_INFO	"frequency"
#define OP_FLL_IN_CLEAR_INFO	"reset"
#define OP_FLL_OUT_DST_INFO	"output"
#define OP_FLL_CLASS_INFO \
\
"Resonant Low-Pass Filter\n" \
"- Too large frequency values can cause unstable behavior!\n" \
"- When the reset input value is greater than 0.0, then all the internal\n" \
"  variables will be reinitialized."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FLL_IN_SRC_INFO	"Eingang"
#define OP_FLL_IN_FREQ_INFO	"Frequenz"
#define OP_FLL_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_FLL_OUT_DST_INFO	"Ausgang"
#define OP_FLL_CLASS_INFO \
\
"Resonanter Tief-Pass-Filter\n" \
"- Zu hohe Frequenz-Werte können instabiles Verhalten hervorrufen.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der Speicher\n" \
"  zurückgesetzt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltLow0::COpFltLow0()
: COp( 3, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flLo0" );
	m_poIn[OP_FLL_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLL_IN_SRC, "i", OP_FLL_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLL_IN_FREQ, "f", OP_FLL_IN_FREQ_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FLL_IN_CLEAR, "cl", OP_FLL_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FLL_OUT_DST, "o", OP_FLL_OUT_DST_INFO );
	Update();
	Validate();
}

COpFltLow0::~COpFltLow0() 
{}

void COpFltLow0::Init()
{
	Reset();
	OP_SET_COUNT_INTERNAL_FLT( 2 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oFlt.m_tVibraPos );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oFlt.m_tVibraSpeed );
	COp::Init();
}

void COpFltLow0::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_FLL_IN_CLEAR].IsValid() )
		if( m_poIn[OP_FLL_IN_CLEAR].Proc() > 0.0 )
			m_oFlt.Reset();
	
	const double dCutoff = m_poIn[OP_FLL_IN_FREQ].Proc();
	if( m_dCutoff != dCutoff )
	{
		m_dCutoff = dCutoff;
		m_oFlt.Init( dCutoff );
	}
	m_pdOut[OP_FLL_OUT_DST] = m_oFlt.Proc( m_poIn[OP_FLL_IN_SRC].Proc() );
}

void COpFltLow0::Update() {
	m_oFlt.Init( m_dCutoff );
}
	
void COpFltLow0::Reset()
{
	COp::Reset();
	m_oFlt.Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpFltLow0, OP_FLL_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltLow0 )
OP_GENERIC_METHODS_DEF( COpFltLow0 )

#endif // OP_INC_OP_FLT_LOW_0 
