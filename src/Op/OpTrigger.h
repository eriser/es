// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTrigger.h

#ifndef _OP_TRIGGER_H_
#define _OP_TRIGGER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_TRIGGER

// Trigger
////////////////////////////////////////////////////////////////////////////////
class COpTrigger : public COp
{
public:
	COpTrigger();
	virtual ~COpTrigger();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpTrigger )
	OP_GENERIC_METHODS_DEC( COpTrigger )
protected:
	double m_dV;
};

#endif // OP_INC_OP_TRIGGER
#endif // _OP_TRIGGER_H_
