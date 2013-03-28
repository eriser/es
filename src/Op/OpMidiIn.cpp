// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiIn.cpp

#include "OpMidiIn.h"
#ifdef OP_INC_OP_MIDI_IN

////////////////////////////////////////////////////////////////////////////////
// COpMidiIn
////////////////////////////////////////////////////////////////////////////////

#define OP_MII_IN_DEV			0
#define OP_MII_IN_CHAN			1
#define OP_MII_IN_VAL			2

#define OP_MII_OUT_VAL			0

#define OP_MII_ROUTINE_FUNC		0
#define OP_MII_ROUTINE_EXPAND		1
#define OP_MII_ROUTINE_SHRINK		2

#define OP_MII_FUNC_NOTE		0
#define OP_MII_FUNC_NOTE_ON		1
#define OP_MII_FUNC_NOTE_OFF		2
#define OP_MII_FUNC_NOTE_VEL		3
#define OP_MII_FUNC_POLY_AFTERTOUCH	4
#define OP_MII_FUNC_CONTROL_CHANGE	5
#define OP_MII_FUNC_PROGRAM_CHANGE	6
#define OP_MII_FUNC_CHANNEL_AFTERTOUCH	7
#define OP_MII_FUNC_PITCHBEND		8
#define OP_MII_FUNC_MAX			9

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MII_IN_DEV_INFO		"device"
#define OP_MII_IN_CHAN_INFO		"channel"
#define OP_MII_IN_VAL_INFO		"value"
#define OP_MII_OUT_VAL_INFO		"output"
#define OP_MII_ROUTINE_FUNC_INFO	"message type"
#define OP_MII_ROUTINE_EXPAND_INFO	"add input/output"
#define OP_MII_ROUTINE_SHRINK_INFO	"remove input/output"
#define OP_MII_CLASS_INFO \
\
"MIDI Input\n" \
"- Available message types:\n" \
"    NOTE (v: key)\n" \
"    NOTE_ON (v: key)\n" \
"    NOTE_OFF (v: key)\n" \
"    NOTE_VEL (v: key)\n" \
"    POLY_AFTERTOUCH (v: key)\n" \
"    CONTROL_CHANGE (v: control num. (cc))\n" \
"    PROGRAM_CHANGE ()\n" \
"    CHANNEL_AFTERTOUCH ()\n" \
"    PITCHBEND ()\n" \
"- An arbitrary number of input messages of equal type can be received.\n" \
"- There are 16 channels. Invalid types get clamped.\n" \
"- All values get rounded to the nearest integer and get clamped to [0,128).\n" \
"- The value inlets have no function for PROGRAM_CHANGE, CHANNEL_AFTERTOUCH\n" \
"  and PITCHBEND messages."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MII_IN_DEV_INFO		"Gerät"
#define OP_MII_IN_CHAN_INFO		"Kanal"
#define OP_MII_IN_VAL_INFO		"Wert"
#define OP_MII_OUT_VAL_INFO		"Ausgang"
#define OP_MII_ROUTINE_FUNC_INFO	"Nachrichten-Typ"
#define OP_MII_ROUTINE_EXPAND_INFO	"Eingang/Ausgang hinzufügen"
#define OP_MII_ROUTINE_SHRINK_INFO	"Eingang/Ausgang entfernen"
#define OP_MII_CLASS_INFO \
\
"MIDI-Eingabe\n" \
"- Es stehen folgende Nachrichten-Typen zur Verfügung:\n" \
"    NOTE (v: key)\n" \
"    NOTE_ON (v: key)\n" \
"    NOTE_OFF (v: key)\n" \
"    NOTE_VEL (v: key)\n" \
"    POLY_AFTERTOUCH (v: key)\n" \
"    CONTROL_CHANGE (v: control num. (cc))\n" \
"    PROGRAM_CHANGE ()\n" \
"    CHANNEL_AFTERTOUCH ()\n" \
"    PITCHBEND ()\n" \
"- Es können bliebig viele Nachrichten gleichen Typs empfangen werden.\n" \
"- Es gibt 16 Kanäle. Ungültige Werte werden gekappt.\n" \
"- Werte werden gerundet und auf [0,128) gekappt.\n" \
"- Bei PROGRAM_CHANGE, CHANNEL_AFTERTOUCH und PITCHBEND bleiben die\n" \
"  Wert-Eingänge funktionslos.\n"

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMidiIn::COpMidiIn()
: COp( 3, 1 )
, m_uiFuncNum( OP_MII_FUNC_NOTE )
{
	Init();
	OP_SET_NAME_INSTANCE( "mIn" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MII_IN_DEV, "d", OP_MII_IN_DEV_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MII_IN_CHAN, "c", OP_MII_IN_CHAN_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MII_IN_VAL, "", OP_MII_IN_VAL_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MII_OUT_VAL, "", OP_MII_OUT_VAL_INFO );
	OP_SET_COUNT_ROUTINE( 3 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MII_ROUTINE_FUNC, "f" OP_STR_ROUTINE, OP_MII_ROUTINE_FUNC_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MII_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_MII_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MII_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_MII_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS | OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Reset();
	Validate();
}

COpMidiIn::~COpMidiIn()
{}

COpMidiIn & COpMidiIn::operator = ( const COpMidiIn &roO )
{
	COp::operator = ( roO );

	m_poDev = 0;
	//m_uiFuncNum = roO.m_uiFuncNum; // Internal.
	//m_iMidiClass = roO.m_iMidiClass; // Internal.

	OP_MEM_COPY( m_aabNoteOnPrev, roO.m_aabNoteOnPrev, sizeof( m_aabNoteOnPrev ) );

	Validate();
	//Update();
	return *this;
}

void COpMidiIn::Init()
{
	m_poDev = 0;
	m_iMidiClass = -1;
	OP_SET_COUNT_INTERNAL_UINT( 1 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	OP_SET_COUNT_INTERNAL_INT( 1 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_iMidiClass );
	COp::Init();
}

void COpMidiIn::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();

	m_poIn[OP_MII_IN_DEV].Proc();

	int iChannel = (int)rint( m_poIn[OP_MII_IN_CHAN].Proc() );
	OP_RANGE_MINMAX( iChannel, 0, 15 );

	unsigned int i = OP_MII_IN_VAL;
	unsigned int o = OP_MII_OUT_VAL;
	while( i < m_uiCountIn )
	{
		int iV = (int)rint(  m_poIn[i].Proc() );
		OP_RANGE_MINMAX( iV, 0, 127 );

		switch( m_uiFuncNum )
		{
		case OP_MII_FUNC_NOTE:
			m_pdOut[o] = ( m_poDev->m_aabNoteOn[iChannel][iV] )?( 1.0 ):( 0.0 );
		break;
		case OP_MII_FUNC_NOTE_ON:
		{
			const bool bOn = m_poDev->m_aabNoteOn[iChannel][iV];
			m_pdOut[o] = ( bOn && !m_aabNoteOnPrev[iChannel][iV] )?( 1.0 ):( 0.0 );
			m_aabNoteOnPrev[iChannel][iV] = bOn;
		}
		break;
		case OP_MII_FUNC_NOTE_OFF:
		{
			const bool bOn = m_poDev->m_aabNoteOn[iChannel][iV];
			m_pdOut[o] = ( !bOn && m_aabNoteOnPrev[iChannel][iV] )?( 1.0 ):( 0.0 );
			m_aabNoteOnPrev[iChannel][iV] = bOn;
		}
		break;
		case OP_MII_FUNC_NOTE_VEL:
			m_pdOut[o] = (double)m_poDev->m_aaucNoteVelocity[iChannel][iV];
		break;
		case OP_MII_FUNC_POLY_AFTERTOUCH:
			m_pdOut[o] = (double)m_poDev->m_aaucPolyAftertouch[iChannel][iV];
		break;
		case OP_MII_FUNC_CONTROL_CHANGE:
			m_pdOut[o] = (double)m_poDev->m_aaucControl[iChannel][iV];
		break;
		case OP_MII_FUNC_PROGRAM_CHANGE:
			m_pdOut[o] = (double)m_poDev->m_aucProgram[iChannel];
		break;
		case OP_MII_FUNC_CHANNEL_AFTERTOUCH:
			m_pdOut[o] = (double)m_poDev->m_aucChannelAftertouch[iChannel];
		break;
		case OP_MII_FUNC_PITCHBEND:
			m_pdOut[o] = (double)m_poDev->m_auiPitchbend[iChannel];
		break;
		}
		++i;
		++o;
	}
}

void COpMidiIn::Update()
{
	COp::Update();

#ifdef OP_USE_RUNTIME_INFO
	static const char * aacFuncName_[] = { "n", "no", "nf", "v", "pa", "cc", "pc", "ca", "pb" };

	unsigned int i = OP_MII_IN_VAL;
	unsigned int o = OP_MII_OUT_VAL;
	while( i < m_uiCountIn )
	{
		SetNameInput( i, "v" );
		SetInfoInput( i++, OP_MII_IN_VAL_INFO );
		SetNameOutput( o, aacFuncName_[m_uiFuncNum] );
		SetInfoOutput( o++, OP_MII_OUT_VAL_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
}

void COpMidiIn::Reset()
{
	COp::Reset();
	OP_MEM_ZERO( m_aabNoteOnPrev, sizeof( m_aabNoteOnPrev ) );
}

void COpMidiIn::Validate()
{
	COp::Validate();

	m_poDev = 0;

	if( !m_bIsNotValid )
	{
		COp *poOp = m_poIn[OP_MII_IN_DEV].GetOp();
		if( poOp )
		{
#ifdef OP_USE_MIDI
			if(
#ifdef OP_USE_RUNTIME_INFO
				strcmp( poOp->GetNameClass(), "COpMidiDevIn" ) == 0
#else
				m_iMidiClass == 0
#endif // OP_USE_RUNTIME_INFO
				)
			{
				m_iMidiClass = 0;
				COpMidiDevIn *poOpDev = (COpMidiDevIn *)( poOp ); // dynamic_cast
				if( poOpDev )
					m_poDev = &poOpDev->m_oDev;
			}
			else
#endif // OP_USE_MIDI

#ifndef OP_INC_OP_MIDI_FILE_IN
			{}
#else
			if(
#ifdef OP_USE_RUNTIME_INFO
				strcmp( poOp->GetNameClass(), "COpMidiFileIn" ) == 0
#else
				m_iMidiClass == 1
#endif // OP_USE_RUNTIME_INFO
				)
			{
				m_iMidiClass = 1;
				COpMidiFileIn *poOpFile = (COpMidiFileIn *)( poOp ); // dynamic_cast
				if( poOpFile )
					m_poDev = &poOpFile->m_oFile;
			}
#endif // OP_INC_OP_MIDI_FILE_IN

			if( m_poDev )
				return;
		}

		m_iMidiClass = -1;
		m_bIsNotValid = true;
	}
}

#ifdef OP_USE_ROUTINES
void COpMidiIn::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_MII_ROUTINE_FUNC:
		if( ++m_uiFuncNum >= OP_MII_FUNC_MAX )
			m_uiFuncNum = 0;
	break;
	case OP_MII_ROUTINE_SHRINK:
		if( m_uiCountIn > OP_MII_IN_VAL + 1 )
		{
			SetCountIn( m_uiCountIn - 1 );
			SetCountOut( m_uiCountOut - 1 );
		}
	break;
	case OP_MII_ROUTINE_EXPAND:
		SetCountIn( m_uiCountIn + 1 );
		SetCountOut( m_uiCountOut + 1 );
	break;
	}
	if( m_poDev )
		OP_MEM_COPY( m_aabNoteOnPrev, m_poDev->m_aabNoteOn, sizeof( m_aabNoteOnPrev ) );
	else
		Reset();
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpMidiIn, OP_MII_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMidiIn )
OP_GENERIC_METHODS_DEF( COpMidiIn )

#endif // OP_INC_OP_MIDI_IN
