// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkBufferDelay.h

#ifndef _OP_TK_BUFFER_DELAY_
#define _OP_TK_BUFFER_DELAY_

#include "OpTkDEF.h"
#include "OpTkBuffer.h"

// Bei dynamischer Änderung mit SetDelay() werdem jeweils bei Überschreitung 
// der aktuellen Puffergrösse OP_TK_DELAY_ALLOC_ADD Samples zusätzlich zur
// erforderlichen Puffergrösse addiert, um bei kontinuierlicher Änderung
// nicht ständig neu Speicher allokieren zu müssen.
#define OP_TK_DELAY_ALLOC_ADD	256

// Anmerkungen: 
// - Ein Delay von 0 entspreicht einer Verzögerung von 1 bei aufeinander
//   folgenden Aufruf von Read(), Write().
// - Um eine Verzögerung von N Samples zu ermöglichen, reicht ein Puffer der
//   Grösse N.
// - Nur die Schreib-Operation verändert die Position. Die Lese-Operation liest
//   berechnet immer die aktuelle realtive Verzögerung, somit besteht bleit
//   der Abstand der Lese und Schreib-Position sicher immer gleich.
// - Die vielen ähnlichen Eigenschaften Buffer*Delay-Klassen lassen sich leider
//   nur schlecht in einer gemeinsamen Basisklasse abstrahieren...
// - Der Max-Wert aus COpTkBuffer bleibt unberührt.

// Verzögerungspuffer. Feste Grösse.
////////////////////////////////////////////////////////////////////////////////
template<class T, const unsigned int SIZE>
class COpTkBufferFixedDelay : public COpTkBufferFixed<T, SIZE>
{
public:
	COpTkBufferFixedDelay();
	COpTkBufferFixedDelay( const COpTkBufferFixedDelay &roO );
	COpTkBufferFixedDelay & operator = ( const COpTkBufferFixedDelay &roO );
	void Clear();
	void Reset();
	inline unsigned int GetDelay() const { return m_iDelay; }
	void SetDelay( unsigned int uiDelay );
	void SetDelay( double dDelay ); // Rundet und ruft dann andere Version auf.
	
	inline T ReadDelay() const
	{
		int iPosRead = m_iPos - m_iDelay;
		if( iPosRead < 0 )
			iPosRead = (int)SIZE + iPosRead;
		return this->m_ptBuffer[iPosRead];
	}
	inline T ReadDelay( int iDelay ) const
	{
		if( abs( iDelay ) > m_iDelay ) // Gegen Unlogik. Maximales Delay.
			iDelay = m_iDelay; 
		int iPosRead = m_iPos - iDelay;
		if( abs( iPosRead ) >= (int)SIZE )
			iPosRead %= (int)SIZE;
		if( iPosRead < 0 )
			iPosRead += (int)SIZE;
		return this->m_ptBuffer[iPosRead];
	}
	inline void WriteDelay( T tIn )
	{
		if( ++m_iPos >= (int)this->m_uiSize )
			m_iPos = 0;
		this->m_ptBuffer[m_iPos] = tIn;
	}
protected:
	int m_iPos;
	int m_iDelay;
};

// Verzögerungspuffer. Variable Grösse.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkBufferVarDelay : public COpTkBufferVar<T>
{
public:
	COpTkBufferVarDelay();
	COpTkBufferVarDelay( const COpTkBufferVarDelay &roO );
	COpTkBufferVarDelay & operator = ( const COpTkBufferVarDelay &roO );
	void Clear();
	void Reset();
	inline unsigned int GetDelay() const { return m_iDelay; }
	void SetDelay( unsigned int uiDelay ); // Wird automatisch vergrössert, falls nötig.
	void SetDelay( double dDelay ); // Rundet und ruft dann andere Version auf.
	void UpdateSize();
	inline T ReadDelay() const
	{
		int iPosRead = m_iPos - m_iDelay;
		if( iPosRead < 0 )
			iPosRead = (int)this->m_uiSize + iPosRead;
		return this->m_ptBuffer[iPosRead];
	}
	inline T ReadDelay( int iDelay ) const
	{
		if( abs( iDelay ) > m_iDelay ) // Gegen Unlogik. Maximales Delay.
			iDelay = m_iDelay; 
		int iPosRead = m_iPos - iDelay;
		if( abs( iPosRead ) >= (int)this->m_uiSize )
		{
			if( this->m_uiSize )
				iPosRead %= (int)this->m_uiSize;
			else
				iPosRead = 0;
		}
		if( iPosRead < 0 )
			iPosRead += (int)this->m_uiSize;
		return this->m_ptBuffer[iPosRead];
	}
	inline void WriteDelay( T tIn )
	{
		if( ++m_iPos >= (int)this->m_uiSize )
			m_iPos = 0;
		this->m_ptBuffer[m_iPos] = tIn;
	}
protected:
	int m_iPos;
	int m_iDelay;
};

// Verzögerungspuffer. Variable Grösse. Lineare Interpolation.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkBufferVarDelayInterpol : public COpTkBufferVarInterpol<T>
{
public:
	COpTkBufferVarDelayInterpol();
	COpTkBufferVarDelayInterpol( const COpTkBufferVarDelayInterpol &roO );
	COpTkBufferVarDelayInterpol & operator = ( const COpTkBufferVarDelayInterpol &roO );
	void Clear();
	void Reset();
	inline double GetDelay() const { return m_iDelay + m_dDelayFrc; }
	void SetDelay( unsigned int uiDelay ); // Castet und ruft dann andere Version auf.
	void SetDelay( double dDelay ); // Wird automatisch vergrössert, falls nötig.
	void UpdateSize();
	T ReadDelay() const;
	T ReadDelay( double dDelay ) const;
	inline void WriteDelay( T tIn )
	{
		if( ++m_iPos >= (int)this->m_uiSize  )
			m_iPos = 0;
		this->m_ptBuffer[m_iPos] = tIn;
	}
protected:
	int m_iPos;
	int m_iDelay;
	double m_dDelayFrc;
};

#include "OpTkBufferDelay.inline.h"

#endif // _OP_TK_BUFFER_DELAY_
