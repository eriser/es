// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqReca.h

#ifndef _OP_SEQ_RECA_H_
#define _OP_SEQ_RECA_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SEQ_RECA

#include "../OpTk/OpTkSeq.h"

// Recamán Sequence
////////////////////////////////////////////////////////////////////////////////
class COpSeqReca : public COp
{
public:
	COpSeqReca();
	virtual ~COpSeqReca();
protected:
	void Init();
	void UpdateSize( double dSize );
	void UpdateInit();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpSeqReca )
	OP_GENERIC_METHODS_DEC( COpSeqReca )
	
protected:
	COpTkSeqRecaman<int> m_oSeq;
	double m_dSize;
};

#endif // OP_INC_OP_SEQ_RECA
#endif // _OP_SEQ_RECA_H_
