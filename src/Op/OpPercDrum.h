// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpPercDrum.h

#ifndef _OP_PERC_DRUM_H_
#define _OP_PERC_DRUM_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_PERC_DRUM

#include "../OpTk/OpTkEnv.h"
#include "../OpTk/OpTkFlt.h"
#include "../OpTk/OpTkOsc.h"
#include "../OpTk/OpTkSeq.h"

// Drum
////////////////////////////////////////////////////////////////////////////////
class COpPercDrum : public COp
{
public:
	COpPercDrum();
	virtual ~COpPercDrum();
protected:
	void Init();
	inline void UpdateEnvFrq();
	inline void UpdateEnvAmp();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpPercDrum )
	OP_GENERIC_METHODS_DEC( COpPercDrum )
	
protected:
	COpTkEnv2Exp<double> m_oEnvFrq, m_oEnvAmp;
	COpTkSeqRandomEx<double> m_oSeq;
	COpTkOscSinVar<double, OP_SAMPLE_FREQ> m_oOsc;
	COpTkFltHighLow0<double, OP_SAMPLE_FREQ> m_oFlt;
	unsigned int m_uiFuncNum;
	double m_dDur;
	double m_dFrqRatio, m_dFrqSlope0, m_dFrqSlope1;
	double m_dAmpRatio, m_dAmpSlope0, m_dAmpSlope1;
};

#endif // OP_INC_OP_PERC_DRUM
#endif // _OP_PERC_DRUM_H_
