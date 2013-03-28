// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpReverb.h

#ifndef _OP_REVERB_H_
#define _OP_REVERB_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_REVERB

#include "../OpTk/OpTkBufferDelay.h"

#define OP_REV_STAGES_COMB	4
#define OP_REV_STAGES_ALLP	3
#define OP_REV_DELAY_MAX	( OP_SAMPLE_FREQ * 4 )

// Reverb
////////////////////////////////////////////////////////////////////////////////
class COpReverb : public COp
{
public:
	COpReverb();
	virtual ~COpReverb();
protected:
	void Init();
	void InitRoomSize( double dRoomSize );
	void InitRoomDelay( double dDelay );
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpReverb )
	OP_GENERIC_METHODS_DEC( COpReverb )

protected:
	COpTkBufferFixedDelay<double, OP_REV_DELAY_MAX> m_aoAllpass[OP_REV_STAGES_ALLP];
	COpTkBufferFixedDelay<double, OP_REV_DELAY_MAX> m_aoComb[OP_REV_STAGES_COMB];
	double m_adCoeffComb[OP_REV_STAGES_COMB];
	double m_dRoomSize;
	double m_dRoomDelay;
	
	static unsigned int m_auiPrime_[OP_REV_STAGES_COMB];
};

#endif // OP_INC_OP_REVERB
#endif // _OP_REVERB_H_
