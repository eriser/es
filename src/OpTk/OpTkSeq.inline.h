// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkSeq.inline.h

////////////////////////////////////////////////////////////////////////////////
// COpTkSeqSto
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkSeqSto<T>::COpTkSeqSto()
: m_uiSize( 0 )
, m_uiPos( 0 )
, m_ptSto( 0 )
{}

template<class T>
COpTkSeqSto<T>::COpTkSeqSto( const COpTkSeqSto &roO )
{
	*this = roO;
}

template<class T>
COpTkSeqSto<T>::~COpTkSeqSto()
{
	Clear();
}

template<class T>
COpTkSeqSto<T> & COpTkSeqSto<T>::operator= ( const COpTkSeqSto<T> &roO )
{
	SetSize( roO.m_uiSize );
	OP_TK_MEM_COPY( m_ptSto, roO.m_ptSto, m_uiSize * sizeof(unsigned int) );
	m_uiPos = roO.m_uiPos;
	return *this;
}

template<class T>
void COpTkSeqSto<T>::SetSize( unsigned int uiSize )
{
	if( m_uiSize == uiSize  )
		return;
	if( !uiSize )
		uiSize = 1;
	if( m_uiSize != uiSize )
	{
		T *ptNew = new T[ uiSize ];
		const unsigned int uiToCopy = OP_TK_MIN( m_uiSize, uiSize );
		if( uiToCopy )
			OP_TK_MEM_COPY( ptNew, m_ptSto, uiToCopy * sizeof(T) );
		OP_TK_MEM_ZERO( ptNew + uiToCopy, ( uiSize - uiToCopy ) * sizeof(T) );
		OP_TK_DELETE_ARRAY( m_ptSto );
		m_ptSto = ptNew;
		m_uiSize = uiSize;
	}
	if( m_uiPos >= m_uiSize )
		m_uiPos %= m_uiSize;
}

template<class T>
void COpTkSeqSto<T>::Clear()
{
	OP_TK_DELETE_ARRAY( m_ptSto );
	m_uiSize = 0;
}


template<class T>
void COpTkSeqSto<T>::Reset()
{
	m_uiPos = 0;
	if( m_ptSto )
	{
		OP_TK_MEM_ZERO( m_ptSto, m_uiSize * sizeof(T) );
	}
}

template<class T>
void COpTkSeqSto<T>::Insert( T tV )
{
	m_ptSto[m_uiPos] = tV;
	++m_uiPos;
}


template<class T>
int COpTkSeqSto<T>::GetIndex( T tV ) const
{
	for( unsigned int i=0; i<m_uiPos; ++i )
	{
		if( m_ptSto[i] == tV )
			return int( i );
	}
	return -1;
}

template<class T>
bool COpTkSeqSto<T>::InsertIfNew( T tV )
{
	T *ptS = m_ptSto + m_uiPos + 1;
	while( --ptS >= m_ptSto )
	{
		if( *ptS == tV )
			return false;
	}
	++m_uiPos;
	if( m_uiPos >= m_uiSize )
		return false;
	m_ptSto[m_uiPos] = tV;
	return true;
}

// new
template<class T>
T COpTkSeqSto<T>::GetNext()
{
	if( !m_ptSto || !m_uiSize )
		return T( 0 );
	const T tRet = m_ptSto[m_uiPos];
	++m_uiPos;
	if( m_uiPos >= m_uiSize )
		m_uiPos = 0;
	return tRet;
}


////////////////////////////////////////////////////////////////////////////////
// COpTkSeqPrime
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkSeqPrime<T>::COpTkSeqPrime()
: COpTkSeqSto<T>()
, m_uiMin( 0 )
{}

template<class T>
COpTkSeqPrime<T>::COpTkSeqPrime( const COpTkSeqPrime &roO )
: COpTkSeqSto<T>()
{
	*this = roO;
}

template<class T>
COpTkSeqPrime<T>::~COpTkSeqPrime()
{}

template<class T>
COpTkSeqPrime<T> & COpTkSeqPrime<T>::operator= ( const COpTkSeqPrime<T> &roO )
{
	COpTkSeqSto<T>::operator =( roO );
	m_uiMin = roO.m_uiMin;
	return *this;
}

template<class T>
void COpTkSeqPrime<T>::SetSize( unsigned int uiSize )
{
	COpTkSeqSto<T>::SetSize( uiSize );
	this->m_ptSto[0] = T(2.0);
	if( m_uiMin >= this->m_uiSize )
		m_uiMin = this->m_uiSize - 1;
	if( this->m_uiPos >= this->m_uiSize )
		Reset();
}

template<class T>
void COpTkSeqPrime<T>::SetMin( unsigned int uiMin )
{
	if( m_uiMin == uiMin || !this->m_uiSize )
		return;
	if( uiMin >= this->m_uiSize )
		uiMin = this->m_uiSize - 1;
	m_uiMin = uiMin;
}

template<class T>
void COpTkSeqPrime<T>::Precalc()
{
	unsigned int c = this->m_uiPos;
	this->m_uiPos = 0;
	
	while( c )
	{
		if( this->m_ptSto[this->m_uiPos] == T(0.0) )
			break;
		++this->m_uiPos;
		--c;
	}
	while( c )
	{
		Proc();
		--c; 
	}
}

template<class T>
T COpTkSeqPrime<T>::Proc()
{
	if( this->m_uiPos >= this->m_uiSize )
		Reset();
	T n = this->m_ptSto[this->m_uiPos];
	if( n == T(0.0) )
	{
		n = this->m_ptSto[( m_uiMin ? this->m_uiPos - 1 : 0 )];
		
		while( true )
		{
			unsigned int i = this->m_uiPos;
			while( i )
			{
				if( !( n % this->m_ptSto[i-1] ) )
					break;
				--i;
			}
			if( !i )
			{
				this->m_ptSto[this->m_uiPos] = n;
				break;
			}
			++n;
		}
	}
	++this->m_uiPos;
	return n;
}

template<class T>
void COpTkSeqPrime<T>::Reset()
{
	//COpTkSeqSto<T>::Reset();
	this->m_uiPos = m_uiMin;
}


////////////////////////////////////////////////////////////////////////////////
// COpTkSeqRecaman
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkSeqRecaman<T>::COpTkSeqRecaman()
: COpTkSeqSto<T>()
, m_tV( T(0) )
, m_tInitV( 0 )
, m_iIndex( 0 )
, m_iInitIndex( 0 )
{}

template<class T>
COpTkSeqRecaman<T>::COpTkSeqRecaman( const COpTkSeqRecaman &roO )
: COpTkSeqSto<T>()
{
	*this = roO;
}

template<class T>
COpTkSeqRecaman<T>::~COpTkSeqRecaman()
{}

template<class T>
COpTkSeqRecaman<T> & COpTkSeqRecaman<T>::operator= ( const COpTkSeqRecaman<T> &roO )
{
	COpTkSeqSto<T>::operator =( roO );
	m_tV = roO.m_tV;
	m_iIndex = roO.m_iIndex;
	m_tInitV = roO.m_tInitV;
	m_iInitIndex = roO.m_iInitIndex;
	return *this;
}

template<class T>
T COpTkSeqRecaman<T>::Proc()
{
	while( this->m_uiPos >= this->m_uiSize )
		Reset();
	++m_iIndex;
	const T tDiff = m_tV - m_iIndex;
	const bool bNotExist = ( GetIndex( tDiff ) < 0 );
	if( m_tV > m_iIndex && bNotExist )
		m_tV = tDiff;
	else
		m_tV += m_iIndex;
	Insert( m_tV );
	//if( !InsertIfNew( m_tV ) )
	//	m_tV = m_ptSto[m_uiPos];
	return m_tV;
}

template<class T>
void COpTkSeqRecaman<T>::Init()
{
	m_tV = m_tInitV;
	m_iIndex = m_iInitIndex;
}

template<class T>
void COpTkSeqRecaman<T>::Reset()
{
	COpTkSeqSto<T>::Reset();
	Init();
	this->m_uiPos = 0;
}

template<class T>
void COpTkSeqRecaman<T>::Precalc()
{
	unsigned int c = this->m_uiPos;
	this->m_uiPos = 0;
	
	/*
	while( c )
	{
		if( this->m_ptSto[this->m_uiPos] == T(0.0) )
			break;
		m_tV = this->m_ptSto[this->m_uiPos];
		++this->m_uiPos;
		++this->m_iIndex;
		--c;
	}
	*/
	while( c )
	{
		Proc();
		--c; 
	}
}


////////////////////////////////////////////////////////////////////////////////
// COpTkSeqAAS
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkSeqAAS<T>::COpTkSeqAAS()
: COpTkSeqSto<T>()
{}

template<class T>
COpTkSeqAAS<T>::COpTkSeqAAS( const COpTkSeqAAS &roO )
: COpTkSeqSto<T>()
{
	*this = roO;
}

template<class T>
COpTkSeqAAS<T>::~COpTkSeqAAS()
{}

template<class T>
COpTkSeqAAS<T> & COpTkSeqAAS<T>::operator= ( const COpTkSeqAAS<T> &roO )
{
	COpTkSeqSto<T>::operator =( roO );
	// ...
	return *this;
}

template<class T>
void COpTkSeqAAS<T>::Precalc( unsigned int uiSize, T tInitSeq )
{
	// Allocate enough storage.
	static const unsigned int MAX = 1000;
	static const unsigned int COUNT = 60; // 60 numbers -> max. ~12 MB data.
	T ** aatBuffer = new T *[ COUNT ];
	OP_TK_MEM_ZERO( aatBuffer, COUNT * sizeof(T *) );
	aatBuffer[0] = new T[ MAX ];
	OP_TK_MEM_ZERO( aatBuffer[0], MAX * sizeof(T) );
	
	unsigned int uiDigitsTotal = 0;

	// Insert every digit of tInit as sequence elements (reversed).
	const double dExp = OP_TK_LOG_N( tInitSeq, 10.0 );
	unsigned int uiInitSeq = (unsigned int)( tInitSeq );
	int d = (int)( dExp ); // d == ( DigitCount( tInit ) - 1 )
	//const double dCheck = pow( 10.0, dExp );
	while( uiInitSeq && d >= 0 )
	{
		if( d < int( MAX ) )
			aatBuffer[0][d] = uiInitSeq % 10;
		uiInitSeq /= 10;
		++uiDigitsTotal;
		--d;
	}

	// Calculate number sequence.
	bool bGoOn = true;
	for( unsigned int i=0; bGoOn && i < COUNT - 1; ++i )
	{
		aatBuffer[i+1] = new T[ MAX ];
		OP_TK_MEM_ZERO( aatBuffer[i+1], MAX * sizeof(T) );
		
		unsigned int jr = 0, jw = 0;
		T n;
		while( n = aatBuffer[i][jr] )
		{
			unsigned int c = 0;
			while( jr < MAX && n == aatBuffer[i][jr] )
			{
				++c;
				++jr;
			}
			if( jw >= MAX - 1
			 || uiDigitsTotal >= uiSize ) // - 1 ) // CAUTION: Calculate more number pairs than needed, to assure uiSize numbers.
			{
				bGoOn = false;
				// CAUTION: Do not invalidate the entire current array of digits,
				// culculate digits up to the index we need below... (controled by uiSize).
				//aatBuffer[i+1][0] = T( 0 ); 
				break;
			}
			aatBuffer[i+1][jw++] = T( c );
			aatBuffer[i+1][jw++] = n;
			uiDigitsTotal += 2;
		}
	}
	
	// Insert digits into internal storage and clean up.
	this->SetSize( uiSize );
	this->m_uiPos = 0;
	
	bGoOn = true;
	for( unsigned int i=0; bGoOn && i < COUNT && aatBuffer[i]; ++i )
	{
		unsigned int j = 0;
		while( T n = aatBuffer[i][j] )
		{
			if( uiSize )
			{
				--uiSize;
				this->Insert( n );
				//printf( "%u", n );
			}
			++j;
			if( j >= MAX )
			{
				bGoOn = false;
				break;
			}
		}
		OP_TK_DELETE_ARRAY( aatBuffer[i] );
		//printf( "\n" );
	}
	OP_TK_DELETE_ARRAY( aatBuffer );

	Reset();
}

template<class T>
T COpTkSeqAAS<T>::Proc()
{
	return COpTkSeqSto<T>::GetNext();
}

template<class T>
void COpTkSeqAAS<T>::Reset()
{
	//COpTkSeqSto<T>::Reset();
	this->m_uiPos = 0;
}



////////////////////////////////////////////////////////////////////////////////
// COpTkSeqLogistic
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkSeqLogistic<T>::COpTkSeqLogistic()
: m_tC( T(0.0) )
, m_tV( T(0.0) )
, m_tV0( T(0.0) )
{}

template<class T>
COpTkSeqLogistic<T>::COpTkSeqLogistic( const COpTkSeqLogistic &roO )
{
	*this = roO;
}

template<class T>
COpTkSeqLogistic<T>::~COpTkSeqLogistic()
{}

template<class T>
COpTkSeqLogistic<T> & COpTkSeqLogistic<T>::operator= ( const COpTkSeqLogistic<T> &roO )
{
	m_tC = roO.m_tC;
	m_tV = roO.m_tV;
	m_tV0 = roO.m_tV0;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
// COpTkSeqMandelbrot
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkSeqMandelbrot<T>::COpTkSeqMandelbrot()
: m_tX( T(0.0) )
, m_tY( T(0.0) )
, m_tA( T(0.0) )
, m_tB( T(0.0) )
{}

template<class T>
COpTkSeqMandelbrot<T>::COpTkSeqMandelbrot( const COpTkSeqMandelbrot &roO )
{
	*this = roO;
}

template<class T>
COpTkSeqMandelbrot<T>::~COpTkSeqMandelbrot()
{}

template<class T>
COpTkSeqMandelbrot<T> & COpTkSeqMandelbrot<T>::operator= ( const COpTkSeqMandelbrot<T> &roO )
{
	m_tX = roO.m_tX;
	m_tY = roO.m_tY;
	m_tA = roO.m_tA;
	m_tB = roO.m_tB;
	return *this;
}

template<class T>
T COpTkSeqMandelbrot<T>::Proc()
{
	const T tXT = m_tX * m_tX - m_tY * m_tY + m_tA;
	const T tYT = T(2.0) * m_tX * m_tY + m_tB;
	m_tX = tXT;
	m_tY = tYT;
	return sqrt( m_tX * m_tX + m_tY * m_tY );
}

template<class T>
void COpTkSeqMandelbrot<T>::Reset()
{
	m_tX = m_tY = T(0.0);
}

////////////////////////////////////////////////////////////////////////////////
// COpTkSeqFibonacci
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkSeqFibonacci<T>::COpTkSeqFibonacci()
: m_tV0( T(0) )
, m_tV1( T(1) )
{}

template<class T>
COpTkSeqFibonacci<T>::COpTkSeqFibonacci( const COpTkSeqFibonacci &roO )
{
	*this = roO;
}

template<class T>
COpTkSeqFibonacci<T>::~COpTkSeqFibonacci()
{}

template<class T>
COpTkSeqFibonacci<T> & COpTkSeqFibonacci<T>::operator= ( const COpTkSeqFibonacci<T> &roO )
{
	m_tV0 = roO.m_tV0;
	m_tV1 = roO.m_tV1;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
// COpTkSeqRandom
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkSeqRandom<T>::COpTkSeqRandom()
: m_iSeed( 40692 )
{
	Reset();
}

template<class T>
COpTkSeqRandom<T>::COpTkSeqRandom( const COpTkSeqRandom<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkSeqRandom<T> & COpTkSeqRandom<T>::operator =( const COpTkSeqRandom &roO )
{
	m_iS0 = roO.m_iS0;
	m_iS1 = roO.m_iS1;
	m_iSeed = roO.m_iSeed;
	return *this;
}

template<class T>
T COpTkSeqRandom<T>::Proc()
{
	int z, k;
	k = m_iS0 / 53668;
	m_iS0 = 40014 * (m_iS0 - k * 53668) - k * 12211;
	if (m_iS0 < 0) m_iS0 = m_iS0 + 2147483563;
	k = m_iS1 / 52774;
	m_iS1 = 40692 * (m_iS1 - k * 52774) - k * 3791;
	if (m_iS1 < 0) m_iS1 = m_iS1 + 2147483399;
	z = m_iS0 - m_iS1;
	if (z < 1) z = z + 2147483562;
	return T( z * 4.65661305956E-10 );
}

template<class T>
void COpTkSeqRandom<T>::Reset()
{
	m_iS0 = m_iSeed;
	m_iS1 = 2147483399;
}


////////////////////////////////////////////////////////////////////////////////
// COpTkSeqRandomEx
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkSeqRandomEx<T>::COpTkSeqRandomEx( unsigned int uiType )
: COpTkSeqRandom<T>()
, m_uiType( uiType )
, m_tB( T(0.0) )
{
	Reset();
}

template<class T>
COpTkSeqRandomEx<T>::COpTkSeqRandomEx( const COpTkSeqRandomEx<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkSeqRandomEx<T> & COpTkSeqRandomEx<T>::operator =( const COpTkSeqRandomEx &roO )
{
	COpTkSeqRandom<T>::operator =( roO );
	m_uiType = roO.m_uiType;
	m_tB = roO.m_tB;
	m_tV0 = roO.m_tV0;
	m_tV1 = roO.m_tV1;
	m_tV2 = roO.m_tV2;
	m_tVL = roO.m_tVL;
	return *this;
}

template<class T>
T COpTkSeqRandomEx<T>::Proc()
{
	T tV = COpTkSeqRandom<T>::Proc();
	if( m_uiType == PINK )
	{
		m_tV0 = T(0.3190) * ( m_tV0 - tV ) + tV;
		tV = COpTkSeqRandom<T>::Proc();
		m_tV1 = T(0.7756) * ( m_tV1 - tV ) + tV;
		tV = COpTkSeqRandom<T>::Proc();
		m_tV2 = T(0.9613) * ( m_tV2 - tV ) + tV;
		tV = ( T(0.02109238) * m_tV0 + T(0.07113478) * m_tV1 + T(0.68873558) * m_tV2 ) 
			* T(3.3) - T( 0.02109238 + 0.07113478 + 0.68873558 ); // ACHTUNG: Eigentlich nur * T(3.0)!
		
		//static T tVVV_ = 0.0;
		//if( fabs( tV ) > tVVV_ )
		//{
		//	tVVV_ = fabs( tV );
		//	printf( "max: %g\n", tV );
		//}
		//if( tV < 0.0 )
		//	printf( "- %g\n", tV );
		//tV -= T( 0.02109238 + 0.07113478 + 0.68873558 );
	}
	tV = m_tB * m_tVL + ( T(1.0) - m_tB ) * tV;
	m_tVL = tV;
	tV *= T(2.0);
	tV -= T(1.0);
	//if( fabs( tV ) > 1.0 )
	//	printf( "gad\n" );
	return tV;
}

template<class T>
unsigned int & COpTkSeqRandomEx<T>::Type()
{
	return m_uiType;
}

template<class T>
void COpTkSeqRandomEx<T>::SetBalance( T tB )
{
	OP_TK_RANGE_MINMAX( tB, 0.0, 1.0 );
	m_tB = tB;
}

template<class T>
void COpTkSeqRandomEx<T>::NextType()
{
	if( ++m_uiType >= TYPE_MAX )
		m_uiType = 0;
}

template<class T>
void COpTkSeqRandomEx<T>::Reset()
{
	COpTkSeqRandom<T>::Reset();
	m_tV0 = T(0.18360870185628544);
	m_tV1 = T(0.70990110977436061);
	m_tV2 = T(0.50509018629370073);
	m_tVL = T(0.48846372010208033);
}


////////////////////////////////////////////////////////////////////////////////
// COpTkSeqCollision1D
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkSeqCollision1D<T>::COpTkSeqCollision1D()
: m_pbCollision( 0 )
, m_tDamp( T(0.0) )
, m_oRand()
, m_poP( 0 )
, m_uiCount( 0 )
{}

template<class T>
COpTkSeqCollision1D<T>::COpTkSeqCollision1D( const COpTkSeqCollision1D &roO )
{
	*this = roO;
}

template<class T>
COpTkSeqCollision1D<T>::~COpTkSeqCollision1D()
{
	Clear();
}

template<class T>
COpTkSeqCollision1D<T> & COpTkSeqCollision1D<T>::operator= ( const COpTkSeqCollision1D<T> &roO )
{
	Clear();
	SetCount( roO.m_uiCount );
	//OP_TK_MEM_COPY( m_pbCollision, roO.m_pbCollision, sizeof(bool) * m_uiCount );
	OP_TK_MEM_COPY( m_poP, roO.m_poP, sizeof(SParticle) * m_uiCount );
	m_tDamp = roO.m_tDamp;
	m_oRand = roO.m_oRand;
	return *this;
}

template<class T>
void COpTkSeqCollision1D<T>::Proc()
{
	const float fDamp = float( m_tDamp );
	
	// Verschiebung.
	unsigned int i = 0;
	while( i < m_uiCount )
	{
		m_poP[i].m_fPos += m_poP[i].m_fVel;
		++i;
	}
	
	// Kollision.
	i = 0;
	if( m_poP[i].m_fPos - m_poP[i].m_fExt < -1.0f )
	{
		m_poP[i].m_fPos = m_poP[i].m_fExt - 1.0f;
		m_poP[i].m_fVel = - m_poP[i].m_fVel;
		m_pbCollision[i] = true;
	}
	else
		m_pbCollision[i] = false;
	
	for( i=0; i<m_uiCount-1; ++i )
	{
		float fDist = m_poP[i+1].m_fPos - m_poP[i].m_fPos;
		float fExt2 = m_poP[i+1].m_fExt + m_poP[i].m_fExt;
		if( fDist <= fExt2 )
		{
			float fTmp = m_poP[i].m_fMass + m_poP[i+1].m_fMass;
			
			float fV0 = ( m_poP[i].m_fMass - m_poP[i+1].m_fMass )
				* m_poP[i].m_fVel
				+ 2.0f * m_poP[i+1].m_fMass * m_poP[i+1].m_fVel;
			fV0 /= fTmp;
			
			float fV1 = ( m_poP[i+1].m_fMass - m_poP[i].m_fMass )
				* m_poP[i+1].m_fVel
				+ 2.0f * m_poP[i].m_fMass * m_poP[i].m_fVel;
			fV1 /= fTmp;
			
			m_poP[i].m_fVel = fV0 * fDamp;
			m_poP[i+1].m_fVel = fV1 * fDamp;
			
			fTmp = ( fExt2 - fDist ) / 2.0f;
			m_poP[i+1].m_fPos += fTmp;
			m_poP[i].m_fPos -= fTmp;
			
			m_pbCollision[i] = true;
		}
		else
			m_pbCollision[i] = false;
	}
	i = m_uiCount-1;
	if( m_poP[i].m_fPos + m_poP[i].m_fExt > 1.0f )
	{
		m_poP[i].m_fPos = 1.0f - m_poP[i].m_fExt;
		m_poP[i].m_fVel = - m_poP[i].m_fVel;
		m_pbCollision[i] = true;
	}
	else
		m_pbCollision[i] = false;
}

template<class T>
void COpTkSeqCollision1D<T>::SetCount( unsigned int uiCount )
{
	Clear();
	m_poP = new SParticle[uiCount];
	m_pbCollision = new bool[uiCount];
	OP_TK_MEM_ZERO( m_poP, uiCount * sizeof(SParticle) );
	OP_TK_MEM_ZERO( m_pbCollision, uiCount * sizeof(bool) );
	m_uiCount = uiCount;
	Reset();
}

template<class T>
void COpTkSeqCollision1D<T>::Clear()
{
	OP_TK_DELETE_ARRAY( m_pbCollision );
	OP_TK_DELETE_ARRAY( m_poP );
}

template<class T>
void COpTkSeqCollision1D<T>::Reset()
{
	m_oRand.Reset();
	
	unsigned int c = m_uiCount;
	while( c )
	{
 		--c;
		m_pbCollision[c] = false;
		
		const float fInitVel = 0.15f * m_oRand.Proc();
		
		m_poP[c].m_fVel = fInitVel * ( m_oRand.Proc() ) - ( fInitVel * 0.5f );
		m_poP[c].m_fExt = ( 1.0f / m_uiCount ) * ( m_oRand.Proc() );
		m_poP[c].m_fMass = m_poP[c].m_fExt * 0.000001f;
		
		m_poP[c].m_fPos = ( 2.0f / m_uiCount - 2.0f * m_poP[c].m_fExt ) * ( m_oRand.Proc() ) / 2.0f;
		m_poP[c].m_fPos += m_poP[c].m_fExt + c * 2.0f / m_uiCount - 1.0f;
	}
}

////////////////////////////////////////////////////////////////////////////////
// COpTkSeqCell1D
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkSeqCell1D<T>::COpTkSeqCell1D()
: m_uiCount( 0 )
, m_oRand()
, m_uiCellScope( 0 )
, m_uiNeighbours( 0 )
, m_uiIndex( 0 )
, m_uiCountRule( 0 )
, m_uiStates( 0 )
, m_atRule( 0 )
, m_atCell( 0 )
, m_atCellTmp( 0 )
{}

template<class T>
COpTkSeqCell1D<T>::COpTkSeqCell1D( const COpTkSeqCell1D &roO )
{
	*this = roO;
}

template<class T>
COpTkSeqCell1D<T>::~COpTkSeqCell1D()
{
	Clear();
}

template<class T>
COpTkSeqCell1D<T> & COpTkSeqCell1D<T>::operator= ( const COpTkSeqCell1D<T> &roO )
{
	Clear();
	//m_uiCount = roO.m_uiCount;
	m_oRand = roO.m_oRand;
	//m_uiCellScope = roO.m_uiCellScope;
	//m_uiNeighbours = roO.m_uiNeighbours;
	//m_uiCountRule = roO.m_uiCountRule;
	//m_uiStates = roO.m_uiStates;
	Init( roO.m_uiCount, roO.m_uiStates, roO.m_uiNeighbours );
	m_uiIndex = roO.m_uiIndex;
	OP_TK_MEM_COPY( m_atRule, roO.m_atRule, sizeof(T) * m_uiCountRule );
	OP_TK_MEM_COPY( m_atCell, roO.m_atCell, sizeof(T) * m_uiCount );
	OP_TK_MEM_COPY( m_atCellTmp, roO.m_atCellTmp, sizeof(T) * m_uiCount );
	return *this;
}

template<class T>
void COpTkSeqCell1D<T>::Init( unsigned int uiCount, unsigned int uiStates, unsigned int uiNeighbours )
{		
	if( uiCount < 3 )
		uiCount = 3;
	if( !uiStates )
		++uiStates;
	if( !uiNeighbours )
		++uiNeighbours;
	
	Clear();
	
	m_uiCount = uiCount;
	m_uiStates = uiStates;
	m_uiNeighbours = uiNeighbours;
	
	m_uiCellScope = m_uiNeighbours * 2 + 1;
	m_uiCountRule = (unsigned int)(pow( double(m_uiStates), double(m_uiCellScope) ));
	m_atRule = new unsigned int[ m_uiCountRule ];
	for( unsigned int i=0; i<m_uiCountRule; i++ )
		m_atRule[i] = rand() % m_uiStates;
	m_atCell = new T[ m_uiCount ];
	m_atCellTmp = new T[ m_uiCount ];
	OP_TK_MEM_ZERO( m_atCell, sizeof(T) * m_uiCount );
	OP_TK_MEM_ZERO( m_atCellTmp, sizeof(T) * m_uiCount );
	m_uiIndex = 0;
	Randomize();
}

template<class T>
void COpTkSeqCell1D<T>::Randomize()
{
	m_oRand.Reset();
	for( unsigned int i=0; i<m_uiCount; i++ )
		m_atCell[i] = (unsigned int)( m_oRand.Proc() * 0x7fff ) % m_uiStates;
}

template<class T>
T COpTkSeqCell1D<T>::Proc()
{
	if( m_uiIndex >= m_uiCount )
	{
		m_uiIndex = 0;
		Gen();
	}
	return int( m_atCell[ m_uiIndex++ ] );
}

template<class T>
void COpTkSeqCell1D<T>::Gen()
{
	for( int i=0; i<int(m_uiCount); i++ )
	{
		T tCode = 0;
		for( int s=i-int(m_uiNeighbours); s<=(i+int(m_uiNeighbours)); s++ )
		{	
			int j;
			if( s < 0 )
				j = s + int(m_uiCount);
			else if( s >= int(m_uiCount) )
				j = s - int(m_uiCount);
			else
				j = s;
			tCode += 
				m_atCell[j]
				* static_cast<T>( pow( double(m_uiStates), double(s-(i-m_uiNeighbours)) ) );
		}
		m_atCellTmp[i] = m_atRule[tCode];
	}
	T *auiTmp = m_atCellTmp;
	m_atCellTmp = m_atCell;
	m_atCell = auiTmp;
}

template<class T>
void COpTkSeqCell1D<T>::Clear()
{
	OP_TK_DELETE_ARRAY( m_atRule );
	OP_TK_DELETE_ARRAY( m_atCell );
	OP_TK_DELETE_ARRAY( m_atCellTmp );
}
