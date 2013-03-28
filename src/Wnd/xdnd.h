// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// xdnd.h

// http://www.newplanetsoftware.com/xdnd/
// http://code.google.com/p/glover/
// dirk.vdb

#ifndef X_DND_H
#define X_DND_H

#define X_DND_VERSION	"0.01"

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xatom.h>

#include "../Str/Str.h"
#include "../Array/Array.h"

// Drag and Drop.
////////////////////////////////////////////////////////////////////////////////
class Dnd
{
public:
	Dnd();
	~Dnd();

	void initialize(Display* display, Window window);
	bool handleEnterEvent(const XEvent& xEvent);
	bool handleLeaveEvent(const XEvent& xEvent);
	bool handleDropEvent();
	void handlePositionEvent(const XEvent& xEvent);

	CStr getDroppedFilenames(const XEvent& xEvent);

public:
	Atom XdndEnter;
	Atom XdndLeave;
	Atom XdndDrop;
	Atom XdndPosition;
	Atom XdndActionPrivate;

private:
	void setDndAware();
	void unsetDndAware();
	void sendFinished();
	bool generateTypeList( Window window, CArray<Atom>& typeList );

private:
	Time m_Time;
	int m_DropPosX;
	int m_DropPosY;
	Window m_Window;
	Window m_DragWindow;

	Display* m_pDisplay;
	Atom m_Version;
	Atom m_DesiredType;
};

#endif // X_DND_H
