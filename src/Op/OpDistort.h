// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDistort.h

#ifndef _OP_DISTORT_H_
#define _OP_DISTORT_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_DISTORT

// Distortion
// ref: Bram de Jong, www.musicdsp.org.
////////////////////////////////////////////////////////////////////////////////
class COpDistort : public COp
{
public:
	COpDistort();
	virtual ~COpDistort();
public:
	virtual void Proc();

	OP_GENERIC_COPY_CTOR_DEC( COpDistort )
	OP_GENERIC_METHODS_DEC( COpDistort )
};

#endif // OP_INC_OP_DISTORT
#endif // _OP_DISTORT_H_
