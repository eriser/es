// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqColl1D.cpp

#include "OpSeqColl1D.h"
#ifdef OP_INC_OP_SEQ_COLL_1D

////////////////////////////////////////////////////////////////////////////////
// COpSeqColl1D
////////////////////////////////////////////////////////////////////////////////

#define OP_CLL_IN_TRIGG			0
#define OP_CLL_IN_DAMP			1
#define OP_CLL_IN_SEED			2
#define OP_CLL_IN_CLEAR			3
#define OP_CLL_OUT_TRIGG		0
#define OP_CLL_OUT_INDEX		1

#define OP_CLL_ROUTINE_SHRINK		0
#define OP_CLL_ROUTINE_EXPAND		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_CLL_IN_TRIGG_INFO		"trigger"
#define OP_CLL_IN_DAMP_INFO		"damp"
#define OP_CLL_IN_SEED_INFO		"seed"
#define OP_CLL_IN_CLEAR_INFO		"reset"
#define OP_CLL_OUT_TRIGG_INFO		"trigger"
#define OP_CLL_OUT_INDEX_INFO		"index"
#define OP_CLL_ROUTINE_EXPAND_INFO	"add object"
#define OP_CLL_ROUTINE_SHRINK_INFO	"remove object"
#define OP_CLL_CLASS_INFO \
\
"1D Collision Sequence\n" \
"- Differently sized objects are placed side by side on a rail with\n" \
"  fixed walls at the end on each side.\n" \
"- Whenever the trigger input value is greater than 0.0, each object\n" \
"  will be moved according to its current attributes.\n" \
"- Objects can only move strictly to the left or to the right.\n" \
"- Objects always change their movement direction and velocity after a\n" \
"  collision with each other or with a wall.\n" \
"- After a collision, the new speed value of each object gets scaled\n" \
"  by the global damp parameter.\n" \
"- When the reset input value is greater than 0.0, new attributes will\n" \
"  be assigned to all objects. The same seed value always generates the\n" \
"  same setting.\n" \
"- When a collision happens, the corresponding trigger output value is\n" \
"  1.0 and the index output value tells the index of the left object,\n" \
"  otherwise the output values are constantly 0.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_CLL_IN_TRIGG_INFO		"Auslöser"
#define OP_CLL_IN_DAMP_INFO		"Dämpfung"
#define OP_CLL_IN_SEED_INFO		"Startwert"
#define OP_CLL_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_CLL_OUT_TRIGG_INFO		"Auslöser"
#define OP_CLL_OUT_INDEX_INFO		"Index"
#define OP_CLL_ROUTINE_EXPAND_INFO	"Objekt hinzufügen"
#define OP_CLL_ROUTINE_SHRINK_INFO	"Objekt entfernen"
#define OP_CLL_CLASS_INFO \
\
"1D-Kollisions-Sequenz\n" \
"- Verschieden grosse Objekte befinden sich neben einander auf einer Schiene\n" \
"  mit festen Wänden an beiden Enden.\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so wird jedes Objekt\n" \
"  verschoben.\n" \
"- Die Objekte können sich nur nach links oder rechts bewegen, und ändern ihre\n" \
"  Bewegungsgeschwindigkeit und -richtung nach jedem Zusammenstoss mit einem\n" \
"  Nachbarobjekt oder einer Wand.\n" \
"- Die Dämpfung wird bei jeder Kollision mit den Geschwindigkeiten multipliziert.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird jedes Objekt neu platziert\n" \
"  und bekommt eine neue Masse zugewiesen. Die verwendete Pseudo-Zufalls-\n" \
"  zahlen-Funktion generiert bei gleichem Startwert immer die gleich Ausgangs-\n" \
"  sitation.\n" \
"- Bei einer Kollision steht der entsprechende Auslöser-Ausgang auf 1.0 und am\n" \
"  zugehörigen Index-Ausgang wird der Index des jeweils linken Objekts angezeigt.\n"

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSeqColl1D::COpSeqColl1D()
: COp( 4, 2 )
{
	Init();
	OP_SET_NAME_INSTANCE( "sqColl" );
	m_poIn[OP_CLL_IN_SEED].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_CLL_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_CLL_IN_TRIGG, "tr", OP_CLL_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CLL_IN_DAMP, "d", OP_CLL_IN_DAMP_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CLL_IN_SEED, "s", OP_CLL_IN_SEED_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CLL_IN_CLEAR, "cl", OP_CLL_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CLL_OUT_TRIGG, "", OP_CLL_OUT_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CLL_OUT_INDEX, "", OP_CLL_OUT_INDEX_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_CLL_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_CLL_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_CLL_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_CLL_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpSeqColl1D::~COpSeqColl1D()
{}

void COpSeqColl1D::Init()
{
	OP_SET_COUNT_INTERNAL_INT( 3 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_oSeq.m_oRand.m_iSeed );
	OP_SET_PTR_INTERNAL_INT( 1, &m_oSeq.m_oRand.m_iS0 );
	OP_SET_PTR_INTERNAL_INT( 2, &m_oSeq.m_oRand.m_iS1 );
	COp::Init();
}

void COpSeqColl1D::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_CLL_IN_CLEAR].IsValid()
	&&  m_poIn[OP_CLL_IN_CLEAR].Proc() > 0.0 )
		Reset();
	
	m_oSeq.m_tDamp = m_poIn[OP_CLL_IN_DAMP].Proc();
	
	double *pdO = &m_pdOut[OP_CLL_OUT_TRIGG];
	if( m_poIn[OP_CLL_IN_TRIGG].Proc() > 0.0 )
	{
		m_oSeq.Proc();
		
		for( unsigned int i=0; i<m_oSeq.GetCount(); ++i )
		{
			if( m_oSeq.m_pbCollision[i] )
			{
				*pdO++ = 1.0;
				*pdO++ = double( i );
			}
			else
			{
				*pdO++ = 0.0;
				*pdO++ = 0.0;
			}
		}
	}
	else
	{
		const double *pdOMax = m_pdOut + m_uiCountOut;
		while( pdO < pdOMax )
		{
			*pdO++ = 0.0;
			++pdO;
		}
	}
}

void COpSeqColl1D::Update()
{
	const unsigned int uiNum = ( m_uiCountOut - OP_CLL_OUT_TRIGG ) / 2;
	const unsigned int uiCoutParticle = m_oSeq.GetCount();
	if( uiCoutParticle != uiNum )
		m_oSeq.SetCount( uiNum );
	
#ifdef OP_USE_RUNTIME_INFO
	unsigned int o = OP_CLL_OUT_TRIGG;
	while( o < m_uiCountOut )
	{
		SetNameOutput( o, "tr" );
		SetInfoOutput( o++, OP_CLL_OUT_TRIGG_INFO );
		SetNameOutput( o, "i" );
		SetInfoOutput( o++, OP_CLL_OUT_INDEX_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
	COp::Update();
}

void COpSeqColl1D::Reset()
{
	if( !m_bIsNotValid && m_poIn[OP_CLL_IN_SEED].IsValid() )
		m_oSeq.m_oRand.m_iSeed = (int)rint( m_poIn[OP_CLL_IN_SEED].Proc() );
	m_oSeq.Reset();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpSeqColl1D::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_CLL_ROUTINE_SHRINK:
		if( m_uiCountOut > OP_CLL_OUT_INDEX + 1 )
			SetCountOut( m_uiCountOut - 2 );
	break;
	case OP_CLL_ROUTINE_EXPAND:
		SetCountOut( m_uiCountOut + 2 );
	break;
	}
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpSeqColl1D, OP_CLL_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSeqColl1D )
OP_GENERIC_METHODS_DEF( COpSeqColl1D )

#endif // OP_INC_OP_SEQ_COLL_1D
