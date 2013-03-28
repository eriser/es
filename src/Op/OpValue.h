// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpValue.h

#ifndef _OP_VALUE_H_
#define _OP_VALUE_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_VALUE

// Value
////////////////////////////////////////////////////////////////////////////////
class COpValue : public COp
{
public:
	COpValue();
	virtual ~COpValue();
public:
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpValue )
	OP_GENERIC_METHODS_DEC( COpValue )
};

#endif // OP_INC_OP_VALUE
#endif // _OP_VALUE_H_
