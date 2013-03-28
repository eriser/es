// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Midi.h
// help: http://www.borg.com/~jglatt/tutr/miditutr.htm
// http://www.zem-college.de/midi/index.htm
//
// rem:
// - Im Kopier-Konstruktor bzw. Zuswisungs-Operator wird versucht das Gerät
//   zu initialisieren. Im Normalfall schlägt dies jedoch fehl, da nur
//   ein Gerät nur einmal benutzt werden kann.
//

#ifndef _MIDI_H_
#define _MIDI_H_

// Präprozessor.
////////////////////////////////////////////////////////////////////////////////

// Extern...
//#define MIDI_USE_LOG
#define MIDI_USE_INFO_FUNC
#define MIDI_USE_INPUT_HANDLER
#define MIDI_USE_FILE

#define MIDI_VERSION			"0.11" // 2009-10-29

#ifdef MIDI_USE_LOG
	#include <cstdio>
	using namespace std;
	#define MIDI_LOG(msg)		printf(msg) // VA_ARGS funktioniert nicht bei allen Compilern.
	#define MIDI_LOG1(msg,x)	printf(msg,x)
	#define MIDI_LOG2(msg,x,y)	printf(msg,x,y)
	#define MIDI_LOG3(msg,x,y,z)	printf(msg,x,y,z)
	#define MIDI_ERROR_REPORT(msg)  { fprintf(stderr, "Fehler: %s\n", msg); return false; }
	#define MIDI_ASSERT(x) 		if( !(x) ) { printf( "ASSERT( %s ) in file %s, line %d\n", #x, __FILE__, __LINE__ ); }
#else
	#define MIDI_LOG(msg)
	#define MIDI_LOG1(msg,x)
	#define MIDI_LOG2(msg,x,y)
	#define MIDI_LOG3(msg,x,y,z)
	#define MIDI_ERROR_REPORT(msg)  { return false; }
	#define MIDI_ASSERT(x)
#endif // MIDI_USE_LOG


#define WORD_UPPER(lowdw)		((lowdw  & 0xFFFF0000) >>16 )
#define WORD_LOWER(highdw)		((highdw & 0x0000FFFF)      )

#define BYTE_UPPER(lowword)		((lowword  & 0xFF00) >>8 )
#define BYTE_LOWER(highword)		((highword & 0x00FF)     )

#define BYTE0(lng)			((lng & 0x000000FF)      )
#define BYTE1(lng)			((lng & 0x0000FF00) >>8  )
#define BYTE2(lng)			((lng & 0x00FF0000) >>16 )
#define BYTE3(lng)			((lng & 0xFF0000FF) >>24 )

#define NIBBLE_UPPER(byte)		((byte & 0xF0) >>4 )
#define NIBBLE_LOWER(byte)		((byte & 0x0F)     )

#define MIDI_NOTE_TO_FREQ(A,notenr)	(A / 32.0) * pow(2.0, ((notenr - 9.0) / 12.0))

#define MIDI_NOTE_OFF			0x80
#define MIDI_NOTE_ON			0x90
#define MIDI_POLY_AFTERTOUCH		0xA0
#define MIDI_CONTROL_CHANGE		0xB0
#define MIDI_PROGRAM_CHANGE		0xC0
#define MIDI_CHANNEL_AFTERTOUCH		0xD0
#define MIDI_PITCHBEND			0xE0

#define MIDI_MASK_STATUS		0xF0

#define MIDI_COUNT_CHANNELS		0x10	// 0-15
#define MIDI_COUNT_CONTROL		0x80	// 0-127
#define MIDI_COUNT_NOTES		0x80	// 0-127

#define MIDI_NEWLINE			"\n" // "\r\n"

// Includes.
////////////////////////////////////////////////////////////////////////////////

#ifdef MIDI_USE_LOG
#include <stdio.h>
#endif // MIDI_USE_LOG

#ifdef MIDI_USE_INPUT_HANDLER
#include "../List/List.h"
#endif // MIDI_USE_INPUT_HANDLER

#ifdef MIDI_USE_INFO_FUNC
#include "../Str/Str.h"
#endif //MIDI_USE_INFO_FUNC

#ifdef MIDI_USE_FILE
#include "../File/FileBlockMidi.h"
#endif // MIDI_USE_FILE

#ifdef __WINDOWS__
#include <windows.h>
#include <mmsystem.h>

#ifdef _MSC_VER
#pragma comment( lib, "winmm.lib" )
#endif
#endif // __WINDOWS__


// CMidi
////////////////////////////////////////////////////////////////////////////////
class CMidi
{
public:
	CMidi();
	CMidi( const CMidi &roO );
	~CMidi();
	CMidi & operator=( const CMidi &roO );
protected:
	int m_iDeviceID;
};



// CMidiIn
// rem:
// - Grosse Datentabelle, dafür einfach zugänglich und übersichtlich!
//   ( 3 * ( 16 * 128 ) + 4 * 16 ) Byte = 6.0625 KByte.
////////////////////////////////////////////////////////////////////////////////
class CMidiIn : public CMidi
{
public:
#ifdef MIDI_USE_INPUT_HANDLER
	// Callback Handler
	////////////////////////////////////////////////////////////////////////////////
	class CHandler
	{
	public:
		virtual void OnNote( bool bOn, unsigned char ucChannel, unsigned char ucNote, unsigned char ucVelocity ) {}
		virtual void OnPolyAftertouch( unsigned char ucChannel, unsigned char ucNote, unsigned char ucValue ) {}
		virtual void OnControlChange( unsigned char ucChannel, unsigned char ucNum, unsigned char ucValue ) {}
		virtual void OnProgramChange( unsigned char ucChannel, unsigned char ucNum ) {}
		virtual void OnChannelAftertouch( unsigned char ucChannel, unsigned char ucValue ) {}
		virtual void OnPitchbend( unsigned char ucChannel, int iValue ) {}
		virtual void OnDestroy() = 0; // Wird aufgerufen, wenn sich die CMidiIn löscht.
	};
#endif // MIDI_USE_INPUT_HANDLER

public:
	CMidiIn();
	CMidiIn( const CMidiIn &roO );
	~CMidiIn();
	CMidiIn & operator=( const CMidiIn &roO );

public:
	void Reset();				// Alle intern gespeicherten Daten zurücksetzen.

public:
#ifdef MIDI_USE_INPUT_HANDLER
	void AddHandler( CHandler *poH );	// Handler registrieren.
	void RemoveHandler( CHandler *poH );	// Handler entfernen.
#endif // MIDI_USE_INPUT_HANDLER

protected:
	void Handle( unsigned char ucStatus, unsigned char ucData0, unsigned char ucData1 );

public:
	bool m_aabNoteOn			[ MIDI_COUNT_CHANNELS ][ MIDI_COUNT_NOTES ];
	unsigned char m_aaucNoteVelocity	[ MIDI_COUNT_CHANNELS ][ MIDI_COUNT_NOTES ];
	unsigned char m_aaucPolyAftertouch	[ MIDI_COUNT_CHANNELS ][ MIDI_COUNT_NOTES ];
	unsigned char m_aaucControl		[ MIDI_COUNT_CHANNELS ][ MIDI_COUNT_CONTROL ];
	unsigned char m_aucProgram		[ MIDI_COUNT_CHANNELS ];
	unsigned char m_aucChannelAftertouch	[ MIDI_COUNT_CHANNELS ];
	unsigned int m_auiPitchbend		[ MIDI_COUNT_CHANNELS ];

protected:
#ifdef MIDI_USE_INPUT_HANDLER
	CList<CHandler *> m_oListHandler;
#endif // MIDI_USE_INPUT_HANDLER
};



#ifdef __WINDOWS__

// CMidiInDev
////////////////////////////////////////////////////////////////////////////////
class CMidiInDev : public CMidiIn
{
public:
	CMidiInDev();
	CMidiInDev( const CMidiInDev &roO );
	~CMidiInDev();
	CMidiInDev & operator=( const CMidiInDev &roO );

public:
	bool Init(				// Initialisieren.
		unsigned int uiDeviceID );	// Device-ID. Kann über GetInfoDevice(...) ausfindig gemacht werden.
	bool Clear();				// Deinitialisieren. Wird im Destruktor automatisch aufgerufen.
	void Reset();				// Alle intern gespeicherten Daten zurücksetzen.

public:
#ifdef MIDI_USE_INPUT_HANDLER
	void AddHandler( CHandler *poH );	// Handler registrieren.
	void RemoveHandler( CHandler *poH );	// Handler entfernen.
#endif // MIDI_USE_INPUT_HANDLER

public:
#ifdef MIDI_USE_INFO_FUNC
	static CStr GetInfoDevice_();		// Informationen über die verfügbaren MIDI-Eingabegeräte ausgeben.
#endif //MIDI_USE_INFO_FUNC
	unsigned int GetDeviceCount();

protected:
	static void CALLBACK Proc_(
		HMIDIIN hHandle,
		UINT_PTR uiMsg,
		DWORD_PTR dwInstance,
		DWORD_PTR dwParam1,
		DWORD dwParam2 );
protected:
	HMIDIIN m_hMidiInput;
};

#else // RtMidi

#include "RtMidi/RtMidi.h"

// CMidiInDev
////////////////////////////////////////////////////////////////////////////////
class CMidiInDev : public CMidiIn
{
public:
	CMidiInDev();
	CMidiInDev( const CMidiInDev &roO );
	~CMidiInDev();
	CMidiInDev & operator=( const CMidiInDev &roO );

public:
	bool Init(				// Initialisieren.
		unsigned int uiDeviceID );	// Device-ID. Kann über GetInfoDevice(...) ausfindig gemacht werden.
	bool Clear();				// Deinitialisieren. Wird im Destruktor automatisch aufgerufen.
	void Reset();				// Alle intern gespeicherten Daten zurücksetzen.

public:
#ifdef MIDI_USE_INPUT_HANDLER
	void AddHandler( CHandler *poH );	// Handler registrieren.
	void RemoveHandler( CHandler *poH );	// Handler entfernen.
#endif // MIDI_USE_INPUT_HANDLER

public:
#ifdef MIDI_USE_INFO_FUNC
	static CStr GetInfoDevice_();		// Informationen über die verfügbaren MIDI-Eingabegeräte ausgeben.
#endif //MIDI_USE_INFO_FUNC
	unsigned int GetDeviceCount();

protected:
	static void Proc_( double dDeltaTime, std::vector<unsigned char> *poMessage, void *pvUserData );
protected:
	RtMidiIn *m_poMidiIn;
};

#endif // platform




#ifdef MIDI_USE_FILE

// CMidiInFile
////////////////////////////////////////////////////////////////////////////////
class CMidiInFile : public CMidiIn
{
public:
	CMidiInFile();
	CMidiInFile( const CMidiInFile &roO );
	~CMidiInFile();
	CMidiInFile & operator=( const CMidiInFile &roO );

public:
	bool Init(				// Initialisieren.
		const char *pcFileName		// Dateiname.
		);
	bool Clear();				// Deinitialisieren. Wird im Destruktor automatisch aufgerufen.
	void Reset();				// Alle intern gespeicherten Daten zurücksetzen.
	void Proc();				// Muss pro Sample einmal aufgerufen werden.
	void UpdateMetrics();			// Aktualisiert die internen Zeitvariablen über m_uiSampleFreq, m_dBPM.

	unsigned int GetTrackCount() const;	// Anzahl Tracks.
	unsigned int GetDivision() const;	// Division.
	unsigned long int GetClockMax() const;	// Anzahl Clocks des längsten Tracks.
	double GetFileBPM() const;		// BPM, 0.0 falls nicht in der Datei gespeichert.
public:
	unsigned int m_uiSampleFreq;		// Samplerate.
	double m_dBPM;				// Tempo (Beats Per Minute).
	unsigned long int m_uliClock;		// Aktuelle Clock-Zahl.
protected:
	unsigned int m_uiSample;		// Aktuelles Sample (Ganzzahl).
	double m_dSamplePart;			// Aktuelles Sample (Part).
	double m_dSamplesPerQuarter;
	unsigned int m_uiSamplesPerClock;
	double m_dSamplesPerClockFrac;
	CFileBlockMidi m_oFile;
};

#endif // MIDI_USE_FILE



#ifdef __WINDOWS__

// CMidiOut
////////////////////////////////////////////////////////////////////////////////
class CMidiOut : public CMidi
{
public:
	CMidiOut();
	CMidiOut( const CMidiOut &roO );
	~CMidiOut();
	CMidiOut & operator=( const CMidiOut &roO );

public:
	bool Init(		   		// Initialisieren.
		unsigned int uiDeviceID	); 	// Device-ID. Kann über GetInfoDevice(...) ausfindig gemacht werden.
	bool Clear();				// Deinitialisieren. Wird im Destruktor automatisch aufgerufen.

public:
#ifdef MIDI_USE_INFO_FUNC
	static CStr GetInfoDevice_();		// Informationen über die verfügbaren MIDI-Eingabegeräte ausgeben.
#endif //MIDI_USE_INFO_FUNC
	unsigned int GetDeviceCount();

public:
	inline void Raw( long int liMsg );
	inline void Raw( unsigned char ucStatus, unsigned char ucVal0, unsigned char ucVal1 );

	inline void NoteOn		( unsigned char ucChannel, unsigned char ucNote, unsigned char ucVelocity	);
	inline void NoteOff		( unsigned char ucChannel, unsigned char ucNote, unsigned char ucVelocity	);
	inline void PolyAftertouch	( unsigned char ucChannel, unsigned char ucNote, unsigned ucValue		);
	inline void ControlChange	( unsigned char ucChannel, unsigned char ucNum, unsigned ucValue		);
	inline void ProgramChange	( unsigned char ucChannel, unsigned char ucNum					);
	inline void ChannelAftertouch	( unsigned char ucChannel, unsigned char ucValue				);
	inline void Pitchbend		( unsigned char ucChannel, unsigned char ucLSB, unsigned char ucMSB		);
	inline void Pitchbend		( unsigned char ucChannel, int iValue						);

protected:
	HMIDIOUT m_hMidiOutput;
};

#else // RtMidi

// CMidiOut
////////////////////////////////////////////////////////////////////////////////
class CMidiOut : public CMidi
{
public:
	CMidiOut();
	CMidiOut( const CMidiOut &roO );
	~CMidiOut();
	CMidiOut & operator=( const CMidiOut &roO );

public:
	bool Init(		   		// Initialisieren.
		unsigned int uiDeviceID	); 	// Device-ID. Kann über GetInfoDevice(...) ausfindig gemacht werden.
	bool Clear();				// Deinitialisieren. Wird im Destruktor automatisch aufgerufen.

public:
#ifdef MIDI_USE_INFO_FUNC
	static CStr GetInfoDevice_();		// Informationen über die verfügbaren MIDI-Eingabegeräte ausgeben.
#endif //MIDI_USE_INFO_FUNC
	unsigned int GetDeviceCount();

public:
	inline void Raw( long int liMsg );
	inline void Raw( unsigned char ucStatus, unsigned char ucVal0, unsigned char ucVal1 );

	inline void NoteOn		( unsigned char ucChannel, unsigned char ucNote, unsigned char ucVelocity	);
	inline void NoteOff		( unsigned char ucChannel, unsigned char ucNote, unsigned char ucVelocity	);
	inline void PolyAftertouch	( unsigned char ucChannel, unsigned char ucNote, unsigned ucValue		);
	inline void ControlChange	( unsigned char ucChannel, unsigned char ucNum, unsigned ucValue		);
	inline void ProgramChange	( unsigned char ucChannel, unsigned char ucNum					);
	inline void ChannelAftertouch	( unsigned char ucChannel, unsigned char ucValue				);
	inline void Pitchbend		( unsigned char ucChannel, unsigned char ucLSB, unsigned char ucMSB		);
	inline void Pitchbend		( unsigned char ucChannel, int iValue						);

protected:
	RtMidiOut *m_poMidiOut;
	std::vector<unsigned char> m_oMessage2, m_oMessage3;
};

#endif // platform



#include "Midi.inline.h"

#endif // _MIDI_H_
