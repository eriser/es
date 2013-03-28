// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltHighLow.h

#ifndef _OP_FLT_HIGH_LOW_H_
#define _OP_FLT_HIGH_LOW_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_HIGH_LOW

#include "../OpTk/OpTkFlt.h"

// High-/Low-Pass Filter
////////////////////////////////////////////////////////////////////////////////
class COpFltHighLow : public COp
{
public:
	COpFltHighLow();
	virtual ~COpFltHighLow();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltHighLow )
	OP_GENERIC_METHODS_DEC( COpFltHighLow )
protected:
	COpTkFltHighLow0<double, OP_SAMPLE_FREQ>  m_oFlt;
	double m_dCutoff, m_dRes;
	unsigned int m_uiFuncNum;
};

#endif // OP_INC_OP_FLT_HIGH_LOW
#endif // _OP_FLT_HIGH_LOW_H_
