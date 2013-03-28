// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpInter.cpp

#include "OpInter.h"
#ifdef OP_INC_OP_INTER

////////////////////////////////////////////////////////////////////////////////
// COpInter
////////////////////////////////////////////////////////////////////////////////

#define OP_INR_IN_SRC			0
#define OP_INR_OUT_DST			0
#define OP_INR_ROUTINE_EXPAND_IN	0
#define OP_INR_ROUTINE_SHRINK_IN	1
#define OP_INR_ROUTINE_EXPAND_OUT	2
#define OP_INR_ROUTINE_SHRINK_OUT	3

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_INR_IN_SRC_INFO		"input"
#define OP_INR_OUT_DST_INFO		"output"
#define OP_INR_ROUTINE_EXPAND_IN_INFO	"add input"
#define OP_INR_ROUTINE_SHRINK_IN_INFO	"remove input"
#define OP_INR_ROUTINE_EXPAND_OUT_INFO	"add output"
#define OP_INR_ROUTINE_SHRINK_OUT_INFO	"remove output"
#define OP_INR_CLASS_INFO \
\
"Interface\n" \
"- This operator is used for communication with the es library.\n" \
"- There can be only one instance of this operator."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_INR_IN_SRC_INFO		"Eingang"
#define OP_INR_OUT_DST_INFO		"Ausgang"
#define OP_INR_ROUTINE_EXPAND_IN_INFO	"Eingang hinzufügen"
#define OP_INR_ROUTINE_SHRINK_IN_INFO	"Eingang entfernen"
#define OP_INR_ROUTINE_EXPAND_OUT_INFO	"Ausgang hinzufügen"
#define OP_INR_ROUTINE_SHRINK_OUT_INFO	"Ausgang entfernen"
#define OP_INR_CLASS_INFO \
\
"Interface\n" \
"- Dieser Operator dient der Kommunikation mit der es library.\n" \
"- Von diesem Operator kann gibt es nur eine Instanz."

#else
#error language not specified
#endif // OP_LANGUAGE_*

double *COpInter::m_pdBufferIn_ = 0, *COpInter::m_pdBufferOut_ = 0;
unsigned int COpInter::m_uiBufferInCount_ = 0, COpInter::m_uiBufferOutCount_ = 0;
bool COpInter::m_bInst_ = false;

COpInter::COpInter()
: COp( 1, 1 )
{
	Init();
	OP_SET_NAME_INSTANCE( "inter" );
	m_poIn[OP_INR_IN_SRC].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_INR_IN_SRC, "i", OP_INR_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_INR_OUT_DST, "o", OP_INR_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 4 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_INR_ROUTINE_EXPAND_IN, "+i" OP_STR_ROUTINE, OP_INR_ROUTINE_EXPAND_IN_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_INR_ROUTINE_SHRINK_IN, "-i" OP_STR_ROUTINE, OP_INR_ROUTINE_SHRINK_IN_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_INR_ROUTINE_EXPAND_OUT, "+o" OP_STR_ROUTINE, OP_INR_ROUTINE_EXPAND_OUT_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_INR_ROUTINE_SHRINK_OUT, "-o" OP_STR_ROUTINE, OP_INR_ROUTINE_SHRINK_OUT_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS | OP_FLAG_DYNAMIC_OUTPUTS | OP_FLAG_SINGLETON );
	Update();
	Validate();
}

COpInter::~COpInter() 
{
	ClearBufferIn();
	ClearBufferOut();
	m_uiBufferInCount_ = m_uiBufferOutCount_ = 0;
	m_bInst_ = false;
}

void COpInter::Init()
{
	m_bInst_ = true;
	
	m_pdBufferIn_ = m_pdBufferOut_ = 0;
	m_uiBufferInCount_ = m_uiBufferOutCount_ = 0;
	
	COp::Init();
}

void COpInter::ClearBufferIn()
{
	if( m_pdBufferIn_ )
	{
		delete [] m_pdBufferIn_;
		m_pdBufferIn_ = 0;
	}
}

void COpInter::ClearBufferOut()
{
	if( m_pdBufferOut_ )
	{
		delete [] m_pdBufferOut_;
		m_pdBufferOut_ = 0;
	}
}

void COpInter::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	unsigned int i = 0;
	while( i < m_uiCountIn )
	{
		if( m_poIn[i].IsValid() )
			m_pdBufferIn_[i] = m_poIn[i].Proc();
		++i;
	}
	i = 0;
	while( i < m_uiCountOut )
	{
		m_pdOut[i] = m_pdBufferOut_[i];
		++i;
	}
}

void COpInter::Update()
{
	COp::Update();
	
	if( m_uiBufferInCount_ != m_uiCountIn )
	{
		m_uiBufferInCount_ = m_uiCountIn;
		ClearBufferIn();
		m_pdBufferIn_ = new double[m_uiBufferInCount_];
	}
	if( m_uiBufferOutCount_ != m_uiCountOut )
	{
		m_uiBufferOutCount_ = m_uiCountOut;
		ClearBufferOut();
		m_pdBufferOut_ = new double[m_uiBufferOutCount_];
	}
	Reset();
	
#ifdef OP_USE_RUNTIME_INFO
	for( unsigned int i=OP_INR_IN_SRC; i<m_uiCountIn; ++i )
	{
		SetNameInput( i, "i" );
		SetInfoInput( i, OP_INR_IN_SRC_INFO );
	}
	for( unsigned int i=OP_INR_OUT_DST; i<m_uiCountOut; ++i )
	{
		SetNameOutput( i, "o" );
		SetInfoOutput( i, OP_INR_OUT_DST_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
}

void COpInter::Reset()
{
	COp::Reset();
	if( m_uiBufferInCount_ )
		OP_MEM_ZERO( m_pdBufferIn_, m_uiBufferInCount_ * sizeof(double) );
	if( m_uiBufferOutCount_)
		OP_MEM_ZERO( m_pdBufferOut_, m_uiBufferOutCount_ * sizeof(double) );
}

#ifdef OP_USE_ROUTINES
void COpInter::Routine( unsigned int uiIndex )
{	
	COp::Routine( uiIndex );
	
	switch( uiIndex )
	{
	case OP_INR_ROUTINE_SHRINK_IN:
		if( m_uiCountIn > 1 )
		{
			SetCountIn( m_uiCountIn - 1 );
		}
	break;
	case OP_INR_ROUTINE_EXPAND_IN:
	{
		SetCountIn( m_uiCountIn + 1 );
		m_poIn[m_uiCountIn - 1].SetFlags( OP_LINK_FLAG_OPTIONAL );
	}
	break;
	case OP_INR_ROUTINE_SHRINK_OUT:
	{
		if( m_uiCountOut > 1 )
		{
			SetCountOut( m_uiCountOut - 1 );
		}
	}
	break;
	case OP_INR_ROUTINE_EXPAND_OUT:
	{
		SetCountOut( m_uiCountOut + 1 );
	}
	break;
	}
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpInter, OP_INR_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpInter )
//OP_GENERIC_METHODS_DEF( COpInter )

COp * COpInter::Clone() const {
	return m_bInst_ ? 0 : new COpInter( *this );
}

#ifdef OP_USE_RUNTIME_INFO
const char * COpInter::GetNameClass() const {
	return m_acNameClass_;
}
const char * COpInter::GetInfoClass() const {
	return m_acInfoClass_;
}
#endif // OP_USE_RUNTIME_INFO

COp * COpInter::Create_() {
	return m_bInst_ ? 0 : new COpInter();
}

#endif // OP_INC_OP_INTER
