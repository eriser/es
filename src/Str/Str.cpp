// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Str.cpp

#include "Str.h"

////////////////////////////////////////////////////////////////////////////////
// CStr
////////////////////////////////////////////////////////////////////////////////

CStr::CStr()
: m_pcData( 0 )
{
	Clear();
}

CStr::CStr( const char cChar )
{
	Init( 1 );
	m_pcData[0] = cChar;
	m_pcData[1] = '\0';
}

CStr::CStr( unsigned int uiSize )
{
	Init( uiSize );
	Zero();
}


CStr::CStr( const char * pcStr )
{
	unsigned int uiSize = Length_( pcStr );
	Init( uiSize );                  // uiSize + 1 Plätze
	Copy_( m_pcData, pcStr, uiSize ); // Null-Terminator muss nicht kopiert werden.
	m_pcData[uiSize] = '\0';
}

CStr::CStr( const CStr &roStr )
: m_pcData( 0 )
{
	*this = roStr;
}

CStr::~CStr()
{
	if( m_pcData )
		delete [] m_pcData;
}

CStr &	CStr::operator=( const CStr &roStr )
{
	const unsigned int uiSize = roStr.GetSize(); // rem: Ist schon -1!
	if( m_pcData )
		delete [] m_pcData;
	Init( uiSize ); 
	Copy_( m_pcData, roStr.m_pcData, uiSize );
	m_pcData[uiSize] = '\0';
	return *this;
}

CStr::operator char *()
{
	return m_pcData;
}

CStr::operator const char *()
{
	return m_pcData;
}

CStr::operator char *() const
{
	return m_pcData;
}

CStr::operator const char *() const
{
	return m_pcData;
}

#ifdef __GNUC__
char &	CStr::operator[]( int iIndex )
{
	return m_pcData[iIndex];
}

char CStr::operator[]( int iIndex ) const
{
	return m_pcData[iIndex];
}
#else
char &	CStr::operator[]( unsigned int uiIndex )
{
	return m_pcData[uiIndex];
}

char CStr::operator[]( unsigned int uiIndex ) const
{
	return m_pcData[uiIndex];
}
#endif // __GNUC__


CStr CStr::operator+( const CStr &roStr ) const
{
	const unsigned int uiSize0 = m_uiSize - 1;
	const unsigned int uiSize1 = roStr.m_uiSize - 1;
	CStr oStr( uiSize0 + uiSize1 );
	Copy_( oStr.m_pcData,           m_pcData,       uiSize0 );
	Copy_( oStr.m_pcData + uiSize0, roStr.m_pcData, uiSize1 );
	oStr.m_pcData[oStr.m_uiSize-1] = '\0'; 
	return oStr;
}

CStr CStr::operator+( const char cChar ) const
{
	return *this + CStr( cChar );
}

CStr & CStr::operator+=( const char cChar )
{
	return *this += CStr( cChar );
}

CStr &	CStr::operator+=( const CStr &roStr )
{
	return *this = *this + roStr;
}

CStr CStr::operator+( const char * pcStr ) const
{
	return *this + CStr( pcStr );
}

CStr &	CStr::operator+=( const char * pcStr )
{
	return *this = *this + CStr( pcStr );
}

int CStr::operator==( const CStr & roStr ) const
{
	return ( Compare_( m_pcData, roStr.m_pcData ) == 0 );
}

int CStr::operator==( const char * pcStr ) const
{
	return ( Compare_( m_pcData, pcStr ) == 0 );
}

int CStr::operator!=( const CStr & roStr ) const
{
	return ( Compare_( m_pcData, roStr.m_pcData ) != 0 );
}

int CStr::operator!=( const char * pcStr ) const
{
	return ( Compare_( m_pcData, pcStr ) != 0 );
}

char & CStr::At( unsigned int uiIndex )
{
	return m_pcData[uiIndex];
}

char CStr::At( unsigned int uiIndex ) const
{
	return m_pcData[uiIndex];
}

void CStr::Clear()
{
	if( m_pcData )
		delete [] m_pcData;
	Init( 0 );
	m_pcData[0] = '\0';
}

void CStr::Clear( unsigned int uiSize )
{
	if( uiSize != GetSize() )
	{
		Clear();
		Init( uiSize );
	}
	Zero();
}

void CStr::Append( const CStr &roStr )
{
	*this += roStr;
}

void CStr::Append( const char * pcStr )
{
	*this += pcStr;
}

void CStr::Append( const char cChar )
{
	*this += cChar;
}

void CStr::Pop()
{
	// rem: Wird aber richtig gelöscht!
	if( m_uiSize > 1 )
	{
		--m_uiSize;
		m_pcData[m_uiSize-1] = '\0';
	}
}

/*
void CStr::Del( unsigned int uiStart, unsigned int uiSize )
{
	const unsigned int uiEnd = uiStart + uiSize;
	if( uiEnd && uiEnd <= m_uiSize )
	{
		const char *pcDataOld = m_pcData;
		const unsigned int uiSizeOld = m_uiSize;
		
		Init( m_uiSize - uiSize );
		Copy_( m_pcData, pcDataOld, uiStart );
		Copy_( m_pcData + uiStart, pcDataOld + uiEnd, uiSizeOld - uiEnd ); // '\0' wird gleich mitkopiert!
		m_pcData[m_uiSize-1] = '\0';
		delete [] pcDataOld;
	}
}
*/

void CStr::Del( unsigned int uiStart, unsigned int uiSize )
{
	const unsigned int uiEnd = uiStart + uiSize;
	if( uiEnd && uiEnd <= m_uiSize - 1 ) // MOD: - 1
	{
		const char *pcDataOld = m_pcData;
		const unsigned int uiSizeOld = m_uiSize;
		
		Init( m_uiSize - uiSize - 1 ); // MOD: - 1
		Copy_( m_pcData, pcDataOld, uiStart );
		Copy_( m_pcData + uiStart, pcDataOld + uiEnd, uiSizeOld - uiEnd ); // '\0' wird gleich mitkopiert!
		m_pcData[m_uiSize-1] = '\0';
		delete [] pcDataOld;
	}
}

void CStr::Sub( unsigned int uiStart, unsigned int uiSize )
{
	if( uiStart )
		Del( 0, uiStart );
	Del( uiSize, m_uiSize-uiSize-1 ); // MOD:  - 1
}

CStr CStr::GetPop() const
{
	CStr oStr( *this );
	oStr.Pop();
	return oStr;
}

CStr CStr::GetDel( unsigned int uiStart, unsigned int uiSize ) const
{
	CStr oStr( *this );
	oStr.Del ( uiStart, uiSize );
	return oStr;
}

CStr CStr::GetSub( unsigned int uiStart, unsigned int uiSize ) const
{
	CStr oStr( *this );
	oStr.Sub( uiStart, uiSize );
	return oStr;
}

int CStr::Find( unsigned int uiStart, char cChar ) const
{
	unsigned int i = uiStart;
	while( i < m_uiSize )
	{
		if ( m_pcData[i] == cChar )
			return i;
		++i;
	}
	return -1;
}

int CStr::FindInv( unsigned int uiStart, char cChar ) const
{
	unsigned int i = uiStart;
	while( i < m_uiSize )
	{
		if ( m_pcData[i] != cChar )
			return i;
		++i;
	}
	return -1;
}

int CStr::Find( unsigned int uiStart, const char * pcStr ) const
{
	const unsigned int uiLen = Length_( pcStr );
	
	if( !uiLen )
		return -1;
	if( uiLen == 1 )
		return Find( uiStart, pcStr[0] );
	
	const unsigned int uiMax = m_uiSize - uiLen; // ACHTUNG: Mod: vorher -1!
	unsigned int i = uiStart;
	
	while( i < uiMax )
	{
		const int iFound = Find( i, pcStr[0] );
		if( iFound < 0 )
		{
			break;
		}
		if( Match_( m_pcData + iFound + 1, pcStr + 1 ) )
		{
			return iFound;
		}
		++i;
	}
	return -1;
}

int CStr::Find( unsigned int uiStart, const CStr & roStr ) const {
	return Find( uiStart, roStr.m_pcData );
}

int CStr::FindVec( unsigned int uiStart, const char * pcStr ) const
{
	const unsigned int uiLen = Length_( pcStr );
	
	unsigned int i = uiStart;
	while( i < m_uiSize )
	{
		for( unsigned int c=0; c<uiLen; ++c )
		{
			if ( m_pcData[i] == pcStr[c] )
				return i;
		}
		++i;
	}
	return -1;
}

int CStr::FindVec( unsigned int uiStart, const CStr & roStr ) const {
	return FindVec( uiStart, roStr.GetData() );
}

int CStr::FindVecInv( unsigned int uiStart, const char * pcStr ) const 
{
	const unsigned int uiLen = Length_( pcStr );
	
	unsigned int i = uiStart;
	while( i < m_uiSize )
	{
		unsigned int c = 0;
		while( c < uiLen )
		{
			if ( m_pcData[i] == pcStr[c] )
				break;
			++c;
		}
		if( c == uiLen )
			return i;
		++i;
	}
	return -1;
}

int CStr::FindVecInv( unsigned int uiStart, const CStr & roStr ) const {
	return FindVecInv( uiStart, roStr.m_pcData );
}


// <new_0.11>

int CStr::FindRev( unsigned int uiStart, char cChar ) const
{
	unsigned int i = uiStart + 1;
	while( i )
	{
		--i;
		if( m_pcData[i] == cChar )
			return i;
	}
	return -1;
}

int CStr::FindInvRev( unsigned int uiStart, char cChar ) const
{
	unsigned int i = uiStart + 1;
	while( i )
	{
		--i;
		if ( m_pcData[i] != cChar )
			return i;
	}
	return -1;
}

int CStr::FindRev( unsigned int uiStart, const char * pcStr ) const
{
	const unsigned int uiLen = Length_( pcStr );
	
	if( !uiLen )
		return -1;
	if( uiLen == 1 )
		return Find( uiStart, pcStr[0] );
	
	const unsigned int uiMax = m_uiSize - 1 - uiLen;
	if( uiStart > uiMax )
		uiStart = uiMax;
	unsigned int i = uiStart + 1;
	
	while( i )
	{
		--i;
		const int iFound = FindRev( i, pcStr[0] );
		if( iFound < 0 )
		{
			break;
		}
		if( Match_( m_pcData + iFound + 1, pcStr + 1 ) )
		{
			return iFound;
		}
	}
	return -1;
}

int CStr::FindRev( unsigned int uiStart, const CStr & roStr ) const {
	return FindRev( uiStart, roStr.m_pcData );
}

int CStr::FindVecRev( unsigned int uiStart, const char * pcStr ) const
{
	const unsigned int uiLen = Length_( pcStr );
	
	unsigned int i = uiStart + 1;
	while( i )
	{
		--i;
		for( unsigned int c=0; c<uiLen; ++c )
		{
			if ( m_pcData[i] == pcStr[c] )
				return i;
		}
	}
	return -1;
}

int CStr::FindVecRev( unsigned int uiStart, const CStr & roStr ) const {
	return FindVecRev( uiStart, roStr.GetData() );
}

int CStr::FindVecInvRev( unsigned int uiStart, const char * pcStr ) const 
{
	const unsigned int uiLen = Length_( pcStr );
	
	unsigned int i = uiStart + 1;
	while( i )
	{
		--i;
		unsigned int c = 0;
		while( c < uiLen )
		{
			if ( m_pcData[i] == pcStr[c] )
				break;
			++c;
		}
		if( c == uiLen )
			return i;
	}
	return -1;
}

int CStr::FindVecInvRev( unsigned int uiStart, const CStr & roStr ) const {
	return FindVecInvRev( uiStart, roStr.m_pcData );
}

int CStr::Replace( char cOld, char cNew )
{
	int iReplaced = 0;
	char * pcEnd = m_pcData + m_uiSize;
	char * pcD = m_pcData;
	while( pcD < pcEnd )
	{
		if( *pcD == cOld )
		{
			*pcD = cNew;
			++iReplaced;
		}
		++pcD;
	}
	return iReplaced;
}

int CStr::Replace( const char * pcOld, const char * pcNew )
{
	int iReplaced = 0;
	CStr oTmp;
	const char cFirst = *pcOld;
	const unsigned int uiLenOld = Length_( pcOld );
	char * pcEnd = m_pcData + m_uiSize;
	char * pcD = m_pcData;
	while( pcD < pcEnd )
	{
		if( *pcD == cFirst )
		{
			if( Match_( pcD, pcOld ) )
			{
				oTmp += pcNew;
				pcD += uiLenOld;
				++iReplaced;
				continue;
			}
		}
		oTmp += *pcD;
		++pcD;
	}
	*this = oTmp;
	return iReplaced;
}

int CStr::Replace( const CStr & roOld, const CStr & roNew )
{
	return Replace( roOld.m_pcData, roNew.m_pcData );
}

void CStr::ToLower()
{
	unsigned int i = m_uiSize - 1;
	while( i )
	{
		--i;
		const char cChar = m_pcData[i];
		if( IsUpper_( cChar ) )
		{
			m_pcData[i] = ToLower_( m_pcData[i] );
		}
	}
}

void CStr::ToUpper()
{
	unsigned int i = m_uiSize - 1;
	while( i )
	{
		--i;
		const char cChar = m_pcData[i];
		if( IsLower_( cChar ) )
		{
			m_pcData[i] = ToUpper_( m_pcData[i] );
		}
	}
}

void CStr::Fill( const char cChar )
{
	unsigned int i = m_uiSize - 1;
	while( i )
	{
		--i;
		m_pcData[i] = cChar;
	}
}

#ifdef STR_USE_SERIALIZE
bool CStr::SerialRead( CFile * poFile )
{
	unsigned int uiSize;
	poFile->Read( (void *)&uiSize, sizeof( uiSize ) );
	Clear( uiSize );
	poFile->Read( (void *)m_pcData, uiSize * sizeof( char ) );
	return true;
}

bool CStr::SerialWrite( CFile * poFile ) const
{
	const unsigned int uiSize( m_uiSize - 1 );
	poFile->Write( (void *)&uiSize, sizeof( uiSize ) );
	poFile->Write( (void *)m_pcData, uiSize * sizeof( char ) );
	return true;
}
#endif // STR_USE_SERIALIZE

void CStr::Init( unsigned int uiSize )
{
	++uiSize; // Null-Terminator!
	m_uiSize = uiSize;
	m_pcData = new char[m_uiSize];
}

void CStr::Zero()
{
	// ACHTUNG: memset/bzero wäre schneller!
	unsigned int uiSize = m_uiSize;
	while( uiSize )
	{
		--uiSize;
		m_pcData[uiSize] = '\0';
	}	
}

void CStr::Copy_( char *pcDst, const char *pcSrc, unsigned int uiSize )
{	
	if( uiSize )
	{
		// ACHTUNG: memcpy/strcpy wäre schneller.
		do
		{
			*pcDst++ = *pcSrc++;
		}
		while( --uiSize );
	}
}

int CStr::Compare_( const char *pcS0, const char *pcS1 )
{
	while( *pcS0 != '\0' && *pcS0 == *pcS1 )
	{
		++pcS0;
		++pcS1;
	}

	return (*(unsigned char *) pcS0) - (*(unsigned char *) pcS1);
}

bool CStr::Match_( const char *pcTarget, const char *pcPattern )
{
	while( *pcPattern != '\0' )
	{
		if( *(unsigned char *)pcTarget != *(unsigned char *)pcPattern )
		{
			return false;
		}
		++pcTarget;
		++pcPattern;
	}
	return true;
}

unsigned int CStr::Length_( const char *pcStr )
{
	unsigned int uiSize = 0;
	while( pcStr[uiSize] != '\0' )
	{
		++uiSize;
	}
	return uiSize;
}

// Global //

CStr operator+( const char * pcStr, const CStr &roStr )
{
	return CStr( pcStr ) + roStr;
}

int operator == ( const char * pcStr, const CStr &roStr )
{
	return ( roStr.Compare_( roStr.m_pcData, pcStr ) == 0 );
}

#ifdef STR_USE_OSTREAM
ostream& operator<<( ostream &roOStream, CStr &roStr )
{
	roOStream << roStr.GetData();
	return roOStream;
}
#endif // STR_USE_OSTREAM

