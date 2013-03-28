// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpLoop.h

#ifndef _OP_LOOP_H_
#define _OP_LOOP_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_LOOP

// Loop
////////////////////////////////////////////////////////////////////////////////
class COpLoop : public COp
{
public:
	COpLoop();
	virtual ~COpLoop();
protected:
	void Init();
	int GetRep( unsigned int iPos );
	void GetStep( int *piStep, int *piInc );
	inline unsigned int GetCountGroup() const;
	bool IncPos( int iInc, int *piPos );
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpLoop )
	OP_GENERIC_METHODS_DEC( COpLoop )
	
protected:
	int m_iPos, m_iRep;
	unsigned int m_uiCountVal;
};

#endif // OP_INC_OP_LOOP
#endif // _OP_LOOP_H_
