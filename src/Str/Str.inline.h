// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Str.inline.h

#ifndef _STR_INLINE_H_
#define _STR_INLINE_H_

inline unsigned int CStr::GetSize() const
{
	return m_uiSize - 1;
}

inline const char *CStr::GetData() const
{
	return m_pcData;
}

inline bool CStr::IsLower_( char cChar ) {
	return ( cChar >= 0x61 && cChar <= 0x7A );
}

inline bool CStr::IsUpper_( char cChar ){
	return ( cChar >= 0x41 && cChar <= 0x5A );
}

inline char CStr::ToLower_( char cChar ) {
	return cChar + ( 0x61 - 0x41 );
}

inline char CStr::ToUpper_( char cChar ){
	return cChar - ( 0x61 - 0x41 );
}

#endif // _STR_INLINE_H_
