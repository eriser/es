// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpLogic.h

#ifndef _OP_LOGIC_H_
#define _OP_LOGIC_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_LOGIC

#define OP_LOGIC_FUNC_NUM	3

// Logical Conjunction
////////////////////////////////////////////////////////////////////////////////
class COpLogic : public COp
{
public:
	COpLogic();
	virtual ~COpLogic();
public:	
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpLogic )
	OP_GENERIC_METHODS_DEC( COpLogic )
protected:
	void Init();
protected:
	static inline bool FuncAND_( bool bX, bool bY );
	static inline bool FuncOR_( bool bX, bool bY );
	static inline bool FuncNOT_( bool bX, bool bY );
protected:
	unsigned int m_uiFuncNum;
	TOpFuncBool2 m_tFunc;
protected:
	static TOpFuncBool2 m_atFunc_[OP_LOGIC_FUNC_NUM];
};

#include "OpLogic.inline.h"

#endif // OP_INC_OP_LOGIC
#endif // _OP_LOGIC_H_
