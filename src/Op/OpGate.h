// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpGate.h

#ifndef _OP_GATE_H_
#define _OP_GATE_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_GATE

#include "../OpTk/OpTkEnv.h"
#include "../OpTk/OpTkBufferDelay.h"

// Gate
////////////////////////////////////////////////////////////////////////////////
class COpGate : public COp
{
public:
	COpGate();
	virtual ~COpGate();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();

	OP_GENERIC_COPY_CTOR_DEC( COpGate )
	OP_GENERIC_METHODS_DEC( COpGate )
protected:
	COpTkEnv1Exp<double> m_oEnvAtt, m_oEnvRel;
	COpTkBufferVarDelay<double> m_oDelay;
	double m_dSqSum;
	double m_dDelay;
	double m_dEnv;
	unsigned int m_uiEnvPos;
};

#endif // OP_INC_OP_GATE
#endif // _OP_GATE_H_
