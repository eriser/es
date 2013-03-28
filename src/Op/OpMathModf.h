// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathModf.h

#ifndef _OP_MATH_MODF_H_
#define _OP_MATH_MODF_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MATH_MODF

// modf Function
////////////////////////////////////////////////////////////////////////////////
class COpMathModf : public COp
{
public:
	COpMathModf();
	virtual ~COpMathModf();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpMathModf )
	OP_GENERIC_METHODS_DEC( COpMathModf )
};

#endif // OP_INC_OP_MATH_MODF
#endif // _OP_MATH_MODF_H_
