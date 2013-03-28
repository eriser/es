// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkEnv.h

#ifndef _OP_TK_ENV_H_
#define _OP_TK_ENV_H_

#include "OpTkDEF.h"

// rem:
// - Dauerwerte werden zwar als 'T' übergeben, werden aber intern als 
//   'unsigned int' behandelt und entsprechend gerundet.


// Hüllkurve.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkEnv
{
public:
	COpTkEnv();
	COpTkEnv( const COpTkEnv &roO );
	virtual ~COpTkEnv();
	COpTkEnv & operator =( const COpTkEnv &roO );
public:
	unsigned int & Index();
	unsigned int & State();
	T & Value();
	inline bool IsOn() const {
		return ( m_uiState != 0 );
	}
	inline void Trigger() {
		m_uiState = 1;
	}
public:
	virtual void Reset() {
		m_tV = m_tV0;
		m_uiIndex = 0;
		m_uiState = 0;
	}
	virtual T Proc() = 0;
protected:
	T m_tV, m_tV0;
	unsigned int m_uiIndex;
	unsigned int m_uiState;
};

// - In Set() werden negeative Dauerwerte auf 0.0 gesetzt.
// - Nach einem Reset() steht die Kurve konstant auf m_tV0, bis Trigger().
// - Ist das Ende erreicht, steht die Hüllkurve konstant auf m_tV1.

// Lineare 1-Stufen Hüllkurve.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkEnv1Lin : public COpTkEnv<T>
{
public:
	COpTkEnv1Lin();
	COpTkEnv1Lin( const COpTkEnv1Lin &roO );
	virtual ~COpTkEnv1Lin();
	COpTkEnv1Lin & operator =( const COpTkEnv1Lin &roO );
public:
	void Set( T tDur, T tV0, T tV1 );
	virtual T Proc();
protected:
	unsigned int m_uiDur;
	T m_tStp;
	T m_tV1;
};

// Exponentielle 1-Stufen Hüllkurve.
// - Für positives tSlope ist die Kurve immer "ansteigend" gekekrümmt.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkEnv1Exp : public COpTkEnv<T>
{
public:
	COpTkEnv1Exp();
	COpTkEnv1Exp( const COpTkEnv1Exp &roO );
	virtual ~COpTkEnv1Exp();
	COpTkEnv1Exp & operator =( const COpTkEnv1Exp &roO );
public:
	T & Position();
	void Set( T tDur, T tV0, T tV1, T tSlope );
	virtual void Reset() {
		m_tP = T(1.0);
		COpTkEnv<T>::Reset();
	}
	virtual T Proc();
protected:
	unsigned int m_uiDur;
	T m_tP;
	T m_tStp;
	T m_tV1;
	T m_tFac;
};

// 1-Stufen Sinus-Hüllkurve.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkEnv1Sin : public COpTkEnv<T>
{
public:
	COpTkEnv1Sin();
	COpTkEnv1Sin( const COpTkEnv1Sin &roO );
	virtual ~COpTkEnv1Sin();
	COpTkEnv1Sin & operator =( const COpTkEnv1Sin &roO );
public:
	T & Position();
	void Set( T tDur, T tV0, T tV1 );
	virtual inline void Reset() {
		m_tP = T(-OP_TK_PI_2);
		COpTkEnv<T>::Reset();
	}
	virtual T Proc();
protected:
	unsigned int m_uiDur;
	T m_tP;
	T m_tStp;
	T m_tV1;
	T m_tFac, m_tExp;
};


// 1-Stufen Sinus-Pow-Hüllkurve.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkEnv1SinPow : public COpTkEnv<T>
{
public:
	COpTkEnv1SinPow();
	COpTkEnv1SinPow( const COpTkEnv1SinPow &roO );
	virtual ~COpTkEnv1SinPow();
	COpTkEnv1SinPow & operator =( const COpTkEnv1SinPow &roO );
public:
	T & Position();
	T & Exponent();
	void Set( T tDur, T tV0, T tV1, T tSlope );
	virtual void Reset() {
		m_tP = T(0.0);
		COpTkEnv<T>::Reset();
	}
	virtual T Proc();
protected:
	unsigned int m_uiDur;
	T m_tP;
	T m_tStp;
	T m_tV1;
	T m_tFac, m_tExp;
};


// 1-Stufen Bézier-Hüllkurve.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkEnv1Bezier2 : public COpTkEnv<T>
{
public:
	COpTkEnv1Bezier2();
	COpTkEnv1Bezier2( const COpTkEnv1Bezier2 &roO );
	virtual ~COpTkEnv1Bezier2();
	COpTkEnv1Bezier2 & operator =( const COpTkEnv1Bezier2 &roO );
public:
	T & Position();
	void Set( T tDur, T tV0, T tV1, T tSlope );
	virtual void Reset() {
		m_tP = T(0.0);
		COpTkEnv<T>::Reset();
	}
	virtual T Proc();
protected:
	unsigned int m_uiDur;
	T m_tP;
	T m_tStp;
	T m_tV1;
	T m_tSlope;
};


// - In Set() werden negeative Dauerwerte auf 0.0 gesetzt, und wenn
//   tRatio grösser 1.0 ist, auf 1.0 gesetzt.
// - Die Hüllkurve bewegt sich im Interval [0.0,1.0].

// Lineare 2-Stufen Hüllkurve.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkEnv2Lin : public COpTkEnv<T>
{
public:
	COpTkEnv2Lin();
	COpTkEnv2Lin( const COpTkEnv2Lin &roO );
	virtual ~COpTkEnv2Lin();
	COpTkEnv2Lin & operator =( const COpTkEnv2Lin &roO );
public:
	void Set( T tDur, T tRatio );
	virtual T Proc();
protected:
	unsigned int m_uiDur0, m_uiDur1;
	T m_tStp0, m_tStp1;
};


// Exponentielle 2-Stufen Hüllkurve.
// - Gewisse "-1.0/+1.0 Fälle" sind noch nicht vollständig abgeklärt.
//   Die Kurve zeigt oft verdoppelt Grenzwerte...
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkEnv2Exp : public COpTkEnv<T>
{
public:
	COpTkEnv2Exp();
	COpTkEnv2Exp( const COpTkEnv2Exp &roO );
	virtual ~COpTkEnv2Exp();
	COpTkEnv2Exp & operator =( const COpTkEnv2Exp &roO );
public:
	T & Position();
	void Set( T tDur, T tRatio, T tSlope0, T tSlope1 );
	virtual void Reset() {
		m_tP = T(1.0);
		COpTkEnv<T>::Reset();
	}
	virtual T Proc();
protected:
	unsigned int m_uiDur0, m_uiDur1;
	T m_tP;
	T m_tStp0, m_tStp1;
	T m_tFac0, m_tFac1;
	T m_tExp0, m_tExp1;
};

// 2-Stufen Sinus-Pow-Hüllkurve.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkEnv2SinPow : public COpTkEnv<T>
{
public:
	COpTkEnv2SinPow();
	COpTkEnv2SinPow( const COpTkEnv2SinPow &roO );
	virtual ~COpTkEnv2SinPow();
	COpTkEnv2SinPow & operator =( const COpTkEnv2SinPow &roO );
public:
	T & Position();
	T & Exponent0();
	T & Exponent1();
	void Set( T tDur, T tRatio, T tSlope0, T tSlope1 );
	virtual void Reset() {
		m_tP = T(0.0);
		COpTkEnv<T>::Reset();
	}
	virtual T Proc();
protected:
	unsigned int m_uiDur0, m_uiDur1;
	T m_tP;
	T m_tStp0, m_tStp1;
	T m_tExp0, m_tExp1;
};

// 2-Stufen Bézier-Hüllkurve.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkEnv2Bzr : public COpTkEnv<T>
{
public:
	COpTkEnv2Bzr();
	COpTkEnv2Bzr( const COpTkEnv2Bzr &roO );
	virtual ~COpTkEnv2Bzr();
	COpTkEnv2Bzr & operator =( const COpTkEnv2Bzr &roO );
public:
	T & Position();
	void Set( T tDur, T tRatio, T tSlope0, T tSlope1 );
	virtual void Reset() {
		m_tP = T(0.0);
		COpTkEnv<T>::Reset();
	}
	virtual T Proc();
protected:
	unsigned int m_uiDur0, m_uiDur1;
	T m_tP;
	T m_tStp0, m_tStp1;
	T m_tSlope0, m_tSlope1;
};



#include "OpTkEnv.inline.h"

#endif // _OP_TK_ENV_H_
