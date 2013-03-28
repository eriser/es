// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathExp.h

#ifndef _OP_MATH_EXP_H_
#define _OP_MATH_EXP_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MATH_EXP

// Exponential Function
////////////////////////////////////////////////////////////////////////////////
class COpMathExp : public COp
{
public:
	COpMathExp();
	virtual ~COpMathExp();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpMathExp )
	OP_GENERIC_METHODS_DEC( COpMathExp )
};

#endif // OP_INC_OP_MATH_EXP
#endif // _OP_MATH_EXP_H_
