// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnv2Bzr.h

#ifndef _OP_ENV_2_BZR_H_
#define _OP_ENV_2_BZR_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ENV_2_BZR

#include "../OpTk/OpTkEnv.h"

// 2-Stage B�zier Envelope
////////////////////////////////////////////////////////////////////////////////
class COpEnv2Bzr : public COp
{
public:
	COpEnv2Bzr();
	virtual ~COpEnv2Bzr();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpEnv2Bzr )
	OP_GENERIC_METHODS_DEC( COpEnv2Bzr )
protected:
	COpTkEnv2Bzr<double> m_oEnv;
	double m_dDur, m_dRatio;
	double m_dSlope0, m_dSlope1;
};

#endif // OP_INC_OP_ENV_2_BZR
#endif // _OP_ENV_2_BZR_H_
