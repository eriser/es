// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Op.cpp

#include "Op.h"

////////////////////////////////////////////////////////////////////////////////
// COp::CLink
////////////////////////////////////////////////////////////////////////////////

COp::CLink::CLink()
: m_poOp( 0 )
, m_uiIndex( 0 )
, m_uiFlags( OP_LINK_FLAG_NONE )
 {}

COp::CLink::CLink( COp *poOp, unsigned int uiIndex, unsigned int uiFlags )
: m_poOp( poOp )
, m_uiIndex( uiIndex )
, m_uiFlags( uiFlags )
{}

//COp::CLink::CLink( const CLink &roLink )
//{
//	*this = roLink;
//}

COp::CLink::~CLink()
{}

//COp::CLink & COp::CLink::operator=( const CLink &roLink )
//{
//	m_poOp = roLink.m_poOp;
//	m_uiIndex = roLink.m_uiIndex;
//	return *this;
//}

void COp::CLink::Reset()
{
	m_poOp = 0;
	m_uiIndex = 0;
	//m_uiFlags = OP_LINK_FLAG_NONE;
}

void COp::CLink::Clear()
{
	Reset();
	m_uiFlags = OP_LINK_FLAG_NONE;
}

////////////////////////////////////////////////////////////////////////////////
// COp
////////////////////////////////////////////////////////////////////////////////

bool COp::m_bFirst_ = true;
double COp::m_dTime_ = 0.0;
unsigned int COp::m_uiState_ = 0;
unsigned int COp::m_uiIDMax_ = 1; // rem: Die ID 0 ist ungültig!
unsigned int COp::m_uiSignal_ = 0;

COp::COp( unsigned int uiCountIn, unsigned int uiCountOut )
: m_poIn( 0 )
, m_uiCountIn( 0 )
, m_pdOut( 0 )
, m_uiCountOut( 0 )
, m_uiState( 0 )
, m_bIsNotValid( true )
, m_uiID( m_uiIDMax_ )
, m_uiFlags( OP_FLAG_NONE )
#ifdef OP_USE_INTERNALS
, m_uiCountInternalInt( 0 )
, m_uiCountInternalUInt( 0 )
, m_uiCountInternalFlt( 0 )
, m_pptPtrInternalInt( 0 )
, m_pptPtrInternalUInt( 0 )
, m_pptPtrInternalFlt( 0 )
#endif // OP_USE_INTERNALS
#ifdef OP_USE_RUNTIME_INFO
, m_oStrNameInstance( "" )
, m_oArrStrNameIn( uiCountIn )
, m_oArrStrNameOut( uiCountOut )
, m_oArrStrInfoIn( uiCountIn )
, m_oArrStrInfoOut( uiCountOut )
#ifdef OP_USE_ROUTINES // Spezialfall.
, m_oArrStrNameRoutine()
, m_oArrStrInfoRoutine()
#endif // OP_USE_ROUTINES
#endif // OP_USE_RUNTIME_INFO
#ifdef OP_USE_USER_DATA
, m_pvData( 0 )
#endif // OP_USE_USER_DATA
{
	++m_uiIDMax_;
	InitInputs( uiCountIn );
	InitOutputs( uiCountOut );
}

COp::COp( const COp &roOp )
: m_uiID( m_uiIDMax_ )
{
	++m_uiIDMax_;
	*this = roOp;
}

COp::~COp()
{
	ClearInputs();
	ClearOutputs();
	ClearInternals();
	// rem: Hier m_uiIDMax_ nicht dekrementieren!
}

COp & COp::operator = ( const COp &roOp )
{
	ClearInputs();
	ClearOutputs();
	InitInputs( roOp.m_uiCountIn ); // m_uiCountIn / m_ppoOpIn
	InitOutputs( roOp.m_uiCountOut ); // m_uiCountOut / m_pdValOut
	//for( unsigned int i=0; i<m_uiCountIn; ++i )
	//	m_poIn[i] = roOp.m_poIn[i];
	//for( unsigned int i=0; i<m_uiCountOut; ++i )
	//	m_pdOut[i] = roOp.m_pdOut[i];
	if( m_uiCountIn )
		OP_MEM_COPY( m_poIn, roOp.m_poIn, m_uiCountIn * sizeof(CLink) );
	if( m_uiCountOut )
		OP_MEM_COPY( m_pdOut, roOp.m_pdOut, m_uiCountOut * sizeof(double) );
	m_uiState = roOp.m_uiState;
	m_bIsNotValid = roOp.m_bIsNotValid;
	// m_uiID bleibt!
	m_uiFlags = roOp.m_uiFlags;
#ifdef OP_USE_INTERNALS
	/*
	// rem: Kann nur in der spezifischen Klasse im Konstruktur initialisiert werden.
	if( m_uiCountInternalInt != roOp.m_uiCountInternalInt )
		SetCountInternalInt( roOp.m_uiCountInternalInt );
	if( m_uiCountInternalUInt != roOp.m_uiCountInternalUInt )
		SetCountInternalUInt( roOp.m_uiCountInternalUInt );
	if( m_uiCountInternalFlt != roOp.m_uiCountInternalFlt )
		SetCountInternalFlt( roOp.m_uiCountInternalFlt );
	OP_MEM_COPY( m_pptPtrInternalInt, roOp.m_pptPtrInternalInt, m_uiCountInternalInt * sizeof(int *) );
	OP_MEM_COPY( m_pptPtrInternalUInt, roOp.m_pptPtrInternalUInt, m_uiCountInternalUInt * sizeof(unsigned int *) );
	OP_MEM_COPY( m_pptPtrInternalFlt, roOp.m_pptPtrInternalFlt, m_uiCountInternalFlt * sizeof(double *) );
	*/
	for( unsigned int i=0; i<m_uiCountInternalInt; ++i )
		*(m_pptPtrInternalInt[i]) = *(roOp.m_pptPtrInternalInt[i]);
	for( unsigned int i=0; i<m_uiCountInternalUInt; ++i )
		*(m_pptPtrInternalUInt[i]) = *(roOp.m_pptPtrInternalUInt[i]);
	for( unsigned int i=0; i<m_uiCountInternalFlt; ++i )
		*(m_pptPtrInternalFlt[i]) = *(roOp.m_pptPtrInternalFlt[i]);
	
#endif // OP_USE_INTERNALS
#ifdef OP_USE_RUNTIME_INFO
	m_oStrNameInstance = roOp.m_oStrNameInstance;
	m_oArrStrNameIn = roOp.m_oArrStrNameIn;
	m_oArrStrNameOut = roOp.m_oArrStrNameOut;
	m_oArrStrInfoIn = roOp.m_oArrStrInfoIn;
	m_oArrStrInfoOut = roOp.m_oArrStrInfoOut;
#ifdef OP_USE_ROUTINES
	m_oArrStrNameRoutine = roOp.m_oArrStrNameRoutine;
	m_oArrStrInfoRoutine = roOp.m_oArrStrInfoRoutine;
#endif // OP_USE_ROUTINES
#endif // OP_USE_RUNTIME_INFO


#ifdef OP_USE_USER_DATA
	m_pvData = roOp.m_pvData;
#endif // OP_USE_USER_DATA
	return *this;
}

void COp::Proc()
{
	// Sollte nie aufgerufen werden!
}

COp * COp::Clone() const
{
	// Sollte nie aufgerufen werden!
	return 0;
}

void COp::Update()
{}

void COp::Reset()
{
	for( unsigned int i=0; i<m_uiCountOut; ++i )
		m_pdOut[i] = 0.0;
	//m_uiState = 0; // ACHTUNG: unsicher?
}

void COp::Validate()
{
	unsigned int i = m_uiCountIn;
	
	m_bIsNotValid = false;
	while( i )
	{
		--i;
		if( !( m_poIn[i].m_uiFlags & OP_LINK_FLAG_OPTIONAL ) 
		    && !m_poIn[i].IsValid() )
		{
			m_bIsNotValid = true;
			break;
		}
	}
	//Update(); ??
}

#ifdef OP_USE_ROUTINES
void COp::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
}
#endif // OP_USE_ROUTINES

void COp::SetCountIn( unsigned int uiCountIn )
{
	if( ( m_uiFlags & OP_FLAG_DYNAMIC_INPUTS )
	    && uiCountIn && uiCountIn != m_uiCountIn )
	{
		CLink * poInOld = m_poIn;
		unsigned int uiCountInOld = m_uiCountIn;
		unsigned int uiCopyMax = uiCountIn;
		
		InitInputs( uiCountIn );
		
		if( uiCountInOld <= m_uiCountIn )
			uiCopyMax = uiCountInOld;
		
		OP_MEM_COPY( m_poIn, poInOld, uiCopyMax * sizeof(CLink) );
		//while( uiCopyMax )
		//{
		//	--uiCopyMax;
		//	m_poIn[uiCopyMax] = poInOld[uiCopyMax];
		//}
		
		if( poInOld )
			delete [] poInOld;
		
#ifdef OP_USE_RUNTIME_INFO
		m_oArrStrNameIn.Resize( m_uiCountIn );
		m_oArrStrInfoIn.Resize( m_uiCountIn );
#endif // OP_USE_RUNTIME_INFO
	}
}

void COp::SetCountOut( unsigned int uiCountOut )
{
	if( ( m_uiFlags & OP_FLAG_DYNAMIC_OUTPUTS )
	    && uiCountOut && uiCountOut != m_uiCountOut )
	{
		double * pdOutOld = m_pdOut;
		unsigned int uiCountOutOld = m_uiCountOut;
		unsigned int uiCopyMax = uiCountOut;
		
		InitOutputs( uiCountOut );
		
		if( uiCountOutOld <= m_uiCountOut )
			uiCopyMax = uiCountOutOld;
		
		OP_MEM_COPY( m_pdOut, pdOutOld, uiCopyMax * sizeof(double) );
		//while( uiCopyMax )
		//{
		//	--uiCopyMax;
		//	m_pdOut[uiCopyMax] = pdOutOld[uiCopyMax];
		//}
		
		if( pdOutOld )
			delete [] pdOutOld;
		
#ifdef OP_USE_RUNTIME_INFO
		m_oArrStrNameOut.Resize( m_uiCountOut );
		m_oArrStrInfoOut.Resize( m_uiCountOut );
#endif // OP_USE_RUNTIME_INFO
	}
}

COp * COp::Create_()
{
	// Sollte nie aufgerufen werden!
	return 0;
}

#ifdef OP_USE_INTERNALS
void COp::SetCountInternalInt( unsigned int uiCount )
{
	if( m_pptPtrInternalInt )
	{
		delete [] m_pptPtrInternalInt;
		m_pptPtrInternalInt = 0;
	}
	m_uiCountInternalInt = uiCount;
	if( m_uiCountInternalInt )
	{
		m_pptPtrInternalInt = new int *[m_uiCountInternalInt];
	}
}

void COp::SetCountInternalUInt( unsigned int uiCount )
{
	if( m_pptPtrInternalUInt )
	{
		delete [] m_pptPtrInternalUInt;
		m_pptPtrInternalUInt = 0;
	}
	m_uiCountInternalUInt = uiCount;
	if( m_uiCountInternalUInt )
	{
		m_pptPtrInternalUInt = new unsigned int *[m_uiCountInternalUInt];
	}
}

void COp::SetCountInternalFlt( unsigned int uiCount )
{
	if( m_pptPtrInternalFlt )
	{
		delete [] m_pptPtrInternalFlt;
		m_pptPtrInternalFlt = 0;
	}
	m_uiCountInternalFlt = uiCount;
	if( m_uiCountInternalFlt )
	{
		m_pptPtrInternalFlt = new double *[m_uiCountInternalFlt];
	}
}
#endif // OP_USE_INTERNALS

void COp::Prepare_() 
{
	m_dTime_ += OP_SAMPLE_TIME;
	++m_uiState_;
}

unsigned int COp::GetIDMax_() {
	return m_uiIDMax_;
}

void COp::Init()
{
}

void COp::InitInputs( unsigned int uiCountIn )
{
	m_uiCountIn = uiCountIn;
	if( m_uiCountIn )
	{
		m_poIn = new CLink[m_uiCountIn];
	}
	else
	{
		m_poIn = 0;
	}
}

void COp::InitOutputs( unsigned int uiCountOut )
{
	m_uiCountOut = uiCountOut;
	if( m_uiCountOut )
	{
		m_pdOut = new double[m_uiCountOut];
		Reset(); // Alle Outputs auf 0.0 zurücksetzen.
	}
	else
	{
		m_pdOut = 0;
	}
}

void COp::ClearInternals()
{	
#ifdef OP_USE_INTERNALS
	SetCountInternalInt( 0 );
	SetCountInternalUInt( 0 );
	SetCountInternalFlt( 0 );
#endif // OP_USE_INTERNALS
}

void COp::ClearInputs()
{
	if( m_poIn )
	{
		delete [] m_poIn;
		m_poIn = 0;
	}
	m_uiCountIn = 0;
}

void COp::ClearOutputs()
{
	if( m_pdOut )
	{
		delete [] m_pdOut;
		m_pdOut = 0;
	}
	m_uiCountOut = 0;	
}
