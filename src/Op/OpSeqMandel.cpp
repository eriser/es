// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqMandel.cpp

#include "OpSeqMandel.h"
#ifdef OP_INC_OP_SEQ_MANDEL

////////////////////////////////////////////////////////////////////////////////
// COpSeqMandel
////////////////////////////////////////////////////////////////////////////////

#define OP_OSM_IN_TRIGG			0
#define OP_OSM_IN_COEFFRE		1
#define OP_OSM_IN_COEFFIM		2
#define OP_OSM_IN_STARTRE		3
#define OP_OSM_IN_STARTIM		4
#define OP_OSM_IN_CLEAR			5
#define OP_OSM_OUT_DST			0
#define OP_OSM_OUT_IM			1
#define OP_OSM_OUT_RE			2
#define OP_OSM_OUT_OVER			3

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_OSM_IN_TRIGG_INFO		"trigger"
#define OP_OSM_IN_COEFFRE_INFO		"coefficient (real part)"
#define OP_OSM_IN_COEFFIM_INFO		"coefficient (imaginary part)"
#define OP_OSM_IN_STARTRE_INFO		"start value (real part)"
#define OP_OSM_IN_STARTIM_INFO		"start value (imaginary part )"
#define OP_OSM_IN_CLEAR_INFO		"reset"
#define OP_OSM_OUT_DST_INFO		"absolute value"
#define OP_OSM_OUT_RE_INFO		"real part"
#define OP_OSM_OUT_IM_INFO		"imaginary part"
#define OP_OSM_OUT_OVER_INFO		"overflow trigger"
#define OP_OSM_CLASS_INFO \
\
"Mandelbrot Sequence\n" \
"- Whenever the trigger input value is greater than 0.0, a new\n" \
"  sequence value will be calculated.\n" \
"- When the reset input value is greater than 0.0, then the internal\n" \
"  sequence variables will be reset to the given start values.\n" \
"- Both start value inlets are only processed when a reset occurs.\n" \
"- For start values unequal to 0.0, Julia sets will be calculated."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_OSM_IN_TRIGG_INFO		"Auslöser"
#define OP_OSM_IN_COEFFRE_INFO		"Realteil des Koeffizienten"
#define OP_OSM_IN_COEFFIM_INFO		"Imaginärteil des Koeffizienten"
#define OP_OSM_IN_STARTRE_INFO		"Realteil des Startwerts"
#define OP_OSM_IN_STARTIM_INFO		"Imaginärteil des Startwerts"
#define OP_OSM_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_OSM_OUT_DST_INFO		"Absolutwert"
#define OP_OSM_OUT_RE_INFO		"Realteil-Ausgang"
#define OP_OSM_OUT_IM_INFO		"Imaginärteil-Ausgang"
#define OP_OSM_OUT_OVER_INFO		"Überlaufs-Auslöser"
#define OP_OSM_CLASS_INFO \
\
"Mandelbrot-Folge\n " \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird eine neuer\n" \
"  Ausgangs-Wert berechnet.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der aktuelle Wert\n" \
"  auf den Startwert gesetzt.\n" \
"- Die Startwert-Eingänge werden nur beim Zurücksetzen abgefragt.\n" \
"- Bei Startwerten ungleich 0.0 werden Julia-Folgen generiert." 

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSeqMandel::COpSeqMandel()
: COp( 6, 4 )
{
	Init();
	OP_SET_NAME_INSTANCE( "sqMdl" );
	m_poIn[OP_OSM_IN_STARTRE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSM_IN_STARTIM].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_OSM_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSM_IN_TRIGG, "tr", OP_OSM_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSM_IN_COEFFRE, "cr", OP_OSM_IN_COEFFRE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSM_IN_COEFFIM, "ci", OP_OSM_IN_COEFFIM_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSM_IN_STARTRE, "sr", OP_OSM_IN_STARTRE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSM_IN_STARTIM, "si", OP_OSM_IN_STARTIM_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_OSM_IN_CLEAR, "cl", OP_OSM_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSM_OUT_DST, "o", OP_OSM_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSM_OUT_RE, "re", OP_OSM_OUT_RE_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSM_OUT_IM, "im", OP_OSM_OUT_IM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_OSM_OUT_OVER, "tr", OP_OSM_OUT_OVER_INFO );
	Reset();
	Validate();
}

COpSeqMandel::~COpSeqMandel()
{}

void COpSeqMandel::Init()
{
	OP_SET_COUNT_INTERNAL_FLT( 4 );
	OP_SET_PTR_INTERNAL_FLT( 0, &m_oSeq.m_tX );
	OP_SET_PTR_INTERNAL_FLT( 1, &m_oSeq.m_tY );
	OP_SET_PTR_INTERNAL_FLT( 2, &m_oSeq.m_tA );
	OP_SET_PTR_INTERNAL_FLT( 3, &m_oSeq.m_tB );
	COp::Init();
}

void COpSeqMandel::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	m_oSeq.m_tA = m_poIn[OP_OSM_IN_COEFFRE].Proc();
	m_oSeq.m_tB = m_poIn[OP_OSM_IN_COEFFIM].Proc();

	if( m_poIn[OP_OSM_IN_CLEAR].IsValid() &&
	    m_poIn[OP_OSM_IN_CLEAR].Proc() > 0.0 )
		Reset();

	double dOver = 0.0;
	if( m_poIn[OP_OSM_IN_TRIGG].Proc() > 0.0 )
	{
		double dOut = m_oSeq.Proc();
		if( OP_IS_VALUE_INVALID( dOut ) )
			dOut = dOver = 1.0;
		m_pdOut[OP_OSM_OUT_DST] = dOut;
		m_pdOut[OP_OSM_OUT_RE] = m_oSeq.m_tX;
		m_pdOut[OP_OSM_OUT_IM] = m_oSeq.m_tY;
	}
	m_pdOut[OP_OSM_OUT_OVER] = dOver;
}

void COpSeqMandel::Reset()
{
	double dX = 0.0, dY = 0.0;
	if( m_poIn[OP_OSM_IN_STARTRE].IsValid() )
		dX = m_poIn[OP_OSM_IN_STARTRE].Proc();
	if( m_poIn[OP_OSM_IN_STARTIM].IsValid() )
		dY = m_poIn[OP_OSM_IN_STARTIM].Proc();
	m_oSeq.m_tX = dX;
	m_oSeq.m_tY = dY;
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpSeqMandel, OP_OSM_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSeqMandel )
OP_GENERIC_METHODS_DEF( COpSeqMandel )

#endif // OP_INC_OP_SEQ_MANDEL
