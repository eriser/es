// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpChorus.cpp

#include "OpChorus.h"
#ifdef OP_INC_OP_CHORUS

////////////////////////////////////////////////////////////////////////////////
// COpChorus
////////////////////////////////////////////////////////////////////////////////

#define OP_CHR_IN_SRC			0
#define OP_CHR_IN_CLEAR			1
#define OP_CHR_IN_DELAY			2
#define OP_CHR_IN_FAC			3
#define OP_CHR_OUT_NUM			0
#define OP_CHR_OUT_DST			1

#define OP_CHR_ROUTINE_MODE		0
#define OP_CHR_ROUTINE_SHRINK		1
#define OP_CHR_ROUTINE_EXPAND		2

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_CHR_IN_SRC_INFO		"input"
#define OP_CHR_IN_CLEAR_INFO		"reset"
#define OP_CHR_IN_DELAY_INFO		"total delay"
#define OP_CHR_IN_FAC_INFO		"delay factor"
#define OP_CHR_OUT_NUM_INFO		"number of read heads"
#define OP_CHR_OUT_DST_INFO		"outputs"
#define OP_CHR_ROUTINE_MODE_INFO	"mode"
#define OP_CHR_ROUTINE_EXPAND_INFO	"add read head"
#define OP_CHR_ROUTINE_SHRINK_INFO	"remove read head"
#define OP_CHR_CLASS_INFO \
\
"Chorus\n" \
"- Each read head reads the input signal with a maximal delay of the number of\n" \
"  samples specified at the total delay inlet.\n" \
"- The delay factor multiplied by the total delay results in the current delay\n" \
"  of each read head.\n" \
"- Minimal delay: 1 sample.\n" \
"- Delay factors greater than 1.0 are clamped to 1.0.\n" \
"- For negative delay factors, their absolute value is used.\n" \
"- To achieve the typical chorus effect, a relatively small total delay should\n" \
"  be chosen, and multiple read heads should be placed in irregular distances.\n" \
"  All delayed output values have to be added, and scaled afterwards.\n" \
"- When the reset shows a value greater than 0.0, then the whole buffer will be\n" \
"  set to 0.0. Its size doesn't change.\n" \
"- The mode routine chooses between a discrete and an interpolated delay buffer.\n" \
"  The latter uses more resources, but is better suited for dynamic changes."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_CHR_IN_SRC_INFO		"Eingang"
#define OP_CHR_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_CHR_IN_DELAY_INFO		"Gesamtverzögerung"
#define OP_CHR_IN_FAC_INFO		"Verzögerungsfaktor"
#define OP_CHR_OUT_NUM_INFO		"Anzahl Leseköpfe"
#define OP_CHR_OUT_DST_INFO		"Ausgang"
#define OP_CHR_ROUTINE_MODE_INFO	"Modus"
#define OP_CHR_ROUTINE_EXPAND_INFO	"Lesekopf hinzufügen"
#define OP_CHR_ROUTINE_SHRINK_INFO	"Lesekopf entfernen"
#define OP_CHR_CLASS_INFO \
\
"Chorus\n" \
"- Jeder Lesekopf liest das Eingabesignal mit einer maximalen Verzögerung\n" \
"  von der am Gesamtverzögerungs-Eingang angegebenen Anzahl Samples.\n" \
"- Die Verzögerungsfaktoren ergeben jeweils mulitpliziert mit der Gesamt-\n" \
"  verzögerung die Verzögerung des einzelnen Lesekopfes.\n" \
"- Die minimale Verzögerung beträgt 1 Sample.\n" \
"- Verzögerungsfaktoren grösser 1.0, werden als 1.0 interpretiert.\n" \
"- Von negativen Verzögerungen zählt deren Absolutwert.\n" \
"- Um den typischen Chorus-Effekt zu erreichen, ist eine relativ kleine Gesamt-\n" \
"  verzögerung zu wählen und mehrere Leseköpfe in unregelmässigen Abständen\n" \
"  zu platzieren. Alle verzögerten Ausgabe-Werte müssen anschliessend mit dem\n" \
"  Eingabesignal addiert und möglicherweise entsprechend skaliert werden.\n" \
"- Ist der Wert am Zurücksetzen-Eingang grösser als 0.0, so wird der gesamte\n" \
"  Puffer gelöscht. An seine Grösse ändert sich nichts.\n" \
"- Die Modus-Routine wechselt zwischen einem diskreten und einem interpolierten\n" \
"  Verzögerungs-Puffer, wobei letzterer mehr Ressourcen benötigt, aber besser für\n" \
"  dynamische Änderungen geeignet ist."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpChorus::COpChorus()
: COp( 4, 2 )
, m_uiMode( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "chrs" );
	m_poIn[OP_CHR_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_CHR_IN_SRC, "i", OP_CHR_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CHR_IN_CLEAR, "cl", OP_CHR_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CHR_IN_DELAY, "d", OP_CHR_IN_DELAY_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_CHR_IN_FAC, "f", OP_CHR_IN_FAC_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CHR_OUT_DST, "o", OP_CHR_OUT_DST_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_CHR_OUT_NUM, "n", OP_CHR_OUT_NUM_INFO );
	OP_SET_COUNT_ROUTINE( 3 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_CHR_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_CHR_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_CHR_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_CHR_ROUTINE_SHRINK_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_CHR_ROUTINE_MODE, "m" OP_STR_ROUTINE, OP_CHR_ROUTINE_MODE_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS | OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpChorus::~COpChorus()
{}

void COpChorus::Init()
{
	m_dDelay = 1.0;
	//m_oDelay.SetDelay( 1u ); // In Update()
	//m_oDelayInterpol.SetDelay( 1.0 );
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiMode );
	COp::Init();
}

void COpChorus::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_CHR_IN_CLEAR].IsValid()
	    && m_poIn[OP_CHR_IN_CLEAR].Proc() > 0.0 )
	{
		if( m_uiMode )
			m_oDelayInterpol.Reset();
		else
			m_oDelay.Reset();
	}
	
	const double dDly = m_poIn[OP_CHR_IN_DELAY].Proc();
	if( dDly != m_dDelay )
	{
		m_dDelay = dDly;
		if( m_uiMode )
			m_oDelayInterpol.SetDelay( dDly );
		else
			m_oDelay.SetDelay( dDly );
	}
	const double dIn = m_poIn[OP_CHR_IN_SRC].Proc();
	unsigned int i = OP_CHR_IN_FAC;
	unsigned int o = OP_CHR_OUT_DST;
	if( m_uiMode )
	{
		while( i < m_uiCountIn )
			m_pdOut[o++] = m_oDelayInterpol.ReadDelay( dDly * m_poIn[i++].Proc() );
		m_oDelayInterpol.WriteDelay( dIn );
	}
	else
	{
		while( i < m_uiCountIn )
			m_pdOut[o++] = m_oDelay.ReadDelay( (int)rint( dDly * m_poIn[i++].Proc() ) );
		m_oDelay.WriteDelay( dIn );
	}
}

void COpChorus::Update()
{
	m_oDelay.SetDelay( m_dDelay );
	m_oDelayInterpol.SetDelay( m_dDelay );
	m_oDelay.UpdateSize();
	m_oDelayInterpol.UpdateSize();
	
	m_pdOut[OP_CHR_OUT_NUM] = double( m_uiCountIn - OP_CHR_IN_FAC );
	
#ifdef OP_USE_RUNTIME_INFO
	static const char * aacName_[] = { "/", "~" };
	unsigned int i = OP_CHR_IN_FAC;
	unsigned int o = OP_CHR_OUT_DST;
	while( i < m_uiCountIn )
	{
		SetNameInput( i, "f" );
		SetInfoInput( i++, OP_CHR_IN_FAC_INFO );
		SetNameOutput( o, aacName_[m_uiMode] );
		SetInfoOutput( o++, OP_CHR_OUT_DST_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
	COp::Update();
}

void COpChorus::Reset()
{
	m_oDelay.Reset();
	m_oDelayInterpol.Reset();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpChorus::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_CHR_ROUTINE_MODE:
		m_uiMode = !m_uiMode;
	break;
	case OP_CHR_ROUTINE_SHRINK:
		if( m_uiCountIn > 4 )
		{
			SetCountIn( m_uiCountIn - 1 );
			SetCountOut( m_uiCountOut - 1 );
		}
	break;
	case OP_CHR_ROUTINE_EXPAND:
		SetCountIn( m_uiCountIn + 1 );
		SetCountOut( m_uiCountOut + 1 );
	break;
	}
	Reset();
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpChorus, OP_CHR_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpChorus )
OP_GENERIC_METHODS_DEF( COpChorus )

#endif // OP_INC_OP_CHORUS
