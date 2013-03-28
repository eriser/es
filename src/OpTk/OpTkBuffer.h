// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkBuffer.h

#ifndef _OP_TK_BUFFER_H_
#define _OP_TK_BUFFER_H_

#include "OpTkDEF.h"

// Buffer.
// - Der Max-Wert erlaubt die logische Grösse effizient zu ändern, alle
//   generischen Read*-/Write*-Methoden beziehen sich auf diese Variabel.
// - Wenn Max nicht angerührt wird, soll sich der Puffer so verhalten, als 
//   würde man nur die Kontrolle über den Size-Parameter verwenden.
// - Ausser direktes At/Read/Write werden alle Index so evaluiert, dass jeder
//   Index einen gültigen Wert zurück liefert (clamped/wrapped).
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkBuffer
{
public:
	COpTkBuffer();
	COpTkBuffer( const COpTkBuffer &roO );
	~COpTkBuffer();
	COpTkBuffer & operator = ( const COpTkBuffer &roO );
	void SetMax( unsigned int uiMax );
	void Reset();
	inline T & At( unsigned int uiPos ) const
	{
		return m_ptBuffer[uiPos];
	}
	T & AtClamp( unsigned int uiPos ) const;
	T & AtClamp( int iPos ) const;
	T & AtWrap( unsigned int uiPos ) const;
	T & AtWrap( int iPos ) const;
	inline T Read( unsigned int uiPos ) const {
		return At( uiPos );
	}
	inline void Write( unsigned int iPos, T tIn ) {
		At( iPos ) = tIn;
	}
	inline T ReadClamp( unsigned int uiPos ) const {
		return AtClamp( uiPos );
	}
	inline T ReadClamp( int iPos ) const {
		return AtClamp( iPos );
	}
	inline void WriteClamp( unsigned int uiPos, T tIn ) {
		AtClamp( uiPos ) = tIn;
	}
	inline void WriteClamp( int iPos, T tIn ) {
		AtClamp( iPos ) = tIn;
	}
	inline T ReadWrap( unsigned int uiPos ) const {
		return AtWrap( uiPos );
	}
	inline T ReadWrap( int iPos ) const {
		return AtWrap( iPos );
	}
	inline void WriteWrap( unsigned int uiPos, T tIn ) {
		AtWrap( uiPos ) = tIn;
	}
	inline void WriteWrap( int iPos, T tIn ) {
		AtWrap( iPos ) = tIn;
	}
	inline unsigned int GetSize() const {
		return m_uiSize;
	}
	inline unsigned int GetMax() const {
		return m_uiMax;
	}
protected:
	T *m_ptBuffer;
	unsigned int m_uiSize;
	unsigned int m_uiMax;
};

// Buffer. Fixed Size.
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SIZE>
class COpTkBufferFixed : public COpTkBuffer<T>
{
public:
	COpTkBufferFixed();
	COpTkBufferFixed( const COpTkBufferFixed &roO );
	COpTkBufferFixed & operator = ( const COpTkBufferFixed &roO );
protected:
	T m_atBuffer[SIZE];
};

// Buffer. Variable Size.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkBufferVar : public COpTkBuffer<T>
{
public:
	COpTkBufferVar();
	COpTkBufferVar( const COpTkBufferVar &roO );
	~COpTkBufferVar();
	COpTkBufferVar & operator = ( const COpTkBufferVar &roO );
	void SetSize( unsigned int uiSize );
	void SetSizeAndCopy( unsigned int uiSize );
protected:
	void Clear();
};

// Time-Varying Buffer. Variable Size. Interpolating.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkBufferVarInterpol : public COpTkBufferVar<T>
{
public:
	COpTkBufferVarInterpol();
	COpTkBufferVarInterpol( const COpTkBufferVarInterpol &roO );
	~COpTkBufferVarInterpol();
	COpTkBufferVarInterpol & operator = ( const COpTkBufferVarInterpol &roO );
	T ReadWrap( double dPos ) const;
	T ReadClamp( double dPos ) const;
	//inline void Write( double dPos, T tIn ) // wanted: Deinterpolation...
};

#include "OpTkBuffer.inline.h"

#endif // _OP_TK_BUFFER_H_
