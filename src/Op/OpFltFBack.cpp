// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltFBack.cpp

#include "OpFltFBack.h"
#ifdef OP_INC_OP_FLT_F_BACK

////////////////////////////////////////////////////////////////////////////////
// COpFltFBack
////////////////////////////////////////////////////////////////////////////////

#define OP_FFB_IN_SRC			0
#define OP_FFB_IN_CLEAR			1
#define OP_FFB_IN_COEFF			2
#define OP_FFB_OUT_VAL			0
#define OP_FFB_OUT_NUM			1
#define OP_FFB_ROUTINE_EXPAND		0
#define OP_FFB_ROUTINE_SHRINK		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FFB_IN_SRC_INFO		"input"
#define OP_FFB_IN_CLEAR_INFO		"reset"
#define OP_FFB_IN_COEFF_INFO		"coefficient"
#define OP_FFB_OUT_VAL_INFO		"output"
#define OP_FFB_OUT_NUM_INFO		"number of taps"
#define OP_FFB_ROUTINE_EXPAND_INFO	"add tap"
#define OP_FFB_ROUTINE_SHRINK_INFO	"add tap"
#define OP_FFB_CLASS_INFO \
\
"Generic Feedback Filter\n" \
"- The structure is equal to the feedback part (denominator) of a\n" \
"  direct-form I IIR filter.\n" \
"- The current input value is not multiplied by any coefficient.\n" \
"- When the reset input value is greater than 0.0, then all the internal\n" \
"  variables will be reinitialized.\n" \
"- An overflow can occur for certain coefficient combinations!"

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FFB_IN_SRC_INFO		"Eingang"
#define OP_FFB_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_FFB_IN_COEFF_INFO		"Koeffizient"
#define OP_FFB_OUT_VAL_INFO		"Ausgang"
#define OP_FFB_OUT_NUM_INFO		"Anzahl Taps"
#define OP_FFB_ROUTINE_EXPAND_INFO	"Tap hinzufügen"
#define OP_FFB_ROUTINE_SHRINK_INFO	"Tap entfernen"
#define OP_FFB_CLASS_INFO \
\
"Generisches Feedback-Filter\n" \
"- Die Struktur entspricht dem Feedback-Teil (Denominator) eines Direct-Form I\n" \
"  IIR-Filters.\n" \
"- Der aktuelle Eingabewert wird mit keinem Koeffizienten multipliziert.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird der Speicher zurück-\n" \
"  gesetzt.\n" \
"- Es kann zu einem Overflow kommen (starkes Feedback). Dann sollten die\n" \
"  Koeffizienten neu gewählt und danach ein Reset ausgeführt werden."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFltFBack::COpFltFBack()
: COp( 3, 2 )
{
	Init();
	OP_SET_NAME_INSTANCE( "flFB" );
	m_poIn[OP_FFB_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_FFB_IN_SRC, "i", OP_FFB_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FFB_IN_CLEAR, "cl", OP_FFB_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FFB_IN_COEFF, "c", OP_FFB_IN_COEFF_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FFB_OUT_VAL, "o", OP_FFB_OUT_VAL_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FFB_OUT_NUM, "n", OP_FFB_OUT_NUM_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_FFB_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_FFB_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_FFB_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_FFB_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS );
	Update();
	Reset();
	Validate();
}

COpFltFBack::~COpFltFBack() 
{
	ClearBuffer();
}

COpFltFBack & COpFltFBack::operator = ( const COpFltFBack & roOp )
{
	COp::operator = ( roOp );
	Update();
	OP_MEM_COPY( m_pdBuffer, roOp.m_pdBuffer, m_uiBufferSize * sizeof(double) );
	m_uiBufferPos = roOp.m_uiBufferPos;
	Validate();
	return *this;
}

void COpFltFBack::Init()
{
	m_pdBuffer = 0;
	m_uiBufferSize = 0;
	COp::Init();
}

void COpFltFBack::ClearBuffer()
{
	if( m_pdBuffer )
	{
		delete [] m_pdBuffer;
		m_pdBuffer = 0;
	}
}

void COpFltFBack::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_FFB_IN_CLEAR].IsValid() )
		if( m_poIn[OP_FFB_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	double dVal = m_poIn[OP_FFB_IN_SRC].Proc();
	unsigned int j = m_uiBufferPos;
	unsigned int i = OP_FFB_IN_COEFF;
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
	
	m_pdBuffer[m_uiBufferPos] = m_pdOut[OP_FFB_OUT_VAL] = dVal;
}

void COpFltFBack::Update()
{
	COp::Update();
	
	const unsigned int uiInSize = m_uiCountIn - 2;
	if( m_uiBufferSize != uiInSize )
	{
		m_uiBufferSize = uiInSize;
		m_pdOut[OP_FFB_OUT_NUM] = double( m_uiBufferSize );
		
		ClearBuffer();
		m_pdBuffer = new double[m_uiBufferSize];
		m_uiBufferPos %= m_uiBufferSize;
		
#ifdef OP_USE_RUNTIME_INFO
		for( unsigned int i=OP_FFB_IN_COEFF; i<m_uiCountIn; ++i )
		{
			SetNameInput( i, "c" );
			SetInfoInput( i, OP_FFB_IN_COEFF_INFO );
		}
#endif // OP_USE_RUNTIME_INFO
	}
}

void COpFltFBack::Reset()
{
	//COp::Reset();
	m_pdOut[OP_FFB_OUT_VAL] = 0.0;

	OP_MEM_ZERO( m_pdBuffer, m_uiBufferSize * sizeof(double) );
	m_uiBufferPos = 0;
}

#ifdef OP_USE_ROUTINES
void COpFltFBack::Routine( unsigned int uiIndex )
{	
	COp::Routine( uiIndex );
	
	switch( uiIndex )
	{
	case OP_FFB_ROUTINE_SHRINK:
	{
		if( m_uiCountIn > 2 )
		{
			m_uiBufferSize = m_uiCountIn - 1;
			SetCountIn( m_uiBufferSize );
		}
	}
	break;
	case OP_FFB_ROUTINE_EXPAND:
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

OP_SET_NAME_AND_INFO_CLASS( COpFltFBack, OP_FFB_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFltFBack )
OP_GENERIC_METHODS_DEF( COpFltFBack )

#endif // OP_INC_OP_FLT_F_BACK
