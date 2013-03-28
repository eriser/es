// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// WndDEF.h
//
// todo:
// - Stencil-Test unter Linux.
// - Color-/Depth-/Stencil-Depth-Detector.
// - VSync unter Linux.

#ifndef _WND_DEF_H_
#define _WND_DEF_H_

#define WND_VERSION			"0.35" // 2011-01-14

#define WND_USE_GLTXT
//#define WND_USE_GLTXT_BDF
//#define WND_USE_FSAA
//#define WND_USE_VSYNC
#define WND_USE_DND
//#define WND_USE_STENCIL
#define WND_USE_RESIZE
//#define WND_USE_TOPMOST

//#define WND_USE_LOG
//#define WND_USE_DEBUG
#define WND_USE_ERROR

#define WND_MIN_WIDTH		   	256
#define WND_MIN_HEIGHT		  	256
#define WND_BORDER_WIDTH		16
#define WND_BORDER_HEIGHT		WND_BORDER_WIDTH
#define WND_FSAA_SAMPLES		4 //2
#define WND_COLOR_BUFFER_BITS		32
#define WND_DEPTH_BUFFER_BITS     	24

#define WND_GLTXT_STD_FONT_BDF		"data/" "5x7.bdf"
//	"5x7.bdf"
//	"4x6.bdf"
//	"micro.bdf"
//	"courB18.bdf"
//	"hl1-11.bdf"
//	"hl2-11.bdf"
//	"phreak-13.bdf"
// 	"future-11.bdf"
//	"amiga-8.bdf"
//	"kung_fu-11.bdf"
//	"timR10.bdf"
//	"UTB___10.bdf"
//	"UTBI___18.bdf"
//	"timB24.bdf"


#if defined( __WINDOWS__ )

#if !defined( __GNUC__ ) && !defined( _WIN32_WINNT )
//#define WINVER			0x0500 // TRACKMOUSEEVENT
#define _WIN32_WINNT			0x0500 // TRACKMOUSEEVENT
#endif // ...
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#define WND_MEM_ZERO(mem, size)		ZeroMemory(mem, size)

#define WND_GLTXT_ATTR			"Arial", 10 // Bitmap Font
#define WND_CLASS_NAME			"wnd_class"


#elif defined( __LINUX__ )

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <X11/extensions/xf86vmode.h>
#include <string.h>
#include <unistd.h>

#define WND_MEM_ZERO(mem,size)		memset( mem, 0, size )
#define WND_GLTXT_STD_FONT_X11		"-*-arial-medium-r-normal-*-10-*-*-*-*-*-*-*"
// "-*-helvetica-medium-r-normal-*-10-*-75-75-*-*-*-*" // (5x7, 6x12, 7x14, 8x16, 12x24)
// "-adobe-courier-medium-r-normal-*-*-*-75-75-*-*-*-*
// "-*-clean-medium-r-normal-*-*-*-75-75-*-*-*-*"
#define WND_X11_STD_DOUBLE_CLICK_DELAY	210 // ms
#define WND_INTERACTION_DELAY		7

#endif // platform



#include "../DEF.h"

#if defined( WND_USE_LOG )
#define WND_LOG(...)			fprintf( stderr, "WND_LOG: " __VA_ARGS__ )
#else
#define WND_LOG(...)			{ ; }
#endif // WND_USE_LOG
#if defined( WND_USE_DEBUG )
#include <stdio.h>
#define WND_DEBUG(...)			fprintf( stderr, "WND_DEBUG: " __VA_ARGS__ )
#else
#define WND_DEBUG(...)			{ ; }
#endif // WND_USE_DEBUG
#if defined( WND_USE_ERROR )
#include <stdio.h>
#define WND_ERROR(...)			fprintf( stderr, "WND_ERROR: " __VA_ARGS__ )
#else
#define WND_ERROR(...)			{ ; }
#endif // WND_USE_ERROR

#ifdef WND_USE_FSAA
#define USE_FSAA
#endif // USE_FSAA

#ifdef WND_USE_DND
#include "../Str/Str.h"
#include "../Array/Array.h"
#ifdef __LINUX__
#include "xdnd.h"
#endif // __LINUX__
#endif // WND_USE_DND

#ifdef WND_USE_GLTXT
#include "../GLTxt/GLTxtFont.h"
#endif // WND_USE_GLTEXT

#if defined( WND_USE_GLTXT ) && defined( WND_USE_GLTXT_BDF ) && !defined( WND_USE_GLTXT_BDF_INCLUDE )
#include <stdio.h>
#if defined( __WINDOWS__ ) && defined( _MSC_VER )
#define WND_FOPEN( file, name, mode )	::fopen_s( &file, name, mode )
#else
#define WND_FOPEN( file, name, mode )	file = ::fopen( name, mode )
#endif
#endif



enum {
	WND_CURSOR_DEFAULT,
	WND_CURSOR_CROSS,
	WND_CURSOR_ARROW_H,
	WND_CURSOR_ARROW_V,
	WND_CURSOR_LEFT,
	WND_CURSOR_RIGHT,
	WND_CURSOR_BOTTOM,
	WND_CURSOR_TOP,
	WND_CURSOR_BOTTOM_LEFT,
	WND_CURSOR_BOTTOM_RIGHT,
	WND_CUROSR_TOP_LEFT,
	WND_CURSOR_TOP_RIGHT,
	WND_CURSOR_SIZEALL,
	_WND_CURSOR_MAX
};

enum {
	WND_MOUSE_BUTTON_LEFT,
	WND_MOUSE_BUTTON_MIDDLE,
	WND_MOUSE_BUTTON_RIGHT,
	//WND_MOUSE_BUTTON_CTRL,
	//WND_MOUSE_BUTTON_SHIFT,
	WND_MOUSE_BUTTON_WHEEL,
	_WND_MOUSE_BUTTON_MAX
};

enum {
	WND_MOUSE_STATE_UP,
	WND_MOUSE_STATE_DOWN,
	WND_MOUSE_STATE_DBLCLK
};


// cat tmp0 | sed 's/\(WND_KEY_[A-Z0-9_]*\)[\t ]*0x[0-9A-Z]\{2\}/\1,/g'
enum {
	WND_KEY_BACKSPACE,
	WND_KEY_TAB,
	//WND_KEY_LINEFEED,
	WND_KEY_CLEAR,
	WND_KEY_RETURN,
	WND_KEY_PAUSE,
	WND_KEY_SCROLL_LOCK,
	//WND_KEY_SYS_REQ,
	WND_KEY_ESCAPE,
	WND_KEY_DELETE,

	WND_KEY_HOME,
	WND_KEY_LEFT,
	WND_KEY_UP,
	WND_KEY_RIGHT,
	WND_KEY_DOWN,
	//WND_KEY_PRIOR,
	WND_KEY_PAGE_UP,
	//WND_KEY_NEXT,
	WND_KEY_PAGE_DOWN,
	WND_KEY_END,
	//WND_KEY_BEGIN,


	WND_KEY_SELECT,
	WND_KEY_PRINT,
	WND_KEY_EXECUTE,
	WND_KEY_INSERT,
	//WND_KEY_UNDO,
	//WND_KEY_REDO,
	//WND_KEY_MENU,
	//WND_KEY_FIND,
	WND_KEY_CANCEL,
	WND_KEY_HELP,
	//WND_KEY_BREAK,
	//WND_KEY_MODE_SWITCH,
	//WND_KEY_SCRIPT_SWITCH,
	WND_KEY_NUM_LOCK,

	//WND_KEY_KP_SPACE,
	//WND_KEY_KP_TAB,
	//WND_KEY_KP_ENTER,
	//WND_KEY_KP_F1,
	//WND_KEY_KP_F2,
	//WND_KEY_KP_F3,
	//WND_KEY_KP_F4,
	//WND_KEY_KP_HOME,
	//WND_KEY_KP_LEFT,
	//WND_KEY_KP_UP,
	//WND_KEY_KP_RIGHT,
	//WND_KEY_KP_DOWN,
	//WND_KEY_KP_PRIOR,
	//WND_KEY_KP_PAGE_UP,
	//WND_KEY_KP_NEXT,
	//WND_KEY_KP_PAGE_DOWN,
	//WND_KEY_KP_END,
	//WND_KEY_KP_BEGIN,
	//WND_KEY_KP_INSERT,
	//WND_KEY_KP_DELETE,
	//WND_KEY_KP_EQUAL,
	//WND_KEY_KP_MULTIPLY,
	//WND_KEY_KP_ADD,
	//WND_KEY_KP_SEPARATOR,
	//WND_KEY_KP_SUBSTRACT,
	//WND_KEY_KP_DECIMAL,
	//WND_KEY_KP_DIVIDE,

	WND_KEY_KP_0,
	WND_KEY_KP_1,
	WND_KEY_KP_2,
	WND_KEY_KP_3,
	WND_KEY_KP_4,
	WND_KEY_KP_5,
	WND_KEY_KP_6,
	WND_KEY_KP_7,
	WND_KEY_KP_8,
	WND_KEY_KP_9,

	WND_KEY_F1,
	WND_KEY_F2,
	WND_KEY_F3,
	WND_KEY_F4,
	WND_KEY_F5,
	WND_KEY_F6,
	WND_KEY_F7,
	WND_KEY_F8,
	WND_KEY_F9,
	WND_KEY_F10,
	WND_KEY_F11,
	//WND_KEY_L1,
	WND_KEY_F12,
	//WND_KEY_L2,
	WND_KEY_F13,
	//WND_KEY_L3,
	WND_KEY_F14,
	//WND_KEY_L4,
	WND_KEY_F15,
	//WND_KEY_L5,
	WND_KEY_F16,
	//WND_KEY_L6,
	WND_KEY_F17,
	//WND_KEY_L7,
	WND_KEY_F18,
	//WND_KEY_L8,
	WND_KEY_F19,
	//WND_KEY_L9,
	WND_KEY_F20,
	//WND_KEY_L10,
	WND_KEY_F21,
	//WND_KEY_R1,
	WND_KEY_F22,
	//WND_KEY_R2,
	WND_KEY_F23,
	//WND_KEY_R3,
	WND_KEY_F24,
	//WND_KEY_R4,
	//WND_KEY_F25,
	//WND_KEY_R5,
	//WND_KEY_F26,
	//WND_KEY_R6,
	//WND_KEY_F27,
	//WND_KEY_R7,
	//WND_KEY_F28,
	//WND_KEY_R8,
	//WND_KEY_F29,
	//WND_KEY_R9,
	//WND_KEY_F30,
	//WND_KEY_R10,
	//WND_KEY_F31,
	//WND_KEY_R11,
	//WND_KEY_F32,
	//WND_KEY_R12,
	//WND_KEY_F33,
	//WND_KEY_R13,
	//WND_KEY_F34,
	//WND_KEY_R14,
	//WND_KEY_F35,
	//WND_KEY_R15,

	WND_KEY_SHIFT,
	//WND_KEY_SHIFT_L,
	//WND_KEY_SHIFT_R,
	WND_KEY_CONTROL,
	//WND_KEY_CONTROL_L,
	//WND_KEY_CONTROL_R,
	WND_KEY_CAPS_LOCK,
	//WND_KEY_SHIFT_LOCK,

	//WND_KEY_META_L,
	//WND_KEY_META_R,
	WND_KEY_ALT,
	//WND_KEY_ALT_L,
	//WND_KEY_ALT_R,
	//WND_KEY_SUPER_L,
	//WND_KEY_SUPER_R,
	//WND_KEY_HYPER_L,
	//WND_KEY_HYPER_R,

	WND_KEY_SPACE, 	 	// SPACE
	//WND_KEY_EXCLAM,		// EXCLAMATION MARK
	//WND_KEY_QUOTEDBL,	// QUOTATION MARK
	//WND_KEY_NUMBERSIGN,	// NUMBER SIGN
	//WND_KEY_DOLLAR,		// DOLLAR SIGN
	//WND_KEY_PERCENT,	// PERCENT SIGN
	//WND_KEY_AMPERSAND,	// AMPERSAND SIGN
	//WND_KEY_APOSTROPHE,	// APOSTROPHE
	//WND_KEY_QUOTERIGHT,	// deprecated
	//WND_KEY_PARENLEFT,	// LEFT PARENTHESIS
	//WND_KEY_PARENRIGHT,	// RIGHT PARENTHESIS
	//WND_KEY_ASTERISK,	// ASTERISK
	//WND_KEY_PLUS,		// PLUS SIGN
	//WND_KEY_COMMA,		// COMMA
	//WND_KEY_MINUS,		// MINUS
	//WND_KEY_PERIOD,		// FULL STOP
	WND_KEY_SLASH,		// SOLIDUS
	WND_KEY_0,
	WND_KEY_1,
	WND_KEY_2,
	WND_KEY_3,
	WND_KEY_4,
	WND_KEY_5,
	WND_KEY_6,
	WND_KEY_7,
	WND_KEY_8,
	WND_KEY_9,
	//WND_KEY_COLON,		// COLON
	//WND_KEY_SEMICOLON,	// SEMICOLON
	//WND_KEY_LESS,		// LESS-THAN SIGN
	//WND_KEY_EQUAL,		// EQUALS SIGN
	//WND_KEY_GREATER,	// GREATER SIGN
	//WND_KEY_QUESTION,	// QUESTION MARK
	//WND_KEY_AT,		// COMMERCIAL AT

	/*
	WND_KEY_CAP_A,
	WND_KEY_CAP_B,
	WND_KEY_CAP_C,
	WND_KEY_CAP_D,
	WND_KEY_CAP_E,
	WND_KEY_CAP_F,
	WND_KEY_CAP_G,
	WND_KEY_CAP_H,
	WND_KEY_CAP_I,
	WND_KEY_CAP_J,
	WND_KEY_CAP_K,
	WND_KEY_CAP_L,
	WND_KEY_CAP_M,
	WND_KEY_CAP_N,
	WND_KEY_CAP_O,
	WND_KEY_CAP_P,
	WND_KEY_CAP_Q,
	WND_KEY_CAP_R,
	WND_KEY_CAP_S,
	WND_KEY_CAP_T,
	WND_KEY_CAP_U,
	WND_KEY_CAP_V,
	WND_KEY_CAP_W,
	WND_KEY_CAP_X,
	WND_KEY_CAP_Y,
	WND_KEY_CAP_Z,
	*/

	//WND_KEY_BRACKETLEFT,
	//WND_KEY_BACKSLASH,
	//WND_KEY_BRACKETRIGHT,
	//WND_KEY_ASCIICIRCUM,
	//WND_KEY_UNDERSCORE,
	//WND_KEY_GRAVE,
	//WND_KEY_QUOTELEFT,

	WND_KEY_A,
	WND_KEY_B,
	WND_KEY_C,
	WND_KEY_D,
	WND_KEY_E,
	WND_KEY_F,
	WND_KEY_G,
	WND_KEY_H,
	WND_KEY_I,
	WND_KEY_J,
	WND_KEY_K,
	WND_KEY_L,
	WND_KEY_M,
	WND_KEY_N,
	WND_KEY_O,
	WND_KEY_P,
	WND_KEY_Q,
	WND_KEY_R,
	WND_KEY_S,
	WND_KEY_T,
	WND_KEY_U,
	WND_KEY_V,
	WND_KEY_W,
	WND_KEY_X,
	WND_KEY_Y,
	WND_KEY_Z,

	//WND_KEY_BRACELEFT,	// LEFT CURLY BRACKET
	//WND_KEY_BAR,		// VERTICAL LINE
	//WND_KEY_BRACERIGHT,	// RIGHT CURLY BRACKET
	//WND_KEY_ASCIITILDE	// TILDE

	_WND_KEY_MAX
};

#define WND_CURSOR_STD		WND_CURSOR_CROSS
#define WND_CURSOR_MOVE		WND_CURSOR_SIZEALL

#endif // _WND_DEF_H_
