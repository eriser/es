// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMAD.cpp

#include "OpMAD.h"
#ifdef OP_INC_OP_MAD

////////////////////////////////////////////////////////////////////////////////
// COpMAD
////////////////////////////////////////////////////////////////////////////////

#define OP_MAD_IN_SRC		0
#define OP_MAD_IN_FAC		1
#define OP_MAD_IN_SUM		2
#define OP_MAD_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MAD_IN_SRC_INFO	"input"
#define OP_MAD_IN_FAC_INFO	"factor"
#define OP_MAD_IN_SUM_INFO	"addend"
#define OP_MAD_OUT_DST_INFO	"output"
#define OP_MAD_CLASS_INFO \
\
"Multiply/Add\n" \
"- output = input * factor + addend."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MAD_IN_SRC_INFO	"Eingang"
#define OP_MAD_IN_FAC_INFO	"Faktor"
#define OP_MAD_IN_SUM_INFO	"Summand"
#define OP_MAD_OUT_DST_INFO	"Ausgang"
#define OP_MAD_CLASS_INFO \
\
"Multiplikation und Addition\n" \
"- Der Eingangswert wird mit dem Faktor multipliziert und anschliessend\n" \
"  mit dem Summand addiert."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMAD::COpMAD()
: COp( 3, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "mad" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MAD_IN_SRC, "i", OP_MAD_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MAD_IN_FAC, "f", OP_MAD_IN_FAC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MAD_IN_SUM, "s", OP_MAD_IN_SUM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MAD_OUT_DST, "o", OP_MAD_OUT_DST_INFO );
	Validate();
}

COpMAD::~COpMAD()
{}

void COpMAD::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	m_pdOut[OP_MAD_OUT_DST] = 
		  m_poIn[OP_MAD_IN_SRC].Proc()
		* m_poIn[OP_MAD_IN_FAC].Proc()
		+ m_poIn[OP_MAD_IN_SUM].Proc();
}

OP_SET_NAME_AND_INFO_CLASS( COpMAD, OP_MAD_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMAD )
OP_GENERIC_METHODS_DEF( COpMAD )

#endif // OP_INC_OP_MAD
