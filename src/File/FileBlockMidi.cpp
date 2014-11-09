// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FileBlockMidi.h

#ifndef _FILE_BLOCK_MIDI_CPP_
#define _FILE_BLOCK_MIDI_CPP_

#include "FileBlockMidi.h"

#ifdef _DEBUG
#define FILE_BLOCK_MIDI_USE_LOG
#endif // _DEBUG

#ifdef FILE_BLOCK_MIDI_USE_LOG
#include <stdio.h>
#define FBM_LOG( ... ) printf( __VA_ARGS__ )
#else
#define FBM_LOG( ... ) { ; }
#endif // FILE_BLOCK_MIDI_USE_LOG


#define WORD_UPPER(lowdw)		((lowdw  & 0xFFFF0000) >>16 )
#define WORD_LOWER(highdw)		((highdw & 0x0000FFFF)      )

#define BYTE_UPPER(lowword)		((lowword  & 0xFF00) >>8 )
#define BYTE_LOWER(highword)		((highword & 0x00FF)     )

#define NIBBLE_UPPER(byte)		((byte & 0xF0) >>4 )
#define NIBBLE_LOWER(byte)		((byte & 0x0F)     )

#define MIDI_NOTE_TO_FREQ(A,notenr)	(A / 32.0) * pow(2.0, ((notenr - 9.0) / 12.0))

#define MIDI_NOTE_OFF			0x80
#define MIDI_NOTE_ON			0x90
#define MIDI_POLY_AFTERTOUCH	0xA0
#define MIDI_CONTROL_CHANGE		0xB0
#define MIDI_PROGRAM_CHANGE		0xC0
#define MIDI_CHANNEL_AFTERTOUCH	0xD0
#define MIDI_PITCHBEND			0xE0
#define MIDI_SYSEX				0xF0

#define MIDI_SONGPOS			0xF2
#define MIDI_SONGSELECT			0xF3
#define MIDI_TUNERREQUEST		0xF6
#define MIDI_ENDSYSEX			0xF7
#define MIDI_CLOCK				0xF8
#define MIDI_START				0xFA
#define MIDI_CONTINUE			0xFB
#define MIDI_STOP				0xFC
#define MIDI_ACTIVESENSE		0xFE
#define MIDI_META				0xFF

#define MIDI_MASK_STATUS		0xF0

#define MIDI_COUNT_CHANNELS		0x10	// 0-15
#define MIDI_COUNT_CONTROL		0x80	// 0-127
#define MIDI_COUNT_NOTES		0x80	// 0-127


#define META_SEQNUM				0x00
#define META_TEXT				0x01
#define META_COPYRIGHT			0x02
#define META_TRACKNAME			0x03
#define META_INSTRUMENT			0x04
#define META_LYRIC				0x05
#define META_MARKER				0x06
#define META_CUEPOINT			0x07
#define META_PROGRAM			0x08
#define META_DEVICE				0x09
#define META_PREFIXCH			0x20
#define META_PREFIXPORT			0x21
#define META_ENDOFTRACK			0x2F
#define META_TEMPO				0x51
#define META_SMPTE				0x54
#define META_TIMESIG			0x58
#define META_KEYSIG				0x59
#define META_PROPRIET			0x7F

struct SChunkMThd
{
	// Here's the 8 byte header that all chunks must have
	BYTE m_tID[4];  // This will be 'M','T','h','d'
	DWORD m_tSize; // This will be 6
	//char m_tID[4];
	//unsigned long int m_uliSize;
};

struct SChungMThd_Data
{
	// Here are the 6 bytes
	WORD m_tFormat;
	WORD m_tTrackCount;
	WORD m_tDivision;
	//unsigned short int m_usiFormat;
	//unsigned short int m_usiTrackCount;
	//unsigned short int m_usiDivision;
};

struct SChunkMTrk
{
	// Here's the 8 byte header that all chunks must have
	BYTE m_tID[4]; // This will be 'M','T','r','k'
	DWORD m_tSize; // This will be the actual size of Data[]
	//char m_acID[4];
	//unsigned long int m_uliSize;
	
	// Here are the data bytes
	//unsigned char m_aucData[]; // Its actual size is Data[Length]
};

////////////////////////////////////////////////////////////////////////////////
// CFileBlockMidi::CEvent
////////////////////////////////////////////////////////////////////////////////

CFileBlockMidi::CEvent::CEvent()
: m_uliDeltaTime( 0L )
, m_ucStatus( 0 )
, m_ucData0( 0 )
, m_ucData1( 0 )
{}

CFileBlockMidi::CEvent::CEvent( unsigned long int uliDeltaTime, unsigned char ucStatus, unsigned char ucData0, unsigned ucData1 )
: m_uliDeltaTime( uliDeltaTime )
, m_ucStatus( ucStatus )
, m_ucData0( ucData0 )
, m_ucData1( ucData1 )
{}

CFileBlockMidi::CEvent::~CEvent()
{}

////////////////////////////////////////////////////////////////////////////////
// CFileBlockMidi::CTrack
////////////////////////////////////////////////////////////////////////////////

CFileBlockMidi::CTrack::CTrack()
: m_poEvent( 0 )
, m_uliCounter( 0L )
, m_uliDuration( 0L )
{}

CFileBlockMidi::CTrack::CTrack( const CFileBlockMidi::CTrack &roO )
{
	*this = roO;
}

CFileBlockMidi::CTrack::~CTrack()
{
	Clear();
}

CFileBlockMidi::CTrack & CFileBlockMidi::CTrack::operator=( const CFileBlockMidi::CTrack &roO )
{
	Clear();
	m_oLstEvent = roO.m_oLstEvent; // Pointer-Position wird auch kopiert.
	for( unsigned int i=0; i<roO.m_oLstEvent.GetSize(); ++i )
	{
		CEvent *poEvent = roO.m_oLstEvent[i];
		m_oLstEvent[i] = new CEvent( *poEvent );
		if( poEvent == roO.m_poEvent )
			m_poEvent = m_oLstEvent[i];
	}
	m_uliCounter = roO.m_uliCounter;
	m_uliDuration = roO.m_uliDuration;
	return *this;
}

void CFileBlockMidi::CTrack::Reset()
{
	m_oLstEvent.MoveToFront();
	m_poEvent = 0; //m_oLstEvent[0];
	m_uliCounter = 0L; //m_poEvent->m_uliDeltaTime; // ACHTUNG: Anfang synchronisieren.
}

bool CFileBlockMidi::CTrack::Proc( CFileBlockMidi::CEvent **ppoEvent )
{
	if( !m_oLstEvent.Next() )
	{
		m_oLstEvent.MoveToFront();
		m_oLstEvent.Next();
		
		//printf( "%d\n", c ); // tmp
		//c = 0;

		*ppoEvent = 0;
		//m_poEvent = m_oLstEvent.Get();
		//return false; // Letzter Event hat sicher keinen folgenden Event zur gleichen Zeit (delta_t == 0).
		//return true; // Das Ende ist gleichzeitig der Beginn!
	}
	else
	{
		*ppoEvent = m_poEvent;
	}
	
	m_poEvent = m_oLstEvent.Get();
	
	if( m_poEvent )
	{
		m_uliCounter = m_poEvent->m_uliDeltaTime;
		return ( m_uliCounter == 0L );
	}
	return false;
}

void CFileBlockMidi::CTrack::Clear()
{
	m_oLstEvent.MoveToFront();
	while( m_oLstEvent.Next() )
	{
		CEvent *poEvent = m_oLstEvent.Get();
		if( poEvent )
			delete poEvent;
	}
	m_oLstEvent.Clear();
	m_poEvent = 0;
	m_uliCounter = 0L;
	m_uliDuration = 0L;
}


////////////////////////////////////////////////////////////////////////////////
// CFileBlockMidi
////////////////////////////////////////////////////////////////////////////////

CFileBlockMidi::CFileBlockMidi()
: CFileBlock()
, m_uiFormat( 0 )
, m_uiDivision( 0 )
, m_uiMicroSecPerQuarter( 0 )
{}

CFileBlockMidi::CFileBlockMidi( const CFileBlockMidi &roO )
: CFileBlock()
{
	*this = roO;
}

CFileBlockMidi::~CFileBlockMidi()
{}

CFileBlockMidi & CFileBlockMidi::operator=( const CFileBlockMidi &roO )
{
	Clear();
	CFileBlock::operator=( roO );
	m_oArrTrack.Resize( roO.m_oArrTrack.GetSize() );
	for( unsigned int i=0; i<m_oArrTrack.GetSize(); ++i )
		m_oArrTrack[i] = new CTrack( *roO.m_oArrTrack[i] );
	m_uiFormat = roO.m_uiFormat;
	m_uiDivision = roO.m_uiDivision;
	m_uiMicroSecPerQuarter = roO.m_uiMicroSecPerQuarter;
	return *this;
}

bool CFileBlockMidi::Load( const char *pcFileName )
{
	Clear();
	
	if( !CFileBlock::Load( pcFileName ) || !m_pucData )
		return false;
	
	unsigned char *pucData = m_pucData;
	
	// MThd-Header.
	SChunkMThd *poHD = (SChunkMThd *)pucData;
	// <mod date="2010-12-09">
	if( m_bIsLittleEndian_ )
	// </mod>
		SwapBytes_( &poHD->m_tSize );
	if( !strcmp( (const char *)&poHD->m_tID[0], "MThd" ) 
	 || poHD->m_tSize != 6 )
		return false;
	pucData += sizeof( SChunkMThd );
	
	// MThd-Header Daten.
	SChungMThd_Data *poHDD = (SChungMThd_Data *)pucData;
	// <mod date="2010-12-09">
	if( m_bIsLittleEndian_ )
	// </mod>
	{
		SwapBytes_( &poHDD->m_tFormat );
		SwapBytes_( &poHDD->m_tTrackCount );
		SwapBytes_( &poHDD->m_tDivision );
	}
	if( poHDD->m_tFormat > 1 || poHDD->m_tTrackCount == 0 )
		return false;
	if( poHDD->m_tFormat == 0 && poHDD->m_tTrackCount > 1 )
		return false;
	
	// rem: Division
	// The last two bytes indicate how many Pulses (i.e. clocks) 
	// Per Quarter Note (abbreviated as PPQN) resolution the time-stamps are based upon, 
	// Division. 
	// For example, if your sequencer has 96 ppqn, this field would be (in hex):	00 60 
	m_uiFormat = poHDD->m_tFormat;
	m_uiDivision = poHDD->m_tDivision;
	
	pucData += sizeof( SChungMThd_Data );
	
	FBM_LOG( "\nMidi File\n\n" );
	FBM_LOG( "\ttype:\t%d\n", m_uiFormat );
	FBM_LOG( "\tdivision:\t%d\n", m_uiDivision );
	FBM_LOG( "\ttrack count:\t%d\n", poHDD->m_tTrackCount );
	FBM_LOG( "\n" );
	
	// MTrk-Header und Daten.
	for( unsigned short int t = 0; t < poHDD->m_tTrackCount; ++t )
	{
		if( (unsigned int)( pucData - m_pucData ) >= m_uiSize )
		{
			FBM_LOG( "empty track found\n" );
			break;
		}
		
		SChunkMTrk *poRK = (SChunkMTrk *)pucData;
		// <mod date="2010-12-09">
		if( m_bIsLittleEndian_ )
		// </mod>
		{
			SwapBytes_( &poRK->m_tSize );
		}
		if( !strcmp( (const char *)&poRK->m_tID[0], "MTrk" ) 
		 || poRK->m_tSize == 0L )
			return false;
		FBM_LOG( "\n\n************\ntrack %d length %d\n\n", t, poRK->m_tSize );
		
		CTrack *poTrack = new CTrack;
		
		unsigned char *pucD = pucData + sizeof(SChunkMTrk);
		pucData = pucD + poRK->m_tSize;
		unsigned long int uliClocks = 0L;
		unsigned long int uliClocksAdd = 0L;
		bool bNotEndOfTrack = true;
		unsigned char ucStatusLast = 0;
		while( pucD < pucData && bNotEndOfTrack )
		{
			unsigned long int uliDeltaTime = 0L;
			const unsigned int uiBytes = VarLen4Read_( &uliDeltaTime, pucD );
			const unsigned long int uliT = uliDeltaTime + uliClocksAdd;
			pucD += uiBytes;
			unsigned char ucStatus = *pucD;
			++pucD;
			
			const unsigned int uiEventCountPrev = poTrack->m_oLstEvent.GetSize();
			
			//const unsigned char ucStatusLow = (unsigned char)( NIBBLE_LOWER(ucStatus) );
			//const unsigned char ucStatusHigh = (unsigned char)( NIBBLE_UPPER(ucStatus) );
			//ucData0 =	(unsigned char)( BYTE_UPPER(WORD_LOWER(dwParam1)) );
			//ucData1 =	(unsigned char)( BYTE_LOWER(WORD_UPPER(dwParam1)) );
			
			//FBM_LOG( "%x\n", dwParam1 );
			//FBM_LOG( "\tstatus: %d, %d (channel)\n", int(ucStatusHigh), int(ucStatusLow) );
			//FBM_LOG( "\tdata0: %d,\tdata1: %d\n", int(ucData0), int(ucData1) );
			
			// rem: Ist die Channel-Message undefiniert (<=0x70),
			// so wird die letzt g�ltige verwendet. Das aktuelle
			// Byte ist dann das erste Daten-Byte der Message.
			// Cubase speichert die Midi-Files mit dieser Methode.
			// Wenn gleichzeitig die Note-On-Messages mit Velocity
			// gleich 0 als Note-Off-Messages interpretiert werden,
			// kann so eine Menge Platz gespart werden.
			if( ( ucStatus & MIDI_MASK_STATUS ) <= 0x70 )
			{
				--pucD;
				ucStatus = ucStatusLast;
			}
			ucStatusLast = ucStatus;
			
			switch( ucStatus & MIDI_MASK_STATUS )
			{
			case MIDI_NOTE_ON:
			{
				const unsigned char ucData0 = *pucD; 
				++pucD;
				const unsigned char ucData1 = *pucD; 
				++pucD;
				if( ucData1 == 0 )
				{
					pucD -= 2;
					goto LNoteOff;
				}
				
				FBM_LOG( "\t\tNote On\n" );
				FBM_LOG( "\t\t\tChannel:\t%d\n", int(NIBBLE_LOWER(ucStatus)) );
				FBM_LOG( "\t\t\tKeyNumber:\t%d\n", int(ucData0) );
				FBM_LOG( "\t\t\tVelocity:\t%d\n", int(ucData1) );
				poTrack->m_oLstEvent.Append( new CEvent( uliT, ucStatus, ucData0, ucData1 ) );
			}
			break;
			case MIDI_NOTE_OFF:
			{
LNoteOff: 
				const unsigned char ucData0 = *pucD; 
				++pucD;
				const unsigned char ucData1 = *pucD; 
				++pucD;
				
				FBM_LOG( "\t\tNote Off\n" );
				FBM_LOG( "\t\t\tChannel:\t%d\n", int(NIBBLE_LOWER(ucStatus)) );
				FBM_LOG( "\t\t\tKeyNumber:\t%d\n", int(ucData0) );
				FBM_LOG( "\t\t\tVelocity:\t%d\n", int(ucData1) );
				poTrack->m_oLstEvent.Append( new CEvent( uliT, ucStatus, ucData0, ucData1 ) );
			}
			break;
			case MIDI_POLY_AFTERTOUCH:
			{
				const unsigned char ucData0 = *pucD; 
				++pucD;
				const unsigned char ucData1 = *pucD; 
				++pucD;
				
				FBM_LOG( "\t\tPolyphon Aftertouch\n" );
				FBM_LOG( "\t\tChannel:\t%d\n", int(NIBBLE_LOWER(ucStatus)) );
				FBM_LOG( "\t\t\tKeyNumber:\t%d\n", int(ucData0) );
				FBM_LOG( "\t\t\tVelocity:\t%d\n", int(ucData1) );
				poTrack->m_oLstEvent.Append( new CEvent( uliT, ucStatus, ucData0, ucData1 ) );
			}
			break;
			case MIDI_CONTROL_CHANGE:
			{
				const unsigned char ucData0 = *pucD; 
				++pucD;
				const unsigned char ucData1 = *pucD; 
				++pucD;
				
				FBM_LOG( "\t\tControl Change\n" );
				FBM_LOG( "\t\t\tChannel:\t%d\n", int(NIBBLE_LOWER(ucStatus)) );
				FBM_LOG( "\t\t\tControl Number:\t%d\n", int(ucData0) );
				FBM_LOG( "\t\t\tControl Value:\t%d\n", int(ucData1) );
				poTrack->m_oLstEvent.Append( new CEvent( uliT, ucStatus, ucData0, ucData1 ) );
			}
			break;
			case MIDI_PROGRAM_CHANGE:
			{
				const unsigned char ucData0 = *pucD; 
				++pucD;
				
				FBM_LOG( "\t\tProgram Change\n" );
				FBM_LOG( "\t\t\tChannel:\t%d\n", int(NIBBLE_LOWER(ucStatus)) );
				FBM_LOG( "\t\t\tProgram Number:\t%d\n", int(ucData0) );
				poTrack->m_oLstEvent.Append( new CEvent( uliT, ucStatus, ucData0 ) );
			}
			break;
			case MIDI_CHANNEL_AFTERTOUCH:
			{
				const unsigned char ucData0 = *pucD; 
				++pucD;
				const unsigned char ucData1 = *pucD; 
				++pucD;
				
				FBM_LOG( "\t\tChannel Aftertouch\n" );
				poTrack->m_oLstEvent.Append( new CEvent( uliT, ucStatus, ucData0, ucData1 ) );
			}
			break;
			case MIDI_PITCHBEND:
			{
				const unsigned char ucData0 = *pucD; 
				++pucD;
				const unsigned char ucData1 = *pucD; 
				++pucD;
				
				FBM_LOG( "\t\tPitchbend\n" );
				poTrack->m_oLstEvent.Append( new CEvent( uliT, ucStatus, ucData0, ucData1 ) );
			}
			break;
			default:
				switch( ucStatus )
				{
				case MIDI_SYSEX:
				{
					unsigned long int uliSize = 0L;
					const unsigned int uiBytes = VarLen4Read_( &uliSize, pucD );
					pucD += uiBytes;
					pucD += uliSize;
					FBM_LOG( "\t\tSysEx\n" );
				}
				break;
				case MIDI_SONGPOS:
				{
					FBM_LOG( "\t\tSong Pos\n" );
				}
				break;
				case MIDI_SONGSELECT:
				{
					FBM_LOG( "\t\tSong Select\n" );
				}
				break;
				case MIDI_TUNERREQUEST:
				{
					FBM_LOG( "\t\tTuner Select\n" );
				}
				break;
				case MIDI_ENDSYSEX:
				{
					FBM_LOG( "\t\tEnd SysEx\n" );
				}
				break;
				case MIDI_CLOCK:
				{
					FBM_LOG( "\t\tClock\n" );
				}
				break;
				case MIDI_START:
				{
					FBM_LOG( "\t\tStart\n" );
				}
				break;
				case MIDI_CONTINUE:
				{
					FBM_LOG( "\t\tContinue\n" );
				}
				break;
				case MIDI_STOP:
				{
					FBM_LOG( "\t\tStop\n" );
				}
				break;
				case MIDI_ACTIVESENSE:
				{
					FBM_LOG( "\t\tActive Sense\n" );
				}
				break;
				case MIDI_META:
				{
					FBM_LOG( "\t\tMeta\n" );
					
					const unsigned char ucID = *pucD;
					++pucD;
					
					bool bTextEvent = false;
					switch( ucID )
					{
					case META_SEQNUM:
					{
						// FF 00 02 ss ss
						// or...
						// FF 00 00
						FBM_LOG( "\t\t\tSequence Number\n" );
						const unsigned char ucData0 = *pucD;
						++pucD;
						if( ucData0 == 0x02 )
						{
							// todo
							pucD += 2;
						}
						// todo
					}
					break;
					case META_TEXT:
					{
						// FF 01 len text
						FBM_LOG( "\t\t\tText\n" );
						bTextEvent = true;
					}
					break;
					case META_COPYRIGHT:
					{
						// FF 02 len text
						FBM_LOG( "\t\t\tCopyright\n" );
						bTextEvent = true;
					}
					break;
					case META_TRACKNAME:
					{
						// FF 03 len text
						FBM_LOG( "\t\t\tTrackname\n" );
						bTextEvent = true;
					}
					break;
					case META_INSTRUMENT:
					{
						// FF 04 len text
						FBM_LOG( "\t\t\tInstrument\n" );
						bTextEvent = true;
					}
					break;
					case META_LYRIC:
					{
						// FF 05 len text
						FBM_LOG( "\t\t\tLyric\n" );
						bTextEvent = true;
					}
					break;
					case META_MARKER:
					{
						// FF 06 len text
						FBM_LOG( "\t\t\tMarker\n" );
						bTextEvent = true;
					}
					break;
					case META_CUEPOINT:
					{
						// FF 07 len text
						FBM_LOG( "\t\t\tCue Point\n" );
						bTextEvent = true;
					}
					break;
					case META_PROGRAM:
					{
						// FF 08 len text
						FBM_LOG( "\t\t\tProgram Name\n" );
						bTextEvent = true;
					}
					break;
					case META_DEVICE:
					{
						// FF 09 len text
						FBM_LOG( "\t\t\tDevice (Port) Name\n" );
						bTextEvent = true;
					}
					break;
					case META_PREFIXCH:
					{
						// FF 20 01 0n
						FBM_LOG( "\t\t\tPrefix Channel\n" );
						pucD += 2;
					}
					break;
					case META_PREFIXPORT:
					{
						// FF 21 01 0n
						FBM_LOG( "\t\t\tPrefix Port\n" );
						pucD += 2;
					}
					break;
					case META_ENDOFTRACK:
					{
						// FF 2F 00
						FBM_LOG( "\t\t\tEnd Of Track:\t" );
						if( *pucD == 0x00 )
						{
							FBM_LOG( "ok" );
							poTrack->m_oLstEvent.Append( new CEvent( uliT, 0, 0, 0 ) );
							bNotEndOfTrack = false;
						}
						else
							FBM_LOG( "error" );
						FBM_LOG( "\n" );
						++pucD;
					}
					break;
					case META_TEMPO:
					{
						// FF 51 03 tt tt tt
						FBM_LOG( "\t\t\tTempo\n" );
						
						if( *pucD == 0x03 )
						{
							++pucD;
							
							m_uiMicroSecPerQuarter = 0;
							m_uiMicroSecPerQuarter |= ( *pucD++ ) << 16;
							m_uiMicroSecPerQuarter |= ( *pucD++ ) << 8;
							m_uiMicroSecPerQuarter |= ( *pucD++ );
							FBM_LOG( "\t\t\tuliMicroSecPerQuarter = %d\n", m_uiMicroSecPerQuarter );
						}
						else
						{
							pucD += 4;
						}
					}
					break;
					case META_SMPTE:
					{
						// FF 54 05 hr mn se fr ff
						FBM_LOG( "\t\t\tSMPTE Offset\n" );
						if( *pucD == 0x05 )
						{
							// todo
						}
						pucD += 6;
					}
					break;
					case META_TIMESIG:
					{
						// FF 58 04 nn dd cc bb
						FBM_LOG( "\t\t\tTime Signature\n" );
						if( *pucD == 0x04 )
						{
							// todo
						}
						pucD += 5;
					}
					break;
					case META_KEYSIG:
					{
						// FF 59 02 sf mi
						FBM_LOG( "\t\t\tKey Signature\n" );
						if( *pucD == 0x02 )
						{
							// todo
						}
						pucD += 3;
					}
					break;
					case META_PROPRIET:
					{
						// FF 7F len data
						FBM_LOG( "\t\t\tProprietary Event\n" );
						bTextEvent = true;
					}
					break;
					}
					if( bTextEvent )
					{
						unsigned long int uliSize = 0L;
						const unsigned int uiBytes = VarLen4Read_( &uliSize, pucD );
						pucD += uiBytes;
						const unsigned char ucBak = pucD[uliSize];
						pucD[uliSize] = 0;
						FBM_LOG( "\t\t\t%s\n", pucD );
						pucD[uliSize] = ucBak;
						pucD += uliSize;
					}
				}
				break;
				default:
					FBM_LOG( "\t\tUNKNOWN (status: %x)\n", ucStatus );
				}
			}
			
			// Wurde kein neuer Event erzeugt?
			if( uiEventCountPrev == poTrack->m_oLstEvent.GetSize() )
			{
				uliClocksAdd += uliDeltaTime;
			}
			else
			{
				uliClocksAdd = 0;
			}
			
			FBM_LOG( "\t\t\tclocks:\t\t%li\n", uliDeltaTime );
			uliClocks += uliDeltaTime;
		}
		poTrack->m_uliDuration = uliClocks;
		FBM_LOG( "track clocks total:\t%li\n", uliClocks );
		if( bNotEndOfTrack )
		{
			FBM_LOG( "error: no END_OF_TRACK META EVENT\n" );
			poTrack->m_oLstEvent.Append( new CEvent( 0, 0, 0, 0 ) );
		}
		
		if( poTrack->m_oLstEvent.GetSize() > 1 )
		{
			m_oArrTrack.Append( poTrack );
		}
		else
		{
			delete poTrack;
			FBM_LOG( "error: invalid track\n" );
		}
	}
	FBM_LOG( "ok\n" );
	
	// Padding.
	// rem: Zu kurze Tracks werden so lang wie der l�ngste gemacht.
	unsigned long int uliDurMax = GetTrackDurationMax();
	for( unsigned int i=0; i<m_oArrTrack.GetSize(); ++i )
	{
		const unsigned long int uliDeltaPad = uliDurMax - m_oArrTrack[i]->m_uliDuration;
		if( uliDeltaPad )
		{
			CList<CEvent *> &roList = m_oArrTrack[i]->m_oLstEvent;
			CEvent * poEvent = 0;
			roList.MoveToBack();
			roList.GetPrev( &poEvent );
			if( poEvent )
			{
				poEvent->m_uliDeltaTime = uliDeltaPad;
				FBM_LOG( "track %d: padding: %li\n", i, uliDeltaPad );
			}
		}
	}
	
	ResetAllTracks();
	
	return true;
}

void CFileBlockMidi::Clear()
{
	for( unsigned int i=0; i<m_oArrTrack.GetSize(); ++i )
	{
		CTrack *poTrack = m_oArrTrack[i];
		if( poTrack )
			delete poTrack;
	}
	m_oArrTrack.Clear();
	
	m_uiFormat = 0;
	m_uiDivision = 0;
	m_uiMicroSecPerQuarter = 0;
	CFileBlock::Clear();
}

//bool CFileBlockMidi::Decode( void *pvData, unsigned int uiBytes ) {
//	return false;
//}

//bool CFileBlockMidi::Encode( void *pvData, unsigned int uiBytes ) {
//	return false;
//}

unsigned int CFileBlockMidi::GetTrackCount() const
{
	return m_oArrTrack.GetSize();
}

unsigned long int CFileBlockMidi::GetTrackDuration( unsigned int uiTrack ) const
{
	return m_oArrTrack[uiTrack]->m_uliDuration;
}

unsigned long int CFileBlockMidi::GetTrackDurationMax() const
{
	unsigned long int uliDuration = 0L;
	for( unsigned int i=0; i<m_oArrTrack.GetSize(); ++i )
	{
		const unsigned long int uliD = m_oArrTrack[i]->m_uliDuration;
		if( uliDuration < uliD )
			uliDuration = uliD;
	}
	return uliDuration;
}

void CFileBlockMidi::ResetAllTracks()
{
	for( unsigned int i=0; i<m_oArrTrack.GetSize(); ++i )
	{
		ResetTrack( i );
	}
}

void CFileBlockMidi::ResetTrack( unsigned int uiTrack )
{
	m_oArrTrack[uiTrack]->Reset();
}

bool CFileBlockMidi::Clock( unsigned int uiTrack, CFileBlockMidi::CEvent **ppoEvent )
{
	if( !ppoEvent || uiTrack >= m_oArrTrack.GetSize() )
		return false;
	
	CTrack *poTrack = m_oArrTrack[uiTrack];
	if( poTrack )
	{
		if( poTrack->m_uliCounter == 0L )
		{
			if( poTrack->Proc( ppoEvent ) )
				return true; // Weitere gleichzeitige Events folgen.
		}
		else
		{
			*ppoEvent = 0;
		}
		--poTrack->m_uliCounter;
		//++poTrack->c; // tmp
	}
	return false;
}

#endif // _FILE_BLOCK_MIDI_CPP_
