// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpAMU.cpp

#include "OpAMU.h"
#ifdef OP_INC_OP_AMU

////////////////////////////////////////////////////////////////////////////////
// COpAMU
////////////////////////////////////////////////////////////////////////////////

#define OP_AMU_IN_SRC		0
#define OP_AMU_IN_SUM		1
#define OP_AMU_IN_FAC		2
#define OP_AMU_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_AMU_IN_SRC_INFO	"input"
#define OP_AMU_IN_SUM_INFO	"addend"
#define OP_AMU_IN_FAC_INFO	"factor"
#define OP_AMU_OUT_DST_INFO	"output"
#define OP_AMU_CLASS_INFO \
\
"Add/Multiply\n" \
"- output = ( input + addend ) * factor."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_AMU_IN_SRC_INFO	"Eingang"
#define OP_AMU_IN_SUM_INFO	"Summand"
#define OP_AMU_IN_FAC_INFO	"Faktor"
#define OP_AMU_OUT_DST_INFO	"Ausgang"
#define OP_AMU_CLASS_INFO \
\
"Addition und Multiplikation\n" \
"- Der Eingangswert wird mit dem Summanden addiert und anschliessend\n" \
"  wird deren Summe mit dem Faktor multipliziert."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpAMU::COpAMU()
: COp( 3, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "amu" );
	OP_SET_NAME_AND_INFO_INPUT( OP_AMU_IN_SRC, "i", OP_AMU_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_AMU_IN_SUM, "s", OP_AMU_IN_SUM_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_AMU_IN_FAC, "f", OP_AMU_IN_FAC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_AMU_OUT_DST, "o", OP_AMU_OUT_DST_INFO );
	Validate();
}

COpAMU::~COpAMU()
{}

void COpAMU::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	m_pdOut[OP_AMU_OUT_DST] = 
		( m_poIn[OP_AMU_IN_SRC].Proc()
		+ m_poIn[OP_AMU_IN_SUM].Proc() )
		* m_poIn[OP_AMU_IN_FAC].Proc();
}

OP_SET_NAME_AND_INFO_CLASS( COpAMU, OP_AMU_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpAMU )
OP_GENERIC_METHODS_DEF( COpAMU )

#endif // OP_INC_OP_AMU
