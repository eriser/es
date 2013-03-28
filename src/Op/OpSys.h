// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscTime.h

#ifndef _OP_SYS_H_
#define _OP_SYS_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SYS

// System Functions
////////////////////////////////////////////////////////////////////////////////
class COpSys : public COp
{
public:
	COpSys();
	virtual ~COpSys();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpSys )
	OP_GENERIC_METHODS_DEC( COpSys )

protected:
	static unsigned int m_uiInstances_;
	bool m_bTrigger;
};

#endif // OP_INC_OP_SYS
#endif // _OP_SYS_H_
