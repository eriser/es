// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Wnd.inline.h

#ifndef WND_INLINE_H
#define WND_INLINE_H


inline int CWnd::GetWidth() const {
	return ( ( m_bIsFullScreen )?( m_iScreenWidth ):( m_iWindowWidth ) );
}

inline int CWnd::GetHeight() const {
	return ( ( m_bIsFullScreen )?( m_iScreenHeight ):( m_iWindowHeight ) );
}

inline int CWnd::GetDesktopWidth() const {
	return m_iDesktopWidth;
}

inline int CWnd::GetDesktopHeight() const {
	return m_iDesktopHeight;
}

inline int CWnd::GetPosX() const {
	return ( ( m_bIsFullScreen )?( 0 ):( m_iPosX ) );
}

inline int CWnd::GetPosY() const {
	return ( ( m_bIsFullScreen )?( 0 ):( m_iPosY ) );
}

inline unsigned int CWnd::GetCursor() const {
	return m_uiCursor;
}

inline bool CWnd::IsCursorInside() const
{
	if( !IsForeground() || 
		m_bIsMouseInResizeArea || m_bIsMouseInMoveArea )
		return false;
	const int iPosX( GetPosX() );
	const int iPosY( GetPosY() );
	int iX, iY;
	GetGlobalCursorPos( &iX, &iY );
	return (iX > iPosX &&
		iY > iPosY &&
		iX < iPosX + GetWidth() &&
		iY < iPosY + GetHeight() );
}

#if defined( __WINDOWS__ )

inline void CWndMS::UpdateTitle() {
	::SetWindowText( m_hWnd, LPCTSTR( m_pcTitle ) );
}

inline void CWndMS::ToggleScreenMode()
{
	::PostMessage( m_hWnd, WM_QUIT, 0, 0 );
	m_bIsToggleScreenMode = true;
}

inline void CWndMS::UpdateSizeAndPos()
{
	if( m_iPosX < 0 )
		m_iPosX = 0;
	if( m_iPosY < 0 )
		m_iPosY = 0;
	if( m_iPosX + m_iWindowWidth > m_iDesktopWidth )
		m_iPosX = m_iDesktopWidth - m_iWindowWidth;
	if( m_iPosY + m_iWindowHeight > m_iDesktopHeight )
		m_iPosY = m_iDesktopHeight - m_iWindowHeight;

	RECT rect = { m_iPosX, m_iPosY,  m_iPosX + m_iWindowWidth, m_iPosY + m_iWindowHeight } ;
	::AdjustWindowRectEx( &rect, m_dwStyle, false, m_dwStyleEx );

	::MoveWindow(
		m_hWnd,
		rect.left,
		rect.top,
		rect.right-rect.left,
		rect.bottom-rect.top,
		true );

	//InvalidateRect( m_hWnd, 0, true );
	//UpdateWindow( m_hWnd );
}

void CWndMS::SwapBuffers()
{
	::SwapBuffers( m_hDC );
}

inline void CWndMS::SetMouseCapture()
{
	::SetCapture( m_hWnd );
	m_bIsCaptured = true;
}

inline void CWndMS::ReleaseMouseCapture()
{
	m_bIsCaptured = false;
	::ReleaseCapture();
}

inline void CWndMS::SetCursor( unsigned int uiCursor )
{
	if( uiCursor != m_uiCursor )
	{
		m_uiCursor = uiCursor;
		::SendMessage( m_hWnd, WM_SETCURSOR, 0, 0 );
	}
}

inline void CWndMS::ShowCursor( bool bShow )
{
	if( bShow == m_bIsVisibleCursor )
		return;
	::ShowCursor( bShow );
	m_bIsVisibleCursor = bShow;
}

inline void CWndMS::BringToTop() const
{
	// ???
	::BringWindowToTop( m_hWnd );
	//::ShowWindow( m_hWnd, SW_SHOW ); //NORMAL );
	//::SetFocus( m_hWnd );
	//::SetActiveWindow( m_hWnd );
	::SetForegroundWindow( m_hWnd );
	//::SwitchToThisWindow( m_hWnd, TRUE );
}

inline void CWndMS::GetGlobalCursorPos( int *piX, int *piY ) const
{
	POINT pnt;
	::GetCursorPos( &pnt );
	*piX = pnt.x;
	*piY = pnt.y;
}

inline void CWndMS::SetGlobalCursorPos( int iX, int iY ) const {
	::SetCursorPos( iX, iY );
}

inline bool CWndMS::IsForeground() const {
	return ( m_hWnd == ::GetForegroundWindow() ); // || m_hWnd == ::GetFocus() || m_hWnd == ::GetActiveWindow() ); // ???
}

#elif defined( __LINUX__ )

inline void CWndX11::UpdateTitle()
{
	XTextProperty textProp;
	textProp.value = (unsigned char *) m_pcTitle;
	textProp.encoding = XA_STRING;
	textProp.format = 8;
	textProp.nitems = strlen( m_pcTitle );

	XSetWMName( m_poDisplay, m_oWindow, &textProp );
}

inline void CWndX11::UpdateSizeAndPos()
{
	XMoveResizeWindow(
		m_poDisplay, m_oWindow,
		m_iPosX, m_iPosY,
		m_iWindowWidth, m_iWindowHeight );
}

void CWndX11::SwapBuffers()
{
	glXSwapBuffers( m_poDisplay, m_oWindow );
}

inline void CWndX11::SetMouseCapture()
{
	XGrabPointer(   m_poDisplay, m_oWindow, True,
			ButtonPressMask | ButtonReleaseMask |
			ButtonMotionMask |
			PointerMotionMask,
			GrabModeAsync, GrabModeAsync,
			m_oWindow, None, CurrentTime );
	m_bIsCaptured = true;
}

inline void CWndX11::ReleaseMouseCapture() {
	XUngrabPointer( m_poDisplay, CurrentTime );
	m_bIsCaptured = false;
}

inline void CWndX11::ShowCursor( bool bShow ) {
	UseCursor( bShow ? m_oCursorNormal : m_oCursorHidden );
}

inline void CWndX11::BringToTop() const {
	XMapRaised( m_poDisplay, m_oWindow );
}

inline void CWndX11::GetGlobalCursorPos( int *piX, int *piY ) const
{
	int root_x,root_y;
	int win_x, win_y;
	unsigned int mask;
	Window root, child;
	XQueryPointer( m_poDisplay, m_oWindow,
		&root, &child,
		&root_x, &root_y, &win_x, &win_y,
		&mask );
	*piX = root_x;
	*piY = root_y;
}

inline void CWndX11::SetGlobalCursorPos( int iX, int iY ) const
{
	// todo
	// XWarpPointer....
	// int x, y;
	// GetGlobalCursorPos( &x, &y );
}

inline bool CWndX11::IsForeground() const {
	// todo
	return true;
}

inline void CWndX11::SetFocus() const {
	XSetInputFocus( m_poDisplay, m_oWindow, RevertToParent, CurrentTime );
}

inline void CWndX11::UseCursor( Cursor oCursor ) const {
	XDefineCursor( m_poDisplay, m_oWindow, oCursor );
}

#endif // platform
#endif // WND_INLINE_H
