// AS.cpp

#include "AS.h"

////////////////////////////////////////////////////////////////////////////////
// CAS
////////////////////////////////////////////////////////////////////////////////

CAS::CAS()
: m_ptBuffer( 0 )
, m_uiChannels( 0 )
, m_uiSampleFreq( 0 )
, m_uiSampleDepth( 0 )
, m_uiBufferSamples( 0 )
//, m_sliBufferBytes( 0 )
{}

CAS::~CAS()
{
	ClearBuffer();
}

void CAS::SetFormat(
	unsigned int uiChannels,
	unsigned int uiSampleFreq,
	unsigned int uiSampleDepth,
	unsigned int uiBufferSamples )
{
	m_uiChannels = uiChannels;
	m_uiSampleFreq = uiSampleFreq;
	m_uiSampleDepth = uiSampleDepth;
	m_uiBufferSamples = uiBufferSamples;
	//m_sliBufferBytes = uliBufferSamples * m_uiChannels * ( uiSampleDepth / 8 );
}

bool CAS::ResetBuffer()
{
	if( m_ptBuffer )
	{
		memset( m_ptBuffer, 0, m_uiBufferSamples * m_uiChannels * sizeof(CAS::TValue) );
		return true;
	}
	return false;
}

CAS::TValue * CAS::GetBuffer() const {
	return m_ptBuffer;
}

unsigned int CAS::GetSampleFreq() const {
	return m_uiSampleFreq;
}

unsigned int CAS::GetBufferSamples() const {
	return m_uiBufferSamples;
}

void CAS::CreateBuffer()
{
	m_ptBuffer = new CAS::TValue[ m_uiBufferSamples * m_uiChannels ];
	ResetBuffer();
}

void CAS::ClearBuffer()
{
	if( m_ptBuffer )
	{
		delete [] m_ptBuffer;
		m_ptBuffer = 0;
	}
}

#if defined( __LINUX__ )

////////////////////////////////////////////////////////////////////////////////
// CASDeviceAlsa
////////////////////////////////////////////////////////////////////////////////

CASDeviceAlsa::CASDeviceAlsa()
: CAS()
, m_poHandle( 0 )
, m_poHW( 0 )
, m_poSW( 0 )
//, m_uiFragments( 0 )
{}

CASDeviceAlsa::~CASDeviceAlsa() {
	Clear();
}

bool CASDeviceAlsa::Init( const char *pcDev, bool bIsOutput )
{
	int iErr;

	if( ( iErr = snd_pcm_open( &m_poHandle, pcDev, ( bIsOutput ? SND_PCM_STREAM_PLAYBACK : SND_PCM_STREAM_CAPTURE ), 0 ) ) < 0 )
		AS_ERROR_RET( "cannot open audio device %s (%s)\n", pcDev, snd_strerror( iErr ) );

	if( ( iErr = snd_pcm_hw_params_malloc( &m_poHW ) ) < 0 )
		AS_ERROR_RET( "cannot allocate hardware parameter structure (%s)\n", snd_strerror( iErr ) );

	if( ( iErr = snd_pcm_hw_params_any( m_poHandle, m_poHW ) ) < 0 )
		AS_ERROR_RET( "cannot initialize hardware parameter structure (%s)\n", snd_strerror( iErr ) );

	if( ( iErr = snd_pcm_hw_params_set_access( m_poHandle, m_poHW, SND_PCM_ACCESS_RW_INTERLEAVED ) ) < 0 )
		AS_ERROR_RET( "cannot set access type (%s)\n", snd_strerror( iErr ) );

	if( ( iErr = snd_pcm_hw_params_set_format( m_poHandle, m_poHW, SND_PCM_FORMAT_S16_LE ) ) < 0 )
		AS_ERROR_RET( "cannot set sample format (%s)\n", snd_strerror( iErr ) );

	unsigned int uiSF = m_uiSampleFreq;
	if( ( iErr = snd_pcm_hw_params_set_rate_near( m_poHandle, m_poHW, &uiSF, 0 ) ) < 0 )
		AS_ERROR_RET( "cannot set sample rate (%s)\n", snd_strerror( iErr ) );
	if( uiSF != m_uiSampleFreq ) {
		fprintf(stderr, "cannot set requested sample rate, asked for %d got %d\n", m_uiSampleFreq, uiSF );
		m_uiSampleFreq = uiSF;
	}

	if( ( iErr = snd_pcm_hw_params_set_channels( m_poHandle, m_poHW, 2 ) ) < 0 )
		AS_ERROR_RET( "cannot set channel count (%s)\n", snd_strerror( iErr ) );

	/*
	m_uiFragments = 2;
	// http://www.bel.fi/~alankila/blog/2005/08/21/Full%20Duplex%20ALSA.html
	//if( ( iErr = snd_pcm_hw_params_set_periods( m_poHandle, m_poHW, 2, 0 ) ) < 0 )
	if( ( iErr = snd_pcm_hw_params_set_periods_near( m_poHandle, m_poHW, &m_uiFragments, 0 ) ) < 0 )
		AS_ERROR_RET( "cannot periods (%s)\n", snd_strerror( iErr ) );
	*/

	// ??
	//if( ( iErr = snd_pcm_hw_params_set_period_size( m_poHandle, m_poHW, m_uliBufferSamples, 0 ) ) < 0 )
	//	AS_ERROR_RET( "cannot set period size (%s)\n", snd_strerror( iErr ) );
	snd_pcm_uframes_t oBS = m_uiBufferSamples;

	/*
	//unsigned int uiFrameSize = m_uiChannels * ( m_uiSampleDepth / 8 );
	unsigned int uiFrag = oBS % m_uiFragments;
	if( uiFrag )
	{
		oBS += m_uiFragments - uiFrag;
	}
	*/

	//printf( "> frames: %li\n", oBS );
	if( ( iErr = snd_pcm_hw_params_set_buffer_size_near( m_poHandle, m_poHW, &oBS ) ) < 0 )
		AS_ERROR_RET( "cannot set buffer size (%s)\n", snd_strerror( iErr ) );
	if( oBS != m_uiBufferSamples )
	{
		fprintf(stderr, "Could not set requested buffer size, asked for %d got %li\n", m_uiBufferSamples, oBS );
		m_uiBufferSamples = oBS;
	}

	/*
	unsigned int frame_size = m_uiChannels * (m_uiSampleDepth / 8);
	snd_pcm_uframes_t frames = m_uiBufferSamples / frame_size * m_uiFragments;
	printf( "> frames: %li\n", frames );

	if ((iErr = snd_pcm_hw_params_set_buffer_size_near(m_poHandle, m_poHW, &frames)) < 0) {
		AS_ERROR_RET( "Error setting buffer_size %li frames: %s\n", frames,
		snd_strerror(iErr));
	return 1;
	}
	if (m_uiBufferSamples != frames * frame_size / m_uiFragments) {
		AS_ERROR_RET( "Could not set requested buffer size, asked for %d got %li\n", m_uiBufferSamples, frames * frame_size / m_uiFragments);
	m_uiBufferSamples = frames * frame_size / m_uiFragments;
	}
	*/

	if( ( iErr = snd_pcm_hw_params( m_poHandle, m_poHW ) ) < 0 )
		AS_ERROR_RET( "cannot set parameters (%s)\n", snd_strerror( iErr ) );

	CreateBuffer();

	if( !bIsOutput )
		return true;

	// tell ALSA to wake us up whenever m_sliBufferSize or more frames
	// of playback data can be delivered. Also, tell
	// ALSA that we'll start the device ourselves.

	if( ( iErr = snd_pcm_sw_params_malloc( &m_poSW ) ) < 0 )
		AS_ERROR_RET( "cannot allocate software parameters structure (%s)\n", snd_strerror( iErr ));

	if( ( iErr = snd_pcm_sw_params_current( m_poHandle, m_poSW ) ) < 0 )
		AS_ERROR_RET( "cannot initialize software parameters structure (%s)\n", snd_strerror( iErr ));

	// doesn't work when recording!
	//if( ( iErr = snd_pcm_sw_params_set_avail_min( m_poHandle, poParamSW, m_sliBufferSize ) ) < 0 )
	//	AS_ERROR_RET( "cannot set minimum available count (%s)\n", snd_strerror( iErr ) );

	if( ( iErr = snd_pcm_sw_params_set_start_threshold( m_poHandle, m_poSW, 0U ) ) < 0 )
		AS_ERROR_RET( "cannot set start mode (%s)\n", snd_strerror( iErr ) );

	if( ( iErr = snd_pcm_sw_params( m_poHandle, m_poSW ) ) < 0 )
		AS_ERROR_RET( "cannot set software parameters (%s)\n", snd_strerror( iErr ) );

	return true;
}

bool CASDeviceAlsa::Start()
{
	int iErr;

	// the interface will interrupt the kernel every m_sliBufferSize frames, and ALSA
	// will wake up this program very soon after that.

	if( ( iErr = snd_pcm_prepare( m_poHandle ) ) < 0 )
		AS_ERROR_RET( "cannot prepare audio interface for use (%s)\n", snd_strerror( iErr ) );

	return true;
}

bool CASDeviceAlsa::Stop()
{
	// Pause/resume PCM:
	// snd_pcm_pause / snd_pcm_hw_params_can_pause
	// snd_pcm_resume

	if( m_poHandle )
		snd_pcm_drop( m_poHandle ); // Stop a PCM dropping pending frames.
	//	snd_pcm_drain( m_poHandle ); // Stop a PCM preserving pending frames.
	return true; // TODO
}

void CASDeviceAlsa::Clear()
{
	Stop();
	if( m_poHandle )
	{
		snd_pcm_close( m_poHandle );
		m_poHandle = 0;
	}
	if( m_poHW )
		snd_pcm_hw_params_free( m_poHW );
	if( m_poSW )
		snd_pcm_sw_params_free( m_poSW );

	ClearBuffer();
}

//unsigned int CASDeviceAlsa::GetFragmentsCout() const {
//	return m_uiFragments;
//}


////////////////////////////////////////////////////////////////////////////////
// CASOut (Linux/ALSA)
////////////////////////////////////////////////////////////////////////////////

CASOut::CASOut()
: CASDeviceAlsa()
{}

CASOut::~CASOut() {
}

bool CASOut::Init() {
	const char *acDeviceName = AS_ALSA_DEV_OUT;
	return CASDeviceAlsa::Init( acDeviceName, true );
}

bool CASOut::Start()
{
	if( !CASDeviceAlsa::Start() )
		return false;

	// fill the whole output buffer...
	// (needed for strange reason)
	// for( unsigned int i = 0; i < m_uiFragments; ++i ) {
	// 	oASOut.Write( ptBufferOut, uiSamples / 2 );
	// }
	//Write( m_uiBufferSamples );

	return true;
}

unsigned int CASOut::Prepare()
{
	int iRet;

	// wait till the interface is ready for data, or 1 second
	// has elapsed.
	if(( iRet = snd_pcm_wait( m_poHandle, 1000 )) < 0 ) {
		AS_ERROR( "poll failed (%s)\n", strerror ( iRet ) );
		// ACHTUNG: Hack!
		Stop();
		Start();
		//return 0;
	}

	// find out how much space is available for playback data
	if(( iRet = snd_pcm_avail_update( m_poHandle ) ) < 0 )
	{
		// an underrun occurred
		if( iRet == -EPIPE ) {
			AS_ERROR( "an xrun occured\n" );
			return 0;
		}
		else {
			AS_ERROR( "unknown ALSA avail update return value (%i)\n",
				 iRet );
			return 0;
		}
	}
	if( (unsigned int)iRet > m_uiBufferSamples )
	{
		//AS_ERROR( "?? more space (%li)\n",
		//	m_uiSamples);
		iRet = m_uiBufferSamples;
	}

	return (unsigned int)( iRet );
}

bool CASOut::Write() {
	return Write( m_uiBufferSamples );
}

bool CASOut::Write( unsigned int uiSamples )
{
	int iRet;

	// deliver the data
	// AS_LOG( "playback callback called with %li frames\n", m_uiSamples );

	while(( iRet = snd_pcm_writei( m_poHandle, m_ptBuffer, uiSamples )) < 0 ) {
		if( iRet == -EAGAIN )
			continue;
		AS_ERROR( "write failed (%s)\n", snd_strerror( iRet ) );
		Start();
	}

	if( (unsigned int)iRet != uiSamples ) {
		AS_ERROR( "playback callback failed\n" );
		//snd_pcm_prepare( m_poHandle ); // ??
		// ACHTUNG: Hack!
		Stop();
		Start();
		//return false;
	}
	return true;
}


////////////////////////////////////////////////////////////////////////////////
// CASIn (Linux/ALSA)
////////////////////////////////////////////////////////////////////////////////

CASIn::CASIn()
: CASDeviceAlsa()
{}

CASIn::~CASIn()
{}

bool CASIn::Init() {
	const char *acDeviceName = AS_ALSA_DEV_IN;
	return CASDeviceAlsa::Init( acDeviceName, false );
}

unsigned int CASIn::Read() {
	return Read( m_uiBufferSamples );
}

unsigned int CASIn::Read( unsigned int uiSamples )
{
	int iRet;
	while(( iRet = snd_pcm_readi( m_poHandle, m_ptBuffer, uiSamples )) < 0 ) {
		if( iRet == -EAGAIN )
			continue;
		AS_ERROR( "read failed (%s)\n", snd_strerror( iRet ) );
		snd_pcm_prepare( m_poHandle );
	}
	if( (unsigned int)iRet != uiSamples ) {
		AS_ERROR( "capture callback failed\n" );
		//snd_pcm_prepare( m_poHandle ); // ??
		return 0;
	}
	return iRet;
}

#elif defined( __WINDOWS__ )

////////////////////////////////////////////////////////////////////////////////
// CASDeviceDX
////////////////////////////////////////////////////////////////////////////////

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

CASDeviceDX::CASDeviceDX()
: CAS()
, m_uiBufferSize( 0 )
, m_pvTransfer0( 0 )
, m_pvTransfer1( 0 )
, m_dwLength0( 0 )
, m_dwLength1( 0 )
, m_dwPosCapture( 0 )
, m_dwPosRead( 0 )
//, m_uiSleepMs( 0 )
{
	ZeroMemory( &m_oWaveFormatEx, sizeof( WAVEFORMATEX ) );
}

CASDeviceDX::~CASDeviceDX()
{}

void CASDeviceDX::SetFormat(
		unsigned int uiChannels,
		unsigned int uiSampleFreq,
		unsigned int uiSampleDepth,
		unsigned int uiBufferSamples )
{
	CAS::SetFormat( uiChannels, uiSampleFreq, uiSampleDepth, uiBufferSamples );
	m_oWaveFormatEx.wFormatTag	= WAVE_FORMAT_PCM;
	m_oWaveFormatEx.nChannels	= uiChannels;
	m_oWaveFormatEx.nSamplesPerSec	= uiSampleFreq;
	m_oWaveFormatEx.nBlockAlign	= ( uiSampleDepth >> 3 ) * uiChannels;
	m_oWaveFormatEx.nAvgBytesPerSec	= uiSampleFreq * m_oWaveFormatEx.nBlockAlign;
	m_oWaveFormatEx.wBitsPerSample	= uiSampleDepth;
	m_oWaveFormatEx.cbSize		= 0;

	m_uiBufferSize = m_uiBufferSamples * m_oWaveFormatEx.nBlockAlign;

	// Teil der Buffer-Länge schlafen, nach jedem Schreizyklus,
	// um performancelastige GetCurrentPosition()-Calls zu vermeiden.
	//#define SND_SLEEP_MS (( 1000 / 12 )/( AUDIO_SAMPLE_FREQ / AUDIO_BUFFER_SIZE ))
	//m_uiSleepMs = (( 1000 / 12 )/( uiSampleFreq / uiBufferSamples ));
}


////////////////////////////////////////////////////////////////////////////////
// CASOut (Windows/DirectX)
////////////////////////////////////////////////////////////////////////////////

unsigned int CASOut::m_uiInstances_ = 0;
LPDIRECTSOUND8 CASOut::m_poDSDev_ = 0;

CASOut::CASOut():
CASDeviceDX(),
m_poDSBuffer(0),
m_dwOffset(0)
{
	timeBeginPeriod(1);
}

CASOut::~CASOut()
{
	timeEndPeriod(1);
	Clear();
}

bool CASOut::Init()
{
	DSBUFFERDESC DSBuffDesc;

	// DirectSound Device Object erzeugen.
	if( !m_uiInstances_ )
	{
		// rem: DSSCL_EXCLUSIVE wäre auch möglich, ist jedoch teilweise nicht unterstützt.
		DirectSoundCreate8( NULL, &m_poDSDev_, 0 );
		m_poDSDev_->SetCooperativeLevel( GetDesktopWindow(), DSSCL_PRIORITY ); // z.B. mit GetConsoleWindow()

		// Primary Buffer erzeugen.
		m_poDSBuffer = 0;
		ZeroMemory(&DSBuffDesc, sizeof(DSBUFFERDESC) );
		DSBuffDesc.dwSize =	sizeof(DSBUFFERDESC);
		DSBuffDesc.dwFlags =	DSBCAPS_PRIMARYBUFFER;
		m_poDSDev_->CreateSoundBuffer( &DSBuffDesc, &m_poDSBuffer, NULL );

		// Format des Primary Buffers setzen.
		m_poDSBuffer->SetFormat( &m_oWaveFormatEx );
		SAFE_RELEASE( m_poDSBuffer );
	}

	// Secondary Buffer erzeugen.
	m_poDSBuffer = 0;
	ZeroMemory(&DSBuffDesc, sizeof(DSBUFFERDESC));
	DSBuffDesc.dwSize =	sizeof(DSBUFFERDESC);
	DSBuffDesc.dwFlags =	//DSBCAPS_CTRLVOLUME |
				//DSBCAPS_CTRLFREQUENCY |
				//DSBCAPS_CTRLPOSITIONNOTIFY |
				DSBCAPS_GETCURRENTPOSITION2 |
				DSBCAPS_GLOBALFOCUS;
	DSBuffDesc.dwBufferBytes =	m_uiBufferSize;
	DSBuffDesc.lpwfxFormat =	&m_oWaveFormatEx;
	m_poDSDev_->CreateSoundBuffer( &DSBuffDesc, &m_poDSBuffer, NULL );

	CreateBuffer();

	++m_uiInstances_;

	return true;
}

bool CASOut::ResetBuffer()
{
	if( !CAS::ResetBuffer() )
		return false;

	if( FAILED( m_poDSBuffer->Lock(	0, 0,
					&m_pvTransfer0, &m_dwLength0,
					&m_pvTransfer1, &m_dwLength1,
					DSBLOCK_FROMWRITECURSOR | DSBLOCK_ENTIREBUFFER ) ) )
		return false;
	if( m_pvTransfer0 )
		ZeroMemory( m_pvTransfer0, m_dwLength0 );
	if( m_pvTransfer1 )
		ZeroMemory( m_pvTransfer1, m_dwLength1 );
	m_poDSBuffer->Unlock( m_pvTransfer0, m_dwLength0, m_pvTransfer1, m_dwLength1 );

	return true;
}

bool CASOut::Start()
{
	if( FAILED( m_poDSBuffer->Play(0, 0, DSBPLAY_LOOPING ) ) )
		return false;
	return true;
}

bool CASOut::Stop()
{
	timeEndPeriod(1);
	if( FAILED( m_poDSBuffer->Stop() ) )
		return false;
	return true;
}

void CASOut::Clear()
{
	// Alle Ressource in korrekter Reihenfolge freigeben.
	//m_poDSBuff->Stop();
	SAFE_RELEASE( m_poDSBuffer )

	if ( m_uiInstances_ > 0 )
		if (--m_uiInstances_ == 0)
		{
			SAFE_RELEASE( m_poDSDev_ );
		}

	ClearBuffer();
}

unsigned int CASOut::Prepare()
{
	// Aktuellen Play-Cursor abfragen.
	m_poDSBuffer->GetCurrentPosition( &m_dwPosCapture, 0);

	// Anzahl Bytes oder Distanz von internen Write-Cursor zum Play-Cursor berechnen.
	unsigned int uiBytesToWrite;
	if( m_dwOffset > m_dwPosCapture ) // "Normalfall": Bis zum Play Cursor, geht es über den Rand des Puffers.
		uiBytesToWrite = ( m_uiBufferSize - m_dwOffset ) + m_dwPosCapture;
	else // Annahme: wrap around (oder wir sind im Rückstand!).
		uiBytesToWrite = m_dwPosCapture - m_dwOffset;
	return uiBytesToWrite / m_oWaveFormatEx.nBlockAlign;
}

bool CASOut::Write() {
	return Write( m_uiBufferSamples );
}

bool CASOut::Write( unsigned int uiSamples )
{
	const unsigned int uiBytesToWrite = uiSamples * m_oWaveFormatEx.nBlockAlign;

	// Streaming Buffer sperren und die Bytes schreiben. Zyklisch!
	// rem: DSERR_BUFFERLOST als Rückgabe evtl. prüfen.
	// rem: Vorausgesetzt: m_uiBytesToWrite = dwByte1+dwByte2.
	if( FAILED( m_poDSBuffer->Lock(	m_dwOffset, uiBytesToWrite,
					&m_pvTransfer0, &m_dwLength0,
					&m_pvTransfer1, &m_dwLength1, 0L ) ) )
		return false;
	CopyMemory( m_pvTransfer0, m_ptBuffer, m_dwLength0 );
	if( m_pvTransfer1 )
		CopyMemory( m_pvTransfer1, (unsigned char*)(m_ptBuffer) + m_dwLength0, m_dwLength1 );
	m_poDSBuffer->Unlock( m_pvTransfer0, m_dwLength0, m_pvTransfer1, m_dwLength1 );

	// Internen Capture-Cursor updaten.
	m_dwOffset += uiBytesToWrite;
	m_dwOffset %= m_uiBufferSize;

	//SleepEx( m_uiSleepMs, TRUE );
	return true;
}



////////////////////////////////////////////////////////////////////////////////
// CASIn (Windows/DirectX)
////////////////////////////////////////////////////////////////////////////////

unsigned int		CASIn::m_uiInstances_ = 0;
LPDIRECTSOUNDCAPTURE	CASIn::m_poDSCaptureDev_ = 0;

CASIn::CASIn():
CASDeviceDX(),
m_uiReadBytes(0),
m_poDSCaptureBuffer(0),
m_dwOffset(0),
m_lLockSize(0)
{}

CASIn::~CASIn()
{
	Clear();
}

bool CASIn::Init()
{
	DSCBUFFERDESC DSBuffDesc;

	if( !m_uiInstances_ )
	{
		if( FAILED( DirectSoundCaptureCreate( 0, &m_poDSCaptureDev_, NULL ) ) )
			return false;
	}

	ZeroMemory( &DSBuffDesc, sizeof(DSBuffDesc) );
	DSBuffDesc.dwSize        = sizeof(DSBuffDesc);
	DSBuffDesc.dwBufferBytes = m_uiBufferSize;
	DSBuffDesc.lpwfxFormat   = &m_oWaveFormatEx;

	if( FAILED( m_poDSCaptureDev_->CreateCaptureBuffer(	&DSBuffDesc,
								&m_poDSCaptureBuffer,
								NULL ) ) )
		return false;

	CreateBuffer();

	m_uiReadBytes = 0;
	m_dwOffset = 0;
	m_lLockSize = 0;

	++m_uiInstances_;
	return true;
}

bool CASIn::ResetBuffer()
{
	if( !CAS::ResetBuffer() )
		return false;

	if( FAILED( m_poDSCaptureBuffer->Lock(	0, 0,
					&m_pvTransfer0, &m_dwLength0,
					&m_pvTransfer1, &m_dwLength1,
					DSBLOCK_FROMWRITECURSOR | DSBLOCK_ENTIREBUFFER ) ) )
		return false;
	if( m_pvTransfer0 )
		ZeroMemory( m_pvTransfer0, m_dwLength0 );
	if( m_pvTransfer1 )
		ZeroMemory( m_pvTransfer1, m_dwLength1 );
	m_poDSCaptureBuffer->Unlock( m_pvTransfer0, m_dwLength0, m_pvTransfer1, m_dwLength1 );

	return true;
}

bool CASIn::Clear()
{
	SAFE_RELEASE( m_poDSCaptureBuffer );
	if ( m_uiInstances_ > 0 )
		if ( --m_uiInstances_ == 0 )
		{
			SAFE_RELEASE( m_poDSCaptureDev_ );
		}
	ClearBuffer();
	return true;
}

bool CASIn::Start()
{
	if( FAILED( m_poDSCaptureBuffer->Start( DSCBSTART_LOOPING ) ) )
		return false;
	return true;
}

bool CASIn::Stop()
{
	if( FAILED( m_poDSCaptureBuffer->Stop() ) )
		return false;
	return true;
}

unsigned int CASIn::Read() {
	return Read( m_uiBufferSamples );
}

unsigned int CASIn::Read( unsigned int uiSamples )
{
	const unsigned int uiBufferSize = uiSamples * m_oWaveFormatEx.nBlockAlign;

	// rem: Bis zum ReadCursor ist es sicher zu lesen.
	if( FAILED( m_poDSCaptureBuffer->GetCurrentPosition( 0, &m_dwPosRead ) ) )
		return false;

	m_lLockSize = m_dwPosRead - m_dwOffset;
	if( m_lLockSize < 0 )
		m_lLockSize += uiBufferSize;

	if( m_lLockSize == 0 )
		return false;

	if( FAILED( m_poDSCaptureBuffer->Lock(	m_dwOffset, m_lLockSize,
						&m_pvTransfer0, &m_dwLength0,
						&m_pvTransfer1, &m_dwLength1, 0L ) ) )
		return false;

	m_dwOffset += m_dwLength0;
	m_dwOffset %= m_uiBufferSize;
	CopyMemory( &m_ptBuffer[0], m_pvTransfer0, m_dwLength0 );

	if( m_pvTransfer1 )
	{
		CopyMemory( &m_ptBuffer[m_dwLength0], m_pvTransfer1, m_dwLength1 );
		m_dwOffset += m_dwLength1;
		m_dwOffset %= m_uiBufferSize;
	}

	m_poDSCaptureBuffer->Unlock( m_pvTransfer0, m_dwLength0, m_pvTransfer1, m_dwLength1 );
	m_uiReadBytes = m_dwLength0 + m_dwLength1;
	return m_uiReadBytes / m_oWaveFormatEx.nBlockAlign;
}

#endif // platform
