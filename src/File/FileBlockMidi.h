// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FileBlockMidi.h

#ifndef _FILE_BLOCK_MIDI_H_
#define _FILE_BLOCK_MIDI_H_

#define FILE_BLOCK_MIDI_VERSION				"0.06" // 2010-12-09

#include "FileBlock.h"
#include "../Array/Array.h"
#include "../List/List.h"

// Midi Datei (Type 0 / Type 1)
// References:
// - http://www.borg.com/~jglatt/tech/midifile.htm
// - http://home.snafu.de/sicpaul/midi/midi3.htm
// - http://www.omega-art.com/midi/mfiles.html
// TODO:
// - Schreiben/Encode.
// - Type 2.
////////////////////////////////////////////////////////////////////////////////
class CFileBlockMidi : public CFileBlock
{
public:
	// Event
	////////////////////////////////////////////////////////////////////////////////
	class CEvent
	{
	public:
		CEvent();
		CEvent( unsigned long int uliDeltaTime, unsigned char ucStatus, unsigned char ucData0, unsigned ucData1=0 );
		~CEvent();
	public:
		unsigned long int m_uliDeltaTime;
		unsigned char m_ucStatus;
		unsigned char m_ucData0, m_ucData1;
	};
protected:
	// Track
	////////////////////////////////////////////////////////////////////////////////
	class CTrack
	{
	public:
		CTrack();
		CTrack( const CTrack &roO );
		~CTrack();
		CTrack & operator=( const CTrack &roO );
		void Reset();
		bool Proc( CFileBlockMidi::CEvent **ppoEvent );
	protected:
		CEvent * NextEvent();
		void Clear();
	public:
		CList<CEvent *> m_oLstEvent; // rem: Der letzte Event bedeuted 'End Of Track' und hat keinen Inhalt.
		CEvent *m_poEvent;
		unsigned long int m_uliCounter; // Anzahl Clocks bis zum Nächsten Event.
		unsigned long int m_uliDuration; // Anzahl Clocks insgesamt.
		//unsigned long int c; // tmp
	};
public:
	CFileBlockMidi();
	CFileBlockMidi( const CFileBlockMidi &roO );
	~CFileBlockMidi();
	CFileBlockMidi & operator=( const CFileBlockMidi &roO );
public:
	bool Load( const char *pcFileName );
	void Clear();
	//bool Decode( void *pvData, unsigned int uiBytes );
	//bool Encode( void *pvData, unsigned int uiBytes );
	
	unsigned int GetTrackCount() const;
	unsigned long int GetTrackDuration( unsigned int uiTrack ) const;
	unsigned long int GetTrackDurationMax() const;
	void ResetAllTracks();
	void ResetTrack( unsigned int uiTrack );
	bool Clock( unsigned int uiTrack, CEvent **ppoEvent );
public:
	unsigned int m_usiFormat;
	unsigned int m_usiDivision; // PPQN (Pulses Per Quarter Note / Resolution / Clocks Per Quarter)
	unsigned int m_uiMicroSecPerQuarter;
protected:
	CArray<CTrack *> m_oArrTrack;
};

#endif // _FILE_BLOCK_MIDI_H_
