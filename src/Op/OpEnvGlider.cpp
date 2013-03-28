// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnvGlider.cpp

#include "OpEnvGlider.h"
#ifdef OP_INC_OP_ENV_GLIDER

////////////////////////////////////////////////////////////////////////////////
// COpEnvGlider
////////////////////////////////////////////////////////////////////////////////

#define OP_EGL_IN_SRC			0
#define OP_EGL_IN_DUR			1
#define OP_EGL_IN_SLOPE			2
#define OP_EGL_OUT_DST			0
#define OP_EGL_OUT_SYNC			1
#define OP_EGL_ROUTINE_FUNC		0

#define OP_EGL_FUNC_LIN			0
#define OP_EGL_FUNC_SIN			1
#define OP_EGL_FUNC_SIN_POW		2
#define OP_EGL_FUNC_MAX			3

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_EGL_IN_SRC_INFO		"input"
#define OP_EGL_IN_DUR_INFO		"duration"
#define OP_EGL_IN_SLOPE_INFO		"slope"
#define OP_EGL_OUT_DST_INFO		"output"
#define OP_EGL_OUT_SYNC_INFO		"sync"
#define OP_EGL_ROUTINE_FUNC_INFO	"function"
#define OP_EGL_CLASS_INFO \
\
"Glider Envelope\n" \
"- Whenever the input value changes, the envelope starts to interpolate\n" \
"  between the present output value and the incoming input value. When\n" \
"  this happens, the sync output value is 1.0, otherwise 0.0.\n" \
"- The duration defines the number of samples until the target value will\n" \
"  be reached.\n" \
"- Available functions: LINEAR, SIN, SIN_POW.\n" \
"- The slope parameter is only used by the SIN_POW function."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_EGL_IN_SRC_INFO		"Eingang"
#define OP_EGL_IN_DUR_INFO		"Dauer"
#define OP_EGL_IN_SLOPE_INFO		"Krümmung"
#define OP_EGL_OUT_DST_INFO		"Ausgang"
#define OP_EGL_OUT_SYNC_INFO		"Synchronisation"
#define OP_EGL_ROUTINE_FUNC_INFO	"Funktion"
#define OP_EGL_CLASS_INFO \
\
"Gleiter-Hüllkurve\n" \
"- Ändert sich der Eingangs-Wert, so wird eine Hüllkurve vom bisherigen\n" \
"  Ausgabewert auf den aktuellen Eingabewert gestartet. Dann steht der\n" \
"  Synchronisations-Ausgang auf 1.0, sonst auf 0.0.\n" \
"- Die Dauer bestimmt die Anzahl Samples bis der Zielwert erreicht wird.\n" \
"- Es stehen folgende Funktion zur Auswahl: LINEAR, SIN, SIN_POW.\n" \
"- Der Krümmungswert hat nur bei SIN_POW Einfluss."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpEnvGlider::COpEnvGlider()
: COp( 3, 2 )
, m_uiFuncNum( 0 )
, m_dDur( 0.0 )
, m_dV0( 0.0 )
, m_dV1( 0.0 )
, m_dSlope( 0.0 )
, m_poEnv( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "eGldr" );
	m_poIn[OP_EGL_IN_SLOPE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_EGL_IN_SRC, "i", OP_EGL_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EGL_IN_DUR, "d", OP_EGL_IN_DUR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_EGL_IN_SLOPE, "s", OP_EGL_IN_SLOPE_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_EGL_OUT_DST, "o", OP_EGL_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_EGL_OUT_SYNC, "tr", OP_EGL_OUT_SYNC_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_EGL_ROUTINE_FUNC, "f" OP_STR_ROUTINE, OP_EGL_ROUTINE_FUNC_INFO );
	Update();
	Validate();
}

COpEnvGlider::~COpEnvGlider() 
{}

void COpEnvGlider::Init() 
{
	Reset();
	OP_SET_COUNT_INTERNAL_UINT( 7 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_oEnvLin.Index() );
	OP_SET_PTR_INTERNAL_UINT( 2, &m_oEnvLin.State() );
	OP_SET_PTR_INTERNAL_UINT( 3, &m_oEnvSin.Index() );
	OP_SET_PTR_INTERNAL_UINT( 4, &m_oEnvSin.State() );
	OP_SET_PTR_INTERNAL_UINT( 5, &m_oEnvSinPow.Index() );
	OP_SET_PTR_INTERNAL_UINT( 6, &m_oEnvSinPow.State() );
	OP_SET_COUNT_INTERNAL_FLT( 10 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oEnvLin.Value() );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oEnvSin.Value() );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oEnvSin.Position() );
	OP_SET_PTR_INTERNAL_FLT( 3, &m_oEnvSinPow.Value() );
	OP_SET_PTR_INTERNAL_FLT( 4, &m_oEnvSinPow.Position() );
	OP_SET_PTR_INTERNAL_FLT( 5, &m_oEnvSinPow.Exponent() );
	OP_SET_PTR_INTERNAL_FLT( 6, &m_dDur );
	OP_SET_PTR_INTERNAL_FLT( 7, &m_dV0 );
	OP_SET_PTR_INTERNAL_FLT( 8, &m_dV1 );
	OP_SET_PTR_INTERNAL_FLT( 9, &m_dSlope );
	COp::Init();
}

inline void COpEnvGlider::UpdateEnvLin() {
	m_oEnvLin.Set( m_dDur, m_dV0, m_dV1 );
}

inline void COpEnvGlider::UpdateEnvSin() {
	m_oEnvSin.Set( m_dDur, m_dV0, m_dV1 );
}

inline void COpEnvGlider::UpdateEnvSinPow() {
	m_oEnvSinPow.Set( m_dDur, m_dV0, m_dV1, m_dSlope );
}

void COpEnvGlider::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dV1 = m_poIn[OP_EGL_IN_SRC].Proc();
	const double dDur = m_poIn[OP_EGL_IN_DUR].Proc();
	double dSlope = 2.0;
	if( m_poIn[OP_EGL_IN_SLOPE].IsValid() )
		dSlope = m_poIn[OP_EGL_IN_SLOPE].Proc();
	double dSync = 0.0;

	if( m_dDur != dDur || m_dV1 != dV1 ) // || m_dSlope != dSlope )
	{
		m_dDur = dDur;
		m_dV0 = m_dV1;
		m_dV1 = dV1;
		m_dSlope = dSlope;
		switch( m_uiFuncNum )
		{
		case OP_EGL_FUNC_LIN:
			UpdateEnvLin();
		break;
		case OP_EGL_FUNC_SIN:
			UpdateEnvSin();
		break;
		case OP_EGL_FUNC_SIN_POW:
			UpdateEnvSinPow();
		break;
		}
		m_poEnv->Reset();
		m_poEnv->Trigger();
		dSync = 1.0;
	}
	m_pdOut[OP_EGL_OUT_DST] = dSync;
	m_pdOut[OP_EGL_OUT_DST] = m_poEnv->Proc();
}

void COpEnvGlider::Update()
{
	COp::Update();
	
	switch( m_uiFuncNum )
	{
	case OP_EGL_FUNC_LIN:
		m_poEnv = &m_oEnvLin;
	break;
	case OP_EGL_FUNC_SIN:
		m_poEnv = &m_oEnvSin;
	break;
	case OP_EGL_FUNC_SIN_POW:
		m_poEnv = &m_oEnvSinPow;
	break;
	}
	
#ifdef OP_USE_RUNTIME_INFO
	const char * aacFuncName_[] = { "l", "s", "sp" };
	SetNameOutput( OP_EGL_OUT_DST, aacFuncName_[m_uiFuncNum] );
#endif // OP_USE_RUNTIME_INFO
	
	UpdateEnvLin();
	UpdateEnvSin();
}

void COpEnvGlider::Reset()
{
	m_dDur = m_dV0 = m_dV1 = m_dSlope = 0.0;
	m_oEnvLin.Reset();
	m_oEnvSin.Reset();
	m_oEnvSinPow.Reset();
	//UpdateEnvLin(); // Achtung: Neu!
	//UpdateEnvSin();
	//UpdateEnvSinPow();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpEnvGlider::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	if( ++m_uiFuncNum >= OP_EGL_FUNC_MAX )
		m_uiFuncNum = 0;
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpEnvGlider, OP_EGL_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpEnvGlider )
OP_GENERIC_METHODS_DEF( COpEnvGlider )

#endif // OP_INC_OP_ENV_GLIDER
