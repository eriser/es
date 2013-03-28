// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFader.h

#ifndef _OP_FADER_H_
#define _OP_FADER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FADER

// Fader
////////////////////////////////////////////////////////////////////////////////
class COpFader : public COp
{
public:
	COpFader();
	virtual ~COpFader();
public:
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpFader )
	OP_GENERIC_METHODS_DEC( COpFader )
};

#endif // OP_INC_OP_FADER
#endif // _OP_FADER_H_
