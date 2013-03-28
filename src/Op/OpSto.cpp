// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOp.cpp

#include "OpSto.h"
#ifdef OP_INC_OP_STO

////////////////////////////////////////////////////////////////////////////////
// COpSto
////////////////////////////////////////////////////////////////////////////////

#define OP_STO_IN_COND		0
#define OP_STO_IN_VAL		1
#define OP_STO_IN_CLEAR		2
#define OP_STO_OUT_DST		0
#define OP_STO_OUT_PRV		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_STO_IN_COND_INFO	"condition"
#define OP_STO_IN_VAL_INFO	"value"
#define OP_STO_IN_CLEAR_INFO	"reset"
#define OP_STO_OUT_DST_INFO	"output"
#define OP_STO_OUT_PRV_INFO	"previous value"
#define OP_STO_CLASS_INFO \
\
"Conditional Storage\n" \
"- When the condition input value is greater than 0.0, then the input\n" \
"  value will be stored and hold constant on the output.\n" \
"- All inlets are processed every cycle.\n" \
"- The value input is 1.0 by default.\n" \
"- When the reset input value is greater than 0.0, then the operator\n" \
"  will be reinitialized, and internal variables will be reset to 0.0.\n" \
"- In reset inlet is processed first."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_STO_IN_COND_INFO	"Kondition"
#define OP_STO_IN_VAL_INFO	"Wert"
#define OP_STO_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_STO_OUT_DST_INFO	"Ausgang"
#define OP_STO_OUT_PRV_INFO	"Letzer Wert"
#define OP_STO_CLASS_INFO \
\
"Bedingter Speicher\n" \
"- Wenn der Konditionswert grösser als 0.0 ist, wird der erste Wert eingelesen\n" \
"  und von nun am Ausgang konstant gehalten.\n" \
"- Alle Eingänge werden trotzdem immer abgefragt.\n" \
"- Der Wert-Eingang steht standardmässig auf 1.0.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der der Operator in den\n" \
"  Ausgangszustand zurückgesetzt.\n" \
"- Der Zurücksetzen-Eingang wird als erster abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSto::COpSto()
: COp( 3, 2 )
, m_dVal( 0.0 )
, m_dValPrev( 0.0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "sto" );
	m_poIn[OP_STO_IN_VAL].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_STO_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_STO_IN_COND, "c", OP_STO_IN_COND_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_STO_IN_VAL, "v", OP_STO_IN_VAL_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_STO_IN_CLEAR, "cl", OP_STO_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_STO_OUT_DST, "o", OP_STO_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_STO_OUT_PRV, "p", OP_STO_OUT_PRV_INFO );
	Update();
	Validate();
}

void COpSto::Init()
{
	OP_SET_COUNT_INTERNAL_FLT( 2 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dVal );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_dValPrev );
	COp::Init();
}

COpSto::~COpSto() 
{}

void COpSto::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();

	if( m_poIn[OP_STO_IN_CLEAR].IsValid() )
		if( m_poIn[OP_STO_IN_CLEAR].Proc() > 0.0 )
			Reset();
	double dIn = 1.0;
	if( m_poIn[OP_STO_IN_VAL].IsValid() )
		dIn = m_poIn[OP_STO_IN_VAL].Proc();
	if( m_poIn[OP_STO_IN_COND].Proc() > 0.0 )
	{
		m_dValPrev = m_dVal;
		m_dVal = dIn;
		Update();
	}
}

void COpSto::Update()
{
	m_pdOut[OP_STO_OUT_DST] = m_dVal;
	m_pdOut[OP_STO_OUT_PRV] = m_dValPrev;
	COp::Update();
}

void COpSto::Reset()
{
	m_dVal = m_dValPrev = 0.0;
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpSto, OP_STO_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSto )
OP_GENERIC_METHODS_DEF( COpSto )

#endif // OP_INC_OP_STO
