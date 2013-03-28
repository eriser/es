// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMul.h

#ifndef _OP_MUL_H_
#define _OP_MUL_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MUL

// Multiplication
////////////////////////////////////////////////////////////////////////////////
class COpMul : public COp
{
public:
	COpMul();
	virtual ~COpMul();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpMul )
	OP_GENERIC_METHODS_DEC( COpMul )
};

#endif // OP_INC_OP_MUL
#endif // _OP_MUL_H_
