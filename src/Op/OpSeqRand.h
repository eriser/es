// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqRand.h

#ifndef _OP_SEQ_RAND_H_
#define _OP_SEQ_RAND_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SEQ_RAND

#include "../OpTk/OpTkSeq.h"

// Random Number Sequence
////////////////////////////////////////////////////////////////////////////////
class COpSeqRand : public COp
{
public:
	COpSeqRand();
	virtual ~COpSeqRand();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpSeqRand )
	OP_GENERIC_METHODS_DEC( COpSeqRand )
	
protected:
	COpTkSeqRandomEx<double> m_oSeqRand;
};

#endif // OP_INC_OP_SEQ_RAND
#endif // _OP_SEQ_RAND_H_
