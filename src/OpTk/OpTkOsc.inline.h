// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkOsc.inline.h

////////////////////////////////////////////////////////////////////////////////
// COpTkOsc
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOsc<T, SAMPLE_FREQ>::COpTkOsc()
: m_tFreq( T(0.0) )
, m_tPhase( T(0.0) )
, m_tPos( T(0.0) )
, m_tFT( T(0.0) )
, m_tPosPhase( T(0.0) )
, m_iPeriod( 0 )
{}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOsc<T, SAMPLE_FREQ>::COpTkOsc( const COpTkOsc<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOsc<T, SAMPLE_FREQ> & COpTkOsc<T, SAMPLE_FREQ>::operator =( const COpTkOsc &roO )
{
	m_tFreq = roO.m_tFreq;
	m_tPhase = roO.m_tPhase;
	m_tPos = roO.m_tPos;
	m_tFT = roO.m_tFT;
	m_tPosPhase = roO.m_tPosPhase;
	m_iPeriod = roO.m_iPeriod;
	return *this;
}


////////////////////////////////////////////////////////////////////////////////
// COpTkOscSin
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscSin<T, SAMPLE_FREQ>::COpTkOscSin()
: COpTkOsc<T, SAMPLE_FREQ>()
{}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscSin<T, SAMPLE_FREQ>::COpTkOscSin( const COpTkOscSin<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscSin<T, SAMPLE_FREQ> & COpTkOscSin<T, SAMPLE_FREQ>::operator =( const COpTkOscSin &roO )
{
	COpTkOsc<T, SAMPLE_FREQ>::operator =( roO );
	return *this;
}


////////////////////////////////////////////////////////////////////////////////
// COpTkOscSinVar
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscSinVar<T, SAMPLE_FREQ>::COpTkOscSinVar( unsigned int uiType )
: COpTkOscSin<T, SAMPLE_FREQ>()
, m_uiType( uiType )
{}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscSinVar<T, SAMPLE_FREQ>::COpTkOscSinVar( const COpTkOscSinVar<T, SAMPLE_FREQ> &roO )
: COpTkOscSin<T, SAMPLE_FREQ>()
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscSinVar<T, SAMPLE_FREQ> & COpTkOscSinVar<T, SAMPLE_FREQ>::operator =( const COpTkOscSinVar &roO )
{
	COpTkOscSin<T, SAMPLE_FREQ>::operator =( roO );
	m_uiType = roO.m_uiType;
	return *this;
}

template<class T, const unsigned int SAMPLE_FREQ>
T COpTkOscSinVar<T, SAMPLE_FREQ>::Proc()
{
	T tV = T(0.0);
	switch( m_uiType )
	{
	case SIN:
		tV = COpTkOscSin<T, SAMPLE_FREQ>::Proc();
	break;
	case SQR:
		tV = ( COpTkOscSin<T, SAMPLE_FREQ>::Proc() >= T(0.0) )?( T(+1.0) ):( T(-1.0) );
	break;
	case SAW:
		//this->m_tPos += this->m_tFT;
		//tV = fmod( this->m_tPos, OP_TK_PI2 ) * ( T(2.0) / OP_TK_PI2 ) - T(1.0);
		this->UpdatePosPhase();
		tV = fmod( this->m_tPosPhase, OP_TK_PI2 ) * ( T(2.0) / OP_TK_PI2 ) - T(1.0);
		this->m_tPos += this->m_tFT;
	break;
	}
	return tV;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkOscSinVar<T, SAMPLE_FREQ>::NextType()
{
	if( ++m_uiType >= _TYPE_MAX )
		m_uiType = 0;
}

////////////////////////////////////////////////////////////////////////////////
// COpTkOscSinVar
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscApprox<T, SAMPLE_FREQ>::COpTkOscApprox( unsigned int uiType, unsigned int uiHarmon )
: COpTkOsc<T, SAMPLE_FREQ>()
, m_uiType( uiType )
, m_uiHarmon( uiHarmon )
{}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscApprox<T, SAMPLE_FREQ>::COpTkOscApprox( const COpTkOscApprox<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscApprox<T, SAMPLE_FREQ> & COpTkOscApprox<T, SAMPLE_FREQ>::operator =( const COpTkOscApprox &roO )
{
	COpTkOsc<T, SAMPLE_FREQ>::operator =( roO );
	m_uiType = roO.m_uiType;
	m_uiHarmon = roO.m_uiHarmon;
	return *this;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkOscApprox<T, SAMPLE_FREQ>::SetHarmon( unsigned int uiHarmon )
{
	// +1 da bei k = 2 begonnen wird, Obertöne hinzuzufügen.
	m_uiHarmon = uiHarmon + 1;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkOscApprox<T, SAMPLE_FREQ>::NextType()
{
	if( ++m_uiType >= _TYPE_MAX )
		m_uiType = 0;
}

template<class T, const unsigned int SAMPLE_FREQ>
T COpTkOscApprox<T, SAMPLE_FREQ>::Proc()
{
 	this->UpdatePosPhase();

	T tOut = T(0.0);
	unsigned int k = 2;
	switch( m_uiType )
	{
	case PLS:
		tOut = sin( this->m_tPosPhase );
		while( k <= m_uiHarmon )
		{
			const T tT = T(2.0) * k - T(1.0);
			tOut += sin( tT * this->m_tPosPhase ) / tT;
			++k;
		}
		tOut *= T( 4.0 / OP_TK_PI );
	break;
	case TRI:
		tOut = cos( this->m_tPosPhase );
		while( k <= m_uiHarmon )
		{
			const T tT = T(2.0) * k - T(1.0);
			tOut += cos( tT * this->m_tPosPhase ) / ( tT * tT );
			++k;
		}
		tOut *= T( 3.0 / OP_TK_PI ); // Modifiziert.
	break;
	case SAW:
		tOut = sin( this->m_tPosPhase );
		while( k <= m_uiHarmon )
		{
			tOut += sin( k * this->m_tPosPhase ) / k;
			++k;
		}
		tOut *= T( -2.0 / OP_TK_PI );
	break;
	};

	this->m_tPos += this->m_tFT;
	return tOut * T(0.82102); // Gibbs-Korrektur.
}

////////////////////////////////////////////////////////////////////////////////
// COpTkOscLUT
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscLUT<T, SAMPLE_FREQ>::COpTkOscLUT( unsigned int uiType )
: COpTkOsc<T, SAMPLE_FREQ>()
, m_uiType( uiType )
, m_uiSize( LUT_SIZE_MIN )
, m_tSizeFac( T(0.0) )
{}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscLUT<T, SAMPLE_FREQ>::COpTkOscLUT( const COpTkOscLUT<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkOscLUT<T, SAMPLE_FREQ> & COpTkOscLUT<T, SAMPLE_FREQ>::operator =( const COpTkOscLUT &roO )
{
	COpTkOsc<T, SAMPLE_FREQ>::operator =( roO );
	m_uiType = roO.m_uiType;
	m_uiSize = roO.m_uiSize;
	m_uiLUTBitmask = roO.m_uiLUTBitmask;
	m_tSizeFac = roO.m_tSizeFac;
	OP_TK_MEM_COPY( m_atLUT, roO.m_atLUT, sizeof( m_atLUT ) );
	return *this;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkOscLUT<T, SAMPLE_FREQ>::Init()
{
	m_uiLUTBitmask = ( m_uiSize - 1 );
	m_tSizeFac = T( m_uiSize ) * T( 1.0 / SAMPLE_FREQ );

	COpTkSeqRandom<T> oRand;
	T tTriStp = ( T(4.0) / m_uiSize );
	T tTri = 0.0;
	const T tXStp = ( T(OP_TK_PI2) / m_uiSize );
	T tX = 0.0;
	for( unsigned int i=0; i<m_uiSize; ++i )
	{
		switch( m_uiType )
		{
		case SIN:
			m_atLUT[i] = T( sin( tX ) );
		break;
		case SQR:
			m_atLUT[i] = ( i < ( m_uiSize / 2 ) ? T(-1.0) : T(1.0) );
		break;
		case SAW:
			m_atLUT[i] = T( fmod( i / double( m_uiSize ), 1.0 ) * 2.0 - 1.0 );
		break;
		case TRI:
			m_atLUT[i] = tTri;
			tTri += tTriStp;
			if( tTri >= 1.0 )
			{
				tTri = 1.0;
				tTriStp = -tTriStp;
			}
			else if( tTri <= -1.0 )
			{
				tTri = -1.0;
				tTriStp = -tTriStp;
			}
		break;
		case RND:
			m_atLUT[i] = T( 2.0 * oRand.Proc() - 1.0 );
		break;
		}

		tX += tXStp;
	}
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkOscLUT<T, SAMPLE_FREQ>::NextType()
{
	if( ++m_uiType >= _TYPE_MAX )
		m_uiType = 0;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkOscLUT<T, SAMPLE_FREQ>::NextSize()
{
	m_uiSize <<= 1;
	if( m_uiSize > LUT_SIZE_MAX )
		m_uiSize = LUT_SIZE_MIN;
}

// totest
template<class T, const unsigned int SAMPLE_FREQ>
bool COpTkOscLUT<T, SAMPLE_FREQ>::SetSize( unsigned int uiExp )
{
	const unsigned int uiSizeNext = ( 1 << uiExp );
	if( uiSizeNext >= LUT_SIZE_MIN && uiSizeNext <= LUT_SIZE_MAX )
	{
		m_uiSize = uiSizeNext;
		return true;
	}
	return false;
}

template<class T, const unsigned int SAMPLE_FREQ>
T COpTkOscLUT<T, SAMPLE_FREQ>::Proc()
{
 	this->UpdatePosPhase();
	this->m_tPos += this->m_tFT;
	return m_atLUT[ (unsigned int)( this->m_tPosPhase ) & m_uiLUTBitmask ]; // + 0.5 -> round?

	//const unsigned int uiIdx = ( (unsigned int)( this->m_tPosPhase ) & m_uiLUTBitmask ); // + 0.5 -> round?
	//const unsigned int uiIdx = ( (unsigned int)( this->m_tPosPhase + T( 0.5 ) ) & m_uiLUTBitmask );
	//const unsigned int uiIdx = ( (unsigned int)( this->m_tPosPhase * m_uiSize + T( 0.5 ) ) & m_uiLUTBitmask );
	//const unsigned int uiIdx = ( (unsigned int)( this->m_tPosPhase * T( m_uiSize / OP_TK_PI2 ) + T( 0.5 ) ) & m_uiLUTBitmask );
	//const unsigned int uiIdx = ( (unsigned int)( tP * LUT_SIZE ) & LUT_BITMASK );
	//const T tP = ( this->m_tPosPhase * T( 1.0 / OP_TK_PI2 ) );
	//T tIntPart;
	//tP = modf( tP, &tIntPart );
	//const unsigned int uiIdx = (unsigned int)( tP * LUT_SIZE );
	
	//return m_atLUT[uiIdx];
}
