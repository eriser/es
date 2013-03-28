// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpChorus.h

#ifndef _OP_CHORUS_H_
#define _OP_CHORUS_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_CHORUS

#include "../OpTk/OpTkBufferDelay.h"

// Chorus
////////////////////////////////////////////////////////////////////////////////
class COpChorus : public COp
{
public:
	COpChorus();
	virtual ~COpChorus();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES

	OP_GENERIC_COPY_CTOR_DEC( COpChorus )
	OP_GENERIC_METHODS_DEC( COpChorus )
protected:
	unsigned int m_uiMode;
	COpTkBufferVarDelay<double> m_oDelay;
	COpTkBufferVarDelayInterpol<double> m_oDelayInterpol;
	double m_dDelay;
};

#endif // OP_INC_OP_CHORUS
#endif // _OP_CHORUS_H_
