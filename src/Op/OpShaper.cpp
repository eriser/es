// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpShaper.cpp

#include "OpShaper.h"
#ifdef OP_INC_OP_SHAPER

////////////////////////////////////////////////////////////////////////////////
// COpShaper
////////////////////////////////////////////////////////////////////////////////

#define OP_SHP_IN_SRC		0
#define OP_SHP_OUT_DST		0
#define OP_SHP_ROUTINE_FUNC	0
#define OP_SHP_FUNC_SMOOTH0	0
#define	OP_SHP_FUNC_POW0	1
#define	OP_SHP_FUNC_SIN0	2
#define	OP_SHP_FUNC_GLOUBI0	3
#define	OP_SHP_FUNC_GLOUBI1	4
#define OP_SHP_FUNC_MAX		5

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_SHP_IN_SRC_INFO	"input"
#define OP_SHP_OUT_DST_INFO	"output"
#define OP_SHP_FUNC_INFO	"function"
#define OP_SHP_CLASS_INFO \
\
"Waveshaper\n" \
"- Available functions: SMOOTH, POW, SIN, GLOUBI0, GLOUBI1.\n" \
"- GLOUBI1 is expensive."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_SHP_IN_SRC_INFO	"Eingang"
#define OP_SHP_OUT_DST_INFO	"Ausgang"
#define OP_SHP_FUNC_INFO	"Funktion"
#define OP_SHP_CLASS_INFO \
\
"Waveshaper\n" \
"- Es stehen folgende Funktionen zur Auswahl: SMOOTH, POW, SIN, GLOUBI0, GLOUBI1.\n" \
"- GLOUBI1 ist teuer."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpShaper::COpShaper()
: COp( 1, 1 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "shaper" );
	OP_SET_NAME_AND_INFO_INPUT( OP_SHP_IN_SRC, "i", OP_SHP_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SHP_OUT_DST, "", OP_SHP_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SHP_ROUTINE_FUNC, "f" OP_STR_ROUTINE, OP_SHP_FUNC_INFO );
	Update();
	Validate();
}

COpShaper::~COpShaper() 
{}

void COpShaper::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	COp::Init();
}

void COpShaper::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	double x = m_poIn[OP_SHP_IN_SRC].Proc();
	switch( m_uiFuncNum )
	{
	case OP_SHP_FUNC_SMOOTH0: // Jon Watte, www.musicdsp.org.
		x = 1.5 * x - 0.5 * x * x * x;
	break;
	case OP_SHP_FUNC_POW0:
		x *= x * x;
	break;
	case OP_SHP_FUNC_SIN0:
		x = sin( x );
	break;
	case OP_SHP_FUNC_GLOUBI0:
		x *= 0.686306;
		x -= 0.15 * x * x - 0.15 * x * x * x;
	break;
	case OP_SHP_FUNC_GLOUBI1: // Laurent de Soras, www.musicdsp.org.
	{
		const double a = 1.0 + exp( sqrt( fabs( x ) ) * -0.75 );
		x *= 0.686306;
		x = ( exp (x) - exp( -x * a ) ) / ( exp( x ) + exp( -x ) );
	}
	break;
	};
	m_pdOut[OP_SHP_OUT_DST] = x;
}

void COpShaper::Update()
{
	COp::Update();
	
#ifdef OP_USE_RUNTIME_INFO
	const char * aacFuncName_[] = { "sm", "p", "si", "g0", "g1" };
	SetNameOutput( OP_SHP_OUT_DST, aacFuncName_[m_uiFuncNum] );
#endif // OP_USE_RUNTIME_INFO
}

#ifdef OP_USE_ROUTINES
void COpShaper::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	if( ++m_uiFuncNum >= OP_SHP_FUNC_MAX )
		m_uiFuncNum = 0;
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpShaper, OP_SHP_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpShaper )
OP_GENERIC_METHODS_DEF( COpShaper )

#endif // OP_INC_OP_SHAPER
