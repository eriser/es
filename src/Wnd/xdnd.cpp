// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// xdnd.cpp

#include "xdnd.h"
#include <string.h>

#define dnd_version_at_least(a,b) ((a) <= (b))

#define XDND_VERSION 3

/* XdndEnter */
#define XDND_THREE 3
#define XDND_ENTER_SOURCE_WIN(e)		((e)->xclient.data.l[0])
#define XDND_ENTER_THREE_TYPES(e)		(((e)->xclient.data.l[1] & 0x1UL) == 0)
#define XDND_ENTER_THREE_TYPES_SET(e,b)		(e)->xclient.data.l[1] = ((e)->xclient.data.l[1] & ~0x1UL) | (((b) == 0) ? 0 : 0x1UL)
#define XDND_ENTER_VERSION(e)			((e)->xclient.data.l[1] >> 24)
#define XDND_ENTER_VERSION_SET(e,v)		(e)->xclient.data.l[1] = ((e)->xclient.data.l[1] & ~(0xFF << 24)) | ((v) << 24)
#define XDND_ENTER_TYPE(e,i)			((e)->xclient.data.l[2 + i])	/* i => (0, 1, 2) */

/* XdndPosition */
#define XDND_POSITION_SOURCE_WIN(e)		((e)->xclient.data.l[0])
#define XDND_POSITION_ROOT_X(e)			((e)->xclient.data.l[2] >> 16)
#define XDND_POSITION_ROOT_Y(e)			((e)->xclient.data.l[2] & 0xFFFFUL)
#define XDND_POSITION_ROOT_SET(e,x,y)		(e)->xclient.data.l[2]  = ((x) << 16) | ((y) & 0xFFFFUL)
#define XDND_POSITION_TIME(e)			((e)->xclient.data.l[3])
#define XDND_POSITION_ACTION(e)			((e)->xclient.data.l[4])

/* XdndStatus */
#define XDND_STATUS_TARGET_WIN(e)		((e)->xclient.data.l[0])
#define XDND_STATUS_WILL_ACCEPT(e)		((e)->xclient.data.l[1] & 0x1L)
#define XDND_STATUS_WILL_ACCEPT_SET(e,b)	(e)->xclient.data.l[1] = ((e)->xclient.data.l[1] & ~0x1UL) | (((b) == 0) ? 0 : 0x1UL)
#define XDND_STATUS_WANT_POSITION(e)		((e)->xclient.data.l[1] & 0x2UL)
#define XDND_STATUS_WANT_POSITION_SET(e,b)	(e)->xclient.data.l[1] = ((e)->xclient.data.l[1] & ~0x2UL) | (((b) == 0) ? 0 : 0x2UL)
#define XDND_STATUS_RECT_X(e)			((e)->xclient.data.l[2] >> 16)
#define XDND_STATUS_RECT_Y(e)			((e)->xclient.data.l[2] & 0xFFFFL)
#define XDND_STATUS_RECT_WIDTH(e)		((e)->xclient.data.l[3] >> 16)
#define XDND_STATUS_RECT_HEIGHT(e)		((e)->xclient.data.l[3] & 0xFFFFL)
#define XDND_STATUS_RECT_SET(e,x,y,w,h)		{(e)->xclient.data.l[2] = ((x) << 16) | ((y) & 0xFFFFUL); (e)->xclient.data.l[3] = ((w) << 16) | ((h) & 0xFFFFUL); }
#define XDND_STATUS_ACTION(e)			((e)->xclient.data.l[4])

/* XdndLeave */
#define XDND_LEAVE_SOURCE_WIN(e)		((e)->xclient.data.l[0])

/* XdndDrop */
#define XDND_DROP_SOURCE_WIN(e)			((e)->xclient.data.l[0])
#define XDND_DROP_TIME(e)			((e)->xclient.data.l[2])

/* XdndFinished */
#define XDND_FINISHED_TARGET_WIN(e)		((e)->xclient.data.l[0])


Atom XdndAware;
Atom XdndStatus;
Atom XdndActionCopy;
Atom XdndSelection;
Atom XdndFinished;
Atom XdndTypeList;

Dnd::Dnd()
: m_Time(0)
, m_DropPosX(0)
, m_DropPosY(0)
, m_Window(0)
, m_DragWindow(0)
, m_DesiredType(None)
{}

Dnd::~Dnd()
{}

void Dnd::initialize(Display* display, Window window)
{
	m_pDisplay      = display;
	m_Window		= window;
	m_Version       = XDND_VERSION;

	XdndAware               = XInternAtom(m_pDisplay, "XdndAware", False);
	XdndSelection           = XInternAtom(m_pDisplay, "XdndSelection", False);
	XdndEnter               = XInternAtom(m_pDisplay, "XdndEnter", False);
	XdndLeave               = XInternAtom(m_pDisplay, "XdndLeave", False);
	XdndPosition            = XInternAtom(m_pDisplay, "XdndPosition", False);
	XdndDrop                = XInternAtom(m_pDisplay, "XdndDrop", False);
	XdndFinished            = XInternAtom(m_pDisplay, "XdndFinished", False);
	XdndStatus              = XInternAtom(m_pDisplay, "XdndStatus", False);
	XdndActionCopy          = XInternAtom(m_pDisplay, "XdndActionCopy", False);
	XdndActionPrivate       = XInternAtom(m_pDisplay, "XdndActionPrivate", False);
	XdndTypeList            = XInternAtom(m_pDisplay, "XdndTypeList", False);

	setDndAware();
}

void Dnd::setDndAware()
{
	XChangeProperty(m_pDisplay, m_Window, XdndAware, XA_ATOM, 32,
		    PropModeAppend, reinterpret_cast<unsigned char*>(&m_Version), 1);
}

void Dnd::unsetDndAware()
{
	XDeleteProperty(m_pDisplay, m_Window, XdndAware);
}

bool Dnd::handleDropEvent()
{
	Window win;
	if (m_DesiredType != 0 && /*time > 0 &&*/ (win = XGetSelectionOwner(m_pDisplay, XdndSelection)))
	{
		XConvertSelection(m_pDisplay, XdndSelection, m_DesiredType, XdndActionPrivate, m_Window, m_Time);
		sendFinished();
		return true;
	}
	return false;
}

bool Dnd::handleEnterEvent(const XEvent& xEvent)
{
	m_DragWindow = XDND_ENTER_SOURCE_WIN(&xEvent);

	CArray<Atom> typeList;

	if (XDND_STATUS_WILL_ACCEPT(&xEvent) == 0)
	{
		//cout << "Only three types" << endl;
		for (int i = 0; i < XDND_THREE; i++)
		{
			if (XDND_ENTER_TYPE(&xEvent, i) != 0)
			{
				typeList.Append(XDND_ENTER_TYPE(&xEvent, i));
			}
		}

		for ( unsigned int i = 0; i < typeList.GetSize(); i++)
		{
			Atom atom = XInternAtom(m_pDisplay, "text/uri-list", False);
			if (typeList[i] == atom)
			{
				m_DesiredType = atom;
			}
		}
	}
	else
	{
		//cout << "More than three types - getting list" << endl;

		generateTypeList(XDND_STATUS_TARGET_WIN(&xEvent), typeList);
		if (!typeList.GetSize())
		{
			for ( unsigned int i = 0; i < typeList.GetSize(); i++)
			{
				Atom atom = XInternAtom(m_pDisplay, "text/uri-list", False);
				if (typeList[i] == atom)
				{
					m_DesiredType = atom;
				}
			}
		}
	}
	return false;
}

bool Dnd::handleLeaveEvent(const XEvent& xEvent)
{
	return true;
}

void Dnd::handlePositionEvent(const XEvent& xEvent)
{
	XEvent  replyXEvent;

	m_DropPosX = XDND_POSITION_ROOT_X((&xEvent));
	m_DropPosY = XDND_POSITION_ROOT_Y((&xEvent));
	m_Time     = XDND_POSITION_TIME((&xEvent));

	memset (&replyXEvent, 0, sizeof(replyXEvent));
	replyXEvent.xany.type            = ClientMessage;
	replyXEvent.xany.display         = m_pDisplay;
	replyXEvent.xclient.window       = m_DragWindow;
	replyXEvent.xclient.message_type = XdndStatus;
	replyXEvent.xclient.format       = 32;

	XDND_STATUS_TARGET_WIN(&replyXEvent) = xEvent.xclient.window;
	XDND_STATUS_WILL_ACCEPT_SET(&replyXEvent, True);
	XDND_STATUS_WANT_POSITION_SET(&replyXEvent, True);
	XDND_STATUS_RECT_SET(&replyXEvent, m_DropPosX, m_DropPosY, 1, 1);
	XDND_STATUS_ACTION(&replyXEvent) = XDND_POSITION_ACTION(&xEvent);

	XSendEvent(m_pDisplay, m_DragWindow, 0, 0, &replyXEvent);
}

CStr Dnd::getDroppedFilenames(const XEvent& xEvent)
{
	CStr filename;

	if (xEvent.xselection.property == None)
	{
		return filename;
	}

	unsigned long   bytesRemaining;
	unsigned long   numItems = 0;
	unsigned char*  s = NULL;
	Atom            actualType;
	int             actualFormat;
	int             offset = 0;

	do
	{
		if(XGetWindowProperty(m_pDisplay, xEvent.xany.window, xEvent.xselection.property, offset / sizeof(unsigned char *), 8, False, AnyPropertyType,
				   &actualType, &actualFormat, &numItems, &bytesRemaining, &s) != Success)
		{
			XFree(s);
			return filename;
		}

		//assert(s);
		if( s ) // mod
		{
			filename += CStr((char*) s);
			XFree(s);
			offset += numItems;
		}
	}
	while (bytesRemaining > 0);

	sendFinished();

	return filename;
}

void Dnd::sendFinished()
{
	XEvent xevent;
	memset (&xevent, 0, sizeof (xevent));
	xevent.xany.type = ClientMessage;
	xevent.xany.display = m_pDisplay;
	xevent.xclient.window = m_DragWindow;
	xevent.xclient.message_type = XdndFinished;
	xevent.xclient.format = 32;

	XDND_FINISHED_TARGET_WIN (&xevent) = m_Window;

	XSendEvent (m_pDisplay, m_DragWindow, 0, 0, &xevent);
}

bool Dnd::generateTypeList(Window window, CArray<Atom>& typeList)
{
	if (window == None)
	{
		return false;
	}

	Atom*           element;
	Atom            type;
	int             format;
	unsigned long   count, remaining;
	unsigned char*  data = NULL;

	XGetWindowProperty(m_pDisplay, window, XdndTypeList, 0, 0x8000000L, False, XA_ATOM, &type, &format, &count, &remaining, &data);

	if((type != XA_ATOM) || (format != 32) || (count == 0) || (!data))
	{
		if(data)
		{
			XFree(data);
		}

		//cerr << "XGetWindowProperty failed in generateTypeList" << endl;
		return false;
	}

	element = (Atom*) data;
	for(unsigned long i = 0; i < count; i++)
	{
		typeList.Append(element[i]);
	}

	XFree(data);

	return true;
}
