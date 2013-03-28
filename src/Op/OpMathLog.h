// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathLog.h

#ifndef _OP_MATH_LOG_H_
#define _OP_MATH_LOG_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MATH_LOG

// Logarithm Function
////////////////////////////////////////////////////////////////////////////////
class COpMathLog : public COp
{
public:
	COpMathLog();
	virtual ~COpMathLog();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpMathLog )
	OP_GENERIC_METHODS_DEC( COpMathLog )
};

#endif // OP_INC_OP_MATH_LOG
#endif // _OP_MATH_LOG_H_
