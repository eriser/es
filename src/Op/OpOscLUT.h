// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscLUT.h

#ifndef _OP_OSC_LUT_H_
#define _OP_OSC_LUT_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_OSC_LUT

#include "../OpTk/OpTkOsc.h"

// Lookup Table Oscillator
////////////////////////////////////////////////////////////////////////////////
class COpOscLUT : public COp
{
public:
	COpOscLUT();
	virtual ~COpOscLUT();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES

	OP_GENERIC_COPY_CTOR_DEC( COpOscLUT )
	OP_GENERIC_METHODS_DEC( COpOscLUT )
	
protected:
	COpTkOscLUT<double, OP_SAMPLE_FREQ> m_oOsc;
	double m_dFreq;
};

#endif // OP_INC_OP_OSC_LUT
#endif // _OP_OSC_LUT_H_
