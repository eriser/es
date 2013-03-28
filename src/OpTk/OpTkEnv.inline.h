// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkEnv.inline.h

////////////////////////////////////////////////////////////////////////////////
// COpTkEnv
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkEnv<T>::COpTkEnv()
: m_tV( T(0.0) )
, m_tV0( T(0.0) )
, m_uiIndex( 0 )
, m_uiState( 0 )
{}

template<class T>
COpTkEnv<T>::COpTkEnv( const COpTkEnv<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkEnv<T>::~COpTkEnv()
{}

template<class T>
COpTkEnv<T> & COpTkEnv<T>::operator =( const COpTkEnv<T> &roO )
{
	m_tV = roO.m_tV;
	m_tV0 = roO.m_tV0;
	m_uiIndex = roO.m_uiIndex;
	m_uiState = roO.m_uiState;
	return *this;
}

template<class T>
unsigned int & COpTkEnv<T>::Index()
{
	return m_uiIndex;
}

template<class T>
unsigned int & COpTkEnv<T>::State()
{
	return m_uiState;
}

template<class T>
T & COpTkEnv<T>::Value()
{
	return m_tV;
}


////////////////////////////////////////////////////////////////////////////////
// COpTkEnv1Lin
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkEnv1Lin<T>::COpTkEnv1Lin()
: COpTkEnv<T>()
, m_uiDur( 0 )
, m_tStp( T(0.0) )
, m_tV1( T(0.0) )
{}

template<class T>
COpTkEnv1Lin<T>::COpTkEnv1Lin( const COpTkEnv1Lin<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkEnv1Lin<T>::~COpTkEnv1Lin()
{}

template<class T>
COpTkEnv1Lin<T> & COpTkEnv1Lin<T>::operator =( const COpTkEnv1Lin<T> &roO )
{
	COpTkEnv<T>::operator =( roO );
	m_uiDur = roO.m_uiDur;
	m_tStp = roO.m_tStp;
	m_tV1 = roO.m_tV1;
	return *this;
}

template<class T>
void COpTkEnv1Lin<T>::Set( T tDur, T tV0, T tV1 )
{
	if( tDur <= T(0.0) )
	{
		m_uiDur = 0;
		m_tStp = T(0.0);
	}
	else
	{
		const T tT0 = rint( tDur );
		m_uiDur = (unsigned int)tT0;
		m_tStp = ( tV1 - tV0 ) / ( tT0 - T(1.0) );
	}
	this->m_tV0 = tV0;
	m_tV1 = tV1;
}

template<class T>
T COpTkEnv1Lin<T>::Proc()
{
	if( this->m_uiState )
	{
		const T tOut = this->m_tV;
		if( this->m_uiIndex < m_uiDur )
		{
			this->m_tV += m_tStp;
			++this->m_uiIndex;
			if( this->m_uiIndex >= m_uiDur )
				this->m_tV = m_tV1;
		}
		else {
			this->m_tV = m_tV1;
			this->m_uiState = 0;
		}
		return tOut;
	}
	else {
		return ( this->m_uiIndex ? this->m_tV : this->m_tV0 );
	}
}


////////////////////////////////////////////////////////////////////////////////
// COpTkEnv1Exp
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkEnv1Exp<T>::COpTkEnv1Exp()
: COpTkEnv<T>()
, m_uiDur( 0 )
, m_tP( T(0.0) )
, m_tStp( T(0.0) )
, m_tV1( T(0.0) )
, m_tFac( T(0.0) )
{}

template<class T>
COpTkEnv1Exp<T>::COpTkEnv1Exp( const COpTkEnv1Exp<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkEnv1Exp<T>::~COpTkEnv1Exp()
{}

template<class T>
COpTkEnv1Exp<T> & COpTkEnv1Exp<T>::operator =( const COpTkEnv1Exp<T> &roO )
{
	COpTkEnv<T>::operator =( roO );
	m_uiDur = roO.m_uiDur;
	m_tP = roO.m_tP;
	m_tStp = roO.m_tStp;
	m_tV1 = roO.m_tV1;
	m_tFac = roO.m_tFac;
	return *this;
}

template<class T>
T & COpTkEnv1Exp<T>::Position() {
	return m_tP;
}

template<class T>
void COpTkEnv1Exp<T>::Set( T tDur, T tV0, T tV1, T tSlope )
{
	if( tSlope == T(0.0) )
		tSlope = T(1e-11);
	if( tDur <= T(0.0) )
	{
		m_uiDur = 0;
		m_tStp = T(0.0);
		m_tFac = T(0.0);
	}
	else
	{
		if( tV0 > tV1 )
			tSlope *= -1.0;
		m_uiDur = (unsigned int)rint( tDur );		
		m_tStp = exp( tSlope / tDur );
		m_tFac = ( tV1 - tV0 ) / ( pow( m_tStp, tDur - T(1.0) ) - T(1.0) );
		if( m_tFac == T(0.0) )
			m_tStp = T(0.0);
	}
	this->m_tV0 = tV0;
	m_tV1 = tV1;
}

template<class T>
T COpTkEnv1Exp<T>::Proc()
{
	if( this->m_uiState )
	{
		if( this->m_uiIndex < m_uiDur )
		{
			this->m_tV = this->m_tV0 + ( m_tP - T(1.0) ) * m_tFac;
			m_tP *= m_tStp;
			++this->m_uiIndex;
			if( this->m_uiIndex >= m_uiDur )
			{
				this->m_tV = m_tV1;
				this->m_tP = T(1.0);
			}
		}
		else
		{
			this->m_tV = m_tV1;
			this->m_uiState = 0;
		}
		return this->m_tV;
	}
	else {
		return ( this->m_uiIndex ? this->m_tV : this->m_tV0 );
	}
}


////////////////////////////////////////////////////////////////////////////////
// COpTkEnv1Sin
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkEnv1Sin<T>::COpTkEnv1Sin()
: COpTkEnv<T>()
, m_uiDur( 0 )
, m_tP( T(0.0) )
, m_tStp( T(0.0) )
, m_tV1( T(0.0) )
, m_tFac( T(0.0) )
{}

template<class T>
COpTkEnv1Sin<T>::COpTkEnv1Sin( const COpTkEnv1Sin<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkEnv1Sin<T>::~COpTkEnv1Sin()
{}

template<class T>
COpTkEnv1Sin<T> & COpTkEnv1Sin<T>::operator =( const COpTkEnv1Sin<T> &roO )
{
	COpTkEnv<T>::operator =( roO );
	m_uiDur = roO.m_uiDur;
	m_tP = roO.m_tP;
	m_tStp = roO.m_tStp;
	m_tV1 = roO.m_tV1;
	m_tFac = roO.m_tFac;
	return *this;
}

template<class T>
T & COpTkEnv1Sin<T>::Position() {
	return m_tP;
}

template<class T>
void COpTkEnv1Sin<T>::Set( T tDur, T tV0, T tV1 )
{
	if( tDur <= T(0.0) )
	{
		m_uiDur = 0;
		m_tStp = T(0.0);
		m_tFac = T(0.0);
	}
	else
	{
		const T tT0 = rint( tDur );
		m_uiDur = (unsigned int)tT0;
		m_tStp = OP_TK_PI / ( tT0 - T(1.0) );
		m_tFac = tV1 - tV0;
	}
	this->m_tV0 = tV0;
	m_tV1 = tV1;
}

template<class T>
T COpTkEnv1Sin<T>::Proc()
{
	if( this->m_uiState )
	{
		const T tOut = this->m_tV;
		if( this->m_uiIndex < m_uiDur )
		{
			T t = sin( m_tP );
			t += T(1.0);
			t *= T(0.5);
			this->m_tV = this->m_tV0 + t * m_tFac;
			m_tP += m_tStp;
			++this->m_uiIndex;
			if( this->m_uiIndex >= m_uiDur )
			{
				this->m_tV = m_tV1;
				this->m_tP = T(OP_TK_PI_2);
			}
		}
		else
		{
			this->m_tV = m_tV1;
			this->m_uiState = 0;
		}
		return tOut;
	}
	else {
		return ( this->m_uiIndex ? this->m_tV : this->m_tV0 );
	}
}


////////////////////////////////////////////////////////////////////////////////
// COpTkEnv1SinPow
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkEnv1SinPow<T>::COpTkEnv1SinPow()
: COpTkEnv<T>()
, m_uiDur( 0 )
, m_tP( T(0.0) )
, m_tStp( T(0.0) )
, m_tV1( T(0.0) )
, m_tFac( T(0.0) )
, m_tExp( T(0.0) )
{}

template<class T>
COpTkEnv1SinPow<T>::COpTkEnv1SinPow( const COpTkEnv1SinPow<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkEnv1SinPow<T>::~COpTkEnv1SinPow()
{}

template<class T>
COpTkEnv1SinPow<T> & COpTkEnv1SinPow<T>::operator =( const COpTkEnv1SinPow<T> &roO )
{
	COpTkEnv<T>::operator =( roO );
	m_uiDur = roO.m_uiDur;
	m_tP = roO.m_tP;
	m_tStp = roO.m_tStp;
	m_tV1 = roO.m_tV1;
	m_tFac = roO.m_tFac;
	m_tExp = roO.m_tExp;
	return *this;
}

template<class T>
T & COpTkEnv1SinPow<T>::Position() {
	return m_tP;
}

template<class T>
T & COpTkEnv1SinPow<T>::Exponent() {
	return m_tExp;
}

template<class T>
void COpTkEnv1SinPow<T>::Set( T tDur, T tV0, T tV1, T tSlope )
{
	if( tSlope < T(0.0) )
		tSlope = T(0.0);
	if( tDur <= T(0.0) )
	{
		m_uiDur = 0;
		m_tStp = T(0.0);
		m_tFac = T(0.0);
	}
	else
	{
		const T tT0 = rint( tDur );
		m_uiDur = (unsigned int)tT0;
		m_tStp = OP_TK_PI_2 / ( tT0 - T(1.0) );
		m_tFac = tV1 - tV0;
	}
	m_tExp = tSlope;
	this->m_tV0 = tV0;
	m_tV1 = tV1;
}

template<class T>
T COpTkEnv1SinPow<T>::Proc()
{
	if( this->m_uiState )
	{
		const T tOut = this->m_tV;
		if( this->m_uiIndex < m_uiDur )
		{
			this->m_tV = this->m_tV0 + pow( sin( m_tP ), m_tExp ) * m_tFac;
			m_tP += m_tStp;
			++this->m_uiIndex;
			if( this->m_uiIndex >= m_uiDur )
			{
				this->m_tV = m_tV1;
				this->m_tP = T(OP_TK_PI_2);
			}
		}
		else
		{
			this->m_tV = m_tV1;
			this->m_uiState = 0;
		}
		return tOut;
	}
	else {
		return ( this->m_uiIndex ? this->m_tV : this->m_tV0 );
	}
}


////////////////////////////////////////////////////////////////////////////////
// COpTkEnv1Bezier2
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkEnv1Bezier2<T>::COpTkEnv1Bezier2()
: COpTkEnv<T>()
, m_uiDur( 0 )
, m_tP( T(0.0) )
, m_tStp( T(0.0) )
, m_tSlope( T(0.0) )
{}

template<class T>
COpTkEnv1Bezier2<T>::COpTkEnv1Bezier2( const COpTkEnv1Bezier2<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkEnv1Bezier2<T>::~COpTkEnv1Bezier2()
{}

template<class T>
COpTkEnv1Bezier2<T> & COpTkEnv1Bezier2<T>::operator =( const COpTkEnv1Bezier2<T> &roO )
{
	COpTkEnv<T>::operator =( roO );
	m_uiDur = roO.m_uiDur;
	m_tP = roO.m_tP;
	m_tStp = roO.m_tStp;
	m_tSlope = roO.m_tSlope;
	return *this;
}

template<class T>
T & COpTkEnv1Bezier2<T>::Position() {
	return m_tP;
}

template<class T>
void COpTkEnv1Bezier2<T>::Set( T tDur, T tV0, T tV1, T tSlope )
{
	if( tDur <= T(0.0) )
	{
		m_uiDur = 0;
		m_tStp = T(0.0);
		m_tSlope = T(0.0);
	}
	else
	{
		const T tT0 = rint( tDur );
		m_uiDur = (unsigned int)tT0;
		m_tStp = T(1.0) / ( tT0 - T(1.0) );
		m_tSlope = tSlope;
	}
	this->m_tV0 = tV0;
	m_tV1 = tV1;
}

template<class T>
T COpTkEnv1Bezier2<T>::Proc()
{
	if( this->m_uiState )
	{
		const T tOut = this->m_tV;
		if( this->m_uiIndex < m_uiDur )
		{
			const T tOneMinusT = T(1.0) - m_tP;
			T t = this->m_tV0;
			t *= tOneMinusT * tOneMinusT;;
			t += m_tSlope * T(2.0) * m_tP * tOneMinusT;
			t += m_tV1 * m_tP * m_tP;
			this->m_tV = t;
			
			m_tP += m_tStp;
			++this->m_uiIndex;
			if( this->m_uiIndex >= m_uiDur )
			{
				this->m_tV = m_tV1;
				m_tP = T(1.0);
			}
		}
		else
		{
			this->m_tV = m_tV1;
			this->m_uiState = 0;
		}
		return tOut;
	}
	else {
		return ( this->m_uiIndex ? this->m_tV : this->m_tV0 );
	}
}


////////////////////////////////////////////////////////////////////////////////
// COpTkEnv2Lin
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkEnv2Lin<T>::COpTkEnv2Lin()
: COpTkEnv<T>()
, m_uiDur0( 0 )
, m_uiDur1( 0 )
, m_tStp0( T(0.0) )
, m_tStp1( T(0.0) )
{}

template<class T>
COpTkEnv2Lin<T>::COpTkEnv2Lin( const COpTkEnv2Lin<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkEnv2Lin<T>::~COpTkEnv2Lin()
{}

template<class T>
COpTkEnv2Lin<T> & COpTkEnv2Lin<T>::operator =( const COpTkEnv2Lin<T> &roO )
{
	COpTkEnv<T>::operator =( roO );
	m_uiDur0 = roO.m_uiDur0;
	m_uiDur1 = roO.m_uiDur1;
	m_tStp0 = roO.m_tStp0;
	m_tStp1 = roO.m_tStp1;
	return *this;
}

template<class T>
void COpTkEnv2Lin<T>::Set( T tDur, T tRatio )
{
	OP_TK_RANGE_MINMAX( tRatio, T(0.0), T(1.0) );
	if( tDur < T(0.0) )
		tDur = T(0.0);
	const T tT0 = rint( tDur * tRatio );
	const T tT1 = rint( tDur - tT0 );
	m_tStp0 = ( ( tT0              )?( T(1.0) / ( tT0 - T(1.0) ) ):( T(1.0) ) );
	m_tStp1 = ( ( tRatio != T(1.0) )?( T(1.0) / ( tT1          ) ):( T(0.0) ) );
	m_uiDur0 = (unsigned int)( tT0 ); 
	m_uiDur1 = (unsigned int)( tT0 + tT1 );
	if( !m_uiDur0 ) // Damit bei ( tRatio == T(0.0) ) keine Probleme enstehen.
		m_uiDur0 = 1;
}

template<class T>
T COpTkEnv2Lin<T>::Proc()
{
	if( this->m_uiState )
	{
		T tOut = this->m_tV;
		if( this->m_uiIndex < m_uiDur1 )
		{
			// Stufe 0
			if( this->m_uiIndex < m_uiDur0 )
			{
				this->m_tV += m_tStp0;
				++this->m_uiIndex;
				if( this->m_uiIndex >= m_uiDur0 )
					this->m_tV = T(1.0);
			}
			// Stufe 1
			else
			{
				this->m_tV -= m_tStp1;
				++this->m_uiIndex;
			}
		}
		else
		{
			this->m_tV = T(0.0);
			this->m_uiState = 0;
		}
		return tOut;
	}
	return T(0.0);
}


////////////////////////////////////////////////////////////////////////////////
// COpTkEnv2Exp
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkEnv2Exp<T>::COpTkEnv2Exp()
: COpTkEnv<T>()
, m_uiDur0( 0 )
, m_uiDur1( 0 )
, m_tP( T(0.0) )
, m_tStp0( T(0.0) )
, m_tStp1( T(0.0) )
, m_tFac0( T(0.0) )
, m_tFac1( T(0.0) )
{}

template<class T>
COpTkEnv2Exp<T>::COpTkEnv2Exp( const COpTkEnv2Exp<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkEnv2Exp<T>::~COpTkEnv2Exp()
{}

template<class T>
COpTkEnv2Exp<T> & COpTkEnv2Exp<T>::operator =( const COpTkEnv2Exp<T> &roO )
{
	COpTkEnv<T>::operator =( roO );
	m_uiDur0 = roO.m_uiDur0;
	m_uiDur1 = roO.m_uiDur1;
	m_tP = roO.m_tP;
	m_tStp0 = roO.m_tStp0;
	m_tStp1 = roO.m_tStp1;
	m_tFac0 = roO.m_tFac0;
	m_tFac1 = roO.m_tFac1;
	return *this;
}

template<class T>
T & COpTkEnv2Exp<T>::Position() {
	return m_tP;
}

template<class T>
void COpTkEnv2Exp<T>::Set( T tDur, T tRatio, T tSlope0, T tSlope1 )
{
	OP_TK_RANGE_MINMAX( tRatio, T(0.0), T(1.0) );
	tSlope1 *= T(-1.0);
	if( tDur < T(0.0) )
		tDur = T(0.0);
	if( tSlope0 == T(0.0) )
		tSlope0 = T(1e-11);
	if( tSlope1 == T(0.0) )
		tSlope1 = T(1e-11);
	const T tT0 = rint( tDur * tRatio );
	const T tT1 = rint( tDur - tT0 );
	if( tT0 )
	{
		m_tStp0 = exp( tSlope0 / tT0 );
		m_tFac0 = T(1.0) / ( pow( m_tStp0, tT0 - T(1.0) ) - T(1.0) );
		if( m_tFac0 == T(0.0) )
			m_tStp0 = T(0.0);
	}
	else
	{
		m_tFac0 = m_tStp0 = T(0.0);
	}
	if( tRatio != T(1.0) )
	{
		m_tStp1 = exp( tSlope1 / tT1 );
		m_tFac1 = T(1.0) / ( pow( m_tStp1, tT1 - T(1.0) ) - T(1.0) );
		if( m_tFac1 == T(0.0) )
			m_tStp1 = T(0.0);
	}
	else
	{
		m_tFac1 = m_tStp1 = T(0.0);
	}
	
	m_uiDur0 = (unsigned int)( tT0 ); 
	m_uiDur1 = (unsigned int)( tT0 + tT1 );
	if( !m_uiDur0 ) // Damit bei ( tRatio == T(0.0) ) keine Probleme enstehen.
		m_uiDur0 = 1;
}

template<class T>
T COpTkEnv2Exp<T>::Proc()
{
	if( this->m_uiState )
	{
		if( this->m_uiIndex < m_uiDur1 )
		{
			// Stufe 0
			if( this->m_uiIndex < m_uiDur0 )
			{
				this->m_tV = ( m_tP - T(1.0) ) * m_tFac0;
				m_tP *= m_tStp0;
				++this->m_uiIndex;
				if( this->m_uiIndex >= m_uiDur0 )
				{
					this->m_tV = T(1.0);
					this->m_tP = T(1.0);
				}
			}
			// Stufe 1
			else
			{
				this->m_tV = T(1.0) - ( m_tP - T(1.0) ) * m_tFac1;
				m_tP *= m_tStp1;
				++this->m_uiIndex;
			}
		}
		else
		{
			this->m_tV = T(0.0);
			this->m_uiState = 0;
		}
		return this->m_tV;
	}
	return T(0.0);
}


////////////////////////////////////////////////////////////////////////////////
// COpTkEnv2SinPow
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkEnv2SinPow<T>::COpTkEnv2SinPow()
: COpTkEnv<T>()
, m_uiDur0( 0 )
, m_uiDur1( 0 )
, m_tP( T(0.0) )
, m_tStp0( T(0.0) )
, m_tStp1( T(0.0) )
, m_tExp0( T(0.0) )
, m_tExp1( T(0.0) )
{}

template<class T>
COpTkEnv2SinPow<T>::COpTkEnv2SinPow( const COpTkEnv2SinPow<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkEnv2SinPow<T>::~COpTkEnv2SinPow()
{}

template<class T>
COpTkEnv2SinPow<T> & COpTkEnv2SinPow<T>::operator =( const COpTkEnv2SinPow<T> &roO )
{
	COpTkEnv<T>::operator =( roO );
	m_uiDur0 = roO.m_uiDur0;
	m_uiDur1 = roO.m_uiDur1;
	m_tP = roO.m_tP;
	m_tStp0 = roO.m_tStp0;
	m_tStp1 = roO.m_tStp1;
	m_tExp0 = roO.m_tExp0;
	m_tExp1 = roO.m_tExp1;
	return *this;
}

template<class T>
T & COpTkEnv2SinPow<T>::Position() {
	return m_tP;
}

template<class T>
T & COpTkEnv2SinPow<T>::Exponent0() {
	return m_tExp0;
}

template<class T>
T & COpTkEnv2SinPow<T>::Exponent1() {
	return m_tExp1;
}

template<class T>
void COpTkEnv2SinPow<T>::Set( T tDur, T tRatio, T tSlope0, T tSlope1 )
{
	OP_TK_RANGE_MINMAX( tRatio, T(0.0), T(1.0) );
	if( tDur < T(0.0) )
		tDur = T(0.0);
	if( tSlope0 < T(0.0) )
		tSlope0 = T(0.0);
	if( tSlope1 < T(0.0) )
		tSlope1 = T(0.0);
	const T tT0 = rint( tDur * tRatio );
	const T tT1 = rint( tDur - tT0 );
	if( tT0 )
	{
		m_tStp0 = OP_TK_PI_2 / ( tT0 - T(1.0) );
	}
	else
	{
		m_tStp0 = T(0.0);
	}
	if( tRatio != T(1.0) )
	{
		m_tStp1 = OP_TK_PI_2 / ( tT1 - T(1.0) );
	}
	else
	{
		m_tStp1 = T(0.0);
	}
	
	m_uiDur0 = (unsigned int)( tT0 ); 
	m_uiDur1 = (unsigned int)( tT0 + tT1 );
	if( !m_uiDur0 ) // Damit bei ( tRatio == T(0.0) ) keine Probleme enstehen.
		m_uiDur0 = 1;

	m_tExp0 = tSlope0;
	m_tExp1 = tSlope1;
}

template<class T>
T COpTkEnv2SinPow<T>::Proc()
{
	if( this->m_uiState )
	{
		if( this->m_uiIndex < m_uiDur1 )
		{
			// Stufe 0
			if( this->m_uiIndex < m_uiDur0 )
			{
				this->m_tV = pow( sin( m_tP ), m_tExp0 );
				m_tP += m_tStp0;
				++this->m_uiIndex;
				if( this->m_uiIndex >= m_uiDur0 )
				{
					this->m_tV = T(1.0);
					this->m_tP = T(OP_TK_PI_2);
				}
			}
			// Stufe 1
			else
			{
				this->m_tV = pow( sin( m_tP ), m_tExp1 );
				m_tP += m_tStp1;
				++this->m_uiIndex;
			}
		}
		else
		{
			this->m_tV = T(0.0);
			this->m_uiState = 0;
		}
		return this->m_tV;
	}
	return T(0.0);
}

////////////////////////////////////////////////////////////////////////////////
// COpTkEnv2Bzr
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkEnv2Bzr<T>::COpTkEnv2Bzr()
: COpTkEnv<T>()
, m_uiDur0( 0 )
, m_uiDur1( 0 )
, m_tP( T(0.0) )
, m_tStp0( T(0.0) )
, m_tStp1( T(0.0) )
, m_tSlope0( T(0.0) )
, m_tSlope1( T(0.0) )
{}

template<class T>
COpTkEnv2Bzr<T>::COpTkEnv2Bzr( const COpTkEnv2Bzr<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkEnv2Bzr<T>::~COpTkEnv2Bzr()
{}

template<class T>
COpTkEnv2Bzr<T> & COpTkEnv2Bzr<T>::operator =( const COpTkEnv2Bzr<T> &roO )
{
	COpTkEnv<T>::operator =( roO );
	m_uiDur0 = roO.m_uiDur0;
	m_uiDur1 = roO.m_uiDur1;
	m_tP = roO.m_tP;
	m_tStp0 = roO.m_tStp0;
	m_tStp1 = roO.m_tStp1;
	m_tSlope0 = roO.m_tSlope0;
	m_tSlope1 = roO.m_tSlope1;
	return *this;
}

template<class T>
T & COpTkEnv2Bzr<T>::Position() {
	return m_tP;
}

template<class T>
void COpTkEnv2Bzr<T>::Set( T tDur, T tRatio, T tSlope0, T tSlope1 )
{
	OP_TK_RANGE_MINMAX( tRatio, T(0.0), T(1.0) );
	if( tDur < T(0.0) )
		tDur = T(0.0);
	
	const T tT0 = rint( tDur * tRatio );
	const T tT1 = rint( tDur - tT0 );
	if( tT0 )
		m_tStp0 = T(1.0) / ( tT0 - T(1.0) );
	else
		m_tStp0 = T(0.0);
	if( tRatio != T(1.0) )
		m_tStp1 = T(1.0) / ( tT1 - T(1.0) );
	else
		m_tStp1 = T(0.0);
	
	m_tSlope0 = tSlope0;
	m_tSlope1 = tSlope1;
	
	m_uiDur0 = (unsigned int)( tT0 ); 
	m_uiDur1 = (unsigned int)( tT0 + tT1 );
	if( !m_uiDur0 ) // Damit bei ( tRatio == T(0.0) ) keine Probleme enstehen.
		m_uiDur0 = 1;
}

template<class T>
T COpTkEnv2Bzr<T>::Proc()
{
	if( this->m_uiState )
	{
		if( this->m_uiIndex < m_uiDur1 )
		{
			const T tOneMinusT = T(1.0) - m_tP;
			// Stufe 0
			if( this->m_uiIndex < m_uiDur0 )
			{
				T t = m_tSlope0 * T(2.0) * m_tP * tOneMinusT;
				t += m_tP * m_tP;
				this->m_tV = t;
				
				m_tP += m_tStp0;
				
				++this->m_uiIndex;
				if( this->m_uiIndex >= m_uiDur0 )
				{
					this->m_tV = T(1.0);
					m_tP = T(0.0);
				}
			}
			// Stufe 1
			else
			{
				T t = tOneMinusT * tOneMinusT;
				t += m_tSlope1 * T(2.0) * m_tP * tOneMinusT;
				this->m_tV = t;
				
				m_tP += m_tStp1;
				
				++this->m_uiIndex;
			}

		}
		else
		{
			this->m_tV = T(0.0);
			this->m_uiState = 0;
		}
		return this->m_tV;
	}
	return T(0.0);
}
