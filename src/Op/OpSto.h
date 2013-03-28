// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSto.h

#ifndef _OP_STO_H_
#define _OP_STO_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_STO

// Conditional Storage
////////////////////////////////////////////////////////////////////////////////
class COpSto : public COp
{
public:
	COpSto();
	virtual ~COpSto();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpSto )
	OP_GENERIC_METHODS_DEC( COpSto )

protected:
	double m_dVal, m_dValPrev;
};

#endif // OP_INC_OP_STO
#endif // _OP_STO_H_
