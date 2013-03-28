// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FileStream.h

#ifndef _FILE_STREAM_H_
#define _FILE_STREAM_H_

#include "File.h"

// Basisklasse aller Stream-Dateien
//
// - Eine Stream-Datei wird �ber die Methoden Read() und Write() immer direkt
//   aus der physischen Datei gelesen. Informationen �ber das Format m�ssen
//   meist nicht schon beim �ffnen mit Open() bekannt sein, sondern werden
//   automatisch bei einem abschliessenden Close() erg�nzt.
////////////////////////////////////////////////////////////////////////////////
class CFileStream : public CFile
{
public:
	CFileStream();
	CFileStream( const CFileStream &roO );
	~CFileStream();
	CFileStream & operator=( const CFileStream &roO );
};

#endif // _FILE_STREAM_H_
