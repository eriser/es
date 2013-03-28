// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// VisOpDEF.h

#ifndef _VIS_OP_DEF_H_
#define _VIS_OP_DEF_H_

#ifdef _DEBUG
#define VIS_OP_USE_CONSOLE
#endif // _DEBUG

#include "../DEF.h"
#include "../Op/Op.h"
#include "../OrgOp/OrgOp.h"
#include "../List/List.h"
#include "../Stack/Stack.h"
#include "../GLTxt/GLTxtPrinter.h"
#include "../GLTool/GLTool.h"
#include "../Time/Time.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <ctype.h>

#define VIS_OP_VERSION				"0.01"

#if defined( APP_LANGUAGE_GERMAN )
#define VIS_OP_LANGUAGE_GERMAN
#else
#define VIS_OP_LANGUAGE_ENGLISH
#endif

#define VIS_OP_LOG(...)				LOG(__VA_ARGS__)

#define VIS_OP_MEM_ZERO( dst, count )		memset( dst, 0, count )
#define VIS_OP_MEM_COPY( dst, src, count )	memmove( dst, src, count )
#define VIS_OP_MEM_SET( dst, val, count )	memset( dst, val, count )

#define VIS_OP_SSCANF(	buf, fmt, ... )		SSCANF( buf, fmt, __VA_ARGS__ )
#define VIS_OP_SPRINTF( buf, size, fmt, ... ) 	SPRINTF( buf, size, fmt, __VA_ARGS__ )

#define VIS_OP_FILE_NAME_STD			"patch.txt"
#define VIS_OP_FILE_NEW_TEMPLATE_STD0		0
#define VIS_OP_FILE_NEW_TEMPLATE_STD1		1
#define VIS_OP_FILE_NEW_TEMPLATE_STD2		2
#define VIS_OP_FILE_NEW_TEMPLATE_STD3		3

#define VIS_OP_DELIM				'#'
#define VIS_OP_ITEM_DELIM			'>'
#define VIS_OP_NEWLINE				'\n'
#define VISOP_STD_TRANSPARENCY0			0xCF

#define VIS_OP_TEXT_MODE_NONE			0
#define VIS_OP_TEXT_MODE_FILE_NAME_SAVE		1
#define VIS_OP_TEXT_MODE_FILE_NAME_LOAD		2
#define VIS_OP_TEXT_MODE_ITEM			4
#define VIS_OP_TEXT_MODE_INFO			8

#define VIS_OP_PICK_MODE_MOVE			0
#define VIS_OP_PICK_MODE_ACTION0		1
#define VIS_OP_PICK_MODE_ACTION1		2
#define VIS_OP_PICK_MODE_TWIDDLE		3
#define VIS_OP_PICK_MODE_CONTEXT0		4
#define VIS_OP_PICK_MODE_DEFPOS			5

#define VIS_OP_ITEM_TYPE_STD			1
#define VIS_OP_ITEM_TYPE_ROOT			2
#define VIS_OP_ITEM_TYPE_SUB			4
#define VIS_OP_ITEM_TYPE_SUP			8

#define VIS_OP_ITEM_STATE_EDIT			0
//#define VIS_OP_ITEM_STATE_COLL		1
#define VIS_OP_ITEM_STATE_MAX			1

#define VIS_OP_ITEM_RENDER_MODE_VISIBLE		0
#define VIS_OP_ITEM_RENDER_MODE_HIDDEN		1

#define VIS_OP_ITEM_STD_X			0.14f
//#define VIS_OP_ITEM_STD_Y			0.15f
#define VIS_OP_ITEM_ELEM_X			0.04f
#define VIS_OP_ITEM_ELEM_Y			0.04f
#define VIS_OP_ITEM_MARGIN_X			( VIS_OP_ITEM_ELEM_X / 3.0f )
#define VIS_OP_ITEM_MARGIN_Y			( VIS_OP_ITEM_ELEM_Y / 4.0f )

#define VIS_OP_ITEM_TEXT_INSTANCE_NAME		0
#define VIS_OP_ITEM_TEXT_PORT_IN		1
#define VIS_OP_ITEM_TEXT_PORT_IN_MAX		( VIS_OP_ITEM_TEXT_PORT_IN + 500 ) // Noch gültig.
#define VIS_OP_ITEM_TEXT_PORT_OUT		VIS_OP_ITEM_TEXT_PORT_IN_MAX
#define	VIS_OP_ITEM_TEXT_PORT_OUT_MAX		( VIS_OP_ITEM_TEXT_PORT_OUT + 500 ) // Noch gültig.

#define VIS_OP_DIALOG_EXT_X			( VIS_OP_ITEM_ELEM_X * 10.0f )
#define VIS_OP_DIALOG_EXT_Y			( VIS_OP_ITEM_ELEM_Y * 3.0f )

#if defined( VIS_OP_LANGUAGE_ENGLISH )
#define VIS_OP_DIALOG_STR_YES			"yes"
#define VIS_OP_DIALOG_STR_NO			"no"
#define VIS_OP_DIALOG_STR_OK			"ok"
#elif defined( VIS_OP_LANGUAGE_GERMAN )
#define VIS_OP_DIALOG_STR_YES			"ja"
#define VIS_OP_DIALOG_STR_NO			"nein"
#define VIS_OP_DIALOG_STR_OK			"ok"
#endif // VIS_OP_LANGUAGE_*

#define VIS_OP_CONTEXT0_EXT_X			( VIS_OP_ITEM_ELEM_X * 7.0f )
#define VIS_OP_CONTEXT0_EXT_Y			VIS_OP_ITEM_ELEM_X
#define VIS_OP_CONTEXT0_MAX_Y			16

#define VIS_OP_LINE_WIDTH_STD			1.0f
#define VIS_OP_LINE_WIDTH_SELECT		2.0f

#define VIS_OP_OSCILLOSCOP_HEIGHT		( VIS_OP_ITEM_ELEM_Y * 5.0f )

#define VIS_OP_STIPPLE				0xAAAA

#define VIS_OP_BLINK(state)			( ((state) & 3 ) > 1 )

#define VIS_OP_ITEM_SELECT_PORT_NONE		0
#define VIS_OP_ITEM_SELECT_PORT_INPUT		1
#define VIS_OP_ITEM_SELECT_PORT_OUTPUT		2
#define VIS_OP_ITEM_SELECT_PORT_ROUTINE		3

#define VIS_OP_TWIDDLE_SMALL			0.001f
#define VIS_OP_TWIDDLE_MEDIUM			10.0f
#define VIS_OP_TWIDDLE_LARGE			1000.0f

#if defined( __LINUX__ )
#define VIS_OP_USE_FRAME
#endif // __LINUX__

#endif // _VIS_OP_DEF_H_
