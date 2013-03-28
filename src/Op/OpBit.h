// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpBit.h

#ifndef _OP_BIT_H_
#define _OP_BIT_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_BIT

// Bit Operator
////////////////////////////////////////////////////////////////////////////////
class COpBit : public COp
{
public:
	COpBit();
	virtual ~COpBit();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpBit )
	OP_GENERIC_METHODS_DEC( COpBit )
protected:
	static inline int FuncAND_( int iX, int iY );
	static inline int FuncOR_( int iX, int iY );
	static inline int FuncXOR_( int iX, int iY );
	static inline int FuncNOT_( int iX, int iY );
	static inline int FuncSAL_( int iX, int iY );
	static inline int FuncSAR_( int iX, int iY );
	
protected:
	unsigned int m_uiFuncNum;
	TOpFuncInt2 m_tFunc;
	double m_dFacUp, m_dFacDown;
	
protected:
	static TOpFuncInt2 m_atFunc_[];
};

#include "OpBit.inline.h"

#endif // OP_INC_OP_BIT
#endif // _OP_BIT_H_
