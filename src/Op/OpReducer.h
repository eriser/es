// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpReducer.h

#ifndef _OP_REDUCER_H_
#define _OP_REDUCER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_REDUCER

// Reducer
////////////////////////////////////////////////////////////////////////////////
class COpReducer : public COp
{
public:
	COpReducer();
	virtual ~COpReducer();
public:	
	virtual void Proc();
	
	OP_GENERIC_COPY_CTOR_DEC( COpReducer )
	OP_GENERIC_METHODS_DEC( COpReducer )
};

#endif // OP_INC_OP_REDUCER
#endif // _OP_REDUCER_H_
