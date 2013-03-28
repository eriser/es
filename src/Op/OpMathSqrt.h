// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathSqrt.h

#ifndef _OP_MATH_SQRT_H_
#define _OP_MATH_SQRT_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MATH_SQRT

// Square Root Function
////////////////////////////////////////////////////////////////////////////////
class COpMathSqrt : public COp
{
public:
	COpMathSqrt();
	virtual ~COpMathSqrt();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpMathSqrt )
	OP_GENERIC_METHODS_DEC( COpMathSqrt )
};

#endif // OP_INC_OP_MATH_SQRT
#endif // _OP_MATH_SQRT_H_
