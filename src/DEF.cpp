// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// DEF.cpp

#include "DEF.h"

#ifndef VERSION_LIB
#if COND_NOSTD
void _Prolog()
{
	_InitGlobals();
#if defined( __GNUC__ )
	// walk and call the constructors in the ctor_list
	// the ctor list is defined in the linker script
	extern void (* _CTOR_LIST__)();
	void (**apConstructor)() = &_CTOR_LIST__;
	
	// the first in is the number of constructors
	int iTotal = *(int *)apConstructor;
	apConstructor++;
	
	while( iTotal )
	{
		(*apConstructor )();
		iTotal--;
		apConstructor++;
	}
#elif defined( _MSC_VER )
#error not implemented yet!
#else
#error Compiler not supported with NOSTD.
#endif // Compiler
	
	_Entry();
}

void _Epilog()
{
#if defined( __GNUC__ )
	// walk and call the deconstructors in the dtor_list	
	// the dtor list is defined in the linker script
	extern void (* _DTOR_LIST__)();
	void (**apDeconstructor)() = &_DTOR_LIST__;
	
	// the first int is the number of deconstructors
	int iTotal = *(int *)apDeconstructor;
	apDeconstructor++;
	
	while( iTotal )
	{
		(*apDeconstructor)();
		iTotal--;
		apDeconstructor++;
	}
	
	// LIFO!
	while( g_uiDTORIndex )
	{
		--g_uiDTORIndex;
		(g_ptDTOR[g_uiDTORIndex])();
	}
	if( g_ptDTOR )
		HeapFree( GetProcessHeap(), 0, g_ptDTOR );
	
#elif defined( _MSC_VER )
#error not implemented yet!
#else
#error Compiler not supported with NOSTD.
#endif // Compiler
}

void _Entry()
{
#else // not COND_NOSTD
#if COND_SUBSYS_WINDOW
int WINAPI WinMain(	HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow )
{
	UNUSED_PARAMETER( hInstance );
	UNUSED_PARAMETER( hPrevInstance );
	UNUSED_PARAMETER( lpCmdLine );
	UNUSED_PARAMETER( nCmdShow );
#else // CONSOLE
int main( int iArg, char *aaArg[] )
{
	UNUSED_PARAMETER( iArg );
	UNUSED_PARAMETER( aaArg );
#endif // SUBSYS
#endif // COND_NOSTD

	Main();

#if !COND_NOSTD
	return 0;
#else
	_Epilog();
#ifdef __WINDOWS__
	ExitProcess( 0 );
#endif // __WINDOWS__
#endif // COND_STD
}



#if COND_NOSTD

HANDLE g_hHeap = 0;
#if	defined( __GNUC__ )
unsigned int g_uiDTORSize = 0;
unsigned int g_uiDTORIndex = 0;
TFuncDTOR *g_ptDTOR = 0;
#endif // __GNUC__


void _InitGlobals()
{
	g_hHeap = GetProcessHeap();
#if	defined( __GNUC__ )
	g_uiDTORSize = INIT_DTOR_SIZE;
	g_uiDTORIndex = 0;
	g_ptDTOR = new TFuncDTOR[ g_uiDTORSize ];
#endif // __GNUC__
}


#if	defined( __GNUC__ )
void __cxa_pure_virtual( void ){
	//LOG( "attempt to use a virtual function before object has been constructed" );
}

int atexit( void (__cdecl *dtor )( void ))
{
	g_ptDTOR[g_uiDTORIndex] = dtor;
	++g_uiDTORIndex;
	if( g_uiDTORIndex == g_uiDTORSize )
	{
		TFuncDTOR * ptDTOROld = g_ptDTOR;
		g_uiDTORSize <<= 1;
		g_ptDTOR = new TFuncDTOR[ g_uiDTORSize ];
		CopyMemory( g_ptDTOR, ptDTOROld, g_uiDTORIndex * sizeof(TFuncDTOR) );
		delete [] ptDTOROld;
	}
	return 0;
}
#elif defined( _MSC_VER )
int _fltused;

int __cdecl __purecall( void ){
	return 0;
}

int __cdecl atexit( void (__cdecl *dtor )( void ) )
{
#error not implemented yet!
	return 0;
}
#endif // Compiler


void * operator new( size_t bytes ){
	return HeapAlloc( g_hHeap, 0, bytes );
}
void * operator new[] ( size_t bytes ){
	return HeapAlloc( g_hHeap, 0, bytes );
}
void operator delete( void *ptr ){
	HeapFree( g_hHeap, 0, ptr );
}
void operator delete[]( void *ptr ){ 
	HeapFree( g_hHeap, 0, ptr );
}

#endif // COND_NOSTD
#endif // VERSION_LIB

