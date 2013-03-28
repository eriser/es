// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSeqMandel.h

#ifndef _OP_SEQ_MANDEL_H_
#define _OP_SEQ_MANDEL_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SEQ_MANDEL

#include "../OpTk/OpTkSeq.h"

// Mandelbrot Sequence
////////////////////////////////////////////////////////////////////////////////
class COpSeqMandel : public COp
{
public:
	COpSeqMandel();
	virtual ~COpSeqMandel();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpSeqMandel )
	OP_GENERIC_METHODS_DEC( COpSeqMandel )
	
protected:
	COpTkSeqMandelbrot<double> m_oSeq;
};

#endif // OP_INC_OP_SEQ_MANDEL
#endif // _OP_SEQ_MANDEL_H_
