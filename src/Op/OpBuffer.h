// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpBuffer.h

#ifndef _OP_BUFFER_H_
#define _OP_BUFFER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_BUFFER

#include "../OpTk/OpTkBuffer.h"

// Buffer
////////////////////////////////////////////////////////////////////////////////
class COpBuffer : public COp
{
public:
	COpBuffer();
	virtual ~COpBuffer();
	COpBuffer & operator = ( const COpBuffer &roO );
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpBuffer )
	OP_GENERIC_METHODS_DEC( COpBuffer )
protected:
	COpTkBufferVarInterpol<double> m_oBuffer;
protected:
	unsigned int m_uiFuncNumIRead;
	unsigned int m_uiModeWrite;
};

#endif // OP_INC_OP_BUFFER
#endif // _OP_BUFFER_H_
