// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqAAS.h

#ifndef _OP_SEQ_AAS_H_
#define _OP_SEQ_AAS_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SEQ_AAS

#include "../OpTk/OpTkSeq.h"

// Audioactive Sequence
////////////////////////////////////////////////////////////////////////////////
class COpSeqAAS : public COp
{
public:
	COpSeqAAS();
	virtual ~COpSeqAAS();
protected:
	void Init();
	void UpdateSize( double dSize, double dStart );
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpSeqAAS )
	OP_GENERIC_METHODS_DEC( COpSeqAAS )
	
protected:
	COpTkSeqAAS<unsigned int> m_oSeq;
	double m_dSize;
	double m_dStart;
};

#endif // OP_INC_OP_SEQ_AAS
#endif // _OP_SEQ_AAS_H_
