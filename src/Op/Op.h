// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Op.h
//
// Richtlinien:
// - Alle Operationen die nur in der Design-Phase benötigt werden, sind über Routinen zu lösen.
//   Der Routine-Code ist in der Runtime-Version nicht mehr enthalten. Kein Overhead durch Routinen!
// - Optionale Inputs müssen über CLink::SetFlags() markiert werden. Sie werden in COp::Validate()
//   nicht beachtet.
// - Namen von Routinen sollten mit definierten Markierung enden (OP_STR_ROUTINE).
// - Interne Variablen, welche nicht explizit als Internal registriert sind, müssen bei jeder Klasse
//   in einer überladenen Init() Methode angemeldet werden. Der generische Kopierkonstruktor bleibt
//   gültig, und ruft Init() auch auf.
// - Operator-spezifische Daten, die intern nicht registriert wurden, werden nicht speziell
//   beachtet. Der Compiler stellt aber einen generischen Zuweisungsoperator bereit, wobei dort aber
//   nur Variablen elementaren Typs und Objekte über deren Zuweisungsoperator kopiert werden. Sind
//   andere Daten vorhanden, also z.B. allokierter Speicher, so muss die tiefe Kopie in einer selbst
//   definierten Zuweisungsoperator-Methode erstellt werden!
// - Informationsangaben sollten eine Zeilenlänge von 80 Zeichen nicht überschreiten.
// - Einfach halten, nicht überladen. Folge dem KISS-Prinzip (Keep It Simple, Stupid!).
// - Kompakte Formulierung, Übersichtlichkeit, möglichst verständlich und lokal bleiben!
// - Trennung zw. Runtime-/Design-Modus.
// - Lieber kompakte, kleine Operatoren, als komplizierte mit breitem Funktionsumfang.
//   So wird verhindert, dass nicht benötigter Code auch nicht in der Runtime-Version auftaucht.
//   Gleichzeitig ist die Ausführung effizienter, da weniger Fallunterscheidungen gemacht werden müssen.
// - Im Normalfall sollten alle Eingänge immer abgefragt werden, auch wenn dies nicht effizient ist.
//   Nur so bleiben alle Operatoren synchron und können einander beliebig modulieren.
// - In Reset() Proc() niemals aufrufen!
//
// Bemerkungen:
// - Die ID 0 ist ungültig!
// - Alle GetName*/GetInfo*-Methoden mit const char * als Rückgabe-Typ,
//   sind hier nicht const, da der zurückgegeben Zeiger ungültig wäre,
//   da das interne CStr-Objekt nur temporär wäre.
//

#ifndef _OP_H_
#define _OP_H_

#include "OpDEF.h"

// Operator ADT
////////////////////////////////////////////////////////////////////////////////
class COp
{

public:
	// Link (implizite Kopie ist gültig)
	////////////////////////////////////////////////////////////////////////////////
	class CLink
	{
	public:
		friend class COp;
	public:
		CLink();
		CLink( COp *poOp,
		       unsigned int uiIndex,
		       unsigned int uiFlags=OP_LINK_FLAG_NONE );
		//CLink( const CLink &roLink );
		~CLink();
	public:
		//CLink & operator=( const CLink &roLink );

	public:
		inline double & Proc();
		inline bool IsValid() const;

		inline COp * GetOp() const;
		inline void SetOp( COp * poOp );
		inline unsigned int GetIndex() const;
		inline void SetIndex( unsigned int uiIndex );
		inline unsigned int GetFlags() const;
		inline void SetFlags( unsigned int uiFlags );

	public:
		void Reset(); // COp-Zeiger und Index werden auf 0 gesetzt.
		void Clear(); // Flags werden auch gelöscht.

	protected:
		COp *m_poOp;
		unsigned int m_uiIndex;
		unsigned int m_uiFlags;
	};

protected:
	typedef double (* TOpFuncFlt1)( double );
	typedef double (* TOpFuncFlt2)( double, double );
	typedef int (* TOpFuncInt1)( int );
	typedef int (* TOpFuncInt2)( int, int );
	typedef bool (* TOpFuncBool1)( bool );
	typedef bool (* TOpFuncBool2)( bool, bool );

public:
	COp( unsigned int uiCountIn, unsigned int uiCountOut );
	COp( const COp &roOp );
	virtual ~COp();

public:
	COp & operator = ( const COp &roOp );

public:
	virtual void Proc() = 0;
	virtual COp * Clone() const = 0;
#ifdef OP_USE_RUNTIME_INFO
	virtual const char * GetNameClass() const = 0;
	virtual const char * GetInfoClass() const = 0;
#endif // OP_USE_RUNTIME_INFO

public:
	virtual void Update();					// Inneres mit Äusserem abgleichen.
	virtual void Reset();					// Zurücksetzen auf Ausganswerte (hier: alle Outputs auf 0.0).
	virtual void Validate();				// m_bIsValid updaten.
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );		// Führt Routine aus.
#endif // OP_USE_ROUTINES

public:
	inline unsigned int GetCountIn() const;
	       void SetCountIn( unsigned int uiCountIn );	// Echten Resize-Operation.
	inline CLink & In( unsigned int uiIndex ) const;

	inline unsigned int GetCountOut() const;
	       void SetCountOut( unsigned int uiCountIn );	// Echten Resize-Operation.
	inline double & Out( unsigned int uiIndex ) const;

	inline bool IsValid();

public:
	static COp * Create_();					// Muss von jeder Klasse redefiniert werden!

public:
	inline unsigned int GetID() const;
	inline void SetID( unsigned int uiID );			// rem: Auf eigene Gefahr!

	inline unsigned int GetFlags() const;
	inline void SetFlags( unsigned int uiFlags );

#ifdef OP_USE_INTERNALS
	void SetCountInternalInt( unsigned int uiCount );
	void SetCountInternalUInt( unsigned int uiCount );
	void SetCountInternalFlt( unsigned int uiCount );
	inline unsigned int GetCountInternalInt() const;
	inline unsigned int GetCountInternalUInt() const;
	inline unsigned int GetCountInternalFlt() const;
	inline int GetValueInternalInt( unsigned int uiIndex ) const;
	inline unsigned int GetValueInternalUInt( unsigned int uiIndex ) const;
	inline double GetValueInternalFlt( unsigned int uiIndex ) const;
	inline void SetValueInternalInt( unsigned int uiIndex, int iVal );
	inline void SetValueInternalUInt( unsigned int uiIndex, unsigned int uiVal );
	inline void SetValueInternalFlt( unsigned int uiIndex, double dVal );
	inline void SetPtrInternalInt( unsigned int uiIndex, int *piVal );
	inline void SetPtrInternalUInt( unsigned int uiIndex, unsigned int *puiVal );
	inline void SetPtrInternalFlt( unsigned int uiIndex, double *pdVal );
#endif // OP_USE_INTERNALS

#ifdef OP_USE_RUNTIME_INFO
	inline const char * GetNameInstance() const;
	inline void SetNameInstance( const char * acName );
	inline const char * GetNameInput( unsigned int uiIndex );
	inline void SetNameInput( unsigned int uiIndex, const char *pcName );
	inline const char * GetNameOutput( unsigned int uiIndex );
	inline void SetNameOutput( unsigned int uiIndex, const char *pcName );

	inline const char * GetInfoOutput( unsigned int uiIndex );
	inline void SetInfoOutput( unsigned int uiIndex, const char *pcInfo );
	inline const char * GetInfoInput( unsigned int uiIndex );
	inline void SetInfoInput( unsigned int uiIndex, const char *pcInfo );

#ifdef OP_USE_ROUTINES
	inline unsigned int GetCountRoutine() const;
	inline void SetCountRoutine( unsigned int uiCount );
	inline const char* GetNameRoutine( unsigned int uiIndex );
	inline void SetNameRoutine( unsigned int uiIndex, const char *pcName );
	inline const char * GetInfoRoutine( unsigned int uiIndex );
	inline void SetInfoRoutine( unsigned int uiIndex, const char *pcInfo );
#endif // OP_USE_ROUTINES
#endif // OP_USE_RUNTIME_INFO


public:
	static void Prepare_();			// Muss vor jedem Sample aufgerufen werden!
	static unsigned int GetIDMax_();

protected:
	void Init();				// Sollte in jeder abgeleiteten Klasse zur Registierung
						// interner Variablen, Initialisierung von konstanten Flags (Inputs)
						// und sonstigen Vorarbeiten überladen werden. Wird in generischem
						// Kopierkonstruktor, und jedem eigenen Konstruktor aufgerufen.
	void InitInputs( unsigned int uiCountIn );
	void InitOutputs( unsigned int uiCountOut );
	void ClearInputs();
	void ClearOutputs();
	void ClearInternals();

protected:
	CLink * m_poIn;				// Input Links.
	unsigned int m_uiCountIn;		// Anzahl Input-Links.

	double * m_pdOut;			// Output-Werte.
	unsigned int m_uiCountOut;		// Anzahl Output-Werte.

	unsigned int m_uiState;			// Status.
	bool m_bIsNotValid;

	unsigned int m_uiID;			// Eindeutige ID.
	unsigned int m_uiFlags;			// Flags.

#ifdef OP_USE_INTERNALS
protected:
	unsigned int m_uiCountInternalInt;
	unsigned int m_uiCountInternalUInt;
	unsigned int m_uiCountInternalFlt;
	int ** m_pptPtrInternalInt;
	unsigned int ** m_pptPtrInternalUInt;
	double ** m_pptPtrInternalFlt;
#endif // OP_USE_INTERNALS

#ifdef OP_USE_RUNTIME_INFO
	CStr m_oStrNameInstance;		// Name der Instanz.
	CArray<CStr> m_oArrStrNameIn;		// Namen der Inputs.
	CArray<CStr> m_oArrStrNameOut;		// Namen der Outputs.
	CArray<CStr> m_oArrStrInfoIn;		// Informationen zu den Inputs.
	CArray<CStr> m_oArrStrInfoOut;		// Informationen zu den Outputs.
#ifdef OP_USE_ROUTINES
	CArray<CStr> m_oArrStrNameRoutine;	// Namen der Routinen.
	CArray<CStr> m_oArrStrInfoRoutine;	// Informationen zu den Routinen.
#endif // OP_USE_ROUTINES
#endif // OP_USE_RUNTIME_INFO

#ifdef OP_USE_USER_DATA
public:
	void *m_pvData;				// Frei verfügbarer Zeiger.
#endif // OP_USE_USER_DATA

protected:
	static bool m_bFirst_;			// Ganz am Anfang wahr.
	static unsigned int m_uiState_;		// Globaler Status.
	static double m_dTime_;			// Aktuelle Zeit.
	static unsigned int m_uiIDMax_;		// Höchste ID im System.
public:
	static unsigned int m_uiSignal_;	// Globale Signal-Variable.
};

#include "Op.inline.h"


// Generische Vorlagen-Makros
///////////////////////////////////////////////////////////////////////////////

// Generischer Kopier-Konstruktor.
// Bei Verwendung von Internals nicht zu gebrauchen, denn die müssen hier
// registriert werden.
#define OP_GENERIC_COPY_CTOR_DEC( class_name ) \
public: \
	class_name( const class_name &roOp ); \

#define OP_GENERIC_COPY_CTOR_DEF( class_name ) \
class_name::class_name( const class_name &roOp ) \
: COp( 0, 0 ) \
{ \
	Init(); \
	*this = roOp; \
	Validate(); \
	Update(); \
	Validate(); \
}

// Generische Methoden, die hiermit in jeder abgeleiten Operator-Klasse
// zu deklarieren sind.

#ifdef OP_USE_RUNTIME_INFO

#define OP_GENERIC_METHODS_DEC( class_name ) \
public: \
	virtual COp * Clone() const; \
	virtual const char * GetNameClass() const; \
	virtual const char * GetInfoClass() const; \
public: \
	static COp * Create_(); \
protected: \
	static const char m_acNameClass_[]; \
	static const char m_acInfoClass_[];
#else
#define OP_GENERIC_METHODS_DEC( class_name ) \
public: \
	virtual COp * Clone() const; \
public: \
	static COp * Create_();
#endif // OP_USE_RUNTIME_INFO

// Generische Methoden, die mit folgenden Makros in jeder abgeleiten
// Operator-Klasse zu definieren sind.

#ifdef OP_USE_RUNTIME_INFO
#define OP_GENERIC_METHODS_DEF( class_name ) \
COp * class_name::Clone() const { \
	return new class_name( *this ); \
} \
const char * class_name::GetNameClass() const { \
	return m_acNameClass_; \
} \
const char * class_name::GetInfoClass() const { \
	return m_acInfoClass_; \
} \
COp * class_name::Create_() { \
	return new class_name(); \
}
#else
#define OP_GENERIC_METHODS_DEF( class_name ) \
COp * class_name::Clone() const { \
	return new class_name( *this ); \
} \
COp * class_name::Create_() { \
	return new class_name(); \
}
#endif // OP_USE_RUNTIME_INFO

// Sollte in jeder Proc-Methode aller Operatoren zu Beginn platziert werden.
#define OP_GENERIC_CODE_PROC_PROLOG() \
	if( m_uiState == COp::m_uiState_ ) \
		return; \
	m_uiState = COp::m_uiState_; \
	if( m_bIsNotValid ) \
		return;

#endif // _OP_H_
