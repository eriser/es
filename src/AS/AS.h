// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// AS.h
/*
rem:

- Value:
  A value is a single value that describes the amplitude of the audio
  signal at a single point in time, on a single channel.
  (ALSA terminology: sample)

- Sample:
  Data that represents all channels at a single point in time. This is a
  collection of samples, one per channel.
  (ALSA terminology: frame)

- Fragments:
  Fragments are the number of equal-sized chunks the full audio buffer
  gets divided into, and alsa calls them periods.
*/

#ifndef _AS_H_
#define _AS_H_

#define AS_VERSION		"0.18" // 2008-02-01
#define AS_USE_ERROR
#define AS_USE_LOG

#ifdef AS_USE_LOG
#define AS_LOG(...)		LOG( "AS_LOG: " __VA_ARGS__ )
#else
#define AS_LOG(...)
#endif // AS_USE_LOG

#ifdef AS_USE_ERROR
#include <stdio.h>
#define AS_ERROR(...)		fprintf( stderr, "AS_ERROR: " __VA_ARGS__ );
#define AS_ERROR_RET(...)	{ fprintf( stderr, "AS_ERROR: " __VA_ARGS__ ); return false; }
#else
#define AS_ERROR(...)
#define AS_ERROR_RET(...)	{ return false; }
#endif // AS_USE_ERROR


// Audio Stream Base Class.
////////////////////////////////////////////////////////////////////////////////
class CAS
{
public:
	typedef signed short int TValue;
public:
	CAS();
	~CAS();
	void SetFormat(
		unsigned int uiChannels,
		unsigned int uiSampleFreq,
		unsigned int uiSampleDepth,
		unsigned int uiBufferSamples );
	bool ResetBuffer();

	TValue *GetBuffer() const;
	unsigned int GetSampleFreq() const;
	unsigned int GetBufferSamples() const;
protected:
	void CreateBuffer();
	void ClearBuffer();
protected:
	TValue *m_ptBuffer;
	unsigned int m_uiChannels;
	unsigned int m_uiSampleFreq;
	unsigned int m_uiSampleDepth;
	unsigned int m_uiBufferSamples;
};


#if defined( __LINUX__ )

// IMPORTANT!
#define AS_ALSA_DEV_IN	"plughw:0,0" // "default"
#define AS_ALSA_DEV_OUT	"plughw:0,0" // "default" //"file:'./out.raw',raw"

// rem:
// http://soundprogramming.net/programming_apis/alsa_tutorial_1_initialization
// aplay -c 2 -t raw -r 48000 --format=S16_LE ./out.raw
// "default" // "hw:0,0" // "plughw:0,0" // "file:'./out.raw',raw"

#include <alsa/asoundlib.h>

// ALSA Device.
// rem: Prepare() im Duplex-Modus nicht aufrufen.
////////////////////////////////////////////////////////////////////////////////
class CASDeviceAlsa : public CAS
{
public:
	CASDeviceAlsa();
	~CASDeviceAlsa();

	bool Start();
	bool Stop();
protected:
	bool Init( const char *pcDev, bool bIsOutput );
	void Clear();
public:
	//unsigned int GetFragmentsCout() const;
protected:
	snd_pcm_t *m_poHandle;
	snd_pcm_hw_params_t *m_poHW;
	snd_pcm_sw_params_t *m_poSW;
	//unsigned int m_uiFragments;
};

// Output Audio Stream (Linux/ALSA).
////////////////////////////////////////////////////////////////////////////////
class CASOut : public CASDeviceAlsa
{
public:
	CASOut();
	~CASOut();
	bool Init();
	//bool ResetBuffer(); // todo
	bool Start();
	unsigned int Prepare();
	bool Write();
	bool Write( unsigned int uiSamples );
};

// Input Audio Stream (Linux/ALSA).
////////////////////////////////////////////////////////////////////////////////
class CASIn : public CASDeviceAlsa
{
public:
	CASIn();
	~CASIn();
public:
	bool Init();
	//bool ResetBuffer(); // todo
	unsigned int Read();
	unsigned int Read( unsigned int uiSamples );
};


#elif defined( __WINDOWS__ )

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>

// DirectX Device.
// rem: Duplex noch nicht möglich.
////////////////////////////////////////////////////////////////////////////////
class CASDeviceDX : public CAS
{
public:
	CASDeviceDX();
	~CASDeviceDX();

	void SetFormat(
		unsigned int uiChannels,
		unsigned int uiSampleFreq,
		unsigned int uiSampleDepth,
		unsigned int uiBufferSamples );

protected:
	WAVEFORMATEX m_oWaveFormatEx;
	unsigned int m_uiBufferSize;

	LPVOID m_pvTransfer0, m_pvTransfer1;
	DWORD m_dwLength0, m_dwLength1;

	DWORD m_dwPosCapture;
	DWORD m_dwPosRead;

	//unsigned int m_uiSleepMs;
};

// Output Audio Stream (Windows/DirectX).
////////////////////////////////////////////////////////////////////////////////
class CASOut : public CASDeviceDX
{
public:
	CASOut();
	~CASOut();
public:
	bool Init();
	bool ResetBuffer();
	bool Start();
	bool Stop();
	unsigned int Prepare();
	bool Write();
	bool Write( unsigned int uiSamples );
protected:
	void Clear();
protected:
	LPDIRECTSOUNDBUFFER m_poDSBuffer;
	DWORD m_dwOffset;
protected:
	static unsigned int m_uiInstances_;
	static LPDIRECTSOUND8 m_poDSDev_;
};

// Input Audio Stream (Windows/DirectX).
////////////////////////////////////////////////////////////////////////////////
class CASIn : public CASDeviceDX
{
public:
	CASIn();
	~CASIn();
public:
	bool Init();
	bool ResetBuffer(); // totest
public:
	bool Start();
	bool Stop();
	unsigned int Read();
	unsigned int Read( unsigned int uiSamples );
protected:
	bool Clear();
public:
	unsigned int m_uiReadBytes;
protected:
	LPDIRECTSOUNDCAPTUREBUFFER m_poDSCaptureBuffer;
	DWORD m_dwOffset;
	LONG m_lLockSize;

protected:
	static unsigned int m_uiInstances_;
	static LPDIRECTSOUNDCAPTURE m_poDSCaptureDev_;
};

#else
#error platform not supported
#endif // platform

#endif // _AS_H_
