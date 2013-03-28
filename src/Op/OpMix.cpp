// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeq.cpp

#include "OpMix.h"
#ifdef OP_INC_OP_MIX

////////////////////////////////////////////////////////////////////////////////
// COpMix
////////////////////////////////////////////////////////////////////////////////

#define OP_MIX_IN_SRC			0
#define OP_MIX_IN_FACTOR		1
#define OP_MIX_OUT_NUM			0
#define OP_MIX_OUT_DST			1
#define OP_MIX_ROUTINE_EXPAND		0
#define OP_MIX_ROUTINE_SHRINK		1
#define OP_MIX_ROUTINE_FUNC		2

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MIX_IN_SRC_INFO		"input"
#define OP_MIX_IN_FACTOR_INFO		"factor"
#define OP_MIX_OUT_NUM_INFO		"number of inputs"
#define OP_MIX_OUT_DST_INFO		"output"
#define OP_MIX_ROUTINE_EXPAND_INFO	"add input"
#define OP_MIX_ROUTINE_SHRINK_INFO	"remove input"
#define OP_MIX_ROUTINE_FUNC_INFO	"mix function"
#define OP_MIX_CLASS_INFO \
\
"Mixer\n" \
"- All input values are combined over the mix function.\n" \
"- Available mix functions: ADD, MULTIPLY.\n" \
"- The factors define the contribution of each input inlet.\n" \
"- Unconnected factor inlets are considered to be 1.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MIX_IN_SRC_INFO		"Eingang"
#define OP_MIX_IN_FACTOR_INFO		"Faktor"
#define OP_MIX_OUT_NUM_INFO		"Anazhl Eingänge"
#define OP_MIX_OUT_DST_INFO		"Ausgang"
#define OP_MIX_ROUTINE_EXPAND_INFO	"Eingang hinzufügen"
#define OP_MIX_ROUTINE_SHRINK_INFO	"Eingang entfernen"
#define OP_MIX_ROUTINE_FUNC_INFO	"Mixfunktion"
#define OP_MIX_CLASS_INFO \
\
"Mixer\n" \
"- Alle Werte an den Eingängen werden über die Mixfunktion miteinander\n" \
"  kombiniert und anschliessend an den einen Ausgang ausgegeben.\n" \
"- Als Mixfunktion stehen Addition und die Multiplikation zur Verfügung.\n" \
"- Die Faktoren bestimmen den Mix-Anteil jedes Eingangs.\n" \
"- Eingäne ohne Faktor werden unverändert gemixt.\n"

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMix::COpMix()
: COp( 2, 2 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "mix" );
	m_poIn[OP_MIX_IN_SRC].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_MIX_IN_FACTOR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_MIX_IN_SRC, "i", OP_MIX_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MIX_IN_FACTOR, "f", OP_MIX_IN_FACTOR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MIX_OUT_NUM, "n", OP_MIX_OUT_NUM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MIX_OUT_DST, "+", OP_MIX_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 3 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MIX_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_MIX_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MIX_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_MIX_ROUTINE_SHRINK_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MIX_ROUTINE_FUNC, "f" OP_STR_ROUTINE, OP_MIX_ROUTINE_FUNC_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS );
	Update();
	Validate();
}

COpMix::~COpMix() 
{}

void COpMix::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	COp::Init();
}

void COpMix::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	double dVal;
	unsigned int i = OP_MIX_IN_SRC;
	if( !m_uiFuncNum )
	{
		dVal = 0.0;
		while( i < m_uiCountIn )
		{
			if( m_poIn[i].IsValid() )
			{
				double dIn = m_poIn[i].Proc();
				++i;
				if( m_poIn[i].IsValid() )
					dIn *= m_poIn[i].Proc();
				dVal += dIn;
				++i;
			}
			else
				i += 2;
		}
	}
	else
	{
		dVal = 1.0;
		while( i < m_uiCountIn )
		{
			if( m_poIn[i].IsValid() )
				dVal *= m_poIn[i].Proc();
			++i;
		}
	}
	m_pdOut[OP_MIX_OUT_DST] = dVal;
}

void COpMix::Update()
{
	COp::Update();
	
	m_pdOut[OP_MIX_OUT_NUM] = double( m_uiCountIn / 2 );
	
#ifdef OP_USE_RUNTIME_INFO
	unsigned int i = 2;
	while( i < m_uiCountIn )
	{
		SetNameInput( i, "i" );
		SetInfoInput( i++, OP_MIX_IN_SRC_INFO );
		SetNameInput( i, "f" );
		SetInfoInput( i++, OP_MIX_IN_FACTOR_INFO );
	}
	
	const char * acNameOut[] = { "+", "*" };
	SetNameOutput( OP_MIX_OUT_DST, acNameOut[m_uiFuncNum] );
#endif // OP_USE_RUNTIME_INFO
}

#ifdef OP_USE_ROUTINES
void COpMix::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_MIX_ROUTINE_SHRINK:
		if( m_uiCountIn > OP_MIX_IN_FACTOR + 1 )
			SetCountIn( m_uiCountIn - 2 );
	break;
	case OP_MIX_ROUTINE_EXPAND:
	{
		SetCountIn( m_uiCountIn + 2 );
		m_poIn[m_uiCountIn - 1].SetFlags( OP_LINK_FLAG_OPTIONAL );
		m_poIn[m_uiCountIn - 2].SetFlags( OP_LINK_FLAG_OPTIONAL );
	}
	break;
	case OP_MIX_ROUTINE_FUNC:
		m_uiFuncNum = !m_uiFuncNum;
	break;
	}
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpMix, OP_MIX_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMix )
OP_GENERIC_METHODS_DEF( COpMix )

#endif // OP_INC_OP_MIX
