// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltAllpass.h

#ifndef _OP_FLT_ALLPASS_H_
#define _OP_FLT_ALLPASS_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_ALLPASS

#include "../OpTk/OpTkBufferDelay.h"

// Allpass Filter
// ref: http://www.harmony-central.com/Effects/Articles/Reverb/allpass.html
////////////////////////////////////////////////////////////////////////////////
class COpFltAllpass : public COp
{
public:
	COpFltAllpass();
	virtual ~COpFltAllpass();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltAllpass )
	OP_GENERIC_METHODS_DEC( COpFltAllpass )
protected:
	double m_dDelay;
	COpTkBufferVarDelay<double> m_oDelay;
};

#endif // OP_INC_OP_FLT_ALLPASS
#endif // _OP_FLT_ALLPASS_H_
