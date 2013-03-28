// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiFileIn.h

#ifndef _OP_MIDI_FILE_IN_H_
#define _OP_MIDI_FILE_IN_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MIDI_FILE_IN

#include "../Midi/Midi.h"
#include "../File/FileBlockMidi.h"

// MIDI Input File
////////////////////////////////////////////////////////////////////////////////
class COpMidiFileIn : public COp
{
public:
	COpMidiFileIn();
	virtual ~COpMidiFileIn();
	COpMidiFileIn & operator = ( const COpMidiFileIn &roO );
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpMidiFileIn )
	OP_GENERIC_METHODS_DEC( COpMidiFileIn )
	
public:
	CMidiInFile m_oFile;
	unsigned int m_uiFileNameLen;
	unsigned int m_auiFileName[64];
};

#endif // OP_INC_OP_MIDI_FILE_IN
#endif // _OP_MIDI_FILE_IN_H_
