// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqCell1D.cpp

#include "OpSeqCell1D.h"
#ifdef OP_INC_OP_SEQ_CELL_1D

////////////////////////////////////////////////////////////////////////////////
// COpSeqCell1D
////////////////////////////////////////////////////////////////////////////////

#define OP_SCL_IN_TRIGG		0
#define OP_SCL_IN_COUNT		1
#define OP_SCL_IN_STATES	2
#define OP_SCL_IN_NEIGH		3
#define OP_SCL_IN_SEED		4
#define OP_SCL_IN_CLEAR		5
#define OP_SCL_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_SCL_IN_TRIGG_INFO	"trigger"
#define OP_SCL_IN_COUNT_INFO	"number of cells"
#define OP_SCL_IN_STATES_INFO	"number of states"
#define OP_SCL_IN_NEIGH_INFO	"number of neighbors"
#define OP_SCL_IN_SEED_INFO	"seed"
#define OP_SCL_IN_CLEAR_INFO	"reset"
#define OP_SCL_OUT_DST_INFO	"output"
#define OP_SCL_CLASS_INFO \
\
"1D Cellular Automata Sequence\n" \
"- An arbitrary number of one-dimensional cellular automata can be\n" \
"  parametrized freely.\n" \
"- Whenever the trigger value is greater than 0.0 the state of the\n" \
"  next cell will be written to the output. After all cells were\n" \
"  traversed, a new generation will be generated.\n" \
"- When the reset input value is greater than 0.0, a new initial generation\n" \
"  will be calculated. The same seed value always generates the\n" \
"  same setting.\n" \
"- Three cells, one state and one neighbor are required at the minimum.\n" \
"- Too many neighbors and/or a large number of states quickly consume\n" \
"  large amounts of resources."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_SCL_IN_TRIGG_INFO	"Auslöser"
#define OP_SCL_IN_COUNT_INFO	"Anzahl Zellen"
#define OP_SCL_IN_STATES_INFO	"Anzahl Zustände"
#define OP_SCL_IN_NEIGH_INFO	"Anzahl Nachbarn"
#define OP_SCL_IN_SEED_INFO	"Startwert"
#define OP_SCL_IN_CLEAR_INFO	"Zurücksetzen"
#define OP_SCL_OUT_DST_INFO	"Ausgang"
#define OP_SCL_CLASS_INFO \
\
"1D-Zelluläre-Automaten-Folge\n" \
"- Eine Reihe eindimensionaler zellulärer Automaten können frei parameterisert\n" \
"  werden.\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird der Zustand der\n" \
"  nächsten Zelle ausgegeben. Sobald alle Zellen ausgegeben wurden, wird eine neue\n" \
"  Generation berechnet.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, wird eine neue Initial-Generation\n" \
"  festgelegt. Die verwendete Pseudo-Zufallszahlen-Funktion generiert bei gleichem\n" \
"  Startwert immer die gleich Ausgangssitation.\n" \
"- Es werden mindestens drei Zellen, ein Zustand und ein Nachbar benötigt.\n" \
"- Werden zuviele Nachbarn oder eine zu grosse Anzahl Zustände definiert, so wird\n" \
"  schnell viel Speicher benötigt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSeqCell1D::COpSeqCell1D()
: COp( 6, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "sqCell" );
	m_poIn[OP_SCL_IN_SEED].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_SCL_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_SCL_IN_TRIGG, "tr", OP_SCL_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SCL_IN_COUNT, "c", OP_SCL_IN_COUNT_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SCL_IN_STATES, "st", OP_SCL_IN_STATES_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SCL_IN_NEIGH, "n", OP_SCL_IN_NEIGH_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SCL_IN_SEED, "s", OP_SCL_IN_SEED_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SCL_IN_CLEAR, "cl", OP_SCL_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SCL_OUT_DST, "o", OP_SCL_OUT_DST_INFO );
	Validate();
}

COpSeqCell1D::~COpSeqCell1D()
{}

void COpSeqCell1D::Init()
{
	m_dCount = m_dNeigh = 1.0;
	m_dStates = 2.0;
	m_oSeq.Init( 1, 2, 1 );
	OP_SET_COUNT_INTERNAL_INT( 3 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_oSeq.m_oRand.m_iSeed );
	OP_SET_PTR_INTERNAL_INT( 1, &m_oSeq.m_oRand.m_iS0 );
	OP_SET_PTR_INTERNAL_INT( 2, &m_oSeq.m_oRand.m_iS1 );
	COp::Init();
}

void COpSeqCell1D::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_SCL_IN_CLEAR].IsValid() &&
	    m_poIn[OP_SCL_IN_CLEAR].Proc() > 0.0 )
	    Reset();
	
	const double dCount = m_poIn[OP_SCL_IN_COUNT].Proc();
	const double dStates = m_poIn[OP_SCL_IN_STATES].Proc();
	const double dNeigh = m_poIn[OP_SCL_IN_NEIGH].Proc();
	if( dCount != m_dCount || dStates != m_dStates || dNeigh != m_dNeigh )
	{
		m_dCount = dCount;
		m_dStates = dStates;
		m_dNeigh = dNeigh;
		m_oSeq.Init(
			(unsigned int)abs( (int)rint( dCount ) ),
			(unsigned int)abs( (int)rint( dStates ) ),
			(unsigned int)abs( (int)rint( dNeigh ) ) );
	}
	
	if( m_poIn[OP_SCL_IN_TRIGG].Proc() > 0.0 )
		m_pdOut[OP_SCL_OUT_DST] = double( m_oSeq.Proc() ); 
}

void COpSeqCell1D::Reset()
{
	if( !m_bIsNotValid && m_poIn[OP_SCL_IN_SEED].IsValid() )
		m_oSeq.m_oRand.m_iSeed = (int)rint( m_poIn[OP_SCL_IN_SEED].Proc() );
	m_oSeq.Randomize();
	COp::Reset();
}

OP_SET_NAME_AND_INFO_CLASS( COpSeqCell1D, OP_SCL_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSeqCell1D )
OP_GENERIC_METHODS_DEF( COpSeqCell1D )

#endif // OP_INC_OP_SEQ_CELL_1D
