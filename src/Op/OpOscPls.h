// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscPls.h

#ifndef _OP_OSC_PLS_H_
#define _OP_OSC_PLS_H_

#include "OpDEF.h"

#ifdef OP_INC_OP_OSC_PLS

#include "Op.h"
#include "../OpTk/OpTkFlt.h"

#if( OP_SAMPLE_FREQ != 44100 )
#if defined( OP_LANGUAGE_GERMAN )
#error Die Anti-Aliasing-Filter sind festgesetzt auf eine Sampling-Frequenz von 44100 Hz!
#else //if defined( OP_LANGUAGE_ENGLISH )
#error Anti-aliasing filters are fixed to a sampling frequency of 44100 Hz!
#endif // OP_LANGUAGE_*
#endif

// Pulse Oscillator
////////////////////////////////////////////////////////////////////////////////
class COpOscPls : public COp, protected COpTkFltAA44100<double>
{
public:
	COpOscPls();
	COpOscPls( const COpOscPls &roOp );
	virtual ~COpOscPls();
public:
	COpOscPls & operator = ( const COpOscPls &roOp );
protected:
	void Init();
	void InitFreq( double dFreq );
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	//OP_GENERIC_COPY_CTOR_DEC( COpOscPls )
	OP_GENERIC_METHODS_DEC( COpOscPls )

protected:
	double m_dFTStp, m_dFTStpAA;
	double m_dFreqLast;
	double m_dFT;
	double m_dSample;
	unsigned int m_uiSample, m_uiSampleMax;
	unsigned int m_uiFilterIndex;
};

#endif // OP_INC_OP_OSC_PLS
#endif // _OP_OSC_PLS_H_
