// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv2Exp.h

#ifndef _OP_ENV_2_EXP_H_
#define _OP_ENV_2_EXP_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ENV_2_EXP

#include "../OpTk/OpTkEnv.h"

// Exponential 2-Stage Envelope
////////////////////////////////////////////////////////////////////////////////
class COpEnv2Exp : public COp
{
public:
	COpEnv2Exp();
	virtual ~COpEnv2Exp();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpEnv2Exp )
	OP_GENERIC_METHODS_DEC( COpEnv2Exp )
protected:
	COpTkEnv2Exp<double> m_oEnv;
	double m_dDur, m_dRatio, m_dSlope0, m_dSlope1;
};

#endif // OP_INC_OP_ENV_2_EXP
#endif // _OP_ENV_2_EXP_H_
