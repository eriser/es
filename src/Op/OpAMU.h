// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpAMU.h

#ifndef _OP_AMU_H_
#define _OP_AMU_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_AMU

// Add/Multiply
////////////////////////////////////////////////////////////////////////////////
class COpAMU : public COp
{
public:
	COpAMU();
	virtual ~COpAMU();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpAMU )
	OP_GENERIC_METHODS_DEC( COpAMU )
};

#endif // OP_INC_OP_AMU
#endif // _OP_AMU_H_
