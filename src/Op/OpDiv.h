// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDiv.h

#ifndef _OP_DIV_H_
#define _OP_DIV_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_DIV

// Division
////////////////////////////////////////////////////////////////////////////////
class COpDiv : public COp
{
public:
	COpDiv();
	virtual ~COpDiv();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpDiv )
	OP_GENERIC_METHODS_DEC( COpDiv )
};

#endif // OP_INC_OP_DIV
#endif // _OP_DIV_H_
