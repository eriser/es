// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FileBlockImg.cpp

#include "FileBlockImg.h"

////////////////////////////////////////////////////////////////////////////////
// CFileBlockImg
////////////////////////////////////////////////////////////////////////////////

CFileBlockImg::CFileBlockImg()
: CFileBlock()
, m_uiWidth( 0 )
, m_uiHeight( 0 )
, m_uiChannels( 0 )
{}

CFileBlockImg::CFileBlockImg( const CFileBlockImg &roO )
: CFileBlock()
{
	*this = roO;
}

CFileBlockImg::~CFileBlockImg()
{
	Clear();
}

CFileBlockImg & CFileBlockImg::operator=( const CFileBlockImg &roO )
{
	CFileBlock::operator=( roO );
	m_uiWidth = roO.m_uiWidth;
	m_uiHeight = roO.m_uiHeight;
	m_uiChannels = roO.m_uiChannels;
	return *this;
}

void CFileBlockImg::Clear()
{
	CFileBlock::Clear();
	m_uiWidth = m_uiHeight = m_uiChannels = 0;
}
