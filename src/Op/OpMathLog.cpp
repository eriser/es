// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathLog.cpp

#include "OpMathLog.h"
#ifdef OP_INC_OP_MATH_LOG

////////////////////////////////////////////////////////////////////////////////
// COpMathLog
////////////////////////////////////////////////////////////////////////////////

#define OP_MLG_IN_SRC		0
#define OP_MLG_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MLG_IN_SRC_INFO	"input"
#define OP_MLG_OUT_DST_INFO	"output"
#define OP_MLG_CLASS_INFO \
\
"Logarithm Function\n" \
"- Domain: (0,+inf).\n" \
"- Range: (-inf,+inf).\n" \
"- Hint: arbitrary basis: logn(x) = log(x) / log(n)\n" \
"- If the divisor is equals 0.0, no operation will be executed."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MLG_IN_SRC_INFO	"Eingang"
#define OP_MLG_OUT_DST_INFO	"Ausgang"
#define OP_MLG_CLASS_INFO \
\
"Logarithmus-Funktion\n" \
"- Definitionsbereich: (0,+inf).\n" \
"- Wertebereich: (-inf,+inf).\n" \
"- Hinweis: Beliebige Basis: Logn(x) = Log(x) / Log(n)\n" \
"- Die Funktion wird nur bei gültigem Argument ausgeführt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMathLog::COpMathLog()
: COp( 1, 1 )
{
	OP_SET_NAME_INSTANCE( "log" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MLG_IN_SRC, "i", OP_MLG_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MLG_OUT_DST, "o", OP_MLG_OUT_DST_INFO );
	Validate();
}

COpMathLog::~COpMathLog() 
{}

void COpMathLog::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const double dSrc = m_poIn[OP_MLG_IN_SRC].Proc();
	if( dSrc > 0.0 )
		m_pdOut[OP_MLG_OUT_DST] = log( dSrc );
}

OP_SET_NAME_AND_INFO_CLASS( COpMathLog, OP_MLG_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMathLog )
OP_GENERIC_METHODS_DEF( COpMathLog )

#endif // OP_INC_OP_MATH_LOG
