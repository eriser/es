// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTransFFT.cpp

#include "OpTransFFT.h"
#ifdef OP_INC_OP_TRANS_FFT

////////////////////////////////////////////////////////////////////////////////
// COpTransFFT
////////////////////////////////////////////////////////////////////////////////

#define OP_TFF_IN_SRC			0
#define OP_TFF_IN_RST			1
#define OP_TFF_OUT_DST			0
#define OP_TFF_OUT_SYNC			1
#define OP_TFF_OUT_NUM			2
#define OP_TFF_ROUTINE_SIZE		0
#define OP_TFF_ROUTINE_FUNC		1

#define OP_TFF_SIZE_MIN			2
#define OP_TFF_SIZE_MAX			4096

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_TFF_IN_SRC_INFO		"input"
#define OP_TFF_IN_RST_INFO		"reset"
#define OP_TFF_OUT_DST_INFO		"output"
#define OP_TFF_OUT_SYNC_INFO		"sync"
#define OP_TFF_OUT_NUM_INFO		"buffer size"
#define OP_TFF_ROUTINE_SIZE_INFO	"size"
#define OP_TFF_ROUTINE_FUNC_INFO	"mode"
#define OP_TFF_CLASS_INFO \
\
"FFT Transformer\n" \
"- The mode routine selects between FFT and IFFT calculation method.\n" \
"- The size routine doubles the buffer size.\n" \
"- A signal remains unchanged, when passing first through a FFT transformer\n" \
"  with FFT behavior, and afterwards directly trough one with IFFT behavior,\n" \
"  of the same size. Needed connections: output - input / sync - reset.\n" \
"- After a FFT transformation cycle has been completed, the sync output value\n" \
"  is 1.0, otherwise always 0.0 (in IFFT mode always).\n" \
"- The FFT transformation is executed whenever the internal buffer is full.\n" \
"- The IFFT transformation is executed whenever the reset input value\n" \
"  is greater than 0.0.\n" \
"- FFT is marked as > and IFFT as < on the output outlet."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_TFF_IN_SRC_INFO		"Eingang"
#define OP_TFF_IN_RST_INFO		"Reset"
#define OP_TFF_OUT_DST_INFO		"Ausgang"
#define OP_TFF_OUT_SYNC_INFO		"Synchronisation"
#define OP_TFF_OUT_NUM_INFO		"Puffer-Grösse"
#define OP_TFF_ROUTINE_SIZE_INFO	"Grössenauswahl"
#define OP_TFF_ROUTINE_FUNC_INFO	"Funktion"
#define OP_TFF_CLASS_INFO \
\
"FFT-Tranformator\n" \
"- Über die Funktionsroutine kann zwischen FFT- und IFFT-Verhalten gewählt\n" \
"  werden.\n" \
"- Die Grössen-Routine verdoppelt die Grösse des Puffers jeweils.\n" \
"- Ein direktes Nacheinanderschalten eines FFT- und eines IFFT-Transformator, \n" \
"  lässt das Signal unverändert passieren. Dabei muss neben dem Aus- und Eingang\n" \
"  der Synchronisationsausgang mit dem Reset-Eingang verbunden, und bei beiden\n" \
"  Tranformatoren dieselbe Puffer-Grösse gewählt werden.\n" \
"- Bei abgeschlossener FFT-Transformation steht der Synchronisationsausgang\n" \
"  auf 1.0, sonst immer auf 0.0 (im IFFT-Modus immer).\n" \
"- Die FFT-Transformation wird ausgeführt, sobald der interne Puffer voll ist.\n" \
"- Die IFFT-Transformation wird ausgeführt, sobald der Wert des Reset-Eingangs\n" \
"  grösser als 0.0 ist.\n" \
"- FFT wird am Ausgang als > und IFFT als < angezeigt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpTransFFT::COpTransFFT()
: COp( 2, 3 )
, m_uiBufferSize( OP_TFF_SIZE_MIN )
, m_uiBufferPos( 0 )
, m_uiFuncNum( 0 )
, m_pdBufferSrc( 0 )
, m_pdBufferDst( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "trsFFT" );
	m_poIn[OP_TFF_IN_RST].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_TFF_IN_SRC, "i", OP_TFF_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_TFF_IN_RST, "cl", OP_TFF_IN_RST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TFF_OUT_DST, "o", OP_TFF_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TFF_OUT_SYNC, "sy", OP_TFF_OUT_SYNC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TFF_OUT_NUM, "n", OP_TFF_OUT_NUM_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( 0, "s" OP_STR_ROUTINE, OP_TFF_ROUTINE_SIZE_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( 1, "f" OP_STR_ROUTINE, OP_TFF_ROUTINE_FUNC_INFO );
	Update();
	Reset();
	Validate();
}

COpTransFFT::COpTransFFT( const COpTransFFT &roO )
: COp( 0, 0 )
, m_pdBufferSrc( 0 )
, m_pdBufferDst( 0 )
{
	Init();
	*this = roO;
	Update();
	Reset();
	Validate();
}

COpTransFFT::~COpTransFFT()
{
	ClearBuffers();
}

COpTransFFT & COpTransFFT::operator = ( const COpTransFFT &roO )
{
	COp::operator = ( roO ); // m_uiBufferSize, m_uiBufferPos, m_uiFuncNum
	Update();
	if( m_pdBufferDst )
		OP_MEM_COPY( m_pdBufferDst, roO.m_pdBufferDst, m_uiBufferSize * sizeof(double) );
	if( m_pdBufferSrc )
		OP_MEM_COPY( m_pdBufferSrc, roO.m_pdBufferSrc, m_uiBufferSize * sizeof(double) );
	Validate();
	return *this;
}

void COpTransFFT::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 3 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiBufferSize );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_uiBufferPos );
	OP_SET_PTR_INTERNAL_UINT( 2, &m_uiFuncNum );
	COp::Init();
}

void COpTransFFT::ClearBuffers()
{
	OP_DELETE_ARRAY( m_pdBufferSrc );
	OP_DELETE_ARRAY( m_pdBufferDst );
}

void COpTransFFT::Proc()
{	
	OP_GENERIC_CODE_PROC_PROLOG();
	
	m_pdBufferSrc[m_uiBufferPos] = m_poIn[OP_TFF_IN_SRC].Proc();
	m_pdOut[OP_TFF_OUT_DST] = m_pdBufferDst[m_uiBufferPos];
	++m_uiBufferPos;
	if( m_uiBufferPos >= m_uiBufferSize )
	{
		if( !m_uiFuncNum )
		{
			m_oFFT.FFT( m_pdBufferDst, m_pdBufferSrc );
			
			/*
			if( ::GetAsyncKeyState( VK_SPACE ) )
			{
				printf( "%d\n", m_uiBufferSize );
				printf( "fft:\n" );
				printf( "- src: " );
				for( unsigned int i=0; i<m_uiBufferSize; ++i )
					printf( "%g, ", m_pdBufferSrc[i] );
				printf( "\n" );
				printf( "- dst: " );
				for( unsigned int i=0; i<m_uiBufferSize; ++i )
					printf( "%g, ", m_pdBufferDst[i] );
				printf( "\n" );
			}
			*/
		}
		m_uiBufferPos = 0;
		m_pdOut[OP_TFF_OUT_SYNC] = +1.0;
	}
	else
	{
		m_pdOut[OP_TFF_OUT_SYNC] = 0.0;
	}

	if( m_poIn[OP_TFF_IN_RST].IsValid() )
	{
		if( m_poIn[OP_TFF_IN_RST].Proc() > 0.0 )
		{
			if( m_uiFuncNum == 1 )
			{
 				m_oFFT.IFFT( m_pdBufferSrc, m_pdBufferDst );
				m_oFFT.Rescale( m_pdBufferDst );
				/*
				if( ::GetAsyncKeyState( VK_SPACE ) )
				{
					printf( "ifft\n" );
					printf( "- src: " );
					for( unsigned int i=0; i<m_uiBufferSize; ++i )
						printf( "%g, ", m_pdBufferSrc[i] );
					printf( "\n" );
					printf( "- dst: " );
					for( unsigned int i=0; i<m_uiBufferSize; ++i )
						printf( "%g, ", m_pdBufferDst[i] );
					printf( "\n" );
				}
				*/
			}
			m_uiBufferPos = 0;
		}
	}
}

void COpTransFFT::Update()
{
	if( m_uiBufferSize )
	{
		ClearBuffers();
		m_pdBufferSrc = new double[m_uiBufferSize];
		m_pdBufferDst = new double[m_uiBufferSize];
		//OP_RANGE_MOD( m_uiBufferPos, m_uiBufferSize );
		m_uiBufferPos = 0;
		m_oFFT.Init( m_uiBufferSize );
	}
	else
	{
		m_uiBufferPos = 0;
		m_pdBufferSrc = m_pdBufferDst = 0;
	}
	m_pdOut[OP_TFF_OUT_NUM] = double(m_uiBufferSize);
	
#ifdef OP_USE_RUNTIME_INFO
	// ACHTUNG: Hack!
	const unsigned int uiN = (unsigned int)(log(double(m_uiBufferSize / OP_TFF_SIZE_MIN)) / log(2.0));
	SetNameOutput( 0, CStr(char('a' + uiN)) + CStr( m_uiFuncNum ? '<' : '>' ) );
#endif // OP_USE_RUNTIME_INF
	//COpFFT::Update();
}

void COpTransFFT::Reset()
{
	if( m_uiBufferSize )
	{
		if( m_pdBufferSrc )
			OP_MEM_ZERO( m_pdBufferSrc, m_uiBufferSize * sizeof(double) );
		if( m_pdBufferDst )
			OP_MEM_ZERO( m_pdBufferDst, m_uiBufferSize * sizeof(double) );
		m_uiBufferPos = 0;
	}
	//COp::Reset();
	m_pdOut[OP_TFF_OUT_SYNC] = m_pdOut[OP_TFF_OUT_DST] = 0.0;
}

void COpTransFFT::Validate()
{
	m_bIsNotValid = !( m_poIn[OP_TFF_IN_SRC].IsValid() && m_uiBufferSize );
	//COp::Validate();
}
 
#ifdef OP_USE_ROUTINES
void COpTransFFT::Routine( unsigned int uiIndex )
{
	UNUSED_PARAMETER( uiIndex );
	switch( uiIndex )
	{
	case OP_TFF_ROUTINE_SIZE:
		m_uiBufferSize <<= 1;
		if( m_uiBufferSize > OP_TFF_SIZE_MAX )
			m_uiBufferSize = OP_TFF_SIZE_MIN;
	break;
	case OP_TFF_ROUTINE_FUNC:
		m_uiFuncNum = !m_uiFuncNum;
	break;
	}
	Update();
	Reset();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpTransFFT, OP_TFF_CLASS_INFO )
//OP_GENERIC_COPY_CTOR_DEF( COpTransFFT )
OP_GENERIC_METHODS_DEF( COpTransFFT )

#endif // OP_INC_OP_TRANS_FFT
