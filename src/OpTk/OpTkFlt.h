// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkOsc.h

#ifndef _OP_TK_FLT_H_
#define _OP_TK_FLT_H_

#include "OpTkDEF.h"

// rem:
// - Status-Variablen sind public.
// - Resonanz-Werte: [0,1] -> [keine,max.] Resonanz.


// Allpass-Filter erster Ordnung
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkFltAllpass1
{
public:
	COpTkFltAllpass1();
	COpTkFltAllpass1( const COpTkFltAllpass1 &roO );
	COpTkFltAllpass1 & operator =( const COpTkFltAllpass1 &roO );
	
	inline void Init( T tDelay ) {
		m_tCoeff = CalcCoeff_( tDelay );
	}
	inline T Proc( T tIn ) {
		return Proc( tIn, m_tCoeff );
	} 
	inline T Proc( T tIn, T tCoeff )
	{
		const T tOut = - tIn * tCoeff + m_tOut;
		m_tOut = tOut * tCoeff + tIn;
		return tOut;
	}
	inline void Reset() {
		m_tOut = T(0.0);
	}
public:
	static inline T CalcCoeff_( T tDelay ) {
		return ( T(1.0) - tDelay ) / ( T(1.0) + tDelay );
	}

public:
	T m_tOut;
protected:
	T m_tCoeff;
};


// Allpass-Filter zweiter Ordnung
//
// Transfer-Funktion:
//        a1 + a0 * z^-1 + z^-2      // forward
// H(z) = -------------------------  //
//        1 + a0 * z^-1 + a1 * z^-2  // backward
//
// rem: Zuerst Radius setzen, dann Frequenz.
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkFltAllpass2
{
public:
	COpTkFltAllpass2();
	COpTkFltAllpass2( const COpTkFltAllpass2 &roO );
	COpTkFltAllpass2 & operator =( const COpTkFltAllpass2 &roO );
	
	void Init( T tFreq, T tRadius );
	inline void InitRadius( T tRadius )
	{
		m_tA1 = tRadius * tRadius;
		m_tRadius = tRadius;
	}
	inline void InitFreq( T tFreq ) {
		m_tA0 = T(-2.0) * m_tRadius * cos( tFreq / T(SAMPLE_FREQ) );
	}
	inline T Proc( T tIn )
	{
		const T tOut = - m_tA0*m_tOut0 - m_tA1*m_tOut1      // backward
			       + m_tA1*tIn + m_tA0*m_tIn0 + m_tIn1; // forward
		m_tIn1 = m_tIn0;
		m_tIn0 = tIn;
		m_tOut1 = m_tOut0;
		m_tOut0 = tOut;
		return tOut;
	}
	
	inline void Reset() {
		m_tIn0 = m_tIn1 = m_tOut0 = m_tOut1 = T(0.0);
	}
public:
	T m_tIn0, m_tIn1;
	T m_tOut0, m_tOut1;
protected:
	T m_tRadius;
	T m_tA0, m_tA1;
};


// Resonanter IIR Lowpass (12dB/oct)
// - http://www.musicdsp.org/showArchiveComment.php?ArchiveID=27
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkFltLow0
{
public:
	COpTkFltLow0();
	COpTkFltLow0( const COpTkFltLow0 &roO );
	COpTkFltLow0 & operator =( const COpTkFltLow0 &roO );
	
	void Init( T tFreq );	
	inline T Proc( T tIn )
	{
		m_tVibraSpeed += ( tIn - m_tVibraPos ) * m_tC;
		m_tVibraPos += m_tVibraSpeed;
		m_tVibraSpeed *= m_tR;
		return m_tVibraPos;
	}
	
	inline void Reset() {
		m_tVibraPos = m_tVibraSpeed = T(0.0);
	}
public:
	T m_tVibraPos;
	T m_tVibraSpeed;
protected:
	T m_tC, m_tR;
};

// LP / HP Filter
// - http://www.musicdsp.org/showArchiveComment.php?ArchiveID=38
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkFltHighLow0
{
public:
	COpTkFltHighLow0();
	COpTkFltHighLow0( const COpTkFltHighLow0 &roO );
	COpTkFltHighLow0 & operator =( const COpTkFltHighLow0 &roO );
	
	void Init( bool bHigh, T tFreq, T tRes );	
	inline T Proc( T tIn )
	{
		const double dVal =
			m_tA0 * tIn +
			m_tA1 * m_tIn0 +
			m_tA0 * m_tIn1 -
			m_tB0 * m_tOut0 -
			m_tB1 * m_tOut1;
		
		m_tIn1 = m_tIn0;
		m_tIn0 = tIn;
		
		m_tOut1 = m_tOut0;
		m_tOut0 = dVal;
		return dVal;
	}
	inline void Reset() {
		m_tOut0 = m_tOut1 = m_tIn0 = m_tIn1 = T(0.0);
	}
public:
	T m_tIn0, m_tIn1;
	T m_tOut0, m_tOut1;
protected:
	T m_tA0, m_tA1;
	T m_tB0, m_tB1;
};


// Biquad
// www.musicdsp.org, Tom St Denis
// Implementation of the RBJ cookbook, in C.
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkFltBiquad
{
public:
	enum {
		TYPE_LPF,   // Low pass filter
		TYPE_HPF,   // High pass filter
		TYPE_BPF,   // Band pass filter
		TYPE_NOTCH, // Notch Filter
		TYPE_PEQ,   // Peaking band EQ filter
		TYPE_LSH,   // Low shelf filter
		TYPE_HSH,   // High shelf filter
		TYPE_MAX    // Dummy
	};
	
public:
	COpTkFltBiquad();
	COpTkFltBiquad( const COpTkFltBiquad &roO );
	COpTkFltBiquad & operator =( const COpTkFltBiquad &roO );
	
	void Init( unsigned int uiType, T tFreq, T tGain, T tBandwidth );
	inline T Proc( T tIn )
	{
		const double dVal =
			m_tA0 * tIn +
			m_tA1 * m_tIn0 +
			m_tA2 * m_tIn1 -
			m_tB0 * m_tOut0 -
			m_tB1 * m_tOut1;
		
		m_tIn1 = m_tIn0;
		m_tIn0 = tIn;

		m_tOut1 = m_tOut0;
		m_tOut0 = dVal;
		return dVal;
	}
	inline void Reset(){
		m_tOut0 = m_tOut1
		= m_tIn0 = m_tIn1 = T(0.0);
	}
public:
	T m_tIn0, m_tIn1, m_tOut0, m_tOut1;
protected:
	T m_tA0, m_tA1, m_tA2;
	T m_tB0, m_tB1;
};


// Resonanter Filter
// - www.musicdsp.org, Paul Kellett/Peter Schoffhauzer
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkFltRes0
{
public:
	COpTkFltRes0();
	COpTkFltRes0( const COpTkFltRes0 &roO );
	COpTkFltRes0 & operator =( const COpTkFltRes0 &roO );
	
	void Init( T tFreq, T tRes )
	{
		m_tF = T(2.0) * sin( OP_TK_PI * tFreq * T( 1.0 / SAMPLE_FREQ ) );
		m_tFB = tRes + tRes / ( T(1.0) - m_tF );
	}
	inline void Proc( T tIn, T *ptOutLP, T *ptOutBP, T *ptOutHP )
	{
		const T tHp = tIn - m_tOut0;
		const T tBp = m_tOut0 - m_tOut1;
		m_tOut0 += m_tF * ( tHp + m_tFB * tBp );
		m_tOut1 += + m_tF * ( m_tOut0 - m_tOut1 );
		
		*ptOutLP = m_tOut1; // lowpass
		*ptOutBP = tBp; // bandpass
		*ptOutHP = tHp; // highpass
	}
	inline void Reset() {
		m_tOut0 = m_tOut1 = T(0.0);
	}
public:
	T m_tOut0, m_tOut1;
protected:
	T m_tF, m_tFB;
};


// Moog Filter (24db resonant lowpass)
// - www.musicdsp.org, Moog VCF, variation 2
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkFltMoog0
{
public:
	COpTkFltMoog0();
	COpTkFltMoog0( const COpTkFltMoog0 &roO );
	COpTkFltMoog0 & operator =( const COpTkFltMoog0 &roO );
	
	void Init( T tFreq, T tRes );
	inline T Proc( T tIn )
	{
		tIn -= m_tOut3 * m_tFB;
		tIn *= m_tFX;
		m_tOut0 = tIn     + 0.3 * m_tIn0 + m_tOneMinusF * m_tOut0;
		m_tIn0  = tIn;
		m_tOut1 = m_tOut0 + 0.3 * m_tIn1 + m_tOneMinusF * m_tOut1;
		m_tIn1  = m_tOut0;
		m_tOut2 = m_tOut1 + 0.3 * m_tIn2 + m_tOneMinusF * m_tOut2;
		m_tIn2  = m_tOut1;
		m_tOut3 = m_tOut2 + 0.3 * m_tIn3 + m_tOneMinusF * m_tOut3;
		m_tIn3  = m_tOut2;
		return    m_tOut3;
	}
	inline void Reset() {
		m_tOut0 = m_tOut1 = m_tOut2 = m_tOut3 
		= m_tIn0 = m_tIn1 = m_tIn2 = m_tIn3 = T(0.0);
	}
public:
	T m_tIn0, m_tIn1, m_tIn2, m_tIn3;
	T m_tOut0, m_tOut1, m_tOut2, m_tOut3;
protected:
	T m_tF, m_tFB, m_tOneMinusF, m_tFX;
};

// TB303-Filter
// - Andy Sloane (gsyn)
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkFltTB303
{
public:
	COpTkFltTB303();
	COpTkFltTB303( const COpTkFltTB303 &roO );
	COpTkFltTB303 & operator =( const COpTkFltTB303 &roO );
	
	void Init( T tFreq, T tRes );
	inline T Proc( T tIn )
	{
		if( m_uiPos >= 64 )
		{
			const T w = m_tE0 + m_tB;
			const T k = exp( -w / m_tResCoeff );
			//m_tB = m_tB * vcf_envdecay;
			m_tA0 = 2.0 * cos( 2.0 * w ) * k;
			m_tA1 = - k * k;
			m_tB = 1.0 - m_tA0 - m_tA1;
			m_uiPos = 0;
		}
		const T tOut = m_tA0 * m_tOut0 + m_tA1 * m_tOut1 + m_tB * tIn;
		m_tOut1 = m_tOut0;
		m_tOut0 = tOut;
		++m_uiPos;
		return tOut;
	}
	inline void Reset() {
		m_tOut0 = m_tOut1 = T(0.0);
		m_tA0 = m_tA1 = T(0.0);
		m_tB = m_tE1;	
		m_uiPos = 64;
	}
public:
	T m_tA0, m_tA1, m_tB, m_tOut0, m_tOut1;
protected:
	T m_tResCoeff;
	T m_tE0, m_tE1;
	unsigned int m_uiPos;
};


// 3 Band Equalizer
// - www.musicdsp.org, Neil C / Etanza Systems, 2006 :)
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SAMPLE_FREQ>
class COpTkFltEq3
{
public:
	COpTkFltEq3();
	COpTkFltEq3( const COpTkFltEq3 &roO );
	COpTkFltEq3 & operator =( const COpTkFltEq3 &roO );
	
public:
	inline void InitGain( T tLow, T tMid, T  tHigh )
	{
		m_tLg = tLow;
		m_tMg = tMid;
		m_tHg = tHigh;
	}
	void InitFreq( T tFreqLow, T tFreqHigh );
	T Proc( T tIn );
	void Reset();
	
public:
	// Sample history buffer
	T m_tSample0;     // Sample data minus 1
	T m_tSample1;     //                   2
	T m_tSample2;     //                   3
	
protected:
	// Filter #1 (Low band)
	T m_tLf;       // Frequency
	T m_tF1p0;     // Poles ...
	T m_tF1p1;     
	T m_tF1p2;
	T m_tF1p3;
	
	// Filter #2 (High band)
	T m_tHf;       // Frequency
	T m_tF2p0;     // Poles ...
	T m_tF2p1;
	T m_tF2p2;
	T m_tF2p3;
	
	// Gain Controls
	T m_tLg;       // low  gain
	T m_tMg;       // mid  gain
	T m_tHg;       // high gain
};


// AntiAlias-Filter: Max. 32xOversampling + 512-tap FIR-Filter bei 44.1 kHz.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkFltAA44100
{
public:
	COpTkFltAA44100();
	COpTkFltAA44100( const COpTkFltAA44100 &roO );
	~COpTkFltAA44100();
	COpTkFltAA44100 & operator =( const COpTkFltAA44100 &roO );
	
protected:
	double Filter();
	void ResetFilter();
	unsigned int SetFilter( unsigned int uiFilter );
	
protected:
	unsigned int m_uiOverSample;
	unsigned int m_uiFilterSize;
	
	unsigned int m_uiBufferInPos;
	T m_atBufferIn[256];
private:
	T m_atFltCoeff[256*2];
private:
	static const T m_atFltCoeff32_[32];   // 4x Oversampling
	static const T m_atFltCoeff64_[64];   // 8x Oversampling
	static const T m_atFltCoeff128_[128]; // 16x Oversampling
	static const T m_atFltCoeff256_[256]; // 32x Oversampling
};

#include "OpTkFlt.inline.h"

#endif // _OP_TK_FLT_H_
