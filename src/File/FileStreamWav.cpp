// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FileStreamWav.cpp

#ifndef _FILE_STREAM_WAV_CPP_
#define _FILE_STREAM_WAV_CPP_

#include "FileStreamWav.h"

#ifndef _WAVEFORMATEX_
#define DWORD unsigned long int
#define WORD unsigned short int
#define BYTE unsigned char
#define WAVE_FORMAT_PCM 1
typedef struct { 
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
	WORD wBitsPerSample;
	//WORD cbSize;
} WAVEFORMATEX; 
typedef struct {
	DWORD	        riff;           // "RIFF"
	DWORD	        filesize;       // total file size - 8
	DWORD	        wave;           // "WAVE"
	DWORD	        fmt_;           // "fmt "
	DWORD	        formatsize;     // format data size
	WAVEFORMATEX	format;         // dudio data format
	DWORD	        data;	        // "data"
	DWORD	        datasize;       // dudio data size
} WAVEHEADER;
#endif // _WAVEFORMATEX_

////////////////////////////////////////////////////////////////////////////////
// CFileStreamWav
////////////////////////////////////////////////////////////////////////////////

#define FILE_WAV_DEPTH	16

CFileStreamWav::CFileStreamWav()
: CFileStream()
, m_uiSampleFreq( 0 )
, m_uiChannels( 0 )
, m_uiDataBytes( 0 )
, m_uiMIDIRootKey( 60 )
, m_uiLoopType( LOOP_NONE )
, m_uiLoopStart( 0 )
, m_uiLoopEnd( 0 )
, m_uiLoopPlayCount( 0 )
{}

CFileStreamWav::CFileStreamWav( const CFileStreamWav &roO )
: CFileStream()
{
	*this = roO;
}

CFileStreamWav::~CFileStreamWav()
{}

CFileStreamWav & CFileStreamWav::operator=( const CFileStreamWav &roO )
{
	CFileStream::operator=( roO );
	m_uiSampleFreq = roO.m_uiSampleFreq;
	m_uiChannels = roO.m_uiChannels;
	m_uiDataBytes = roO.m_uiDataBytes;
	m_uiMIDIRootKey = roO.m_uiMIDIRootKey;
	m_uiLoopType = roO.m_uiLoopType;
	m_uiLoopStart = roO.m_uiLoopStart;
	m_uiLoopEnd = roO.m_uiLoopEnd;
	m_uiLoopPlayCount = roO.m_uiLoopPlayCount;
	return *this;
}

// <mod date="2010-12-10">

bool CFileStreamWav::Open( const char * pcFileName, unsigned int uiFlags )
{
#define CMP4( a, b ) ( a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3] )

	if( !CFileStream::Open( pcFileName, uiFlags ) )
		return false;
	
	//     t   r   w
	// 0:  1   1   1  // header schreiben					x
	// 1:  1   1   0  // error (hier)					x
	// 2:  1   0   1  // header schreiben					x
	// 3:  1   0   0  // error (in CFile)					x
	// 4:  0   1   1  // header lesen, überprüfen, vorspulen, schreiben	x
	// 5:  0   1   0  // header lesen, überprüfen				x
	// 6:  0   0   1  // error (hier)					x
	// 7:  0   0   0  // error (in CFile)					x
	
	if( ( m_bIsTrunc && m_bIsRead && !m_bIsWrite ) || // Geleerte Datei lesen?
	    ( !m_bIsTrunc && !m_bIsRead && m_bIsWrite ) ) // Schreiben ohne lesen, !trunc...
		return false;
	
	m_uiMIDIRootKey  = 60;
	m_uiLoopType = LOOP_NONE;
	m_uiLoopStart = 0;
	m_uiLoopEnd = 0;
	m_uiLoopPlayCount = 0;	
	
	const unsigned int uiFileSize = GetSize();
	if( !m_bIsTrunc && uiFileSize != 0 )
	{
		/*
		WAVEHEADER oHeader;
		
		if( Read( &oHeader, sizeof(WAVEHEADER) ) != sizeof(WAVEHEADER) )
			return false;
		
		if( //oHeader.filesize != uiFileSize - 8 || // kann man nicht drauf gehen.
		    //oHeader.datasize != uiFileSize - 44 || // "
		    oHeader.format.wBitsPerSample != FILE_WAV_DEPTH )
			return false;
		m_uiSampleFreq	= oHeader.format.nSamplesPerSec;
		m_uiChannels	= oHeader.format.nChannels;
		m_uiDataBytes	= uiFileSize - 44;//oHeader.datasize;
		*/
		
		// ref: http://www.sonicspot.com/guide/wavefiles.html
		// ref: https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
		
		bool bContinue( true );
		bool bRIFFChunkOK( false );
		while( bContinue )
		{
			char acChunkID[4];
			//unsigned int uiChunkID;
			unsigned int uiChunkDataSize;
			if( !Read( &acChunkID, 4 ) //!Read( &uiChunkID, sizeof(uiChunkID) )
			 || !Read( &uiChunkDataSize, 4 ) ) //sizeof(uiChunkDataSize) ) )
			{
				bContinue = false;
				break;
			}
			//uiChunkID = ForceBigEndian_<unsigned int>( uiChunkID );
			uiChunkDataSize = ForceLittleEndian_<unsigned int>( uiChunkDataSize );
			
			if( CMP4( acChunkID, "RIFF" ) ) //uiChunkID == 0x46464952 ) // "RIFF"
			{
				char acRIFFType[4];
				//unsigned int uiRIFFType;
				if( !Read( &acRIFFType, 4 ) // sizeof(uiRIFFType) )
				 || !CMP4( acRIFFType, "WAVE" ) //|| uiRIFFType != 0x45564157 // "WAVE"
				 //|| uiChunkDataSize != uiFileSize - 8
				 )
				{
					bContinue = false;
				}
				else
				{
					bRIFFChunkOK = true;
				}
			}
			else if( bRIFFChunkOK )
			{
				//switch( uiChunkID )
				//{
				//case 0x20746D66: // "fmt"
				if( CMP4( acChunkID, "fmt " ) )
				{
					WAVEFORMATEX oFmt;
					if(	Read( &oFmt.wFormatTag, 2 ) &&
						Read( &oFmt.nChannels, 2 ) &&
						Read( &oFmt.nSamplesPerSec, 4 ) &&
						Read( &oFmt.nAvgBytesPerSec, 4 ) &&
						Read( &oFmt.nBlockAlign, 2 ) &&
						Read( &oFmt.wBitsPerSample, 2 ) )
					{
						m_uiSampleFreq = ForceLittleEndian_<unsigned int>( oFmt.nSamplesPerSec );
						m_uiChannels = ForceLittleEndian_<unsigned int>( oFmt.nChannels );
					}
					
					//if( Read( &oFmt, sizeof(oFmt) ) )
					//{
					//	m_uiSampleFreq	= oFmt.nSamplesPerSec;
					//	m_uiChannels	= oFmt.nChannels;
					//}
				}
				//break;
				//case 0x61746164: // "data"
				else if( CMP4( acChunkID, "data" ) )
				{
					//MovePointer( uiChunkDataSize, SEEK_CUR );
					m_uiDataBytes = uiChunkDataSize; // - 8;
					MovePointer( uiChunkDataSize, SEEK_CUR );
					//bContinue = false;
				}
				//break;
				//case 0x6C706D73: // "smpl"
				else if( CMP4( acChunkID, "smpl" ) )
				{
					unsigned int uiManufacturer;
					unsigned int uiProduct;
					unsigned int uiSamplePeriod;
					unsigned int uiMIDIUnityNote;
					unsigned int uiMIDIPitchFraction;
					unsigned int uiSMPTEFormat;
					unsigned int uiSMPTEOffset;
					unsigned int uiNumSampleLoops;
					unsigned int uiSamplerData;
					if( Read( &uiManufacturer, 4 ) //sizeof(uiManufacturer) )
					 && Read( &uiProduct, 4 ) //sizeof(uiProduct) )
					 && Read( &uiSamplePeriod, 4 ) //sizeof(uiSamplePeriod) )
					 && Read( &uiMIDIUnityNote, 4 ) //sizeof(uiMIDIUnityNote) )
					 && Read( &uiMIDIPitchFraction, 4 ) //sizeof(uiMIDIPitchFraction) )
					 && Read( &uiSMPTEFormat, 4 ) //sizeof(uiSMPTEFormat) )
					 && Read( &uiSMPTEOffset, 4 ) //sizeof(uiSMPTEOffset) )
					 && Read( &uiNumSampleLoops, 4 ) //sizeof(uiNumSampleLoops) )
					 && Read( &uiSamplerData, 4 ) ) //sizeof(uiSamplerData) ) )
					{
						m_uiMIDIRootKey = ForceLittleEndian_<unsigned int>( uiMIDIUnityNote );
						
						// List of Sample Loops
						unsigned int uiLoopCuePointID;
						unsigned int uiLoopType;
						unsigned int uiLoopStart;
						unsigned int uiLoopEnd;
						unsigned int uiLoopFraction;
						unsigned int uiLoopPlayCount;
						while( uiNumSampleLoops > 0
						 && Read( &uiLoopCuePointID, 4 ) //, sizeof(uiLoopCuePointID) )
						 && Read( &uiLoopType, 4 ) //, sizeof(uiLoopType) )
						 && Read( &uiLoopStart, 4 ) //, sizeof(uiLoopStart) )
						 && Read( &uiLoopEnd, 4 ) //, sizeof(uiLoopEnd) )
						 && Read( &uiLoopFraction, 4 ) //, sizeof(uiLoopFraction) )
						 && Read( &uiLoopPlayCount, 4 ) ) //, sizeof(uiLoopPlayCount) ) )
						{
							--uiNumSampleLoops;
							m_uiLoopPlayCount = ForceLittleEndian_<unsigned int>( uiLoopPlayCount );
							m_uiLoopStart = ForceLittleEndian_<unsigned int>( uiLoopStart );
							m_uiLoopEnd = ForceLittleEndian_<unsigned int>( uiLoopEnd );
							uiLoopType = ForceLittleEndian_<unsigned int>( uiLoopType );
							switch( uiLoopType )
							{
							case 0:
								m_uiLoopType = LOOP_FORWARD;
							break;
							case 1:
								m_uiLoopType = LOOP_PINGPONG;
							break;
							case 2:
								m_uiLoopType = LOOP_BACKWARD;
							break;
							default:
								m_uiLoopType = LOOP_NONE;
							}
						}
					}
				}
				//break;
				//}
			}
		}
		if( m_bIsWrite )
			MovePointer( 0, CFile::PTR_END );
		else
			MovePointer( sizeof(WAVEHEADER), CFile::PTR_BEG ); // hmmm
	}
	else if( m_bIsWrite )
	{
		// <mod date="2010-12-09"> // totest // (Assuming: WORD ~ 2 Bytes, DWORD ~ 4 Bytes.)
		const WORD wBlockAlign = m_uiChannels * ( FILE_WAV_DEPTH / 8 );
		static const char * acRiffWave_( "RIFF0000WAVEfmt " ); // C-String: No endianess issues.
		static const char * acData_( "data0000" ); // "
		const DWORD dwFormatSize = ForceLittleEndian_<DWORD>( sizeof(WAVEFORMATEX) );
		const WAVEFORMATEX oWaveFormat = {
				ForceLittleEndian_<WORD>( WAVE_FORMAT_PCM ),
				ForceLittleEndian_<WORD>( m_uiChannels ),
				ForceLittleEndian_<DWORD>( m_uiSampleFreq ),
				ForceLittleEndian_<DWORD>( m_uiSampleFreq * wBlockAlign ),
				ForceLittleEndian_<WORD>( wBlockAlign ),
				ForceLittleEndian_<WORD>( FILE_WAV_DEPTH ) };
		Write( (void *)acRiffWave_, 16 );
		Write( (void *)&dwFormatSize, 4 );
		//Write( (void *)&oWaveFormat, sizeof(WAVEFORMATEX) );
		Write( (void *)&oWaveFormat.wFormatTag, 2 );
		Write( (void *)&oWaveFormat.nChannels, 2 );
		Write( (void *)&oWaveFormat.nSamplesPerSec, 4 );
		Write( (void *)&oWaveFormat.nAvgBytesPerSec, 4 );
		Write( (void *)&oWaveFormat.nBlockAlign, 2 );
		Write( (void *)&oWaveFormat.wBitsPerSample, 2 );
		Write( (void *)acData_, 8 );
		// </mod>
	}
	return true;
	
#undef CMP4
} 
// </mod>

// <mod date="2010-12-10">
unsigned int CFileStreamWav::ReadValues( signed short int *pssiData, unsigned int uiCount )
{
	if( m_bIsLittleEndian_ )
	{
		return Read( (void *)pssiData, uiCount * 2 );
	}
	
	unsigned int uiRet( 0 );
	while( uiCount )
	{
		--uiCount;
		signed short int ssiValue;
		uiRet += Read( (void *)&ssiValue, 2 );
		*pssiData = ForceLittleEndian_<signed short int>( ssiValue );
		++pssiData;
	}
	return uiRet;
}

unsigned int CFileStreamWav::WriteValues( const signed short int *pssiData, unsigned int uiCount )
{
	if( m_bIsLittleEndian_ )
	{
		return Write( (void *)pssiData, uiCount * 2 );
	}
	
	unsigned int uiRet( 0 );
	while( uiCount )
	{
		--uiCount;
		const signed short int ssiValue( ForceLittleEndian_<signed short int>( *pssiData ) );
		uiRet += Write( (void *)&ssiValue, 2 );
		++pssiData;
	}
	return uiRet;
}
// </mod>

bool CFileStreamWav::Close()
{
	bool bRet = true;
	if( m_bIsWrite && m_uiChannels )
	{
		// <mod date="2010-12-09"> // totest // (Assuming: WORD ~ 2 Bytes, DWORD ~ 4 Bytes.)
		const DWORD dwFileSize( GetSize() );
		const DWORD dwFileSizeM8( ForceLittleEndian_<DWORD>( dwFileSize - 8 ) );
		const DWORD dwDataSize( ForceLittleEndian_<DWORD>( dwFileSize - 44 ) );
		MovePointer( 4, CFile::PTR_BEG );
		Write( (void *)&dwFileSizeM8, 4 );
		MovePointer( 40, CFile::PTR_BEG );
		Write( (void *)&dwDataSize, 4 );
		// </mod>
	}
	else
		bRet = false;
	if( !CFileStream::Close() )
		return false;
	return bRet;
}

#endif // _FILE_STREAM_WAV_CPP_
