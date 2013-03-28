// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTk.inline.h

////////////////////////////////////////////////////////////////////////////////
// COpTkBuffer
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkBuffer<T>::COpTkBuffer()
: m_ptBuffer( 0 )
, m_uiSize( 0 )
, m_uiMax( 0 )
{}

template<class T>
COpTkBuffer<T>::COpTkBuffer( const COpTkBuffer &roO )
{
	*this = roO;
}

template<class T>
COpTkBuffer<T>::~COpTkBuffer()
{}

template<class T>
COpTkBuffer<T> & COpTkBuffer<T>::operator =( const COpTkBuffer<T> &roO )
{
	// m_uiSize in abgeleiteter Klasse behandeln!
	SetMax( roO.m_uiMax );
	if( m_uiSize )
		OP_TK_MEM_COPY( m_ptBuffer, roO.m_ptBuffer, m_uiSize * sizeof(T) );
	return *this;
}

template<class T>
void COpTkBuffer<T>::SetMax( unsigned int uiMax )
{
	if( uiMax > m_uiSize )
		uiMax = m_uiSize;
	m_uiMax = uiMax;
}

template<class T>
void COpTkBuffer<T>::Reset()
{
	if( m_ptBuffer )
		OP_TK_MEM_ZERO( m_ptBuffer, m_uiSize * sizeof(T) );
}

template<class T>
T & COpTkBuffer<T>::AtClamp( unsigned int uiPos ) const
{
	if( uiPos >= m_uiMax )
	{
		uiPos = m_uiMax;
		if( uiPos )
			--uiPos;
	}
	return m_ptBuffer[uiPos];
}

template<class T>
T & COpTkBuffer<T>::AtClamp( int iPos ) const
{
	if( iPos >= (int)m_uiMax )
	{
		iPos = (int)m_uiMax;
		if( iPos )
			--iPos;
	}
	else if( iPos < 0 )
		iPos = 0;
	
	return m_ptBuffer[iPos];
}

template<class T>
T & COpTkBuffer<T>::AtWrap( unsigned int uiPos ) const
{
	if( uiPos >= m_uiMax )
	{
		if( m_uiMax )
			uiPos %= m_uiMax;
		else
			uiPos = 0;
	}
	return m_ptBuffer[uiPos];
}

template<class T>
T & COpTkBuffer<T>::AtWrap( int iPos ) const
{
	if( abs( iPos ) >= (int)m_uiMax )
	{
		if( m_uiMax )
			iPos %= (int)m_uiMax;
		else
			iPos = 0;
	}
	if( iPos < 0 )
		iPos += (int)m_uiMax;
	return m_ptBuffer[iPos];
}


////////////////////////////////////////////////////////////////////////////////
// COpTkBufferFixed
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SIZE>
COpTkBufferFixed<T, SIZE>::COpTkBufferFixed()
: COpTkBuffer<T>()
{
	this->m_ptBuffer = m_atBuffer;
	this->m_uiSize = this->m_uiMax = SIZE;
	this->Reset();
}

template<class T, const unsigned int SIZE>
COpTkBufferFixed<T, SIZE>::COpTkBufferFixed( const COpTkBufferFixed<T, SIZE> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SIZE>
COpTkBufferFixed<T, SIZE> & COpTkBufferFixed<T, SIZE>::operator = ( const COpTkBufferFixed<T, SIZE> &roO )
{
	// m_uiSize muss gleich sein!
	COpTkBuffer<T>::operator =( roO );
	return *this;
}


////////////////////////////////////////////////////////////////////////////////
// COpTkBufferVar
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkBufferVar<T>::COpTkBufferVar()
: COpTkBuffer<T>()
{}

template<class T>
COpTkBufferVar<T>::COpTkBufferVar( const COpTkBufferVar<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkBufferVar<T>::~COpTkBufferVar()
{
	Clear();
}

template<class T>
COpTkBufferVar<T> & COpTkBufferVar<T>::operator = ( const COpTkBufferVar<T> &roO )
{
	if( this->m_uiSize < roO.m_uiSize )
		SetSize( roO.m_uiSize );
	COpTkBuffer<T>::operator =( roO );
	return *this;
}
template<class T>
void COpTkBufferVar<T>::SetSize( unsigned int uiSize )
{
	if( uiSize == this->m_uiSize )
		return;
	if( !uiSize )
	{
		Clear();
		return;
	}
	if( this->m_ptBuffer )
		delete [] this->m_ptBuffer;
	this->m_ptBuffer = new T[ uiSize ];
	this->m_uiSize = uiSize;
	if( this->m_uiMax > uiSize )
		this->m_uiMax = uiSize;
	this->Reset();
}

template<class T>
void COpTkBufferVar<T>::SetSizeAndCopy( unsigned int uiSize )
{
	if( uiSize == this->m_uiSize )
		return;
	if( !uiSize )
	{
		Clear();
		return;
	}
	T *ptNew = new T[ uiSize ];
	if( this->m_uiSize )
	{
		const unsigned int uiMin = OP_TK_MIN( uiSize, this->m_uiSize );
		OP_TK_MEM_COPY( ptNew, this->m_ptBuffer, uiMin * sizeof(T) );
	}
	if( uiSize > this->m_uiSize )
		OP_TK_MEM_ZERO( ptNew + this->m_uiSize, ( uiSize - this->m_uiSize ) * sizeof(T) );
	this->m_uiSize = uiSize;
	if( this->m_uiMax > this->m_uiSize )
		this->m_uiMax = this->m_uiSize;
	if( this->m_ptBuffer )
		delete [] this->m_ptBuffer;
	this->m_ptBuffer = ptNew;
}

template<class T>
void COpTkBufferVar<T>::Clear()
{
	if( this->m_ptBuffer )
	{
		delete [] this->m_ptBuffer;
		this->m_ptBuffer = 0;
	}
	this->m_uiSize = this->m_uiMax = 0;
}

////////////////////////////////////////////////////////////////////////////////
// COpTkBufferVarInterpol
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkBufferVarInterpol<T>::COpTkBufferVarInterpol()
: COpTkBufferVar<T>()
{}

template<class T>
COpTkBufferVarInterpol<T>::COpTkBufferVarInterpol( const COpTkBufferVarInterpol<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkBufferVarInterpol<T>::~COpTkBufferVarInterpol()
{}

template<class T>
COpTkBufferVarInterpol<T> & COpTkBufferVarInterpol<T>::operator = ( const COpTkBufferVarInterpol<T> &roO )
{
	COpTkBufferVar<T>::operator =( roO );
	return *this;
}

template<class T>
T COpTkBufferVarInterpol<T>::ReadWrap( double dPos ) const
{
	double dPosInt;
	const double dPosFrc = modf( dPos, &dPosInt );
	int iPos = (int)dPosInt;
	
	if( this->m_uiMax && (unsigned int)abs( iPos ) >= this->m_uiMax )
		iPos %= (int)this->m_uiMax;
	if( iPos < 0 )
		iPos += (int)this->m_uiMax;
	
	int iPos1 = iPos + 1;
	if( iPos1 >= (int)this->m_uiMax )
		iPos1 = 0;
	return ( 1.0 - dPosFrc ) * this->m_ptBuffer[iPos] 
		+ dPosFrc        * this->m_ptBuffer[iPos1];
}

template<class T>
T COpTkBufferVarInterpol<T>::ReadClamp( double dPos ) const
{
	double dPosInt;
	const double dPosFrc = modf( dPos, &dPosInt );
	int iPos = (int)dPosInt;
	
	if( iPos >= (int)this->m_uiMax )
		return this->m_ptBuffer[this->m_uiMax-1];
	else if( iPos < 0 )
		return this->m_ptBuffer[0];
	
	int iPos1 = iPos + 1;
	if( iPos1 >= (int)this->m_uiMax )
		return this->m_ptBuffer[this->m_uiMax-1];
	
	return ( 1.0 - dPosFrc ) * this->m_ptBuffer[iPos] 
		+ dPosFrc        * this->m_ptBuffer[iPos1];
}
