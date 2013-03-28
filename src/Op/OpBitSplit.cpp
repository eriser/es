// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpBitSplit.cpp

#include "OpBitSplit.h"
#ifdef OP_INC_OP_BIT_SPLIT

////////////////////////////////////////////////////////////////////////////////
// COpBitSplit
////////////////////////////////////////////////////////////////////////////////

#define OP_BSP_IN_SRC			0
#define OP_BSP_IN_BIT			1
#define OP_BSP_OUT_DST			0

#define OP_BSP_ROUTINE_EXPAND		0
#define OP_BSP_ROUTINE_SHRINK		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_BSP_IN_SRC_INFO		"input"
#define OP_BSP_IN_BIT_INFO		"bit index"
#define OP_BSP_OUT_DST_INFO		"output"
#define OP_BSP_ROUTINE_EXPAND_INFO	"add bit"
#define OP_BSP_ROUTINE_SHRINK_INFO	"remove bit"
#define OP_BSP_CLASS_INFO \
\
"BitSplit Operator\n" \
"- The input value is rounded, then its absolute value is used.\n" \
"- Each bit index selects one bit of the input value. If set, the\n" \
"  the corresponding outlet outputs 1.0, otherwise 0.0.\n" \
"- Bit indices get clamped."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_BSP_IN_SRC_INFO		"Eingang"
#define OP_BSP_IN_BIT_INFO		"Bit-Index"
#define OP_BSP_OUT_DST_INFO		"Ausgang"
#define OP_BSP_ROUTINE_EXPAND_INFO	"Bit hinzufügen"
#define OP_BSP_ROUTINE_SHRINK_INFO	"Bit entfernen"
#define OP_BSP_CLASS_INFO \
\
"BitSplit-Operator\n" \
"- Von allen Eingabe-Werten wird der Absolutbetrag genommen und gerundet.\n" \
"- Aus jedem Bit-Index wird jeweils eine Bit-Maske gebildet, welche das\n" \
"  entsprechende Bit auswählt, und falls gesetzt am zugehörigen Ausgang\n" \
"  mit einer 1.0 anzeigt, bzw. mit einer 0.0, falls nicht gesetzt.\n "\
"- Zu grosse Bit-Indizes werden gekappt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpBitSplit::COpBitSplit()
: COp( 2, 1 )
{
	OP_SET_NAME_INSTANCE( "bitSpl" );
	OP_SET_NAME_AND_INFO_INPUT( OP_BSP_IN_SRC, "i", OP_BSP_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_BSP_IN_BIT, "", OP_BSP_IN_BIT_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_BSP_OUT_DST, "", OP_BSP_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_BSP_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_BSP_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_BSP_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_BSP_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS | OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpBitSplit::~COpBitSplit()
{}

void COpBitSplit::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const unsigned int uiIn = (unsigned int) abs( (int)rint( m_poIn[OP_BSP_IN_SRC].Proc() ) );
	unsigned int i = OP_BSP_IN_BIT;
	double *pdO = m_pdOut + OP_BSP_OUT_DST;
	while( i < m_uiCountIn )
	{
		unsigned int uiBit = (unsigned int)abs( (int)rint( m_poIn[i].Proc() ) );
		if( uiBit > sizeof(unsigned int) * 8 )
			uiBit = ( sizeof(unsigned int) * 8 ) - 1;
		*pdO++ = ( uiIn & ( 1 << uiBit ) ? 1.0 : 0.0 );
		++i;
	}
}

void COpBitSplit::Update()
{
	COp::Update();
	
#ifdef OP_USE_RUNTIME_INFO
	unsigned int i = OP_BSP_IN_BIT;
	unsigned int o = OP_BSP_OUT_DST;
	while( i < m_uiCountIn )
	{
		SetNameInput( i, "b" );
		SetInfoInput( i++, OP_BSP_IN_SRC_INFO );
		SetNameOutput( o, "o" );
		SetInfoOutput( o++, OP_BSP_OUT_DST_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
}

#ifdef OP_USE_ROUTINES
void COpBitSplit::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	
	switch( uiIndex )
	{
	case OP_BSP_ROUTINE_SHRINK:
		if( m_uiCountIn > OP_BSP_IN_BIT + 1 )
		{
			SetCountIn( m_uiCountIn - 1 );
			SetCountOut( m_uiCountOut - 1 );
		}
	break;
	case OP_BSP_ROUTINE_EXPAND:
		SetCountIn( m_uiCountIn + 1 );
		SetCountOut( m_uiCountOut + 1 );
	break;
	}
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpBitSplit, OP_BSP_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpBitSplit )
OP_GENERIC_METHODS_DEF( COpBitSplit )

#endif // OP_INC_OP_BIT_SPLIT
