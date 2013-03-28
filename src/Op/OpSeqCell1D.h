// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqCell1D.h

#ifndef _OP_SEQ_CELL_1D_H_
#define _OP_SEQ_CELL_1D_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SEQ_CELL_1D

#include "../OpTk/OpTkSeq.h"

// 1D Cellular Automata Sequence
////////////////////////////////////////////////////////////////////////////////
class COpSeqCell1D : public COp
{
public:
	COpSeqCell1D();
	virtual ~COpSeqCell1D();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpSeqCell1D )
	OP_GENERIC_METHODS_DEC( COpSeqCell1D )
	
protected:
	COpTkSeqCell1D<unsigned int> m_oSeq;
	double m_dCount, m_dStates, m_dNeigh;
};

#endif // OP_INC_OP_SEQ_CELL_1D
#endif // OP_INC_OP_SEQ_CELL_1D
