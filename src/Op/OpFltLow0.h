// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltLow0.h

#ifndef _OP_FLT_LOW_0_H_
#define _OP_FLT_LOW_0_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_LOW_0

#include "../OpTk/OpTkFlt.h"

// Resonant Low-Pass Filter
////////////////////////////////////////////////////////////////////////////////
class COpFltLow0 : public COp
{
public:
	COpFltLow0();
	virtual ~COpFltLow0();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltLow0 )
	OP_GENERIC_METHODS_DEC( COpFltLow0 )
protected:
	COpTkFltLow0<double, OP_SAMPLE_FREQ> m_oFlt;
	double m_dCutoff;
};

#endif // OP_INC_OP_FLT_LOW_0
#endif // _OP_FLT_LOW_0_H_
