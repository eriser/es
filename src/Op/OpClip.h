// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpClip.h

#ifndef _OP_CLIP_H_
#define _OP_CLIP_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_CLIP

// Clip
// ref: fold back distortion: www.musicdsp.org, hellfire[AT]upb[DOT]de
////////////////////////////////////////////////////////////////////////////////
class COpClip : public COp
{
public:
	COpClip();
	virtual ~COpClip();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES

	OP_GENERIC_COPY_CTOR_DEC( COpClip )
	OP_GENERIC_METHODS_DEC( COpClip )
protected:
	unsigned int m_uiFuncNum;
};

#endif // OP_INC_OP_CLIP
#endif // _OP_CLIP_H_
