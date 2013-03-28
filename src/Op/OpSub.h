// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSub.h

#ifndef _OP_SUB_H_
#define _OP_SUB_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SUB

// Subtraction
////////////////////////////////////////////////////////////////////////////////
class COpSub : public COp
{
public:
	COpSub();
	virtual ~COpSub();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpSub )
	OP_GENERIC_METHODS_DEC( COpSub )
};

#endif // OP_INC_OP_SUB
#endif // _OP_SUB_H_
