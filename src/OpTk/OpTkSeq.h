// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkSeq.h

#ifndef _OP_TK_SEQ_H_
#define _OP_TK_SEQ_H_

#include "OpTkDEF.h"


// Basisklasse für Folgen mit Speicher
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkSeqSto
{
public:
	COpTkSeqSto();
	COpTkSeqSto( const COpTkSeqSto &roO );
	~COpTkSeqSto();
	COpTkSeqSto & operator= ( const COpTkSeqSto &roO );
public:
	void SetSize( unsigned int uiSize );
	void Clear();
	void Reset();
protected:
	int GetIndex( T tV ) const;
	void Insert( T tV );
	bool InsertIfNew( T tV );
	T GetNext(); // new
public:
	unsigned int m_uiSize, m_uiPos;
protected:
	T *m_ptSto;
};


// Primzahlen-Folge
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkSeqPrime : public COpTkSeqSto<T>
{
public:
	COpTkSeqPrime();
	COpTkSeqPrime( const COpTkSeqPrime &roO );
	~COpTkSeqPrime();
	COpTkSeqPrime & operator= ( const COpTkSeqPrime &roO );
public:
	T Proc();
	void Reset();
	void SetSize( unsigned int uiSize );
	void SetMin( unsigned int uiMin );
	void Precalc(); // Von 0 bis m_uiPos.
protected:
	unsigned int m_uiMin;
};


// Recamán-Folge
// 1, 3, 6, 2, 7, 13, 20, 12, 21, 11, ... (Sloane's A005132).
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkSeqRecaman : public COpTkSeqSto<T>
{
public:
	COpTkSeqRecaman();
	COpTkSeqRecaman( const COpTkSeqRecaman &roO );
	~COpTkSeqRecaman();
	COpTkSeqRecaman & operator= ( const COpTkSeqRecaman &roO );
public:
	T Proc();
	void Init();
	void Reset();
	void Precalc(); // Von 0 bis m_uiPos. m_iIndex / m_tV müssen initialisiert sein.
public:
	T m_tV, m_tInitV;
	int m_iIndex, m_iInitIndex;
};


// Audioactive Sequence ("Conway's Sequence" or "Look and Say Sequence").
// 1, 11, 21, 1211, 111221, 312211, 13112221, ... (Sloane's A005150).
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkSeqAAS : public COpTkSeqSto<T>
{
public:
	COpTkSeqAAS();
	COpTkSeqAAS( const COpTkSeqAAS &roO );
	~COpTkSeqAAS();
	COpTkSeqAAS & operator =( const COpTkSeqAAS &roO );
public:
	T Proc();
	void Reset();
	void Precalc( unsigned int uiSize, T tInitSeq=T(1) );
protected:
};


// Fibonacci-Folge
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkSeqFibonacci
{
public:
	COpTkSeqFibonacci();
	COpTkSeqFibonacci( const COpTkSeqFibonacci &roO );
	~COpTkSeqFibonacci();
	COpTkSeqFibonacci & operator= ( const COpTkSeqFibonacci &roO );
public:
	inline T Proc()
	{
		const T tV2 = m_tV0 + m_tV1;
		m_tV0 = m_tV1;
		m_tV1 = tV2;
		return m_tV0;
	}
	inline void Reset()
	{
		m_tV0 = T(0);
		m_tV1 = T(1);
	}
public:
	T m_tV0, m_tV1;
};


// Logistische Folge
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkSeqLogistic
{
public:
	COpTkSeqLogistic();
	COpTkSeqLogistic( const COpTkSeqLogistic &roO );
	~COpTkSeqLogistic();
	COpTkSeqLogistic & operator= ( const COpTkSeqLogistic &roO );
public:
	inline T Proc() {
		return m_tV *= m_tC * ( T(1.0) - m_tV );
	}
	inline void Reset() {
		m_tV = m_tV0;
	}
	
public:
	T m_tC, m_tV, m_tV0;
};


// Mandelbrot-Folge
// - Um Julia-Folgen zu berechnen setze man m_tX/Y, anstatt Reset() aufzu-
//   rufen.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkSeqMandelbrot
{
public:
	COpTkSeqMandelbrot();
	COpTkSeqMandelbrot( const COpTkSeqMandelbrot &roO );
	~COpTkSeqMandelbrot();
	COpTkSeqMandelbrot & operator= ( const COpTkSeqMandelbrot &roO );
public:
	T Proc();
	void Reset();
public:
	T m_tX, m_tY;
	T m_tA, m_tB;
};


// Zufallszahlen [0.0,1.0]
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkSeqRandom
{
public:
	COpTkSeqRandom();
	COpTkSeqRandom( const COpTkSeqRandom &roO );
	COpTkSeqRandom & operator =( const COpTkSeqRandom &roO );
public:
	T Proc();
	void Reset();
public:
	int m_iS0, m_iS1;
	int m_iSeed;
};


// Zufallszahlen [-1.0,1.0], mehr Kontrolle
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkSeqRandomEx : public COpTkSeqRandom<T>
{
public:
	enum EType
	{ WHITE, PINK, TYPE_MAX };
public:
	COpTkSeqRandomEx( unsigned int uiType=WHITE );
	COpTkSeqRandomEx( const COpTkSeqRandomEx &roO );
	COpTkSeqRandomEx & operator =( const COpTkSeqRandomEx &roO );
public:
	unsigned int & Type();
	void SetBalance( T tB );
	void NextType();
	T Proc();
	void Reset();
protected:
	unsigned int m_uiType;
	T m_tB;
	T m_tV0, m_tV1, m_tV2;
	T m_tVL;
};


// 1-D-Kollisionen.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkSeqCollision1D
{
protected:
	struct SParticle {
		float m_fVel, m_fMass, m_fExt, m_fPos;
	};
public:
	COpTkSeqCollision1D();
	COpTkSeqCollision1D( const COpTkSeqCollision1D &roO );
	~COpTkSeqCollision1D();
	COpTkSeqCollision1D & operator= ( const COpTkSeqCollision1D &roO );
public:
	void Proc();
	void SetCount( unsigned int uiCount );
	inline unsigned int GetCount() const { return m_uiCount; }
	void Reset();
protected:
	void Clear();
public:
	bool *m_pbCollision;
	T m_tDamp;
	COpTkSeqRandom<float> m_oRand;
protected:
	SParticle *m_poP;
	unsigned int m_uiCount;
};


// 1-D-Automaten.
// - Min. 3 Zellen, 1 Zustande und 1 Nachbar. Ungültige Parameter werden
//   auf korrigiert.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkSeqCell1D
{
public:
	COpTkSeqCell1D();
	COpTkSeqCell1D( const COpTkSeqCell1D &roO );
	~COpTkSeqCell1D();
	COpTkSeqCell1D & operator= ( const COpTkSeqCell1D &roO );
public:
	T Proc();
	void Init( unsigned int uiCount, unsigned int uiStates, unsigned int uiNeighbours );
	inline unsigned int GetCount() const { return m_uiCount; }
	void Randomize();
protected:
	void Clear();
	void Gen();
public:
	unsigned int m_uiCount;
	COpTkSeqRandom<float> m_oRand;
protected:
	unsigned int m_uiCellScope;
	unsigned int m_uiNeighbours;
	unsigned int m_uiIndex;
	unsigned int m_uiCountRule;
	unsigned int m_uiStates;
	T *m_atRule;
	T *m_atCell, *m_atCellTmp;
};


#include "OpTkSeq.inline.h"

#endif // _OP_TK_SEQ_H_
