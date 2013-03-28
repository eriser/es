// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpRetrigger.h

#ifndef _OP_RETRIGGER_H_
#define _OP_RETRIGGER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_RETRIGGER

// Retrigger
////////////////////////////////////////////////////////////////////////////////
class COpRetrigger : public COp
{
public:
	COpRetrigger();
	virtual ~COpRetrigger();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpRetrigger )
	OP_GENERIC_METHODS_DEC( COpRetrigger )

protected:
	int m_iPos;
};

#endif // OP_INC_OP_RETRIGGER
#endif // _OP_RETRIGGER_H_
