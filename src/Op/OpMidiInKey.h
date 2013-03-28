// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiInKey.h

#ifndef _OP_MIDI_IN_KEY_H_
#define _OP_MIDI_IN_KEY_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_MIDI_IN_KEY

#include "OpMidiDevIn.h"
#include "OpMidiFileIn.h"
#include "../Array/Array.h"

// MIDI Keyboard
////////////////////////////////////////////////////////////////////////////////
class COpMidiInKey : public COp, public CMidiIn::CHandler
{
protected:
	struct SVoice
	{
		SVoice();
		void Reset( unsigned int uiStamp );
		inline bool Update( unsigned int uiStamp );
		bool m_bIsOn, m_bTriggOn, m_bTriggOff;
		unsigned char m_ucNote;
		unsigned int m_uiStamp;
	};
public:
	COpMidiInKey();
	virtual ~COpMidiInKey();
	COpMidiInKey & operator = ( const COpMidiInKey &roO );
protected:
	void Init();
	void Detach();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Validate();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpMidiInKey )
	OP_GENERIC_METHODS_DEC( COpMidiInKey )
	
public:
	virtual void OnNote( bool bOn, unsigned char ucChannel, unsigned char ucNote, unsigned char ucVelocity );
	virtual void OnDestroy();
	
protected:
	CMidiIn *m_poDev;
	CArray<SVoice> m_oArrVoice;
	double m_dCountVoice;
	int m_iMidiClass;
};

#endif // OP_INC_OP_MIDI_IN_KEY
#endif // _OP_MIDI_IN_KEY_H_
