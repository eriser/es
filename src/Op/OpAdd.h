// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpAdd.h

#ifndef _OP_ADD_H_
#define _OP_ADD_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ADD

// Addition
////////////////////////////////////////////////////////////////////////////////
class COpAdd : public COp
{
public:
	COpAdd();
	virtual ~COpAdd();
public:
	virtual void Proc();
	OP_GENERIC_COPY_CTOR_DEC( COpAdd )
	OP_GENERIC_METHODS_DEC( COpAdd )
};

#endif // OP_INC_OP_ADD
#endif // _OP_ADD_H_
