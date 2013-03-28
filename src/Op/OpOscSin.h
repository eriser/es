// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscSin.h

#ifndef _OP_OSC_SIN_H_
#define _OP_OSC_SIN_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_OSC_SIN

#include "../OpTk/OpTkOsc.h"

// Sine Oscillator
////////////////////////////////////////////////////////////////////////////////
class COpOscSin : public COp
{
public:
	COpOscSin();
	virtual ~COpOscSin();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpOscSin )
	OP_GENERIC_METHODS_DEC( COpOscSin )
	
protected:
	COpTkOscSin<double, OP_SAMPLE_FREQ> m_oOsc;
	double m_dFreq;
};

#endif // OP_INC_OP_OSC_SIN
#endif // _OP_OSC_SIN_H_
