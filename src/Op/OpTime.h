// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpOscTime.h

#ifndef _OP_TIME_H_
#define _OP_TIME_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_TIME

// Time
////////////////////////////////////////////////////////////////////////////////
class COpTime : public COp
{
public:
	COpTime();
	virtual ~COpTime();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpTime )
	OP_GENERIC_METHODS_DEC( COpTime )
	
protected:
	double m_dTime;
};

#endif // OP_INC_OP_TIME
#endif // _OP_TIME_H_
