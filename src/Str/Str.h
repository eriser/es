// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Str.h
//
// Todo: 
// - Konstruktor für Zahlen (float, double, int, unsigned int...) überladen!
// - regex
// - Besseres Interface. Konvention mit Pos, Size entweder verallgemeinern, oder entfernen.
// - Von hinten her suchen.

#ifndef _STR_H_
#define _STR_H_

#define STR_VERSION	"0.15" // 2009-08-13

//#define STR_USE_OSTREAM
//#define STR_USE_SERIALIZE


#ifdef STR_USE_OSTREAM
#include <ostream>
using std::ostream;
#endif // STR_USE_OSTREAM

#ifdef STR_USE_SERIALIZE
#include "../File/File.h"
#endif // STR_USE_SERIALIZE

// String
////////////////////////////////////////////////////////////////////////////////
class CStr
{
public:
	CStr();
	CStr( const char cChar );
	CStr( const char *pcStr );
	CStr( unsigned int uiSize );				// Reserviert uiSize + 1 Bytes.
	CStr( const CStr &roStr );
	~CStr();
	
public:
	CStr &	operator = ( const CStr &roStr );
		operator char * ();
		operator char * () const;
		operator const char * ();
		operator const char * () const;
#ifdef __GNUC__
	char &	operator [] ( int iIndex );
	char	operator [] ( int iIndex ) const;
#else
	char &	operator [] ( unsigned int uiIndex );
	char	operator [] ( unsigned int uiIndex ) const;
#endif // __GNUC__
	CStr	operator +  ( const char cChar ) const;
	CStr &	operator += ( const char cChar );
	CStr	operator +  ( const CStr &roStr ) const;
	CStr &	operator += ( const CStr &roStr );
	CStr	operator +  ( const char * pcStr ) const;
	CStr &	operator += ( const char * pcStr );
	int	operator == ( const CStr & roStr ) const;
	int	operator == ( const char * pcStr ) const;
	int	operator != ( const CStr & roStr ) const;
	int	operator != ( const char * pcStr ) const;
	
public:
	char &	At( unsigned int uiIndex );
	char	At( unsigned int uiIndex ) const;
	
	void Clear(); // Auf "" zurücksetzen.
	void Clear( unsigned int uiSize ); // ClearAndSetSizeAndZero().
	
	void Append( const CStr &roStr );
	void Append( const char * pcStr );
	void Append( const char cChar );
	
	void Pop();
	void Del( unsigned int uiStart, unsigned int uiSize ); // rem: Bis und ohne!
	void Sub( unsigned int uiStart, unsigned int uiSize );
	CStr GetPop() const;
	CStr GetDel( unsigned int uiStart, unsigned int uiSize ) const;
	CStr GetSub( unsigned int uiStart, unsigned int uiSize ) const;
	
	int Find( unsigned int uiStart, char cChar ) const;
	int FindInv( unsigned int uiStart, char cChar ) const; // Position des ersten Zeichens, das ungleich cChar enthalten ist.
	int Find( unsigned int uiStart, const char * pcStr ) const;
	int Find( unsigned int uiStart, const CStr & roStr ) const;
	int FindVec( unsigned int uiStart, const char * pcStr ) const; // Position des ersten Zeichens, das in pcStr enthalten ist.
	int FindVec( unsigned int uiStart, const CStr & roStr ) const;
	int FindVecInv( unsigned int uiStart, const char * pcStr ) const; // Position des ersten Zeichens, das nicht in pcStr enthalten ist.
	int FindVecInv( unsigned int uiStart, const CStr & roStr ) const; // Siehe oben.
	
	// new
	int FindRev( unsigned int uiStart, char cChar ) const;
	int FindInvRev( unsigned int uiStart, char cChar ) const;
	int FindRev( unsigned int uiStart, const char * pcStr ) const;
	int FindRev( unsigned int uiStart, const CStr & roStr ) const;
	int FindVecRev( unsigned int uiStart, const char * pcStr ) const;
	int FindVecRev( unsigned int uiStart, const CStr & roStr ) const;
	int FindVecInvRev( unsigned int uiStart, const char * pcStr ) const;
	int FindVecInvRev( unsigned int uiStart, const CStr & roStr ) const;
	
	int Replace( char cOld, char cNew );
	int Replace( const char * pcOld, const char * pcNew );
	int Replace( const CStr & roOld, const CStr & roNew );
	
	// new
	void ToLower();
	void ToUpper();
	
	void Fill( const char cChar );
	
#ifdef STR_USE_SERIALIZE
	bool SerialRead( CFile * poFile );
	bool SerialWrite( CFile * poFile ) const;
#endif // STR_USE_SERIALIZE

public:
	inline unsigned int GetSize() const;			// Ohne Null-Terminator!
	inline const char*  GetData() const;		
	
protected:
	void Init( unsigned int uiSize );
	void Zero();						// Setzt alle Bytes auf 0!
	
	static void Copy_( char *pcDst, const char *pcSrc, 
			  unsigned int uiSize );		// uiSize Bytes kopieren (memcpy).
	static int Compare_( 
		const char *pcS0, const char *pcS1 );		// Vergleich (strcmp).
	static bool Match_( 
		const char *pcTarget, const char *pcPattern ); 	// Vergleich bis zu best. Index.
	static unsigned int Length_( const char *pcStr );	// Länge (strlen).
	
	// new
	static inline bool IsLower_( char cChar );
	static inline bool IsUpper_( char cChar );
	static inline char ToLower_( char cChar );
	static inline char ToUpper_( char cChar );
	
protected:
	char *m_pcData;
	unsigned int m_uiSize;					// Gibt die tatsächliche Länge mit Null-Terminator an!
	
	
	friend CStr	operator +  ( const char * pcStr, const CStr &roStr );
	friend int	operator == ( const char * pcStr, const CStr &roStr );
#ifdef STR_USE_OSTREAM
	friend ostream& operator<<( ostream &roOStream, CStr &roStr );
#endif // STR_USE_OSTREAM
};

CStr	operator +  ( const char * pcStr, const CStr &roStr );
int	operator == ( const char * pcStr, const CStr &roStr );
#ifdef STR_USE_OSTREAM
ostream& operator << ( ostream &roOStream, CStr &roStr );
#endif // STR_USE_OSTREAM


#include "Str.inline.h"


////////////////////////////////////////////////////////////////////////////////
// HELP
////////////////////////////////////////////////////////////////////////////////

/*
#include <iostream>

static void PrintLargeString_( CStr &roStr )
{
	const unsigned int uiChunkSize = 256;
	const unsigned int uiStringSize = roStr.GetSize();
	const unsigned int uiChunkCount = uiStringSize / uiChunkSize;
	unsigned int uiPos = 0;
	
	for( unsigned int uiChunk=0; uiChunk<uiChunkCount; uiChunk++, uiPos += uiChunkSize )
	{
		std::cout << roStr.GetSub( uiPos, uiChunkSize ).GetData();
	}
	std::cout << roStr.GetSub( uiPos, uiStringSize - uiChunkCount * uiChunkSize ).GetData();
	std::cout << std::endl << std::flush;
}
*/

#endif // _STR_H_
