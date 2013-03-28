// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFader.cpp

#include "OpFader.h"
#ifdef OP_INC_OP_FADER

////////////////////////////////////////////////////////////////////////////////
// COpFader
////////////////////////////////////////////////////////////////////////////////

#define OP_FAD_IN_SRC0		0
#define OP_FAD_IN_SRC1		1
#define OP_FAD_IN_MIX		2
#define OP_FAD_OUT_DST		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FAD_IN_SRC0_INFO	"input 0"
#define OP_FAD_IN_SRC1_INFO	"input 1"
#define OP_FAD_IN_MIX_INFO	"mix"
#define OP_FAD_OUT_DST_INFO	"output"
#define OP_FLT_CLASS_INFO \
\
"Fader\n" \
"- The output value a weighted sum of both input values.\n" \
"- The smaller the mix value, the bigger the influence of input 0.\n" \
"- Mix values are clamped to [-1.0,1.0].\n" \
"- The interpolation is linear."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FAD_IN_SRC0_INFO	"Eingang 0"
#define OP_FAD_IN_SRC1_INFO	"Eingang 1"
#define OP_FAD_IN_MIX_INFO	"Mix"
#define OP_FAD_OUT_DST_INFO	"Ausgang"
#define OP_FLT_CLASS_INFO \
\
"Fader\n" \
"- Zwischen den Werte an den beiden Eingängen wird eine gewichtete Summe\n" \
"  gebildet und ausgegeben.\n" \
"- Je kleiner der Mix-Wert, umso grösser ist der Anteil aus Eingang 0.\n" \
"- Der Mix-Wert wird auf Intervall [-1.0,1.0] gekappt.\n" \
"- Das Verhalten ist linear."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFader::COpFader():
COp( 3, 1 )
{
	OP_SET_NAME_INSTANCE( "fader" );
	OP_SET_NAME_AND_INFO_INPUT( OP_FAD_IN_SRC0, "a", OP_FAD_IN_SRC0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FAD_IN_SRC1, "b", OP_FAD_IN_SRC1_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FAD_IN_MIX, "m", OP_FAD_IN_MIX_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FAD_OUT_DST, "o", OP_FAD_OUT_DST_INFO );
	SetFlags( OP_FLAG_COMBINATIVE );
	Validate();
}

COpFader::~COpFader() 
{}

void COpFader::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	double dRatio = m_poIn[OP_FAD_IN_MIX].Proc();
	OP_RANGE_MINMAX( dRatio, -1.0, +1.0 );
	dRatio += 1.0;
	dRatio *= 0.5;
	const double dSrc0 = m_poIn[OP_FAD_IN_SRC0].Proc();
	const double dSrc1 = m_poIn[OP_FAD_IN_SRC1].Proc();
	m_pdOut[OP_FAD_OUT_DST] = dRatio * ( dSrc1 - dSrc0 ) + dSrc0;
	
}

OP_SET_NAME_AND_INFO_CLASS(COpFader, OP_FLT_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFader )
OP_GENERIC_METHODS_DEF( COpFader )

#endif // OP_INC_OP_CLIP
