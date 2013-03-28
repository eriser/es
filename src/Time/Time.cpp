// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Time.cpp

#include "Time.h"

////////////////////////////////////////////////////////////////////////////////
// CTime
////////////////////////////////////////////////////////////////////////////////

#if defined( TIME_USE_ANSI ) || defined( TIME_USE_LINUX )
tm *CTime::m_poTM_;
#elif defined( TIME_USE_WINDOWS )
SYSTEMTIME CTime::m_oTime_;
#	if defined( TIME_CHECK_HIGHRES )
double CTime::m_dTimerFrequencyFac_;
LARGE_INTEGER CTime::m_i64Frequency_;
	#endif // TIME_CHECK_HIGHRES
#endif // platform

unsigned int CTime::m_uiInstances_ = 0;
bool CTime::m_bUseHighRes_ = false;

CTime::CTime()
{
	if( !m_uiInstances_ )
	{
#if defined( TIME_USE_ANSI )
#elif defined( TIME_USE_WINDOWS )
		timeBeginPeriod( 0 );
#	if defined( TIME_CHECK_HIGHRES )
		// MSDN: If the installed hardware supports a high-resolution
		//       performance counter, the return value is nonzero.
		if( QueryPerformanceFrequency( &m_i64Frequency_ ) != 0 )
		{
			m_bUseHighRes_ = true;
			m_dTimerFrequencyFac_ = 1.0 / m_i64Frequency_.QuadPart;
		}
#	endif // TIME_CHECK_HIGHRES
#elif defined( TIME_USE_LINUX )
#	if defined( TIME_CHECK_HIGHRES )
		m_bUseHighRes_ = true;
#	endif // TIME_CHECK_HIGHRES
#endif // platform
	}
	Reset();
	Update();
	++m_uiInstances_;
}

CTime::~CTime()
{
	--m_uiInstances_;
#if defined( TIME_USE_ANSI )
#elif defined( TIME_USE_WINDOWS )
	if( !m_uiInstances_ )
		timeEndPeriod( 0 );
#endif // platform
}

void CTime::Update()
{
#if defined( TIME_USE_ANSI ) || defined( TIME_USE_LINUX )
	time_t lTime;
	time( &lTime );
	m_poTM_ = localtime( &lTime );
#elif defined( TIME_USE_WINDOWS )
	GetLocalTime( &m_oTime_ );
#endif // platform
}

const char * CTime::GetString() const
{
	static  char acTime_[21];
	TIME_SPRINTF( &acTime_[0], 21, "%.4d-%.2d-%.2d_%.2d.%.2d.%.2d",
		GetYear(), GetMonth(), GetDay(),
		GetHour(), GetMinute(), GetSecond() );
	return acTime_;
}
