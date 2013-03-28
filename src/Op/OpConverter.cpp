// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpConverter.cpp

#include "OpConverter.h"
#ifdef OP_INC_OP_CONVERTER

#include "../Midi/Midi.h"

////////////////////////////////////////////////////////////////////////////////
// COpConverter
////////////////////////////////////////////////////////////////////////////////

#define OP_CVT_IN_BPM			0
#define OP_CVT_IN_MIDI			1
#define OP_CVT_IN_MIDIBASE		2
#define OP_CVT_OUT_BPM			0
#define OP_CVT_OUT_MIDI			1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_CVT_IN_BPM_INFO		"BPM"
#define OP_CVT_IN_MIDI_INFO		"MIDI note"
#define OP_CVT_IN_MIDIBASE_INFO		"base frequency"
#define OP_CVT_OUT_BPM_INFO		"number of samples"
#define OP_CVT_OUT_MIDI_INFO		"frequency"
#define OP_CVT_CLASS_INFO \
\
"Converter\n" \
"- BPM units (beats per minute) can be converted to samples per beat.\n" \
"- MIDI notes can be converted to their corresponding frequency.\n" \
"- MIDI note 69 matches with the middle A, at 440 Hz."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_CVT_IN_BPM_INFO		"BPM"
#define OP_CVT_IN_MIDI_INFO		"MIDI Note"
#define OP_CVT_IN_MIDIBASE_INFO		"Basisfrequenz"
#define OP_CVT_OUT_BPM_INFO		"Anzahl Samples"
#define OP_CVT_OUT_MIDI_INFO		"Frequenz"
#define OP_CVT_CLASS_INFO \
\
"Konverter\n" \
"- Geschwindigkeiten in der BMP-Einheit können in die entsprechende Anzahl\n" \
"  Samples pro Schlag umgewandelt werden.\n" \
"- MIDI-Noten können in entsprechende Frequenzen umgewandelt werden.\n" \
"- MIDI-Note 69 entspricht dem mittleren A und wird standardmässig\n" \
"  auf 440 Hz festgelegt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpConverter::COpConverter()
: COp( 3, 2 )
{
	//Init();
	OP_SET_NAME_INSTANCE( "conv" );
	m_poIn[OP_CVT_IN_BPM].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_CVT_IN_MIDI].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_CVT_IN_MIDIBASE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_CVT_IN_BPM, "bm", OP_CVT_IN_BPM_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CVT_IN_MIDI, "nt", OP_CVT_IN_MIDI_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CVT_IN_MIDIBASE, "fb", OP_CVT_IN_MIDIBASE_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CVT_OUT_BPM, "s", OP_CVT_OUT_BPM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CVT_OUT_MIDI, "f", OP_CVT_OUT_MIDI_INFO );
	ConvertBPM( 0.0 );
	ConvertMIDI( 0.0, 440.0 );
	Update();
	Validate();
}

COpConverter::~COpConverter()
{}

void COpConverter::ConvertBPM( double dBPM )
{
	m_dBPM = dBPM;
	m_pdOut[OP_CVT_OUT_BPM] = ( dBPM ? ( OP_SAMPLE_FREQ * 60.0 ) / dBPM : 0.0 );
}

void COpConverter::ConvertMIDI( double dMIDI, double dBase )
{
	m_dMIDI = dMIDI;
	m_dBase = dBase;
	m_pdOut[OP_CVT_OUT_MIDI] = MIDI_NOTE_TO_FREQ( dBase, dMIDI );
}

void COpConverter::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();

	if( m_poIn[OP_CVT_IN_BPM].IsValid() )
	{
		const double dBPM = m_poIn[OP_CVT_IN_BPM].Proc();
		if( dBPM != m_dBPM )
			ConvertBPM( dBPM );
	}
	if( m_poIn[OP_CVT_IN_MIDI].IsValid() )
	{
		const double dMIDI = m_poIn[OP_CVT_IN_MIDI].Proc();
		double dBase = 440.0;
		if( m_poIn[OP_CVT_IN_MIDIBASE].IsValid() )
			dBase = m_poIn[OP_CVT_IN_MIDIBASE].Proc();
		if( dMIDI != m_dMIDI || dBase != m_dBase )
			ConvertMIDI( dMIDI, dBase );
	}
}

void COpConverter::Reset()
{
	ConvertBPM( m_dBPM );
	ConvertMIDI( m_dMIDI, m_dBase );
	COp::Update();
}

OP_SET_NAME_AND_INFO_CLASS( COpConverter, OP_CVT_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpConverter )
OP_GENERIC_METHODS_DEF( COpConverter )

#endif // OP_INC_OP_CONVERTER
