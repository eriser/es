// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpCmp.h

#ifndef _OP_CMP_H_
#define _OP_CMP_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_CMP

// Compare
////////////////////////////////////////////////////////////////////////////////
class COpCmp : public COp
{
public:
	COpCmp();
	virtual ~COpCmp();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpCmp )
	OP_GENERIC_METHODS_DEC( COpCmp )
protected:
	static double FuncGreater_( double dX, double dY );
	static double FuncGreaterOrEqual_( double dX, double dY );
	static double FuncLess_( double dX, double dY );
	static double FuncLessOrEqual_( double dX, double dY );
	static double FuncEqual_( double dX, double dY );
	static double FuncNotEqual_( double dX, double dY );
	
protected:
	unsigned int m_uiNum;
	TOpFuncFlt2 m_tFunc;
protected:
	static TOpFuncFlt2 m_atFunc_[];
};

#include "OpCmp.inline.h"

#endif // OP_INC_OP_CMP
#endif // _OP_CMP_H_
