// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv2Sin.h

#ifndef _OP_ENV_2_SIN_H_
#define _OP_ENV_2_SIN_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ENV_2_SIN

#include "../OpTk/OpTkEnv.h"

// 2-Stage Sine Envelope
////////////////////////////////////////////////////////////////////////////////
class COpEnv2Sin : public COp
{
public:
	COpEnv2Sin();
	virtual ~COpEnv2Sin();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpEnv2Sin )
	OP_GENERIC_METHODS_DEC( COpEnv2Sin )
protected:
	COpTkEnv2SinPow<double> m_oEnv;
	double m_dDur, m_dRatio;
	double m_dSlope0, m_dSlope1;
};

#endif // OP_INC_OP_ENV_2_SIN
#endif // _OP_ENV_2_SIN_H_
