// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpScale.h

#ifndef _OP_SCALE_H_
#define _OP_SCALE_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SCALE

// Scale
////////////////////////////////////////////////////////////////////////////////
class COpScale : public COp
{
public:
	COpScale();
	virtual ~COpScale();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES

	OP_GENERIC_COPY_CTOR_DEC( COpScale )
	OP_GENERIC_METHODS_DEC( COpScale )

public:
	unsigned int m_uiCountIndex;
};

#endif // OP_INC_OP_SCALE
#endif // _OP_SCALE_H_
