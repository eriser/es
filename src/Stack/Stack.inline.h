// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Stack.inline.h

////////////////////////////////////////////////////////////////////////////////
// CStack::CNode
////////////////////////////////////////////////////////////////////////////////

template <class T>
CStack<T>::CNode::CNode( T tO )
: m_tO( tO )
, m_poNext( 0 )
, m_poPre( 0 )
{

}

template <class T>
CStack<T>::CNode::~CNode()
{

}


////////////////////////////////////////////////////////////////////////////////
// CStack
////////////////////////////////////////////////////////////////////////////////

template <class T>
CStack<T>::CStack()
: m_oNodeRoot( T(0) )
, m_poNodeTop( &m_oNodeRoot )
, m_uiSize( 0 )
{
	
}

template <class T>
CStack<T>::CStack( const CStack<T> &roStk )
: m_oNodeRoot( T(0) )
, m_poNodeTop( &m_oNodeRoot )
, m_uiSize( 0 )
{
	*this = roStk;
}

template <class T>
CStack<T>::~CStack()
{
	Clear();
}

template <class T>
CStack<T> & CStack<T>::operator=( const CStack<T> &roStk )
{
	Clear();
	Push( roStk );
	// m_uiSize, m_poNodeTop werden in Push behandelt.
	
	return *this;
}

template <class T>
inline CStack<T>::operator T & ()
{
	return m_poNodeTop->m_tO; // rem: Wenn ( m_uiSize == 0 ) ist m_tO beim root-Node 0!
}

template <class T>
inline CStack<T>::operator const T () const
{
	return m_poNodeTop->m_tO; // rem: Wenn ( m_uiSize == 0 ) ist m_tO beim root-Node 0!
}

template <class T>
CStack<T> CStack<T>::operator + ( const CStack &roStk ) const
{
	CStack oStk( *this );
	oStk += roStk;
	return oStk;
}

template <class T>
CStack<T> & CStack<T>::operator += ( const CStack &roStk )
{
	Push( roStk );
	return *this;
}

template <class T>
void CStack<T>::Push( T tVal )
{
	CNode *poNew = new CNode( tVal );
	poNew->m_poPre = m_poNodeTop;
	m_poNodeTop->m_poNext = poNew;
	m_poNodeTop = poNew;
	++m_uiSize;
}

template <class T>
void CStack<T>::Push( const CStack<T> &roStk )
{
	CNode *poN = roStk.m_oNodeRoot.m_poNext;
	while( poN )
	{
		Push( poN->m_tO );
		poN = poN->m_poNext;
	}
}

template <class T>
bool CStack<T>::Pop()
{
	if( m_uiSize )
	{
		CNode *poTopOld = m_poNodeTop;
		m_poNodeTop = m_poNodeTop->m_poPre;
		m_poNodeTop->m_poNext = 0;
		delete poTopOld;
		--m_uiSize;
		return ( m_uiSize != 0 );
	}
	return false;
}

template <class T>
void CStack<T>::Clear()
{
	while( Pop() )
	{}
	m_poNodeTop = &m_oNodeRoot;
}

template <class T>
inline T& CStack<T>::Top()
{
	return m_poNodeTop->m_tO; // rem: Wenn ( m_uiSize == 0 ) ist m_tO beim root-Node 0!
}

template <class T>
inline T CStack<T>::Top() const
{
	return m_poNodeTop->m_tO; // rem: Wenn ( m_uiSize == 0 ) ist m_tO beim root-Node 0!
}

template <class T>
inline unsigned int CStack<T>::GetSize() const
{
	return m_uiSize;
}

