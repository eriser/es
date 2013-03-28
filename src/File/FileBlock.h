// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FileBlock.h

#ifndef _FILE_BLOCK_H_
#define _FILE_BLOCK_H_

#include "File.h"

// Basisklasse aller Block-Dateien
//
// - Eine Block-Datei speichert alle Daten zuerst in einem dynamisch 
//   allokierten Array. Es ist somit möglich die Datei als Puffer bestimmter
//   Grösse zur Laufzeit ohne Performance-Verluste zu lesen und zu schrieben.
//   Zugriffe auf die Festplatte werden so minimiert und können effizient in
//   einem Durchgang erledigt werden.
// - Die Methoden aus CFile Open(), Read(), Write() werden hier intern zwar
//   benützt, sollten aber nicht selbst aufgerufen werden. Statt dessen sollte
//   man Save() und Load() verwenden.
// - Save() verwendet FILE_FLAG_WRITE | FILE_FLAG_TRUNC, löscht also eine
//   bereits existierende Datei. Wenn Daten angehängt, eingefügt oder sonstwie
//   manipuliert werden sollen, muss dies "offline". 
// - Um Daten in ein gewünschtes Format zu transformieren, muss jede abge-
//   leitete Klasse eigene Decode-/Encode-Methoden bereitstellen.
////////////////////////////////////////////////////////////////////////////////
class CFileBlock : public CFile
{
public:
	CFileBlock();
	CFileBlock( const CFileBlock &roO );
	virtual ~CFileBlock();
	CFileBlock & operator=( const CFileBlock &roO );
public:
	virtual bool Load( const char *acFileName );
	virtual bool Save( const char *acFileName );
	//virtual bool Decode( void *pvData, unsigned int uiBytes );
	//virtual bool Encode( void *pvData, unsigned int uiBytes );
	virtual void Init( unsigned int uiSize );
	virtual void Clear();
protected:
	unsigned int m_uiSize;
	unsigned char *m_pucData;
};

#endif // _FILE_BLOCK_H_
