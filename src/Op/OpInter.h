// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpInter.h

#ifndef _OP_INTER_H_
#define _OP_INTER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_INTER

// Interface
////////////////////////////////////////////////////////////////////////////////
class COpInter : public COp
{
public:
	COpInter();
	virtual ~COpInter();
protected:
	void Init();
	void ClearBufferIn();
	void ClearBufferOut();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpInter )
	OP_GENERIC_METHODS_DEC( COpInter )
public:
	static double *m_pdBufferIn_, *m_pdBufferOut_;
	static unsigned int m_uiBufferInCount_, m_uiBufferOutCount_;
protected:
	static bool m_bInst_;
};

#endif // OP_INC_OP_INTER
#endif // _OP_INTER_H_
