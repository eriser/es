// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiDevOut.cpp

#include "OpMidiDevOut.h"
#ifdef OP_INC_OP_MIDI_DEV_OUT

////////////////////////////////////////////////////////////////////////////////
// COpMidiDevOut
////////////////////////////////////////////////////////////////////////////////

#define OP_MDO_OUT_VAL			0
#define OP_MDO_ROUTINE_DEV		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MDO_OUT_VAL_INFO		"output"
#define OP_MDO_ROUTINE_DEV_INFO		"device\n"
#define OP_MDO_CLASS_INFO \
\
"MIDI Output Device\n" \
"- Represents any type of MIDI output device.\n" \
"- Multiple instances of same device are not allowed!"

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MDO_OUT_VAL_INFO		"Ausgang"
#define OP_MDO_ROUTINE_DEV_INFO		"Gerät\n"
#define OP_MDO_CLASS_INFO \
\
"MIDI-Ausgabe-Gerät\n" \
"- Räpresentiert ein beliebiges MIDI Eingabe Gerät.\n" \
"- Mehrere Instanzen des selben Geräts sind nicht erlaubt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMidiDevOut::COpMidiDevOut()
: COp( 0, 1 )
, m_uiDevNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "mDot" );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MDO_OUT_VAL, "", OP_MDO_OUT_VAL_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MDO_ROUTINE_DEV, "d" OP_STR_ROUTINE, OP_MDO_ROUTINE_DEV_INFO
		+ CStr( "\n" ) + CMidiOut::GetInfoDevice_() );
	Update();
	Validate();
}

COpMidiDevOut::~COpMidiDevOut() 
{}

COpMidiDevOut & COpMidiDevOut::operator = ( const COpMidiDevOut &roO )
{
	COp::operator = ( roO ); // m_uiDevNum
	m_oDev = roO.m_oDev;
	Update();
	return *this;
}

void COpMidiDevOut::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiDevNum );
	COp::Init();
}

void COpMidiDevOut::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
}

void COpMidiDevOut::Update()
{
	COp::Update();
	m_oDev.Init( m_uiDevNum );
	
#ifdef OP_USE_RUNTIME_INFO
	char acName[] = "0";
	acName[0] += m_uiDevNum;
	SetNameOutput( OP_MDO_OUT_VAL, acName );
#endif // OP_USE_RUNTIME_INFO
}

#ifdef OP_USE_ROUTINES
void COpMidiDevOut::Routine( unsigned int uiIndex )
{
	if( ++m_uiDevNum >= m_oDev.GetDeviceCount() )
		m_uiDevNum = 0;
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpMidiDevOut, OP_MDO_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMidiDevOut )
OP_GENERIC_METHODS_DEF( COpMidiDevOut )

#endif // OP_INC_OP_MIDI_DEV_OUT
