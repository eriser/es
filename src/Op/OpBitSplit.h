// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpBit.h

#ifndef _OP_BIT_SPLIT_H_
#define _OP_BIT_SPLIT_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_BIT_SPLIT

// BitSplit Operator
////////////////////////////////////////////////////////////////////////////////
class COpBitSplit : public COp
{
public:
	COpBitSplit();
	virtual ~COpBitSplit();
protected:
public:	
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpBitSplit )
	OP_GENERIC_METHODS_DEC( COpBitSplit )
	
protected:
	unsigned int m_uiFuncNum;
	TOpFuncInt2 m_tFunc;
	double m_dFacUp, m_dFacDown;
	
protected:
	static TOpFuncInt2 m_atFunc_[];
};

#endif // OP_INC_OP_BIT_SPLIT
#endif // _OP_BIT_SPLIT_H_
