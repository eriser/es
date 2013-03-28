// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSampler.h

#ifndef _OP_SAMPLER_H_
#define _OP_SAMPLER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_SAMPLER

#include "../File/FileStreamWav.h"
#include "../OpTk/OpTkBuffer.h"

// Sampler 
// - rem: COpFileWavIn is similar, but simpler.
// - Problem: COpTkBufferVarInterpol:
//   Interpolation doesn't handle loop region correctly.
////////////////////////////////////////////////////////////////////////////////
class COpSampler : public COp
{
public:
	COpSampler();
	virtual ~COpSampler();
	COpSampler & operator = ( const COpSampler &roO );
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpSampler )
	OP_GENERIC_METHODS_DEC( COpSampler )
protected:
	COpTkBufferVarInterpol<double> m_oBuffer;
	//COpTkBufferVar<double> m_oBuffer;
	unsigned int m_uiFileNameLen;
	unsigned int m_auiFileName[64];
	unsigned int m_uiFuncNum;
	double m_dBufferPos;
	
	double m_dMIDIRootKey;
	unsigned int m_uiLoopType;
	unsigned int m_uiLoopStart;
	unsigned int m_uiLoopEnd;
	unsigned int m_uiLoopPlayCount;
	unsigned int m_uiLoopCounter;
	bool m_bIsSustained;
	bool m_bIsFresh;
	double m_dStpSample;
	double m_dSampleRateFactor;
};

#endif // OP_INC_OP_SAMPLER
#endif // _OP_SAMPLER_H_
