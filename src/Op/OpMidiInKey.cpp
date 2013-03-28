// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiInKey.cpp

#include "OpMidiInKey.h"
#ifdef OP_INC_OP_MIDI_IN_KEY


////////////////////////////////////////////////////////////////////////////////
// COpMidiInKey::SVoice
////////////////////////////////////////////////////////////////////////////////

COpMidiInKey::SVoice::SVoice()
: m_bIsOn( false )
, m_bTriggOn( false )
, m_bTriggOff( false )
, m_ucNote( 0 )
, m_uiStamp( 0 )
{}

void COpMidiInKey::SVoice::Reset( unsigned int uiStamp )
{
	m_bIsOn = false;
	m_bTriggOn = false;
	m_bTriggOff = false;
	m_ucNote = 0;
	m_uiStamp = uiStamp;
}

inline bool COpMidiInKey::SVoice::Update( unsigned int uiStamp )
{
	if( m_uiStamp < (uiStamp - 1) && ( m_bTriggOn || m_bTriggOff ) )
	{
		m_bTriggOn = m_bTriggOff = false;
		return true;
	}
	return false;
}


////////////////////////////////////////////////////////////////////////////////
// COpMidiInKey
////////////////////////////////////////////////////////////////////////////////

#define OP_MIK_IN_DEV			0
#define OP_MIK_IN_CHAN			1
#define OP_MIK_OUT_NUM			0
#define OP_MIK_OUT_VOICES		1
#define OP_MIK_OUT_ON			2
#define OP_MIK_OUT_OFF			3
#define OP_MIK_OUT_NOTE			4
#define OP_MIK_OUT_VEL			5

#if defined( OP_LANGUAGE_ENGLISH )

#elif defined( OP_LANGUAGE_GERMAN )

#else
#error language not specified
#endif // OP_LANGUAGE_*


#define OP_MIK_ROUTINE_EXPAND		0
#define OP_MIK_ROUTINE_SHRINK		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MIK_IN_DEV_INFO		"device"
#define OP_MIK_IN_CHAN_INFO		"channel"
#define OP_MIK_OUT_NUM_INFO		"number of voices"
#define OP_MIK_OUT_VOICES_INFO		"number of active voices"
#define OP_MIK_OUT_ON_INFO		"start trigger"
#define OP_MIK_OUT_OFF_INFO		"end trigger"
#define OP_MIK_OUT_NOTE_INFO		"note"
#define OP_MIK_OUT_VEL_INFO		"velocity"
#define OP_MIK_ROUTINE_EXPAND_INFO	"add voice"
#define OP_MIK_ROUTINE_SHRINK_INFO	"remove voice"
#define OP_MIK_CLASS_INFO \
\
"MIDI Keyboard\n" \
"- All NOTE_ON messages on the selected channel get mapped\n" \
"  onto an arbitrary number of voices.\n" \
"- Start and end trigger events, note index and velocity values are\n" \
"  received per voice and written to the corresponding outlets.\n" \
"- An incoming message activates always the oldest inactive voice.\n" \
"- When all voices are in use, then the oldest active voice is used.\n" \
"- The start/end trigger output values are 1.0 when a note begins/ends.\n" \
"  Otherwise they are always 0.0.\n" \
"- There are 16 channels. Invalid values get clamped."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MIK_IN_DEV_INFO		"Gerät"
#define OP_MIK_IN_CHAN_INFO		"Kanal"
#define OP_MIK_OUT_NUM_INFO		"Anazhl Stimmen"
#define OP_MIK_OUT_VOICES_INFO		"Anzahl aktive Stimmen"
#define OP_MIK_OUT_ON_INFO		"Start-Auslöser"
#define OP_MIK_OUT_OFF_INFO		"End-Auslöser"
#define OP_MIK_OUT_NOTE_INFO		"Note"
#define OP_MIK_OUT_VEL_INFO		"Anschlagsstärke"
#define OP_MIK_ROUTINE_EXPAND_INFO	"Stimme hinzufügen"
#define OP_MIK_ROUTINE_SHRINK_INFO	"Stimme entfernen"
#define OP_MIK_CLASS_INFO \
\
"MIDI-Keyboard\n" \
"- Alle NOTE_ON-Nachrichten auf dem gewählten Kanal des verbundenen Geräts\n" \
"  werden auf eine bliebige Anzahl Stimmen abgebildet.\n" \
"- Pro Stimme werden Start- und End-Auslöser, Noten-Index und Anschlags-\n" \
"  stärke ausgegeben.\n" \
"- Eine eintreffende Nachrichten aktiviert immer die älteste, nicht aktive\n" \
"  Stimme.\n" \
"- Sind alle Stimmen in Gebrauch, wird die älteste aktive Stimme gewählt.\n" \
"- Die Start- und End-Auslöser-Ausgänge stehen immer dann auf 1.0, wenn eine\n" \
"  Note beginnt bzw. endet, sonst immer auf 0.0.\n" \
"- Es gibt 16 Kanäle. Ungültige Werte werden gekappt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMidiInKey::COpMidiInKey()
: COp( 2, 6 )
{
	Init();
	OP_SET_NAME_INSTANCE( "mInKy" );
	OP_SET_NAME_AND_INFO_INPUT( OP_MIK_IN_DEV, "d", OP_MIK_IN_DEV_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MIK_IN_CHAN, "c", OP_MIK_IN_CHAN_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MIK_OUT_NUM, "n", OP_MIK_OUT_NUM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MIK_OUT_VOICES, "a", OP_MIK_OUT_VOICES_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MIK_OUT_ON, "", OP_MIK_OUT_ON_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MIK_OUT_OFF, "", OP_MIK_OUT_OFF_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MIK_OUT_NOTE, "", OP_MIK_OUT_NOTE_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MIK_OUT_VEL, "", OP_MIK_OUT_VEL_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MIK_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_MIK_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MIK_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_MIK_ROUTINE_SHRINK_INFO );
	SetFlags( OP_FLAG_DYNAMIC_OUTPUTS );
	Update();
	Validate();
}

COpMidiInKey::~COpMidiInKey()
{
	Detach();
}

COpMidiInKey & COpMidiInKey::operator = ( const COpMidiInKey &roO )
{
	COp::operator = ( roO );

	m_poDev = 0;
	m_oArrVoice = roO.m_oArrVoice;
	m_dCountVoice = roO.m_dCountVoice;
	//m_iMidiClass = roO.m_iMidiClass; // Internal.

	Validate();
	//Update();
	return *this;
}

void COpMidiInKey::Init()
{
	m_iMidiClass = -1;
	OP_SET_COUNT_INTERNAL_INT( 1 );
	OP_SET_PTR_INTERNAL_INT( 0, &m_iMidiClass );
	m_dCountVoice = 0.0;
	m_poDev = 0;
	Reset();
	COp::Init();
}

void COpMidiInKey::Detach()
{
	if( m_poDev )
	{
		m_poDev->RemoveHandler( (CMidiIn::CHandler *)this );
		m_poDev = 0;
	}
}

void COpMidiInKey::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();

	m_poIn[OP_MIK_IN_DEV].Proc();

	const unsigned int uiCountVoice = m_oArrVoice.GetSize();
	unsigned int i = 0, o = OP_MIK_OUT_ON;
	while( i < uiCountVoice )
	{
		if( m_oArrVoice[i].Update( m_uiState ) )
			m_pdOut[o] = m_pdOut[o+1] = 0.0;
		++i;
		o += 4;
	}
	m_pdOut[OP_MIK_OUT_VOICES] = m_dCountVoice;
}

void COpMidiInKey::Update()
{
	COp::Update();

	const unsigned int uiCountVoice = ( ( m_uiCountOut - OP_MIK_OUT_ON ) / 4 );
	if( m_oArrVoice.GetSize() != uiCountVoice )
		m_oArrVoice.Resize( uiCountVoice );

	m_pdOut[OP_MIK_OUT_NUM] = (double)uiCountVoice;

#ifdef OP_USE_RUNTIME_INFO
	unsigned int o = OP_MIK_OUT_ON;
	while( o < m_uiCountOut )
	{
		SetNameOutput( o, "ta" );
		SetInfoOutput( o++, OP_MIK_OUT_ON_INFO );
		SetNameOutput( o, "tr" );
		SetInfoOutput( o++, OP_MIK_OUT_OFF_INFO );
		SetNameOutput( o, "nt" );
		SetInfoOutput( o++, OP_MIK_OUT_NOTE_INFO );
		SetNameOutput( o, "v" );
		SetInfoOutput( o++, OP_MIK_OUT_VEL_INFO );
	}
#endif // OP_USE_RUNTIME_INFO
}

void COpMidiInKey::Reset()
{
	for( unsigned int i=0; i<m_oArrVoice.GetSize(); ++i )
		m_oArrVoice[i].Reset( m_uiState );
	m_dCountVoice = 0.0;
	COp::Reset();
}

void COpMidiInKey::Validate()
{
	COp::Validate();

	Detach();

	if( !m_bIsNotValid )
	{
		COp *poOp = m_poIn[OP_MIK_IN_DEV].GetOp();
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
			{
				m_poDev->AddHandler( (CMidiIn::CHandler *)this );
				return;
			}
		}

		m_iMidiClass = -1;
		m_bIsNotValid = true;
	}
}

#ifdef OP_USE_ROUTINES
void COpMidiInKey::Routine( unsigned int uiIndex )
{
	Detach();

	switch( uiIndex )
	{
	case OP_MIK_ROUTINE_SHRINK:
		if( m_uiCountOut > 6 )
			SetCountOut( m_uiCountOut - 4 );
	break;
	case OP_MIK_ROUTINE_EXPAND:
		SetCountOut( m_uiCountOut + 4 );
	break;
	}
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpMidiInKey, OP_MIK_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMidiInKey ) // Warning: CMidiIn::CHandler() not explicitly initialized
OP_GENERIC_METHODS_DEF( COpMidiInKey )


void COpMidiInKey::OnNote( bool bOn, unsigned char ucChannel, unsigned char ucNote, unsigned char ucVelocity )
{
	if( m_bIsNotValid
	 || !m_poIn[OP_MIK_IN_CHAN].IsValid() )
		return;

	int iChannel = (int)rint( m_poIn[OP_MIK_IN_CHAN].Proc() );
	OP_RANGE_MINMAX( iChannel, 0, 15 );

	if( ucChannel != iChannel )
		return;

	if( bOn )
	{
		const unsigned int uiCountVoice = m_oArrVoice.GetSize();
		int iIndex = -1;
		unsigned int s = m_uiState + 1;
		unsigned int i = 0;
		while( i < uiCountVoice )
		{
			if( !m_oArrVoice[i].m_bIsOn )
			{
				const unsigned int sn = m_oArrVoice[i].m_uiStamp;
				if( sn < s )
				{
					s = sn;
					iIndex = i;
				}
			}
			++i;
		}
		if( iIndex < 0 )
		{
			iIndex = 0;
			s = m_uiState + 1;
			i = 0;
			while( i < uiCountVoice )
			{
				if( m_oArrVoice[i].m_bIsOn )
				{
					const unsigned int sn = m_oArrVoice[i].m_uiStamp;
					if( sn < s )
					{
						s = sn;
						iIndex = i;
					}
					++i;
				}
			}
		}
		else
			m_dCountVoice += 1.0;

		m_oArrVoice[iIndex].m_bIsOn = true;
		m_oArrVoice[iIndex].m_bTriggOn = true;
		m_oArrVoice[iIndex].m_bTriggOff = false;
		m_oArrVoice[iIndex].m_ucNote = ucNote;
		m_oArrVoice[iIndex].m_uiStamp = m_uiState;
		unsigned int j = OP_MIK_OUT_ON + ( iIndex * 4 );
		m_pdOut[j++] = 1.0;
		m_pdOut[j++] = 0.0;
		m_pdOut[j++] = (double)ucNote;
		m_pdOut[j++] = (double)ucVelocity;
	}
	else
	{
		for( unsigned int i=0; i<m_oArrVoice.GetSize(); ++i )
		{
			if( m_oArrVoice[i].m_ucNote == ucNote )
			{
				m_oArrVoice[i].m_bIsOn = false;
				m_oArrVoice[i].m_bTriggOn = true;
				m_oArrVoice[i].m_bTriggOff = false;
				m_oArrVoice[i].m_ucNote = 0;
				m_oArrVoice[i].m_uiStamp = m_uiState;
				unsigned int j = OP_MIK_OUT_ON + ( i * 4 );
				m_pdOut[j++] = 0.0;
				m_pdOut[j++] = 1.0;
				//m_pdOut[j++] = 0.0;
				//m_pdOut[j++] = 0.0;

				m_dCountVoice -= 1.0;
				break;
			}
		}
	}
}

void COpMidiInKey::OnDestroy()
{
	m_poDev = 0;
}

#endif // OP_INC_OP_MIDI_IN_KEY
