// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMix.h

#ifndef _OP_MIX_H_
#define _OP_MIX_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MIX

// Mixer
////////////////////////////////////////////////////////////////////////////////
class COpMix : public COp
{
public:
	COpMix();
	virtual ~COpMix();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpMix )
	OP_GENERIC_METHODS_DEC( COpMix )
	
protected:
	unsigned m_uiFuncNum;
};

#endif // OP_INC_OP_MIX
#endif // _OP_MIX_H_
