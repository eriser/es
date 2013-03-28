// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkBufferDelay.inline.h

////////////////////////////////////////////////////////////////////////////////
// COpTkBufferFixedDelay
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SIZE>
COpTkBufferFixedDelay<T, SIZE>::COpTkBufferFixedDelay()
: COpTkBufferFixed<T, SIZE>()
, m_iPos( 0 )
, m_iDelay( 0 )
{}

template<class T, const unsigned int SIZE>
COpTkBufferFixedDelay<T, SIZE>::COpTkBufferFixedDelay( const COpTkBufferFixedDelay<T, SIZE> &roO )
: COpTkBufferFixed<T, SIZE>()
{
	*this = roO;
}

template<class T, const unsigned int SIZE>
COpTkBufferFixedDelay<T, SIZE> & COpTkBufferFixedDelay<T, SIZE>::operator = ( const COpTkBufferFixedDelay<T, SIZE> &roO )
{
	COpTkBufferFixed<T, SIZE>::operator =( roO );
	m_iPos = roO.m_iPos;
	m_iDelay = roO.m_iDelay;
	return *this;
}

template<class T, const unsigned int SIZE>
void COpTkBufferFixedDelay<T, SIZE>::Clear()
{
	COpTkBufferFixed<T, SIZE>::Clear();
	m_iPos = m_iDelay = 0;
}

template<class T, const unsigned int SIZE>
void COpTkBufferFixedDelay<T, SIZE>::Reset( )
{
	COpTkBufferFixed<T, SIZE>::Reset();
	m_iPos = 0;
}

template<class T, const unsigned int SIZE>
void COpTkBufferFixedDelay<T, SIZE>::SetDelay( unsigned int uiDelay )
{
	if( uiDelay > this->m_uiSize )
		uiDelay = this->m_uiSize;
	if( uiDelay )
		--uiDelay;
	this->m_iDelay = (int)uiDelay;
}

template<class T, const unsigned int SIZE>
void COpTkBufferFixedDelay<T, SIZE>::SetDelay( double dDelay )
{
	SetDelay( (unsigned int)( abs( (int)rint( dDelay ) ) ) );
}


////////////////////////////////////////////////////////////////////////////////
// COpTkBufferVarDelay
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkBufferVarDelay<T>::COpTkBufferVarDelay()
: COpTkBufferVar<T>()
, m_iPos( 0 )
, m_iDelay( 0 )
{
}

template<class T>
COpTkBufferVarDelay<T>::COpTkBufferVarDelay( const COpTkBufferVarDelay<T> &roO )
: COpTkBufferVar<T>()
{
	*this = roO;
}

template<class T>
COpTkBufferVarDelay<T> & COpTkBufferVarDelay<T>::operator = ( const COpTkBufferVarDelay<T> &roO )
{
	COpTkBufferVar<T>::operator =( roO );
	m_iPos = roO.m_iPos;
	m_iDelay = roO.m_iDelay;
	return *this;
}

template<class T>
void COpTkBufferVarDelay<T>::Clear( )
{
	COpTkBufferVar<T>::Clear();
	m_iPos = m_iDelay = 0;
}

template<class T>
void COpTkBufferVarDelay<T>::Reset()
{
	COpTkBufferVar<T>::Reset();
	m_iPos = 0;
}

template<class T>
void COpTkBufferVarDelay<T>::SetDelay( unsigned int uiDelay )
{ 
	if( uiDelay > this->m_uiSize )
		this->SetSize( uiDelay + OP_TK_DELAY_ALLOC_ADD );
	if( uiDelay )
		--uiDelay;
	this->m_iDelay = (int)uiDelay;
}

template<class T>
void COpTkBufferVarDelay<T>::SetDelay( double dDelay )
{
	SetDelay( (unsigned int)( abs( (int)rint( dDelay ) ) ) );
}

template<class T>
void COpTkBufferVarDelay<T>::UpdateSize()
{
	this->SetSize( (unsigned int)m_iDelay + 1 );
}

////////////////////////////////////////////////////////////////////////////////
// COpTkBufferVarDelayInterpol
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkBufferVarDelayInterpol<T>::COpTkBufferVarDelayInterpol()
: COpTkBufferVarInterpol<T>()
, m_iPos( 0 )
, m_iDelay( 0 )
, m_dDelayFrc( 0.0 )
{}

template<class T>
COpTkBufferVarDelayInterpol<T>::COpTkBufferVarDelayInterpol( const COpTkBufferVarDelayInterpol<T> &roO )
: COpTkBufferVarInterpol<T>()
{
	*this = roO;
}

template<class T>
COpTkBufferVarDelayInterpol<T> & COpTkBufferVarDelayInterpol<T>::operator = ( const COpTkBufferVarDelayInterpol<T> &roO )
{
	COpTkBufferVarInterpol<T>::operator = ( roO );
	m_iPos = roO.m_iPos;
	m_iDelay = roO.m_iDelay;
	m_dDelayFrc = roO.m_dDelayFrc;
	return *this;
}

template<class T>
void COpTkBufferVarDelayInterpol<T>::Clear()
{
	COpTkBufferVarInterpol<T>::Clear();
	m_iPos = m_iDelay = 0;
	m_dDelayFrc = 0.0;
}

template<class T>
void COpTkBufferVarDelayInterpol<T>::Reset()
{
	COpTkBufferVarInterpol<T>::Reset();
	m_iPos = 0;
}

template<class T>
void COpTkBufferVarDelayInterpol<T>::SetDelay( unsigned int uiDelay )
{
	SetDelay( double( uiDelay ) );
}

template<class T>
void COpTkBufferVarDelayInterpol<T>::SetDelay( double dDelay )
{
	double dDlyInt;
	m_dDelayFrc = modf( fabs( dDelay ), &dDlyInt );
	unsigned int uiDly = (unsigned int)dDlyInt;
	// Für m_uiDelay Samples Verzögerung braucht es min. einen Puffer der Grösse m_uiDelay.
	// Um nun die zusätzlichen Bruchteil eines Samples ebenfalls zu berücksichtigen, wird die
	// noch Puffergrösse um eins erhöht.
	if( uiDly >= this->m_uiSize )
		this->SetSize( uiDly + 1 + OP_TK_DELAY_ALLOC_ADD );
	if( uiDly )
		--uiDly;
	this->m_iDelay = (int)uiDly;
}

template<class T>
void COpTkBufferVarDelayInterpol<T>::UpdateSize()
{
	this->SetSize( (unsigned int)m_iDelay + 1 );
}

template<class T>
T COpTkBufferVarDelayInterpol<T>::ReadDelay() const
{
	int iPosRead0 = m_iPos - m_iDelay;
	if( iPosRead0 < 0 )
		iPosRead0 = (int)this->m_uiSize + iPosRead0;
	int iPosRead1 = iPosRead0 - 1;
	if( iPosRead1 < 0 )
		iPosRead1 = this->m_uiSize - 1;
	
	const T tV0 = this->m_ptBuffer[iPosRead0];
	const T tV1 = this->m_ptBuffer[iPosRead1];
	return tV0 + T(m_dDelayFrc) * ( tV1 - tV0 );
}


template<class T>
T COpTkBufferVarDelayInterpol<T>::ReadDelay( double dDelay ) const
{
	double dDlyInt;
	double dFrc = modf( dDelay, &dDlyInt );
	int iDly = (int)dDlyInt;
	if( iDly > m_iDelay ) // Gegen Unlogik. Maximales Delay.
	{
		iDly =  m_iDelay; 
		dFrc = m_dDelayFrc;
	}
	//if( iDly >= (int)this->m_uiSize - 1 )   // Sollte keine Problemen geben, da
	//	iDly = (int)this->m_uiSize - 2; // die Mindestgrösse gleich 2.0 ist.
	
	int iPosRead0 = m_iPos - iDly;
	
	if( abs( iPosRead0 ) >= (int)this->m_uiSize )
	{
		if( this->m_uiSize )
			iPosRead0 %= (int)this->m_uiSize;
		else
			iPosRead0 = 0;
	}
	if( iPosRead0 < 0 )
		iPosRead0 += (int)this->m_uiSize;
	int iPosRead1 = iPosRead0 - 1;
	if( iPosRead1 < 0 )
		iPosRead1 = this->m_uiSize - 1;
	
	const T tV0 = this->m_ptBuffer[iPosRead0];
	const T tV1 = this->m_ptBuffer[iPosRead1];
	return tV0 + T(dFrc) * ( tV1 - tV0 );
}
