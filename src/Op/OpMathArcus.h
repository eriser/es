// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscMathArcus.h

#ifndef _OP_MATH_ARCUS_H_
#define _OP_MATH_ARCUS_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MATH_ARCUS

// Arcus Functions
////////////////////////////////////////////////////////////////////////////////
class COpMathArcus : public COp
{
public:
	COpMathArcus();
	virtual ~COpMathArcus();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpMathArcus )
	OP_GENERIC_METHODS_DEC( COpMathArcus )
protected:
	unsigned m_uiFuncNum;
	TOpFuncFlt1 m_tFunc;
protected:
	static TOpFuncFlt1 m_atFunc_[];
};

#endif // OP_INC_OP_MATH_ARCUS
#endif // _OP_MATH_ARCUS_H_
