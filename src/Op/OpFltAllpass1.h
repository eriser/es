// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltAllpass1.h

#ifndef _OP_FLT_ALLPASS_1_H_
#define _OP_FLT_ALLPASS_1_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_ALLPASS_1

#include "../OpTk/OpTkFlt.h"

// First-Order Allpass Filter
////////////////////////////////////////////////////////////////////////////////
class COpFltAllpass1 : public COp
{
public:
	COpFltAllpass1();
	virtual ~COpFltAllpass1();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltAllpass1 )
	OP_GENERIC_METHODS_DEC( COpFltAllpass1 )
protected:
	COpTkFltAllpass1<double> m_oFlt;
	double m_dDelay;
};

#endif // OP_INC_OP_FLT_ALLPASS_1
#endif // _OP_FLT_ALLPASS_1_H_
