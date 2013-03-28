// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpShaper.h

#ifndef _OP_SHAPER_H_
#define _OP_SHAPER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SHAPER

// Waveshaper
////////////////////////////////////////////////////////////////////////////////
class COpShaper : public COp
{
public:
	COpShaper();
	virtual ~COpShaper();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpShaper )
	OP_GENERIC_METHODS_DEC( COpShaper )
protected:
	unsigned m_uiFuncNum;
};

#endif // OP_INC_OP_SHAPER
#endif // _OP_SHAPER_H_
