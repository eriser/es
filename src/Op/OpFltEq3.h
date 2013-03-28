// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltEq3.h

#ifndef _OP_FLT_EQ_3_H_
#define _OP_FLT_EQ_3_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_EQ_3

#include "../OpTk/OpTkFlt.h"

// 3-Band Equalizer
////////////////////////////////////////////////////////////////////////////////
class COpFltEq3 : public COp
{
public:
	COpFltEq3();
	virtual ~COpFltEq3();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltEq3 )
	OP_GENERIC_METHODS_DEC( COpFltEq3 )
protected:
	COpTkFltEq3<double, OP_SAMPLE_FREQ> m_oFlt;
	double m_dF0, m_dF1, m_dF2;
};

#endif // OP_INC_OP_FLT_EQ_3
#endif // _OP_FLT_EQ_3_H_
