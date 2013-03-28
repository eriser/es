// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// List.inline.h

////////////////////////////////////////////////////////////////////////////////
// CList::CNode
////////////////////////////////////////////////////////////////////////////////

template <class T>
CList<T>::CNode::CNode()
: m_tO( T() )
, m_poNext( 0 )
, m_poPrev( 0 )
{}

template <class T>
CList<T>::CNode::~CNode()
{}


////////////////////////////////////////////////////////////////////////////////
// CList
////////////////////////////////////////////////////////////////////////////////

template <class T>
CList<T>::CList()
: m_oNodeFront()
, m_oNodeBack()
, m_poNodeGetPos( 0 )
, m_uiSize( 0 )
{
	Clear();
}

template <class T>
CList<T>::CList( const CList &roList )
{
	*this = roList;
}

template <class T>
CList<T>::~CList()
{
	Clear();
}

template <class T>
CList<T> & CList<T>::operator=( const CList<T> &roList )
{
	Clear();
	Copy( this, &roList );
	m_uiSize = roList.m_uiSize;
	
	MoveTo( roList.GetPos() );
	
	return *this;
}

template <class T>
inline T & CList<T>::operator[]( unsigned int uiIndex )
{
	return At( uiIndex );
}

template <class T>
inline T CList<T>::operator[]( unsigned int uiIndex ) const
{
	return At( uiIndex );
}

template <class T>
CList<T> CList<T>::operator + ( const CList<T> &roList ) const
{
	CList oList( *this );
	oList.Insert( m_uiSize, roList );
	return oList;
}

template <class T>
CList<T> & CList<T>::operator += ( const CList<T> &roList )
{
	Insert( m_uiSize, roList );
	return *this;
}

template <class T>
void CList<T>::Clear()
{
	CNode *pN = m_oNodeFront.m_poNext;
	m_oNodeFront.m_poNext = &m_oNodeBack;
	m_oNodeFront.m_poPrev = 0;
	m_oNodeBack.m_poNext = 0;
	m_oNodeBack.m_poPrev = &m_oNodeFront;
	m_poNodeGetPos = &m_oNodeFront;
	while( pN && pN->m_poNext )
	{
		CNode *pT = pN->m_poNext;
		delete pN;
		pN = pT;
	}
	m_uiSize = 0;
}

template <class T>
void CList<T>::Append( const T tO )
{
	CNode *poN = new CNode;
	poN->m_tO = tO;
	CNode * poEnd = m_oNodeBack.m_poPrev;
	poEnd->m_poNext = poN;
	poN->m_poPrev = poEnd;
	poN->m_poNext = &m_oNodeBack;
	m_oNodeBack.m_poPrev = poN;
	++m_uiSize;
}


template <class T>
void CList<T>::Append( const CList &roList )
{
	*this += roList;
}

template <class T>
bool CList<T>::Insert( unsigned int uiIndex, T tO )
{
	if( uiIndex > m_uiSize )
		return false;
	
	CNode *poBegin = ( uiIndex )?( reinterpret_cast<CNode *>( GetNode( uiIndex - 1 )) ):( &m_oNodeFront );
	CNode *poEnd = poBegin->m_poNext;
	
	CNode *poNew = new CNode;
	poNew->m_poNext = poEnd;
	poNew->m_tO = tO;
	
	poBegin->m_poNext = poNew;
	if( poEnd )
		poEnd->m_poPrev = poNew;
	
	++m_uiSize;
	
	return true;
}

template <class T>
bool CList<T>::Insert( unsigned int uiIndex, const CList &roList )
{
	// Spezialfall: List in dieselbe Liste einfügen...
	if( this == &roList )
	{
		return Insert( uiIndex, CList( roList ) ); // Temporäre Kopie!
	}	
	
	if( uiIndex > m_uiSize || !roList.GetSize() )
		return false;
	
	CNode *poBegin = ( uiIndex )?( reinterpret_cast<CNode *>( GetNode( uiIndex - 1 )) ):( &m_oNodeFront );
	CNode *poEnd = poBegin->m_poNext;
	CNode *poN = poBegin;
	CNode *poSrc = roList.m_oNodeFront.m_poNext;	// roList.MoveToFront()
	
	while( poSrc->m_poNext )
	{
		CNode *poNew = new CNode;
		poN->m_poNext = poNew;
		poNew->m_poPrev = poN;
		poN = poNew;
		poN->m_tO = poSrc->m_tO;
		
		poSrc = poSrc->m_poNext;		// roList.GetNext()
	}
	
	poN->m_poNext = poEnd;
	if( poEnd )
		poEnd->m_poPrev = poN;
	
	m_uiSize += roList.m_uiSize;
	
	return true;
}


template <class T>
bool CList<T>::Pop()
{
	return Remove( m_uiSize );
}

template <class T>
bool CList<T>::Remove()
{
	return RemoveNode( m_poNodeGetPos );
}

template <class T>
bool CList<T>::Remove( unsigned int uiIndex )
{
	//CNode * poPre = ( uiIndex )?( reinterpret_cast<CNode *>( GetNode( uiIndex - 1 ) ) ):( &m_oNodeFront );
	return RemoveNode( (CNode *)GetNode( uiIndex ) );
}

template <class T>
bool CList<T>::Remove( unsigned int uiIndex, unsigned int uiCount )
{
	while( uiCount-- )
	{
		if( !Remove( uiIndex ) )
			return false;
	}
	return true;
}

template <class T>
bool CList<T>::RemoveFirst( const T &rtO )
{
	CNode *pN = m_oNodeFront.m_poNext;
	unsigned int i = 0;
	
	while( pN->m_poNext )
	{
		if( pN->m_tO == rtO )
		{
			Remove( i );
			return true;
		}
		pN = pN->m_poNext;
		++i;
	}
	return false;
}

template <class T>
unsigned int CList<T>::RemoveEach( const T &rtO )
{
	unsigned int uiRemoved = 0;
	while( RemoveFirst( rtO ) )
	{
		++uiRemoved;
	}
	return uiRemoved;
}

template <class T>
int CList<T>::FindFirst( const T &rtO ) const
{
	// Workaround für Front/GetNext-Loop!
	CNode *poN = m_oNodeFront.m_poNext;	// MoveToFront()
	int i = 0;
	
	while( poN->m_poNext )
	{
		if( poN->m_tO == rtO )
			return i;
		poN = poN->m_poNext;		// GetNext()
		++i;
	}
	return -1;
}

template <class T>
inline void CList<T>::MoveToFront()
{
	m_poNodeGetPos = &m_oNodeFront;
}

template <class T>
inline void CList<T>::MoveToBack()
{
	m_poNodeGetPos = &m_oNodeBack;
}

template <class T>
inline void CList<T>::MoveTo( int iIndex )
{
	if( iIndex < 0 )
		MoveToFront();
	else if( (unsigned int)iIndex >= m_uiSize )
		MoveToBack();
	else
		m_poNodeGetPos = reinterpret_cast<CNode *>( GetNode( (unsigned int)( iIndex ) ) );
}

template <class T>
inline void CList<T>::Move( int iSteps )
{
	if( iSteps > 0 )
	{
		while( iSteps-- )
		{
			m_poNodeGetPos = m_poNodeGetPos->m_poNext;
			if( !m_poNodeGetPos->m_poNext )
			{
				m_poNodeGetPos = m_oNodeBack.m_poPrev;
				return;
			}
		}
	}
	else if( iSteps < 0 )
	{
		while( iSteps++ )
		{
			m_poNodeGetPos = m_poNodeGetPos->m_poPrev;
			if( !m_poNodeGetPos->m_poPrev )
			{
				m_poNodeGetPos = m_oNodeFront.m_poNext;
				return;
			}
		}
	}
}

template <class T>
int CList<T>::GetPos() const
{
	const CNode *poN = &m_oNodeFront;
	int i = -1;
	
	do
	{
		if( poN == m_poNodeGetPos )
			return i;
		poN = poN->m_poNext;
		++i;
	}
	while( poN );
	return -2;
}

template <class T>
bool CList<T>::GetNext( T *ptO )
{
	m_poNodeGetPos = m_poNodeGetPos->m_poNext;
	if( !m_poNodeGetPos->m_poNext )
		return false;
 	*ptO = m_poNodeGetPos->m_tO;
	return true;
}

template <class T>
bool CList<T>::Next()
{
	m_poNodeGetPos = m_poNodeGetPos->m_poNext;
	if( !m_poNodeGetPos->m_poNext )
		return false;
	return true;
}

template <class T>
inline T & CList<T>::Get() const
{
	return m_poNodeGetPos->m_tO;
}

template <class T>
bool CList<T>::GetPrev( T *ptO )
{
	m_poNodeGetPos = m_poNodeGetPos->m_poPrev;
	if( !m_poNodeGetPos->m_poPrev )
		return false;
 	*ptO = m_poNodeGetPos->m_tO;
	return true;
}

template <class T>
bool CList<T>::Prev()
{
	m_poNodeGetPos = m_poNodeGetPos->m_poPrev;
	if( !m_poNodeGetPos->m_poPrev )
		return false;
	return true;
}

template <class T>
inline T & CList<T>::At( unsigned int uiIndex )
{
	return reinterpret_cast<CNode *>( GetNode( uiIndex ) )->m_tO;
}

template <class T>
inline T CList<T>::At( unsigned int uiIndex ) const
{
	return reinterpret_cast<CNode *>( GetNode( uiIndex ) )->m_tO;
}

template <class T>
inline unsigned int CList<T>::GetSize() const
{
	return m_uiSize;
}


template <class T>
bool CList<T>::RemoveNode( typename CList<T>::CNode * poNode )
{
	if( poNode )
	{
		CNode * poPre = poNode->m_poPrev;
		if( poPre )
		{
			poPre->m_poNext = poNode->m_poNext;
			if( poPre->m_poNext )
				poPre->m_poNext->m_poPrev = poNode->m_poPrev;
			if( m_poNodeGetPos == poNode )
				m_poNodeGetPos = poPre;
			delete poNode;
			--m_uiSize;
			return true;
		}
	}
	return false;
}

template <class T>
void * CList<T>::GetNode( unsigned int uiIndex ) const
{
	CNode *poN = m_oNodeFront.m_poNext;
	
	while( uiIndex-- )
	{
		if( !poN )	   // Speziell: ->m_poNext kann weggelassen werden,
			return 0;  // da sowieso null zurückgegben wird.
		poN = poN->m_poNext;
	}
	
	return poN;
}

template <class T>
void CList<T>::Copy( CList<T> *poListDst, const CList<T> *poListSrc ) const
{
	// Workaround für Front/GetNext-Loop!
	CNode *poSrc = poListSrc->m_oNodeFront.m_poNext;	// roList.MoveToFront()
	
	while( poSrc->m_poNext )
	{
		poListDst->Append( poSrc->m_tO );
		poSrc = poSrc->m_poNext;		// roList.GetNext()
	}
}
