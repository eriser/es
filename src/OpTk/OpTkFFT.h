// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkFFT.h

#ifndef _OP_TK_FFT_H_
#define _OP_TK_FFT_H_

#include "OpTkDEF.h"

// Real-to-Complex FFT and Complex-to-Real IFFT (formal modifiziert)
// Quelle(n):
// - http://www.musicdsp.org/archive.php?classid=2#82
//   Version 1.01, 1999/11/07
//   (c) 1999 - Laurent de Soras
//
////////////////////////////////////////////////////////////////////////////////
template<class T>
class COpTkFFT
{
public:	
	explicit COpTkFFT();
	COpTkFFT( const COpTkFFT &roO );
	virtual ~COpTkFFT();
	const COpTkFFT& operator = ( const COpTkFFT &roO );
	
public:
	void Init( const long int uliLength );
	void Clear();
	
	void FFT( T f [], const T x [] ) const;
	void IFFT( const T f [], T x [] ) const;
	void Rescale( T x [] ) const;
	
private:
	// Bit-reversed look-up table nested class
	////////////////////////////////////////////////////////////////////////////////
	class BitReversedLUT
	{
	public:
		BitReversedLUT ();
		~BitReversedLUT ();
		void init(const int nbr_bits);
		void clear();
		const long * get_ptr () const
		{
			return (_ptr);
		}
	private:
		long * _ptr;
	};

	// Trigonometric look-up table nested class
	////////////////////////////////////////////////////////////////////////////////
	class	TrigoLUT
	{
	public:
		TrigoLUT ();
		~TrigoLUT ();
		void init(const int nbr_bits);
		void clear();
		const T * get_ptr (const int level) const
		{
			return (_ptr + (1L << (level - 1)) - 4);
		};
	private:
		T * _ptr;
	};
	
	BitReversedLUT	_bit_rev_lut;
	TrigoLUT	_trigo_lut;
	long		_length;
	int		_nbr_bits;
	T *		_buffer_ptr;
	const T		_sqrt2_2;
};

#include "OpTkFFT.inline.h"

#endif // _OP_TK_FFT_H_
