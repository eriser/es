// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// List.h

#ifndef _LIST_H_
#define _LIST_H_

#define LIST_VERSION	"0.09"

#define LIST_DELETE(list,type) \
	list.MoveToFront(); \
	while( list.Next() ) { \
		type *poO = list.Get(); \
		if( poO ) delete poO; \
	} \
	list.Clear(); \

// Doppelt verkettete Liste
////////////////////////////////////////////////////////////////////////////////
template <class T>
class CList
{
protected:
	class CNode
	{
	public:
		CNode();
		~CNode();
	public:
		T m_tO;
		CNode *m_poNext;
		CNode *m_poPrev;
	};
	
public:
	CList();
	CList( const CList &roLst );
	~CList();
	
public:
	CList &    operator=( const CList &roLst );		// Position des Pointers wird kopiert.
	inline T & operator[]( unsigned int uiIndex );		// Achtung: Overhead!
	inline T   operator[]( unsigned int uiIndex ) const;	// Achtung: Overhead!
	CList      operator + ( const CList &roLst ) const;
	CList &    operator += ( const CList &roLst );
	//bool operator == ( const CList &roLst ) const;
	
public:
	void Clear();
	
	void Append( const T tO );
	void Append( const CList &roLst );
	
	bool Insert( unsigned int uiIndex, T tO );
	bool Insert( unsigned int uiIndex, const CList &roLst );
	
	bool Pop();
	bool Remove();						// Den aktuellen Knoten entfernen. Erlaubt in  Next()/Get()-Loop.
	bool Remove( unsigned int uiIndex );
	bool Remove( unsigned int uiIndex, unsigned int uiCount );
	
	bool RemoveFirst( const T &rtO );
	unsigned int RemoveEach( const T &rtO );
	
	int FindFirst( const T &rtO ) const;
	
public:
	inline void MoveToFront();				// Position muss vor jeder Next-/Get- 
	inline void MoveToBack();				// bzw. Prev-/Get-Seq. gesetzt werden.
	inline void MoveTo( int iIndex );			// MoveToFront(), MoveToBack() setzen den Pointer
	inline void Move( int iSteps );				// auf die ungültige Positionen back/head, es
								// muss ein Next/Prev bzw. GetNext/GetPrev folgen.
								// Move() resultiert in immer gültigen Positionen wenn 
								// die Liste nicht leer ist.
								// MoveTo( i ) verhält sich wie Move(), ausser wenn
								// i < 0 oder i >= m_uiSize ist, dann entsprecht wird
								// MoveToFront() bzw. MoveToBack() aufgerufen.
	
	int GetPos() const;					// Aktuelle Position des Pointers. -1 ist gültig und entspricht
								// verweist auf den Anfangsknoten (Zusand wie nach
								// einem Aufruf von MoveToFront()). Rückgabewerte kleiner als -1
								// sind ungültig.
	
	bool GetNext( T * ptO );				// Next(); Get(); Jedoch ohne Schreibzugriff!
	bool Next();						// Nächstes Element.
	inline T & Get() const;					// Element auslesen.
	
	bool GetPrev( T * ptO );				// analog zu oben.
	bool Prev();
	
	inline T & At( unsigned int uiIndex );			// Achtung: Overhead!
	inline T   At( unsigned int uiIndex ) const;		// Achtung: Overhead!
	inline unsigned int GetSize() const;
	
protected:
	bool RemoveNode( CNode * poPre );
	
	void * GetNode( unsigned int uiIndex )  const;		// rem: CNode * macht Probleme... deswegen void *!
	void Copy( CList *poLstDst, const CList *poLstSrc ) const;
	
protected:
	CNode m_oNodeFront, m_oNodeBack;
	CNode *m_poNodeGetPos;
	unsigned int m_uiSize;
};

#include "List.inline.h"

#endif // _LIST_H_
