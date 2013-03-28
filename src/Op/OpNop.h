// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpNop.h

#ifndef _OP_NOP_H_
#define _OP_NOP_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_NOP

// No Operation
////////////////////////////////////////////////////////////////////////////////
class COpNop : public COp
{
public:
	COpNop();
	virtual ~COpNop();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpNop )
	OP_GENERIC_METHODS_DEC( COpNop )
};

#endif // OP_INC_OP_NOP
#endif // _OP_NOP_H_
