// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnvAdsr.h

#ifndef _OP_ENV_ADSR_H_
#define _OP_ENV_ADSR_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ENV_ADSR

#include "../OpTk/OpTkEnv.h"

// ADSR Envelope
////////////////////////////////////////////////////////////////////////////////
class COpEnvAdsr : public COp
{
public:
	COpEnvAdsr();
	virtual ~COpEnvAdsr();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpEnvAdsr )
	OP_GENERIC_METHODS_DEC( COpEnvAdsr )
protected:
	COpTkEnv1Exp<double> m_oEnvA;
	COpTkEnv1Exp<double> m_oEnvD;
	COpTkEnv1Exp<double> m_oEnvR;
	double m_dA, m_dD, m_dS, m_dR;
	double m_dSlopeA, m_dSlopeD, m_dSlopeR;
	double m_dL0, m_dL1;
	int m_iPos;
};

#endif // OP_INC_OP_ENV_ADSR
#endif // _OP_ENV_ADSR_H_
