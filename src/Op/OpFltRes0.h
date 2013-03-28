// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltRes0.h

#ifndef _OP_FLT_RES_0_H_
#define _OP_FLT_RES_0_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_RES_0

#include "../OpTk/OpTkFlt.h"

// Resonant Filter
////////////////////////////////////////////////////////////////////////////////
class COpFltRes0 : public COp
{
public:
	COpFltRes0();
	virtual ~COpFltRes0();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltRes0 )
	OP_GENERIC_METHODS_DEC( COpFltRes0 )
protected:
	COpTkFltRes0<double, OP_SAMPLE_FREQ>  m_oFlt;
	double m_dCutoff, m_dRes;
};

#endif // OP_INC_OP_FLT_RES_0
#endif // _OP_FLT_RES0_H_
