// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiDevIn.h

#ifndef _OP_MIDI_DEV_IN_H_
#define _OP_MIDI_DEV_IN_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MIDI_DEV_IN

#include "../Midi/Midi.h"

// MIDI Input Device
////////////////////////////////////////////////////////////////////////////////
class COpMidiDevIn : public COp
{
public:
	COpMidiDevIn();
	virtual ~COpMidiDevIn();
	COpMidiDevIn & operator = ( const COpMidiDevIn &roO );
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpMidiDevIn )
	OP_GENERIC_METHODS_DEC( COpMidiDevIn )

	unsigned int m_uiDevNum;
public:
	CMidiInDev m_oDev;
};

#endif // OP_INC_OP_MIDI_DEV_IN
#endif // _OP_MIDI_DEV_IN_H_
