// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiIn.h

#ifndef _OP_MIDI_IN_H_
#define _OP_MIDI_IN_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MIDI_IN

#include "OpMidiDevIn.h"
#include "OpMidiFileIn.h"

// MIDI Input
////////////////////////////////////////////////////////////////////////////////
class COpMidiIn : public COp
{
public:
	COpMidiIn();
	virtual ~COpMidiIn();
	COpMidiIn & operator = ( const COpMidiIn &roO );
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	virtual void Validate();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpMidiIn )
	OP_GENERIC_METHODS_DEC( COpMidiIn )
	
protected:
	CMidiIn *m_poDev;
	unsigned int m_uiFuncNum;
	int m_iMidiClass;
	
	bool m_aabNoteOnPrev[ MIDI_COUNT_CHANNELS ][ MIDI_COUNT_NOTES ];
};

#endif // OP_INC_OP_MIDI_IN
#endif // _OP_MIDI_IN_H_
