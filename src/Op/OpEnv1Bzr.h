// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv1Bzr.h

#ifndef _OP_ENV_1_BZR_H_
#define _OP_ENV_1_BZR_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ENV_1_BZR

#include "../OpTk/OpTkEnv.h"

// 1-Stage Bézier Envelope
////////////////////////////////////////////////////////////////////////////////
class COpEnv1Bzr : public COp
{
public:
	COpEnv1Bzr();
	virtual ~COpEnv1Bzr();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpEnv1Bzr )
	OP_GENERIC_METHODS_DEC( COpEnv1Bzr )
protected:
	COpTkEnv1Bezier2<double> m_oEnv;
	double m_dDur, m_dV0, m_dV1, m_tSlope;
};

#endif // OP_INC_OP_ENV_1_BZR
#endif // _OP_ENV_1_BZR_H_
