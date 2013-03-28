// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Time.h

#ifndef _TIME__H_
#define _TIME__H_

//#define TIME_USE_ANSI
#define TIME_CHECK_HIGHRES

#define TIME_VERSION	"0.03"

#if defined( TIME_USE_ANSI )
#	include <time.h>
#elif defined( __WINDOWS__ )
#	define TIME_USE_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <mmsystem.h>	// Winmm.lib
#elif defined( __LINUX__ )
#	define TIME_USE_LINUX
#	include <time.h>
#	include <sys/time.h>
#else
#	error platform not supported
#endif // platform

#include <stdio.h>
#if defined( _MSC_VER )
#	define TIME_SPRINTF( buf, size, frm, ... )	sprintf_s( buf, size, frm, __VA_ARGS__ )
#else // ANSI C
#	define TIME_SPRINTF( buf, size, frm, ... )	sprintf( buf, frm, __VA_ARGS__ )
#endif //

// Zeit.
//
// - Update() bestimmt den Zeitpunkt, den die folgenden Methoden wieder-
//   geben. Update() wird im Konstruktor einmal automatisch aufgerufen.
//
// ANSI:
// - Bei definiertem TIME_USE_ANSI wird TIME_CHECK_HIGHRES nicht beachtet.
//
// Windows:
// - Unter Windows unterscheiden sich die Systemfunktionen timeGetTime() und
//   die HighResolution-Timer-Funktionen - wie der Name schon sagt - im unteren
//   Bereich (unter 20 Millisekunden). timeGetTime() ist hier nur noch sehr
//   grob aufgelöst.
//
// Linux:
// - Update() und alle folgenden Methoden benützen die ANSI-Funktionen.
// - Ist TIME_CHECK_HIGHRES definiert, wird im Konstruktor m_bUseHighRes_
//   ohne weitere Abfrage auf true gesetzt. Im Moment habe noch ich zu wenig
//   Kenntnisse über Linux, um hier eine besseres Verfahren zur Bestimmung von
//   HighRes-Timern zu verwenden.
////////////////////////////////////////////////////////////////////////////////
class CTime
{
public:
	CTime();
	~CTime();
public:
	inline void Reset();
	inline double GetSeconds() const;
	inline unsigned long int GetMilliseconds() const;

	void Update();
	inline unsigned int GetYear() const;
	inline unsigned int GetMonth() const;
	inline unsigned int GetDay() const;
	inline unsigned int GetHour() const;
	inline unsigned int GetMinute() const;
	inline unsigned int GetSecond() const;
	const char * GetString() const;
private:
#if defined( TIME_USE_ANSI )
	clock_t m_oStart;
	static tm *m_poTM_;
#elif defined( TIME_USE_WINDOWS )
	DWORD m_dwStart;
#	if defined( TIME_CHECK_HIGHRES )
	LARGE_INTEGER m_i64CountStart;
	static double m_dTimerFrequencyFac_;
	static LARGE_INTEGER m_i64Frequency_;
#	endif // TIME_CHECK_HIGHRES
	static SYSTEMTIME m_oTime_;
#elif defined( TIME_USE_LINUX )			// todo: time struct
	timeval m_oStart;
#	if defined( TIME_CHECK_HIGHRES )
	timespec m_oStartHR;
#	endif // TIME_CHECK_HIGHRES
	static tm *m_poTM_;
#endif // platform
	static unsigned int m_uiInstances_;
	static bool m_bUseHighRes_;
};

#include "Time.inline.h"

#endif // _TIME__H_
