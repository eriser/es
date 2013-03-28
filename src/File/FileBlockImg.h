// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FileBlockImg.h

#ifndef _FILE_BLOCK_IMG_H_
#define _FILE_BLOCK_IMG_H_

#include "FileBlock.h"

// Bild-Datei
////////////////////////////////////////////////////////////////////////////////
class CFileBlockImg : public CFileBlock
{
public:
	CFileBlockImg();
	CFileBlockImg( const CFileBlockImg &roO );
	virtual ~CFileBlockImg();
	CFileBlockImg & operator=( const CFileBlockImg &roO );
public:
	virtual bool Decode( void *pvData, unsigned int uiBytes ) = 0;
	virtual bool Encode( void *pvData, unsigned int uiBytes ) = 0;
	virtual void Clear();
public:
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
	unsigned int m_uiChannels;
};

#endif // _FILE_BLOCK_IMG_H_
