// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Wnd.cpp

#include "Wnd.h"
#include "VSync.h"

////////////////////////////////////////////////////////////////////////////////
// CWnd::CHandler
////////////////////////////////////////////////////////////////////////////////

CWnd::CHandler::CHandler()
{}

CWnd::CHandler::~CHandler()
{}

////////////////////////////////////////////////////////////////////////////////
// CWnd
////////////////////////////////////////////////////////////////////////////////

// http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/xlib-programming.html#user_keyboard_focus
const CWnd::TPairKey CWnd::m_atMapKey_[] =
{
	KEY( WND_KEY_BACKSPACE, VK_BACK, XK_BackSpace ),
	KEY( WND_KEY_TAB, VK_TAB, XK_Tab ),
	//	KEY( WND_KEY_LINEFEED, XK_Linefeed ),
	KEY( WND_KEY_CLEAR, VK_CLEAR, XK_Clear ),
	KEY( WND_KEY_RETURN, VK_RETURN, XK_Return ),
	KEY( WND_KEY_PAUSE, VK_PAUSE, XK_Pause ),
	KEY( WND_KEY_SCROLL_LOCK, VK_SCROLL, XK_Scroll_Lock ),
	//	KEY( WND_KEY_SYS_REQ, XK_Sys_Req ),
	KEY( WND_KEY_ESCAPE, VK_ESCAPE, XK_Escape ),
	KEY( WND_KEY_DELETE, VK_DELETE, XK_Delete ),

	KEY( WND_KEY_HOME, VK_HOME, XK_Home ),
	KEY( WND_KEY_LEFT, VK_LEFT, XK_Left ),
	KEY( WND_KEY_UP, VK_UP, XK_Up ),
	KEY( WND_KEY_RIGHT, VK_RIGHT, XK_Right ),
	KEY( WND_KEY_DOWN, VK_DOWN, XK_Down ),
	KEY( WND_KEY_PAGE_UP, VK_PRIOR, XK_Prior ),
	KEY( WND_KEY_PAGE_UP, VK_PRIOR, XK_Page_Up ),
	KEY( WND_KEY_PAGE_DOWN, VK_NEXT, XK_Next ),
	KEY( WND_KEY_PAGE_DOWN, VK_NEXT, XK_Page_Down ),
	KEY( WND_KEY_END, VK_END, XK_End ),
	//	KEY( WND_KEY_BEGIN, VK_BEGIN ),

	KEY( WND_KEY_SELECT, VK_SELECT, XK_Select ),
	KEY( WND_KEY_PRINT, VK_PRINT, XK_Print ),
	KEY( WND_KEY_EXECUTE, VK_EXECUTE, XK_Execute ),
	KEY( WND_KEY_INSERT, VK_INSERT, XK_Insert ),
	//	KEY( WND_KEY_UNDO, XK_Undo ),
	//	KEY( WND_KEY_REDO, XK_Redo ),
	KEY( WND_KEY_ALT, VK_MENU, XK_Menu ), // ??? lmenu rmenu
	//	KEY( WND_KEY_FIND, XK_Find ),
	KEY( WND_KEY_CANCEL, VK_CANCEL, XK_Cancel ),
	KEY( WND_KEY_HELP, VK_HELP, XK_Help ),
	//	KEY( WND_KEY_BREAK, XK_Break ),
	//	KEY( WND_KEY_MODE_SWITCH, XK_Mode_switch ),
	//	KEY( WND_KEY_SCRIPT_SWITCH, XK_script_switch ),
	KEY( WND_KEY_NUM_LOCK, VK_NUMLOCK, XK_Num_Lock ),

	//	KEY( WND_KEY_KP_SPACE, XK_KP_Space ),
	//	KEY( WND_KEY_KP_TAB, XK_KP_Tab ),
	//	KEY( WND_KEY_KP_ENTER, XK_KP_Enter ),
	//	KEY( WND_KEY_KP_F1, XK_KP_F1 ),
	//	KEY( WND_KEY_KP_F2, XK_KP_F2 ),
	//	KEY( WND_KEY_KP_F3, XK_KP_F3 ),
	//	KEY( WND_KEY_KP_F4, XK_KP_F4 ),
	//	KEY( WND_KEY_KP_HOME, XK_KP_Home ),
	//	KEY( WND_KEY_KP_LEFT, XK_KP_Left ),
	//	KEY( WND_KEY_KP_UP, XK_KP_Up ),
	//	KEY( WND_KEY_KP_RIGHT, XK_KP_Right ),
	//	KEY( WND_KEY_KP_DOWN, XK_KP_Down ),
	//	KEY( WND_KEY_KP_PRIOR, XK_KP_Prior ),
	//	KEY( WND_KEY_KP_PAGE_UP, XK_KP_Page_Up ),
	//	KEY( WND_KEY_KP_NEXT, XK_KP_Next ),
	//	KEY( WND_KEY_KP_PAGE_DOWN, XK_KP_Page_Down ),
	//	KEY( WND_KEY_KP_END, XK_KP_End ),
	//	KEY( WND_KEY_KP_BEGIN, XK_KP_Begin ),
	//	KEY( WND_KEY_KP_INSERT, XK_KP_Insert ),
	//	KEY( WND_KEY_KP_DELETE, XK_KP_Delete ),
	//	KEY( WND_KEY_KP_EQUAL, XK_KP_Equal ),
	//	KEY( WND_KEY_KP_MULTIPLY, XK_KP_Multiply ),
	//	KEY( WND_KEY_KP_ADD, XK_KP_Add ),
	//	KEY( WND_KEY_KP_SEPARATOR, XK_KP_Separator ),
	//	KEY( WND_KEY_KP_SUBSTRACT, XK_KP_Subtract ),
	//	KEY( WND_KEY_KP_DECIMAL, XK_KP_Decimal ),
	//	KEY( WND_KEY_KP_DIVIDE, XK_KP_Divide ),

	KEY( WND_KEY_KP_0, VK_NUMPAD0, XK_KP_0 ),
	KEY( WND_KEY_KP_1, VK_NUMPAD1, XK_KP_1 ),
	KEY( WND_KEY_KP_2, VK_NUMPAD2, XK_KP_2 ),
	KEY( WND_KEY_KP_3, VK_NUMPAD3, XK_KP_3 ),
	KEY( WND_KEY_KP_4, VK_NUMPAD4, XK_KP_4 ),
	KEY( WND_KEY_KP_5, VK_NUMPAD5, XK_KP_5 ),
	KEY( WND_KEY_KP_6, VK_NUMPAD6, XK_KP_6 ),
	KEY( WND_KEY_KP_7, VK_NUMPAD7, XK_KP_7 ),
	KEY( WND_KEY_KP_8, VK_NUMPAD8, XK_KP_8 ),
	KEY( WND_KEY_KP_9, VK_NUMPAD9, XK_KP_9 ),

	KEY( WND_KEY_F1, VK_F1, XK_F1 ),
	KEY( WND_KEY_F2, VK_F2, XK_F2 ),
	KEY( WND_KEY_F3, VK_F3, XK_F3 ),
	KEY( WND_KEY_F4, VK_F4, XK_F4 ),
	KEY( WND_KEY_F5, VK_F5, XK_F5 ),
	KEY( WND_KEY_F6, VK_F6, XK_F6 ),
	KEY( WND_KEY_F7, VK_F7, XK_F7 ),
	KEY( WND_KEY_F8, VK_F8, XK_F8 ),
	KEY( WND_KEY_F9, VK_F9, XK_F9 ),
	KEY( WND_KEY_F10, VK_F10, XK_F10 ),
	KEY( WND_KEY_F11, VK_F11, XK_F11 ),
	//	KEY( WND_KEY_L1, XK_L1 ),
	KEY( WND_KEY_F12, VK_F12, XK_F12 ),
	//	KEY( WND_KEY_L2, XK_L2 ),
	KEY( WND_KEY_F13, VK_F13, XK_F13 ),
	//	KEY( WND_KEY_L3, XK_L3 ),
	KEY( WND_KEY_F14, VK_F14, XK_F14 ),
	//	KEY( WND_KEY_L4, XK_L4 ),
	KEY( WND_KEY_F15, VK_F15, XK_F15 ),
	//	KEY( WND_KEY_L5, XK_L5 ),
	KEY( WND_KEY_F16, VK_F16, XK_F16 ),
	//	KEY( WND_KEY_L6, XK_L6 ),
	KEY( WND_KEY_F17, VK_F17, XK_F17 ),
	//	KEY( WND_KEY_L7, XK_L7 ),
	KEY( WND_KEY_F18, VK_F18, XK_F18 ),
	//	KEY( WND_KEY_L8, XK_L8 ),
	KEY( WND_KEY_F19, VK_F19, XK_F19 ),
	//	KEY( WND_KEY_L9, XK_L9 ),
	KEY( WND_KEY_F20, VK_F20, XK_F20 ),
	//	KEY( WND_KEY_L10, VK_L10 ),
	KEY( WND_KEY_F21, VK_F21, XK_F21 ),
	//	KEY( WND_KEY_R1, XK_R1 ),
	KEY( WND_KEY_F22, VK_F22, XK_F22 ),
	//	KEY( WND_KEY_R2, XK_R2 ),
	KEY( WND_KEY_F23, VK_F23, XK_F23 ),
	//	KEY( WND_KEY_R3, XK_R3 ),
	KEY( WND_KEY_F24, VK_F24, XK_F24 ),
	//	KEY( WND_KEY_R4, XK_R4 ),
	//	KEY( WND_KEY_F25, XK_F25 ),
	//	KEY( WND_KEY_R5, XK_R5 ),
	//	KEY( WND_KEY_F26, XK_F26 ),
	//	KEY( WND_KEY_R6, XK_R6 ),
	//	KEY( WND_KEY_F27, XK_F27 ),
	//	KEY( WND_KEY_R7, XK_R7 ),
	//	KEY( WND_KEY_F28, XK_F28 ),
	//	KEY( WND_KEY_R8, XK_R8 ),
	//	KEY( WND_KEY_F29, XK_F29 ),
	//	KEY( WND_KEY_R9, XK_R9 ),
	//	KEY( WND_KEY_F30, XK_F30 ),
	//	KEY( WND_KEY_R10, XK_R10 ),
	//	KEY( WND_KEY_F31, XK_F31 ),
	//	KEY( WND_KEY_R11, XK_R11 ),
	//	KEY( WND_KEY_F32, XK_F32 ),
	//	KEY( WND_KEY_R12, XK_R12 ),
	//	KEY( WND_KEY_F33, XK_F33 ),
	//	KEY( WND_KEY_R13, XK_R13 ),
	//	KEY( WND_KEY_F34, XK_F34 ),
	//	KEY( WND_KEY_R14, XK_R14 ),
	//	KEY( WND_KEY_F35, XK_F35 ),
	//	KEY( WND_KEY_R15, XK_R15 ),

	KEY( WND_KEY_SHIFT, VK_SHIFT, XK_Shift_L ), //VK_LSHIFT
	KEY( WND_KEY_SHIFT, VK_SHIFT, XK_Shift_R ), //VK_RSHIFT
	KEY( WND_KEY_CONTROL, VK_CONTROL, XK_Control_L ), //VK_LCONTROL
	KEY( WND_KEY_CONTROL, VK_CONTROL, XK_Control_R ), //VK_RCONTROL
	KEY( WND_KEY_CAPS_LOCK, VK_CAPITAL, XK_Caps_Lock ),
	//	KEY( WND_KEY_SHIFT_LOCK, XK_Shift_Lock ),

	//	KEY( WND_KEY_META_L, XK_Meta_L ),
	//	KEY( WND_KEY_META_R, XK_Meta_R ),
	KEY( WND_KEY_ALT, VK_LMENU, XK_Alt_L ),
	KEY( WND_KEY_ALT, VK_RMENU, XK_Alt_R ),
	//	KEY( WND_KEY_SUPER_L, XK_Super_L ),
	//	KEY( WND_KEY_SUPER_R, XK_Super_R ),
	//	KEY( WND_KEY_HYPER_L, XK_Hyper_L ),
	//	KEY( WND_KEY_HYPER_R, XK_Hyper_R ),

	KEY( WND_KEY_SPACE, VK_SPACE, XK_space ),
	//	KEY( WND_KEY_EXCLAM, XK_exclam ),
	//	KEY( WND_KEY_QUOTEDBL, XK_quotedbl ),
	//	KEY( WND_KEY_NUMBERSIGN, XK_numbersign ),
	//	KEY( WND_KEY_DOLLAR, XK_dollar ),
	//	KEY( WND_KEY_PERCENT, XK_percent ),
	//	KEY( WND_KEY_AMPERSAND, XK_ampersand ),
	//	KEY( WND_KEY_APOSTROPHE, XK_apostrophe ),
	//	KEY( WND_KEY_QUOTERIGHT, XK_quoteright ),
	//	KEY( WND_KEY_PARENLEFT, XK_parenleft ),
	//	KEY( WND_KEY_PARENRIGHT, XK_parenright ),
	//	KEY( WND_KEY_ASTERISK, XK_asterisk ),
	//KEY( WND_KEY_PLUS, VK_OEM_PLUS, XK_plus ), // mingw...
	//KEY( WND_KEY_COMMA, VK_OEM_COMMA, XK_comma ),
	//KEY( WND_KEY_MINUS, VK_OEM_MINUS, XK_minus ),
	//KEY( WND_KEY_PERIOD, VK_OEM_PERIOD, XK_period ),
	KEY( WND_KEY_SLASH, VK_OEM_2, XK_slash ), // VK_OEM_102 ???
	KEY( WND_KEY_0, 0x30, XK_0 ),
	KEY( WND_KEY_1, 0x31, XK_1 ),
	KEY( WND_KEY_2, 0x32, XK_2 ),
	KEY( WND_KEY_3, 0x33, XK_3 ),
	KEY( WND_KEY_4, 0x34, XK_4 ),
	KEY( WND_KEY_5, 0x35, XK_5 ),
	KEY( WND_KEY_6, 0x36, XK_6 ),
	KEY( WND_KEY_7, 0x37, XK_7 ),
	KEY( WND_KEY_8, 0x38, XK_8 ),
	KEY( WND_KEY_9, 0x39, XK_9 ),
	//	KEY( WND_KEY_COLON, XK_colon ),
	//	KEY( WND_KEY_SEMICOLON, XK_semicolon ),
	//	KEY( WND_KEY_LESS, XK_less ),
	//	KEY( WND_KEY_EQUAL, XK_equal ),
	//	KEY( WND_KEY_GREATER, XK_greater ),
	//	KEY( WND_KEY_QUESTION, XK_question ),
	//	KEY( WND_KEY_AT, XK_at ),

	// NEW!
	/*
	KEY( WND_KEY_CAP_A, 0x41, XK_A ),
	KEY( WND_KEY_CAP_B, 0x42, XK_B ),
	KEY( WND_KEY_CAP_C, 0x43, XK_C ),
	KEY( WND_KEY_CAP_D, 0x44, XK_D ),
	KEY( WND_KEY_CAP_E, 0x45, XK_E ),
	KEY( WND_KEY_CAP_F, 0x46, XK_F ),
	KEY( WND_KEY_CAP_G, 0x47, XK_G ),
	KEY( WND_KEY_CAP_H, 0x48, XK_H ),
	KEY( WND_KEY_CAP_I, 0x49, XK_I ),
	KEY( WND_KEY_CAP_J, 0x4A, XK_J ),
	KEY( WND_KEY_CAP_K, 0x4B, XK_K ),
	KEY( WND_KEY_CAP_L, 0x4C, XK_L ),
	KEY( WND_KEY_CAP_M, 0x4D, XK_M ),
	KEY( WND_KEY_CAP_N, 0x4E, XK_N ),
	KEY( WND_KEY_CAP_O, 0x4F, XK_O ),
	KEY( WND_KEY_CAP_P, 0x50, XK_P ),
	KEY( WND_KEY_CAP_Q, 0x51, XK_Q ),
	KEY( WND_KEY_CAP_R, 0x52, XK_R ),
	KEY( WND_KEY_CAP_S, 0x53, XK_S ),
	KEY( WND_KEY_CAP_T, 0x54, XK_T ),
	KEY( WND_KEY_CAP_U, 0x55, XK_U ),
	KEY( WND_KEY_CAP_V, 0x56, XK_V ),
	KEY( WND_KEY_CAP_W, 0x57, XK_W ),
	KEY( WND_KEY_CAP_X, 0x58, XK_X ),
	KEY( WND_KEY_CAP_Y, 0x59, XK_Y ),
	KEY( WND_KEY_CAP_Z, 0x5A, XK_Z ),
	*/
	KEY( WND_KEY_A, 0x41, XK_A ),
	KEY( WND_KEY_B, 0x42, XK_B ),
	KEY( WND_KEY_C, 0x43, XK_C ),
	KEY( WND_KEY_D, 0x44, XK_D ),
	KEY( WND_KEY_E, 0x45, XK_E ),
	KEY( WND_KEY_F, 0x46, XK_F ),
	KEY( WND_KEY_G, 0x47, XK_G ),
	KEY( WND_KEY_H, 0x48, XK_H ),
	KEY( WND_KEY_I, 0x49, XK_I ),
	KEY( WND_KEY_J, 0x4A, XK_J ),
	KEY( WND_KEY_K, 0x4B, XK_K ),
	KEY( WND_KEY_L, 0x4C, XK_L ),
	KEY( WND_KEY_M, 0x4D, XK_M ),
	KEY( WND_KEY_N, 0x4E, XK_N ),
	KEY( WND_KEY_O, 0x4F, XK_O ),
	KEY( WND_KEY_P, 0x50, XK_P ),
	KEY( WND_KEY_Q, 0x51, XK_Q ),
	KEY( WND_KEY_R, 0x52, XK_R ),
	KEY( WND_KEY_S, 0x53, XK_S ),
	KEY( WND_KEY_T, 0x54, XK_T ),
	KEY( WND_KEY_U, 0x55, XK_U ),
	KEY( WND_KEY_V, 0x56, XK_V ),
	KEY( WND_KEY_W, 0x57, XK_W ),
	KEY( WND_KEY_X, 0x58, XK_X ),
	KEY( WND_KEY_Y, 0x59, XK_Y ),
	KEY( WND_KEY_Z, 0x5A, XK_Z ),

	//	KEY( WND_KEY_BRACKETLEFT, XK_bracketleft ),
	//	KEY( WND_KEY_BACKSLASH, XK_backslash ),
	//	KEY( WND_KEY_BRACKETRIGHT, XK_bracketright ),
	//	KEY( WND_KEY_ASCIICIRCUM, XK_asciicircum ),
	//	KEY( WND_KEY_UNDERSCORE, XK_underscore ),
	//	KEY( WND_KEY_GRAVE, XK_grave ),
	//	KEY( WND_KEY_QUOTELEFT, XK_quoteleft ), // VK_OEM_7?

	KEY( WND_KEY_A, 0x41, XK_a ),
	KEY( WND_KEY_B, 0x42, XK_b ),
	KEY( WND_KEY_C, 0x43, XK_c ),
	KEY( WND_KEY_D, 0x44, XK_d ),
	KEY( WND_KEY_E, 0x45, XK_e ),
	KEY( WND_KEY_F, 0x46, XK_f ),
	KEY( WND_KEY_G, 0x47, XK_g ),
	KEY( WND_KEY_H, 0x48, XK_h ),
	KEY( WND_KEY_I, 0x49, XK_i ),
	KEY( WND_KEY_J, 0x4A, XK_j ),
	KEY( WND_KEY_K, 0x4B, XK_k ),
	KEY( WND_KEY_L, 0x4C, XK_l ),
	KEY( WND_KEY_M, 0x4D, XK_m ),
	KEY( WND_KEY_N, 0x4E, XK_n ),
	KEY( WND_KEY_O, 0x4F, XK_o ),
	KEY( WND_KEY_P, 0x50, XK_p ),
	KEY( WND_KEY_Q, 0x51, XK_q ),
	KEY( WND_KEY_R, 0x52, XK_r ),
	KEY( WND_KEY_S, 0x53, XK_s ),
	KEY( WND_KEY_T, 0x54, XK_t ),
	KEY( WND_KEY_U, 0x55, XK_u ),
	KEY( WND_KEY_V, 0x56, XK_v ),
	KEY( WND_KEY_W, 0x57, XK_w ),
	KEY( WND_KEY_X, 0x58, XK_x ),
	KEY( WND_KEY_Y, 0x59, XK_y ),
	KEY( WND_KEY_Z, 0x5A, XK_z )

	//	KEY( WND_KEY_BRACELEFT, XK_braceleft ),
	//	KEY( WND_KEY_BAR, XK_bar ),
	//	KEY( WND_KEY_BRACERIGHT, XK_braceright ),
	//	KEY( WND_KEY_ASCIITILDE, XK_asciitilde )
};
const unsigned int CWnd::m_uiMapKeyCount_ = sizeof( m_atMapKey_ ) / sizeof( TPairKey );


// http://tronche.com/gui/x/xlib/appendix/b/
const CWnd::TPairCursor CWnd::m_atMapCursor_[] =
{
	CURSOR( WND_CURSOR_DEFAULT, IDC_ARROW, XC_left_ptr ),
	CURSOR( WND_CURSOR_CROSS, IDC_CROSS, XC_crosshair ),
	CURSOR( WND_CURSOR_ARROW_H, IDC_SIZEWE, XC_sb_h_double_arrow ),
	CURSOR( WND_CURSOR_ARROW_V, IDC_SIZENS, XC_sb_v_double_arrow ),
	CURSOR( WND_CURSOR_LEFT, IDC_SIZEWE, XC_left_side ),
	CURSOR( WND_CURSOR_RIGHT, IDC_SIZEWE, XC_right_side ),
	CURSOR( WND_CURSOR_BOTTOM, IDC_SIZENS, XC_bottom_side ),
	CURSOR( WND_CURSOR_TOP, IDC_SIZENS, XC_top_side ),
	CURSOR( WND_CURSOR_BOTTOM_LEFT, IDC_SIZENESW, XC_bottom_left_corner ),
	CURSOR( WND_CURSOR_BOTTOM_RIGHT, IDC_SIZENWSE, XC_bottom_right_corner ),
	CURSOR( WND_CUROSR_TOP_LEFT, IDC_SIZENWSE, XC_top_left_corner ),
	CURSOR( WND_CURSOR_TOP_RIGHT, IDC_SIZENESW, XC_top_right_corner ),
	CURSOR( WND_CURSOR_SIZEALL, IDC_SIZEALL, XC_fleur ) // ???
};
const unsigned int CWnd::m_uiMapCursorCount_ = sizeof( m_atMapCursor_ ) / sizeof( TPairCursor );


CWnd::CWnd()
: m_poHandler( 0 )
, m_pcTitle( 0 )
, m_iWindowWidth( 500 )
, m_iWindowHeight( 500 )
, m_iScreenWidth( 0 )
, m_iScreenHeight( 0 )
, m_uiColorBits( WND_COLOR_BUFFER_BITS )
, m_uiDepthBits( WND_DEPTH_BUFFER_BITS )
, m_uiStencilBits( 8 )
, m_bIsFullScreen( false )
, m_iPosX( 100 )
, m_iPosY( 100 )
, m_iMousePosX( 0 )
, m_iMousePosY( 0 )
, m_uiCursor( WND_CURSOR_DEFAULT )
, m_bIsMouseInResizeArea( false )
, m_bIsMouseInMoveArea( false )
, m_bIsMouseXL( false )
, m_bIsMouseXR( false )
, m_bIsMouseYT( false )
, m_bIsMouseYB( false )
, m_bIsCaptured( false )
, m_bIsMoving( false )
, m_bIsResizing( false )
, m_iMoveDiffX( 0 )
, m_iMoveDiffY( 0 )
#if defined( WND_USE_GLTXT )
, m_poFont( 0 )
#endif // WND_USE_GLTXT
, m_iDesktopWidth( 0 )
, m_iDesktopHeight( 0 )
, m_bHasOGLContext( false )
{}

CWnd::~CWnd()
{
#if defined( WND_USE_GLTXT )
	if( m_poFont )
		delete m_poFont;
#endif // WND_USE_GLTXT
}

#if defined( WND_USE_GLTXT ) && defined( WND_USE_GLTXT_BDF )
void CWnd::LoadFontBDF()
{
#if defined( GLTXT_USE_FONT_BDF_INCLUDE )
	CGLTxtFontBDF *poFontBDF = new CGLTxtFontBDF;
	poFontBDF->Setup( g_acGLTxt_BDF_STD_FONT, false );
	m_poFont = poFontBDF;
	WND_LOG( "using compiled bdf font\n" );
#else
	FILE *poFont;
	WND_FOPEN( poFont, WND_GLTXT_STD_FONT_BDF, "r" );
	if( poFont )
	{
		fseek( poFont, 0L, SEEK_END );
		long lEnd = ftell( poFont );
		if( lEnd > 0 )
		{
			char *pcFont = new char[ lEnd ];
			fseek( poFont, 0L, SEEK_SET );
			fread( pcFont, 1, lEnd, poFont );
			WND_LOG( "bytes: %d\n", (unsigned int)oEnd );
			fclose( poFont );

			CGLTxtFontBDF *poFontBDF = new CGLTxtFontBDF;
			poFontBDF->Setup( pcFont, false );
			m_poFont = poFontBDF;
			delete [] pcFont;
		}
	}
	else
	{
		WND_DEBUG( "BDF file\n" );
	}
	WND_LOG( "using bdf font file\n" );
#endif // defined( GLTXT_USE_FONT_BDF_INCLUDE )
}
#endif // defined( WND_USE_GLTXT ) && defined( WND_USE_GLTXT_BDF )

bool CWnd::Init()
{
	WND_MEM_ZERO( m_abKeyDown, sizeof(m_abKeyDown) );
	WND_MEM_ZERO( m_aiMouseState, sizeof(m_aiMouseState) );

	m_bIsMouseInResizeArea	= false;
	m_bIsMouseInMoveArea	= false;
	m_bIsMouseXL		= false;
	m_bIsMouseXR		= false;
	m_bIsMouseYT		= false;
	m_bIsMouseYB		= false;
	m_bIsMoving		= false;
	m_bIsResizing		= false;
	m_iMoveDiffX		= 0;
	m_iMoveDiffY		= 0;
	WND_MEM_ZERO( m_aiResizeStart, sizeof(m_aiResizeStart) );
	
	this->SetCursor( m_uiCursor );
	ShowCursor( false );
	
#ifdef WND_USE_VSYNC
	CVSync::Init_();
	CVSync::Enable_();
#endif // WND_USE_VSYNC
	
	m_bHasOGLContext = true;
	
	if( !m_poHandler->OnInit()  )
		return false;
	m_poHandler->OnSize();
	return true;
}

void CWnd::BorderCheck()
{
	int iCX, iCY;
	GetGlobalCursorPos( &iCX, &iCY );
	if( m_bIsMoving )
	{
		m_iPosX = iCX - m_iMoveDiffX;
		m_iPosY = iCY - m_iMoveDiffY;
		UpdateSizeAndPos();
	}
	else if( m_bIsResizing )
	{
		if( m_bIsMouseXL )
			iCX -= m_aiResizeStart[4];
		else if( m_bIsMouseYT )
			iCY -= m_aiResizeStart[5];

		int iWndPosXTemp = ( m_bIsMouseXL )?( iCX ):( m_aiResizeStart[0] );
		int iWndPosYTemp = ( m_bIsMouseYT )?( iCY ):( m_aiResizeStart[1] );
		int iWndSizeXTemp = m_iWindowWidth;
		int iWndSizeYTemp = m_iWindowHeight;

		if( m_bIsMouseXL )
			iWndSizeXTemp = m_aiResizeStart[0] + m_aiResizeStart[2] - iCX;
		if( m_bIsMouseYT )
			iWndSizeYTemp = m_aiResizeStart[1] + m_aiResizeStart[3] - iCY;
		if( m_bIsMouseXR )
			iWndSizeXTemp = iCX - m_aiResizeStart[0] + m_aiResizeStart[6];
		if( m_bIsMouseYB )
			iWndSizeYTemp = iCY - m_aiResizeStart[1] + m_aiResizeStart[7];

		if( iWndSizeXTemp >= WND_MIN_WIDTH )
		{
			m_iWindowWidth = iWndSizeXTemp;
			m_iPosX = iWndPosXTemp;
		}
		else
		{
			if( m_bIsMouseXL )
				m_iPosX = ( m_iPosX + m_iWindowWidth - WND_MIN_WIDTH );
			m_iWindowWidth = WND_MIN_WIDTH;
		}

		if( iWndSizeYTemp >= WND_MIN_HEIGHT )
		{
			m_iWindowHeight = iWndSizeYTemp;
			m_iPosY = iWndPosYTemp;
		}
		else
		{
			if( m_bIsMouseYT )
				m_iPosY = ( m_iPosY + m_iWindowHeight - WND_MIN_HEIGHT );
			m_iWindowHeight = WND_MIN_HEIGHT;
		}

		// Würde sonst in UpdateSizeAndPos() verschoben, hier wird aber die
		// Grösse angepasst, nicht die Position.
		// mod (0.20): Kein Ahnung was das soll!
		//if( m_iPosX + m_iWindowWidth > m_iScreenWidth )
		//	m_iWindowWidth = m_iScreenWidth - m_iPosX;
		//if( m_iPosY + m_iWindowHeight > m_iScreenHeight )
		//	m_iWindowHeight = m_iScreenHeight - m_iPosY;

		UpdateSizeAndPos();      // Setzt Koordinaten.
		m_poHandler->OnSize();   // Aktualisiert den OpenGL-Kontext.
		m_poHandler->OnUpdate(); // Aktualisiert pixelgrössenabhängige Werte.
		m_poHandler->OnDisplay(); // Zeichnet neu.
	}
	else
	{
		if( !m_bIsCaptured ) // Undefiniert, am äussersten Rand.
		{
			const bool bX = ( m_iMousePosX < 0 );
			const bool bY = ( m_iMousePosY < 0 );
			if( bX || bY )
			{
				// Normaler Windows-Cursor setzten.
				// rem: Wird oft aufgerufen...
				this->SetCursor( WND_CURSOR_DEFAULT );
				ShowCursor( true );
				m_poHandler->OnCursor( false );
				return;
			}
		}

		m_bIsMouseXL = ( m_iMousePosX < WND_BORDER_WIDTH );
		m_bIsMouseXR = ( m_iMousePosX >= m_iWindowWidth - WND_BORDER_WIDTH );
		m_bIsMouseYT = ( m_iMousePosY < WND_BORDER_HEIGHT );
		m_bIsMouseYB = ( m_iMousePosY >= m_iWindowHeight - WND_BORDER_HEIGHT );

		m_bIsMouseInMoveArea = m_bIsMouseYT && !m_bIsMouseXL && !m_bIsMouseXR;
#ifdef WND_USE_RESIZE
		m_bIsMouseInResizeArea = m_bIsMouseXL || m_bIsMouseXR || m_bIsMouseYB;
#endif // WND_USE_RESIZE

		const bool bIsInside = ( m_uiCursor == WND_CURSOR_STD );

		if( m_bIsMouseInMoveArea || m_bIsMouseInResizeArea )
		{
			unsigned int uiCursor = 0;
#ifdef WND_USE_RESIZE
			if( m_bIsMouseXL )
			{
				if( m_bIsMouseYB )
					uiCursor = WND_CURSOR_BOTTOM_LEFT;
				else if( m_bIsMouseYT )
					uiCursor = WND_CUROSR_TOP_LEFT;
				else
					uiCursor = WND_CURSOR_LEFT;
			}
			else if( m_bIsMouseXR )
			{
				if( m_bIsMouseYB )
					uiCursor = WND_CURSOR_BOTTOM_RIGHT;
				else if( m_bIsMouseYT )
					uiCursor = WND_CURSOR_TOP_RIGHT;
				else
					uiCursor = WND_CURSOR_RIGHT;
			}
			else if( m_bIsMouseYB ) {
				uiCursor = WND_CURSOR_BOTTOM;
			}
			else
#endif // WND_USE_RESIZE
			if( m_bIsMouseYT ) {
				uiCursor = WND_CURSOR_MOVE; // top :  move
			}
			if( uiCursor != m_uiCursor )
			{
				this->SetCursor( uiCursor );
			}
			if( bIsInside )
			{
				ShowCursor( true );
				m_poHandler->OnCursor( false );
			}
		}
		else if( !bIsInside )
		{
			this->SetCursor( WND_CURSOR_STD );
			ShowCursor( false );
			m_poHandler->OnCursor( true );
		}
	}
}

#if defined( __WINDOWS__ )

#ifdef WND_USE_FSAA
#include "FSAA.h"
#endif // WND_USE_FSAA

////////////////////////////////////////////////////////////////////////////////
// CWndMS
////////////////////////////////////////////////////////////////////////////////

// <mod date="2010-12-07">
 CWndMS * CWndMS::m_poWnd_( 0 );
// </mod>

CWndMS::CWndMS()
: CWnd()
, m_hWnd( 0 )
, m_bIsLooping( false )
, m_bIsVisibleWnd( false )
, m_bIsVisibleCursor( true )
, m_bIsToggleScreenMode( false )
, m_bEnterMouse( true )
, m_hDC( 0 )
, m_hRC( 0 )
, m_dwStyle( 0 )
, m_dwStyleEx( 0 )
{
	// <mod date="2010-12-07">
	m_poWnd_ = this;
	// </mod>
	
	WNDCLASSEX wc;

	m_iDesktopWidth = m_iScreenWidth = GetSystemMetrics( SM_CXSCREEN );
	m_iDesktopHeight = m_iScreenHeight = GetSystemMetrics( SM_CYSCREEN );

	WND_MEM_ZERO( &wc, sizeof(wc) );
	wc.cbSize		= sizeof(WNDCLASSEX);
	wc.style		= CS_DBLCLKS;
	wc.lpfnWndProc		= reinterpret_cast<WNDPROC>( Proc_ );
	wc.hInstance		= GetModuleHandle( 0 );
	wc.hbrBackground	= (HBRUSH)( COLOR_WINDOW + 1 );
	wc.hCursor		= NULL; //CreateCursor_( m_uiCursor );
	wc.lpszClassName	= TEXT(WND_CLASS_NAME);

	RegisterClassEx( &wc );
}

CWndMS::~CWndMS()
{
	UnregisterClass( TEXT(WND_CLASS_NAME), GetModuleHandle( 0 ) );
}

bool CWndMS::Run()
{
	MSG msg;
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat = 0;
	bool bIsMsgPumpActive;
	DEVMODE dmScreenSettings;
	RECT rect;

	if( !m_poHandler )
		return false;

#ifdef WND_USE_FSAA
LRunStart:
#endif // WND_USE_FSAA

	m_bIsLooping = true;
	while( m_bIsLooping )
	{
		//m_dwStyle	= ( m_bIsFullScreen )?( WS_POPUP ):( WS_OVERLAPPEDWINDOW );
		//m_dwStyle	= ( m_bIsFullScreen )?( WS_POPUP ):( WS_POPUP | WS_BORDER | WS_THICKFRAME ); // Mit "resize-frame"..
		m_dwStyle	= ( m_bIsFullScreen )?( WS_POPUP ):( WS_POPUP | WS_BORDER  );
		m_dwStyleEx	= ( m_bIsFullScreen )?( WS_EX_APPWINDOW | WS_EX_TOPMOST ):( WS_EX_APPWINDOW
#ifdef WND_USE_TOPMOST
			 | WS_EX_TOPMOST
#endif // WND_USE_TOPMOST
			); // | WS_EX_CLIENTEDGE

		if( m_bIsFullScreen )
		{
			rect.left		= 0;
			rect.top		= 0;
			rect.right		= m_iScreenWidth;
			rect.bottom		= m_iScreenHeight;

			WND_MEM_ZERO( &dmScreenSettings, sizeof(dmScreenSettings) );
			dmScreenSettings.dmSize		= sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth	= rect.right;
			dmScreenSettings.dmPelsHeight	= rect.bottom;
			dmScreenSettings.dmBitsPerPel	= m_uiColorBits;
			dmScreenSettings.dmFields	= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
			{
				m_bIsFullScreen = false;
			}
		}

		if( !m_bIsFullScreen )
		{
			const unsigned int uiOffsetMinY = 30;
			if( m_iPosX + m_iWindowWidth > m_iDesktopWidth )
				m_iPosX = m_iDesktopWidth - m_iWindowWidth;
			if( m_iPosX < 0 )
				m_iPosX = 0;
			if( m_iPosY + m_iWindowHeight + int( uiOffsetMinY ) > m_iDesktopHeight )
			{
				m_iPosY = ( m_iDesktopHeight - m_iWindowHeight - uiOffsetMinY ) / 2;
				//m_iPosY = m_iDesktopHeight - m_iWindowHeight - uiOffsetMinY;
			}
			if( m_iPosY < 0 )
				m_iPosY = 0;

			rect.left	= m_iPosX;
			rect.top	= m_iPosY;
			rect.right	= rect.left + m_iWindowWidth;
			rect.bottom	= rect.top + m_iWindowHeight;

			//m_dwStyleEx |= WS_EX_TOOLWINDOW | WS_EX_OVERLAPPEDWINDOW | WS_EX_ACCEPTFILES;
			m_dwStyleEx |= WS_EX_ACCEPTFILES;

			AdjustWindowRectEx( &rect, m_dwStyle, 0, m_dwStyleEx );
			if( rect.left < 0 )
			{
				rect.right -= rect.left;
				rect.left = 0;
			}
			if( rect.top < 0 )
			{
				rect.bottom -= rect.top;
				rect.top = 0;
			}
		}

		m_hWnd = CreateWindowEx(
			m_dwStyleEx,
			TEXT(WND_CLASS_NAME),
			LPCTSTR( m_pcTitle ),
			m_dwStyle,
			rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			HWND_DESKTOP,
			0,
			GetModuleHandle( 0 ),
			this );

		if( !m_hWnd )
			return false;

		m_hDC = GetDC( m_hWnd );

		WND_MEM_ZERO( &pfd, sizeof( pfd ) );
		pfd.nSize	= sizeof( pfd );
		pfd.nVersion	= 1;
		pfd.dwFlags	= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // PFD_STEREO
		pfd.iPixelType	= PFD_TYPE_RGBA;
		pfd.cColorBits	= m_uiColorBits;
		pfd.cDepthBits	= m_uiDepthBits; // 16
#ifdef WND_USE_STENCIL
		pfd.cStencilBits = m_uiStencilBits;
#else
		pfd.cStencilBits = 0;
#endif // WND_USE_STENCIL
		pfd.iLayerType	= PFD_MAIN_PLANE;

#ifdef WND_USE_FSAA
		if( !CFSAA::m_bIsMultisampleSupported_ )
			iFormat = ChoosePixelFormat( m_hDC, &pfd );
		else
			iFormat = CFSAA::m_iMultisampleFormat_;
#else
		iFormat = ChoosePixelFormat( m_hDC, &pfd );
#endif // WND_USE_FSAA

		SetPixelFormat( m_hDC, iFormat, &pfd );

		m_hRC = wglCreateContext( m_hDC );
		wglMakeCurrent( m_hDC, m_hRC );
		
#ifdef WND_USE_FSAA
		if( !CFSAA::m_bIsMultisampleSupported_ && !CFSAA::m_iMultisampleFormat_ )
		{
			if( CFSAA::InitMultisample_( this, m_hDC, WND_FSAA_SAMPLES ) )
			{
				Destroy();
				goto LRunStart;
			}
			else
				CFSAA::m_iMultisampleFormat_ = -1;
		}
#endif // WND_USE_FSAA
		m_bEnterMouse = true; // WM_MOUSEHOVER/WM_MOUSELEAVE

		ShowWindow( m_hWnd, SW_NORMAL );
		UpdateWindow( m_hWnd );
		SetFocus( m_hWnd );
		
		if( !CWnd::Init() )
			return false;

#if defined( WND_USE_GLTXT )
		if( m_poFont )
		{
			delete m_poFont;
			m_poFont = 0;
		}

#if defined( WND_USE_GLTXT )
#if defined( WND_USE_GLTXT_BDF )
		LoadFontBDF();
#else
		CGLTxtFontMS *poFontMS = new CGLTxtFontMS;
		poFontMS->Setup( m_hDC, WND_GLTXT_ATTR );
		m_poFont = poFontMS;
		WND_LOG( "using windows font\n" );
#endif // WND_USE_GLTXT_BDF
#endif // WND_USE_GLTXT

		m_poFont->Create();
#endif // WND_USE_GLTXT

		m_bIsVisibleWnd = true;
		bIsMsgPumpActive = true;

		m_poHandler->OnUpdate();

		while( bIsMsgPumpActive )
		{
			if( PeekMessage( &msg, m_hWnd, 0, 0, PM_REMOVE ) != 0 )
			{
				if( msg.message != WM_QUIT )
				{
					TranslateMessage( &msg );
					DispatchMessage(&msg);
				}
				else
				{
					bIsMsgPumpActive = false;
				}
			}
			else
			{
				if( !m_bIsVisibleWnd )
					WaitMessage();
				else
				{
					if( !m_poHandler->OnDisplay() )
					{
						Terminate();
					}
					if( !m_bIsFullScreen )
						BorderCheck();
					SwapBuffers();
				}
			}
		}
		
		m_poHandler->OnClear();
		
		Destroy();
		
		if( m_bIsToggleScreenMode )
			m_bIsFullScreen = !m_bIsFullScreen;
		
		//if( !m_poHandler->OnClear() )
		//	return false;
	}
	
	return true;
}

void CWndMS::Destroy()
{
#if defined( WND_USE_GLTXT )
	if( m_poFont )
	{
		delete m_poFont;
		m_poFont = 0;
	}
#endif // WND_USE_GLTXT
	
	if( m_hWnd )
	{
		if ( m_hDC )
		{
			wglMakeCurrent( m_hDC, 0 );
			if( m_hRC )
			{
				wglDeleteContext( m_hRC );
				m_hRC = 0;
			}
			ReleaseDC( m_hWnd, m_hDC );
			m_hDC = 0;
		}
		DestroyWindow( m_hWnd );
		m_hWnd = 0;
	}

	if( m_bIsFullScreen )
	{
		ChangeDisplaySettings( 0, 0 );
		ShowCursor( true );
	}

	if( m_bHasOGLContext )
	{
		m_bHasOGLContext = false;
	}
}

bool CWndMS::Terminate()
{
	if( !(m_hWnd && m_bIsLooping) )
		return false;
	m_bIsLooping = false;
	PostMessage( m_hWnd, WM_QUIT, 0, 0 );
	return true;
}

LRESULT CALLBACK CWndMS::Proc_( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#define UPDATE_MOUSE_MOD() \
	poWnd->m_abKeyDown[WND_KEY_CONTROL] = (( wParam & MK_CONTROL ) != 0 ); \
	poWnd->m_abKeyDown[WND_KEY_SHIFT] = (( wParam & MK_SHIFT ) != 0 );

	// <mod date="2010-12-16">
	//LONG lUserData = GetWindowLong( hWnd, GWL_USERDATA );
	// </mod>

	// <mod date="2010-12-07">
	CWndMS *poWnd( m_poWnd_ );
	//CWndMS *poWnd;
	// </mod>
	
	// <mod date="2010-12-07">
	//if( !lUserData )
	//{
	//	if( uMsg == WM_CREATE )
	//	{
	//		CREATESTRUCT *pCreateStruct = reinterpret_cast<CREATESTRUCT*>( lParam );
	//		CWndMS *pWnd = reinterpret_cast<CWndMS*>( pCreateStruct->lpCreateParams );
	//		SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG_PTR)( pWnd ) );
	//	}
	//}
	//else
	// </mod>
	{
		// <mod date="2010-12-07">
		CHandler *poHandler = poWnd->m_poHandler;
		//poWnd = reinterpret_cast<CWndMS*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
		//CHandler *poHandler = poWnd->m_poHandler;
		// </mod>
		
		switch( uMsg )
		{
			case WM_MOUSEMOVE:
				if( poWnd->m_bEnterMouse )
				{
					TRACKMOUSEEVENT oTrackEvent = { sizeof(TRACKMOUSEEVENT), TME_LEAVE | TME_HOVER, poWnd->m_hWnd, 1 };
					TrackMouseEvent( &oTrackEvent ); // receive next WM_MOUSELEAVE
					poWnd->m_bEnterMouse = false;
				}
				poWnd->m_iMousePosX = (signed short)(LOWORD(lParam));
				poWnd->m_iMousePosY = (signed short)(HIWORD(lParam));
				UPDATE_MOUSE_MOD();
			return 0;

			case WM_MOUSELEAVE:
				poWnd->m_bEnterMouse = true;
				if( !poWnd->m_bIsCaptured )
				{
					poWnd->m_iMousePosX = -1;
					poWnd->m_iMousePosY = -1;
				}
			return 0;
			case WM_MOUSEHOVER:
				poWnd->m_iMousePosX = (signed short)(LOWORD(lParam));
				poWnd->m_iMousePosY = (signed short)(HIWORD(lParam));
			break;

			case WM_LBUTTONDOWN:
				if( poWnd->m_bIsMouseInMoveArea )
				{
					POINT oPnt;
					::GetCursorPos( &oPnt );
					poWnd->m_iMoveDiffX = oPnt.x - poWnd->m_iPosX;
					poWnd->m_iMoveDiffY = oPnt.y - poWnd->m_iPosY;
					poWnd->m_bIsMoving = true;
					poWnd->SetMouseCapture();
				}
				else if( poWnd->m_bIsMouseInResizeArea )
				{
					POINT oPnt;
					::GetCursorPos( &oPnt );
					poWnd->m_aiResizeStart[0] = poWnd->m_iPosX;
					poWnd->m_aiResizeStart[1] = poWnd->m_iPosY;
					poWnd->m_aiResizeStart[2] = poWnd->m_iWindowWidth;
					poWnd->m_aiResizeStart[3] = poWnd->m_iWindowHeight;
					// Pedantisch: Damit das Fenster nicht sprunghaft seine Grösse ändert.
					poWnd->m_aiResizeStart[4] = oPnt.x - poWnd->m_iPosX;
					poWnd->m_aiResizeStart[5] = oPnt.y - poWnd->m_iPosY;
					poWnd->m_aiResizeStart[6] = poWnd->m_iPosX + poWnd->m_iWindowWidth - oPnt.x;
					poWnd->m_aiResizeStart[7] = poWnd->m_iPosY + poWnd->m_iWindowHeight - oPnt.y;
					poWnd->m_bIsResizing = true;
					poWnd->SetMouseCapture();
				}
				else
				{
					UPDATE_MOUSE_MOD();
					poWnd->m_aiMouseState[WND_MOUSE_BUTTON_LEFT] = WND_MOUSE_STATE_DOWN;
					poHandler->OnMouse( WND_MOUSE_BUTTON_LEFT, WND_MOUSE_STATE_DOWN );
				}
			return 0;
			case WM_LBUTTONDBLCLK:
				UPDATE_MOUSE_MOD();
				poWnd->m_aiMouseState[WND_MOUSE_BUTTON_LEFT] = WND_MOUSE_STATE_DBLCLK;
				poHandler->OnMouse( WND_MOUSE_BUTTON_LEFT, WND_MOUSE_STATE_DBLCLK );
			return 0;
			case WM_LBUTTONUP:
 				if( poWnd->m_bIsMoving || poWnd->m_bIsResizing )
				{
					poWnd->ReleaseMouseCapture();
					poWnd->m_bIsResizing = poWnd->m_bIsMoving = false;
				}
				else
				{
					UPDATE_MOUSE_MOD();
					poWnd->m_aiMouseState[WND_MOUSE_BUTTON_LEFT] = WND_MOUSE_STATE_UP;
					poHandler->OnMouse( WND_MOUSE_BUTTON_LEFT, WND_MOUSE_STATE_UP );
				}
			return 0;
			case WM_MBUTTONDOWN:
				UPDATE_MOUSE_MOD();
				poWnd->m_aiMouseState[WND_MOUSE_BUTTON_MIDDLE] = WND_MOUSE_STATE_DOWN;
				poHandler->OnMouse( WND_MOUSE_BUTTON_MIDDLE, WND_MOUSE_STATE_DOWN );
			return 0;
			case WM_MBUTTONDBLCLK:
				UPDATE_MOUSE_MOD();
				poWnd->m_aiMouseState[WND_MOUSE_BUTTON_MIDDLE] = WND_MOUSE_STATE_DBLCLK;
				poHandler->OnMouse( WND_MOUSE_BUTTON_MIDDLE, WND_MOUSE_STATE_DBLCLK );
			break;
			case WM_MBUTTONUP:
				UPDATE_MOUSE_MOD();
				poWnd->m_aiMouseState[WND_MOUSE_BUTTON_MIDDLE] = WND_MOUSE_STATE_UP;
				poHandler->OnMouse( WND_MOUSE_BUTTON_MIDDLE, WND_MOUSE_STATE_UP );
			return 0;
			case WM_RBUTTONDOWN:
				UPDATE_MOUSE_MOD();
				poWnd->m_aiMouseState[WND_MOUSE_BUTTON_RIGHT] = WND_MOUSE_STATE_DOWN;
				poHandler->OnMouse( WND_MOUSE_BUTTON_RIGHT, WND_MOUSE_STATE_DOWN );
			return 0;
			case WM_RBUTTONDBLCLK:
				UPDATE_MOUSE_MOD();
				poWnd->m_aiMouseState[WND_MOUSE_BUTTON_RIGHT] = WND_MOUSE_STATE_DBLCLK;
				poHandler->OnMouse( WND_MOUSE_BUTTON_RIGHT, WND_MOUSE_STATE_DBLCLK );
			break;
			case WM_RBUTTONUP:
				UPDATE_MOUSE_MOD();
				poWnd->m_aiMouseState[WND_MOUSE_BUTTON_RIGHT] = WND_MOUSE_STATE_UP;
				poHandler->OnMouse( WND_MOUSE_BUTTON_RIGHT, WND_MOUSE_STATE_UP );
			return 0;
			case WM_MOUSEWHEEL:
			{
				UPDATE_MOUSE_MOD();
				int iW = GET_WHEEL_DELTA_WPARAM(wParam) / 120;
				poWnd->m_aiMouseState[WND_MOUSE_BUTTON_WHEEL] = iW;
				poHandler->OnMouse( WND_MOUSE_BUTTON_WHEEL, iW );
			}
			return 0;


			// wParam -> VK / lParam -> add info:
			// |_key_|already|alt key_|_not used_|extend_|_scan code_|_repeat count_|
			// |_up__|_down_ |_down___|(reserved)|_key___|___________|______________|
			// |__31_|__30__ |__29____|_25-28____|__24___|__16-23____|___0-15_______|
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				//if( lParam & 29 )
				//	poWnd->m_abKeyDown[ WND_KEY_MENU ] = true;
				unsigned int uiKey;
				if( ConvKey_( wParam, &uiKey ) )
				{
					poHandler->OnKeyDown( uiKey );
					poWnd->m_abKeyDown[ uiKey ] = true;
				}
			}
			return 0;

			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				//if( lParam & 29 == 0 )
				//	poWnd->m_abKeyDown[ WND_KEY_MENU ] = false;
				unsigned int uiKey;
				if( ConvKey_( wParam, &uiKey ) )
				{
					poWnd->m_abKeyDown[ uiKey ] = false;
					poHandler->OnKeyUp( uiKey );
				}
			}
			return 0;

			case WM_CHAR: // rem: Von TranslateMessage(...).
				poWnd->m_poHandler->OnChar( static_cast<unsigned int>(wParam) );
			break;

			//case WM_LBUTTONDBLCLK:
			//break;
			//case WM_NCLBUTTONDOWN:
			//break;

			case WM_SETCURSOR:
			{
				::SetCursor( CWndMS::CreateCursor_( poWnd->m_uiCursor ) );
			}
			return true;
			case WM_MOVE:
				if( !poWnd->m_bIsFullScreen )
				{
					poWnd->m_iPosX = LOWORD(lParam);
					poWnd->m_iPosY = HIWORD(lParam);
				}
			return 0;

			case WM_SIZE:
				switch (wParam)
				{
					case SIZE_MINIMIZED:
						poWnd->m_bIsVisibleWnd = false;
						return 0;
					break;

					case SIZE_MAXIMIZED:
					case SIZE_RESTORED:
						poWnd->m_bIsVisibleWnd = true;
						if( !poWnd->m_bIsFullScreen )
						{
							poWnd->m_iWindowWidth = LOWORD(lParam);
							poWnd->m_iWindowHeight = HIWORD(lParam);
						}
						if( poWnd->m_bHasOGLContext )
						{
							poHandler->OnSize();
						}
						return 0;
					break;
				}

			case WM_SYSCOMMAND:
				switch (wParam)
				{
					case SC_SCREENSAVE:
					case SC_MONITORPOWER:
						return 0;
					break;
				}
			break;

			case WM_CLOSE:
				poWnd->m_bIsLooping = false;
				PostMessage( poWnd->m_hWnd, WM_QUIT, 0, 0 );
			return 0;
#ifdef WND_USE_DND
			case WM_DROPFILES:
			{
				UINT	nCnt, nIndex, nSize;
				LPTSTR  pszFileName;
				HDROP   hDrop;

				hDrop = (HDROP)wParam;
				nCnt  = DragQueryFile( hDrop, (UINT)-1, NULL, 0 ); // Anzahl der Dateien ermitteln

				for(nIndex = 0; nIndex < nCnt; ++nIndex)
				{
					// Länge des Dateinamens ermitteln
					if( !( nSize = DragQueryFile( hDrop, nIndex, NULL, 0 ) ) )
						continue;

					pszFileName = new TCHAR[++nSize];

					if( DragQueryFile( hDrop, nIndex, pszFileName, nSize ) ) // Dateinamen ermitteln
					{
						// pszFileName refernziert nun den Dateinamen
						poWnd->m_oArrDrop.Append( CStr( pszFileName ) );
					}

					delete [] pszFileName;
				}
				// Das Handle freigeben
				DragFinish( hDrop );

				poWnd->m_poHandler->OnDrop();
				return 0;
			}
#endif // WND_USE_DND
			default:
				return DefWindowProc( hWnd, uMsg, wParam, lParam);
		}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

bool CWndMS::ConvKey_( WPARAM wParam, unsigned int *puiKey )
{
	for( unsigned int i=0; i<m_uiMapKeyCount_; ++i )
	{
		if( m_atMapKey_[i].m_tY == wParam )
		{
			*puiKey = m_atMapKey_[i].m_tX;
			return true;
		}
	}
	*puiKey = 0;
	return false;
}

HCURSOR CWndMS::CreateCursor_( unsigned int uiCursor )
{
	for( unsigned int i=0; i<m_uiMapCursorCount_; ++i )
	{
		if( m_atMapKey_[i].m_tX == uiCursor )
		{
			return LoadCursor( 0, m_atMapCursor_[i].m_tY );
		}
	}
	return 0;
}



#elif defined( __LINUX__ )

/*

// SDL_x11events.c
// X11 modifier masks for various keys
static unsigned meta_l_mask, meta_r_mask, alt_l_mask, alt_r_mask;
static unsigned num_mask, mode_switch_mask;

static void GetModifierKeyMasks_( Display *display )
{
	static unsigned got_masks;
	XModifierKeymap *xmods;
	unsigned n;

	if(got_masks)
		return;

	xmods = XGetModifierMapping(display);
	n = xmods->max_keypermod;

	for(unsigned int i = 3; i < 8; i++) {
		for(unsigned int j = 0; j < n; j++) {
			KeyCode kc = xmods->modifiermap[i * n + j];
			KeySym ks = XKeycodeToKeysym(display, kc, 0);
			unsigned mask = 1 << i;
			switch(ks) {
			case XK_Num_Lock:
				num_mask = mask; break;
			case XK_Alt_L:
				alt_l_mask = mask; break;
			case XK_Alt_R:
				alt_r_mask = mask; break;
			case XK_Meta_L:
				meta_l_mask = mask; break;
			case XK_Meta_R:
				meta_r_mask = mask; break;
			case XK_Mode_switch:
				mode_switch_mask = mask; break;
			}
		}
	}
	XFreeModifiermap(xmods);
	got_masks = 1;
}
*/

// http://www.koders.com/cpp/fid394E30D73CBE3ABC025667C83925A8350C866443.aspx
// XCheckMaskEvent() doesn't get ClientMessage Events so use XCheckIfEvent()
// with this Predicate function as a work-around (ClientMessage events
// are needed in order to catch "WM_DELETE_WINDOW")
static Bool AlwaysTruePredicate_( Display*, XEvent*, char* ) {
	return True;
}

CWndX11::CWndX11()
: CWnd()
, m_bIsLooping( false )
, m_bIsVisible( false )
, m_poDisplay( 0 )
, m_uiBorder( 0 )
, m_iScreen( 0 )
, m_uliEventMask( 0 )
, m_bIsDoubleBuffered( false )
, m_oCursorNormal( 0 )
, m_uliDoubleClickDelay( WND_X11_STD_DOUBLE_CLICK_DELAY )
{
	WND_MEM_ZERO( m_abKeyDown, sizeof( m_abKeyDown ) * sizeof( m_abKeyDown[0] ) );

	m_poDisplay = XOpenDisplay( 0 );
	m_iScreen = DefaultScreen( m_poDisplay );

#if defined( WND_USE_GLTXT )
#if defined( WND_USE_GLTXT_BDF )
	LoadFontBDF();
#else
	CGLTxtFontX11 *poFontX11 = new CGLTxtFontX11;
	poFontX11->Setup( m_poDisplay, WND_GLTXT_STD_FONT_X11 );
	m_poFont = poFontX11;
	WND_LOG( "using x window font\n" );
#endif // WND_USE_GLTXT_BDF
#endif // WND_USE_GLTXT

	// Aktuelle Screen-Grösse ermitteln, und als
	// Default-Auflösung für den Fullscreen-Modus setzen.
	// Wird nur einmal abgefragt...
	XWindowAttributes oWndAttr;
	XGetWindowAttributes( m_poDisplay,  DefaultRootWindow( m_poDisplay ), &oWndAttr );
	m_iDesktopWidth = m_iScreenWidth = oWndAttr.width;
	m_iDesktopHeight= m_iScreenHeight = oWndAttr.height;
	WND_LOG( "screen size: %d, %d\n", m_iScreenWidth, m_iScreenHeight );
}

CWndX11::~CWndX11()
{
#if defined( WND_USE_GLTXT )
	if( m_poFont )
		delete m_poFont;
#endif // WND_USE_GLTXT
	m_poFont = 0;
	Clear();
	XCloseDisplay( m_poDisplay );
}

bool CWndX11::Init()
{
	// attributes for a single buffered visual in RGBA format with at least
	// 4 bits per color and a 16 bit depth buffer
	static int aiAttrListSgl[] = {
		GLX_RGBA,
		GLX_RED_SIZE, 4,
		GLX_GREEN_SIZE, 4,
		GLX_BLUE_SIZE, 4,
		GLX_DEPTH_SIZE, 16,
#ifdef WND_USE_FSAA
		GLX_SAMPLE_BUFFERS_ARB, 1,
		GLX_SAMPLES_ARB, WND_FSAA_SAMPLES,
#endif // WND_USE_FSAA
		None };

	// attributes for a double buffered visual in RGBA format with at least
	// 4 bits per color and a 16 bit depth buffer
	static int aiAttrListDbl[] = {
		GLX_RGBA, GLX_DOUBLEBUFFER,
		GLX_RED_SIZE, 4,
		GLX_GREEN_SIZE, 4,
		GLX_BLUE_SIZE, 4,
		GLX_DEPTH_SIZE, WND_DEPTH_BUFFER_BITS, // UNTESTED
#ifdef WND_USE_FSAA
		GLX_SAMPLE_BUFFERS_ARB, 1,
		GLX_SAMPLES_ARB, WND_FSAA_SAMPLES,
#endif // WND_USE_FSAA
		None };

	int iVersionMajor, iVersionMinor;
	XVisualInfo *poVisInfo = 0;
	Colormap oColorMap;
	XSetWindowAttributes oWndAttr;

	m_iScreen = DefaultScreen( m_poDisplay );

	XF86VidModeQueryVersion( m_poDisplay, &iVersionMajor, &iVersionMinor );
	WND_LOG( "XF86VidModeExtension version: %d, %d\n", iVersionMajor, iVersionMinor );

	poVisInfo = glXChooseVisual( m_poDisplay, m_iScreen, aiAttrListDbl );
	m_bIsDoubleBuffered = true;
	if( !poVisInfo )
	{
		poVisInfo = glXChooseVisual( m_poDisplay, m_iScreen, aiAttrListSgl );
		m_bIsDoubleBuffered = false;
	}

	// http://oss.sgi.com/projects/ogl-sample/registry/ARB/multisample.txt
	//int iBuffers, iSamples;
	//glXGetConfig( m_poDisplay, poVisInfo, GLX_SAMPLE_BUFFERS_ARB, &iBuffers );
	//glXGetConfig( m_poDisplay, poVisInfo, GLX_SAMPLES_ARB, &iSamples );
	//WND_LOG( "buffers: %d\n", iBuffers );
	//WND_LOG( "samples: %d\n", iSamples );

	m_oWindowRoot = RootWindow( m_poDisplay, poVisInfo->screen );

	glXQueryVersion( m_poDisplay, &iVersionMajor, &iVersionMinor );
	WND_LOG( "GLX version: %d, %d\n", iVersionMajor, iVersionMinor );

	m_oContextGLX = glXCreateContext( m_poDisplay, poVisInfo, 0, True );

	oColorMap = XCreateColormap(
		m_poDisplay, m_oWindowRoot,
		poVisInfo->visual, AllocNone );
	oWndAttr.colormap = oColorMap;
	oWndAttr.border_pixel = 0;

	//  | PointerMotionHintMask??
	oWndAttr.event_mask = m_uliEventMask =
		KeyPressMask | KeyReleaseMask | KeymapStateMask |
		PointerMotionMask |
		ButtonMotionMask | ButtonPressMask | ButtonReleaseMask |
		EnterWindowMask | LeaveWindowMask |
		ExposureMask | FocusChangeMask | StructureNotifyMask;

	if( m_bIsFullScreen )
	{
	    // HACK: Bypass window manager, just assume that the physical screen resolution
	    // for the fullscreen window will work.
	    // XF86 stuff seems to be not supported consistently...
	    /*
        int iBestVideoMode = -1;
        int iVideoModeCount = 0;
        XF86VidModeModeInfo **ppoVideoMode = 0;
        
		XF86VidModeGetAllModeLines(
			m_poDisplay, m_iScreen,
			&iVideoModeCount, &ppoVideoMode );

		m_oVideoModeInfoPrev = *ppoVideoMode[0];

	LDetectVideoMode:
		WND_LOG( "video modes: \n" );
		for( int i=0; i<iVideoModeCount; ++i )
		{
			WND_LOG( "%dx%d\n", ppoVideoMode[i]->hdisplay, ppoVideoMode[i]->vdisplay );
			if( ppoVideoMode[i]->hdisplay == m_iScreenWidth
			&&  ppoVideoMode[i]->vdisplay == m_iScreenHeight )
				iBestVideoMode = i;
		}
		WND_LOG( "\n" );

		if( iBestVideoMode < 0 )
		{
			WND_LOG( "%dx%d not supported!\n", m_iScreenWidth, m_iScreenHeight );
			exit( EXIT_FAILURE );

			m_iWindowWidth = DisplayWidth( m_poDisplay, m_iScreen );
			m_iWindowHeight = DisplayHeight( m_poDisplay, m_iScreen );
			goto LDetectVideoMode;
		}

		XF86VidModeModeInfo *poVidMode = ppoVideoMode[iBestVideoMode];
		XF86VidModeSwitchToMode( m_poDisplay, m_iScreen, poVidMode );
		XF86VidModeSetViewPort( m_poDisplay, m_iScreen, 0, 0 );
		m_iScreenWidth = poVidMode->hdisplay;
		m_iScreenHeight = poVidMode->vdisplay;
		XFree( ppoVideoMode );
        */
        
		oWndAttr.override_redirect = True;

		m_oWindow = XCreateWindow( m_poDisplay, m_oWindowRoot,
			0, 0, m_iScreenWidth, m_iScreenHeight, 0, poVisInfo->depth, InputOutput, poVisInfo->visual,
			CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
			&oWndAttr );

		//XWarpPointer(
		//	m_poDisplay, None, m_oWindow, 0, 0, 0, 0,
		//	m_iScreenWidth / 2, m_iScreenHeight / 2 );

		// mod (0.20)
		XMapRaised( m_poDisplay, m_oWindow );
		XSync( m_poDisplay, False );

		while( GrabSuccess != XGrabPointer( m_poDisplay, m_oWindow, True, ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
			GrabModeAsync, GrabModeAsync, m_oWindow, None, CurrentTime) )
		{
			usleep( 100 );
		}

		SetFocus();
		XGrabKeyboard( m_poDisplay, m_oWindow, True, GrabModeAsync,
				GrabModeAsync, CurrentTime );

		m_uiColorBits = poVisInfo->depth;
		m_uiBorder = 0;
	}
	else // if( !m_bIsFullScreen )
	{
		m_uiBorder = 0;
		//m_uiBorder = 1;
		//oWndAttr.background_pixel = BlackPixel( m_poDisplay, DefaultScreen( m_poDisplay ) );
		//oWndAttr.border_pixel = BlackPixel( m_poDisplay, DefaultScreen( m_poDisplay ) );

		m_oWindow = XCreateWindow( m_poDisplay, m_oWindowRoot,
			m_iPosX, m_iPosY, m_iWindowWidth, m_iWindowHeight,
			m_uiBorder, poVisInfo->depth, InputOutput, poVisInfo->visual,
			CWColormap | CWEventMask, // | CWBackPixel | CWBorderPixel,
			&oWndAttr );

		m_oAtomWMDelete = XInternAtom( m_poDisplay, "WM_DELETE_WINDOW", True );
		// <mod0>
		m_oAtomDndProtocol = XInternAtom( m_poDisplay, "DndProtocol", False );
		m_oAtomDndSelection = XInternAtom( m_poDisplay, "DndSelection", False );
		// </mod0>
		XSetWMProtocols( m_poDisplay, m_oWindow, &m_oAtomWMDelete, 1 );
		XSetStandardProperties( m_poDisplay, m_oWindow, m_pcTitle,
			m_pcTitle, None, 0, 0, 0 );
		XMapRaised( m_poDisplay, m_oWindow );

		XStoreName( m_poDisplay, m_oWindow, m_pcTitle );

		XSizeHints *size_hints;
		XWMHints *wm_hints;
		XClassHint *class_hints;

		if( !(size_hints = XAllocSizeHints())) {
			WND_LOG( "failure allocating memory" );
			return false;
		}
		if (!(wm_hints = XAllocWMHints())) {
			WND_LOG( "failure allocating memory" );
			return false;;
		}
		if (!(class_hints = XAllocClassHint())) {
			WND_LOG( "failure allocating memory" );
			return false;;
		}

		size_hints->flags = PPosition | PSize | PMinSize;
		size_hints->min_width = WND_MIN_WIDTH;
		size_hints->min_height = WND_MIN_HEIGHT;

		wm_hints->initial_state = NormalState; //IconicState;
		wm_hints->input = True;
		//wm_hints->icon_pixmap = icon_pixmap;
		wm_hints->flags = StateHint | InputHint; // | IconPixmapHint

		class_hints->res_name = (char *)m_pcTitle;
		class_hints->res_class = (char *)"GL_WND_CLASS";

		XTextProperty oTextPropTitle;
		//m_uliEventMask
		char * apcTitle[] = { (char *)m_pcTitle };
		XStringListToTextProperty( apcTitle, 1, &oTextPropTitle );
		char *argv[] = { (char*)m_pcTitle };
		int argc = 1;
		XSetWMProperties(
			m_poDisplay, m_oWindow, &oTextPropTitle, &oTextPropTitle,
			argv, argc,
			//m_ppcArg, m_iArg,
			size_hints, wm_hints,
			class_hints );

		XFree( size_hints );
		XFree( wm_hints );
		XFree( class_hints );

		// Remove Window Decorations
		// [SDL] Re: SDL and window decorations
		// http://lists.libsdl.org/pipermail/sdl-libsdl.org/1999-April/001954.html
		Atom WM_HINTS;

		// MWM Hints.
		WM_HINTS = XInternAtom(m_poDisplay, "_MOTIF_WM_HINTS", True);
		if ( WM_HINTS != None ) {
#			define  MWM_HINTS_DECORATIONS   (1L << 1)
			struct {
			    int flags;
			    int functions;
			    int decorations;
			    int inputMode;
			    int unknown;
			} MWMHints = { MWM_HINTS_DECORATIONS, 0, 0, 0, 0 };

			XChangeProperty(m_poDisplay, m_oWindow, WM_HINTS, WM_HINTS, 32,
					PropModeReplace, (unsigned char *)&MWMHints,
					sizeof(MWMHints)/4);
		}

		// KWM Hints.
		WM_HINTS = XInternAtom( m_poDisplay, "KWM_WIN_DECORATION", True );
		if ( WM_HINTS != None )
		{
			long KWMHints = 0;

			XChangeProperty(m_poDisplay, m_oWindow, WM_HINTS, WM_HINTS, 32,
					PropModeReplace, (unsigned char *)&KWMHints,
					sizeof(KWMHints)/4);
		}

#if 0  /* The Butcher way of removing window decorations -- not polite */
		attributes.override_redirect = True;
		XChangeWindowAttributes( m_poDisplay, m_oWindow,
					 CWOverrideRedirect, &attributes );
#endif // 0

		/*
		// KeepAbove    = 1<<6,	///< @since 3.2
		// StaysOnTop   = KeepAbove,	// NOT STANDARD
		// http://www.libsdl.org/release/SDL-1.2.0/src/video/x11/SDL_x11video.c
		// KWM_KEEP_ON_TOP
		{
			XEvent ev;
			long mask;

			WND_MEM_ZERO(&ev, sizeof(ev));
			ev.xclient.type = ClientMessage;
			ev.xclient.window = m_oWindowRoot;
			ev.xclient.message_type = XInternAtom( m_poDisplay,
							      "KWM_KEEP_ON_TOP", False);
			ev.xclient.format = 32;
			ev.xclient.data.l[0] = m_oWindow;
			ev.xclient.data.l[1] = CurrentTime;
			mask = SubstructureRedirectMask;
			XSendEvent( m_poDisplay, m_oWindowRoot, False, mask, &ev);
		}
		*/

		m_uiColorBits = poVisInfo->depth;

		// mod (0.20)
		//UpdatePosition();
		UpdateSizeAndPos();
	}

	glXMakeCurrent( m_poDisplay, m_oWindow, m_oContextGLX );
	//XGetGeometry( m_poDisplay, m_oWindow, &oWindowDummy, &m_iPosX, &m_iPosY,
	//	&m_uiDimX, &m_uiDimY, &m_uiBorder, &uiDepth );

	if( glXIsDirect( m_poDisplay, m_oContextGLX ) )
	{
		WND_LOG( "Direct Rendering\n" );
	}
	else
	{
		WND_LOG( "No Direct Rendering\n" );
	}

	SetCursor( WND_CURSOR_DEFAULT );

	// http://www.linuxforums.org/forum/linux-programming-scripting/59012-xlib-hide-mouse-pointer.html
	XColor oColorDummy;
	static const char acNoData_[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	m_oColormapCursorHidden = DefaultColormap( m_poDisplay, m_iScreen );
	XAllocNamedColor( m_poDisplay, m_oColormapCursorHidden, "black", &m_oColorBlack, &oColorDummy );
	m_oPixmapCursorHidden = XCreateBitmapFromData( m_poDisplay, m_oWindow, acNoData_, 8, 8 );
	m_oCursorHidden =
		XCreatePixmapCursor( m_poDisplay, m_oPixmapCursorHidden, m_oPixmapCursorHidden,
					&m_oColorBlack, &m_oColorBlack, 0, 0 );
	ShowCursor( false );

	m_poHandler->OnInit();
	m_poHandler->OnSize();

#ifdef WND_USE_GLTXT
	if( !m_poFont->Create() )
	{
		WND_ERROR( "Font\n" );
		exit( -1 );
	}
#endif // WND_USE_GLTXT

	m_poHandler->OnUpdate();

	XClearWindow( m_poDisplay, m_oWindow );
	XMapRaised( m_poDisplay, m_oWindow );
	XFlush( m_poDisplay );

#ifdef WND_USE_DND
	m_oDND.initialize( m_poDisplay, m_oWindow );
#endif // WND_USE_DND

	int gx, gy;
	Window oWDummy;
	GetGlobalCursorPos( &gx, &gy );
	XTranslateCoordinates(
		m_poDisplay, m_oWindowRoot, m_oWindow,
		gx, gy, &m_iMousePosX, &m_iMousePosY, &oWDummy );

	return true;
}

void CWndX11::Clear()
{
	if( m_oCursorHidden != m_oCursorHidden )
	{
		XFreeCursor( m_poDisplay, m_oCursorHidden );
		XFreePixmap( m_poDisplay, m_oPixmapCursorHidden );
		XFreeColors( m_poDisplay, m_oColormapCursorHidden, &m_oColorBlack.pixel, 1, 0 );
	}
	if( m_oCursorNormal != None )
	{
		XFreeCursor( m_poDisplay, m_oCursorNormal );
		m_oCursorNormal = None;
	}
	if( m_oContextGLX )
	{
		if( !glXMakeCurrent( m_poDisplay, None, 0 ) )
		{
			WND_LOG( "Could not release drawing context." );
		}
		glXDestroyContext( m_poDisplay, m_oContextGLX );
		m_oContextGLX = 0;
	}
	// HACK: Bypass windows manger. XF86 seems to be not supported consistently...
	// See also: CWndX11::Init().
	//if( m_bIsFullScreen )
	//{
	//	XF86VidModeSwitchToMode( m_poDisplay, m_iScreen, &m_oVideoModeInfoPrev );
	//	XF86VidModeSetViewPort( m_poDisplay, m_iScreen, 0, 0 );
	//}
	XDestroyWindow( m_poDisplay, m_oWindow );
	//m_poHandler->OnClear();
}

bool CWndX11::Run()
{
	//KeyCode oKC_Enter = XKeysymToKeycode( m_poDisplay, XK_Return );
	//KeyCode oKC_Escape = XKeysymToKeycode( m_poDisplay, XK_Escape );
	//KeyCode oKC_F1 = XKeysymToKeycode( m_poDisplay, XK_F1 );

	typedef SPair<unsigned int, unsigned int> TPairMouse;
	//static const unsigned int auiButtonMask_[] =
	//	{ Button1Mask, Button2Mask, Button3Mask, Button4Mask, Button5Mask };
	static const TPairMouse atMapMouseButton_[] = {
		TPairMouse( Button1, WND_MOUSE_BUTTON_LEFT ),
		TPairMouse( Button2, WND_MOUSE_BUTTON_MIDDLE ),
		TPairMouse( Button3, WND_MOUSE_BUTTON_RIGHT ),
		//TPairMouse( Button4, WND_MOUSE_NONE ),
		//TPairMouse( Button5, WND_MOUSE_NONE ),
	};
	const unsigned int m_uiMapMouseButtonCount_ =
		sizeof( atMapMouseButton_ ) / sizeof( atMapMouseButton_[0] );

	m_bIsLooping = true;

	struct SClick {
		SClick(): m_uliTime(0), m_iX(0), m_iY(0) {}
		unsigned long int m_uliTime;
		int m_iX, m_iY;
	} aoLastClick[5];
	Window oWDummy;

	Init();

	do
	{
		//do
		//{
		XEvent e;
		WND_MEM_ZERO( &e, sizeof( XEvent ) );
		//XNextEvent( m_poDisplay, &e );

		//while( XCheckMaskEvent( m_poDisplay, m_uliEventMask, &e ) )
		//{
		while( XCheckIfEvent( m_poDisplay, &e, AlwaysTruePredicate_, 0 ) )
		{
#ifdef WND_USE_DND
			if( e.xselection.property == m_oDND.XdndActionPrivate )
			{
				CStr oFileNames( m_oDND.getDroppedFilenames( e ) );
				if( oFileNames.GetSize() )
				{
					oFileNames.Replace( "%20", " " );
					oFileNames.Replace( "file://", "" );

					while( oFileNames.GetSize() )
					{
						const int i = oFileNames.Find( 0, "\n" );
						if( i < 0 )
							break;
						int j = i;
						if( oFileNames[j-1] == '\r' ) // check for "\r\n"
							--j;
						const CStr oSub = oFileNames.GetSub( 0, j );
						WND_LOG( "drop %s\n", oSub.GetData() );
						m_oArrDrop.Append( oSub );
						oFileNames.Del( 0, i + 1 );
					}
					m_poHandler->OnDrop();
				}
			}
#endif // WND_USE_DND
			switch( e.type )
			{
			case Expose:
				WND_LOG( "Expose\n" );
				//XDrawString( m_poDisplay, m_oWindow, m_oGC, 40, 40, "test", 4 );
			break;
			case UnmapNotify: // icon
				WND_LOG( "UnmapNotify\n" );
			break;
			case MapNotify: // restore
				WND_LOG( "MapNotify\n" );
			break;
			case FocusIn:
				WND_LOG( "FocusIn\n" );
			break;
			case FocusOut:
				WND_LOG( "FocusOut\n" );
			break;
			case EnterNotify:
				WND_LOG( "EnterNotify\n" );
				//SetFocus();
				// mod (0.20)
				if( !m_bIsMouseInMoveArea && !m_bIsMouseInResizeArea )
					m_poHandler->OnCursor( true );
			break;
			case LeaveNotify:
				WND_LOG( "LeaveNotify\n" );
				m_poHandler->OnCursor( false ); // mod (0.20)
			break;
			case KeymapNotify:
				WND_LOG( "KeymapNotify\n" );
				//GetModifierKeyMasks_( m_poDisplay );
			break;
			case KeyPress:
			{
				WND_LOG( "KeyPress\n" );
                
                
                // fix 1 for XKeycodeToKeysym (requires X Keyboard Extension -> #include <X11/XKBlib.h>).
				//const int iIndex = ( e.xkey.state & ShiftMask )?( 1 ):( 0 );
                //SetKey( XkbKeycodeToKeysym( m_poDisplay, e.xkey.keycode, 0, iIndex ), true );
                //
                // fix 2 for XKeycodeToKeysym
				SetKey( GetKey( e.xkey.keycode ), true );
                //
                //
                // deprecated
				//const int iIndex = ( e.xkey.state & ShiftMask )?( 1 ):( #include <X11/XKBlib.h>)0 );
				//SetKey( XKeycodeToKeysym( m_poDisplay, e.xkey.keycode, iIndex ), true );
                
                
				//if( XK_Return == XLookupKeysym( &e.xkey, 0 ) )
				//	WND_LOG( "yeah\n" );

				//if( e.xkey.keycode == oKC_Escape )
				//	m_bIsLooping = false;
				//if( e.xkey.keycode == oKC_F1 )
				//	ToggleScreenMode();

				static char acBuffer_[16];
				KeySym keySym;
				const int iLen = XLookupString( &e.xkey, acBuffer_, 15, &keySym, NULL);
				if( iLen )
				{
					acBuffer_[iLen] = '\0';
					for( int i=0; i<iLen; ++i )
						m_poHandler->OnChar( acBuffer_[i] );
				}
			}
			break;
			case KeyRelease:
			{
				// siehe KeyPress.
				
				// fix 1 for XKeycodeToKeysym (requires X Keyboard Extension)
				//const int iIndex = ( e.xkey.state & ShiftMask )?( 1 ):( 0 );
                //SetKey( XkbKeycodeToKeysym( m_poDisplay, e.xkey.keycode, 0, iIndex ), false );
                // fix 2 for XKeycodeToKeysym
				SetKey( GetKey( e.xkey.keycode ), false );
				//
				// deprecated
				//const int iIndex = ( e.xkey.state & ShiftMask )?( 1 ):( 0 );
				//SetKey( XKeycodeToKeysym( m_poDisplay, e.xkey.keycode, iIndex ), false );

				WND_LOG( "KeyRelease\n" );
			}
			break;
			case ButtonPress:
			{
				//bool bSingleClick = false;
				int iButton = -1;
				unsigned int uiState = 0;

				WND_LOG( "ButtonPress " );

				// todo: ButtonMask Ctrl/Shift
				for( unsigned int i=0; i<m_uiMapMouseButtonCount_; ++i )
					if( e.xbutton.button == atMapMouseButton_[i].m_tX )
					{
						WND_LOG( "Button%d (", i + 1 );
						iButton = atMapMouseButton_[i].m_tY;

						SClick &roClick = aoLastClick[i];
						if( e.xbutton.time - roClick.m_uliTime <= m_uliDoubleClickDelay
						 && e.xbutton.x == roClick.m_iX
						 && e.xbutton.y == roClick.m_iY )
						{
							WND_LOG( "double click" );
							uiState = WND_MOUSE_STATE_DBLCLK;
							//m_uiMouseDoubleClick |= uiMask;
						}
						else
						{
							WND_LOG( "click" );
							uiState = WND_MOUSE_STATE_DOWN;
							//m_uiMouseDown |= uiMask;
							//bSingleClick = true;
						}
						WND_LOG( ")\n" );

						//if( e.xbutton.state & ControlMask )
						//	m_aiMouseState[WND_MOUSE_BUTTON_CTRL] = WND_MOUSE_STATE_DOWN;
						//if( e.xbutton.state & ShiftMask )
						//	m_aiMouseState[WND_MOUSE_BUTTON_SHIFT] = WND_MOUSE_STATE_DOWN;

						roClick.m_uliTime = e.xbutton.time;
						roClick.m_iX = e.xbutton.x;
						roClick.m_iY = e.xbutton.y;
						break;
					}
				//m_abMouseDown[ e.xbutton.button - 1 ] = true;

				if( iButton == WND_MOUSE_BUTTON_LEFT && uiState == WND_MOUSE_STATE_DOWN
					&& ( m_bIsMouseInMoveArea || m_bIsMouseInResizeArea ) )
				{
					int gx, gy;
					XTranslateCoordinates(
						m_poDisplay, m_oWindow, m_oWindowRoot,
						e.xbutton.x, e.xbutton.y, &gx, &gy, &oWDummy );

					//GetGlobalCursorPos( &gx, &gy );

					if( m_bIsMouseInMoveArea )
					{
						m_iMoveDiffX = gx - m_iPosX;
						m_iMoveDiffY = gy - m_iPosY;
						m_bIsMoving = true;
						//SetMouseCapture();
					}
					else if( m_bIsMouseInResizeArea )
					{
						m_aiResizeStart[0] = m_iPosX;
						m_aiResizeStart[1] = m_iPosY;
						m_aiResizeStart[2] = m_iWindowWidth;
						m_aiResizeStart[3] = m_iWindowHeight;
						// Pedantisch: Damit das Fenster nicht sprunghaft seine Grösse ändert.
						m_aiResizeStart[4] = gx - m_iPosX;
						m_aiResizeStart[5] = gy - m_iPosY;
						m_aiResizeStart[6] = m_iPosX + m_iWindowWidth - gx;
						m_aiResizeStart[7] = m_iPosY + m_iWindowHeight - gy;
						m_bIsResizing = true;
						//SetMouseCapture();
					}
				}
				else if( iButton >= 0 )
				{
					//UPDATE_MOUSE_MOD();
					m_aiMouseState[iButton] = uiState;
					m_poHandler->OnMouse( (unsigned int)iButton, uiState );
				}

				/*
				//for( unsigned int i=0; i<m_uiMapMouseButtonCount_; ++i )
				//	if( e.xbutton.state & auiButtonMask_[i] )
				//		WND_LOG( "Button%dMask\n", i + 1 );
				*/
			}
			break;
			case MotionNotify:
			{
				/*
				if( poWnd->m_bEnterMouse )
				{
					TRACKMOUSEEVENT oTrackEvent = { sizeof(TRACKMOUSEEVENT), TME_LEAVE | TME_HOVER, poWnd->m_hWnd, 1 };
					TrackMouseEvent( &oTrackEvent ); // receive next WM_MOUSELEAVE
					poWnd->m_bEnterMouse = false;
				}
				*/
				m_iMousePosX = e.xmotion.x;
				m_iMousePosY = e.xmotion.y;

				WND_LOG( "MotionNotify %d, %d\n", m_iMousePosX, m_iMousePosY );
			}
			break;
			case ButtonRelease:
			{
				// siehe ButtonPress.

				WND_LOG( "ButtonRelease " );

				int iButton = -1;
				for( unsigned int i=0; i<m_uiMapMouseButtonCount_; ++i )
					if( e.xbutton.button == atMapMouseButton_[i].m_tX )
					{
						WND_LOG( "Button%d ", i + 1 );
						iButton = atMapMouseButton_[i].m_tY;
						m_aiMouseState[iButton] = WND_MOUSE_STATE_UP;
						//const unsigned int uiMask = ~ atMapMouseButton_[i].m_tY;
						//m_uiMouseDown &= uiMask;
						//m_uiMouseDoubleClick &= uiMask;
						break;
					}

				//for( unsigned int i=0; i<6; ++i )
				//	if( e.xbutton.state & auiButtonMask_[i] )
				//		WND_LOG( "Button%dMask", i + 1 );

				if( iButton >= 0 )
				{
					if( iButton == WND_MOUSE_BUTTON_LEFT && ( m_bIsMoving || m_bIsResizing ) )
					{
						//if( m_bIsMouseInMoveArea )
						//	ReleaseMouseCapture();
						m_bIsResizing = m_bIsMoving = false;
					}
					else
					{
						m_aiMouseState[iButton] = WND_MOUSE_STATE_UP;
						m_poHandler->OnMouse( iButton, WND_MOUSE_STATE_UP );
					}
				}

				WND_LOG( "\n" );
			}
			break;
			case ConfigureNotify: // resized or moved?
				WND_LOG( "ConfigureNotify\n" );
				/*
				if( !m_bIsFullScreen )
				{
					//m_iPosX = e.xconfigure.x;
					//m_iPosY = e.xconfigure.y;
					if( m_bIsResizing
					|| e.xconfigure.width != m_iWindowWidth
					|| e.xconfigure.height != m_iWindowHeight )
					{
						m_iWindowWidth = e.xconfigure.width;
						m_iWindowHeight = e.xconfigure.height;
						m_poHandler->OnUpdate();
						m_poHandler->OnSize();
					}
				}
				*/
			break;
			case ClientMessage:
			{
				WND_LOG( "ClientMessage\n" );
				Atom oAtomReq = (Atom)e.xclient.data.l[0];
				if( oAtomReq == m_oAtomWMDelete )
					m_bIsLooping = false;

				//else if( oAtomReq == m_oAtomDndProtocol )
				//{
				//	WND_DEBUG( "dnd\n" );
				//	//m_oAtomDndProtocolm_oAtomDndSelection
				//}

#ifdef WND_USE_DND
				if( e.xclient.message_type == m_oDND.XdndEnter )
				{
					WND_LOG( "XdndEnter\n" );
					m_oDND.handleEnterEvent( e );
				}
				else if( e.xclient.message_type == m_oDND.XdndPosition )
				{
					WND_LOG( "XDndPostion\n" );
					m_oDND.handlePositionEvent( e );
				}
				else if( e.xclient.message_type == m_oDND.XdndDrop )
				{
					WND_LOG( "XdndDrop\n" );
					m_oDND.handleDropEvent();
				}
				else if( e.xclient.message_type == m_oDND.XdndLeave )
				{
					WND_LOG( "XdndLeave\n" );
					m_oDND.handleLeaveEvent( e );
				}
#endif // WND_USE_DND
			}
			break;
			};
		}
		//while( XPending( m_poDisplay ) );

		if( !m_poHandler->OnDisplay() )
			return false;

		if( !m_bIsFullScreen )
			BorderCheck();

		if( m_bIsDoubleBuffered )
			SwapBuffers();
	}
	while( m_bIsLooping );

	return true;
}

bool CWndX11::Terminate()
{
	if( !m_bIsLooping )
		return false;
	m_bIsLooping = false;
	return true;
}

void CWndX11::ToggleScreenMode()
{
	if( !m_bIsFullScreen )
	{
		Window oWndRoot;
		int iPosXRel; //, iPosYRel;
		unsigned int uiWidth, uiHeight;
		XGetGeometry(
			m_poDisplay, m_oWindow, &oWndRoot,
			&iPosXRel, &iPosXRel,
			&uiWidth, &uiHeight,
			&m_uiBorder, &m_uiColorBits );
		//iPosYRel = 0;
		iPosXRel = 0; // ...compiler warning
		m_iWindowWidth = uiWidth;
		m_iWindowHeight = uiHeight;

		//XTranslateCoordinates( m_poDisplay, m_oWindow, oWndRoot,
		//			0, 0, &m_iPosX, &m_iPosY, &oWndChild );
		UpdatePosition();
	}
	else
	{
		//XUngrabPointer( m_poDisplay, CurrentTime );
	}
	Clear();
	m_bIsFullScreen = !m_bIsFullScreen;
	Init();
}

void CWndX11::SetCursor( unsigned int uiCursor )
{
	if( uiCursor != m_uiCursor )
	{
		unsigned int uiShape;

		m_uiCursor = uiCursor;

		if( m_oCursorNormal )
			XFreeCursor( m_poDisplay, m_oCursorNormal );

		unsigned int i;
		for( i=0; i<m_uiMapCursorCount_; ++i )
		{
			if( m_atMapCursor_[i].m_tX == uiCursor )
			{
				uiShape = m_atMapCursor_[i].m_tY;
				break;
			}
		}
		if( i == m_uiMapCursorCount_ )
			uiShape = m_atMapCursor_[WND_CURSOR_DEFAULT].m_tY;

		m_oCursorNormal = XCreateFontCursor( m_poDisplay, uiShape );
	}
	UseCursor( m_oCursorNormal );
}

void CWndX11::SetKey( KeySym oKey, bool bState )
{
	for( unsigned int i=0; i<m_uiMapKeyCount_; ++i )
	{
		if( m_atMapKey_[i].m_tY == oKey )
		{
			const unsigned int uiKey = m_atMapKey_[i].m_tX;
			if( bState )
				m_poHandler->OnKeyDown( uiKey );
			else
				m_poHandler->OnKeyUp( uiKey );
			m_abKeyDown[ uiKey ] = bState;
		}
	}

	/*
	// ACHTUNG: temp:
	if( oKey >= XK_A && oKey <= XK_Z ) {
		const unsigned char ucAsciiKey = oKey - XK_A + 'A';
		m_poHandler->OnChar( ucAsciiKey );
		WND_LOG( "key: %c\n", ucAsciiKey  );
	}
	if( oKey >= XK_a && oKey <= XK_z ) {
		const unsigned char ucAsciiKey = oKey - XK_a + 'a';
		m_poHandler->OnChar( ucAsciiKey );
		WND_LOG( "key: %c\n", ucAsciiKey  );
	}
	*/

	/*
	switch (key_symbol)
	{
		case XK_1:
		case XK_KP_1: // '1' key was pressed, either the normal '1', or
			  // the '1' on the keypad. draw the current pixel.
		//XDrawPoint(display, the_win, gc_draw, x, y);
		WND_LOG( "1\n" );
		break;
		case XK_Delete: // DEL key was pressed, erase the current pixel.
		//XDrawPoint(display, the_win, gc_erase, x, y);
		WND_LOG( "del\n" );
		break;
	}
	*/
}

void CWndX11::UpdatePosition()
{
	Window oWDummy;
	XTranslateCoordinates(
		m_poDisplay, m_oWindow, m_oWindowRoot,
		0, 0, &m_iPosX, &m_iPosY, &oWDummy );

	WND_LOG( "update position: %d, %d\n", m_iPosX, m_iPosY );
}

// new
KeySym CWndX11::GetKey( unsigned int uiKeyCode ) const
{
    KeySym oRetKeySym;
    int iKeySyms;
    KeySym * poKeySym = XGetKeyboardMapping(
        m_poDisplay,
        uiKeyCode,
        1,
        &iKeySyms );
    oRetKeySym = poKeySym[0];
    XFree( poKeySym );
    return oRetKeySym;
}

#endif // platform
