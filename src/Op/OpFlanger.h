// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFlanger.h

#ifndef _OP_FLANGER_H_
#define _OP_FLANGER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLANGER

#include "../OpTk/OpTkOsc.h"
#include "../OpTk/OpTkBufferDelay.h"

// Flanger
////////////////////////////////////////////////////////////////////////////////
class COpFlanger : public COp
{
public:
	COpFlanger();
	virtual ~COpFlanger();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpFlanger )
	OP_GENERIC_METHODS_DEC( COpFlanger )
protected:
	COpTkBufferVarDelayInterpol<double> m_oDelay;
};

#endif // OP_INC_OP_FLANGER
#endif // _OP_FLANGER_H_
