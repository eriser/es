// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiDevIn.cpp

#include "OpMidiDevIn.h"
#ifdef OP_INC_OP_MIDI_DEV_IN

////////////////////////////////////////////////////////////////////////////////
// COpMidiDevIn
////////////////////////////////////////////////////////////////////////////////

#define OP_MDI_OUT_VAL			0
#define OP_MDI_ROUTINE_DEV		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MDI_OUT_VAL_INFO		"output"
#define OP_MDI_ROUTINE_DEV_INFO		"device\n"
#define OP_MDI_CLASS_INFO \
\
"MIDI Input Device\n" \
"- Represents any type of MIDI input device.\n" \
"- Multiple instances of same device are not allowed!"

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MDI_OUT_VAL_INFO		"Ausgang"
#define OP_MDI_ROUTINE_DEV_INFO		"Gerät\n"
#define OP_MDI_CLASS_INFO \
\
"MIDI-Eingabe-Gerät\n" \
"- Räpresentiert ein beliebiges MIDI Eingabe Gerät.\n" \
"- Mehrere Instanzen des selben Geräts sind nicht erlaubt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMidiDevIn::COpMidiDevIn()
: COp( 0, 1 )
, m_uiDevNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "mDin" );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MDI_OUT_VAL, "", OP_MDI_OUT_VAL_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MDI_ROUTINE_DEV, "d" OP_STR_ROUTINE, OP_MDI_ROUTINE_DEV_INFO
		+ CStr( "\n" ) + CMidiInDev::GetInfoDevice_() );
	Update();
	Validate();
}

COpMidiDevIn::~COpMidiDevIn() 
{
}

COpMidiDevIn & COpMidiDevIn::operator = ( const COpMidiDevIn &roO )
{
	COp::operator = ( roO ); // m_uiDevNum
	m_oDev = roO.m_oDev;
	Update();
	return *this;
}

void COpMidiDevIn::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiDevNum );
	COp::Init();
}

void COpMidiDevIn::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
}

void COpMidiDevIn::Update()
{
	COp::Update();
	m_oDev.Init( m_uiDevNum );
	
#ifdef OP_USE_RUNTIME_INFO
	char acName[] = "0";
	acName[0] += m_uiDevNum;
	SetNameOutput( OP_MDI_OUT_VAL, acName );
#endif // OP_USE_RUNTIME_INFO
}

#ifdef OP_USE_ROUTINES
void COpMidiDevIn::Routine( unsigned int uiIndex )
{
	if( ++m_uiDevNum >= m_oDev.GetDeviceCount() )
		m_uiDevNum = 0;
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpMidiDevIn, OP_MDI_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMidiDevIn )
OP_GENERIC_METHODS_DEF( COpMidiDevIn )

#endif // OP_INC_OP_MIDI_DEV_IN
