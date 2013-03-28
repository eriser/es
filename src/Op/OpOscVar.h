// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscVar.h

#ifndef _OP_OSC_VAR_H_
#define _OP_OSC_VAR_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_OSC_VAR

#include "../OpTk/OpTkOsc.h"

// Variable Oscillator
////////////////////////////////////////////////////////////////////////////////
class COpOscVar : public COp
{
public:
	COpOscVar();
	virtual ~COpOscVar();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES

	OP_GENERIC_COPY_CTOR_DEC( COpOscVar )
	OP_GENERIC_METHODS_DEC( COpOscVar )
	
protected:
	COpTkOscSinVar<double, OP_SAMPLE_FREQ> m_oOsc;
	double m_dFreq;
};

#endif // OP_INC_OP_OSC_VAR
#endif // _OP_OSC_VAR_H_
