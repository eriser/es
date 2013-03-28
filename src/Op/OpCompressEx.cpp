// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpCompressEx.cpp

#include "OpCompressEx.h"
#ifdef OP_INC_OP_COMPRESS_EX

////////////////////////////////////////////////////////////////////////////////
// Simple Source 
////////////////////////////////////////////////////////////////////////////////

// ref: Original Sources:
//
// Simple Source 
// 2006, ChunkWare Music Software, OPEN-SOURCE
//

// USE:
// 1. init envelope state to DC_OFFSET before processing
// 2. add to input before envelope runtime function
const double COpCompressEx::DC_OFFSET = 1.0E-25;
const double COpCompressEx::LOG_2_DB = 8.6858896380650365530225783783321;	// 20 / ln( 10 )
const double COpCompressEx::DB_2_LOG = 0.11512925464970228420089957273422;	// ln( 10 ) / 20

////////////////////////////////////////////////////////////////////////////////
// COpCompressEx::COpCompressEx
////////////////////////////////////////////////////////////////////////////////

COpCompressEx::CEnvelopeDetector::CEnvelopeDetector( double ms ) //, double sampleRate )
{
	//if( sampleRate > 0.0 ) // mod
	//	sampleRate_ = sampleRate; // mod
	if( ms < 0.0 ) // mod
		ms = -ms; // mod
	ms_ = ms;
	setCoef();
}

void COpCompressEx::CEnvelopeDetector::setTc( double ms )
{
	if( ms < 0.0 ) // mod
		ms = -ms; // mod
	ms_ = ms;
	setCoef();
}

//void COpCompressEx::CEnvelopeDetector::setSampleRate( double sampleRate )
//{
//	if( sampleRate > 0.0 ) // mod
//	{
//		sampleRate_ = sampleRate; // mod
//		setCoef(); // mod
//	}
//}

void COpCompressEx::CEnvelopeDetector::setCoef( void )
{
	coef_ = exp( -1000.0 / ( ms_ * OP_SAMPLE_FREQ ) ); //sampleRate_ ) );
}


////////////////////////////////////////////////////////////////////////////////
// COpCompressEx::COpCompressEx
////////////////////////////////////////////////////////////////////////////////

COpCompressEx::CAttRelEnvelope::CAttRelEnvelope( double att_ms, double rel_ms ) //, double sampleRate )
: att_( att_ms ) //, sampleRate )
, rel_( rel_ms ) //, sampleRate )
{
}

void COpCompressEx::CAttRelEnvelope::setAttack( double ms )
{
	att_.setTc( ms );
}

void COpCompressEx::CAttRelEnvelope::setRelease( double ms )
{
	rel_.setTc( ms );
}

//void COpCompressEx::CAttRelEnvelope::setSampleRate( double sampleRate )
//{
//	att_.setSampleRate( sampleRate );
//	rel_.setSampleRate( sampleRate );
//}


////////////////////////////////////////////////////////////////////////////////
// COpCompressEx::CSimpleComp
////////////////////////////////////////////////////////////////////////////////

COpCompressEx::CSimpleComp::CSimpleComp()
: CAttRelEnvelope( 10.0, 100.0 )
, threshdB_( 0.0 )
, ratio_( 1.0 )
, envdB_( DC_OFFSET )
{
}

void COpCompressEx::CSimpleComp::setThresh( double dB )
{
	threshdB_ = dB;
}

void COpCompressEx::CSimpleComp::setThreshLinear( double dThreshold )
{
	threshdB_ = lin2dB( dThreshold );
}

void COpCompressEx::CSimpleComp::setRatio( double ratio )
{
	if( ratio < 0.0 ) // mod
		ratio = -ratio; // mod
	ratio_ = 1.0 / ratio; // mod // CAUTION CAUTION CAUTION. THIS WAS WRONG!!! (Expander instead of Compressor!!)
	//ratio_ = ratio; // mod
}

void COpCompressEx::CSimpleComp::initRuntime( void )
{
	envdB_ = DC_OFFSET;
}


////////////////////////////////////////////////////////////////////////////////
// COpCompressEx::CSimpleCompRms
////////////////////////////////////////////////////////////////////////////////

COpCompressEx::CSimpleCompRms::CSimpleCompRms()
: aveOfSqrs_( DC_OFFSET )
, ave_( 5.0 )
, m_bEnableRMS( true ) // mod
{
}

//void COpCompressEx::CSimpleCompRms::setSampleRate( double sampleRate )
//{
//	COpCompressEx::CSimpleComp::setSampleRate( sampleRate );
//	ave_.setSampleRate( sampleRate );
//}

void COpCompressEx::CSimpleCompRms::setWindow( double ms )
{
	// mod
	if( ms > 0.0 )
	{
		ave_.setTc( ms );
		m_bEnableRMS = true;
	}
	else
	{
		m_bEnableRMS = false;
	}
}

void COpCompressEx::CSimpleCompRms::initRuntime( void )
{
	COpCompressEx::CSimpleComp::initRuntime();
	aveOfSqrs_ = DC_OFFSET;
}


////////////////////////////////////////////////////////////////////////////////
// COpCompressEx
////////////////////////////////////////////////////////////////////////////////

#define OP_CPE_IN_SRC			0
#define OP_CPE_IN_THRESHOLD		1
#define OP_CPE_IN_RATIO			2
#define OP_CPE_IN_ATT			3
#define OP_CPE_IN_REL			4
#define OP_CPE_IN_RMS_WINDOW		5
#define OP_CPE_OUT_GAIN			0
#define OP_CPE_OUT_ENV			1
#define OP_CPE_OUT_ENERGY		2

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_CPE_IN_SRC_INFO		"input"
#define OP_CPE_IN_THRESHOLD_INFO	"threshold"
#define OP_CPE_IN_RATIO_INFO		"slope"
#define OP_CPE_IN_ATT_INFO		"attack"
#define OP_CPE_IN_REL_INFO		"release"
#define OP_CPE_IN_RMS_WINDOW_INFO	"RMS window"
#define OP_CPE_OUT_GAIN_INFO		"gain"
#define OP_CPE_OUT_ENV_INFO		"envelope"
#define OP_CPE_OUT_ENERGY_INFO		"energy"
#define OP_CPE_CLASS_INFO \
\
"Extended Compressor\n" \
"- The gain output value represents the gain factor, and not the\n" \
"  mixed output value! This allows more flexibility and is useful\n" \
"  for sidechaining. In the simplest case, it can be used as\n" \
"  a multiplier to scale the original input signal.\n" \
"- To process a multiple signals, provide the maximum absolute\n" \
"  value of all signals for every cycle.\n" \
"- Available compression modes: PEAK, RMS.\n" \
"- If the RMS window inlet is not connected or if its value is\n" \
"  smaller or equal to 0.0, then the compression mode is RMS,\n" \
"  otherwise automatically PEAK.\n" \
"- The attack and release values are measured in milliseconds\n" \
"  rather than samples.\n" \
"- The energy outlet is only used in RMS mode.\n" \
"- All inlets are processed every cycle."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_CPE_IN_SRC_INFO		"Eingang"
#define OP_CPE_IN_THRESHOLD_INFO	"Threshold"
#define OP_CPE_IN_RATIO_INFO		"Slope"
#define OP_CPE_IN_ATT_INFO		"Attack"
#define OP_CPE_IN_REL_INFO		"Release"
#define OP_CPE_IN_RMS_WINDOW_INFO	"RMS-Window"
#define OP_CPE_OUT_GAIN_INFO		"Gain"
#define OP_CPE_OUT_ENV_INFO		"Hüllkurve"
#define OP_CPE_OUT_ENERGY_INFO		"Energie"
#define OP_CPE_CLASS_INFO \
\
"Erweiterter Kompressor\n" \
"- Der Gain-Ausgabewert repräsentiert den Gain-Faktor, und nicht den\n" \
"  gemischten Ausgabe-Wert! Dies ermöglicht grössere Flexibilität, und\n" \
"  ist nützlich für beliebiges Sidechaining. Im einfachsten Fall, dient\n" \
"  dieser als Multiplikator um das originale Signal zu skalieren.\n" \
"- Um mehrere Signale zu prozessieren, stelle den maximalen Absolutbetrag\n" \
"  aller Signale für jeden Zyklus.\n" \
"- Verfügbare Kompressions-Modus: PEAK, RMS.\n" \
"- Ist der RMS-Window-Eingang nicht verbunden, oder dessen Wert\n" \
"  kleiner oder gleich 0.0, so ist der Kompressions-Modus RMS\n" \
"  aktiv, sonst immer PEAK.\n" \
"- Die Attack- und Release-Werte sind in Millisekunden anzugeben,\n" \
"  anstatt Samples, wie sonst üblich.\n" \
"- Der Energie-Ausgang wird nur im RMS-Modus benützt.\n" \
"- Alle Eingänge werden immer abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpCompressEx::COpCompressEx()
: COp( 6, 3 )
{
	Init();
	OP_SET_NAME_INSTANCE( "cprEx" );
	m_poIn[OP_CPE_IN_RMS_WINDOW].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_CPE_IN_SRC, "i", OP_CPE_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CPE_IN_THRESHOLD, "th", OP_CPE_IN_THRESHOLD_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CPE_IN_RATIO, "ra", OP_CPE_IN_RATIO_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CPE_IN_ATT, "a", OP_CPE_IN_ATT_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CPE_IN_REL, "r", OP_CPE_IN_REL_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CPE_IN_RMS_WINDOW, "w", OP_CPE_IN_RMS_WINDOW_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CPE_OUT_GAIN, "g", OP_CPE_OUT_GAIN_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CPE_OUT_ENV, "ev", OP_CPE_OUT_ENV_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CPE_OUT_ENERGY, "e", OP_CPE_OUT_ENERGY_INFO );
	Update();
	Validate();
}

COpCompressEx::~COpCompressEx()
{}

void COpCompressEx::Init()
{
	//m_oCompressor.setSampleRate( OP_SAMPLE_FREQ );
	Reset();
	OP_SET_COUNT_INTERNAL_FLT( 2 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oCompressor.aveOfSqrs_ );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oCompressor.envdB_ );
	//OP_SET_PTR_INTERNAL_FLT( x, &m_oCompressor.ratio_ );
	//OP_SET_PTR_INTERNAL_FLT( x, &m_oCompressor.threshdB_ );
	//OP_SET_PTR_INTERNAL_FLT( x, &m_oCompressor.att_.coef_ );
	//OP_SET_PTR_INTERNAL_FLT( x, &m_oCompressor.att_.ms_ );
	//OP_SET_PTR_INTERNAL_FLT( x, &m_oCompressor.rel_.coef_ );
	//OP_SET_PTR_INTERNAL_FLT( x, &m_oCompressor.rel_.ms_ );
	//OP_SET_PTR_INTERNAL_FLT( x, &m_oCompressor.ave_.coef_ );
	//OP_SET_PTR_INTERNAL_FLT( x, &m_oCompressor.ave_.ms_ );
	COp::Init();
}

void COpCompressEx::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();

	const double dIn = m_poIn[OP_CPE_IN_SRC].Proc();
	const double dThreshold = m_poIn[OP_CPE_IN_THRESHOLD].Proc();
	const double dRatio = m_poIn[OP_CPE_IN_RATIO].Proc();
	const double dAtt = m_poIn[OP_CPE_IN_ATT].Proc();
	const double dRel = m_poIn[OP_CPE_IN_REL].Proc();
	double dRMSWindow = -1.0; // m_dRMSWindow is 0.0 after Reset().
	if( m_poIn[OP_CPE_IN_RMS_WINDOW].IsValid() )
		dRMSWindow = m_poIn[OP_CPE_IN_RMS_WINDOW].Proc();

	if( dThreshold != m_dThreshold )
	{
		m_dThreshold = dThreshold;
		m_oCompressor.setThreshLinear( dThreshold );
	}
	if( dRatio != m_dRatio )
	{
		m_dRatio = dRatio;
		m_oCompressor.setRatio( dRatio );
	}
	if( dAtt != m_dAtt )
	{
		m_dAtt = dAtt;
		m_oCompressor.setAttack( dAtt );
	}
	if( dRel != m_dRel )
	{
		m_dRel = dRel;
		m_oCompressor.setRelease( dRel );
	}
	if( dRMSWindow != m_dRMSWindow )
	{
		m_dRMSWindow = dRMSWindow;
		m_oCompressor.setWindow( dRMSWindow ); // Automatically switches to Peak-Mode when ( dRMSWindow <= 0.0 ).
	}

	m_pdOut[OP_CPE_OUT_GAIN] = m_oCompressor.calcGainReductionRMS( dIn );
	m_pdOut[OP_CPE_OUT_ENV] = m_oCompressor.getEnv();
	m_pdOut[OP_CPE_OUT_ENERGY] = m_oCompressor.getEnergy();
}

void COpCompressEx::Reset()
{
	m_dThreshold = 0.0;
	m_dRatio = 0.0;
	m_dAtt = 0.0;
	m_dRel = 0.0;
	m_dRMSWindow = 0.0; // Not -1.0! -> Initialize in Proc()!
	m_oCompressor.initRuntime();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpCompressEx, OP_CPE_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpCompressEx )
OP_GENERIC_METHODS_DEF( COpCompressEx )

#endif // OP_INC_OP_COMPRESS_EX
