// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpCompress.cpp

#include "OpCompress.h"
#ifdef OP_INC_OP_COMPRESS

////////////////////////////////////////////////////////////////////////////////
// COpCompress
////////////////////////////////////////////////////////////////////////////////

#define OP_CMP_IN_SRC			0
#define OP_CMP_IN_THRESHOLD		1
#define OP_CMP_IN_SLOPE			2
#define OP_CMP_IN_ATT			3
#define OP_CMP_IN_REL			4
#define OP_CMP_OUT_DST			0
#define OP_CMP_OUT_ENV			1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_CMP_IN_SRC_INFO		"input"
#define OP_CMP_IN_THRESHOLD_INFO	"threshold"
#define OP_CMP_IN_SLOPE_INFO		"slope"
#define OP_CMP_IN_ATT_INFO		"attack"
#define OP_CMP_IN_REL_INFO		"release"
#define OP_CMP_OUT_DST_INFO		"output"
#define OP_CMP_OUT_ENV_INFO		"envelope"
#define OP_CMP_CLASS_INFO \
\
"Compressor\n" \
"- When the input value exceeds the threshold value, the compression\n" \
"  envelope will be triggered.\n" \
"- The attack and release values define the responsiveness of the envelope.\n" \
"- The envelope output always lies between 0.0 and 1.0, and shows the\n" \
"  current state of the compression envelope.\n" \
"- The slope value sets the strength of the compression.\n" \
"- The signal will be delayed a few milliseconds."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_CMP_IN_SRC_INFO		"Eingang"
#define OP_CMP_IN_THRESHOLD_INFO	"Threshold"
#define OP_CMP_IN_SLOPE_INFO		"Slope"
#define OP_CMP_IN_ATT_INFO		"Attack"
#define OP_CMP_IN_REL_INFO		"Release"
#define OP_CMP_OUT_DST_INFO		"Ausgang"
#define OP_CMP_OUT_ENV_INFO		"Hüllkurve"
#define OP_CMP_CLASS_INFO \
\
"Kompressor\n" \
"- Sobald der Wert am Eingang den Threshold-Wert übersteigt, wird die\n" \
"  Kompressions-Hüllkurve ausgelöst.\n" \
"- Die Werte am Attack- und Release-Eingang bestimmen das Zeitverhalten.\n" \
"- Der Hüllkurvenausgang gibt mit Werten zwischen 0.0 und 1.0 den\n" \
"  aktuellen Ausschlag der Kompressions-Hüllkurve an.\n" \
"- Der Wert am Slope-Eingang gibt die Stärke der Kompression an.\n" \
"- Das Signal wird um ein paar Millisekunden verzögert."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpCompress::COpCompress()
: COp( 5, 2 )
{
	Init();
	OP_SET_NAME_INSTANCE( "compr" );
	OP_SET_NAME_AND_INFO_INPUT( OP_CMP_IN_SRC, "i", OP_CMP_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CMP_IN_THRESHOLD, "th", OP_CMP_IN_THRESHOLD_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CMP_IN_SLOPE, "s", OP_CMP_IN_SLOPE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CMP_IN_ATT, "a", OP_CMP_IN_ATT_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CMP_IN_REL, "r", OP_CMP_IN_REL_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CMP_OUT_DST, "o", OP_CMP_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CMP_OUT_ENV, "ev", OP_CMP_OUT_ENV_INFO );
	Update();
	Validate();
}

COpCompress::~COpCompress()
{}

void COpCompress::Init()
{
	m_dA = 1.0;
	m_dR = 1.0;
	SetAttRel( m_dAtt, m_dA );
	SetAttRel( m_dRel, m_dR );
	Reset();
	OP_SET_COUNT_INTERNAL_FLT( 1 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_dEnv );
	COp::Init();
}

void COpCompress::SetAttRel( double &rdDst, double dSrc )
{
	rdDst = ( dSrc == 0.0 ) ? ( 0.0 ) : ( exp( -1.0 / fabs(dSrc) ) );
}

void COpCompress::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	const double dIn = m_poIn[OP_CMP_IN_SRC].Proc();
	m_dSqSum += dIn * dIn;
	double dVal = m_oDelay.ReadDelay();
	m_oDelay.WriteDelay( dIn );
	m_dSqSum -= dVal * dVal;
	
	const double dRMS = sqrt( ( m_dSqSum + 0.0000001 ) * (1.0 / (OP_CMP_BUFFER_SIZE + 1) ) );
	const double dThreshold = m_poIn[OP_CMP_IN_THRESHOLD].Proc();
	
	const double dAtt = m_poIn[OP_CMP_IN_ATT].Proc();
	const double dRel = m_poIn[OP_CMP_IN_REL].Proc();
	if( dAtt != m_dA )
	{
		m_dA = dAtt;
		SetAttRel( m_dAtt, dAtt );
	}
	if( dRel != m_dR )
	{
		m_dR = dRel;
		SetAttRel( m_dRel, dRel );
	}
	
	// dynamic selection: attack or release?
	const double dTheta = ( dRMS > m_dEnv ) ? ( m_dAtt ) : ( m_dRel );
	
	// smoothing with capacitor, envelope extraction...
	// here be aware of pIV denormal numbers glitch
	m_dEnv = ( 1.0 - dTheta ) * dRMS + dTheta * m_dEnv;
	
	// the very easy hard knee 1:N compressor
	double dGain = 1.0;
	if( m_dEnv > dThreshold )
		dGain = dGain - ( m_dEnv - dThreshold ) * m_poIn[OP_CMP_IN_SLOPE].Proc();
	
	m_pdOut[OP_CMP_OUT_DST] = dVal * dGain;
	m_pdOut[OP_CMP_OUT_ENV] = m_dEnv;
}

void COpCompress::Reset()
{
	m_oDelay.Reset();
	m_dSqSum = 0.0;
	m_dEnv = 0.0;
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpCompress, OP_CMP_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpCompress )
OP_GENERIC_METHODS_DEF( COpCompress )

#endif // OP_INC_OP_COMPRESS
