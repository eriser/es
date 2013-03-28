// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpClip.cpp

#include "OpClip.h"
#ifdef OP_INC_OP_CLIP

////////////////////////////////////////////////////////////////////////////////
// COpClip
////////////////////////////////////////////////////////////////////////////////

#define OP_CLP_IN_SRC		0
#define OP_CLP_IN_MAX		1
#define OP_CLP_IN_MIN		2
#define OP_CLP_OUT_DST		0

#define OP_CLP_FUNC_CLIP	0
#define OP_CLP_FUNC_FLIP	1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_CLP_IN_SRC_INFO	"input"
#define OP_CLP_IN_MAX_INFO	"max"
#define OP_CLP_IN_MIN_INFO	"min"
#define OP_CLP_OUT_DST_INFO	"output"
#define OP_CLP_ROUTINE_SET_INFO	"mode"
#define OP_CLP_CLASS_INFO \
\
"Clip\n" \
"- Two modes: clip and flip.\n" \
"- In clip mode:\n" \
"    if( input < min )\n" \
"        output = min\n" \
"    if( input > max )\n" \
"        output = max\n" \
"- In flip mode, the input value will be mirrored between the min and\n" \
"  max boundaries until it lies inside the valid range.\n" \
"- Input values remain unchanged, when they lie within [min,max].\n" \
"- If the min value is bigger then the max value, they get swapped.\n" \
"- For invalid inputs -- often caused by a previous overflow -- the output is 0.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_CLP_IN_SRC_INFO	"Eingang"
#define OP_CLP_IN_MAX_INFO	"Maximum"
#define OP_CLP_IN_MIN_INFO	"Minimum"
#define OP_CLP_OUT_DST_INFO	"Ausgang"
#define OP_CLP_ROUTINE_SET_INFO	"Clip-/Flip-Modus"
#define OP_CLP_CLASS_INFO \
\
"Clip\n" \
"- Im Clip-Modus gilt:\n" \
"    wenn( Eingang < Minimum )\n" \
"        Ausgang = Minimum\n" \
"    wenn( Eingang > Maximum )\n" \
"        Ausgang = Maximum\n" \
"- Im Flip-Modus wird das Eingabe-Sample solange an den Minimum-/Maximum-\n" \
"  Grenzen gespiegelt, bis das aktuelle Sample innerhalb [Minimum,Maximum] liegt.\n" \
"- Der Wert am Eingang wird unverändert ausgegeben, wenn er kleiner als das\n" \
"  Maximum, oder grösser als das Minimum ist.\n" \
"- Ist der Maximum-Wert grösser als der Minimum-Wert so werden sie vertauscht.\n" \
"- Bei ungültigen Eingaben - oft verursacht durch einen Overflow - wird 0.0\n" \
"  ausgegeben."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpClip::COpClip()
: COp( 3, 1 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "clip" );
	OP_SET_NAME_AND_INFO_INPUT( OP_CLP_IN_SRC, "i", OP_CLP_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CLP_IN_MAX, "mx", OP_CLP_IN_MAX_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CLP_IN_MIN, "mi", OP_CLP_IN_MIN_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CLP_OUT_DST, "o", OP_CLP_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "f" OP_STR_ROUTINE, OP_CLP_ROUTINE_SET_INFO );
	Validate();
}

COpClip::~COpClip()
{}

void COpClip::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	COp::Init();
}

void COpClip::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();

	double dMax = m_poIn[OP_CLP_IN_MAX].Proc();
	double dMin = m_poIn[OP_CLP_IN_MIN].Proc();
	double dIn = m_poIn[OP_CLP_IN_SRC].Proc();

	if( dMax < dMin )
	{
		const double dTmp = dMax;
		dMax = dMin;
		dMin = dTmp;
	}

	if( OP_IS_VALUE_INVALID( dIn ) )
		dIn = 0.0;
	else if( m_uiFuncNum ) // OP_CLP_FUNC_FLIP
	{
		/*
		while( dIn < dMin || dIn > dMax )
		{
			if( dIn < dMin )
				dIn = dMin + fabs( dIn - dMin );
			else if( dIn > dMax )
				dIn = dMax - fabs( dIn - dMax );
		}
		*/
		const double dThres = fabs( dMax - dMin ) * 0.5;
		const double dDisp = dMin + dThres;
		dIn -= dDisp;
		if( dIn < -dThres || dIn > dThres )
		{
			if( dThres )
				dIn = fabs( fabs( fmod( dIn - dThres, dThres * 4.0 ) ) - dThres * 2.0 ) - dThres;
			else
				dIn = 0.0;
		}
		dIn += dDisp;
	}
	else // OP_CLP_FUNC_CLIP
	{
		if( dIn < dMin )
			dIn = dMin;
		else if( dIn > dMax )
			dIn = dMax;
	}

	m_pdOut[OP_CLP_OUT_DST] = dIn;
}

void COpClip::Update()
{
	COp::Update();
#ifdef OP_USE_RUNTIME_INFO
	SetNameOutput( 0, ( m_uiFuncNum ? "f" : "c" ) );
#endif // OP_USE_RUNTIME_INFO
}

#ifdef OP_USE_ROUTINES
void COpClip::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	m_uiFuncNum = !m_uiFuncNum;
	Update();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpClip, OP_CLP_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpClip )
OP_GENERIC_METHODS_DEF( COpClip )

#endif // OP_INC_OP_CLIP
