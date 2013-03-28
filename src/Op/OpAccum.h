// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpAccum.h

#ifndef _OP_ACCUM_H_
#define _OP_ACCUM_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ACCUM

// Accumulator
////////////////////////////////////////////////////////////////////////////////
class COpAccum : public COp
{
public:
	COpAccum();
	virtual ~COpAccum();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	OP_GENERIC_COPY_CTOR_DEC( COpAccum )
	OP_GENERIC_METHODS_DEC( COpAccum )
protected:
	double m_dAccum;
	unsigned int m_uiFuncNum;
};

#endif // OP_INC_OP_ACCUM
#endif // _OP_ACCUM_H_
