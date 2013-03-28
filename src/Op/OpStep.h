// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpStep.h

#ifndef _OP_STEP_H_
#define _OP_STEP_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_STEP

// Step
////////////////////////////////////////////////////////////////////////////////
class COpStep : public COp
{
public:
	COpStep();
	virtual ~COpStep();
protected:
	void Init();
	void GetV0V1( int *piV0, int *piV1 );
	double UpdateDelay();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpStep )
	OP_GENERIC_METHODS_DEC( COpStep )
	
protected:
	unsigned int m_uiAccum;
	unsigned int m_uiDly;
	int m_iStp, m_iInc;
	double m_dAccumFrc;
	double m_dDlyFrc;
	double m_dDlyLast;
	bool m_bUpdateDelay;
};

#endif // OP_INC_OP_STEP
#endif // _OP_STEP_H_
