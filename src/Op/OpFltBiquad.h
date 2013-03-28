// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltBiquad.h

#ifndef _OP_FLT_BIQUAD_H_
#define _OP_FLT_BIQUAD_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_BIQUAD

#include "../OpTk/OpTkFlt.h"

// Biquad Filter
////////////////////////////////////////////////////////////////////////////////
class COpFltBiquad : public COp
{
public:
	COpFltBiquad();
	virtual ~COpFltBiquad();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltBiquad )
	OP_GENERIC_METHODS_DEC( COpFltBiquad )
protected:
	COpTkFltBiquad<double, OP_SAMPLE_FREQ>  m_oFlt;
	double m_dCutoff, m_dGain, m_dBandwidth;
	unsigned int m_uiFuncNum;
};

#endif // OP_INC_OP_FLT_BIQUAD
#endif // _OP_FLT_BIQUAD_H_
