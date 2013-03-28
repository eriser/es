// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqLog.h

#ifndef _OP_SEQ_LOG_H_
#define _OP_SEQ_LOG_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SEQ_LOG

#include "../OpTk/OpTkSeq.h"

// Logistic Sequence
////////////////////////////////////////////////////////////////////////////////
class COpSeqLog : public COp
{
public:
	COpSeqLog();
	virtual ~COpSeqLog();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpSeqLog )
	OP_GENERIC_METHODS_DEC( COpSeqLog )
	
protected:
	COpTkSeqLogistic<double> m_oSeq;
};

#endif // OP_INC_OP_SEQ_LOG
#endif // _OP_SEQ_LOG_H_
