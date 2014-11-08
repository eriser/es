// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Wnd.h
//
// Windows:
// - opengl32.lib, glu32.lib.
// - dnd: shell32.lib
//
// Linux:
// -lGL -lGLU -lX11 -lXxf86vm
//

#ifndef _WND_H_
#define _WND_H_

#include "WndDEF.h"


// Fenster-ADT.
////////////////////////////////////////////////////////////////////////////////
class CWnd
{
public:
	class CHandler
	{
	public:
		CHandler();
		virtual ~CHandler();
	public:
		virtual bool OnInit() = 0;
		virtual void OnUpdate() = 0;
		virtual	void OnClear() = 0;
		virtual bool OnDisplay() = 0;
		virtual void OnKeyUp( unsigned int uiKey ) = 0;
		virtual void OnKeyDown( unsigned int uiKey ) = 0;
		virtual void OnChar( char cKey ) = 0;
		virtual void OnMouse( unsigned int uiButton, int iState ) = 0;
		virtual	void OnSize() = 0;
		virtual void OnDrop() = 0;
		virtual void OnCursor( bool bInside ) = 0;
	public:
		CWnd *m_poWnd;
	};

public:
	CWnd();
	virtual ~CWnd();

public:
	inline int GetWidth() const;
	inline int GetHeight() const;
	inline int GetDesktopWidth() const;
	inline int GetDesktopHeight() const;
	inline int GetPosX() const;
	inline int GetPosY() const;
	inline unsigned int GetCursor() const;
	inline bool IsCursorInside() const;
public:
	virtual bool Run() = 0;
	virtual bool Terminate() = 0;

	virtual void UpdateTitle() = 0;
	virtual void ToggleScreenMode() = 0;
	virtual void UpdateSizeAndPos() = 0;
	virtual void SwapBuffers() = 0;
	virtual void SetMouseCapture() = 0;
	virtual void ReleaseMouseCapture() = 0;
	virtual void SetCursor( unsigned int uiCursor ) = 0;
	virtual void ShowCursor( bool bShow ) = 0;
	virtual void BringToTop() const = 0;
	virtual void GetGlobalCursorPos( int *piX, int *piY ) const = 0;
	virtual void SetGlobalCursorPos( int iX, int iY ) const = 0;
	virtual bool IsForeground() const = 0;

protected:
#if defined( WND_USE_GLTXT ) && defined( WND_USE_GLTXT_BDF )
	void LoadFontBDF();
#endif // BDF
	bool Init();
	void BorderCheck();

public:
	CHandler * m_poHandler;
	const char *m_pcTitle;
	int m_iWindowWidth, m_iWindowHeight;
	int m_iScreenWidth, m_iScreenHeight;
	unsigned int m_uiColorBits;
	unsigned int m_uiDepthBits;
	unsigned int m_uiStencilBits;
	bool m_bIsFullScreen;
	
	int m_iPosX, m_iPosY;
	int m_iMousePosX, m_iMousePosY;
	bool m_abKeyDown[256];
	int m_aiMouseState[_WND_MOUSE_BUTTON_MAX];
	unsigned int m_uiCursor;

	bool m_bIsMouseInResizeArea, m_bIsMouseInMoveArea;
	bool m_bIsMouseXL, m_bIsMouseXR;
	bool m_bIsMouseYT, m_bIsMouseYB;
	bool m_bIsCaptured;

	bool m_bIsMoving;
	bool m_bIsResizing;

	unsigned int m_iMoveDiffX, m_iMoveDiffY;
	unsigned int m_aiResizeStart[8];

#ifdef WND_USE_DND
	CArray<CStr> m_oArrDrop;
#endif // WND_USE_DND
#ifdef WND_USE_GLTXT
	CGLTxtFont *m_poFont;
#endif // WND_USE_GLTXT

protected:
	int m_iDesktopWidth, m_iDesktopHeight;
	bool m_bHasOGLContext;
	
protected:
	template <class X, class Y>
	class SPair
	{
	public:
		SPair( X tX, Y tY ):
		m_tX( tX ),
		m_tY( tY )
		{}
		~SPair() {}
	public:
		X m_tX;
		Y m_tY;
	};

protected:
	typedef SPair<unsigned int, unsigned int> TPairMouse;
#if defined( __WINDOWS__ )
	typedef SPair<unsigned int, WPARAM> TPairKey;
	typedef SPair<unsigned int, LPCTSTR> TPairCursor;
#	define KEY( wndKey, winKey, X11Key ) CWnd::TPairKey( wndKey, winKey )
#	define CURSOR( wndCur, winCur, X11Cur ) CWnd::TPairCursor( wndCur, winCur )
#elif defined( __LINUX__ )
	typedef SPair<unsigned int, KeySym> TPairKey;
	typedef SPair<unsigned int, unsigned int> TPairCursor;
#	define KEY( wndKey, winKey, X11Key ) CWnd::TPairKey( wndKey, X11Key )
#	define CURSOR( wndCur, winCur, X11Cur ) CWnd::TPairCursor( wndCur, X11Cur )
#else
	platform not supported
#endif // platform

	static const TPairKey m_atMapKey_[];
	static const unsigned int m_uiMapKeyCount_;
	static const TPairCursor m_atMapCursor_[];
	static const unsigned int m_uiMapCursorCount_;
};




#if defined( __WINDOWS__ )

// MS Windows.
////////////////////////////////////////////////////////////////////////////////
class CWndMS : public CWnd
{
public:
	CWndMS();
	virtual ~CWndMS();
public:
	virtual bool Run();
	virtual bool Terminate();

	inline virtual void UpdateTitle();
	inline virtual void ToggleScreenMode();
	inline virtual void UpdateSizeAndPos();
	inline virtual void SwapBuffers();
	inline virtual void SetMouseCapture();
	inline virtual void ReleaseMouseCapture();
	inline virtual void SetCursor( unsigned int uiCursor );
	inline virtual void ShowCursor( bool bShow );
	inline virtual void BringToTop() const;
	inline virtual void GetGlobalCursorPos( int *piX, int *piY ) const;
	inline virtual void SetGlobalCursorPos( int iX, int iY ) const;
	inline virtual bool IsForeground() const;

private:
	void Destroy();
	static LRESULT CALLBACK Proc_( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	static bool ConvKey_( WPARAM wParam, unsigned int *puiKey );
	static HCURSOR CreateCursor_( unsigned int uiCursor );
	
public:
	HWND m_hWnd;
private:
	bool m_bIsLooping;
	bool m_bIsVisibleWnd;
	bool m_bIsVisibleCursor;
	bool m_bIsToggleScreenMode;
	bool m_bEnterMouse;
	HDC m_hDC;
	HGLRC m_hRC;

	DWORD m_dwStyle, m_dwStyleEx;
	
	// <mod date="2010-12-07">
private:
	static CWndMS * m_poWnd_;
	// </mod>
};

#elif defined( __LINUX__ )


// X Window System.
////////////////////////////////////////////////////////////////////////////////
class CWndX11 : public CWnd
{
public:
	CWndX11();
	virtual ~CWndX11();
public:
	virtual bool Run();
	virtual bool Terminate();

	inline virtual void UpdateTitle();
	inline virtual void ToggleScreenMode();
	inline virtual void UpdateSizeAndPos();
	inline virtual void SwapBuffers();
	inline virtual void SetMouseCapture();
	inline virtual void ReleaseMouseCapture();
	inline virtual void SetCursor( unsigned int uiCursor );
	inline virtual void ShowCursor( bool bShow );
	inline virtual void BringToTop() const;
	inline virtual void GetGlobalCursorPos( int *piX, int *piY ) const;
	inline virtual void SetGlobalCursorPos( int iX, int iY ) const; // todo!
	inline virtual bool IsForeground() const; // todo!

private:
	bool Init();
	void Clear();

	void SetKey( KeySym oKey, bool bState );
	void UpdatePosition();
	KeySym GetKey( unsigned int uiKeyCode ) const;
	inline void SetFocus() const;
	inline void UseCursor( Cursor oCursor ) const;
    
private:
	bool m_bIsLooping;
	bool m_bIsVisible;

	Display	*m_poDisplay;
	unsigned int m_uiBorder;
	int m_iScreen;
	unsigned long int m_uliEventMask;
	bool m_bIsDoubleBuffered;
	Window m_oWindow;
	Window m_oWindowRoot;
	XF86VidModeModeInfo m_oVideoModeInfoPrev;
	GC m_oGC;
	GLXContext m_oContextGLX;
	Atom m_oAtomWMDelete;
	Atom m_oAtomDndProtocol, m_oAtomDndSelection;
	Cursor m_oCursorNormal;
	Cursor m_oCursorHidden;
	Pixmap m_oPixmapCursorHidden;
	Colormap m_oColormapCursorHidden;
	XColor m_oColorBlack;
	unsigned long int m_uliDoubleClickDelay;

#ifdef WND_USE_DND
	Dnd m_oDND;
#endif // WND_USE_DND
};

#else
#error platform not supported
#endif // platform

#include "Wnd.inline.h"


#if defined( __WINDOWS__ )
typedef CWndMS TWnd;
#elif defined( __LINUX__ )
typedef CWndX11 TWnd;
#endif // platform


#endif
