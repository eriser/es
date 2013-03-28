// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqPrime.h

#ifndef _OP_SEQ_PRIME_H_
#define _OP_SEQ_PRIME_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SEQ_PRIME

#include "../OpTk/OpTkSeq.h"

// Prime Number Sequence
////////////////////////////////////////////////////////////////////////////////
class COpSeqPrime : public COp
{
public:
	COpSeqPrime();
	virtual ~COpSeqPrime();
protected:
	void Init();
	void UpdateSize( double dSize );
	void UpdateMin( double dMin );
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpSeqPrime )
	OP_GENERIC_METHODS_DEC( COpSeqPrime )
	
protected:
	COpTkSeqPrime<unsigned int> m_oSeq;
	double m_dSize, m_dMin;
};

#endif // OP_INC_OP_SEQ_PRIME
#endif // _OP_SEQ_PRIME_H_
