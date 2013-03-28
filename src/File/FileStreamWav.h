// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FileStreamWav.h

#ifndef _FILE_STREAM_WAV_H_
#define _FILE_STREAM_WAV_H_

#include "FileStream.h"

// 16-Bit Wave File
// - Leere Dateien werden bei CFile::FLAG_READ | CFile::FLAG_WRITE mit
//   gültigen Header und den nachfolgenden Daten überschrieben, bei allen
//   anderen Dateien wird zuerst der Header überprüft, und falls gültig, der
//   File-Pointer ganz ans Ende der Datei verschoben, folgende Daten also
//   an das bestehende File angehängt werden. Ist der Header ungültig, so
//   gibt Open() false zurück.
// - Um jede bestehende oder neue Datei zu beschreiben, genügt es Open()
//   mit CFile::FLAG_WRITE | CFile::FLAG_TRUNC zu öffnen.
// - Loop Informationen ('smpl' chunk) werden gelesen, aber nicht geschrieben.
////////////////////////////////////////////////////////////////////////////////
class CFileStreamWav : public CFileStream
{
public:
	enum { LOOP_NONE, LOOP_FORWARD, LOOP_PINGPONG, LOOP_BACKWARD };
public:
	CFileStreamWav();
	CFileStreamWav( const CFileStreamWav &roO );
	~CFileStreamWav();
	CFileStreamWav & operator=( const CFileStreamWav &roO );
public:
	bool Open( const char * pcFileName, unsigned int uiFlags );
	// <mod date="2010-12-10">
	unsigned int ReadValues( signed short int *pssiData, unsigned int uiCount );
	unsigned int WriteValues( const signed short int *pssiData, unsigned int uiCount );
	// </mod>
	bool Close();
public:
	unsigned int m_uiSampleFreq;	// Muss vor Open()/Schreiboperation gesetzt werden.
	unsigned int m_uiChannels;	// "
	unsigned int m_uiDataBytes;
	
	unsigned int m_uiMIDIRootKey;
	unsigned int m_uiLoopType;
	unsigned int m_uiLoopStart;
	unsigned int m_uiLoopEnd;
	unsigned int m_uiLoopPlayCount;		
};

#endif // _FILE_STREAM_WAV_H_
