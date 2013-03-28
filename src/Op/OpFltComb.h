// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltComb.h

#ifndef _OP_FLT_COMB_H_
#define _OP_FLT_COMB_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_COMB

#include "../OpTk/OpTkBufferDelay.h"

// Comb Filter
// ref: http://www-ccrma.stanford.edu/~jos/filters/Signal_Flow_Graph.html
////////////////////////////////////////////////////////////////////////////////
class COpFltComb : public COp
{
public:
	COpFltComb();
	virtual ~COpFltComb();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltComb )
	OP_GENERIC_METHODS_DEC( COpFltComb )
protected:
	COpTkBufferVarDelayInterpol<double> m_oDelayFor, m_oDelayBack;
	double m_dDelayFor, m_dDelayBack;
};

#endif // OP_INC_OP_FLT_COMB
#endif // _OP_FLT_COMB_H_
