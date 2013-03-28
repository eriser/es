// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSelector.h

#ifndef _OP_SELECTOR_H_
#define _OP_SELECTOR_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SELECTOR

// Selector
////////////////////////////////////////////////////////////////////////////////
class COpSelector : public COp
{
public:
	COpSelector();
	virtual ~COpSelector();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpSelector )
	OP_GENERIC_METHODS_DEC( COpSelector )
	
protected:
	unsigned int m_uiCountPos;
};

#endif // OP_INC_OP_SELECTOR
#endif // _OP_SELECTOR_H_
