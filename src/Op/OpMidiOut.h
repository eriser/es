// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiOut.h

#ifndef _OP_MIDI_OUT_H_
#define _OP_MIDI_OUT_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MIDI_OUT

#include "OpMidiDevOut.h"

// MIDI Output
////////////////////////////////////////////////////////////////////////////////
class COpMidiOut : public COp
{
public:
	COpMidiOut ();
	virtual ~COpMidiOut();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Validate();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpMidiOut )
	OP_GENERIC_METHODS_DEC( COpMidiOut )
protected:
	void Init();
	
protected:
	CMidiOut *m_poDev;
	unsigned int m_uiFuncNum;
};

#endif // OP_INC_OP_MIDI_OUT
#endif // _OP_MIDI_H_
