// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpValue.cpp

#include "OpValue.h"
#ifdef OP_INC_OP_VALUE

////////////////////////////////////////////////////////////////////////////////
// COpValue
////////////////////////////////////////////////////////////////////////////////

#define OP_VAL_OUT_VAL		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_VAL_OUT_VAL_INFO	"value"
#define OP_VAL_CLASS_INFO \
\
"Value\n" \
"- Stores a arbitrary floating point number."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_VAL_OUT_VAL_INFO	"Wert"
#define OP_VAL_CLASS_INFO \
\
"Wert\n" \
"- Speichert eine beliebige Fliesskommazahl."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpValue::COpValue()
: COp( 0, 1 )
{
	OP_SET_NAME_INSTANCE( "" );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_VAL_OUT_VAL, "v", OP_VAL_OUT_VAL_INFO );
	SetFlags( OP_FLAG_SHOW_OUTPUTS | OP_FLAG_COMPACT );
	Validate();
}

COpValue::~COpValue() 
{}

void COpValue::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
}

void COpValue::Reset()
{}

OP_SET_NAME_AND_INFO_CLASS( COpValue, OP_VAL_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpValue )
OP_GENERIC_METHODS_DEF( COpValue )

#endif // OP_INC_OP_VALUE
