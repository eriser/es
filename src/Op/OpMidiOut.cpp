// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiOut.cpp

#include "OpMidiOut.h"
#ifdef OP_INC_OP_MIDI_OUT

////////////////////////////////////////////////////////////////////////////////
// COpMidiOut
////////////////////////////////////////////////////////////////////////////////

#define OP_MIO_IN_DEV			0
#define OP_MIO_IN_TRIGG			1
#define OP_MIO_IN_CHAN			2
#define OP_MIO_IN_VAL0			3
#define OP_MIO_IN_VAL1			4
#define OP_MIO_OUT_CON			0
#define OP_MIO_ROUTINE_FUNC		0
#define OP_MIO_ROUTINE_EXPAND		1
#define OP_MIO_ROUTINE_SHRINK		2
#define OP_MIO_FUNC_NOTE_ON		0
#define OP_MIO_FUNC_NOTE_OFF		1
#define OP_MIO_FUNC_POLY_AFTERTOUCH	2
#define OP_MIO_FUNC_CONTROL_CHANGE	3
#define OP_MIO_FUNC_PROGRAM_CHANGE	4
#define OP_MIO_FUNC_CHANNEL_AFTERTOUCH	5
#define OP_MIO_FUNC_PITCHBEND		6
#define OP_MIO_FUNC_MAX			7

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MIO_IN_DEV_INFO		"device"
#define OP_MIO_IN_TRIGG_INFO		"trigger"
#define OP_MIO_IN_CHAN_INFO		"channel"
#define OP_MIO_IN_VAL0_INFO		"first value"
#define OP_MIO_IN_VAL1_INFO		"second value"
#define OP_MIO_OUT_CON_INFO		"connector"
#define OP_MIO_ROUTINE_FUNC_INFO	"message type"
#define OP_MIO_ROUTINE_EXPAND_INFO	"add input/output"
#define OP_MIO_ROUTINE_SHRINK_INFO	"remove input/output"
#define OP_MIO_CLASS_INFO \
\
"MIDI Output\n" \
"- Available message types:\n" \
"    NOTE_ON (v0: key, v1: velocity)\n" \
"    NOTE_OFF (v0: key, v1: velocity)\n" \
"    POLY_AFTERTOUCH (v0: key, v1: value)\n" \
"    CONTROL_CHANGE (v0: control num. (cc), v1: value)\n" \
"    PROGRAM_CHANGE (v0: program num.)\n" \
"    CHANNEL_AFTERTOUCH (v0: value)\n" \
"    PITCHBEND (v0: lsb, v1: msb)\n" \
"- Whenever the trigger input value is greater than 0.0, the current values\n" \
"  will be sent to the connected device.\n" \
"- An arbitrary number of messages of equal type can be sent.\n" \
"- There are 16 channels. Invalid values get clamped.\n" \
"- All values get rounded to the nearest integer and get clamped to [0,128).\n" \
"- The second value inlets have no function for PROGRAM_CHANGE\n" \
"  and CHANNEL_AFTERTOUCH messages.\n" \
"- The connector output value is always 0.0 and only used to activate\n" \
"  the operator."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MIO_IN_DEV_INFO		"Gerät"
#define OP_MIO_IN_TRIGG_INFO		"Auslöser"
#define OP_MIO_IN_CHAN_INFO		"Kanal"
#define OP_MIO_IN_VAL0_INFO		"erster Wert"
#define OP_MIO_IN_VAL1_INFO		"zweiter Wert"
#define OP_MIO_OUT_CON_INFO		"Anschluss"
#define OP_MIO_ROUTINE_FUNC_INFO	"Nachrichten-Typ"
#define OP_MIO_ROUTINE_EXPAND_INFO	"Eingang/Ausgang hinzufügen"
#define OP_MIO_ROUTINE_SHRINK_INFO	"Eingang/Ausgang entfernen"
#define OP_MIO_CLASS_INFO \
\
"MIDI Output\n" \
"- Es stehen folgende Nachrichten-Typen zur Verfügung:\n" \
"    NOTE_ON (v0: key, v1: velocity)\n" \
"    NOTE_OFF (v0: key, v1: velocity)\n" \
"    POLY_AFTERTOUCH (v0: key, v1: value)\n" \
"    CONTROL_CHANGE (v0: control num. (cc), v1: value)\n" \
"    PROGRAM_CHANGE (v0: program num.)\n" \
"    CHANNEL_AFTERTOUCH (v0: value)\n" \
"    PITCHBEND (v0: lsb, v1: msb)\n" \
"- Ist der Wert am Auslöser-Eingang grösser als 0.0, so werden die\n" \
"  aktuellen Werte an das verbundene Gerät gesendet.\n" \
"- Es können bliebig viele Nachrichten gleichen Typs gesendet werden.\n" \
"- Es gibt 16 Kanäle. Ungültige Werte werden gekappt.\n" \
"- Werte werden gerundet und auf [0,128) gekappt.\n" \
"- Bei PROGRAM_CHANGE, CHANNEL_AFTERTOUCH bleiben jeweils die zweiten\n" \
"  Wert-Eingänge funktionslos.\n" \
"- Der Anschluss-Ausgang gibt immer 0.0 aus und wird ausschliesslich\n" \
"  dazu verwendet den Operator zu aktivieren."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMidiOut::COpMidiOut()
: COp( 5, 1 )
, m_uiFuncNum( OP_MIO_FUNC_NOTE_ON )
{
	Init();
	OP_SET_NAME_INSTANCE( "mOut" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MIO_IN_DEV, "d", OP_MIO_IN_DEV_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MIO_IN_TRIGG, "tr", OP_MIO_IN_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MIO_IN_CHAN, "c", OP_MIO_IN_CHAN_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MIO_IN_VAL0, "", OP_MIO_IN_VAL0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MIO_IN_VAL1, "", OP_MIO_IN_VAL1_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MIO_OUT_CON, "", OP_MIO_OUT_CON_INFO );
	OP_SET_COUNT_ROUTINE( 3 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MIO_ROUTINE_FUNC, "f" OP_STR_ROUTINE, OP_MIO_ROUTINE_FUNC_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MIO_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_MIO_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MIO_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_MIO_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS );
	Update();
	Validate();
}

COpMidiOut::~COpMidiOut() 
{}

void COpMidiOut::Init()
{
	m_poDev = 0;
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	COp::Init();
}

void COpMidiOut::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();

	const double dChan = m_poIn[OP_MIO_IN_CHAN].Proc();
	const bool bProc = ( m_poIn[OP_MIO_IN_TRIGG].Proc() > 0.0 );
	int iChannel = 0;
	if( bProc )
	{
		iChannel = (int)rint( dChan );
		OP_RANGE_MINMAX( iChannel, 0, 15 );
	}
	
	unsigned int i = OP_MIO_IN_VAL0;
	while( i < m_uiCountIn )
	{
		const double dV0 = m_poIn[i++].Proc();
		const double dV1 = m_poIn[i++].Proc();
		
		if( bProc )
		{
			int iV0 = (int)rint( dV0 );
			int iV1 = (int)rint( dV1 );
			OP_RANGE_MINMAX( iV0, 0, 127 );
			OP_RANGE_MINMAX( iV1, 0, 127 );
			
			switch( m_uiFuncNum )
			{
			case OP_MIO_FUNC_NOTE_ON:
				m_poDev->NoteOn( iChannel, iV0, iV1 );
			break;
			case OP_MIO_FUNC_NOTE_OFF:
				m_poDev->NoteOff( iChannel, iV0, iV1 );
			break;
			case OP_MIO_FUNC_POLY_AFTERTOUCH:
				m_poDev->PolyAftertouch( iChannel, iV0, iV1 );
			break;
			case OP_MIO_FUNC_CONTROL_CHANGE:
				m_poDev->ControlChange( iChannel, iV0, iV1 );
			break;
			case OP_MIO_FUNC_PROGRAM_CHANGE:
				m_poDev->ProgramChange( iChannel, iV0 );
			break;
			case OP_MIO_FUNC_CHANNEL_AFTERTOUCH:
				m_poDev->ChannelAftertouch( iChannel, iV0 );
			break;
			case OP_MIO_FUNC_PITCHBEND:
				m_poDev->Pitchbend( iChannel, iV0, iV1 );
			break;
			}
		}
	}
}

void COpMidiOut::Update()
{
	COp::Update();
	
#ifdef OP_USE_RUNTIME_INFO
	static const char * aacFuncName_[] = { "n+", "n-", "pa", "cc", "pc", "ca", "pb" };
	SetNameOutput( OP_MIO_OUT_CON, aacFuncName_[m_uiFuncNum] );
	unsigned int i = OP_MIO_IN_VAL0;
	while( i < m_uiCountIn )
	{
		SetNameInput( i, "v0" );
		SetInfoInput( i++, OP_MIO_IN_VAL0_INFO );
		SetNameInput( i, "v1" );
		SetInfoInput( i++, OP_MIO_IN_VAL1_INFO );
	}
#endif // OP_USE_RUNTIME_INF
}

void COpMidiOut::Validate()
{
	COp::Validate();
	if( !m_bIsNotValid )
	{
		COp *poOp = m_poIn[OP_MIO_IN_DEV].GetOp();
		if( poOp )
		{
#ifdef OP_USE_RUNTIME_INFO
			if( strcmp( poOp->GetNameClass(), "COpMidiDevOut" ) == 0 )
#endif // OP_USE_RUNTIME_INFO
			{
				COpMidiDevOut *poOpDev = (COpMidiDevOut *)( poOp ); // dynamic_cast
				if( poOpDev )
				{
					m_poDev = &poOpDev->m_oDev;
					return;
				}
			}
		}
		m_bIsNotValid = true;
	}
}

#ifdef OP_USE_ROUTINES
void COpMidiOut::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_MIO_ROUTINE_FUNC:
		if( ++m_uiFuncNum >= OP_MIO_FUNC_MAX )
			m_uiFuncNum = 0;
	break;
	case OP_MIO_ROUTINE_SHRINK:
		if( m_uiCountIn > 5 )
			SetCountIn( m_uiCountIn - 2 );
	break;
	case OP_MIO_ROUTINE_EXPAND:
		SetCountIn( m_uiCountIn + 2 );
	break;
	}
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpMidiOut, OP_MIO_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMidiOut )
OP_GENERIC_METHODS_DEF( COpMidiOut )

#endif // OP_INC_OP_MIDI_OUT
