// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltMoog.h

#ifndef _OP_FLT_MOOG_H_
#define _OP_FLT_MOOG_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_MOOG

#include "../OpTk/OpTkFlt.h"

// Moog Filter
////////////////////////////////////////////////////////////////////////////////
class COpFltMoog : public COp
{
public:
	COpFltMoog();
	virtual ~COpFltMoog();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltMoog )
	OP_GENERIC_METHODS_DEC( COpFltMoog )
protected:
	COpTkFltMoog0<double, OP_SAMPLE_FREQ>  m_oFlt;
	double m_dCutoff, m_dRes;
};

#endif // OP_INC_OP_FLT_MOOG
#endif // _OP_FLT_MOOG_H_
