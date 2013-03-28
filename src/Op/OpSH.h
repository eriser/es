// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSH.h

#ifndef _OP_SH_H_
#define _OP_SH_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SH

// Sample and Hold
////////////////////////////////////////////////////////////////////////////////
class COpSH : public COp
{
public:
	COpSH();
	virtual ~COpSH();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpSH )
	OP_GENERIC_METHODS_DEC( COpSH )
	
protected:
	int m_iCounter;
	int m_iPeriod;
	double m_dCounterFrc;
	double m_dPeriodFrc;
	double m_dPeriodLast;
	double m_dSample;
};

#endif // OP_INC_OP_SH
#endif // _OP_SH_H_
