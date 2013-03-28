// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FileBlockImgBmp.h

#ifndef _FILE_BLOCK_IMG_BMP_H_
#define _FILE_BLOCK_IMG_BMP_H_

#include "FileBlockImg.h"

// 24-/32-Bit Bitmap-Datei
//
// - Beim schreiben �ber Set() werden m_uiWidth/m_uiHeight folgendermassen
//   interpretiert:
//   Ist eine Wert der beiden gleich 0, so wird dieser enstprechend der
//   �bergebenen Datenmenge angepasst, so dass alle Daten im Bild platz finden.
//   Sind beide gleich 0, so werden die Masse  ungef�hr quadratisch 
//   gew�hlt, die H�he ist dabei unter Umst�nden etwas gr�sser als die Breite. 
//   �berschreitet die gew�nschte Bildgr�sse  die Datengr�sse, 
//   wird false zur�ckgegeben.
// - Merke: In m_pucData befindet sich die ganze Datei, nich nur die Bilddaten,
//   wie bei den anderen CFileBlockImg* Klassen. Das ist hier effizienter.
//   Deshalb sollte von aussen niemals direkt auf m_pucData, da sich der Inhalt
//   von Klasse zu Klasse unterscheidet. CFileBlock::m_pucData ist nun pro-
//   tected.
//
////////////////////////////////////////////////////////////////////////////////
class CFileBlockImgBmp : public CFileBlockImg
{
public:
	CFileBlockImgBmp();
	//CFileBlockImgBmp( const CFileBlockImgBmp &roO );
	virtual ~CFileBlockImgBmp();
	//CFileBlockImgBmp & operator=( const CFileBlockImgBmp &roO );
public:
	virtual bool Load( const char *pcFileName );
	virtual bool Decode( void *pvData, unsigned int uiBytes );
	virtual bool Encode( void *pvData, unsigned int uiBytes ); // Vorher m_uiWidth, m_uiHeight, m_uiChannels setzen.
	virtual void Clear();
};

#endif // _FILE_BLOCK_IMG_BMP_H_
