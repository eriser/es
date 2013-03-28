// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Stack.h

#ifndef _STACK_H_
#define _STACK_H_

#define STACK_VERSION	"0.03"

// Dynamischer Stack
////////////////////////////////////////////////////////////////////////////////
template <class T>
class CStack
{
protected:
	class CNode
	{
	public:
		CNode( T tO );
		~CNode();
	public:
		T m_tO;
		CNode *m_poNext;
		CNode *m_poPre;
	};
	
public:
	CStack();
	CStack( const CStack &roStk );
	~CStack();
	
public:
	CStack & operator = ( const CStack &roStk );
	inline   operator T & ();
	inline   operator const T () const;
	CStack   operator + ( const CStack &roStk ) const;
	CStack & operator += ( const CStack &roStk );
	
public:	
	void Push( T tVal );
	void Push( const CStack &roStk );
	bool Pop();
	void Clear();
	
public:
	inline T& Top();
	inline T  Top() const;
	inline unsigned int GetSize() const;
	
protected:
	CNode m_oNodeRoot;
	CNode *m_poNodeTop;
	
	unsigned int m_uiSize;
};

#include "Stack.inline.h"

#endif // _STACK_H_
