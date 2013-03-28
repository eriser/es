// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpLadder.h

#ifndef _OP_LADDER_H_
#define _OP_LADDER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_LADDER

// Ladder
////////////////////////////////////////////////////////////////////////////////
class COpLadder : public COp
{
public:
	COpLadder();
	virtual ~COpLadder();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES

	OP_GENERIC_COPY_CTOR_DEC( COpLadder )
	OP_GENERIC_METHODS_DEC( COpLadder )

protected:
	double m_dIn, m_dStp;
	unsigned int m_uiFuncNum;
};

#endif // OP_INC_OP_CHORUS
#endif // _OP_LADDER_H_
