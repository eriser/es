// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Array.h

#ifndef _ARRAY_H_
#define _ARRAY_H_

#define ARRAY_VERSION	"0.07" // 2009-02-18

#define ARRAY_DELETE(arr,type) \
	for( unsigned int i=0; i<arr.GetSize(); ++i ) { \
		type *poO = arr[i]; \
		if( poO ) delete poO; \
	} \
	arr.Clear();
#define ARRAY_DELETE_ARRAY(arr,type) \
	for( unsigned int i=0; i<arr.GetSize(); ++i ) { \
		type *poO = arr[i]; \
		if( poO ) delete [] poO; \
	} \
	arr.Clear(); \

// Dynamisches Array
////////////////////////////////////////////////////////////////////////////////
template <class T>
class CArray
{
public:	
	CArray();
	CArray( unsigned int uiSize );
	CArray( unsigned int uiSize, T tValInit );
	CArray( const CArray &roArr );
	~CArray();
	
public:
	CArray &   operator=( const CArray &roArr );
	inline T & operator[]( unsigned int uiIndex );
	inline T   operator[]( unsigned int uiIndex ) const;
	//inline     operator T *();
	//inline     operator const T *() const;
	CArray     operator + ( const CArray &roArr );
	CArray &   operator += ( const CArray &roArr );
	//bool	operator == 
	
public:
	void Clear();
	
	void Fill( T tVal );
	
	void Append( T tVal );
	void Append( const CArray &roArr );
	
	bool Insert( unsigned int uiIndex, T tVal );
	bool Insert( unsigned int uiIndex, const CArray &roArr );
	bool Remove( unsigned int uiIndex );
	bool RemoveEach( T tVal );
	
	bool InsertElements( unsigned int uiIndex, unsigned int uiCount );
	bool RemoveElements( unsigned int uiIndex, unsigned int uiCount );
	
	bool Resize( unsigned int uiSize );
	
public:
	inline T & At( unsigned int uiIndex );
	inline T   At( unsigned int uiIndex ) const;
	unsigned int GetSize() const;
	T* GetData() const;
	
protected:
	static void Copy_( T *ptDst, T *ptSrc, unsigned int uiSize );
	
protected:
	T *m_ptData;
	unsigned int m_uiSize;
};

#include "Array.inline.h"

#endif // _ARRAY_H_
