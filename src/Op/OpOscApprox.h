// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscApprox.h

#ifndef _OP_OSC_APPROX_H_
#define _OP_OSC_APPROX_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_OSC_APPROX

#include "../OpTk/OpTkOsc.h"

// Approximative Oscillator
////////////////////////////////////////////////////////////////////////////////
class COpOscApprox : public COp
{
public:
	COpOscApprox();
	virtual ~COpOscApprox();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpOscApprox )
	OP_GENERIC_METHODS_DEC( COpOscApprox )

protected:
	COpTkOscApprox<double, OP_SAMPLE_FREQ> m_oOsc;
	double m_dHarmon;
};

#endif // OP_INC_OP_OSC_APPROX
#endif // _OP_OSC_APPROX_H_
