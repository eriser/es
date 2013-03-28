// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Array.inline.h

////////////////////////////////////////////////////////////////////////////////
// CArray
////////////////////////////////////////////////////////////////////////////////

template <class T>
CArray<T>::CArray()
: m_ptData( 0 )
, m_uiSize( 0 )
{

}

template <class T>
CArray<T>::CArray( unsigned int uiSize )
{
	m_uiSize = uiSize;
	m_ptData = new T[ m_uiSize ];
}

template <class T>
CArray<T>::CArray( unsigned int uiSize, T tValInit )
{
	m_uiSize = uiSize;
	m_ptData = new T[ m_uiSize ];
	Fill( tValInit );
}

template <class T>
CArray<T>::CArray( const CArray<T> &roArr )
: m_ptData( 0 )
, m_uiSize( 0 )
{
	*this = roArr;
}

template <class T>
CArray<T>::~CArray()
{
	Clear();
}

template <class T>
CArray<T> & CArray<T>::operator=( const CArray<T> &roArr )
{
	Clear();
	m_uiSize = roArr.m_uiSize;
	if( m_uiSize )
	{
		m_ptData = new T[ m_uiSize ];
		Copy_( m_ptData, roArr.m_ptData, m_uiSize );
	}
	else
	{
		m_ptData = 0;
	}
	return *this;
}

template <class T>
inline T & CArray<T>::operator[]( unsigned int uiIndex )
{
	return m_ptData[uiIndex];
}

template <class T>
inline T CArray<T>::operator[]( unsigned int uiIndex ) const
{
	return m_ptData[uiIndex];
}

/*
template <class T>
inline CArray<T>::operator T *()
{
	return m_ptData;
}

template <class T>
inline CArray<T>::operator const T *() const
{
	return m_ptData;
}
*/

template <class T>
CArray<T> CArray<T>::operator + ( const CArray<T> &roArr )
{
	CArray oArr( m_uiSize + roArr.m_uiSize );
	Copy_( oArr.m_ptData,            m_ptData,       m_uiSize );
	Copy_( oArr.m_ptData + m_uiSize, roArr.m_ptData, roArr.m_uiSize );
	return oArr;
}

template <class T>
CArray<T> & CArray<T>::operator += ( const CArray<T> &roArr )
{
	return *this = *this + roArr;
}


template <class T>
void CArray<T>::Clear()
{
	if( m_ptData )
		delete [] m_ptData;
	m_ptData = 0;
	m_uiSize = 0;
}

template <class T>
void CArray<T>::Fill( const T tVal )
{
	unsigned int i = m_uiSize;
	while( i )
	{
		--i;
		m_ptData[i] = tVal;
	}
}

template <class T>
void CArray<T>::Append( T tVal )
{
	T *ptDataOld = m_ptData;
	
	m_ptData = new T[ m_uiSize + 1 ];
	Copy_( m_ptData, ptDataOld, m_uiSize );
	
	m_ptData[ m_uiSize ] = tVal;
	++m_uiSize;
	
	if( ptDataOld )
		delete [] ptDataOld;
}

template <class T>
void CArray<T>::Append( const CArray<T> &roArr )
{
	*this += roArr;
}

template <class T>
bool CArray<T>::Insert( unsigned int uiIndex, T tVal )
{
	if( InsertElements( uiIndex, 1 ) )
	{
		m_ptData[uiIndex] = tVal;
		return true;
	}
	return false;
}

template <class T>
bool CArray<T>::Insert( unsigned int uiIndex, const CArray<T> &roArr )
{
	if( InsertElements( uiIndex, roArr.m_uiSize ) )
	{
		Copy_( m_ptData + uiIndex, roArr.m_ptData, roArr.m_uiSize );
		return true;
	}
	return false;
}

template <class T>
bool CArray<T>::InsertElements( unsigned int uiIndex, unsigned int uiCount )
{
	T *ptDataOld = m_ptData;
	unsigned int uiSizeTotal = m_uiSize + uiCount;
	unsigned int uiSizeEnd = uiSizeTotal - uiCount - uiIndex;
	
	if( uiIndex > m_uiSize || !uiCount )
		return false;	
	
	m_uiSize = uiSizeTotal;
	m_ptData = new T[ m_uiSize ];

	
	if( uiIndex )
		Copy_( m_ptData, ptDataOld, uiIndex );
	//if( uiCount ) 
	//	-zero-( m_ptData + uiIndex, roArr.m_ptData, uiCount );
	if( uiSizeEnd )
		Copy_( m_ptData + uiIndex + uiCount, ptDataOld + uiIndex, uiSizeEnd );
	
	if( ptDataOld )
		delete [] ptDataOld;
	
	return true;
}

template <class T>
bool CArray<T>::Remove( unsigned int uiIndex )
{
	return RemoveElements( uiIndex, 1 );
}

template <class T>
bool CArray<T>::RemoveEach( T tVal )
{
	const unsigned int uiSizePrev = m_uiSize;
	unsigned int i = m_uiSize;
	do
	{
		--i;
		if( m_ptData[i] == tVal )
			Remove( i );
	}
	while( i );
	return ( uiSizePrev != m_uiSize );
}

template <class T>
bool CArray<T>::RemoveElements( unsigned int uiIndex, unsigned int uiCount )
{
	T *ptDataOld = m_ptData;
	unsigned int uiSizeEnd = m_uiSize - uiIndex - uiCount;
	
	if( !uiCount || ( uiIndex + uiCount ) > m_uiSize )
		return false;
	
	m_uiSize = m_uiSize - uiCount;
	m_ptData = new T[ m_uiSize ];
	
	if( uiIndex )
		Copy_( m_ptData, ptDataOld, uiIndex );
	if( uiSizeEnd )
		Copy_( m_ptData + uiIndex, ptDataOld + uiIndex + uiCount, uiSizeEnd );
	
	if( ptDataOld )
		delete [] ptDataOld;
	
	return true;
}

template <class T>
bool CArray<T>::Resize( unsigned int uiSize )
{
	T *ptDataOld = m_ptData;
	unsigned int uiSizeOld = m_uiSize;
	
	if( uiSize )
	{
		m_uiSize = uiSize;
		m_ptData = new T[ m_uiSize ];
		if( ptDataOld )
		{
			Copy_( m_ptData, ptDataOld, ( uiSize < uiSizeOld )?( uiSize ):( uiSizeOld ) );
			delete [] ptDataOld;
		}
		
		return true;
	}
	return false;
}

template <class T>
inline T & CArray<T>::At( unsigned int uiIndex )
{
	return m_ptData[uiIndex];
}

template <class T>
inline T   CArray<T>::At( unsigned int uiIndex ) const
{
	return m_ptData[uiIndex];
}

template <class T>
unsigned int CArray<T>::GetSize() const
{
	return m_uiSize;
}

template <class T>
T* CArray<T>::GetData() const
{
	return m_ptData;
}

template <class T>
void CArray<T>::Copy_( T *ptDst, T *ptSrc, unsigned int uiSize )
{
	while( uiSize )
	{
		*ptDst++ = *ptSrc++;
		--uiSize;
	}
}

