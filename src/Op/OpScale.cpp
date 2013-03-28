// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpScale.cpp

#include "OpScale.h"
#ifdef OP_INC_OP_SCALE

////////////////////////////////////////////////////////////////////////////////
// COpScale
////////////////////////////////////////////////////////////////////////////////

#define OP_SCL_IN_BASE			0
#define OP_SCL_IN_INDEX			1
#define OP_SCL_IN_STEP			2
#define OP_SCL_OUT_NUM			0
#define OP_SCL_OUT_DST			1
#define OP_SCL_OUT_EXP			2
#define OP_SCL_OUT_REP			3
#define OP_SCL_ROUTINE_SHRINK_IDX	0
#define OP_SCL_ROUTINE_EXPAND_IDX	1
#define OP_SCL_ROUTINE_SHRINK_STP	2
#define OP_SCL_ROUTINE_EXPAND_STP	3

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_SCL_IN_BASE_INFO		"base"
#define OP_SCL_IN_INDEX_INFO		"index"
#define OP_SCL_IN_STEP_INFO		"step width"
#define OP_SCL_OUT_NUM_INFO		"number of steps"
#define OP_SCL_OUT_DST_INFO		"output"
#define OP_SCL_OUT_EXP_INFO		"exponent"
#define OP_SCL_OUT_REP_INFO		"stage"
#define OP_SCL_ROUTINE_EXPAND_IDX_INFO	"add index"
#define OP_SCL_ROUTINE_SHRINK_IDX_INFO	"remove index"
#define OP_SCL_ROUTINE_EXPAND_STP_INFO	"add step"
#define OP_SCL_ROUTINE_SHRINK_STP_INFO	"remove step"
#define OP_SCL_CLASS_INFO \
\
"Scale\n" \
"- The value with index 0.0 is equal to 0.0.\n" \
"- The value with index 1.0 is equal to the first step width.\n" \
"- The value with index 2.0 is equal to the sum of the first\n" \
"  and the second step width, and so on.\n" \
"- All step width values together build a cyclic scale.\n" \
"- The value with index -1.0 is equal to negated last step width.\n" \
"- The stage output tells the current repetition/transposition\n" \
"  of the base scale.\n" \
"- All index values are rounded to the nearest integer.\n" \
"- All inlets are processed every cycle."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_SCL_IN_BASE_INFO		"Basis"
#define OP_SCL_IN_INDEX_INFO		"Index"
#define OP_SCL_IN_STEP_INFO		"Schrittweite"
#define OP_SCL_OUT_NUM_INFO		"Anzahl Schritte"
#define OP_SCL_OUT_DST_INFO		"Ausgang"
#define OP_SCL_OUT_EXP_INFO		"Exponent"
#define OP_SCL_OUT_REP_INFO		"Stufe"
#define OP_SCL_ROUTINE_EXPAND_IDX_INFO	"Index hinzufügen"
#define OP_SCL_ROUTINE_SHRINK_IDX_INFO	"Index entfernen"
#define OP_SCL_ROUTINE_EXPAND_STP_INFO	"Schritt hinzufügen"
#define OP_SCL_ROUTINE_SHRINK_STP_INFO	"Schritt entfernen"
#define OP_SCL_CLASS_INFO \
\
"Skala\n" \
"- Der Wert mit Index 0.0 ist gleich 0.0.\n" \
"- Der Wert mit Index 1.0 ist gleich der ersten Schrittweite.\n" \
"- Der Wert mit Index 2.0 ist gleich der Summe aus der ersten und\n" \
"  der zweiten Schrittweite, und so weiter.\n" \
"- Die Werte an den Schrittweite-Eingänge werden zyklisch behandelt.\n" \
"- Der Wert mit dem Index -1.0 ist gleich der negierten letzen Schrittweite.\n" \
"- Der Stufe-Ausgang gibt die aktuelle Wiederholung der repetierten Schritt-\n" \
"  weite-Werte an.\n" \
"- Index-Werte werden gerundet.\n" \
"- Alle Eingänge werden immer abgefragt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpScale::COpScale()
: COp( 3, 4 )
, m_uiCountIndex( 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "scale" );
	m_poIn[OP_SCL_IN_BASE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_SCL_IN_BASE, "b", OP_SCL_IN_BASE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SCL_IN_INDEX, "", OP_SCL_IN_INDEX_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SCL_IN_STEP, "", OP_SCL_IN_STEP_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SCL_OUT_NUM, "n", OP_SCL_OUT_NUM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SCL_OUT_DST, "", OP_SCL_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SCL_OUT_EXP, "", OP_SCL_OUT_EXP_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SCL_OUT_REP, "", OP_SCL_OUT_REP_INFO );
	OP_SET_COUNT_ROUTINE( 4 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SCL_ROUTINE_EXPAND_IDX, "i+" OP_STR_ROUTINE, OP_SCL_ROUTINE_EXPAND_IDX_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SCL_ROUTINE_SHRINK_IDX, "i-" OP_STR_ROUTINE, OP_SCL_ROUTINE_SHRINK_IDX_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SCL_ROUTINE_EXPAND_STP, "s+" OP_STR_ROUTINE, OP_SCL_ROUTINE_EXPAND_STP_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SCL_ROUTINE_SHRINK_STP, "s-" OP_STR_ROUTINE, OP_SCL_ROUTINE_SHRINK_STP_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS | OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpScale::~COpScale()
{}

void COpScale::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiCountIndex );
	COp::Init();
}

void COpScale::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	double dSum = 0.0;
	const int iCountStp = (int)( m_uiCountIn - ( OP_SCL_IN_INDEX + m_uiCountIndex ) );
	unsigned int c = m_uiCountIn;
	while( c > OP_SCL_IN_INDEX + m_uiCountIndex )
	{
		--c;
		dSum += m_poIn[c].Proc();
	}
	
	double dBase = OP_SQRT_2_12;
	if( m_poIn[OP_SCL_IN_BASE].IsValid() )
		dBase = m_poIn[OP_SCL_IN_BASE].Proc();
	
	double *pdO = &m_pdOut[OP_SCL_OUT_DST];
	unsigned int j = 0;
	while( j < m_uiCountIndex )
	{
		double dOut = 0.0;
		double dRep = 0.0;

		int iIndex = (int)rint( m_poIn[OP_SCL_IN_INDEX + j].Proc() );
		
		// rem: integer overflow: abs( -2147483648 ) == -2147483648
		if( iIndex == __MAX_SIGNED(int) || iIndex == __MIN_SIGNED(int)  )
			iIndex = 0;
		
		const bool bUp = ( iIndex >= 0 );
		if( abs( iIndex ) >= iCountStp )
		{
			int iRep = iIndex / iCountStp;
			dRep = double( iRep );
			dOut = dRep * dSum;
			iIndex -= iRep * iCountStp;
		}
		
		if( bUp )
		{
			CLink *poD = &m_poIn[OP_SCL_IN_INDEX + m_uiCountIndex];
			int i = 0;
			while( i < iIndex )
				dOut += poD[i++].Proc();
		}
		else
		{
			dRep -= 1.0;
			CLink *poD = &m_poIn[m_uiCountIn];
			
			while( iIndex < 0 )
			{
				--poD;
				dOut -= poD->Proc();
				++iIndex;
			}
		}
		*pdO++ = pow( dBase, dOut );
		*pdO++ = dOut;
		*pdO++ = dRep;
		++j;
	}
}

void COpScale::Update()
{
	m_pdOut[OP_SCL_OUT_NUM] = double( m_uiCountIn - ( OP_SCL_IN_INDEX + m_uiCountIndex ) );
	
#ifdef OP_USE_RUNTIME_INFO
	unsigned int i = OP_SCL_IN_INDEX;
	while( i < OP_SCL_IN_INDEX + m_uiCountIndex )
	{
		SetNameInput( i, "i" );
		SetInfoInput( i++, OP_SCL_IN_INDEX_INFO );
	}
	while( i < m_uiCountIn )
	{
		SetNameInput( i, "s" );
		SetInfoInput( i++, OP_SCL_IN_STEP_INFO );
	}
	i = OP_SCL_OUT_DST;
	while( i < m_uiCountOut )
	{
		SetNameOutput( i, "o" );
		SetInfoOutput( i++, OP_SCL_OUT_DST_INFO );
		SetNameOutput( i, "e" );
		SetInfoOutput( i++, OP_SCL_OUT_EXP_INFO );
		SetNameOutput( i, "r" );
		SetInfoOutput( i++, OP_SCL_OUT_REP_INFO );
	}
	
#endif // OP_USE_RUNTIME_INFO
	COp::Update();
}

#ifdef OP_USE_ROUTINES
void COpScale::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_SCL_ROUTINE_SHRINK_IDX:
	{
		if( m_uiCountIndex <= 1 )
			break;
		--m_uiCountIndex;
		unsigned int i = OP_SCL_IN_INDEX + m_uiCountIndex;
		do
			m_poIn[i] = m_poIn[i+1];
		while( ++i < m_uiCountIn - 1 );
		SetCountIn( m_uiCountIn - 1 );
		SetCountOut( m_uiCountOut - 3 );
	}
	break;
	case OP_SCL_ROUTINE_EXPAND_IDX:
	{
		SetCountIn( m_uiCountIn + 1 );
		SetCountOut( m_uiCountOut + 3 );
		unsigned int i = m_uiCountIn;
		const unsigned int uiMax = OP_SCL_IN_INDEX + m_uiCountIndex;
		while( --i >= uiMax )
			m_poIn[i] = m_poIn[i-1];
		m_poIn[uiMax].Clear();
		++m_uiCountIndex;
	}
	break;
	case OP_SCL_ROUTINE_SHRINK_STP:
		if( m_uiCountIn > m_uiCountIndex + 2 )
			SetCountIn( m_uiCountIn - 1 );
	break;
	case OP_SCL_ROUTINE_EXPAND_STP:
		SetCountIn( m_uiCountIn + 1 );
	break;
	}
	Reset();
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpScale, OP_SCL_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpScale )
OP_GENERIC_METHODS_DEF( COpScale )

#endif // OP_INC_OP_SCALE
