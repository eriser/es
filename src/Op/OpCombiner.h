// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpCombiner.h

#ifndef _OP_COMBINER_H_
#define _OP_COMBINER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_COMBINER

// Combiner
////////////////////////////////////////////////////////////////////////////////
class COpCombiner : public COp
{
public:
	COpCombiner();
	virtual ~COpCombiner();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES

	OP_GENERIC_COPY_CTOR_DEC( COpCombiner )
	OP_GENERIC_METHODS_DEC( COpCombiner )
	
protected:
	unsigned int m_uiFuncNum;
};

#endif // OP_INC_OP_COMBINER
#endif // _OP_COMBINER_H_
