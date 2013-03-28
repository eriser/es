// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpReverbAL.h

#ifndef _OP_REVERB_AL_H_
#define _OP_REVERB_AL_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_REVERB_AL

struct ALeffectslot;
struct ALeffect;

// ReverbAL
//
// -> Experimental modification/test/adaption of OpenAL (Soft) Reverb.
//    
//    The original code can be found here:
//    http://connect.creativelabs.com/openal/Downloads/
//
//    It's licensed under LGPL, so there might be some issues...
////////////////////////////////////////////////////////////////////////////////
class COpReverbAL : public COp
{
public:
	COpReverbAL();
	virtual ~COpReverbAL();
	COpReverbAL & operator = ( const COpReverbAL &roO );
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpReverbAL )
	OP_GENERIC_METHODS_DEC( COpReverbAL )

protected:
	ALeffectslot * m_poSlot;
	ALeffect* m_poEffect;
	double m_adParamPrev[15];
};

#endif // OP_INC_OP_REVERB_AL
#endif // _OP_REVERB_AL_H_
