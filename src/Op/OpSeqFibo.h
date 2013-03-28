// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqFibo.h

#ifndef _OP_SEQ_FIBO_H_
#define _OP_SEQ_FIBO_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SEQ_FIBO

#include "../OpTk/OpTkSeq.h"

// Fibonacci Sequence
////////////////////////////////////////////////////////////////////////////////
class COpSeqFibo : public COp
{
public:
	COpSeqFibo();
	virtual ~COpSeqFibo();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpSeqFibo )
	OP_GENERIC_METHODS_DEC( COpSeqFibo )
	
protected:
	COpTkSeqFibonacci<int> m_oSeq;
};

#endif // OP_INC_OP_SEQ_FIBO
#endif // _OP_SEQ_FIBO_H_
