// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMAD.h

#ifndef _OP_MAD_H_
#define _OP_MAD_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MAD

// Multiply/Add
////////////////////////////////////////////////////////////////////////////////
class COpMAD : public COp
{
public:
	COpMAD();
	virtual ~COpMAD();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpMAD )
	OP_GENERIC_METHODS_DEC( COpMAD )
};

#endif // OP_INC_OP_MAD
#endif // _OP_MAD_H_
