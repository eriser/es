// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpIf.cpp

#include "OpIf.h"
#ifdef OP_INC_OP_IF

////////////////////////////////////////////////////////////////////////////////
// COpIf
////////////////////////////////////////////////////////////////////////////////

#define OP_IF_IN_COND		0
#define OP_IF_IN_X		1
#define OP_IF_IN_Y		2
#define OP_IF_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_IF_IN_COND_INFO	"condition"
#define OP_IF_IN_X_INFO		"first value"
#define OP_IF_IN_Y_INFO		"second value"
#define OP_IF_OUT_DST_INFO	"output"
#define OP_IF_CLASS_INFO \
\
"If Condition\n" \
"- When the condition value is greater than 0.0, then the first value is output,\n" \
"  otherwise the second one.\n" \
"- In each case, the unused input inlet is not going to be processed. That way,\n" \
"  all operators connected to it can be excluded completely from the process.\n" \
"- When unconnected, the second value is assumed to be 0.0."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_IF_IN_COND_INFO	"Kondition"
#define OP_IF_IN_X_INFO		"erste Wert"
#define OP_IF_IN_Y_INFO		"zweiter Wert"
#define OP_IF_OUT_DST_INFO	"Ausgang"
#define OP_IF_CLASS_INFO \
\
"Bedingung\n" \
"- Wenn der Konditionswert grösser als 0.0 ist, wird der erste Wert ausgegeben,\n" \
"  andernfalls der zweite.\n" \
"- Der jeweils ungültige Eingang wird nicht abgefragt. Somit lässt sich der an-\n" \
"  geschlossene Bereich gezielt deaktivieren.\n" \
"- Ist der zweite Eingang nicht besetzt, so wird dieser Wert als 0.0 interpretiert."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpIf::COpIf()
: COp( 3, 1 )
{
	OP_SET_NAME_INSTANCE( "if" );
	m_poIn[OP_IF_IN_Y].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_IF_IN_COND, "c", OP_IF_IN_COND_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_IF_IN_X, "x", OP_IF_IN_X_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_IF_IN_Y, "y", OP_IF_IN_Y_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_IF_OUT_DST, "o", OP_IF_OUT_DST_INFO );
	Validate();
}

COpIf::~COpIf() 
{}

void COpIf::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	double dOut = 0.0;
	if( m_poIn[OP_IF_IN_COND].Proc() > 0.0 )
		dOut = m_poIn[OP_IF_IN_X].Proc();
	else if( m_poIn[OP_IF_IN_Y].IsValid() )
		dOut = m_poIn[OP_IF_IN_Y].Proc();
	m_pdOut[OP_IF_OUT_DST] = dOut;
}

OP_SET_NAME_AND_INFO_CLASS( COpIf, OP_IF_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpIf )
OP_GENERIC_METHODS_DEF( COpIf )

#endif // OP_INC_OP_IF
