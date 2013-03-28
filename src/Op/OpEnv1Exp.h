// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv1Exp.h

#ifndef _OP_ENV_1_EXP_H_
#define _OP_ENV_1_EXP_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ENV_1_EXP

#include "../OpTk/OpTkEnv.h"

// Exponential 1-Stage Envelope
////////////////////////////////////////////////////////////////////////////////
class COpEnv1Exp : public COp
{
public:
	COpEnv1Exp();
	virtual ~COpEnv1Exp();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpEnv1Exp )
	OP_GENERIC_METHODS_DEC( COpEnv1Exp )
protected:
	COpTkEnv1Exp<double> m_oEnv;
	double m_dDur, m_dV0, m_dV1, m_dSlope;
};

#endif // OP_INC_OP_ENV_1_EXP
#endif // _OP_ENV_1_EXP_H_
