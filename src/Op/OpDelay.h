// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDelay.h

#ifndef _OP_DELAY_H_
#define _OP_DELAY_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_DELAY

#include "../OpTk/OpTkBufferDelay.h"

// Delay
////////////////////////////////////////////////////////////////////////////////
class COpDelay : public COp
{
public:
	COpDelay();
	virtual ~COpDelay();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpDelay )
	OP_GENERIC_METHODS_DEC( COpDelay )
protected:
	COpTkBufferVarDelay<double> m_oDelay;
	COpTkBufferVarDelayInterpol<double> m_oDelayInterpol;
	unsigned int m_uiMode;
	double m_dDelay;
};

#endif // OP_INC_OP_DELAY
#endif // _OP_DELAY_H_
