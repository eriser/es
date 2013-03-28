// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Midi.cpp

#include "Midi.h"

////////////////////////////////////////////////////////////////////////////////
// CMidi
////////////////////////////////////////////////////////////////////////////////

CMidi::CMidi()
: m_iDeviceID( -1 )
{}

CMidi::CMidi( const CMidi &roO )
{
	*this = roO;
}

CMidi::~CMidi()
{}

CMidi & CMidi::operator=( const CMidi &roO )
{
	m_iDeviceID = roO.m_iDeviceID;
	return *this;
}


////////////////////////////////////////////////////////////////////////////////
// CMidiIn
////////////////////////////////////////////////////////////////////////////////

CMidiIn::CMidiIn()
: CMidi()
{
	Reset();
}

CMidiIn::CMidiIn( const CMidiIn &roO )
: CMidi()
{
	*this = roO;
}

CMidiIn::~CMidiIn()
{
#ifdef MIDI_USE_INPUT_HANDLER
	m_oListHandler.MoveToFront();
	while( m_oListHandler.Next() )
	{
		CHandler *poHandler = m_oListHandler.Get();
		if( poHandler )
			poHandler->OnDestroy();
	}
#endif // MIDI_USE_INPUT_HANDLER
}

CMidiIn & CMidiIn::operator=( const CMidiIn &roO )
{
	CMidi::operator =( roO );

	memcpy( m_aabNoteOn, roO.m_aabNoteOn, sizeof(m_aabNoteOn) );
	memcpy( m_aaucNoteVelocity, roO.m_aaucNoteVelocity, sizeof(m_aaucNoteVelocity) );
	memcpy( m_aaucPolyAftertouch, roO.m_aaucPolyAftertouch, sizeof(m_aaucPolyAftertouch) );
	memcpy( m_aaucControl, roO.m_aaucControl, sizeof(m_aaucControl) );
	memcpy( m_aucProgram, roO.m_aucProgram, sizeof(m_aucProgram) );
	memcpy( m_aucChannelAftertouch, roO.m_aucChannelAftertouch, sizeof(m_aucChannelAftertouch) );
	memcpy( m_auiPitchbend, roO.m_auiPitchbend, sizeof(m_auiPitchbend) );

#ifdef MIDI_USE_INPUT_HANDLER
	m_oListHandler = roO.m_oListHandler;
#endif // MIDI_USE_INPUT_HANDLER

	m_iDeviceID = roO.m_iDeviceID; // ???

	//m_iDeviceID = -1;
	//if( roO.m_iDeviceID >= 0 )
	//	Init( roO.m_iDeviceID );

	return *this;
}

void CMidiIn::Reset()
{
	memset( m_aabNoteOn,		0, sizeof( m_aabNoteOn ) );
	memset( m_aaucNoteVelocity,	0, sizeof( m_aaucNoteVelocity ) );
	memset( m_aaucPolyAftertouch,	0, sizeof( m_aaucPolyAftertouch ) );
	memset( m_aaucControl,		0, sizeof( m_aaucControl ) );
	memset( m_aucProgram,		0, sizeof( m_aucProgram ) );
	memset( m_aucChannelAftertouch,	0, sizeof( m_aucChannelAftertouch ) );
	memset( m_auiPitchbend,		0, sizeof( m_auiPitchbend ) );
}

#ifdef MIDI_USE_INPUT_HANDLER
void CMidiIn::AddHandler( CHandler *poH )
{
	if( poH )
	{
		m_oListHandler.MoveToFront();
		while( m_oListHandler.Next() )
		{
			if( m_oListHandler.Get() == poH )
				return;
		}
		m_oListHandler.Append( poH );
	}
}

void CMidiIn::RemoveHandler( CHandler *poH )
{
	if( poH )
		m_oListHandler.RemoveEach( poH );
}
#endif // MIDI_USE_INPUT_HANDLER

void CMidiIn::Handle( unsigned char ucStatus, unsigned char ucData0, unsigned char ucData1 )
{
#ifdef MIDI_USE_INPUT_HANDLER
	const bool bUseHandler = ( m_oListHandler.GetSize() != 0 );
#endif // MIDI_USE_INPUT_HANDLER

	const unsigned char ucStatusLow = (unsigned char)( NIBBLE_LOWER(ucStatus) );
	//const unsigned char ucStatusHigh = (unsigned char)( NIBBLE_UPPER(ucStatus) );

	MIDI_LOG1( "%x\n", dwParam1 );
	MIDI_LOG2( "\tstatus: %d, %d (channel)\n", int(ucStatusHigh), int(ucStatusLow) );
	MIDI_LOG2( "\tdata0: %d,\tdata1: %d\n", int(ucData0), int(ucData1) );

	switch( ucStatus & MIDI_MASK_STATUS )
	{

	case MIDI_NOTE_ON:
		if( ucData1 == 0 )
			goto LNoteOff;

		MIDI_LOG( "\t\tNote On\n" );
		MIDI_LOG1( "\t\t\tChannel:\t%d", int(ucStatusLow) );
		MIDI_LOG1( "\t\t\tKeyNumber:\t%d", int(ucData0) );
		MIDI_LOG1( "\t\t\tVelocity:\t%d", int(ucData1) );
		//MIDI_LOG1( "\tfrequency=\t\t%g\n", MIDI_NOTE_TO_FREQ(440.0, ucData0) );

		m_aaucNoteVelocity[ ucStatusLow ][ ucData0 ] = ucData1;

		// Wenn ucData1 (entspricht Velocity) == 0, ist die Message MIDI_NOTE_OFF.
		if( ucData1 )
		{
			m_aabNoteOn[ ucStatusLow ][ ucData0 ] = true;
#ifdef MIDI_USE_INPUT_HANDLER
			if( bUseHandler )
			{
				m_oListHandler.MoveToFront();
				while( m_oListHandler.Next() )
					m_oListHandler.Get()->OnNote( true, ucStatusLow, ucData0, ucData1 );
			}
#endif // MIDI_USE_INPUT_HANDLER
		}
		else
		{
			m_aabNoteOn[ ucStatusLow ][ ucData0 ] = false;
		}
	break;

	case MIDI_NOTE_OFF:
LNoteOff:
		MIDI_LOG( "\t\tNote Off\n" );
		MIDI_LOG1( "\t\t\tChannel:\t%d\n", int(ucStatusLow) );
		MIDI_LOG1( "\t\t\tKeyNumber:\t%d\n", int(ucData0) );
		MIDI_LOG1( "\t\t\tVelocity:\t%d\n", int(ucData1) );

		m_aabNoteOn[ ucStatusLow ][ ucData0 ] = false;
		m_aaucNoteVelocity[ ucStatusLow ][ ucData0 ] = ucData1;

#ifdef MIDI_USE_INPUT_HANDLER
		if( bUseHandler )
		{
			m_oListHandler.MoveToFront();
			while( m_oListHandler.Next() )
				m_oListHandler.Get()->OnNote( false, ucStatusLow, ucData0, ucData1 );
		}
#endif // MIDI_USE_INPUT_HANDLER
	break;

	case MIDI_POLY_AFTERTOUCH:
		MIDI_LOG( "\t\tPolyhon Aftertouch" );

		MIDI_LOG1( "\t\tChannel:\t%d\n", int(ucStatusLow) );
		MIDI_LOG1( "\t\t\tKeyNumber:\t%d", int(ucData0) );
		MIDI_LOG1( "\t\t\tVelocity:\t%d", int(ucData1) );

		m_aaucPolyAftertouch[ ucStatusLow ][ ucData0 ] = ucData1;
#ifdef MIDI_USE_INPUT_HANDLER
		if( bUseHandler )
		{
			m_oListHandler.MoveToFront();
			while( m_oListHandler.Next() )
				m_oListHandler.Get()->OnPolyAftertouch( ucStatusLow, ucData0, ucData1 );
		}
#endif // MIDI_USE_INPUT_HANDLER
	break;

	case MIDI_CONTROL_CHANGE:
		MIDI_LOG( "\t\tControl Change\n" );

		MIDI_LOG1( "\t\t\tChannel:\t%d\n", int(ucStatusLow) );
		MIDI_LOG1( "\t\t\tControl Number:\t%d\n", int(ucData0) );
		MIDI_LOG1( "\t\t\tControl Value:\t%d\n", int(ucData1) );

		m_aaucControl[ ucStatusLow ][ ucData0 ] = ucData1;
#ifdef MIDI_USE_INPUT_HANDLER
		if( bUseHandler )
		{
			m_oListHandler.MoveToFront();
			while( m_oListHandler.Next() )
				m_oListHandler.Get()->OnControlChange( ucStatusLow, ucData0, ucData1 );
		}
#endif // MIDI_USE_INPUT_HANDLER
	break;

	case MIDI_PROGRAM_CHANGE:
		MIDI_LOG( "\t\tProgram Change\n" );

		MIDI_LOG1( "\t\t\tChannel:\t%d", int(ucStatusLow) );
		MIDI_LOG1( "\t\t\tProgram Number:\t%d", int(ucData0) );

		m_aucProgram[ ucStatusLow ] = ucData0;
#ifdef MIDI_USE_INPUT_HANDLER
		if( bUseHandler )
		{
			m_oListHandler.MoveToFront();
			while( m_oListHandler.Next() )
				m_oListHandler.Get()->OnProgramChange( ucStatusLow, ucData0 );
		}
#endif // MIDI_USE_INPUT_HANDLER
	break;

	case MIDI_CHANNEL_AFTERTOUCH:
		MIDI_LOG( "\t\tChannel Aftertouch\n" );

		m_aucChannelAftertouch[ ucStatusLow ] = ucData0;
#ifdef MIDI_USE_INPUT_HANDLER
		if( bUseHandler )
		{
			m_oListHandler.MoveToFront();
			while( m_oListHandler.Next() )
				m_oListHandler.Get()->OnChannelAftertouch( ucStatusLow, ucData0 );
		}
#endif // MIDI_USE_INPUT_HANDLER
	break;

	case MIDI_PITCHBEND:
	{
		MIDI_LOG( "\t\tPitchbend\n" );

		const unsigned int uiPitchbend = (unsigned int)ucData0 | (unsigned int)( ucData1 << 8 );
		m_auiPitchbend[ ucStatusLow ] = uiPitchbend;
#ifdef MIDI_USE_INPUT_HANDLER
		if( bUseHandler )
		{
			m_oListHandler.MoveToFront();
			while( m_oListHandler.Next() )
				m_oListHandler.Get()->OnPitchbend( ucStatusLow, uiPitchbend );
		}
#endif // MIDI_USE_INPUT_HANDLER
	}
	break;
	}
}


#ifdef __WINDOWS__

////////////////////////////////////////////////////////////////////////////////
// CMidiInDev
////////////////////////////////////////////////////////////////////////////////

CMidiInDev::CMidiInDev()
: CMidiIn()
, m_hMidiInput( 0 )
{
	Reset();
}

CMidiInDev::CMidiInDev( const CMidiInDev &roO )
: CMidiIn()
{
	*this = roO;
}

CMidiInDev::~CMidiInDev()
{
	Clear();
}

CMidiInDev & CMidiInDev::operator=( const CMidiInDev &roO )
{
	Clear();
	CMidiIn::operator =( roO );

#ifdef MIDI_USE_INPUT_HANDLER
	m_oListHandler = roO.m_oListHandler;
#endif // MIDI_USE_INPUT_HANDLER

	m_iDeviceID = -1;
	if( roO.m_iDeviceID >= 0 )
		Init( roO.m_iDeviceID );

	return *this;
}

bool CMidiInDev::Init( unsigned int uiDeviceID )
{
	if( m_iDeviceID == int( uiDeviceID ) )
		MIDI_ERROR_REPORT( "Geraet bereits initialisiert." );

	if( m_iDeviceID >= 0 )
		Clear();

	if( uiDeviceID >= midiInGetNumDevs() )
		MIDI_ERROR_REPORT( "Ungueltiger Geraet-Index." );

	if( midiInOpen( &m_hMidiInput,
			uiDeviceID,
			reinterpret_cast<DWORD_PTR>(CMidiInDev::Proc_),
			reinterpret_cast<DWORD_PTR>(this),
			CALLBACK_FUNCTION ) != MMSYSERR_NOERROR
	   || !m_hMidiInput )
		MIDI_ERROR_REPORT( "Oeffnen des Geraetes." );

	if( midiInReset( m_hMidiInput ) != MMSYSERR_NOERROR )
		MIDI_ERROR_REPORT( "Reset." );

	if( midiInStart( m_hMidiInput ) != MMSYSERR_NOERROR )
		MIDI_ERROR_REPORT( "MIDI Geraet starten." );

	Sleep(100);

	if( midiInStart( m_hMidiInput ) != MMSYSERR_NOERROR )
		MIDI_ERROR_REPORT( "MIDI Geraet starten." );

	m_iDeviceID = int( uiDeviceID );

	return true;
}

bool CMidiInDev::Clear()
{
	if( m_iDeviceID < 0 || !m_hMidiInput )
		MIDI_ERROR_REPORT( "Gar noch nicht initialisiert." );

	if( midiInStop( m_hMidiInput ) != MMSYSERR_NOERROR )
		MIDI_ERROR_REPORT( "MIDI Geraet stoppen." );

	if( midiInReset( m_hMidiInput ) != MMSYSERR_NOERROR )
		MIDI_ERROR_REPORT( "Reset." );

	if( m_hMidiInput )
		midiInClose( m_hMidiInput );
	m_hMidiInput = 0;

	m_iDeviceID = -1;

	return true;
}

void CMidiInDev::Reset()
{
	CMidiIn::Reset();
}

#ifdef MIDI_USE_INPUT_HANDLER
void CMidiInDev::AddHandler( CHandler *poH )
{
	CMidiIn::AddHandler( poH );
}

void CMidiInDev::RemoveHandler( CHandler *poH )
{
	if( m_hMidiInput && poH )
	{
		//const int iID = m_iDeviceID;
		//Clear();
		midiInStop( m_hMidiInput );
		midiInReset( m_hMidiInput );
		CMidiIn::RemoveHandler( poH );
		midiInStart( m_hMidiInput );
		//if( iID >= 0 )
		//	Init( iID );
	}
}
#endif // MIDI_USE_INPUT_HANDLER


#ifdef MIDI_USE_INFO_FUNC
CStr CMidiInDev::GetInfoDevice_()
{
	MIDIINCAPS MIDICapsIn;
	unsigned int i;
	CStr s;
	char c = '0';

	for(i = 0; i < midiInGetNumDevs(); i++)
	{
		if( !midiInGetDevCaps(i, &MIDICapsIn, sizeof(MIDIINCAPS)))
		{
			s += "Input Device ID #";
			s += c;
			s += CStr(": ");
			s += MIDICapsIn.szPname + CStr(MIDI_NEWLINE);
			//s += MIDICapsIn.vDriverVersion;
			c++;
		}
	}
	return s;
}
#endif //MIDI_USE_INFO_FUNC

unsigned int CMidiInDev::GetDeviceCount()
{
	return midiInGetNumDevs();
}

void CALLBACK CMidiInDev::Proc_( HMIDIIN hHandle, UINT_PTR uiMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD dwParam2 )
{
	//float fFreq;
	//TVFloat tVelo;
	//TVIntUS tVoiceNr;

	// rem: dwParam1 ist die Zeit.

	switch( uiMsg )
	{
	case MIM_OPEN:
		MIDI_LOG( "midi_msg: MIM_OPEN\n" );
	break;
	case MIM_DATA:
	{
		MIDI_LOG( "midi_msg: MIM_DATA\n" );

		CMidiInDev *poMidiIn = reinterpret_cast<CMidiInDev*>( dwInstance );
		if( poMidiIn )
		{
			const unsigned char ucStatus =	(unsigned char)( BYTE_LOWER(WORD_LOWER(dwParam1)) );
			const unsigned char ucData0 =	(unsigned char)( BYTE_UPPER(WORD_LOWER(dwParam1)) );
			const unsigned char ucData1 =	(unsigned char)( BYTE_LOWER(WORD_UPPER(dwParam1)) );

			poMidiIn->Handle( ucStatus, ucData0, ucData1 );
		}
	}
	break;
	case MIM_LONGDATA:
		MIDI_LOG( "midi_msg: MIM_LONGDATA\n" );
		// todo: System Exclusive.
	break;
	case MIM_MOREDATA:
		MIDI_LOG( "midi_msg: MIM_MOREDATA\n" );
		// todo: MIDI_IO_STATUS flag to midiInOpen() -> Fehlerbehebung.
	break;
	case MIM_ERROR:
		MIDI_LOG( "midi_msg: MIM_ERROR\n" );
		MIDI_LOG2( "\tdata: %d, %d", dwParam1, dwParam2 );
	break;
	case MIM_LONGERROR:
		MIDI_LOG( "midi_msg: MIM_LONGERROR\n" );
		// todo: SysEx Fehlerbehebung.
	break;
	case MIM_CLOSE:
		MIDI_LOG( "midi_msg: MIM_CLOSE\n" );
	break;
	};
}

#else // RtMidi

////////////////////////////////////////////////////////////////////////////////
// CMidiInDev
////////////////////////////////////////////////////////////////////////////////

CMidiInDev::CMidiInDev()
: CMidiIn()
, m_poMidiIn( 0 )
{
	Reset();
}

CMidiInDev::CMidiInDev( const CMidiInDev &roO )
: CMidiIn()
{
	*this = roO;
}

CMidiInDev::~CMidiInDev()
{
	Clear();
}

CMidiInDev & CMidiInDev::operator=( const CMidiInDev &roO )
{
	Clear();
	CMidiIn::operator =( roO );

#ifdef MIDI_USE_INPUT_HANDLER
	m_oListHandler = roO.m_oListHandler;
#endif // MIDI_USE_INPUT_HANDLER

	m_iDeviceID = -1;
	if( roO.m_iDeviceID >= 0 )
		Init( roO.m_iDeviceID );

	return *this;
}

bool CMidiInDev::Init( unsigned int uiDeviceID )
{
	if( m_iDeviceID == int( uiDeviceID ) )
		MIDI_ERROR_REPORT( "Geraet bereits initialisiert." );

	if( m_iDeviceID >= 0 )
		Clear();

	try {
		m_poMidiIn = new RtMidiIn();
	}
	catch ( RtError &roErr ) {
		roErr.printMessage();
		return false;
	}
	
	if( uiDeviceID >= m_poMidiIn->getPortCount() ) {
		delete m_poMidiIn;
		MIDI_ERROR_REPORT( "Ungueltiger Geraet-Index." );
	}

	try {
		m_poMidiIn->openPort( uiDeviceID );
	}
	catch ( RtError &roErr ) {
		roErr.printMessage();
		return false;
	}

	// Set our callback function.  This should be done immediately after
	// opening the port to avoid having incoming messages written to the
	// queue instead of sent to the callback function.
	m_poMidiIn->setCallback( &Proc_, this );

	// Don't ignore sysex, timing, or active sensing messages.
	m_poMidiIn->ignoreTypes( false, false, false );

	m_iDeviceID = int( uiDeviceID );
	return true;
}

bool CMidiInDev::Clear()
{
	if( m_iDeviceID < 0 || !m_poMidiIn )
		MIDI_ERROR_REPORT( "Gar noch nicht initialisiert." );

	if( m_poMidiIn )
	{
		delete m_poMidiIn;
		m_poMidiIn = 0;
	}
	m_iDeviceID = -1;
	return true;
}

void CMidiInDev::Reset()
{
	CMidiIn::Reset();
}

#ifdef MIDI_USE_INPUT_HANDLER
void CMidiInDev::AddHandler( CHandler *poH )
{
	CMidiIn::AddHandler( poH );
}

void CMidiInDev::RemoveHandler( CHandler *poH )
{
	if( m_poMidiIn && poH )
	{
		//const int iID = m_iDeviceID;
		//Clear();

		// ACHTUNG: Unsicher!?
		//midiInStop( m_hMidiInput );
		//midiInReset( m_hMidiInput );
		CMidiIn::RemoveHandler( poH );
		//midiInStart( m_hMidiInput );

		//if( iID >= 0 )
		//	Init( iID );
	}
}
#endif // MIDI_USE_INPUT_HANDLER

#ifdef MIDI_USE_INFO_FUNC
CStr CMidiInDev::GetInfoDevice_()
{
	CStr s;
	RtMidiIn *poMidiIn = 0;
	unsigned int uiPorts = 0;
	char c = '0';

	try {
		poMidiIn = new RtMidiIn();
	}
	catch ( RtError &roErr ) {
		roErr.printMessage();
		goto LCleanUp;
	}

	uiPorts = poMidiIn->getPortCount();
	for ( unsigned int i=0; i<uiPorts; i++ ) {
		try
		{
			s += "Input Device ID #";
			s += c;
			s += CStr(": ");
			s += poMidiIn->getPortName(i).data() + CStr(MIDI_NEWLINE);
			++c;
		}
		catch ( RtError &roErr ) {
			roErr.printMessage();
			goto LCleanUp;
		}
	}

LCleanUp:
	if( poMidiIn )
		delete poMidiIn;

	return s;
}
#endif //MIDI_USE_INFO_FUNC

unsigned int CMidiInDev::GetDeviceCount()
{
	RtMidiIn *poMidiIn = 0;
	unsigned int uiPorts = 0;
	try
	{
		poMidiIn = new RtMidiIn();
		uiPorts = poMidiIn->getPortCount();
	}
	catch ( RtError &roErr ) {
		roErr.printMessage();
	}
	if( poMidiIn )
		delete poMidiIn;
	return uiPorts;
}

void CMidiInDev::Proc_( double dDeltaTime, std::vector<unsigned char> *poMessage, void *pvUserData )
{
	CMidiInDev *poMidiIn = reinterpret_cast<CMidiInDev*>( pvUserData );
	unsigned int i = 0;
	const unsigned int uiByteCount = poMessage->size();
	MIDI_ASSERT( ( uiByteCount % 3 ) == 0 );

	while( i<uiByteCount )
	{
		const unsigned char ucStatus =	poMessage->at( i ); ++i;
		const unsigned char ucData0 =	poMessage->at( i ); ++i;
		const unsigned char ucData1 =	poMessage->at( i ); ++i;
		poMidiIn->Handle( ucStatus, ucData0, ucData1 );
	}
}

#endif // platform






#ifdef MIDI_USE_FILE

////////////////////////////////////////////////////////////////////////////////
// CMidiInFile
////////////////////////////////////////////////////////////////////////////////

CMidiInFile::CMidiInFile()
: CMidiIn()
, m_uiSampleFreq( 0 )
, m_dBPM( 120.0 )
, m_uliClock( 0L )
, m_uiSample( 0 )
, m_dSamplePart( 0.0 )
, m_dSamplesPerQuarter( 0.0 )
, m_uiSamplesPerClock( 0 )
, m_dSamplesPerClockFrac( 0.0 )
{
}

CMidiInFile::CMidiInFile( const CMidiInFile &roO )
: CMidiIn()
{
	*this = roO;
}

CMidiInFile::~CMidiInFile()
{
	Clear();
}

CMidiInFile & CMidiInFile::operator=( const CMidiInFile &roO )
{
	Clear();
	CMidiIn::operator=( roO );
	m_oFile = roO.m_oFile;
	m_uiSampleFreq = roO.m_uiSampleFreq;
	m_dBPM = roO.m_dBPM;
	m_uliClock = roO.m_uliClock;
	m_uiSample = roO.m_uiSample;
	m_dSamplePart = roO.m_dSamplePart;
	m_dSamplesPerQuarter = roO.m_dSamplesPerQuarter;
	m_uiSamplesPerClock = roO.m_uiSamplesPerClock;
	m_dSamplesPerClockFrac = roO.m_dSamplesPerClockFrac;
	return *this;
}

bool CMidiInFile::Init( const char *pcFileName )
{
	if( !m_oFile.Load( pcFileName ) )
		return false;
	Reset();
	m_dBPM = GetFileBPM();
	UpdateMetrics();
	return true;
}

bool CMidiInFile::Clear()
{
	m_oFile.Clear();
	m_uiSample = 0;
	m_dSamplePart = 0.0;
	m_uliClock = 0L;
	return true;
}

void CMidiInFile::Reset()
{
	m_uiSample = 0;
	m_dSamplePart = 0.0;
	m_uliClock = 0L;
	m_oFile.ResetAllTracks();
	CMidiIn::Reset();
}

void CMidiInFile::Proc()
{
	if( m_uiSample == 0 )
	{
		for( unsigned int i=0; i<m_oFile.GetTrackCount(); ++i )
		{
			bool bRep = false;
			do
			{
				CFileBlockMidi::CEvent *poEvent = 0;
				bRep = m_oFile.Clock( i, &poEvent );
				if( poEvent )
				{
					Handle( poEvent->m_ucStatus, poEvent->m_ucData0, poEvent->m_ucData1 );
				}
			}
			while( bRep );
		}

		m_uiSample = m_uiSamplesPerClock;
		m_dSamplePart += m_dSamplesPerClockFrac;
		if( m_dSamplePart >= 1.0 )
		{
			m_dSamplePart -= 1.0;
			++m_uiSample;
			//MIDI_LOG( "#" );
		}
		//MIDI_LOG( "." );
		++m_uliClock;
	}
	--m_uiSample;
}

void CMidiInFile::UpdateMetrics()
{
	if( m_oFile.m_usiDivision == 0 )
		return;
	double dBPM = m_dBPM;
	if( dBPM < 0.0 )
		dBPM = -dBPM;
	if( dBPM == 0.0 )
		dBPM = 120.0;
	//const double dDurQuarter = ( 60.0 / dBPM );
	//const double dDurClock = dDurQuarter / m_oFile.m_usiDivision;
	m_dSamplesPerQuarter = ( m_uiSampleFreq * 60.0 ) / dBPM;
	const double dSamplesPerClock = m_dSamplesPerQuarter / m_oFile.m_usiDivision;
	m_uiSamplesPerClock = (unsigned int)dSamplesPerClock;
	m_dSamplesPerClockFrac = dSamplesPerClock - m_uiSamplesPerClock;
}

unsigned int CMidiInFile::GetTrackCount() const
{
	return m_oFile.GetTrackCount();
}

unsigned int CMidiInFile::GetDivision() const
{
	return m_oFile.m_usiDivision;
}

unsigned long int CMidiInFile::GetClockMax() const
{
	return m_oFile.GetTrackDurationMax();
}

double CMidiInFile::GetFileBPM() const
{
	if( m_oFile.m_uiMicroSecPerQuarter )
	{
		return 60.0e+6 / m_oFile.m_uiMicroSecPerQuarter;
	}
	return 0.0;
}

#endif // MIDI_USE_FILE


#ifdef __WINDOWS__

////////////////////////////////////////////////////////////////////////////////
// CMidiOut
////////////////////////////////////////////////////////////////////////////////

CMidiOut::CMidiOut()
: CMidi()
, m_hMidiOutput( 0 )
{}

CMidiOut::CMidiOut( const CMidiOut &roO )
: CMidi()
{
	*this = roO;
}

CMidiOut::~CMidiOut()
{
	Clear();
}

CMidiOut & CMidiOut::operator=( const CMidiOut &roO )
{
	Clear();
	CMidi::operator =( roO );

	m_iDeviceID = -1;
	if( m_iDeviceID >= 0 )
		Init( roO.m_iDeviceID );

	return *this;
}

bool CMidiOut::Init( unsigned int uiDeviceID )
{
	if( m_iDeviceID == int( uiDeviceID ) )
		MIDI_ERROR_REPORT( "Geraet bereits initialisiert." );

	if( m_iDeviceID >= 0 )
		Clear();

	if( uiDeviceID >= midiOutGetNumDevs() )
		MIDI_ERROR_REPORT( "Ungueltiger Geraet-Index." );

	if( midiOutOpen(
			&m_hMidiOutput,
			uiDeviceID,
			0,
			0,
			CALLBACK_WINDOW ) != MMSYSERR_NOERROR
		|| !m_hMidiOutput )
		MIDI_ERROR_REPORT( "Oeffnen des Geraetes." );

	if( midiOutReset( m_hMidiOutput ) != MMSYSERR_NOERROR )
		MIDI_ERROR_REPORT( "Reset." );

	m_iDeviceID = int( uiDeviceID );

	return true;
}

bool CMidiOut::Clear()
{
	if( m_iDeviceID < 0 || !m_hMidiOutput )
		MIDI_ERROR_REPORT( "Gar noch nicht initialisiert." );

	if( m_hMidiOutput )
		midiOutClose( m_hMidiOutput );
	m_hMidiOutput = 0;

	m_iDeviceID = -1;

	return true;
}

#ifdef MIDI_USE_INFO_FUNC
CStr CMidiOut::GetInfoDevice_()
{
	MIDIOUTCAPS MIDICapsOut;
	unsigned int i;
	CStr s;
	char c = '0';

	for(i = 0; i < midiOutGetNumDevs(); i++)
	{
		if( !midiOutGetDevCaps(i, &MIDICapsOut, sizeof(MIDIOUTCAPS)))
		{
			s += "Output Device ID #";
			s += c;
			s += CStr(": ");
			s += MIDICapsOut.szPname + CStr(MIDI_NEWLINE);
			//s += MIDICapsOut.vDriverVersion;
			c++;
		}
	}
	return s;
}
#endif //MIDI_USE_INFO_FUNC

unsigned int CMidiOut::GetDeviceCount()
{
	return midiOutGetNumDevs();
}


#else // RtMidi

////////////////////////////////////////////////////////////////////////////////
// CMidiOut
////////////////////////////////////////////////////////////////////////////////

CMidiOut::CMidiOut()
: CMidi()
, m_poMidiOut( 0 )
{
	m_oMessage2.resize( 2 );
	m_oMessage3.resize( 3 );
}

CMidiOut::CMidiOut( const CMidiOut &roO )
: CMidi()
{
	*this = roO;
}

CMidiOut::~CMidiOut()
{
	Clear();
}

CMidiOut & CMidiOut::operator=( const CMidiOut &roO )
{
	Clear();
	CMidi::operator =( roO );

	m_iDeviceID = -1;
	if( m_iDeviceID >= 0 )
		Init( roO.m_iDeviceID );

	return *this;
}

bool CMidiOut::Init( unsigned int uiDeviceID )
{
	if( m_iDeviceID == int( uiDeviceID ) )
		MIDI_ERROR_REPORT( "Geraet bereits initialisiert." );

	if( m_iDeviceID >= 0 )
		Clear();
	try {
		m_poMidiOut = new RtMidiOut();
	}
	catch ( RtError &roErr )
	{
		roErr.printMessage();
		return false;
	}

	if( uiDeviceID >= m_poMidiOut->getPortCount() )
		MIDI_ERROR_REPORT( "Ungueltiger Geraet-Index." );
	try
	{
		m_poMidiOut->openPort( uiDeviceID );
		//m_poMidiOut->openVirtualPort(); // ???
	}
	catch ( RtError &roErr )
	{
		roErr.printMessage();
		Clear();
		return false;
	}

	m_iDeviceID = int( uiDeviceID );

	return true;
}

bool CMidiOut::Clear()
{
	if( m_iDeviceID < 0 || !m_poMidiOut )
	{
		MIDI_ERROR_REPORT( "Gar noch nicht initialisiert." );
		return false;
	}

	if( m_poMidiOut )
	{
		delete m_poMidiOut;
		m_poMidiOut = 0;
	}

	m_iDeviceID = -1;
	return true;
}

#ifdef MIDI_USE_INFO_FUNC
CStr CMidiOut::GetInfoDevice_()
{
	CStr s;
	RtMidiOut *poMidiOut = 0;
	unsigned int uiPorts = 0;
	char c = '0';

	try {
		poMidiOut = new RtMidiOut();
	}
	catch ( RtError &roErr )
	{
		roErr.printMessage();
		goto LCleanUp;
	}

	uiPorts = poMidiOut->getPortCount();
	for ( unsigned int i=0; i<uiPorts; i++ ) {
		try
		{
			s += "Output Device ID #";
			s += c;
			s += CStr(": ");
			s += poMidiOut->getPortName(i).data() + CStr(MIDI_NEWLINE);
			++c;
		}
		catch ( RtError &roErr )
		{
			roErr.printMessage();
			goto LCleanUp;
		}
	}

LCleanUp:
	if( poMidiOut )
		delete poMidiOut;

	return s;
}
#endif //MIDI_USE_INFO_FUNC

unsigned int CMidiOut::GetDeviceCount()
{
	RtMidiOut *poMidiOut = 0;
	unsigned int uiPorts = 0;
	try
	{
		poMidiOut = new RtMidiOut();
		uiPorts = poMidiOut->getPortCount();
	}
	catch ( RtError &roErr ) {
		roErr.printMessage();
	}
	if( poMidiOut )
		delete poMidiOut;
	return uiPorts;
}

#endif // platform
