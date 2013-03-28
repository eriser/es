// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathPow.h

#ifndef _OP_MATH_POW_H_
#define _OP_MATH_POW_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MATH_POW

// Power Function
////////////////////////////////////////////////////////////////////////////////
class COpMathPow : public COp
{
public:
	COpMathPow();
	virtual ~COpMathPow();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpMathPow )
	OP_GENERIC_METHODS_DEC( COpMathPow )
};

#endif // OP_INC_OP_MATH_POW
#endif // _OP_MATH_POW_H_
