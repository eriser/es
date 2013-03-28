// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpBit.cpp

#include "OpBit.h"
#ifdef OP_INC_OP_BIT

////////////////////////////////////////////////////////////////////////////////
// COpBit
////////////////////////////////////////////////////////////////////////////////

#define OP_BIT_IN_SRC0			0
#define OP_BIT_IN_SRC1			1
#define OP_BIT_IN_FAC			2
#define OP_BIT_IN_NUM			3
#define OP_BIT_OUT_DST			0
#define OP_BIT_ROUTINE_FUNC		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_BIT_IN_SRC0_INFO		"first operand"
#define OP_BIT_IN_SRC1_INFO		"second operand"
#define OP_BIT_IN_FAC_INFO		"scale"
#define OP_BIT_IN_NUM_INFO		"operation"
#define OP_BIT_OUT_DST_INFO		"result"
#define OP_BIT_ROUTINE_FUNC_INFO	"operation"
#define OP_BIT_CLASS_INFO \
\
"Bit Operator\n" \
"- Both operands are combined over one of the following bitwise operations:\n" \
"  AND, OR, XOR, NOT, SHL, SHR.\n" \
"- NOT, as an unary operation, only applies to the first operand.\n" \
"- Both operands get multiplied with the value of on the optional\n" \
"  scale inlet before the bitwise calculation and divided by it, afterwards.\n" \
"- The current operation can be selected over the operation inlet, or\n" \
"  the corresponding routine. The inlet has higher priority.\n" \
"- Both operands are treated as integers."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_BIT_IN_SRC0_INFO		"erster Operand"
#define OP_BIT_IN_SRC1_INFO		"zweiter Operand"
#define OP_BIT_IN_FAC_INFO		"Skalierung"
#define OP_BIT_IN_NUM_INFO		"Operation"
#define OP_BIT_OUT_DST_INFO		"Resultat"
#define OP_BIT_ROUTINE_FUNC_INFO	"Operation"
#define OP_BIT_CLASS_INFO \
\
"Bit-Operator\n" \
"- Die Operanden können über die folgenden Bit-Operatoren verknüpft werden:\n" \
"  AND, OR, XOR, NOT, SHL, SHR.\n" \
"- NOT, als unärer Operator, wird nur auf den ersten Operand angewendet.\n" \
"- Beiden Operanden werden jeweils vor der Rechnung mit dem optionalen\n" \
"  Skalierungs-Wert multipliziert, und danach durch diesen dividiert.\n" \
"- Die Operationsauswahl wird per Routine und Input angeboten, dabei wird\n" \
"  der Input präferiert.\n" \
"- Beiden Operanden werden als Integer verrechnet."

#else
#error language not specified
#endif // OP_LANGUAGE_*

#define OP_BIT_FUNC_NUM 6
COpBit::TOpFuncInt2 COpBit::m_atFunc_[] = { 
	COpBit::FuncAND_, 
	COpBit::FuncOR_, 
	COpBit::FuncXOR_,
	COpBit::FuncNOT_,
	COpBit::FuncSAL_,
	COpBit::FuncSAR_ };

COpBit::COpBit()
: COp( 4, 1 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "bit" );
	m_poIn[OP_BIT_IN_FAC].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_BIT_IN_NUM].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_BIT_IN_SRC0, "x", OP_BIT_IN_SRC0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_BIT_IN_SRC1, "y", OP_BIT_IN_SRC1_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_BIT_IN_FAC, "a", OP_BIT_IN_FAC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_BIT_IN_NUM, "n", OP_BIT_IN_NUM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_BIT_OUT_DST, "", OP_BIT_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_BIT_ROUTINE_FUNC, "f" OP_STR_ROUTINE, OP_BIT_ROUTINE_FUNC_INFO );
	Update();
	Validate();
}

COpBit::~COpBit()
{}

void COpBit::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	COp::Init();
}

void COpBit::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	double dX = m_poIn[OP_BIT_IN_SRC0].Proc();
	double dY = m_poIn[OP_BIT_IN_SRC1].Proc();
	
	// Faktor.
	const bool bValidFac = m_poIn[OP_BIT_IN_FAC].IsValid();
	if( bValidFac )
	{
		const double dFac = m_poIn[OP_BIT_IN_FAC].Proc();
		if( m_dFacUp != dFac )
		{
			m_dFacUp = dFac;
			if( dFac )
				m_dFacDown = 1.0 / dFac;
			else
				m_dFacDown = 1.0;
		}
		dX *= m_dFacUp;
		dY *= m_dFacUp;
	}
	
	if( m_poIn[OP_BIT_IN_NUM].IsValid() )
	{
		m_tFunc = m_atFunc_[ 
			abs( (int)rint( m_poIn[OP_BIT_IN_NUM].Proc() ) ) 
			% OP_BIT_FUNC_NUM ];
	}
	
	double dOut = static_cast<double>( 
			m_tFunc( int( dX ), int( dY ) ) );
	
	if( bValidFac )
		dOut *= m_dFacDown;
	
	m_pdOut[OP_BIT_OUT_DST] = dOut;
}

void COpBit::Update()
{
	COp::Update();
	
	m_tFunc = m_atFunc_[m_uiFuncNum];
	
#ifdef OP_USE_RUNTIME_INFO
	static const char * aacFuncName_[] = { "&", "|", "^", "~x", "<<", ">>" };
	if( m_poIn[OP_BIT_IN_NUM].IsValid() )
		SetNameOutput( 0, "o(n)" );
	else
		SetNameOutput( 0, aacFuncName_[m_uiFuncNum] );
#endif // OP_USE_RUNTIME_INF
}

#ifdef OP_USE_ROUTINES
void COpBit::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	
	// Input hat Vortritt!
	if( m_poIn[OP_BIT_IN_NUM].IsValid() )
		return;
	
	if( ++m_uiFuncNum >= OP_BIT_FUNC_NUM )
		m_uiFuncNum = 0;

	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpBit, OP_BIT_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpBit )
OP_GENERIC_METHODS_DEF( COpBit )

#endif // OP_INC_OP_BIT
