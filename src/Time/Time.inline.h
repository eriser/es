// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Time.inline.h

inline void CTime::Reset()
{
#if defined( TIME_USE_ANSI )
	m_oStart = clock();
#elif defined( TIME_USE_WINDOWS )
#	if defined( TIME_CHECK_HIGHRES )
	if( m_bUseHighRes_ )
		QueryPerformanceCounter( &m_i64CountStart );
	else
#	endif // TIME_CHECK_HIGHRES
		m_dwStart = timeGetTime();
#elif defined( TIME_USE_LINUX )
#	if defined( TIME_CHECK_HIGHRES )
	if( m_bUseHighRes_ )
		clock_gettime( CLOCK_MONOTONIC, &m_oStartHR );				// UNTESTED
	else
#	endif // TIME_CHECK_HIGHRES
		gettimeofday( &m_oStart, 0 );
#endif // platform
}

inline double CTime::GetSeconds() const
{
#if defined( TIME_USE_ANSI )
	return ( clock() - m_oStart ) * ( 1.0 / CLOCKS_PER_SEC );
#elif defined( TIME_USE_WINDOWS )
#	if defined( TIME_CHECK_HIGHRES )
	if( m_bUseHighRes_ )
	{
		LARGE_INTEGER i64CountCurr;
		QueryPerformanceCounter( &i64CountCurr );
		return ( double( i64CountCurr.QuadPart - m_i64CountStart.QuadPart ) )
			* m_dTimerFrequencyFac_;
	}
#	endif // TIME_CHECK_HIGHRES
	return double( timeGetTime() - m_dwStart ) * ( 1.0 / 1000.0 );
#elif defined( TIME_USE_LINUX )
#	if defined( TIME_CHECK_HIGHRES )						// UNTESTED
	if( m_bUseHighRes_ )
	{
		struct timespec oNow;
		clock_gettime( CLOCK_MONOTONIC, &oNow );
		return double(  oNow.tv_sec - m_oStartHR.tv_sec ) +
		       ( oNow.tv_nsec - m_oStartHR.tv_nsec ) * ( 1.0 / 1000000000.0 );
	}
#	endif // TIME_CHECK_HIGHRES							// UNTESTED
	struct timeval oNow;
	gettimeofday( &oNow, 0 );
	return double( oNow.tv_sec - m_oStart.tv_sec )
		+ ( oNow.tv_usec - m_oStart.tv_usec ) * ( 1.0 / 1000000.0 );
#endif // platform
}

inline unsigned long int CTime::GetMilliseconds() const
{
#if defined( TIME_USE_ANSI )
	return (unsigned long int)( 1000 * ( clock() - m_oStart ) / CLOCKS_PER_SEC );
#elif defined( TIME_USE_WINDOWS )
#	if defined( TIME_CHECK_HIGHRES )
	if( m_bUseHighRes_ )
	{
		LARGE_INTEGER i64CountCurr;
		QueryPerformanceCounter( &i64CountCurr );
		return (unsigned long int)( 1000 * ( i64CountCurr.QuadPart - m_i64CountStart.QuadPart )
			/ m_i64Frequency_.QuadPart );
	}
#	endif // TIME_CHECK_HIGHRES
	return (unsigned long int)( timeGetTime() - m_dwStart );
#elif defined( TIME_USE_LINUX )
#	if defined( TIME_CHECK_HIGHRES )  						// UNTESTED
	if( m_bUseHighRes_ )
	{
		struct timespec oNow;
		clock_gettime( CLOCK_MONOTONIC, &oNow );
		return ( oNow.tv_sec - m_oStartHR.tv_sec ) * 1000
			+ ( oNow.tv_nsec - m_oStartHR.tv_nsec ) / 1000000;
	}
#	endif // TIME_CHECK_HIGHRES							// UNTESTED
	struct timeval oNow;
	gettimeofday( &oNow, 0 );
	return ( oNow.tv_sec - m_oStart.tv_sec ) * 1000
		+ ( oNow.tv_usec - m_oStart.tv_usec ) / 1000;
#endif // platform
}

inline unsigned int CTime::GetYear() const {
#if defined( TIME_USE_ANSI ) || defined( TIME_USE_LINUX )
	return m_poTM_->tm_year + 1900;
#elif defined( TIME_USE_WINDOWS )
	return m_oTime_.wYear;
#endif // platform
}

inline unsigned int CTime::GetMonth() const {
#if defined( TIME_USE_ANSI ) || defined( TIME_USE_LINUX )
	return m_poTM_->tm_mon + 1;
#elif defined( TIME_USE_WINDOWS )
	return m_oTime_.wMonth;
#endif // platform
}

inline unsigned int CTime::GetDay() const {
#if defined( TIME_USE_ANSI ) || defined( TIME_USE_LINUX )
	return m_poTM_->tm_mday;
#elif defined( TIME_USE_WINDOWS )
	return m_oTime_.wDay;
#endif // platform
}

inline unsigned int CTime::GetHour() const {
#if defined( TIME_USE_ANSI ) || defined( TIME_USE_LINUX )
	return m_poTM_->tm_hour;
#elif defined( TIME_USE_WINDOWS )
	return m_oTime_.wHour;
#endif // platform
}

inline unsigned int CTime::GetMinute() const {
#if defined( TIME_USE_ANSI ) || defined( TIME_USE_LINUX )
	return m_poTM_->tm_min;
#elif defined( TIME_USE_WINDOWS )
	return m_oTime_.wMinute;
#endif // platform
}

inline unsigned int CTime::GetSecond() const {
#if defined( TIME_USE_ANSI ) || defined( TIME_USE_LINUX )
	return m_poTM_->tm_sec;
#elif defined( TIME_USE_WINDOWS )
	return m_oTime_.wSecond;
#endif // platform
}
