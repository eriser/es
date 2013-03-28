// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OrgOp.h
//
// Syntax für Patches:
// -> Jede Zeile entspricht einem Operator.
//
// :class:instance:id:flags:i:id0:port0:flags0:id1:port1:flags1:...id(i-1):port(i-1):flasg(i-1):o:val0:val1:...val(o-1):n:int0:int1:...:int(n-1):u:uint0:uint1:...:uint(u-1):m:flt0:flt1:...:flt(m-1):\n
//                          <----inputs--------------------------------------------------------><---outputs------------><---int-internals-------><---unsigned int-internals-><---float-internals----->
//
// - Die ersten zwei Zeilen entsprechen RootL/R.
// - IDs und Grössenangaben werden im Hexadezimal-Format behandelt (%x-Format).
// - UInts und Ints als Zahlenwerte werden als Deziaml-Zahl behandelt (%u/%d-Format).
// - Fliesskommazahlen werden als Double behandelt (%.16g-Format).
// - Die ID 0 ist ungültig, und wird als Input im Array als Null-Pointer eingetragen.
//

#ifndef _ORG_OP_H_
#define _ORG_OP_H_

#include "../DEF.h"
#include "../List/List.h"
#include "../Array/Array.h"
#include "../Str/Str.h"

#include "../Op/Op.h"

#define	ORG_OP_VERSION				"0.02" // 2010-12-07

#define ORG_OP_LOG( ... )			LOG( __VA_ARGS__ )
#define ORG_OP_SSCANF(	buf, fmt, ... )		SSCANF( buf, fmt, __VA_ARGS__ )
#define ORG_OP_SPRINTF( buf, size, fmt, ... )	SPRINTF( buf, size, fmt, __VA_ARGS__ )

#define ORG_OP_DELIM				':'
#define ORG_OP_NEWLINE				'\n'

#define ORG_OP_CACHE				( OP_SAMPLE_FREQ * 2 )

#define ORG_OP_REGISTER_CLASS( class ) \
	RegisterOpClass( #class, COp##class::Create_ )


// Operator-Organizer (nur eine Instanz, da statische Mechanismen verwendet!)
////////////////////////////////////////////////////////////////////////////////
class COrgOp
{
public:
	typedef COp * ( * TFuncCreate )( void );
	
public:
	COrgOp();
	~COrgOp();
	
public:
	COp * CreateShort( const char *pcClassNameShort );
	COp * Create( const char *pcClassName ); // rem: Objekt wird mit 'new' erstellt!
	bool ValidateAll();
	void UpdateAll();
	void ResetAll();

#if !defined( VERSION_RUNTIME ) && !defined( VERSION_LIB )
	const char * GetNameClass( unsigned int uiClassID );
	const char * GetNameClassShort( unsigned int uiClassID );
	bool Insert( COp * poOp );
	bool Delete( COp * poOp ); // rem: Obj. wird mit 'delete' gelöscht und aus der Liste entfernt!
	
public:
	unsigned int GetCountOp();
	COp * GetOp( unsigned int uiID );
	bool SaveAll( CStr &roDst ); // const
	bool Save( CStr &roDst, COp *poOp );
	bool Save( CStr &roDst, COp **apoOp, unsigned int uiSize );
	
#endif // VERSION_RUNTIME // VERSION_LIB

	bool LoadAll( CStr &roSrc );
	bool Load( CStr &roSrc, unsigned int uiIDOffset );
	void Clear();
	
public:
	static inline void Proc_( double *pdLeft, double *pdRight );

#if !defined( VERSION_RUNTIME ) && !defined( VERSION_LIB )
	static inline COp * GetOpRootLeft_();
	static inline COp * GetOpRootRight_();
	static inline void SetOpRootLeft_( COp *poOp );
	static inline void SetOpRootRight_( COp *poOp );
	
public:
	inline void Rewind();
	inline bool GetNext( COp ** ppoOp );
	
private:
	bool Encode_Op( CStr &roStrDst, COp *poOp ) const;
#endif // VERSION_RUNTIME // VERSION_LIB
	
	bool Decode_GetNextLine( CStr &roDst, CStr &roSrc ) const;
	bool Decode_GetNextToken( CStr &roDst, CStr &roSrc ) const;
	
	void RegisterOpClass( const char *pcClassName, COrgOp::TFuncCreate tFuncCreate );
	
private:
	CList<COp *> m_oLstOp;
	
	CArray<CStr> m_oArrClassName;
	CArray<TFuncCreate> m_oArrClassCreate;
	CStr m_oStrTmp;
	
private:
	static COp *m_poOpRootL_;
	static COp *m_poOpRootR_;

#if !defined( VERSION_RUNTIME ) && !defined( VERSION_LIB )
public:
	static bool m_bEnableCache_;
	static unsigned int m_uiCachePos_;
	static double m_adCache_[ORG_OP_CACHE];
#endif // VERSION_RUNTIME // VERSION_LIB
};

#include "OrgOp.inline.h"

#endif // _ORG_OP_H_
