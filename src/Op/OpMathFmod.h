// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMathFmod.h

#ifndef _OP_MATH_FMOD_H_
#define _OP_MATH_FMOD_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MATH_FMOD

// Modulo Function
////////////////////////////////////////////////////////////////////////////////
class COpMathFmod : public COp
{
public:
	COpMathFmod();
	virtual ~COpMathFmod();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpMathFmod )
	OP_GENERIC_METHODS_DEC( COpMathFmod )
};

#endif // OP_INC_OP_MATH_FMOD
#endif // _OP_MATH_FMOD_H_
