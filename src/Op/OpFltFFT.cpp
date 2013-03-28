// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltFForw.cpp

#include "OpFltFFT.h"
#ifdef OP_INC_OP_FLT_FFT

////////////////////////////////////////////////////////////////////////////////
// COpFltFFT
////////////////////////////////////////////////////////////////////////////////

#define OP_FFF_IN_SRC			0
#define OP_FFF_IN_CLEAR			1
#define OP_FFF_IN_FAC			2
#define OP_FFF_OUT_VAL			0
#define OP_FFF_ROUTINE_EXPAND		0
#define OP_FFF_ROUTINE_SHRINK		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FFF_IN_SRC_INFO		"input"
#define OP_FFF_IN_CLEAR_INFO		"reset"
#define OP_FFF_IN_FAC_INFO		"factor"
#define OP_FFF_OUT_VAL_INFO		"output"
#define OP_FFF_ROUTINE_EXPAND_INFO	"add band"
#define OP_FFF_ROUTINE_SHRINK_INFO	"remove band"
#define OP_FFF_CLASS_INFO \
\
"FFT Filter\n" \
"- Each individual band can be scaled by an arbitrary factor.\n" \
"- Unconnected factor inlets are interpreted as 0.0.\n" \
"- All inlets are processed every cycle."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FFF_IN_SRC_INFO		"Eingang"
#define OP_FFF_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_FFF_IN_FAC_INFO		"Faktor"
#define OP_FFF_OUT_VAL_INFO		"Ausgang"
#define OP_FFF_ROUTINE_EXPAND_INFO	"Band hinzufügen"
#define OP_FFF_ROUTINE_SHRINK_INFO	"Band entfernen"
#define OP_FFF_CLASS_INFO \
\
"FFT Filter\n" \
"- Jedes Band kann mit frei skaliert werden.\n" \
"- Unbesetzte Faktor-Eingänge werden als 0.0 interpretiert.\n" \
"- Alle Eingänge werden immer abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltFFT::COpFltFFT()
: COp( 3, 1 )
, m_uiBufferSize( 1 )
, m_uiBufferPos( 0 )
, m_pdBufferCache( 0 )
, m_pdBufferTmp( 0 )
, m_pdBufferOut( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flFFT" );
	m_poIn[OP_FFF_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_FFF_IN_SRC, "i", OP_FFF_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FFF_IN_CLEAR, "cl", OP_FFF_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FFF_IN_FAC, "f", OP_FFF_IN_FAC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FFF_OUT_VAL, "o", OP_FFF_OUT_VAL_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_FFF_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_FFF_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_FFF_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_FFF_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS );
	Update();
	Reset();
	Validate();
}

COpFltFFT::COpFltFFT( const COpFltFFT &roO )
: COp( 0, 0 )
, m_pdBufferCache( 0 )
, m_pdBufferTmp( 0 )
, m_pdBufferOut( 0 )
{
	Init();
	*this = roO;
	Update();
	Reset();
	Validate();
}

COpFltFFT::~COpFltFFT() 
{
	ClearBuffers();
}

COpFltFFT & COpFltFFT::operator = ( const COpFltFFT &roO )
{
	COp::operator = ( roO ); // m_uiBufferSize, m_uiBufferPos
	Update();
	if( m_pdBufferCache )
		OP_MEM_COPY( m_pdBufferCache, roO.m_pdBufferCache, m_uiBufferSize * sizeof(double) );
	if( m_pdBufferTmp )
		OP_MEM_COPY( m_pdBufferTmp, roO.m_pdBufferTmp, m_uiBufferSize * sizeof(double) );
	if( m_pdBufferOut )
		OP_MEM_COPY( m_pdBufferOut, roO.m_pdBufferOut, m_uiBufferSize * sizeof(double) );
	Validate();
	return *this;
}

void COpFltFFT::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 2 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiBufferSize );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_uiBufferPos );
	COp::Init();
}

void COpFltFFT::ClearBuffers()
{
	OP_DELETE_ARRAY( m_pdBufferCache );
	OP_DELETE_ARRAY( m_pdBufferTmp );
	OP_DELETE_ARRAY( m_pdBufferOut );
}

void COpFltFFT::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_FFF_IN_CLEAR].IsValid() )
		if( m_poIn[OP_FFF_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	m_pdBufferCache[m_uiBufferPos] = m_poIn[OP_FFF_IN_SRC].Proc();
	++m_uiBufferPos;
	
	unsigned int i = OP_FFF_IN_FAC;
	if( m_uiBufferPos >= m_uiBufferSize )
	{
		m_oFFT.FFT( m_pdBufferTmp, m_pdBufferCache );
		double *pdB = m_pdBufferTmp;
		while( i < m_uiCountIn )
		{
			*pdB++ *= ( m_poIn[i].IsValid() ? m_poIn[i].Proc() : 0.0 );
			++i;
		}
		m_oFFT.IFFT( m_pdBufferTmp, m_pdBufferOut );
		m_oFFT.Rescale( m_pdBufferOut );
		m_uiBufferPos = 0;
	}
	else
	{
		while( i < m_uiCountIn )
		{
			if( m_poIn[i].IsValid() )
				m_poIn[i].Proc();
			++i;
		}
	}
	
	m_pdOut[OP_FFF_OUT_VAL] = m_pdBufferOut[m_uiBufferPos];
}

void COpFltFFT::Update()
{
	COp::Update();
	
	if( m_uiBufferSize )
	{
		ClearBuffers();
		m_pdBufferCache  = new double[m_uiBufferSize];
		m_pdBufferTmp = new double[m_uiBufferSize];
		m_pdBufferOut = new double[m_uiBufferSize];
		m_uiBufferPos = 0;
		m_oFFT.Init( m_uiBufferSize );
	}
	else
	{
		m_uiBufferPos = 0;
		m_pdBufferCache = m_pdBufferTmp = m_pdBufferOut = 0;
	}
		
#ifdef OP_USE_RUNTIME_INFO
	for( unsigned int i=OP_FFF_IN_FAC; i<m_uiCountIn; ++i )
	{
		m_poIn[i].SetFlags( OP_LINK_FLAG_OPTIONAL );
		SetNameInput( i, "c" );
		SetInfoInput( i, OP_FFF_IN_FAC_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
}

void COpFltFFT::Reset()
{
	if( m_uiBufferSize )
	{
		if( m_pdBufferCache )
			OP_MEM_ZERO( m_pdBufferCache, m_uiBufferSize * sizeof(double) );
		if( m_pdBufferTmp )
			OP_MEM_ZERO( m_pdBufferTmp, m_uiBufferSize * sizeof(double) );
		if( m_pdBufferOut )
			OP_MEM_ZERO( m_pdBufferOut, m_uiBufferSize * sizeof(double) );
		m_uiBufferPos = 0;
	}
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpFltFFT::Routine( unsigned int uiIndex )
{	
	COp::Routine( uiIndex );
	
	switch( uiIndex )
	{
	case OP_FFF_ROUTINE_SHRINK:
		if( m_uiBufferSize > 1 )
		{
			m_uiBufferSize >>= 1;
			SetCountIn( OP_FFF_IN_FAC + m_uiBufferSize );
		}
	break;
	case OP_FFF_ROUTINE_EXPAND:
		m_uiBufferSize <<= 1;
		SetCountIn( OP_FFF_IN_FAC + m_uiBufferSize );
	break;
	}
	Update();
	Reset();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpFltFFT, OP_FFF_CLASS_INFO )
//OP_GENERIC_COPY_CTOR_DEF( COpFltFFT )
OP_GENERIC_METHODS_DEF( COpFltFFT )

#endif // OP_INC_OP_FLT_FFT
