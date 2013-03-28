// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv2Lin.h

#ifndef _OP_ENV_2_LIN_H_
#define _OP_ENV_2_LIN_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ENV_2_LIN

#include "../OpTk/OpTkEnv.h"

// Linear 2-Stage Envelope
////////////////////////////////////////////////////////////////////////////////
class COpEnv2Lin : public COp
{
public:
	COpEnv2Lin();
	virtual ~COpEnv2Lin();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpEnv2Lin )
	OP_GENERIC_METHODS_DEC( COpEnv2Lin )
protected:
	COpTkEnv2Lin<double> m_oEnv;
	double m_dDur, m_dRatio;
};

#endif // OP_INC_OP_ENV_2_LIN
#endif // _OP_ENV_2_LIN_H_
