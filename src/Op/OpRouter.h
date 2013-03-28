// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpRouter.h

#ifndef _OP_ROUTER_H_
#define _OP_ROUTER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ROUTER

// Router
////////////////////////////////////////////////////////////////////////////////
class COpRouter : public COp
{
public:
	COpRouter();
	virtual ~COpRouter();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES

	OP_GENERIC_COPY_CTOR_DEC( COpRouter )
	OP_GENERIC_METHODS_DEC( COpRouter )

protected:
	int m_iPosPrev;
};

#endif // OP_INC_OP_ROUTER
#endif // _OP_ROUTER_H_
