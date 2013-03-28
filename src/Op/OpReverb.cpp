// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpReverb.cpp

#include "OpReverb.h"
#ifdef OP_INC_OP_REVERB

////////////////////////////////////////////////////////////////////////////////
// COpReverb
////////////////////////////////////////////////////////////////////////////////

#define OP_REV_IN_SRC			0
#define OP_REV_IN_ROOMSIZE		1
#define OP_REV_IN_ROOMDELAY		2
#define OP_REV_IN_FACA			3
#define OP_REV_IN_FACB			4
#define OP_REV_IN_CLEAR			5
#define OP_REV_OUT_DST			0
#define OP_REV_ROUTINE_SIZE		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_REV_IN_SRC_INFO		"input"
#define OP_REV_IN_ROOMSIZE_INFO		"room size"
#define OP_REV_IN_ROOMDELAY_INFO	"release time"
#define OP_REV_IN_FACA_INFO		"allpass factor A"
#define OP_REV_IN_FACB_INFO		"allpass factor B"
#define OP_REV_IN_CLEAR_INFO		"reset"
#define OP_REV_OUT_DST_INFO		"output"
#define OP_REV_CLASS_INFO \
\
"Reverb\n" \
"- The room size parameter defines the approximate number of samples\n" \
"  between two successive echoes.\n" \
"- The release time defines the number of samples until the reverb fades\n" \
"  completely out.\n" \
"- The allpass factors define the filter section (feedback and feedforward\n" \
"  coefficients). By default they are both set to 0.7.\n" \
"- When the reset input value is greater than 0.0, then all filters will be\n" \
"  reinitialized."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_REV_IN_SRC_INFO		"Eingang"
#define OP_REV_IN_ROOMSIZE_INFO		"Raumgrösse"
#define OP_REV_IN_ROOMDELAY_INFO	"Ausklingzeit"
#define OP_REV_IN_FACA_INFO		"Allpass-Faktor A"
#define OP_REV_IN_FACB_INFO		"Allpass-Faktor B"
#define OP_REV_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_REV_OUT_DST_INFO		"Ausgang"
#define OP_REV_CLASS_INFO \
\
"Reverb\n" \
"- Der Raumgrössen-Parameter bestimmt die ungefähre Anzahl Samples zwischen zwei\n" \
"  aufeinanderfolgenden Echos.\n" \
"- Die Ausklingzeit bestimmt die Anzahl Samples, bis der Hall ganz ausklingt.\n" \
"- Die Allpass-Faktoren A und B beeinflussen die Filter-Sektion (Feedback- und\n" \
"  Forward-Koeffizienten). Wenn nicht verbunden sind sie gleich 0.7.\n" \
"- Ist der Wert am Zurücksetzen-Eingang grösser als 0.0, so werden alle\n" \
"  Filter neu initialisiert."

#else
#error language not specified
#endif // OP_LANGUAGE_*

unsigned int COpReverb::m_auiPrime_[OP_REV_STAGES_COMB] = { 2, 3, 5, 7 };

COpReverb::COpReverb()
: COp( 6, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "reverb" );
	m_poIn[OP_REV_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_REV_IN_FACA].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_REV_IN_FACB].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_REV_IN_SRC, "i", OP_REV_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_REV_IN_ROOMSIZE, "rs", OP_REV_IN_ROOMSIZE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_REV_IN_ROOMDELAY, "dl", OP_REV_IN_ROOMDELAY_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_REV_IN_FACA, "a", OP_REV_IN_FACA_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_REV_IN_FACB, "b", OP_REV_IN_FACB_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_REV_IN_CLEAR, "cl", OP_REV_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_REV_OUT_DST, "o", OP_REV_OUT_DST_INFO );
	Update();
	Validate();
}

COpReverb::~COpReverb()
{}

void COpReverb::Init()
{
	InitRoomSize( OP_REV_DELAY_MAX );
	InitRoomDelay( OP_REV_DELAY_MAX );
	Reset();
	COp::Init();
}

void COpReverb::InitRoomSize( double dRoomSize )
{
	m_dRoomSize = dRoomSize;
	unsigned int uiRoomSize = abs( (int)rint( m_dRoomSize ) ) + 1;
	const unsigned int uiMax = OP_MAX( OP_REV_STAGES_COMB, OP_REV_STAGES_ALLP );
	const unsigned int uiRSMax = OP_REV_DELAY_MAX / m_auiPrime_[uiMax-1];
	if( uiRoomSize > uiRSMax )
		uiRoomSize = uiRSMax;
	for( unsigned int i=0; i<OP_REV_STAGES_ALLP; ++i )
		m_aoAllpass[i].SetDelay( uiRoomSize * m_auiPrime_[i] );
	for( unsigned int i=0; i<OP_REV_STAGES_COMB; ++i )
		m_aoComb[i].SetDelay( uiRoomSize * m_auiPrime_[i] );
}

void COpReverb::InitRoomDelay( double dDelay )
{
	m_dRoomDelay = dDelay;
	const double dS = fabs( m_dRoomDelay );
	for( unsigned int i=0; i<OP_REV_STAGES_COMB; ++i )
	{
		m_adCoeffComb[i] = 
			pow( 10.0, ( -3.0 * m_aoComb[i].GetDelay() / dS ));
	}
}

void COpReverb::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_REV_IN_CLEAR].IsValid() )
		if( m_poIn[OP_REV_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	const double dRoomSize = m_poIn[OP_REV_IN_ROOMSIZE].Proc();
	const double dRoomDelay = m_poIn[OP_REV_IN_ROOMDELAY].Proc();
	if( dRoomSize != m_dRoomSize )
		InitRoomSize( dRoomSize );
	if( dRoomDelay != m_dRoomDelay )
		InitRoomDelay( dRoomDelay );
	
	double dA = 0.7, dB = 0.7;
	if( m_poIn[OP_REV_IN_FACA].IsValid() )
		dA = m_poIn[OP_REV_IN_FACA].Proc();
	if( m_poIn[OP_REV_IN_FACB].IsValid() )
		dB = m_poIn[OP_REV_IN_FACB].Proc();
	
	double dIn = m_poIn[OP_REV_IN_SRC].Proc();
	for( unsigned int i=0; i<OP_REV_STAGES_ALLP; ++i )
	{
		const double dOut = m_aoAllpass[i].ReadDelay();
		const double dT = dIn + dOut * dA;	
		m_aoAllpass[i].WriteDelay( dT );
		dIn = dOut - dB * dT;
	}
	
	double dVal = 0.0;
	for( unsigned int i=0; i<OP_REV_STAGES_COMB; ++i )
	{
		const double dT = dIn + m_aoComb[i].ReadDelay() * m_adCoeffComb[i];
		m_aoComb[i].WriteDelay( dT );
		dVal += dT;
	}
	
	m_pdOut[OP_REV_OUT_DST] = dVal * ( 1.0 / OP_REV_STAGES_COMB );
}

void COpReverb::Update()
{
	for( unsigned int i=0; i<OP_REV_STAGES_ALLP; ++i )
		m_aoAllpass[i].SetDelay( m_aoAllpass[i].GetDelay() );
	for( unsigned int i=0; i<OP_REV_STAGES_COMB; ++i )
		m_aoComb[i].SetDelay( m_aoComb[i].GetDelay() );
	COp::Update();
}

void COpReverb::Reset()
{
	unsigned int i;
	for( i=0; i<OP_REV_STAGES_COMB; ++i )
		m_aoComb[i].Reset();
	for( i=0; i<OP_REV_STAGES_ALLP; ++i )
		m_aoAllpass[i].Reset();
	COp::Reset();
	Validate();
}

OP_SET_NAME_AND_INFO_CLASS( COpReverb, OP_REV_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpReverb )
OP_GENERIC_METHODS_DEF( COpReverb )

#endif // OP_INC_OP_REVERB
