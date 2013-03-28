// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltFForw.cpp

#include "OpFltFForw.h"
#ifdef OP_INC_OP_FLT_F_FORW

////////////////////////////////////////////////////////////////////////////////
// COpFltFForw
////////////////////////////////////////////////////////////////////////////////

#define OP_FFF_IN_SRC			0
#define OP_FFF_IN_CLEAR			1
#define OP_FFF_IN_COEFF			2
#define OP_FFF_OUT_VAL			0
#define OP_FFF_OUT_NUM			1
#define OP_FFF_ROUTINE_EXPAND		0
#define OP_FFF_ROUTINE_SHRINK		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FFF_IN_SRC_INFO		"input"
#define OP_FFF_IN_CLEAR_INFO		"reset"
#define OP_FFF_IN_COEFF_INFO		"coefficient"
#define OP_FFF_OUT_VAL_INFO		"output"
#define OP_FFF_OUT_NUM_INFO		"number of taps"
#define OP_FFF_ROUTINE_EXPAND_INFO	"add tap"
#define OP_FFF_ROUTINE_SHRINK_INFO	"remove tap"
#define OP_FFF_CLASS_INFO \
\
"Generic Feedforward Filter\n" \
"- The structure is equal to the feedforward part (numerator) of a\n" \
"  direct-form I IIR filter (resp. a simple FIR filter).\n" \
"- The current input value is multiplied by the first coefficient.\n" \
"- When the reset input value is greater than 0.0, then all the internal\n" \
"  variables will be reinitialized."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FFF_IN_SRC_INFO		"Eingang"
#define OP_FFF_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_FFF_IN_COEFF_INFO		"Koeffizient"
#define OP_FFF_OUT_VAL_INFO		"Ausgang"
#define OP_FFF_OUT_NUM_INFO		"Anzahl Taps"
#define OP_FFF_ROUTINE_EXPAND_INFO	"Tap hinzufügen"
#define OP_FFF_ROUTINE_SHRINK_INFO	"Tap entfernen"
#define OP_FFF_CLASS_INFO \
\
"Generisches Feedforward-Filter\n" \
"- Die Struktur entspricht dem Feedforward-Teil (Numerator) eines\n" \
"  Direct-Form I IIR-Filters, bzw. einem einfachen FIR-Filter.\n" \
"- Der erste Koeffizient wird mit dem aktuellen Eingabewert mutlipliziert.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der Speicher zurück-\n" \
"  gesetzt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltFForw::COpFltFForw()
: COp( 3, 2 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flFF" );
	m_poIn[OP_FFF_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_FFF_IN_SRC, "i", OP_FFF_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FFF_IN_CLEAR, "cl", OP_FFF_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FFF_IN_COEFF, "c", OP_FFF_IN_COEFF_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FFF_OUT_VAL, "o", OP_FFF_OUT_VAL_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FFF_OUT_NUM, "n", OP_FFF_OUT_NUM_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_FFF_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_FFF_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_FFF_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_FFF_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS );
	Update();
	Reset();
	Validate();
}

COpFltFForw::~COpFltFForw() 
{
	ClearBuffer();
}

COpFltFForw & COpFltFForw::operator = ( const COpFltFForw &roOp )
{
	COp::operator = ( roOp );
	Update();
	OP_MEM_COPY( m_pdBuffer, roOp.m_pdBuffer, m_uiBufferSize * sizeof(double) );
	m_uiBufferPos = roOp.m_uiBufferPos;
	Validate();
	return *this;
}

void COpFltFForw::Init()
{
	m_pdBuffer = 0;
	m_uiBufferSize = 0;
	COp::Init();
}

void COpFltFForw::ClearBuffer()
{
	if( m_pdBuffer )
	{
		delete [] m_pdBuffer;
		m_pdBuffer = 0;
	}
}

void COpFltFForw::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_FFF_IN_CLEAR].IsValid() )
		if( m_poIn[OP_FFF_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	m_pdBuffer[m_uiBufferPos] = m_poIn[OP_FFF_IN_SRC].Proc();
	
	double dVal = 0.0;
	unsigned int j = m_uiBufferPos;
	unsigned int i = OP_FFF_IN_COEFF;
	do
	{
		dVal += m_poIn[i].Proc() * m_pdBuffer[j];
		if( !j )
			j = m_uiBufferSize;
		--j;
		++i;
	}
	while( i < m_uiCountIn );
	++m_uiBufferPos;
	if( m_uiBufferPos == m_uiBufferSize )
		m_uiBufferPos = 0;
	
	m_pdOut[OP_FFF_OUT_VAL] = dVal;
}

void COpFltFForw::Update()
{
	COp::Update();
	
	const unsigned int uiInSize = m_uiCountIn - 2;
	if( m_uiBufferSize != uiInSize )
	{
		m_uiBufferSize = uiInSize;
		m_pdOut[OP_FFF_OUT_NUM] = double( m_uiBufferSize );
		
		ClearBuffer();
		m_pdBuffer = new double[m_uiBufferSize];
		m_uiBufferPos %= m_uiBufferSize;
		
#ifdef OP_USE_RUNTIME_INFO
		for( unsigned int i=OP_FFF_IN_COEFF; i<m_uiCountIn; ++i )
		{
			SetNameInput( i, "c" );
			SetInfoInput( i, OP_FFF_IN_COEFF_INFO );
		}
#endif // OP_USE_RUNTIME_INFO
	}
}

void COpFltFForw::Reset()
{
	//COp::Reset();
	m_pdOut[OP_FFF_OUT_VAL] = 0.0;
	
	OP_MEM_ZERO( m_pdBuffer, m_uiBufferSize * sizeof(double) );
	m_uiBufferPos = 0;
}

#ifdef OP_USE_ROUTINES
void COpFltFForw::Routine( unsigned int uiIndex )
{	
	COp::Routine( uiIndex );
	
	switch( uiIndex )
	{
	case OP_FFF_ROUTINE_SHRINK:
		if( m_uiCountIn > 2 )
		{
			m_uiBufferSize = m_uiCountIn - 1;
			SetCountIn( m_uiBufferSize );
		}
	break;
	case OP_FFF_ROUTINE_EXPAND:
	{
		m_uiBufferSize = m_uiCountIn + 1;
		SetCountIn( m_uiBufferSize );
	}
	break;
	}
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpFltFForw, OP_FFF_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltFForw )
OP_GENERIC_METHODS_DEF( COpFltFForw )

#endif // OP_INC_OP_FLT_F_FORW
