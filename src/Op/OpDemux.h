// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDemux.h

#ifndef _OP_DEMUX_H_
#define _OP_DEMUX_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_DEMUX

// Demultiplexer
////////////////////////////////////////////////////////////////////////////////
class COpDemux : public COp
{
public:
	COpDemux();
	virtual ~COpDemux();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES

	OP_GENERIC_COPY_CTOR_DEC( COpDemux )
	OP_GENERIC_METHODS_DEC( COpDemux )

protected:
	unsigned int m_uiPos;
};

#endif // OP_INC_OP_DEMUX
#endif // _OP_DEMUX_H_
