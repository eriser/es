// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkOsc.h

#ifndef _OP_TK_OSC_H_
#define _OP_TK_OSC_H_

#include "OpTkDEF.h"
#include "OpTkSeq.h"

// Oszillator.
// - Die Phase beeinflusst das Sync-Signal.
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkOsc
{
public:
	COpTkOsc();
	COpTkOsc( const COpTkOsc &roO );
	COpTkOsc & operator = ( const COpTkOsc &roO );
	inline int & Period() { return m_iPeriod; }
	inline T & Pos() {
		return m_tPos;
	}
	inline void Reset() {
		m_tPos = m_tPosPhase = T(0.0);
		m_iPeriod = 0;
	}
	inline T GetFreq() const {
		return m_tFreq;
	}
	inline void SetFreq( T tFreq ) {
		m_tFT = T( OP_TK_PI2 * tFreq ) * T( 1.0 / SAMPLE_FREQ );
	}
	inline T GetPhase() const {
		return m_tPhase;
	}
	inline void SetPhase( T tPhase ) {
		m_tPhase = tPhase;
	}
	inline void SetPhaseZero() {
		m_tPhase = 0.0;
	}
	inline bool Sync()
	{
		const int iPCurr = (int)( m_tPosPhase * T( 1.0 / OP_TK_PI2 ) );
		if( iPCurr != m_iPeriod )
		{
			m_iPeriod = iPCurr;
			return true;
		}
		return false;
	}
protected:
	inline void UpdatePosPhase() {
		m_tPosPhase = m_tPos + m_tPhase;
	}

protected:
	T m_tFreq;
	T m_tPhase;
	T m_tPos;
	T m_tFT;
	T m_tPosPhase; // Temporär. Wird immer neu berechnet.
	int m_iPeriod;
};

// Sinus Oscillator.
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkOscSin : public COpTkOsc<T, SAMPLE_FREQ>
{
public:
	COpTkOscSin();
	COpTkOscSin( const COpTkOscSin &roO );
	COpTkOscSin & operator =( const COpTkOscSin &roO );
	inline T Proc()
	{
		this->UpdatePosPhase();
		const T tOut = sin( this->m_tPosPhase );
		this->m_tPos += this->m_tFT;
		return tOut;
	}
};

// Variabler Oscillator.
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkOscSinVar : public COpTkOscSin<T, SAMPLE_FREQ>
{
public:
	enum EType
	{ SIN, SQR, SAW, _TYPE_MAX };
public:
	COpTkOscSinVar( unsigned int uiType=SIN );
	COpTkOscSinVar( const COpTkOscSinVar &roO );
	COpTkOscSinVar & operator =( const COpTkOscSinVar &roO );
	inline unsigned int & Type() { return m_uiType; }
	void NextType();
	T Proc();
protected:
	unsigned int m_uiType;
};

// Fourier-Reihen-Oscillator.
// - Die Amplitude wird um ~18.0% vermindert (Gibbs) und sollte somit immer
//   innerhalb [-1.0,+1.0] liegen.
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkOscApprox : public COpTkOsc<T, SAMPLE_FREQ>
{
public:
	enum EType
	{ PLS, TRI, SAW, _TYPE_MAX };
public:
	COpTkOscApprox( unsigned int uiType=PLS, unsigned int uiHarmon=12 );
	COpTkOscApprox( const COpTkOscApprox &roO );
	COpTkOscApprox & operator =( const COpTkOscApprox &roO );
	inline unsigned int & Type() { return m_uiType; }
	inline unsigned int & Harmon() { return m_uiHarmon; }
	void SetHarmon( unsigned int uiHarmon );
	void NextType();
	T Proc();
protected:
	unsigned int m_uiType;
	unsigned int m_uiHarmon;
};

// Lookup Table Oscillator.
// CAUTION: Changing the size, invalidates the internal frequency and phase
// settings! SetFreq()/SetPhase() MUST be called after NextSize()/SetSize()!
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkOscLUT : public COpTkOsc<T, SAMPLE_FREQ>
{
public:
	enum EType
	{ SIN, SQR, SAW, TRI, RND, _TYPE_MAX };
public:
	COpTkOscLUT( unsigned int uiType=SIN );
	COpTkOscLUT( const COpTkOscLUT &roO );
	COpTkOscLUT & operator =( const COpTkOscLUT &roO );
	inline unsigned int & Type() { return m_uiType; }
	inline unsigned int & Size() { return m_uiSize; }
	void Init();
	void NextType();
	void NextSize(); // CAUTION! Invalidates freq/phase! Init() must be called as well!
	bool SetSize( unsigned int uiExp ); // CAUTION! Invalidates freq/phase! Init() must be called as well!
	inline void SetFreq( T tFreq ) { // Depends on m_uiSize!
		this->m_tFT = tFreq * m_tSizeFac;
	}
	inline void SetPhase( T tPhase ) { // "Backward compatibility"...
		SetPhaseDirect( tPhase * T( 1.0 / OP_TK_PI2 ) );
	}
	inline void SetPhaseDirect( T tPhase ) { // Depends on m_uiSize!
		this->m_tPhase = tPhase * m_uiSize;
	}
	inline bool Sync()
	{
		const int iPCurr = ( (unsigned int)( this->m_tPosPhase ) & ( ~m_uiLUTBitmask ) ); // + 0.5 -> round?
		//const int iPCurr = (int)( m_tPosPhase * T( 1.0 / m_uiSize ) /** T( 1.0 / OP_TK_PI2 )*/ );
		if( iPCurr != this->m_iPeriod )
		{
			this->m_iPeriod = iPCurr;
			return true;
		}
		return false;
	}
	T Proc();
public:
	static const unsigned int LUT_SIZE_MAX = 4096;
	static const unsigned int LUT_SIZE_MIN = 4;
protected:
	unsigned int m_uiType;
	unsigned int m_uiSize;
	unsigned int m_uiLUTBitmask;
	T m_tSizeFac;
	T m_atLUT[LUT_SIZE_MAX];
};

#include "OpTkOsc.inline.h"

#endif // _OP_TK_OSC_H_
