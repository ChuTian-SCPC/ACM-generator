/*
MIT License

Copyright (c) 2024-2050 Twilight-Dream & With-Sky

https://github.com/Twilight-Dream-Of-Magic/
https://github.com/With-Sky

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "BigInteger.hpp"
#include "hint_for_easy_bigint.hpp"

namespace TwilightDream::BigInteger
{
	/*
	
	BigInteger Private
	
	*/

	void BigInteger::Clean()
	{
		size_t length = values.size();
		while ( length > 0 && values[ length - 1 ] == 0 )
		{
			length--;
		}
		values.resize( length );
	}

	/*
	
	BigInteger Public
	
	*/

	BigInteger::BigInteger() {}

	BigInteger::BigInteger( uint64_t value ) : values( 1, value )
	{
		//这个是有争议的，我们目前把它设置为数值"0"，而不是空比特容器的"0"
		//This is controversial, we currently set it to the value "0" instead of "0" for an empty bit container.
		//Clean();
	}

	BigInteger::BigInteger( const std::string& number_string )
	{
		FromString( number_string );
	}

	BigInteger::BigInteger( const std::string& number_string, uint32_t base )
	{
		FromString( number_string, base );
	}

	BigInteger::BigInteger( const BigInteger& other ) noexcept : values( other.values ) {}

	BigInteger::BigInteger( BigInteger&& other ) noexcept : values( std::move( other.values ) ) {}

	void BigInteger::Print( uint32_t base_value ) const
	{
		if ( base_value == 2 )
		{
			PrintBinary( false );
		}
		if ( base_value == 10 )
		{
			std::cout << ToString( 10 ) << "\n\n";
		}
		if ( base_value == 16 )
		{
			std::cout << ToString( 16 ) << "\n\n";
		}
	}

	void BigInteger::PrintBinary( bool have_space_with_block ) const
	{
		if ( !have_space_with_block )
		{
			std::cout << ToBinaryString( BitLength() ) << "\n\n";
			return;
		}

		if ( IsZero() )
		{
			std::cout << "0";
		}
		size_t i = values.size();
		while ( i > 0 )
		{
			i--;
			std::cout << std::bitset<64>( values[ i ] ) << " ";
		}
		std::cout << "\n\n";
	}

	//Return *this = *this[block_index] * uint64_t(multiply_num[block_index]) + uint64_t(add_num[block_index])
	BigInteger& BigInteger::AddMultiplyNumber( uint64_t add_num, uint64_t multiply_num )
	{
		size_t len = values.size();
		values.resize( len + 1 );
		HyperInt::Arithmetic::abs_mul_add_num64( values.data(), len, values.data(), add_num, multiply_num );
		Clean();
		return *this;
	}

	//Return *this = *this[block_index] % uint64_t(divisor)
	uint64_t BigInteger::DividModuloNumber( uint64_t divisor )
	{
		uint64_t rem = HyperInt::Arithmetic::abs_div_rem_num64( values.data(), values.size(), values.data(), divisor );
		Clean();
		return rem;
	}

	BigInteger& BigInteger::operator+=( const BigInteger& other )
	{
		return Add( other );
	}

	BigInteger& BigInteger::Add( const BigInteger& other )
	{
		// Determine the maximum size for the loop and result storage
		const size_t this_size = values.size();
		const size_t other_size = other.values.size();

		// Resize the storage for the result
		values.resize( std::max( this_size, other_size ) + 1 );

		// Initialize carry and perform addition
		HyperInt::Arithmetic::abs_add_binary( values.data(), this_size, other.values.data(), other_size, values.data() );

		// Remove leading zeros and resize the storage
		Clean();

		// Return a reference to the modified current instance
		return *this;
	}

	BigInteger& BigInteger::operator-=( const BigInteger& other )
	{
		return Subtract( other );
	}

	BigInteger BigInteger::Difference( const BigInteger& other ) const
	{
		if ( *this > other )
		{
			return *this - other;
		}
		return other - *this;
	}

	BigInteger& BigInteger::Subtract( const BigInteger& other )
	{
		// Determine the size of current and other BigIntegers
		const size_t this_size = values.size();
		const size_t other_size = other.values.size();

		// Perform subtraction for each digit
		HyperInt::Arithmetic::abs_sub_binary( values.data(), this_size, other.values.data(), other_size, values.data() );

		// Remove leading zeros and resize the storage
		Clean();

		// Return a reference to the modified current instance
		return *this;
	}

	BigInteger BigInteger::operator++()
	{
		*this += BigInteger( 1 );
		return *this;
	}

	BigInteger BigInteger::operator--()
	{
		*this -= BigInteger( 1 );
		return *this;
	}

	BigInteger BigInteger::operator++( int )
	{
		BigInteger copy = *this;
		*this += BigInteger( 1 );
		return copy;
	}

	BigInteger BigInteger::operator--( int )
	{
		BigInteger copy = *this;
		*this -= BigInteger( 1 );
		return copy;
	}

	//Return 2^{bits} / (*this * 2^{offset})
	BigInteger BigInteger::ReciprocalNewtonIteration( size_t bits, size_t offset ) const
	{
		size_t bits_n = BitLength();
		if ( bits_n < 64 )
		{
			if ( bits_n < offset + bits_n )
			{
				return 0;
			}
			BigInteger dividend = TwoPowerN( bits - offset );
			dividend.DividModuloNumber( values[ 0 ] );
			return dividend;
		}
		if ( bits > ( bits_n + offset ) * 2 )
		{
			offset = bits - ( bits_n + offset ) * 2;
			return ReciprocalNewtonIteration( bits + offset, offset );
		}
		size_t bits_n0 = ( bits_n + offset ) / 2 + 1;
		size_t bits0 = bits_n0 * 2, shift = 0, offset0 = 0;
		if ( bits_n0 > bits_n )
		{
			offset0 = bits_n0 - bits_n;
		}
		else
		{
			shift = bits_n - bits_n0;
		}
		BigInteger x0 = *this >> shift;
		x0 = x0.ReciprocalNewtonIteration( bits0, offset0 );
		BigInteger square = x0 * x0;
		square *= ( *this );
		bits0 = bits0 - offset0 + shift;
		bits = bits - offset;
		x0 = x0 << ( bits0 + 1 );
		x0 -= square;
		return x0 >> ( bits0 * 2 - bits );
	}

	//Return *this / divisor
	BigInteger BigInteger::DivideModuloNewtonIteration( const BigInteger& divisor, BigInteger& remainder ) const
	{
		if ( divisor.IsZero() )
		{
			throw std::overflow_error( "Divide by zero" );
		}
		if ( *this < divisor )
		{
			remainder = *this;
			return 0;
		}
		size_t	   bits_a = this->BitLength();
		size_t	   bits_b = divisor.BitLength();
		BigInteger quotient;
		if ( divisor.Size() == 1 )
		{
			quotient = *this;
			remainder = quotient.DividModuloNumber( divisor.values[ 0 ] );
			return quotient;
		}
		size_t shift = 0;
		if ( bits_b * 2 > bits_a )
		{
			shift = bits_b * 2 - bits_a;
			shift = std::min( shift, bits_b - 1 );
		}
		size_t	   bits = bits_a - shift + 1;
		BigInteger inv = ( divisor >> shift ).ReciprocalNewtonIteration( bits );
		quotient = ( ( *this >> shift ) * inv ) >> bits;
		BigInteger product = quotient * divisor;
		while ( product > *this )
		{
			quotient -= 1;
			product -= divisor;
		}
		remainder = *this - product;
		while ( remainder >= divisor )
		{
			quotient += 1;
			remainder -= divisor;
		}
		return quotient;
	}

	BigInteger BigInteger::DivideModuloDivideConquer(const BigInteger& divisor, BigInteger& remainder) const
	{
		if (divisor.IsZero())
		{
			throw std::overflow_error("Divide by zero");
		}
		if (this == &divisor)
		{
			remainder.values.clear();
			return BigInteger{ 1 };
		}
		if (*this < divisor)
		{
			remainder = *this;
			return BigInteger{ 0 };
		}
		size_t	   dividend_len = Size();
		size_t	   divisor_len = divisor.Size();
		BigInteger result;
		if (divisor.Size() == 1)
		{
			result = *this;
			remainder = result.DividModuloNumber(divisor.values[0]);
		}
		else
		{
			result.values.resize(dividend_len - divisor_len + 1);
			remainder = *this;
			HyperInt::Arithmetic::abs_div64(remainder.values.data(), dividend_len, divisor.values.data(), divisor_len, result.values.data());
		}
		result.Clean();
		remainder.Clean();
		return result;
	}

	//Return *this / divisor
	BigInteger BigInteger::DivideModulo(const BigInteger& divisor, BigInteger& remainder) const
	{
		if (divisor.IsZero())
		{
			throw std::overflow_error("Divide by zero");
		}
		if (this == &divisor)
		{
			remainder.values.clear();
			return BigInteger{ 1 };
		}
		if (*this < divisor)
		{
			remainder = *this;
			return BigInteger{ 0 };
		}
		BigInteger result;
		if (divisor.Size() == 1)
		{
			result = *this;
			remainder = result.DividModuloNumber(divisor.values[0]);
		}
		else if (divisor.Size() <= 8192)
		{
			result = this->DivideModuloDivideConquer(divisor, remainder);
		}
		else
		{
			result = this->DivideModuloNewtonIteration(divisor, remainder);
		}
		result.Clean();
		remainder.Clean();
		return result;
	}

	BigInteger& BigInteger::operator*=( const BigInteger& other )
	{
		if ( this->IsZero() || other.IsZero() )
		{
			return *this = 0;
		}
		size_t this_len = values.size();
		size_t other_len = other.values.size();
		if ( other_len == 1 )
		{
			return AddMultiplyNumber( 0, other.values[ 0 ] );
		}
		values.resize( this_len + other_len );
		auto& multiplier = HyperInt::Arithmetic::getMultiplier();
		multiplier( values.data(), this_len, other.values.data(), other_len, values.data() );
		Clean();
		return *this;
	}

	BigInteger& BigInteger::operator/=( const BigInteger& other )
	{
		if ( this == &other )
		{
			*this = 1;
			return *this;
		}
		if ( other.Size() == 1 )
		{
			this->DividModuloNumber( other.values[ 0 ] );
			return *this;
		}
		BigInteger r;
		*this = DivideModulo( other, r );
		return *this;
	}

	BigInteger& BigInteger::operator%=( const BigInteger& other )
	{
		if ( this == &other )
		{
			*this = 0;
			return *this;
		}
		if ( other.Size() == 1 )
		{
			uint64_t remainder = this->DividModuloNumber( other.values[ 0 ] );
			return *this = remainder;
		}
		BigInteger r;
		DivideModulo( other, r );
		return *this = r;
	}

	BigInteger& BigInteger::Power( size_t exponent )
	{
		BigInteger result = 1;
		BigInteger base = *this;

		if ( exponent == 0 )
		{
			*this = 1;
			return *this;
		}

		while ( true )
		{
			if ( exponent & 1 )
			{
				result *= base;
			}
			if ( exponent == 0 )
			{
				break;
			}
			base *= base;
			exponent >>= 1;
		}

		values = std::move( result.values );
		return *this;
	}

	BigInteger& BigInteger::BigPower( const BigInteger& exponent )
	{
		BigInteger result = 1;
		BigInteger base = *this;

		if ( exponent.IsZero() )
		{
			*this = 1;
			return *this;
		}

		size_t index_bits = exponent.BitLength();
		for ( size_t i = 0; i < index_bits; i++ )
		{
			if ( exponent.GetBit( i ) )
			{
				result *= base;
			}

			base *= base;
		}

		values = std::move( result.values );
		return *this;
	}

	BigInteger BigInteger::ModuloBasePower( size_t n ) const
	{
		if ( n >= Size() )
		{
			return *this;
		}
		BigInteger result;
		result.values.assign( values.begin(), values.begin() + n );
		return result;
	}

	// Return BASE^{n}
	// BASE = 2 ^{64} (65 Bit)
	BigInteger BigInteger::BasePowerN( size_t n )
	{
		BigInteger result;
		result.values.resize( n + 1, 0 );
		result.values[ n ] = 1;
		return result;
	}

	// Return 2^{n}
	BigInteger BigInteger::TwoPowerN( size_t n )
	{
		size_t	   bits = n + 1;
		BigInteger result;
		result.values.resize( ( bits + DIGIT_BITS - 1 ) / DIGIT_BITS, 0 );
		result.SetBit( n );
		return result;
	}

	BigInteger BigInteger::RightShiftBlock( size_t d ) const
	{
		if ( d >= Size() )
			return 0;
		BigInteger tmp;
		tmp.values.assign( values.begin() + d, values.end() );	//shift d element, not bit
		return tmp;
	}

	BigInteger BigInteger::LeftShiftBlock( size_t d ) const
	{
		if ( IsZero() )
			return 0;
		BigInteger tmp;
		tmp.values.reserve( values.size() + d );
		tmp.values.resize( d );
		tmp.values.insert( tmp.values.end(), values.begin(), values.end() );
		return tmp;
	}

	BigInteger& BigInteger::Sqrt()
	{
		BigInteger s = 0, t, u = *this;
		do
		{
			s = u;
			t = s + ( *this ) / s;
			u = t >> ( uint32_t )1;
		} while ( u < s );

		values = std::move( s.values );
		return *this;
	}

	BigInteger& BigInteger::Cbrt()
	{
		if ( *this == 0 )
		{
			return *this;
		}

		BigInteger x0 = *this;
		BigInteger x1 = (x0 * 2 + (*this / (x0 * x0))) / 3;

		while ( x1 < x0 )
		{
			x0 = x1;
			x1 = (x0 * 2 + (*this / (x0 * x0))) / 3;
		}

		values = std::move( x0.values );
		return *this;
	}

	BigInteger& BigInteger::PowerWithModulo( const BigInteger& exponent, const BigInteger& modulo )
	{
		const BigInteger ZERO = 0;
		const BigInteger ONE = 1;
		BigInteger		 result = ONE;
		BigInteger		 base = *this;
		BigInteger		 exponent_value = exponent;

		if ( modulo == ONE )
		{
			*this = ZERO;
			return *this;
		}

		size_t index_bits = exponent_value.BitLength();
		for ( size_t i = 0; i < index_bits; i++ )
		{
			if ( exponent_value.GetBit( i ) )
			{
				result *= base;
				result %= modulo;
			}

			base = base * base % modulo;
		}

		values = std::move( result.values );
		return *this;
	}

	BigInteger& BigInteger::MultiplyWithModulo( const BigInteger& other, const BigInteger modulo )
	{
		const BigInteger ZERO = 0;
		const BigInteger ONE = 1;
		const BigInteger TWO = 2;

		if ( modulo == ONE )
		{
			*this = ZERO;
			return *this;
		}

		BigInteger result = ZERO;
		BigInteger self = *this;
		BigInteger other_value = other;

		while ( !other_value.IsZero() )
		{
			if ( !other_value.IsEven() )
			{
				result += self;
				result %= modulo;
			}

			self *= TWO;
			self %= modulo;

			if ( other_value.IsPowerOfTwo() )
				other_value >>= 1;
			else
				other_value /= TWO;
		}

		values = std::move( result.values );
		return *this;
	}

	bool BigInteger::IsEven() const
	{
		if ( values.empty() )
		{
			return true;
		}
		return ( values[ 0 ] & 1 ) == 0;
	}

	bool BigInteger::IsPowerOfTwo() const
	{
		if ( this->IsZero() )
			return false;
		size_t	   length = values.size();
		digit_type n = values[ length - 1 ];
		if ( ( n & ( n - 1 ) ) != 0 )
		{
			return false;
		}
		size_t i = length - 1;
		while ( i > 0 )
		{
			i--;
			if ( values[ i ] != 0 )
			{
				return false;
			}
		}
		return true;
	}

	// size() == 0, all elements are zeros -> true
	bool BigInteger::IsZero() const
	{
		if ( values.empty() )
		{
			return true;
		}

		return this->values == std::vector<digit_type>( this->values.size(), 0 );
	}

	bool BigInteger::IsNegative() const
	{
		return false;
	}

	size_t BigInteger::Size() const
	{
		return values.size();
	}

	size_t BigInteger::CountLeadingZeros() const
	{
		if ( IsZero() )
			return 0;

		size_t block_length = values.size(), count = 0;
		for ( size_t i = block_length; i > 0; i-- )
		{
			if ( values[ i - 1 ] != 0 )
			{
				count = DIGIT_BITS * ( block_length - i ) + HyperInt::hint_clz( values[ i - 1 ] );
				break;
			}
		}

		return count;
	}

	size_t BigInteger::CountTrailingZeros() const
	{
		if ( IsZero() )
		{
			return 0;
		}
		size_t block_length = values.size(), count = 0;
		for ( size_t i = 0; i < block_length; i++ )
		{
			if ( values[ i ] != 0 )
			{
				count = DIGIT_BITS * i + HyperInt::hint_ctz( values[ i ] );
				break;
			}
		}
		return count;
	}

	size_t BigInteger::BitLength() const
	{
		size_t length = values.size();
		if ( length == 0 )
		{
			return 0;
		}
		return ( length - 1 ) * DIGIT_BITS + HyperInt::hint_bit_length( values[ length - 1 ] );
	}

	void BigInteger::SetBlock( size_t block_position, uint64_t value )
	{
		if ( block_position >= values.size() )
		{
			return;
		}
		values[ block_position ] = value;
	}

	digit_type BigInteger::GetBlock( size_t block_position ) const
	{
		if ( block_position >= values.size() )
		{
			return 0;
		}
		return values[ block_position ];
	}

	bool BigInteger::GetBit( size_t bit_position ) const
	{
		size_t wrapper_index = bit_position / DIGIT_BITS;
		if ( wrapper_index >= values.size() )
		{
			throw std::out_of_range( "Index out of range from GetBit( size_t bit_position )" );
		}
		size_t bit_index = bit_position - ( wrapper_index * DIGIT_BITS );
		return values[ wrapper_index ] & ( digit_type( 1 ) << bit_index );
	}

	void BigInteger::SetBit( size_t bit_position )
	{
		size_t wrapper_index = bit_position / DIGIT_BITS;
		if ( wrapper_index >= values.size() )
		{
			throw std::out_of_range( "Index out of range from SetBit( size_t bit_position )" );
		}
		size_t bit_index = bit_position - ( wrapper_index * DIGIT_BITS );
		values[ wrapper_index ] = values[ wrapper_index ] | ( digit_type( 1 ) << bit_index );
	}

	void BigInteger::SetBit( bool value, size_t bit_position )
	{
		size_t wrapper_index = bit_position / DIGIT_BITS;
		if ( wrapper_index >= values.size() )
		{
			throw std::out_of_range( "Index out of range from SetBit( bool value, size_t bit_position )" );
		}
		size_t bit_index = bit_position - ( wrapper_index * DIGIT_BITS );

		if ( value )
		{
			this->values[ wrapper_index ] = values[ wrapper_index ] | ( digit_type( 1 ) << bit_index );
		}
		else
		{
			this->values[ wrapper_index ] = values[ wrapper_index ] & ( ~( digit_type( 1 ) << bit_index ) );
		}
	}

	std::byte BigInteger::GetByte( size_t byte_position ) const
	{
		size_t wrapper_index = byte_position / DIGIT_BYTES;
		if ( wrapper_index >= values.size() )
		{
			return std::byte { 0 };
		}
		size_t	   byte_index = byte_position - ( wrapper_index * DIGIT_BYTES );
		digit_type digit = values[ wrapper_index ];
		return static_cast<std::byte>( digit >> ( byte_index * byte_bits ) );
	}

	void BigInteger::SetByte( size_t byte_position, std::byte byte_in )
	{
		size_t wrapper_index = byte_position / DIGIT_BYTES;
		if ( wrapper_index >= values.size() )
		{
			return;
		}
		size_t	   byte_index = byte_position - ( wrapper_index * DIGIT_BYTES );
		digit_type digit = values[ wrapper_index ];
		digit &= ~( digit_type( ( 1 << byte_bits ) - 1 ) << ( byte_index * byte_bits ) );
		digit |= digit_type( byte_in ) << ( byte_index * byte_bits );
		values[ wrapper_index ] = digit;
	}

	void BigInteger::SqueezeLeadingBits( size_t count )
	{
		if ( count >= BitLength() )
		{
			return;
		}
		size_t blocks = count / DIGIT_BITS;
		size_t rem = count % DIGIT_BITS;
		if ( count == 0 )
		{
			values.resize( blocks );
		}
		else
		{
			values.resize( blocks + 1 );
			digit_type mask = ( digit_type( 1 ) << count ) - 1;
			values[ blocks ] &= mask;
		}
		Clean();
	}

	void BigInteger::FromUnsignedInt( uint64_t value )
	{
		values.clear();

		values = std::vector<digit_type>( 1, 0 );

		this->SetBlock( 0, value );
	}

	uint64_t BigInteger::ToUnsignedInt() const
	{
		if ( IsZero() )
		{
			return 0;
		}
		return values[ 0 ];
	}

	BigInteger BigInteger::operator&( const BigInteger& other ) const
	{
		BigInteger result = this->values.size() < other.values.size() ? ( *this ) : other;
		size_t	   digit_size = std::min( this->values.size(), other.values.size() );
		for ( size_t i = 0; i < digit_size; i++ )
		{
			result.values[ i ] = this->values[ i ] & other.values[ i ];
		}
		result.Clean();
		return result;
	}

	BigInteger& BigInteger::operator&=( const BigInteger& other )
	{
		*this = *this & other;
		return *this;
	}

	BigInteger BigInteger::operator|( const BigInteger& other ) const
	{
		BigInteger result = this->values.size() > other.values.size() ? ( *this ) : other;
		size_t	   digit_size = std::min( this->values.size(), other.values.size() );
		for ( size_t i = 0; i < digit_size; i++ )
		{
			result.values[ i ] = this->values[ i ] | other.values[ i ];
		}
		return result;
	}

	BigInteger& BigInteger::operator|=( const BigInteger& other )
	{
		*this = *this | other;
		return *this;
	}

	BigInteger BigInteger::operator~() const
	{
		BigInteger result = *this;
		for ( size_t i = 0; i < this->values.size(); i++ )
		{
			result.values[ i ] = ~result.values[ i ];
		}
		return result;
	}

	BigInteger BigInteger::operator^( const BigInteger& other ) const
	{
		BigInteger result = this->values.size() > other.values.size() ? ( *this ) : other;
		size_t	   digit_size = std::min( this->values.size(), other.values.size() );
		for ( size_t i = 0; i < digit_size; i++ )
		{
			result.values[ i ] = this->values[ i ] ^ other.values[ i ];
		}
		result.Clean();
		return result;
	}

	BigInteger& BigInteger::operator^=( const BigInteger& other )
	{
		*this = *this ^ other;
		return *this;
	}

	BigInteger BigInteger::LeftShiftBit( size_t shift ) const
	{
		const size_t block_shift = shift / DIGIT_BITS;
		const size_t offset_shift = shift % DIGIT_BITS;
		if ( 0 == offset_shift )
		{
			return LeftShiftBlock( block_shift );
		}
		size_t	   length = values.size();
		BigInteger result;
		result.values.resize( length + block_shift + 1 );

		digit_type	 last = 0, current = 0;
		const size_t high_shift = DIGIT_BITS - offset_shift;
		for ( size_t i = 0; i < length; i++ )
		{
			last = current;
			current = values[ i ];
			result.values[ i + block_shift ] = ( last >> high_shift ) | ( current << offset_shift );
		}
		result.values[ length + block_shift ] = current >> high_shift;
		result.Clean();
		return result;
	}

	BigInteger BigInteger::RightShiftBit( size_t shift ) const
	{
		const size_t offset_shift = shift % DIGIT_BITS;
		const size_t block_shift = shift / DIGIT_BITS;
		if ( 0 == offset_shift )
		{
			return RightShiftBlock( block_shift );
		}
		size_t length = values.size();
		if ( length < block_shift )
		{
			return 0;
		}
		BigInteger result;
		result.values.resize( length - block_shift );
		size_t		 i = length - block_shift;
		digit_type	 last = 0, current = 0;
		const size_t high_shift = DIGIT_BITS - offset_shift;
		while ( i > 0 )
		{
			i--;
			last = current;
			current = values[ i + block_shift ];
			result.values[ i ] = ( last << high_shift ) | ( current >> offset_shift );
		}
		result.Clean();
		return result;
	}

	BigInteger& BigInteger::operator<<=( const uint32_t shift )
	{
		*this = this->LeftShiftBit( shift );

		// Return the modified BigInteger
		return *this;
	}

	BigInteger& BigInteger::operator>>=( const uint32_t shift )
	{
		*this = this->RightShiftBit( shift );

		// Return the modified BigInteger
		return *this;
	}

	// F_{2}
	// 2^{7} < bits < 2^{8}
	// 10110110, shift = 1, reference_bit_capacity = 8
	// 01101101
	// n <<< m
	// [m,n-m]
	// [n-m,m]
	// n<<m
	// n>>n-m

	BigInteger BigInteger::BitRotateLeft( const BigInteger& bits, size_t shift, size_t reference_bit_capacity = 0 )
	{
		// Check if the BigInteger is zero, if so, no rotation is needed
		if ( bits.IsZero() )
		{
			return bits;
		}
		if ( shift >= reference_bit_capacity )
		{
			shift %= reference_bit_capacity;
		}

		BigInteger result;
		size_t	   true_bits = bits.BitLength();
		if ( reference_bit_capacity >= true_bits )
		{
			BigInteger left = bits << shift;
			BigInteger right = bits >> ( reference_bit_capacity - shift );

			result = left | right;
			result.SqueezeLeadingBits( reference_bit_capacity );
			return result;
		}
		else
		{
			BigInteger rotate_part = bits, steady_part = bits >> reference_bit_capacity;
			rotate_part.SqueezeLeadingBits( reference_bit_capacity );
			steady_part <<= reference_bit_capacity;
			BigInteger left = rotate_part << shift;
			BigInteger right = rotate_part >> ( reference_bit_capacity - shift );
			result = left | right;
			result.SqueezeLeadingBits( reference_bit_capacity );
			result |= steady_part;
		}
		return result;
	}

	BigInteger BigInteger::BitRotateRight( const BigInteger& bits, size_t shift, size_t reference_bit_capacity = 0 )
	{
		// Check if the BigInteger is zero, if so, no rotation is needed
		if ( bits.IsZero() )
		{
			return bits;
		}
		if ( shift >= reference_bit_capacity )
		{
			shift %= reference_bit_capacity;
		}

		BigInteger result;
		size_t	   true_bits = bits.BitLength();
		if ( reference_bit_capacity >= true_bits )
		{
			BigInteger left = bits >> shift;
			BigInteger right = bits << ( reference_bit_capacity - shift );

			result = left | right;
			result.SqueezeLeadingBits( reference_bit_capacity );
			return result;
		}
		else
		{
			BigInteger rotate_part = bits, steady_part = bits >> reference_bit_capacity;
			rotate_part.SqueezeLeadingBits( reference_bit_capacity );
			steady_part <<= reference_bit_capacity;
			BigInteger left = rotate_part >> shift;
			BigInteger right = rotate_part << ( reference_bit_capacity - shift );
			result = left | right;
			result.SqueezeLeadingBits( reference_bit_capacity );
			result |= steady_part;
		}
		return result;
	}

	BigInteger& BigInteger::operator=( const BigInteger& other ) noexcept
	{
		if ( this == &other )
			return *this;

		values = other.values;
		return *this;
	}

	BigInteger& BigInteger::operator=( BigInteger&& other ) noexcept
	{
		if ( this == &other )
			return *this;

		values = std::move( other.values );
		other.values.clear();
		return *this;
	}

	BigInteger BigInteger::Abs() const
	{
		return BigInteger( *this );
	}

	std::string BigInteger::ToBinaryString( const uint32_t reference_bit_capacity, bool have_leading_zeros ) const
	{
		if ( reference_bit_capacity == 0 )
			return "";

		if ( have_leading_zeros )
		{
			// Initialize the binary string with leading zeros
			std::string number_string( reference_bit_capacity, '0' );

			// Copy the current BigInteger for modification
			BigInteger result = *this;
			size_t	   bit_size = result.BitLength();

			// Set binary digits based on the BigInteger bits
			for ( size_t i = 0; i < bit_size; i++ )
			{
				if ( result.GetBit( i ) )
				{
					if ( i < reference_bit_capacity )
						number_string[ i ] = '1';
					else
						break;
				}
			}

			// Reverse the string to get correct binary representation
			std::reverse( number_string.begin(), number_string.end() );
			return number_string;
		}
		else
		{
			// Initialize the binary string without leading zeros
			std::string number_string;

			// Copy the current BigInteger for processing
			BigInteger result = *this;
			size_t	   bit_size = result.BitLength();

			// Set binary digits based on the BigInteger bits
			for ( size_t i = 0; i < bit_size; i++ )
			{
				number_string.push_back( result.GetBit( i ) ? '1' : '0' );
			}

			// Reverse the string to get correct binary representation
			std::reverse( number_string.begin(), number_string.end() );
			return number_string;
		}
	}

	std::string BigInteger::ToString( uint32_t base_value ) const
	{
		if ( IsZero() )
		{
			return "0";
		}

		auto numberToChar = []( uint32_t n ) {
			if ( 0 <= n && n <= 9 )
			{
				return char( n + '0' );
			}
			if ( 10 <= n && n <= 35 )
			{
				return char( n - ( 10 - 'A' ) );
			}
			return '\0';
		};

		// Calculate the number of the current bit
		if ( base_value == 2 )
		{
			return ToBinaryString( BitLength() );
		}

		BigInteger	copy = *this;
		std::string number_string;

		while ( !copy.IsZero() )
		{
			// Calculate the current digit, divide the number in the current bit by 2 from a.
			char c = numberToChar( copy.DividModuloNumber( base_value ) );
			number_string += c;
		}

		std::reverse( number_string.begin(), number_string.end() );
		return number_string;
	}

	void BigInteger::FromString( const std::string& number_string )
	{
		FromString( number_string, 10 );
	}

	void BigInteger::FromString( const std::string& number_string, uint32_t base_value )
	{
		if ( base_value == 0 )
		{
			throw std::invalid_argument( "Base must be greater than 0." );
		}

		if ( base_value > 36 )
		{
			throw std::invalid_argument( "Base must be less than or equal to 36." );
		}

		auto charToNumber = []( char c ) {
			if ( '0' <= c && c <= '9' )
			{
				return c - '0';
			}
			if ( 'A' <= c && c <= 'Z' )
			{
				return c - ( 'A' - 10 );
			}
			if ( 'a' <= c && c <= 'z' )
			{
				return c - ( 'a' - 10 );
			}
			return 37;
		};

		size_t count = number_string.length();
		values.clear();
		if ( 0 == count )
		{
			return;
		}
		if ( base_value == 2 )
		{
			values.resize( ( count + 63 ) / 64 );
			for ( size_t i = 0; i < count; i++ )
			{
				SetBit( number_string[ count - i - 1 ] == '1', i );
			}
			return;
		}
		for ( size_t i = 0; i < count; i++ )
		{
			int n = charToNumber( number_string[ i ] );
			if ( n >= base_value || n < 0 )
			{
				throw std::invalid_argument( "Invalid digit for specified base." );
			}
			AddMultiplyNumber( n, base_value );
		}
	}

	size_t BigInteger::SipHash( const BigInteger& Integer, std::vector<uint8_t>* keys ) const
	{
		if ( Integer.values.empty() )
		{
			return 0;
		}

		std::vector<uint8_t> datas( Integer.values.size() * sizeof( digit_type ), 0 );
		::memcpy( &datas, Integer.values.data(), Integer.values.size() * sizeof( digit_type ) );

		uint64_t state = 0x736f6d6570736575UL;
		uint64_t state2 = 0x646f72616e646f6dUL;
		uint64_t key = 0;
		uint64_t key2 = 0;

		if ( keys != nullptr && !( keys->empty() ) )
		{

			if ( keys->size() < sizeof( uint64_t ) )
			{
				::memcpy( &key, keys->data(), keys->size() * sizeof( uint8_t ) );
			}
			else if ( keys->size() >= sizeof( uint64_t ) && keys->size() < sizeof( uint64_t ) * 2 )
			{
				::memcpy( &key, keys->data(), sizeof( uint64_t ) );
				::memcpy( &key2, keys->data() + sizeof( uint64_t ), keys->size() - sizeof( uint64_t ) );
			}
			else if ( keys->size() > sizeof( uint64_t ) * 2 )
			{
				::memcpy( &key, keys->data(), sizeof( uint64_t ) );
				::memcpy( &key2, keys->data() + sizeof( uint64_t ), sizeof( uint64_t ) );
			}

			state ^= key;
			state2 ^= key2;
		}

		uint64_t v0 = state;
		uint64_t v1 = state2;
		uint64_t v2 = 0x6c7967656e657261 ^ key;
		uint64_t v3 = 0x7465646279746573 ^ key2;

		uint64_t		block = 0;
		const uint64_t* start_block = reinterpret_cast<uint64_t*>( datas[ 0 ] );
		const uint64_t* end_block = start_block + ( datas.size() & ~7 );

		//Update blocks with One-way compress function
		uint64_t* block_pointer = ( uint64_t* )start_block;
		while ( block_pointer < end_block )
		{
			block = *block_pointer;

			v3 ^= block;
			for ( size_t round = 0; round < 2; round++ )
			{
				v0 += v1;
				v2 += v3;
				v1 = v1 << 13 | v1 >> 51;
				v3 = v3 << 16 | v3 >> 48;
				v1 ^= v0;
				v3 ^= v2;
				v0 = v0 << 32 | v0 >> 32;
				v2 += v1;
				v0 += v3;
				v1 = v1 << 17 | v1 >> 47;
				v3 = v3 << 21 | v3 >> 43;
				v1 ^= v2;
				v3 ^= v0;
				v2 = v2 << 32 | v2 >> 32;
			}
			v0 ^= block;
		}

		// Update end blocks with One-way compress function
		std::vector<uint8_t> end_block_bytes = std::vector<uint8_t>( reinterpret_cast<uint8_t*>( &end_block ), reinterpret_cast<uint8_t*>( &end_block + 1 ) );
		uint64_t			 last_block = 0;
		switch ( end_block_bytes.size() )
		{
		case 7:
			last_block |= static_cast<uint64_t>( end_block_bytes[ 6 ] ) << 48;
			[[fallthrough]];
		case 6:
			last_block |= static_cast<uint64_t>( end_block_bytes[ 5 ] ) << 40;
			[[fallthrough]];
		case 5:
			last_block |= static_cast<uint64_t>( end_block_bytes[ 4 ] ) << 32;
			[[fallthrough]];
		case 4:
			last_block |= static_cast<uint64_t>( end_block_bytes[ 3 ] ) << 24;
			[[fallthrough]];
		case 3:
			last_block |= static_cast<uint64_t>( end_block_bytes[ 2 ] ) << 16;
			[[fallthrough]];
		case 2:
			last_block |= static_cast<uint64_t>( end_block_bytes[ 1 ] ) << 8;
			[[fallthrough]];
		case 1:
			last_block |= static_cast<uint64_t>( end_block_bytes[ 0 ] );
		}

		v3 ^= last_block;
		for ( size_t round = 0; round < 2; round++ )
		{
			v0 += v1;
			v2 += v3;
			v1 = v1 << 13 | v1 >> 51;
			v3 = v3 << 16 | v3 >> 48;
			v1 ^= v0;
			v3 ^= v2;
			v0 = v0 << 32 | v0 >> 32;
			v2 += v1;
			v0 += v3;
			v1 = v1 << 17 | v1 >> 47;
			v3 = v3 << 21 | v3 >> 43;
			v1 ^= v2;
			v3 ^= v0;
			v2 = v2 << 32 | v2 >> 32;
		}
		v0 ^= last_block;

		// Finalization.
		v2 ^= 0xff;
		for ( size_t round = 0; round < 4; round++ )
		{
			v0 += v1;
			v2 += v3;
			v1 = v1 << 13 | v1 >> 51;
			v3 = v3 << 16 | v3 >> 48;
			v1 ^= v0;
			v3 ^= v2;
			v0 = v0 << 32 | v0 >> 32;
			v2 += v1;
			v0 += v3;
			v1 = v1 << 17 | v1 >> 47;
			v3 = v3 << 21 | v3 >> 43;
			v1 ^= v2;
			v3 ^= v0;
			v2 = v2 << 32 | v2 >> 32;
		}

		return v0 ^ v1 ^ v2 ^ v3;
	}

	BigInteger BigInteger::GCD( BigInteger a, BigInteger b )
	{
		/*
			Stein’s binary & Classic Algorithm for finding GCD
			Hybrid mode
		*/

		if ( a.IsZero() )
		{
			return b;
		}

		if ( b.IsZero() )
		{
			return a;
		}

		size_t trailing_zeroes_a = a.CountTrailingZeros();
		size_t trailing_zeroes_b = b.CountTrailingZeros();
		size_t common_factors_of_2 = std::min( trailing_zeroes_a, trailing_zeroes_b );

		a >>= trailing_zeroes_a;
		b >>= trailing_zeroes_b;

		do
		{
			b >>= b.CountTrailingZeros();

			if ( a > b )
			{
				Swap( b, a );
			}

			b -= a;

		} while ( !b.IsZero() );

		return a << common_factors_of_2;
	}

	BigInteger BigInteger::LCM( const BigInteger& a, const BigInteger& b )
	{
		BigInteger result = a * b;
		result /= GCD( a, b );
		return result;
	}

	BigInteger BigInteger::PollardRho( const BigInteger& n )
	{
		const size_t	 bits = n.BitLength();
		const BigInteger zero( 0 );
		const BigInteger one( 1 );
		const BigInteger two( 2 );

		// Handle special cases
		if ( n == one )
			return n;
		if ( n % two == zero )
			return 2;

		BigInteger counter = 0;
		BigInteger sqrt_n = n;
		sqrt_n.Sqrt();
	RetryAlgorithm:

		/* Initialize variables for the algorithm */

		// We will pick from the range [2, N)
		BigInteger x = ( RandomGenerateNBit( bits ) % ( n - two ) ) + two;
		BigInteger y = x;

		// The constant in f(x).
		// Algorithm can be re-run with a different c if it throws failure for a composite.
		BigInteger c = ( RandomGenerateNBit( bits ) % ( n - one ) ) + one;

		BigInteger d = one, diff;

		/* Perform Pollard's Rho algorithm */
		while ( d == one )
		{
			// Tortoise Move: x(i+1) = f(x(i))
			x = ( x.PowerWithModulo( two, n ) + c + n ) % n;

			// Hare Move: y(i+1) = f(f(y(i)))
			y = ( y.PowerWithModulo( two, n ) + c + n ) % n;
			y = ( y.PowerWithModulo( two, n ) + c + n ) % n;

			// Check gcd of |x-y| and n
			diff = x;
			d = GCD( diff.Subtract( y ).Abs(), n );

			// Check if a non-trivial factor is found
			if ( d == n )
			{
				if ( counter == sqrt_n )
				{
					break;
				}
				counter = counter + 1;
				//Retry if the algorithm fails to find prime factor with chosen x and c
				goto RetryAlgorithm;
			}
			else
				return d;
		}

		return n;
	}

	BigInteger BigInteger::BigInteger::BabyStepGiantStep( const BigInteger& base, const BigInteger& result, const BigInteger& modulo )
	{
		BigInteger bounds = modulo;
		bounds = bounds.Sqrt() + 1;

		// Calculate a ^ n
		BigInteger an = 1;
		for ( BigInteger i = 0; i < bounds; i = i + 1 )
			an.MultiplyWithModulo( base, modulo );

		// Baby steps: Save all values of a^(n*i) of LHS
		BigInteger												 value = an;
		std::unordered_map<BigInteger, BigInteger, HashFunction> BabySteps;
		for ( BigInteger i = 1; i <= bounds; i = i + 1 )
		{
			if ( !BabySteps[ value ] )
				BabySteps[ value ] = i;
			value.MultiplyWithModulo( an, modulo );
		}

		value = result;
		// Giant steps: load baby steps
		for ( BigInteger i = 0; i <= bounds; i = i + 1 )
		{
			// Calculate (a ^ j) * b and check for collision
			if ( BabySteps[ value ] )
			{
				BigInteger answer = BabySteps[ value ] * bounds - i;
				if ( answer < modulo )
					return answer;
			}
			value.MultiplyWithModulo( base, modulo );
		}

		// By a^{x} ≡ b (mod p), is not exist x
		return BigInteger( 0 );
	}

	BigInteger BigInteger::RandomGenerateNBit( size_t n )
	{
		static std::mt19937_64					rng( std::random_device {}() );
		std::uniform_int_distribution<uint64_t> dist( 0, std::numeric_limits<uint64_t>::max() );
		BigInteger								ud_result = BigInteger( 0 );
		size_t									digit_type_count = ( n + DIGIT_BITS - 1 ) / DIGIT_BITS;
		ud_result.values = std::vector<digit_type>( digit_type_count, 0 );
		for ( size_t i = 0; i < digit_type_count; ++i )
		{
			ud_result.values[ i ] = dist( rng );
		}
		ud_result.SetBit( n - 1 );
		for ( size_t i = n; i < digit_type_count * DIGIT_BITS; i++ )
		{
			ud_result.SetBit( false, i );
		}
		return ud_result;
	}

	BigInteger BigInteger::Factorial(size_t n)
	{
		auto tab = HyperInt::Factorial::get_prime_factors(n);
		return HyperInt::Factorial::calculate_factorial(tab);
	}

	size_t BigInteger::Log2() const
	{
		if ( IsZero() )
		{
			throw std::invalid_argument("Logarithm base 2 of 0 is undefined");
		}
		return BitLength() - 1;
	}

	/*
	
		BigSignedInteger
	
	*/

	BigSignedInteger::BigSignedInteger() {}

	BigSignedInteger::BigSignedInteger( int64_t n ) : sign( n < 0 ), uint_data( std::abs( n ) ) {}

	BigSignedInteger::BigSignedInteger( const BigSignedInteger& other ) noexcept : sign( other.sign ), uint_data( other.uint_data ) {}

	BigSignedInteger::BigSignedInteger( BigSignedInteger&& other ) noexcept : sign( std::move( other.sign ) ), uint_data( std::move( other.uint_data ) ) {}

	BigSignedInteger::BigSignedInteger( const BigInteger& other, bool new_sign ) : sign( new_sign ), uint_data( other ) {}

	BigSignedInteger::BigSignedInteger( BigInteger&& other, bool new_sign ) : sign( new_sign ), uint_data( std::move( other ) ) {}

	BigSignedInteger::BigSignedInteger( const std::string& number_string )
	{
		if ( !number_string.empty() && number_string[ 0 ] == '-' )
		{
			sign = true;
			std::string str = std::string( number_string.begin() + 1, number_string.end() );
			uint_data.FromString( str, 10 );
		}
		else
		{
			uint_data.FromString( number_string, 10 );
		}
	}

	BigSignedInteger::BigSignedInteger( const std::string& number_string, uint32_t base )
	{
		if ( !number_string.empty() && number_string[ 0 ] == '-' )
		{
			sign = true;
			std::string str = std::string( number_string.begin() + 1, number_string.end() );
			uint_data.FromString( str, base );
		}
		else
		{
			uint_data.FromString( number_string, base );
		}
	}

	BigSignedInteger::operator BigInteger() const
	{
		return uint_data;
	}

	BigSignedInteger BigSignedInteger::Abs() const
	{
		if ( this->sign == true )
		{
			BigSignedInteger abs = *this;
			abs.sign = false;
			return abs;
		}

		return *this;
	}

	bool BigSignedInteger::IsZero() const
	{
		return !this->sign && this->uint_data.IsZero();
	}

	bool BigSignedInteger::IsNegative() const
	{
		return this->sign;
	}

	size_t BigSignedInteger::Size() const
	{
		return this->uint_data.Size();
	}

	size_t BigSignedInteger::BitLength() const
	{
		return this->uint_data.BitLength();
	}

	void BigSignedInteger::Print( uint32_t base_value ) const
	{
		if ( sign )
		{
			std::cout << "-\t";
		}
		else
		{
			std::cout << "+\t";
		}
		uint_data.Print( base_value );
	}

	void BigSignedInteger::PrintBinary( bool have_space_with_block ) const
	{
		if ( sign )
		{
			std::cout << "-\t";
		}
		else
		{
			std::cout << "+\t";
		}
		uint_data.PrintBinary( have_space_with_block );
	}

	std::string BigSignedInteger::ToBinaryString( const uint32_t reference_bit_capacity, bool have_leading_zeros ) const
	{
		std::string result;
		if ( this->IsNegative() )
		{
			result += '-';
		}
		result += uint_data.ToBinaryString( reference_bit_capacity, have_leading_zeros );
		return result;
	}

	std::string BigSignedInteger::ToString( uint32_t base_value ) const
	{
		std::string result;
		if ( this->IsNegative() )
		{
			result += '-';
		}
		result += uint_data.ToString( base_value );
		return result;
	}

	void BigSignedInteger::FromString( const std::string& number_string, bool new_sign )
	{
		uint_data.FromString( number_string );
		this->sign = new_sign;
	}

	void BigSignedInteger::FromString( const std::string& number_string, uint32_t base_value, bool new_sign )
	{
		uint_data.FromString( number_string, base_value );
		this->sign = new_sign;
	}

	void BigSignedInteger::FromUnsignedInt( uint64_t value )
	{
		uint_data.FromUnsignedInt( value );
		this->sign = false;
	}

	uint64_t BigSignedInteger::ToUnsignedInt() const
	{
		return uint_data.GetBlock( 0 );
	}

	void BigSignedInteger::FromSignedInt( int64_t value )
	{
		uint_data.FromUnsignedInt( static_cast<uint64_t>( std::abs( value ) ) );
		sign = ( value < 0 );
	}

	int64_t BigSignedInteger::ToSignedInt( bool is_negative ) const
	{
		uint64_t value = uint_data.GetBlock( 0 );

		if ( value > static_cast<uint64_t>( std::numeric_limits<int64_t>::max() ) )
		{
			value %= static_cast<uint64_t>( std::numeric_limits<int64_t>::max() ) + 1;
		}

		return is_negative ? -static_cast<int64_t>( value ) : static_cast<int64_t>( value );
	}

	BigSignedInteger BigSignedInteger::operator-() const
	{
		if ( this->sign == false )
		{
			BigSignedInteger copy( *this );

			copy.sign = true;

			return copy;
		}

		return *this;
	}

	BigSignedInteger BigSignedInteger::operator+() const
	{
		if ( this->sign == true )
		{
			BigSignedInteger copy( *this );

			copy.sign = false;

			return copy;
		}

		return *this;
	}

	BigSignedInteger operator+( const BigSignedInteger& lhs, const BigSignedInteger& rhs )
	{
		BigSignedInteger result;
		if ( lhs.IsNegative() == rhs.IsNegative() )
		{
			result.sign = lhs.IsNegative();
			result.uint_data = lhs.uint_data + rhs.uint_data;
		}
		else
		{
			if ( lhs.IsNegative() )
			{
				result.sign = lhs.uint_data > rhs.uint_data;
			}
			else
			{
				result.sign = lhs.uint_data < rhs.uint_data;
			}
			result.uint_data = lhs.uint_data.Difference( rhs.uint_data );
		}
		if ( result.uint_data.IsZero() )
		{
			result.sign = false;
		}
		return result;
	}

	BigSignedInteger operator-( const BigSignedInteger& lhs, const BigSignedInteger& rhs )
	{
		BigSignedInteger result;
		if ( lhs.IsNegative() != rhs.IsNegative() )
		{
			result.sign = lhs.IsNegative();
			result.uint_data = lhs.uint_data + rhs.uint_data;
		}
		else
		{
			if ( lhs.IsNegative() )
			{
				result.sign = lhs.uint_data > rhs.uint_data;
			}
			else
			{
				result.sign = lhs.uint_data < rhs.uint_data;
			}
			result.uint_data = lhs.uint_data.Difference( rhs.uint_data );
		}
		if ( result.uint_data.IsZero() )
		{
			result.sign = false;
		}
		return result;
	}

	BigSignedInteger operator*( const BigSignedInteger& lhs, const BigSignedInteger& rhs )
	{
		BigSignedInteger result;
		result.uint_data = lhs.uint_data * rhs.uint_data;
		if ( result.uint_data.IsZero() )
		{
			result.sign = false;
		}
		else
		{

			result.sign = lhs.IsNegative() != rhs.IsNegative();
		}
		return result;
	}

	BigSignedInteger operator/( const BigSignedInteger& lhs, const BigSignedInteger& rhs )
	{
		BigSignedInteger quotient, remainder;
		quotient = lhs.uint_data.DivideModulo( rhs.uint_data, remainder.uint_data );
		if ( lhs.sign == rhs.sign )
		{
			quotient.sign = false;
		}
		else
		{
			if ( !remainder.uint_data.IsZero() )
			{
				++quotient.uint_data;
			}
			if ( !quotient.uint_data.IsZero() )
			{
				quotient.sign = true;
			}
		}
		return quotient;
	}

	BigSignedInteger operator%( const BigSignedInteger& lhs, const BigSignedInteger& rhs )
	{
		BigSignedInteger quotient, remainder;
		quotient = lhs.uint_data.DivideModulo( rhs.uint_data, remainder.uint_data );
		if ( !remainder.uint_data.IsZero() )
		{
			remainder.sign = rhs.sign;
			if ( lhs.sign != rhs.sign )
			{
				remainder.uint_data = rhs.uint_data - remainder.uint_data;
			}
		}
		return remainder;
	}

	BigSignedInteger& BigSignedInteger::operator=( const BigSignedInteger& other ) noexcept
	{
		this->sign = other.sign;
		this->uint_data = other.uint_data;
		return *this;
	}

	BigSignedInteger& BigSignedInteger::operator=( BigSignedInteger&& other ) noexcept
	{
		this->sign = other.sign;
		this->uint_data = std::move( other.uint_data );
		return *this;
	}

	BigSignedInteger& BigSignedInteger::operator+=( const BigSignedInteger& other )
	{
		*this = *this + other;
		return *this;
	}

	BigSignedInteger& BigSignedInteger::operator-=( const BigSignedInteger& other )
	{
		*this = *this - other;
		return *this;
	}

	BigSignedInteger& BigSignedInteger::operator*=( const BigSignedInteger& other )
	{
		*this = *this * other;
		return *this;
	}

	BigSignedInteger& BigSignedInteger::operator/=( const BigSignedInteger& other )
	{
		*this = *this / other;
		return *this;
	}

	BigSignedInteger& BigSignedInteger::operator%=( const BigSignedInteger& other )
	{
		*this = *this % other;
		return *this;
	}

	BigSignedInteger BigSignedInteger::operator&( const BigSignedInteger& other ) const
	{
		return BigSignedInteger( this->uint_data & other.uint_data );
	}

	BigSignedInteger& BigSignedInteger::operator&=( const BigSignedInteger& other )
	{
		return *this = *this & other;
	}

	BigSignedInteger BigSignedInteger::operator|( const BigSignedInteger& other ) const
	{
		return BigSignedInteger( this->uint_data | other.uint_data );
	}

	BigSignedInteger& BigSignedInteger::operator|=( const BigSignedInteger& other )
	{
		return *this = *this | other;
	}

	BigSignedInteger BigSignedInteger::operator~() const
	{
		return ~uint_data;
	}

	BigSignedInteger BigSignedInteger::operator^( const BigSignedInteger& other ) const
	{
		return BigSignedInteger( this->uint_data ^ other.uint_data );
	}

	BigSignedInteger& BigSignedInteger::operator^=( const BigSignedInteger& other )
	{
		return *this = *this ^ other;
	}

	BigSignedInteger& BigSignedInteger::operator<<=( size_t shift )
	{
		uint_data = uint_data.LeftShiftBit( shift );
		if ( uint_data.IsZero() )
		{
			sign = false;
		}
		return *this;
	}

	BigSignedInteger& BigSignedInteger::operator>>=( size_t shift )
	{
		uint_data = uint_data.RightShiftBit( shift );
		if ( uint_data.IsZero() )
		{
			sign = false;
		}
		return *this;
	}

	bool operator==( const BigSignedInteger& lhs, const BigSignedInteger& rhs )
	{
		return lhs.sign == rhs.sign && lhs.uint_data == rhs.uint_data;
	}

	bool operator!=( const BigSignedInteger& lhs, const BigSignedInteger& rhs )
	{
		return !( lhs == rhs );
	}

	bool operator>( const BigSignedInteger& lhs, const BigSignedInteger& rhs )
	{
		if ( lhs.IsNegative() != rhs.IsNegative() )
		{
			return rhs.IsNegative();
		}
		return lhs.IsNegative() != ( lhs.uint_data > rhs.uint_data );
	}

	bool operator>=( const BigSignedInteger& lhs, const BigSignedInteger& rhs )
	{
		return !( lhs < rhs );
	}

	bool operator<( const BigSignedInteger& lhs, const BigSignedInteger& rhs )
	{
		return rhs > lhs;
	}

	bool operator<=( const BigSignedInteger& lhs, const BigSignedInteger& rhs )
	{
		return !( lhs > rhs );
	}

	void BigSignedInteger::EGCD( BigSignedInteger a, BigSignedInteger b, BigSignedInteger& gcd, BigSignedInteger& co1, BigSignedInteger& co2 )
	{
		if ( a < b )
		{
			BigSignedInteger::EGCD( b, a, gcd, co2, co1 );
			return;
		}
		BigSignedInteger x( 1 ), x0( 0 );
		BigSignedInteger y( 0 ), y0( 1 );

		auto exec_func = []( BigSignedInteger& lhs, BigSignedInteger& rhs, const BigSignedInteger& k ) {
			BigSignedInteger temp = std::move( rhs );
			rhs = lhs - k * temp;
			lhs = std::move( temp );
		};
		while ( b > 0 )
		{
			BigSignedInteger k = a / b;
			exec_func( a, b, k );
			exec_func( y, y0, k );
			exec_func( x, x0, k );
		}
		gcd = std::move( a );
		co1 = std::move( x );
		co2 = std::move( y );
	}

	BigSignedInteger BigSignedInteger::ModuloArithmetic( ArithmeticMode mode, const BigSignedInteger& a, const BigSignedInteger& b, const BigSignedInteger& modulus )
	{
		if ( modulus.IsZero() )
		{
			throw std::invalid_argument( "The modulus cannot be zero!" );
		}

		BigSignedInteger temporary;

		switch ( mode )
		{
		case ArithmeticMode::Addition:
			temporary = a + b;
			break;
		case ArithmeticMode::Subtraction:
			temporary = a - b;
			break;
		case ArithmeticMode::Multiplication:
			temporary = a * b;
			break;
		case ArithmeticMode::Division:
			if ( !b.IsZero() )
			{
				BigSignedInteger compute_inverse = BigSignedInteger::ModuloInverse( b, modulus );
				if ( !compute_inverse.IsZero() )
				{
					temporary = a * compute_inverse;
				}
				else
				{
					throw std::invalid_argument( "The b is not have multiplication inverse with modulo the modulus number, This mean (a * b) mod modulus_number != (a * inverse(b)) mod modulus_number" );
				}
			}
			else
			{
				throw std::invalid_argument( "Division by zero!" );
			}
			break;
		default:
			throw std::invalid_argument( "Invalid modulo arithmetic mode!" );
			break;
		}

		auto Modulo = []( const BigSignedInteger& number, const BigSignedInteger& modulus ) {
			if ( number.IsZero() )
			{
				return BigSignedInteger( 0 );
			}

			BigSignedInteger result;

			/*if ( modulus.IsPowerOfTwo() )
			{
				result = number & ( modulus - 1 );
				result.Clean();
				return result;
			}*/

			result = number % modulus;
			return result;
		};

		return Modulo( temporary, modulus );
	}

	BigSignedInteger BigSignedInteger::ModuloInverse( const BigSignedInteger& a, const BigSignedInteger& b )
	{
		BigSignedInteger gcd, x, y;

		EGCD( a, b, gcd, x, y );
		return ( x % b + b ) % b;
	}

	size_t BigSignedInteger::SipHash( const BigSignedInteger& Integer, std::vector<uint8_t>* keys ) const
	{
		const BigInteger& UnInteger = Integer.uint_data;
		return UnInteger.SipHash( UnInteger, keys );
	}

	/*
	
	Montgomery

	*/

	BigInteger Montgomery::GetR() const
	{
		return BigInteger::BasePowerN( modulo.Size() + 1 );
	}

	BigInteger Montgomery::ModuloR( const BigInteger& a ) const
	{
		return a.ModuloBasePower( radix_blocks );
	}

	BigInteger Montgomery::DivisionR( const BigInteger& a ) const
	{
		return a.RightShiftBlock( radix_blocks );
	}

	BigInteger Montgomery::ReduceLazy( const BigInteger& a ) const
	{
		BigInteger m = ModuloR( a ) * modulo_inverse_neg;
		m = ModuloR( m );
		m = DivisionR( a + m * modulo );
		return m;
	}

	BigInteger Montgomery::Reduce( const BigInteger& a ) const
	{
		BigInteger m = ReduceLazy( a );
		if ( m >= modulo )
		{
			m -= modulo;
		}
		return m;
	}

	Montgomery::Montgomery( const BigInteger& new_modulo ) : modulo( new_modulo ), modulo2( new_modulo * 2 )
	{
		radix = GetR();
		radix_blocks = radix.Size() - 1;
		modulo_inverse_neg = BigSignedInteger::ModuloInverse( modulo, radix );
		modulo_inverse_neg = radix - modulo_inverse_neg;
		radix %= modulo;
		radix_square = radix * radix % modulo;
	}

	BigInteger Montgomery::ToInt( const BigInteger& a ) const
	{
		return Reduce( a );
	}

	BigInteger Montgomery::ToMontgomery( const BigInteger& a ) const
	{
		return Reduce( a * radix_square );
	}

	BigInteger Montgomery::Addtion( const BigInteger& a, const BigInteger& b ) const
	{
		BigInteger sum = a + b;
		if ( sum >= modulo2 )
		{
			sum -= modulo2;
		}
		return sum;
	}

	BigSignedInteger Montgomery::Addtion( const BigSignedInteger& a, const BigSignedInteger& b ) const
	{
		return Addtion( BigInteger( a ), BigInteger( b ) );
	}

	BigInteger Montgomery::Substraction( const BigInteger& a, const BigInteger& b ) const
	{
		if ( a >= b )
		{
			return a - b;
		}
		return modulo2 - b + a;
	}

	BigSignedInteger Montgomery::Substraction( const BigSignedInteger& a, const BigSignedInteger& b ) const
	{
		return Substraction( BigInteger( a ), BigInteger( b ) );
	}

	BigInteger Montgomery::Multiplication( const BigInteger& a, const BigInteger& b ) const
	{
		return ReduceLazy( a * b );
	}

	BigSignedInteger Montgomery::Multiplication( const BigSignedInteger& a, const BigSignedInteger& b ) const
	{
		return ReduceLazy( a * b );
	}

	BigInteger Montgomery::Inverse( const BigInteger& a ) const
	{
		return Power( a, modulo - 2 );
	}

	BigSignedInteger Montgomery::Inverse( const BigSignedInteger& a ) const
	{
		return BigSignedInteger( Inverse( BigInteger( a ) ), a.IsNegative() );
	}

	BigInteger Montgomery::Power( BigInteger base, const BigInteger& index ) const
	{
		base = ToMontgomery( base );
		size_t			 index_length = index.Size();
		BigInteger		 result = radix;
		constexpr size_t FOUR_ARY_THRESHOLD = 4;
		if ( index_length <= FOUR_ARY_THRESHOLD )
		{
			size_t bits = index.BitLength();
			for ( size_t i = 0; i < bits; i++ )
			{
				if ( index.GetBit( i ) )
				{
					result = Multiplication( result, base );
				}
				base = Multiplication( base, base );
			}
		}
		else
		{
			constexpr int					   INDEX_BLOCK = 4;
			constexpr size_t				   TABLE_SIZE = size_t( 1 ) << INDEX_BLOCK;
			std::array<BigInteger, TABLE_SIZE> table { radix, base };
			for ( size_t i = 2; i < table.size(); i++ )
			{
				table[ i ] = Multiplication( table[ i - 1 ], base );
			}

			size_t i = index_length;
			while ( i > 0 )
			{
				i--;
				const auto block = index.GetBlock( i );
				size_t	   j = sizeof( block ) * CHAR_BIT;
				while ( j > 0 )
				{
					j -= INDEX_BLOCK;
					const auto sub_index = ( block >> j ) % TABLE_SIZE;
					for ( size_t k = 0; k < INDEX_BLOCK; k++ )
					{
						result = Multiplication( result, result );
					}
					if ( sub_index > 0 )
					{
						result = Multiplication( result, table[ sub_index ] );
					}
				}
			}
		}
		return ToInt( result );
	}

	BigSignedInteger Montgomery::Power( BigSignedInteger base, const BigInteger& index ) const
	{
		return Power( BigInteger( base ), BigInteger( index ) );
	}

}  // namespace TwilightDream::BigInteger