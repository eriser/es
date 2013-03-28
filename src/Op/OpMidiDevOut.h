// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiDevOut.h

#ifndef _OP_MIDI_DEV_OUT_H_
#define _OP_MIDI_DEV_OUT_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MIDI_DEV_OUT

#include "../Midi/Midi.h"

// MIDI Output Device
////////////////////////////////////////////////////////////////////////////////
class COpMidiDevOut : public COp
{
public:
	COpMidiDevOut();
	virtual ~COpMidiDevOut();
	COpMidiDevOut & operator = ( const COpMidiDevOut &roO );
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpMidiDevOut )
	OP_GENERIC_METHODS_DEC( COpMidiDevOut )

protected:
	unsigned int m_uiDevNum;
public:
	CMidiOut m_oDev;
};

#endif // OP_INC_OP_MIDI_DEV_OUT
#endif // _OP_MIDI_DEV_OUT_H_
