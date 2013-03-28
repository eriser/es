// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpIf.h

#ifndef _OP_IF_H_
#define _OP_IF_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_IF

// If Condition
////////////////////////////////////////////////////////////////////////////////
class COpIf : public COp
{
public:
	COpIf();
	virtual ~COpIf();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpIf )
	OP_GENERIC_METHODS_DEC( COpIf )
};

#endif // OP_INC_OP_IF
#endif // _OP_IF_H_
