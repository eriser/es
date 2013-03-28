// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltAllpass2.h

#ifndef _OP_FLT_ALLPASS_2_H_
#define _OP_FLT_ALLPASS_2_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_ALLPASS_2

#include "../OpTk/OpTkFlt.h"

// Second-Order Allpass Filter
////////////////////////////////////////////////////////////////////////////////
class COpFltAllpass2 : public COp
{
public:
	COpFltAllpass2();
	virtual ~COpFltAllpass2();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltAllpass2 )
	OP_GENERIC_METHODS_DEC( COpFltAllpass2 )
protected:
	COpTkFltAllpass2<double, OP_SAMPLE_FREQ> m_oFlt;
	double m_dRadius, m_dFreq;
};

#endif // OP_INC_OP_FLT_ALLPASS_2
#endif // _OP_FLT_ALLPASS_2_H_
