// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv1Sin.h

#ifndef _OP_ENV_1_SIN_H_
#define _OP_ENV_1_SIN_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ENV_1_SIN

#include "../OpTk/OpTkEnv.h"

// 1-Stage Sine Envelope
////////////////////////////////////////////////////////////////////////////////
class COpEnv1Sin : public COp
{
public:
	COpEnv1Sin();
	virtual ~COpEnv1Sin();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpEnv1Sin )
	OP_GENERIC_METHODS_DEC( COpEnv1Sin )
protected:
	COpTkEnv1SinPow<double> m_oEnv;
	double m_dDur, m_dV0, m_dV1, m_dSlope;
};

#endif // OP_INC_OP_ENV_1_SIN
#endif // _OP_ENV_1_SIN_H_
