// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv1Lin.h

#ifndef _OP_ENV_1_LIN_H_
#define _OP_ENV_1_LIN_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ENV_1_LIN

#include "../OpTk/OpTkEnv.h"

// Linear 1-Stage Envelope
////////////////////////////////////////////////////////////////////////////////
class COpEnv1Lin : public COp
{
public:
	COpEnv1Lin();
	virtual ~COpEnv1Lin();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpEnv1Lin )
	OP_GENERIC_METHODS_DEC( COpEnv1Lin )
protected:
	COpTkEnv1Lin<double> m_oEnv;
	double m_dDur, m_dV0, m_dV1;
};

#endif // OP_INC_OP_ENV_1_LIN
#endif // _OP_ENV_1_LIN_H_
