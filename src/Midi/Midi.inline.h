// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Midi.inline.h

#ifdef __WINDOWS__

////////////////////////////////////////////////////////////////////////////////
// CMidiOut
////////////////////////////////////////////////////////////////////////////////

inline void CMidiOut::Raw( long int liMsg ) {
	midiOutShortMsg( m_hMidiOutput,	liMsg );
}

inline void CMidiOut::Raw( unsigned char ucStatus, unsigned char ucVal0, unsigned char ucVal1 ) {
	midiOutShortMsg( m_hMidiOutput, ucStatus | ( ucVal0 << 8 ) | ( ucVal1 << 16 ) );
}

inline void CMidiOut::NoteOn( unsigned char ucChannel, unsigned char ucNote, unsigned char ucVelocity ) {
	midiOutShortMsg(
		m_hMidiOutput,
		MIDI_NOTE_ON | ucChannel | ( ucNote << 8 ) | ( ucVelocity << 16 )
		);
}

inline void CMidiOut::NoteOff( unsigned char ucChannel, unsigned char ucNote, unsigned char ucVelocity ) {
	midiOutShortMsg(
		m_hMidiOutput,
		MIDI_NOTE_OFF | ucChannel | ( ucNote << 8 ) | ( ucVelocity << 16 )
		);
}

inline void CMidiOut::PolyAftertouch( unsigned char ucChannel, unsigned char ucNote, unsigned ucValue ) {
	midiOutShortMsg(
		m_hMidiOutput,
		MIDI_POLY_AFTERTOUCH | ucChannel | ( ucNote << 8 ) | ( ucValue << 16 )
		);
}

inline void CMidiOut::ControlChange( unsigned char ucChannel, unsigned char ucNum, unsigned ucValue ) {
	midiOutShortMsg(
		m_hMidiOutput,
		MIDI_CONTROL_CHANGE | ucChannel | ( ucNum << 8 ) | ( ucValue << 16 )
		);
}

inline void CMidiOut::ProgramChange( unsigned char ucChannel, unsigned char ucNum ) {
	midiOutShortMsg(
		m_hMidiOutput,
		MIDI_PROGRAM_CHANGE | ucChannel | ( ucNum << 8 )
		);
}

inline void CMidiOut::ChannelAftertouch( unsigned char ucChannel, unsigned char ucValue ) {
	midiOutShortMsg(
		m_hMidiOutput,
		MIDI_CHANNEL_AFTERTOUCH | ucChannel | ( ucValue << 8 )
		);
}

inline void CMidiOut::Pitchbend( unsigned char ucChannel, unsigned char ucLSB, unsigned char ucMSB ) {
	midiOutShortMsg(
		m_hMidiOutput,
		MIDI_PITCHBEND | ucChannel | (ucLSB << 8 ) | ( ucMSB << 16 )
		);
}

inline void CMidiOut::Pitchbend( unsigned char ucChannel, int iValue ) {
	midiOutShortMsg(
		m_hMidiOutput,
		MIDI_PITCHBEND | ucChannel | ( iValue << 8 )
		);
}

#else // RtMidi

////////////////////////////////////////////////////////////////////////////////
// CMidiOut
////////////////////////////////////////////////////////////////////////////////

inline void CMidiOut::Raw( long int liMsg )
{
	m_oMessage3[0] = BYTE0( liMsg );
	m_oMessage3[1] = BYTE1( liMsg );
	m_oMessage3[2] = BYTE2( liMsg );
	m_poMidiOut->sendMessage( &m_oMessage3 );
}

inline void CMidiOut::Raw( unsigned char ucStatus, unsigned char ucVal0, unsigned char ucVal1 )
{
	m_oMessage3[0] = ucStatus;
	m_oMessage3[1] = ucVal0;
	m_oMessage3[2] = ucVal1;
	m_poMidiOut->sendMessage( &m_oMessage3 );
}

inline void CMidiOut::NoteOn( unsigned char ucChannel, unsigned char ucNote, unsigned char ucVelocity )
{
	m_oMessage3[0] = MIDI_NOTE_ON | ucChannel;
	m_oMessage3[1] = ucNote;
	m_oMessage3[2] = ucVelocity;
	m_poMidiOut->sendMessage( &m_oMessage3 );
}

inline void CMidiOut::NoteOff( unsigned char ucChannel, unsigned char ucNote, unsigned char ucVelocity )
{
	m_oMessage3[0] = MIDI_NOTE_OFF | ucChannel;
	m_oMessage3[1] = ucNote;
	m_oMessage3[2] = ucVelocity;
	m_poMidiOut->sendMessage( &m_oMessage3 );
}

inline void CMidiOut::PolyAftertouch( unsigned char ucChannel, unsigned char ucNote, unsigned ucValue )
{
	m_oMessage3[0] = MIDI_POLY_AFTERTOUCH | ucChannel;
	m_oMessage3[1] = ucNote;
	m_oMessage3[2] = ucValue;
	m_poMidiOut->sendMessage( &m_oMessage3 );
}

inline void CMidiOut::ControlChange( unsigned char ucChannel, unsigned char ucNum, unsigned ucValue )
{
	m_oMessage3[0] = MIDI_CONTROL_CHANGE | ucChannel;
	m_oMessage3[1] = ucNum;
	m_oMessage3[2] = ucValue;
	m_poMidiOut->sendMessage( &m_oMessage3 );
}

inline void CMidiOut::ProgramChange( unsigned char ucChannel, unsigned char ucNum )
{
	// totest
	m_oMessage2[0] = MIDI_PROGRAM_CHANGE | ucChannel;
	m_oMessage2[1] = ucNum;
	m_poMidiOut->sendMessage( &m_oMessage2 );
}

inline void CMidiOut::ChannelAftertouch( unsigned char ucChannel, unsigned char ucValue )
{
	m_oMessage2[0] = MIDI_CHANNEL_AFTERTOUCH | ucChannel;
	m_oMessage2[1] = ucValue;
	m_poMidiOut->sendMessage( &m_oMessage2 );
}

inline void CMidiOut::Pitchbend( unsigned char ucChannel, unsigned char ucLSB, unsigned char ucMSB )
{
	m_oMessage3[0] = MIDI_PITCHBEND | ucChannel;
	m_oMessage3[1] = ucLSB;
	m_oMessage3[2] = ucMSB;
	m_poMidiOut->sendMessage( &m_oMessage3 );
}

inline void CMidiOut::Pitchbend( unsigned char ucChannel, int iValue )
{
	m_oMessage3[0] = MIDI_PITCHBEND | ucChannel;
	m_oMessage3[1] = BYTE0( iValue );
	m_oMessage3[2] = BYTE1( iValue );
	m_poMidiOut->sendMessage( &m_oMessage3 );
}

#endif // platform
