// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSys.cpp

#include "OpSys.h"
#ifdef OP_INC_OP_SYS

#include <time.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// COpSys
////////////////////////////////////////////////////////////////////////////////

#define OP_TM_IN_EXIT			0
#define OP_TM_IN_SIGNAL			1
#define OP_TM_IN_PRINT			2
#define OP_TM_IN_PRINT_TRIGG		3
#define OP_TM_OUT_SAMPLE_FREQ		0
#define OP_TM_OUT_SEED			1
#define OP_TM_OUT_TRIGGER		2
#define OP_TM_OUT_PI			3
#define OP_TM_OUT_PI2			4
#define OP_TM_OUT_S12			5
#define OP_TM_OUT_NULL			6

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_TM_IN_EXIT_INFO		"quit program"
#define OP_TM_IN_SIGNAL_INFO		"signal"
#define OP_TM_IN_PRINT_INFO		"console"
#define OP_TM_IN_PRINT_TRIGG_INFO	"console trigger"
#define OP_TM_OUT_SAMPLE_FREQ_INFO	"sampling frequency"
#define OP_TM_OUT_SEED_INFO		"seed"
#define OP_TM_OUT_TRIGGER_INFO		"trigger"
#define OP_TM_OUT_PI_INFO		"PI"
#define OP_TM_OUT_PI2_INFO		"PI * 2.0"
#define OP_TM_OUT_S12_INFO		"12th root of 2.0"
#define OP_TM_OUT_NULL_INFO		"null"
#define OP_TM_CLASS_INFO \
\
"System Functions\n" \
"- When the quit program input value is greater than 0.0, the program\n" \
"  exits abruptly.\n" \
"- When the signal input value is greater than 0.0, then the global\n" \
"  signal variable is incremented (-> es library).\n" \
"- When the console inlet is connected, then its input value will be\n" \
"  written to the editor console whenever the associated trigger\n" \
"  value is greater than 0.0.\n" \
"- The trigger output value is 1.0 only at the start of the program,\n" \
"  afterwards permanently 0.0.\n" \
"- The sampling frequency defines the number of samples the system\n" \
"  actually outputs per second."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_TM_IN_EXIT_INFO		"Programm beenden"
#define OP_TM_IN_SIGNAL_INFO		"Signal"
#define OP_TM_IN_PRINT_INFO		"Text-Ausgabe"
#define OP_TM_IN_PRINT_TRIGG_INFO	"Text-Ausgabe-Auslöser"
#define OP_TM_OUT_SAMPLE_FREQ_INFO	"Abtastfrequenz"
#define OP_TM_OUT_SEED_INFO		"Einmalige Zufallszahl"
#define OP_TM_OUT_TRIGGER_INFO		"Auslöser"
#define OP_TM_OUT_PI_INFO		"Pi"
#define OP_TM_OUT_PI2_INFO		"Pi * 2.0"
#define OP_TM_OUT_S12_INFO		"Zwölfte Wurzel von 2.0"
#define OP_TM_OUT_NULL_INFO		"Null"
#define OP_TM_CLASS_INFO \
\
"Systemfunktionen\n" \
"- Ist der Wert des 'Programm beenden'-Eingangs grösser als 0.0,\n" \
"  so wird das Programm abrupt beedet.\n" \
"- Ist der Wert des Signal-Eingangs grösser als 0.0,\n" \
"  so wird die globale Signal-Variable inkrementiert.\n" \
"- Ist die Text-Eingang verbunden, so wird der Wert in der Konsole\n" \
"  ausgegeben, solange der entsprechende Auslöser grösser als 0.0 ist.\n" \
"- Der Auslöser steht nur ganz zu Beginn und nach jedem Reset auf 1.0,\n" \
"  sonst immer auf 0.0.\n" \
"- Die Abtastfrequenz fs entspricht der Anzahl Samples pro Sekunde die\n" \
"  das System tatsächlich ausgibt."

#else
#error language not specified
#endif // OP_LANGUAGE_*

unsigned int COpSys::m_uiInstances_ = 0;

COpSys::COpSys()
: COp( 4, 7 )
{
	Init();
	OP_SET_NAME_INSTANCE( "sys" );
	m_poIn[OP_TM_IN_EXIT].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_TM_IN_SIGNAL].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_TM_IN_PRINT].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_TM_IN_PRINT_TRIGG].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_TM_IN_EXIT, "qit", OP_TM_IN_EXIT_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_TM_IN_SIGNAL, "sig", OP_TM_IN_SIGNAL_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_TM_IN_PRINT, "prt", OP_TM_IN_PRINT_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_TM_IN_PRINT_TRIGG, "tr", OP_TM_IN_PRINT_TRIGG_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TM_OUT_SAMPLE_FREQ, "fs", OP_TM_OUT_SAMPLE_FREQ_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TM_OUT_SEED, "s", OP_TM_OUT_SEED_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TM_OUT_TRIGGER, "tr", OP_TM_OUT_TRIGGER_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TM_OUT_PI, "pi", OP_TM_OUT_PI_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TM_OUT_PI2, "pi2", OP_TM_OUT_PI2_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TM_OUT_S12, "s12", OP_TM_OUT_PI2_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_TM_OUT_NULL, "n", OP_TM_OUT_NULL_INFO );
	Reset();
	Validate();
}

COpSys::~COpSys()
{}

void COpSys::Init()
{
	m_bTrigger = true;
	if( !m_uiInstances_++ )
		srand( (unsigned)time( NULL ) );
	COp::Init();
}

void COpSys::Proc()
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_TM_IN_EXIT].IsValid() )
		if( m_poIn[OP_TM_IN_EXIT].Proc() > 0.0 )
			exit( 0 );
	if( m_poIn[OP_TM_IN_SIGNAL].IsValid() )
		if( m_poIn[OP_TM_IN_SIGNAL].Proc() > 0.0 )
			++m_uiSignal_;
	if( m_poIn[OP_TM_IN_PRINT_TRIGG].IsValid()
	 && m_poIn[OP_TM_IN_PRINT_TRIGG].Proc() > 0.0 )
	{
		double dIn = 0.0;
		if( m_poIn[OP_TM_IN_PRINT].IsValid() )
			dIn = m_poIn[OP_TM_IN_PRINT].Proc();
		OP_LOG_FLT( m_oStrNameInstance.GetData(), dIn );
	}
	if( m_bTrigger )
	{
		m_bTrigger = false;
		m_pdOut[OP_TM_OUT_TRIGGER] = 1.0;
	}
	else
		m_pdOut[OP_TM_OUT_TRIGGER] = 0.0;
}

void COpSys::Reset()
{
	COp::Reset();
	m_pdOut[OP_TM_OUT_SAMPLE_FREQ] = (double)( OP_SAMPLE_FREQ );
	m_pdOut[OP_TM_OUT_SEED] = rand();
	m_pdOut[OP_TM_OUT_PI] = OP_PI;
	m_pdOut[OP_TM_OUT_PI2] = OP_PI2;
	m_pdOut[OP_TM_OUT_S12] = OP_SQRT_2_12;
	//m_pdOut[OP_TM_OUT_NULL] = 0.0;
	m_bTrigger = true;
}

OP_SET_NAME_AND_INFO_CLASS( COpSys, OP_TM_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSys )
OP_GENERIC_METHODS_DEF( COpSys )

#endif // OP_INC_OP_SYS
