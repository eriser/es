// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// FileBlock.cpp

#ifndef _FILE_BLOCK_CPP_
#define _FILE_BLOCK_CPP_

#include "FileBlock.h"

////////////////////////////////////////////////////////////////////////////////
// CFileBlock
////////////////////////////////////////////////////////////////////////////////

CFileBlock::CFileBlock()
: CFile()
, m_uiSize( 0 )
, m_pucData( 0 )
{}

CFileBlock::CFileBlock( const CFileBlock &roO )
: CFile()
, m_uiSize( 0 )
, m_pucData( 0 )
{
	*this = roO;
}

CFileBlock::~CFileBlock()
{
	Clear();
}

CFileBlock & CFileBlock::operator=( const CFileBlock &roO )
{
	CFile::operator=( roO );
	Init( roO.m_uiSize );
	FILE_MEM_COPY( m_pucData, roO.m_pucData, m_uiSize );
	return *this;
}

bool CFileBlock::Load( const char *acFileName )
{
	if( CFile::Open( acFileName, FLAG_READ ) )
	{
		const unsigned int uiSize = CFile::GetSize();
		Init( uiSize );
		CFile::Read( (void *)m_pucData, uiSize );
		return CFile::Close();
	}
	return false;
}

bool CFileBlock::Save( const char *acFileName )
{
	if( CFile::Open( acFileName, FLAG_WRITE | FLAG_TRUNC ) )
	{
		CFile::Write( (void *)m_pucData, m_uiSize );
		return CFile::Close();
	}
	return false;
}

void CFileBlock::Init( unsigned int uiSize )
{
	Clear();
	m_pucData = new unsigned char[uiSize];
	m_uiSize = uiSize;
}

void CFileBlock::Clear()
{
	if( m_pucData )
		delete [] m_pucData;
	m_pucData = 0;
	m_uiSize = 0;
}

#endif // _FILE_BLOCK_CPP_
