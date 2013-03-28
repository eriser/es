// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkFFT.inline.h

////////////////////////////////////////////////////////////////////////////////
// COpTkFFT
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
// COpTkFFT
////////////////////////////////////////////////////////////////////////////////

/*==========================================================================*/
/*      Name: Constructor                                                   */
/*      Input parameters:                                                   */
/*        - length: length of the array on which we want to do a FFT.       */
/*                  Range: power of 2 only, > 0.                            */
/*      Throws: std::bad_alloc, anything                                    */
/*==========================================================================*/

template<class T>
COpTkFFT<T>::COpTkFFT()
:	_bit_rev_lut ()
,	_trigo_lut ()
,	_length (0)
,	_nbr_bits (0)
,	_buffer_ptr(0)
,	_sqrt2_2 (T (sqrt (2.0) * 0.5))
/*
,	_length (lLength)
,	_nbr_bits    (int (floor (log (double(lLength)) / log (2.0) + 0.5)))
,	_bit_rev_lut (int (floor (log (double(lLength)) / log (2.0) + 0.5)))
,	_trigo_lut   (int (floor (log (double(lLength)) / log (2.0) + 0.5)))
,	_sqrt2_2 (T (sqrt (2.0) * 0.5))
*/
{}

template<class T>
COpTkFFT<T>::COpTkFFT( const COpTkFFT &roO )
{
	*this = roO;
}

template<class T>
const COpTkFFT<T>& COpTkFFT<T>::operator = ( const COpTkFFT<T> &roO )
{
	Clear();
	Init( roO._length );
	return *this;
}

/*==========================================================================*/
/*      Name: Destructor                                                    */
/*==========================================================================*/

template<class T>
COpTkFFT<T>::~COpTkFFT()
{
	Clear();
}

// mod
template<class T>
void COpTkFFT<T>::Init( const long int uliLength )
{
	_length = uliLength;
	_nbr_bits = int (floor (log (double(uliLength)) / log (2.0) + 0.5));
	_bit_rev_lut.init( _nbr_bits );
	_trigo_lut.init( _nbr_bits );

	assert ((1L << _nbr_bits) == uliLength);
	
	if( _buffer_ptr )
		Clear();
	
	_buffer_ptr = 0;
	if (_nbr_bits > 2)
	{
		_buffer_ptr = new T [_length];
	}
}

// mod
template<class T>
void COpTkFFT<T>::Clear()
{
	if( _buffer_ptr )
	{
		delete [] _buffer_ptr;
		_buffer_ptr = 0;
	}
}

/*==========================================================================*/
/*      Name: FFT                                                        */
/*      Description: Compute the FFT of the array.                          */
/*      Input parameters:                                                   */
/*        - x: pointer on the source array (time).                          */
/*      Output parameters:                                                  */
/*        - f: pointer on the destination array (frequencies).              */
/*             f [0...length(x)/2] = real values,                           */
/*             f [length(x)/2+1...length(x)-1] = imaginary values of        */
/*               coefficents 1...length(x)/2-1.                             */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

template<class T>
void COpTkFFT<T>::FFT (T f [], const T x []) const
{

/*______________________________________________
 *
 * General case
 *______________________________________________
 */

	if (_nbr_bits > 2)
	{
		T *			sf;
		T *			df;

		if (_nbr_bits & 1)
		{
			df = _buffer_ptr;
			sf = f;
		}
		else
		{
			df = f;
			sf = _buffer_ptr;
		}

		/* Do the transformation in several pass */
		{
			int		pass;
			long		nbr_coef;
			long		h_nbr_coef;
			long		d_nbr_coef;
			long		coef_index;

			/* First and second pass at once */
			{
				const long * const	bit_rev_lut_ptr = _bit_rev_lut.get_ptr ();
				coef_index = 0;
				do
				{
					const long		rev_index_0 = bit_rev_lut_ptr [coef_index];
					const long		rev_index_1 = bit_rev_lut_ptr [coef_index + 1];
					const long		rev_index_2 = bit_rev_lut_ptr [coef_index + 2];
					const long		rev_index_3 = bit_rev_lut_ptr [coef_index + 3];

					T	* const	df2 = df + coef_index;
					df2 [1] = x [rev_index_0] - x [rev_index_1];
					df2 [3] = x [rev_index_2] - x [rev_index_3];

					const T		sf_0 = x [rev_index_0] + x [rev_index_1];
					const T		sf_2 = x [rev_index_2] + x [rev_index_3];

					df2 [0] = sf_0 + sf_2;
					df2 [2] = sf_0 - sf_2;
					
					coef_index += 4;
				}
				while (coef_index < _length);
			}

			/* Third pass */
			{
				coef_index = 0;
				const T		sqrt2_2 = _sqrt2_2;
				do
				{
					T				v;

					sf [coef_index] = df [coef_index] + df [coef_index + 4];
					sf [coef_index + 4] = df [coef_index] - df [coef_index + 4];
					sf [coef_index + 2] = df [coef_index + 2];
					sf [coef_index + 6] = df [coef_index + 6];

					v = (df [coef_index + 5] - df [coef_index + 7]) * sqrt2_2;
					sf [coef_index + 1] = df [coef_index + 1] + v;
					sf [coef_index + 3] = df [coef_index + 1] - v;

					v = (df [coef_index + 5] + df [coef_index + 7]) * sqrt2_2;
					sf [coef_index + 5] = v + df [coef_index + 3];
					sf [coef_index + 7] = v - df [coef_index + 3];

					coef_index += 8;
				}
				while (coef_index < _length);
			}

			/* Next pass */
			for (pass = 3; pass < _nbr_bits; ++pass)
			{
				coef_index = 0;
				nbr_coef = 1 << pass;
				h_nbr_coef = nbr_coef >> 1;
				d_nbr_coef = nbr_coef << 1;
				const T	* const	cos_ptr = _trigo_lut.get_ptr (pass);
				do
				{
					long				i;
					const T	*	const sf1r = sf + coef_index;
					const T	*	const sf2r = sf1r + nbr_coef;
					T *			const dfr = df + coef_index;
					T *			const dfi = dfr + nbr_coef;

					/* Extreme coefficients are always real */
					dfr [0] = sf1r [0] + sf2r [0];
					dfi [0] = sf1r [0] - sf2r [0];	// dfr [nbr_coef] =
					dfr [h_nbr_coef] = sf1r [h_nbr_coef];
					dfi [h_nbr_coef] = sf2r [h_nbr_coef];

					/* Others are conjugate complex numbers */
					const T	* const	sf1i = sf1r + h_nbr_coef;
					const T	* const	sf2i = sf1i + nbr_coef;
					for (i = 1; i < h_nbr_coef; ++ i)
					{
						const T		c = cos_ptr [i];					// cos (i*PI/nbr_coef);
						const T		s = cos_ptr [h_nbr_coef - i];	// sin (i*PI/nbr_coef);
						T				v;

						v = sf2r [i] * c - sf2i [i] * s;
						dfr [i] = sf1r [i] + v;
						dfi [-i] = sf1r [i] - v;	// dfr [nbr_coef - i] =

						v = sf2r [i] * s + sf2i [i] * c;
						dfi [i] = v + sf1i [i];
						dfi [nbr_coef - i] = v - sf1i [i];
					}

					coef_index += d_nbr_coef;
				}
				while (coef_index < _length);

				/* Prepare to the next pass */
				{
					T	* const		temp_ptr = df;
					df = sf;
					sf = temp_ptr;
				}
			}
		}
	}

/*______________________________________________
 *
 * Special cases
 *______________________________________________
 */

	/* 4-point FFT */
	else if (_nbr_bits == 2)
	{
		f [1] = x [0] - x [2];
		f [3] = x [1] - x [3];

		const T			b_0 = x [0] + x [2];
		const T			b_2 = x [1] + x [3];
		
		f [0] = b_0 + b_2;
		f [2] = b_0 - b_2;
	}

	/* 2-point FFT */
	else if (_nbr_bits == 1)
	{
		f [0] = x [0] + x [1];
		f [1] = x [0] - x [1];
	}

	/* 1-point FFT */
	else
	{
		f [0] = x [0];
	}
}



/*==========================================================================*/
/*      Name: IFFT                                                       */
/*      Description: Compute the inverse FFT of the array. Notice that      */
/*                   IFFT (FFT (x)) = x * length (x). Data must be          */
/*                   post-scaled.                                           */
/*      Input parameters:                                                   */
/*        - f: pointer on the source array (frequencies).                   */
/*             f [0...length(x)/2] = real values,                           */
/*             f [length(x)/2+1...length(x)] = imaginary values of          */
/*               coefficents 1...length(x)-1.                               */
/*      Output parameters:                                                  */
/*        - x: pointer on the destination array (time).                     */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

template<class T>
void COpTkFFT<T>::IFFT (const T f [], T x []) const
{

/*______________________________________________
 *
 * General case
 *______________________________________________
 */

	if (_nbr_bits > 2)
	{
		T *			sf = const_cast <T *> (f);
		T *			df;
		T *			df_temp;

		if (_nbr_bits & 1)
		{
			df = _buffer_ptr;
			df_temp = x;
		}
		else
		{
			df = x;
			df_temp = _buffer_ptr;
		}

		/* Do the transformation in several pass */
		{
			int			pass;
			long			nbr_coef;
			long			h_nbr_coef;
			long			d_nbr_coef;
			long			coef_index;

			/* First pass */
			for (pass = _nbr_bits - 1; pass >= 3; --pass)
			{
				coef_index = 0;
				nbr_coef = 1 << pass;
				h_nbr_coef = nbr_coef >> 1;
				d_nbr_coef = nbr_coef << 1;
				const T	*const cos_ptr = _trigo_lut.get_ptr (pass);
				do
				{
					long				i;
					const T	*	const sfr = sf + coef_index;
					const T	*	const sfi = sfr + nbr_coef;
					T *			const df1r = df + coef_index;
					T *			const df2r = df1r + nbr_coef;

					/* Extreme coefficients are always real */
					df1r [0] = sfr [0] + sfi [0];		// + sfr [nbr_coef]
					df2r [0] = sfr [0] - sfi [0];		// - sfr [nbr_coef]
					df1r [h_nbr_coef] = sfr [h_nbr_coef] * 2;
					df2r [h_nbr_coef] = sfi [h_nbr_coef] * 2;

					/* Others are conjugate complex numbers */
					T * const	df1i = df1r + h_nbr_coef;
					T * const	df2i = df1i + nbr_coef;
					for (i = 1; i < h_nbr_coef; ++ i)
					{
						df1r [i] = sfr [i] + sfi [-i];		// + sfr [nbr_coef - i]
						df1i [i] = sfi [i] - sfi [nbr_coef - i];

						const T		c = cos_ptr [i];					// cos (i*PI/nbr_coef);
						const T		s = cos_ptr [h_nbr_coef - i];	// sin (i*PI/nbr_coef);
						const T		vr = sfr [i] - sfi [-i];		// - sfr [nbr_coef - i]
						const T		vi = sfi [i] + sfi [nbr_coef - i];

						df2r [i] = vr * c + vi * s;
						df2i [i] = vi * c - vr * s;
					}

					coef_index += d_nbr_coef;
				}
				while (coef_index < _length);

				/* Prepare to the next pass */
				if (pass < _nbr_bits - 1)
				{
					T	* const	temp_ptr = df;
					df = sf;
					sf = temp_ptr;
				}
				else
				{
					sf = df;
					df = df_temp;
				}
			}

			/* Antepenultimate pass */
			{
				const T		sqrt2_2 = _sqrt2_2;
				coef_index = 0;
				do
				{
					df [coef_index] = sf [coef_index] + sf [coef_index + 4];
					df [coef_index + 4] = sf [coef_index] - sf [coef_index + 4];
					df [coef_index + 2] = sf [coef_index + 2] * 2;
					df [coef_index + 6] = sf [coef_index + 6] * 2;

					df [coef_index + 1] = sf [coef_index + 1] + sf [coef_index + 3];
					df [coef_index + 3] = sf [coef_index + 5] - sf [coef_index + 7];

					const T		vr = sf [coef_index + 1] - sf [coef_index + 3];
					const T		vi = sf [coef_index + 5] + sf [coef_index + 7];

					df [coef_index + 5] = (vr + vi) * sqrt2_2;
					df [coef_index + 7] = (vi - vr) * sqrt2_2;

					coef_index += 8;
				}
				while (coef_index < _length);
			}

			/* Penultimate and last pass at once */
			{
				coef_index = 0;
				const long *	bit_rev_lut_ptr = _bit_rev_lut.get_ptr ();
				const T	*	sf2 = df;
				do
				{
					{
						const T		b_0 = sf2 [0] + sf2 [2];
						const T		b_2 = sf2 [0] - sf2 [2];
						const T		b_1 = sf2 [1] * 2;
						const T		b_3 = sf2 [3] * 2;

						x [bit_rev_lut_ptr [0]] = b_0 + b_1;
						x [bit_rev_lut_ptr [1]] = b_0 - b_1;
						x [bit_rev_lut_ptr [2]] = b_2 + b_3;
						x [bit_rev_lut_ptr [3]] = b_2 - b_3;
					}
					{
						const T		b_0 = sf2 [4] + sf2 [6];
						const T		b_2 = sf2 [4] - sf2 [6];
						const T		b_1 = sf2 [5] * 2;
						const T		b_3 = sf2 [7] * 2;

						x [bit_rev_lut_ptr [4]] = b_0 + b_1;
						x [bit_rev_lut_ptr [5]] = b_0 - b_1;
						x [bit_rev_lut_ptr [6]] = b_2 + b_3;
						x [bit_rev_lut_ptr [7]] = b_2 - b_3;
					}

					sf2 += 8;
					coef_index += 8;
					bit_rev_lut_ptr += 8;
				}
				while (coef_index < _length);
			}
		}
	}

/*______________________________________________
 *
 * Special cases
 *______________________________________________
 */

	/* 4-point IFFT */
	else if (_nbr_bits == 2)
	{
		const T		b_0 = f [0] + f [2];
		const T		b_2 = f [0] - f [2];

		x [0] = b_0 + f [1] * 2;
		x [2] = b_0 - f [1] * 2;
		x [1] = b_2 + f [3] * 2;
		x [3] = b_2 - f [3] * 2;
	}

	/* 2-point IFFT */
	else if (_nbr_bits == 1)
	{
		x [0] = f [0] + f [1];
		x [1] = f [0] - f [1];
	}

	/* 1-point IFFT */
	else
	{
		x [0] = f [0];
	}
}



/*==========================================================================*/
/*      Name: Rescale                                                       */
/*      Description: Scale an array by divide each element by its length.   */
/*                   This function should be called after FFT + IFFT.       */
/*      Input/Output parameters:                                            */
/*        - x: pointer on array to rescale (time or frequency).             */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

template<class T>
void COpTkFFT<T>::Rescale (T x []) const
{
	const T		mul = T (1.0 / _length);
	long				i = _length - 1;

	do
	{
		x [i] *= mul;
		--i;
	}
	while (i >= 0);
}



/*\\\ NESTED CLASS MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*==========================================================================*/
/*      Name: Constructor                                                   */
/*==========================================================================*/

template<class T>
COpTkFFT<T>::BitReversedLUT::BitReversedLUT ()
: _ptr( 0 )
{}

/*==========================================================================*/
/*      Name: Destructor                                                    */
/*==========================================================================*/
template<class T>
COpTkFFT<T>::BitReversedLUT::~BitReversedLUT (void)
{
	clear();
}


/*==========================================================================*/
/*      Name: Init						            */
/*      Input parameters:                                                   */
/*        - nbr_bits: number of bits of the array on which we want to do a  */
/*                    FFT. Range: > 0                                       */
/*      Throws: std::bad_alloc                                              */
/*==========================================================================*/
template<class T>
void COpTkFFT<T>::BitReversedLUT::init (const int nbr_bits)
{
	long length;
	long cnt;
	long br_index;
	long bit;
	
	if( _ptr )
		clear();
	
	length = 1L << nbr_bits;
	_ptr = new long [length];

	br_index = 0;
	_ptr [0] = 0;
	for (cnt = 1; cnt < length; ++cnt)
	{
		/* ++br_index (bit reversed) */
		bit = length >> 1;
		while (((br_index ^= bit) & bit) == 0)
		{
			bit >>= 1;
		}

		_ptr [cnt] = br_index;
	}
}

template<class T>
void COpTkFFT<T>::BitReversedLUT::clear ()
{
	if( _ptr )
	{
		delete [] _ptr;
		_ptr = 0;
	}
}


/*==========================================================================*/
/*      Name: Constructor                                                   */
/*==========================================================================*/
template<class T>
COpTkFFT<T>::TrigoLUT::TrigoLUT (void)
: _ptr( 0 )
{}

/*==========================================================================*/
/*      Name: Destructor                                                    */
/*==========================================================================*/
template<class T>
COpTkFFT<T>::TrigoLUT::~TrigoLUT (void)
{
	clear();
}

/*==========================================================================*/
/*      Name: init                                                          */
/*      Input parameters:                                                   */
/*        - nbr_bits: number of bits of the array on which we want to do a  */
/*                    FFT. Range: > 0                                       */
/*      Throws: std::bad_alloc, anything                                    */
/*==========================================================================*/

template<class T>
void COpTkFFT<T>::TrigoLUT::init(const int nbr_bits)
{
	long total_len;
	
	if( _ptr )
		clear();
	
	_ptr = 0;
	if (nbr_bits > 3)
	{
		total_len = (1L << (nbr_bits - 1)) - 4;
		_ptr = new T [total_len];

		//const double	PI = atan (1) * 4;
		for (int level = 3; level < nbr_bits; ++level)
		{
			const long		level_len = 1L << (level - 1);
			T	* const	level_ptr = const_cast<T *> (get_ptr (level));
			const double	mul = OP_TK_PI / (level_len << 1);

			for (long i = 0; i < level_len; ++ i)
			{
				level_ptr [i] = (T) cos (i * mul);
			}
		}
	}
}

// mod
template<class T>
void COpTkFFT<T>::TrigoLUT::clear()
{
	if( _ptr )
	{
		delete [] _ptr;
		_ptr = 0;
	}
}
