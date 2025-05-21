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

#ifndef TWILIGHT_DREAM_BIG_INTEGER_HPP
#define TWILIGHT_DREAM_BIG_INTEGER_HPP

#include <cmath>
#include <cstdint>
#include <climits>
#include <cstring>

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <complex>
#include <optional>
#include <functional>
#include <type_traits>
#include <random>
#include <bitset>

namespace TwilightDream
{
	struct PrimeNumberTester;
}

namespace TwilightDream::BigInteger
{
	using digit_type = uint64_t;
	constexpr uint32_t DIGIT_BITS = std::numeric_limits<digit_type>::digits;
	constexpr uint32_t DIGIT_BYTES = DIGIT_BITS / CHAR_BIT;

	/**
	 * @brief Check if the current platform is Little Endian.
	 * 
	 * @return true if the platform is Little Endian, false otherwise.
	 */
	inline bool is_little_endian()
	{
		const uint64_t value = 1;
		return *reinterpret_cast<const char*>( &value ) == 1;
	}

	/**
	 * @brief Universal byte order conversion, implemented based on bitwise operations, applicable to all integer types (cross-platform).
	 * 
	 * @tparam T The type of the value to perform byte order conversion.
	 * @param value The value to be byte order converted.
	 * @return The byte order converted value.
	 */
	template <typename T, typename std::enable_if<std::is_integral_v<T>>::type* = nullptr>
	constexpr T endian_swap( T value )
	{
		static_assert( !std::is_same<T, bool>::value, "bool is not supported" );
		// For integer types of 8 bits or less, no byte order conversion is needed
		if ( sizeof( T ) == 1 )
			return value;

		if constexpr ( sizeof( T ) == 2 )  // 16-bit integers
		{
			return ( ( ( value & 0x00FFU ) << 8 ) | ( ( value & 0xFF00U ) >> 8 ) );
		}
		else if constexpr ( sizeof( T ) == 4 )	// 32-bit integers
		{
			constexpr uint32_t bit_mask = 0x00FF00FF;
			value = ( ( value & bit_mask ) << 8 ) | ( ( value >> 8 ) & bit_mask );
			return ( value << 16 ) | ( value >> 16 );
		}
		else if constexpr ( sizeof( T ) == 8 )	// 64-bit integers
		{
			constexpr uint64_t bit_mask0 = 0x00FF00FF00FF00FF;
			constexpr uint64_t bit_mask1 = 0x0000FFFF0000FFFF;
			value = ( ( value & bit_mask0 ) << 8 ) | ( ( value >> 8 ) & bit_mask0 );
			value = ( ( value & bit_mask1 ) << 16 ) | ( ( value >> 16 ) & bit_mask1 );
			return ( value << 32 ) | ( value >> 32 );
		}
		// Add corresponding bitwise operation logic for other sizes of integer types as needed
	}

	class BigInteger
	{
	private:
		friend struct TwilightDream::PrimeNumberTester;

		void Clean();

		std::vector<digit_type> values;

		static constexpr uint32_t byte_bits = std::numeric_limits<unsigned char>::digits;

	public:

		BigInteger();

		template <size_t N>
		BigInteger( const std::bitset<N>& bits )
		{
			size_t digitCount = ( N + DIGIT_BITS - 1 ) / DIGIT_BITS;

			values.resize( digitCount, 0 );

			for ( size_t i = 0; i < N; ++i )
			{
				if ( i >= DIGIT_BITS * values.size() )
				{
					break;
				}

				if ( bits.test( i ) )
				{
					SetBit( i );
				}
			}
		}

		BigInteger( uint64_t value );

		explicit BigInteger( const std::string& number_string );

		BigInteger( const std::string& number_string, uint32_t base );

		BigInteger( const BigInteger& other ) noexcept;

		BigInteger( BigInteger&& other ) noexcept;

		void Print( uint32_t base_value ) const;

		void PrintBinary(bool have_space_with_block = true) const;

		BigInteger& operator=( const BigInteger& other ) noexcept;

		BigInteger& operator=( BigInteger&& other ) noexcept;

		BigInteger& AddMultiplyNumber( uint64_t add_num, uint64_t multiply_num );

		uint64_t DividModuloNumber( uint64_t divisor );

		BigInteger& operator+=( const BigInteger& other );

		/**
		 * @brief Adds another BigInteger to the current instance.
		 *
		 * This function performs addition of two BigIntegers and updates the current
		 * instance with the result.
		 *
		 * @param other The BigInteger to be added.
		 * @return Reference to the modified current instance after addition.
		 */
		BigInteger& Add( const BigInteger& other );

		BigInteger& operator-=( const BigInteger& other );

		BigInteger Difference( const BigInteger& other ) const;

		/**
		 * @brief Subtracts another BigInteger from the current instance.
		 *
		 * This function performs subtraction of two BigIntegers and updates the current
		 * instance with the result.
		 *
		 * @param other The BigInteger to be subtracted.
		 * @return Reference to the modified current instance after subtraction.
		 */
		BigInteger& Subtract( const BigInteger& other );

		BigInteger operator++();

		BigInteger operator--();

		BigInteger operator++( int );

		BigInteger operator--( int );

		BigInteger RightShiftBlock( size_t d ) const;

		BigInteger LeftShiftBlock( size_t d ) const;

		/**
		 * Computes the integer square root of the current BigInteger instance.
		 * 
		 * @return A reference to the current object, with its value updated to the integer square root of the original value.
		 * 
		 * @warning This function modifies the current BigInteger instance and returns a reference to itself. Any subsequent modifications to this reference will affect the original object. The computed square root is the largest integer less than or equal to the true square root.
		 */
		BigInteger& Sqrt();

		/**
		 * Computes the integer cube root of the current BigInteger instance.
		 * 
		 * @return A reference to the current object, with its value updated to the integer cube root of the original value.
		 * 
		 * @warning This function modifies the current BigInteger instance and returns a reference to itself. Any subsequent modifications to this reference will affect the original object. The computed square root is the largest integer less than or equal to the true square root.
		 */
		BigInteger& Cbrt();

		/**
		 * Computes the modular exponentiation of the current BigInteger instance.
		 * this = this^{exponent} mod modulo
		 * 
		 * @param exponent The exponent value as a BigInteger.
		 * @param modulo The modulus value as a BigInteger.
		 * @return A reference to the current object, with its value updated to the result of the modular exponentiation.
		 * 
		 * @warning This function modifies the current BigInteger instance and returns a reference to itself. Any subsequent modifications to this reference will affect the original object. Also, if the modulo is 1, the function sets the object to 0 and returns immediately.
		 */
		BigInteger& PowerWithModulo( const BigInteger& exponent, const BigInteger& modulo );

		/**
		 * Performs modular multiplication of the current BigInteger instance with another BigInteger.
		 * this = this * other mod modulo
		 * 
		 * @param other The BigInteger to multiply with.
		 * @param modulo The modulus value as a BigInteger.
		 * @return A reference to the current object, with its value updated to the result of the modular multiplication.
		 * 
		 * @warning This function modifies the current BigInteger instance and returns a reference to itself. Any subsequent modifications to this reference will affect the original object. If the modulo is 1, the function sets the object to 0 and returns immediately. This function uses a binary exponentiation algorithm, which is efficient for large numbers.
		 */
		BigInteger& MultiplyWithModulo( const BigInteger& other, const BigInteger modulo );

		BigInteger ReciprocalNewtonIteration( size_t bit, size_t offset = 0 ) const;

		/**
		* Performs division using Newton's iteration method for BigIntegers.
		*
		* This function implements a division algorithm based on Newton's iteration method, which is an
		* efficient way to calculate the division of two large integers. The method works by
		* approximating the reciprocal of the divisor and iteratively refining the quotient until
		* the remainder is less than the divisor.
		*
		* @param divisor The BigInteger representing the divisor.
		* @return A pair containing the quotient and the remainder after division.
		*/
		BigInteger DivideModuloNewtonIteration( const BigInteger& divisor, BigInteger& remainder ) const;

		BigInteger DivideModuloDivideConquer(const BigInteger& divisor, BigInteger& remainder) const;

		BigInteger DivideModulo( const BigInteger& divisor, BigInteger& remainder ) const;

		BigInteger& operator*=( const BigInteger& other );

		BigInteger& operator/=( const BigInteger& other );

		BigInteger& operator%=( const BigInteger& other );

		bool IsEven() const;

		bool IsPowerOfTwo() const;

		bool IsZero() const;

		bool IsNegative() const;

		size_t Size() const;

		size_t CountLeadingZeros() const;

		size_t CountTrailingZeros() const;

		size_t BitLength() const;

		// Block reference operator
		digit_type& operator[](size_t index)
		{
			// Ensure the index is within bounds
			if (index >= values.size())
			{
				throw std::out_of_range("Index out of range");
			}
			return values[index];
		}

		const digit_type& operator[](size_t index) const
		{
			// Ensure the index is within bounds
			if (index >= values.size())
			{
				throw std::out_of_range("Index out of range");
			}
			return values[index];
		}

		void SetBlock( size_t block_position, uint64_t value );

		digit_type GetBlock( size_t block_position ) const;

		bool GetBit( size_t bit_position ) const;

		void SetBit( size_t bit_position );

		void SetBit( bool value, size_t bit_position );

		std::byte GetByte( size_t byte_position ) const;

		void SetByte( size_t byte_position, std::byte byte_in );

		// Remove 'count' number of bits from the most significant end of the integer.
		void SqueezeLeadingBits( size_t count );

		void FromUnsignedInt(uint64_t value);

		uint64_t ToUnsignedInt() const;

		BigInteger operator&( const BigInteger& other ) const;

		BigInteger& operator&=( const BigInteger& other );

		BigInteger operator|( const BigInteger& other ) const;

		BigInteger& operator|=( const BigInteger& other );

		BigInteger operator~() const;

		BigInteger operator^( const BigInteger& other ) const;

		BigInteger& operator^=( const BigInteger& other );

		/**
		 * Computes the power of the current BigInteger instance.
		 * result = result.Power(exponent)
		 * 
		 * @param exponent The exponent value, which must be a non-negative integer.
		 * @return A reference to the current object, with its value updated to the result of the computation.
		 * 
		 * @warning Be aware that this function returns a reference to the current object, not a copy. Any modifications to the returned reference will affect the original BigInteger object.
		 */
		BigInteger& Power( const size_t exponent );

		/**
		 * Computes the power of the current BigInteger instance, where the exponent is also a BigInteger object.
		 * result = result.BigPower(exponent)
		 * 
		 * @param exponent The exponent value, which must be a positive integer.
		 * @return A reference to the current object, with its value updated to the result of the computation.
		 * 
		 * @warning Be aware that this function returns a reference to the current object, not a copy. Any modifications to the returned reference will affect the original BigInteger object.
		 */
		BigInteger& BigPower( const BigInteger& exponent );

		BigInteger ModuloBasePower( size_t n ) const;

		static BigInteger BasePowerN( size_t n );

		static BigInteger TwoPowerN( size_t );

		BigInteger LeftShiftBit( size_t shift ) const;

		BigInteger RightShiftBit( size_t shift ) const;

		BigInteger& operator<<=( const uint32_t shift );

		BigInteger& operator>>=( const uint32_t shift );

		friend BigInteger operator<<( const BigInteger& lhs, const uint32_t shift )
		{
			BigInteger result( lhs );
			result = result.LeftShiftBit(shift);
			return result;
		}

		friend BigInteger operator>>( const BigInteger& lhs, const uint32_t shift )
		{
			BigInteger result( lhs );
			result = result.RightShiftBit(shift);
			return result;
		}

		/**
		 * @brief Rotates the bits of a BigInteger to the left by a specified number of positions.
		 *
		 * @param bits The BigInteger to be rotated.
		 * @param shift The number of positions to rotate the bits to the left.
		 * @param reference_bit_capacity The bit capacity to reference for the rotation (default is 0).
		 * @return A new BigInteger that represents the result of the left bit rotation.
		 *
		 * This method performs a left bitwise rotation on the given BigInteger. The rotation
		 * effectively shifts the bits to the left by the specified number of positions, with the bits
		 * that overflow on the left being placed back on the right.
		 * 
		 * The rotation is performed with respect to a reference bit capacity:
		 * - If the reference bit capacity is greater than or equal to the actual bit length of the input,
		 *   the rotation is straightforward, and the leading bits are squeezed to fit the reference capacity.
		 * - If the reference bit capacity is less than the actual bit length, the rotation is split into two
		 *   parts: the part to be rotated and the steady part that remains unchanged. The rotated part is
		 *   processed and combined with the steady part to form the final result.
		 *
		 * @example
		 * BigInteger num("1001", 2); // Binary representation: 1001
		 * BigInteger result = BigInteger::BitRotateLeft(num, 2, 4); // Result: 0110 (binary)
		 */
		static BigInteger BitRotateLeft( const BigInteger& bits, size_t shift, size_t reference_bit_capacity );

		/**
		 * @brief Rotates the bits of a BigInteger to the right by a specified number of positions.
		 *
		 * @param bits The BigInteger to be rotated.
		 * @param shift The number of positions to rotate the bits to the right.
		 * @param reference_bit_capacity The bit capacity to reference for the rotation (default is 0).
		 * @return A new BigInteger that represents the result of the right bit rotation.
		 *
		 * This method performs a right bitwise rotation on the given BigInteger. The rotation
		 * effectively shifts the bits to the right by the specified number of positions, with the bits
		 * that overflow on the right being placed back on the left.
		 * 
		 * The rotation is performed with respect to a reference bit capacity:
		 * - If the reference bit capacity is greater than or equal to the actual bit length of the input,
		 *   the rotation is straightforward, and the leading bits are squeezed to fit the reference capacity.
		 * - If the reference bit capacity is less than the actual bit length, the rotation is split into two
		 *   parts: the part to be rotated and the steady part that remains unchanged. The rotated part is
		 *   processed and combined with the steady part to form the final result.
		 *
		 * @example
		 * BigInteger num("1001", 2); // Binary representation: 1001
		 * BigInteger result = BigInteger::BitRotateRight(num, 2, 4); // Result: 0110 (binary)
		 */
		static BigInteger BitRotateRight( const BigInteger& bits, size_t shift, size_t reference_bit_capacity );

		explicit operator bool()
		{
			return !this->IsZero();
		}

		// Absolute value
		BigInteger Abs() const;

		/*
		* Reference: https://lemire.me/blog/2024/04/13/greatest-common-divisor-the-extended-euclidean-algorithm-and-speed/
		*/
		static BigInteger GCD( BigInteger a, BigInteger b );

		static BigInteger LCM( const BigInteger& a, const BigInteger& b );

		/**
		 * @brief Pollard's Rho algorithm for integer factorization.
		 *
		 * This function applies Pollard's Rho algorithm to find a non-trivial factor
		 * of the given integer `n`. It returns a factor if found, or -1 if no non-trivial
		 * factor is discovered.
		 *
		 * @param n The integer to factorize.
		 * @return A non-trivial factor of `n`, or -1 if none is found.
		 */
		static BigInteger PollardRho( const BigInteger& n );

		/**
		 * @brief Baby-step Giant-step algorithm for computing discrete logarithms.
		 *
		 * This function uses the Baby-step Giant-step algorithm to solve the discrete
		 * logarithm problem in a finite field. It takes a base `base`, a result `result`,
		 * and a modulus `mod`, and returns the smallest non-negative integer `x` such that
		 * `base^x ≡ result (mod mod)`. If no solution is found, it returns 0.
		 *
		 * @param base The base used for computing the discrete logarithm.
		 * @param result The result of the discrete logarithm computation.
		 * @param mod The modulus.
		 * @return The smallest non-negative integer `x` such that `base^x ≡ result (mod mod)`,
		 * or 0 if no solution is found.
		 */
		static BigInteger BabyStepGiantStep( const BigInteger& base, const BigInteger& result, const BigInteger& mod );

		static BigInteger RandomGenerateNBit( size_t n );

		static BigInteger Factorial(size_t n);

		friend void Swap(BigInteger& lhs, BigInteger& rhs)
		{
			lhs.values.swap(rhs.values);
		}

		/**
		* Calculates the base-2 logarithm (log2) of a BigInteger using binary search.
		*
		* This function iterates through the bits of the BigInteger from the most significant bit to the least
		* significant bit, checking if the bit is set (1). The position of the first set bit is the log2 of the number.
		*
		* @return The value of log2(n) as an integer, or -1 if n is 0 or 1.
		*/
		size_t Log2() const;

		/**
		* Convert the BigInteger to a binary string representation.
		*
		* @param reference_bit_capacity The desired bit capacity of the resulting binary string.
		* @param have_leading_zeros     Flag indicating whether to include leading zeros in the binary string.
		* @return                       Binary string representation of the BigInteger.
		*/
		std::string ToBinaryString( const uint32_t reference_bit_capacity, bool have_leading_zeros = true ) const;

		/**
		* Convert the BigInteger to a string representation with the specified base.
		*
		* @param base_value The desired base for the string representation.
		* @warning Binary base is unsupported in ToString function.
		* @return String representation of the BigInteger in the specified base.
		*/
		std::string ToString( uint32_t base_value = 10 ) const;

		void FromString( const std::string& number_string );

		void FromString( const std::string& number_string, uint32_t base_value );

		template <size_t N>
		void BitsetData( std::bitset<N>& bits ) const
		{
			bool bit = false;
			for ( size_t i = 0; i < N; i++ )
			{
				if ( i >= DIGIT_BITS * values.size() )
				{
					break;
				}

				bool bit = GetBit( i );

				if ( bit )
				{
					bits[ i ] = true;
				}
			}
		}

		/**
		 * @brief Convert the LittleEndian data in this->values to host byte order and export it to OutputData.
		 * 
		 * @param OutputData The vector to store the exported data.
		 */
		void ExportData( std::vector<std::byte>& OutputData, size_t length = 0, bool is_big_endian = false )
		{
			size_t bit_length = BitLength();
			size_t bytes = ( bit_length + CHAR_BIT - 1 ) / CHAR_BIT;
			if ( length == 0 )
			{
				length = bytes;
			}
			OutputData.clear();
			OutputData.resize( length );
			if ( IsZero() )
			{
				return;
			}
			size_t count = std::min( length, bytes );
			for ( size_t i = 0; i < count; i++ )
			{
				OutputData[ i ] = GetByte( i );
			}
			if ( is_big_endian )
			{
				std::reverse( OutputData.begin(), OutputData.end() );
			}
		}

		/**
		 * @brief Convert host byte order data to this->values of the LittleEndian data and import from InData.
		 * 
		 * @param InData The vector containing the data to be imported.
 		 */
		void ImportData( std::vector<std::byte> InData, bool is_big_endian = false )
		{
			if ( InData.empty() )
			{
				this->values.clear();
				return;
			}
			if ( is_big_endian )
			{
				std::reverse( InData.begin(), InData.end() );
			}
			size_t bytes = InData.size();
			size_t block_length = ( bytes + DIGIT_BYTES - 1 ) / DIGIT_BYTES;
			this->values.resize( block_length );
			for ( size_t i = 0; i < bytes; i++ )
			{
				this->SetByte( i, InData[ i ] );
			}
			Clean();
		}

		/**
		 * @brief Convert the LittleEndian data in this->values to host byte order and export it to OutputData.
		 * 
		 * @param OutputData The vector to store the exported data.
		 */
		void ExportData( std::vector<uint8_t>& OutputData, size_t length = 0, bool is_big_endian = false )
		{
			std::vector<std::byte> output_byte;
			ExportData( output_byte, length, is_big_endian );
			OutputData.resize( output_byte.size() );
			for ( size_t i = 0; i < output_byte.size(); i++ )
			{
				OutputData[ i ] = uint8_t( output_byte[ i ] );
			}
		}

		/**
		 * @brief Convert host byte order data to this->values of the LittleEndian data and import from InData.
		 * 
		 * @param InData The vector containing the data to be imported.
 		 */
		void ImportData( std::vector<uint8_t> InData, bool is_big_endian = false )
		{
			std::vector<std::byte> input_byte( InData.size() );
			for ( size_t i = 0; i < input_byte.size(); i++ )
			{
				input_byte[ i ] = std::byte { InData[ i ] };
			}
			ImportData( input_byte, is_big_endian );
		}

		size_t SipHash( const BigInteger& Integer, std::vector<uint8_t>* keys = nullptr ) const;

		friend class HashFunction;

		// 加法运算符重载
		BigInteger operator+(const BigInteger& other) const
		{
			BigInteger result(*this);
			result += other;
			return result;
		}

		// 减法运算符重载
		BigInteger operator-(const BigInteger& other) const
		{
			BigInteger result(*this);
			result -= other;
			return result;
		}

		// 乘法运算符重载
		BigInteger operator*(const BigInteger& other) const
		{
			BigInteger result(*this);
			result *= other;
			return result;
		}

		// 除法运算符重载
		BigInteger operator/(const BigInteger& other) const
		{
			BigInteger result(*this);
			result /= other;
			return result;
		}

		// 取模运算符重载
		BigInteger operator%(const BigInteger& other) const
		{
			BigInteger result(*this);
			result %= other;
			return result;
		}

		friend int compare( const BigInteger& a, const BigInteger& b )
		{
			const size_t len_a = a.values.size();
			const size_t len_b = b.values.size();

			if ( len_a != len_b )
			{
				return len_a > len_b ? 1 : -1;
			}
			size_t i = len_a;
			while ( i > 0 )
			{
				i--;
				if ( a.values[ i ] != b.values[ i ] )
				{
					return a.values[ i ] > b.values[ i ] ? 1 : -1;
				}
			}
			return 0;
		}

		// 等于运算符重载
		bool operator==(const BigInteger& other) const
		{
			return compare(*this, other) == 0;
		}

		// 不等于运算符重载
		bool operator!=(const BigInteger& other) const
		{
			return !(*this == other);
		}

		// 大于运算符重载
		bool operator>(const BigInteger& other) const
		{
			return compare(*this, other) > 0;
		}

		// 大于等于运算符重载
		bool operator>=(const BigInteger& other) const
		{
			return compare(*this, other) >= 0;
		}

		// 小于运算符重载
		bool operator<(const BigInteger& other) const
		{
			return compare(*this, other) < 0;
		}

		// 小于等于运算符重载
		bool operator<=(const BigInteger& other) const
		{
			return compare(*this, other) <= 0;
		}
	};

	class BigSignedInteger
	{
	private:
		friend struct TwilightDream::PrimeNumberTester;

		BigInteger uint_data;
		bool	   sign = false;  //MSB is true, then the number is negative

	public:
		enum class ArithmeticMode : uint8_t
		{
			Addition = 0,
			Subtraction = 1,
			Multiplication = 2,
			Division = 4
		};

		BigSignedInteger();

		BigSignedInteger( int64_t n );

		BigSignedInteger( const BigSignedInteger& other ) noexcept;

		BigSignedInteger( BigSignedInteger&& other ) noexcept;

		BigSignedInteger( const BigInteger& other, bool new_sign = false );

		BigSignedInteger( BigInteger&& other, bool new_sign = false );

		explicit BigSignedInteger( const std::string& number_string );

		BigSignedInteger( const std::string& number_string, uint32_t base );

		operator BigInteger() const;

		BigSignedInteger Abs() const;

		bool IsZero() const;

		bool IsNegative() const;

		size_t Size() const;

		size_t BitLength() const;

		void Print(uint32_t base_value) const;

		void PrintBinary(bool have_space_with_block = true) const;

		/**
		* Convert the BigInteger to a binary string representation.
		*
		* @param reference_bit_capacity The desired bit capacity of the resulting binary string.
		* @param have_leading_zeros     Flag indicating whether to include leading zeros in the binary string.
		* @return                       Binary string representation of the BigInteger.
		*/
		std::string ToBinaryString( const uint32_t reference_bit_capacity, bool have_leading_zeros = true ) const;

		/**
		* Convert the BigInteger to a string representation with the specified base.
		*
		* @param base_value The desired base for the string representation.
		* @warning Binary base is unsupported in ToString function.
		* @return String representation of the BigInteger in the specified base.
		*/
		std::string ToString( uint32_t base_value = 10 ) const;

		void FromString( const std::string& number_string, bool new_sign = false );

		void FromString( const std::string& number_string, uint32_t base_value, bool new_sign = false );

		void FromUnsignedInt(uint64_t value);

		uint64_t ToUnsignedInt() const;

		void FromSignedInt(int64_t value);

		int64_t ToSignedInt(bool is_negative) const;

		void ExportData( bool& is_negative, std::vector<uint8_t>& OutputData, size_t length = 0, bool is_big_endian = false )
		{
			is_negative = sign;
			uint_data.ExportData(OutputData, length, is_big_endian);
		}

		void ImportData( bool is_negative, std::vector<uint8_t> InData, bool is_big_endian = false )
		{
			sign = is_negative;
			uint_data.ImportData(InData, is_big_endian);
		}

		BigSignedInteger operator-() const;

		BigSignedInteger operator+() const;

		friend BigSignedInteger operator+( const BigSignedInteger& lhs, const BigSignedInteger& rhs );

		friend BigSignedInteger operator-( const BigSignedInteger& lhs, const BigSignedInteger& rhs );

		friend BigSignedInteger operator*( const BigSignedInteger& lhs, const BigSignedInteger& rhs );

		friend BigSignedInteger operator/( const BigSignedInteger& lhs, const BigSignedInteger& rhs );

		friend BigSignedInteger operator%( const BigSignedInteger& lhs, const BigSignedInteger& rhs );

		BigSignedInteger& operator=( const BigSignedInteger& other ) noexcept;

		BigSignedInteger& operator=( BigSignedInteger&& other ) noexcept;

		BigSignedInteger& operator+=( const BigSignedInteger& other );

		BigSignedInteger& operator-=( const BigSignedInteger& other );

		BigSignedInteger& operator*=( const BigSignedInteger& other );

		BigSignedInteger& operator/=( const BigSignedInteger& other );

		BigSignedInteger& operator%=( const BigSignedInteger& other );

		BigSignedInteger operator&( const BigSignedInteger& other ) const;

		BigSignedInteger& operator&=( const BigSignedInteger& other );

		BigSignedInteger operator|( const BigSignedInteger& other ) const;

		BigSignedInteger& operator|=( const BigSignedInteger& other );

		BigSignedInteger operator~() const;

		BigSignedInteger operator^( const BigSignedInteger& other ) const;

		BigSignedInteger& operator^=( const BigSignedInteger& other );

		BigSignedInteger& operator<<=( size_t shift );

		BigSignedInteger& operator>>=( size_t shift );

		friend BigSignedInteger operator<<( const BigSignedInteger& lhs, const uint32_t shift )
		{
			BigSignedInteger result( lhs );
			result.uint_data = result.uint_data.LeftShiftBit(shift);
			if(result.uint_data.IsZero())
			{
				result.sign = false;
			}
			return result;
		}

		friend BigSignedInteger operator>>( const BigSignedInteger& lhs, const uint32_t shift )
		{
			BigSignedInteger result( lhs );
			result.uint_data = result.uint_data.RightShiftBit(shift);
			if(result.uint_data.IsZero())
			{
				result.sign = false;
			}
			return result;
		}

		explicit operator bool()
		{
			return !this->IsZero();
		}

		friend bool operator==( const BigSignedInteger& lhs, const BigSignedInteger& rhs );

		friend bool operator!=( const BigSignedInteger& lhs, const BigSignedInteger& rhs );

		friend bool operator>( const BigSignedInteger& lhs, const BigSignedInteger& rhs );

		friend bool operator>=( const BigSignedInteger& lhs, const BigSignedInteger& rhs );

		friend bool operator<( const BigSignedInteger& lhs, const BigSignedInteger& rhs );

		friend bool operator<=( const BigSignedInteger& lhs, const BigSignedInteger& rhs );

		static void EGCD( BigSignedInteger a, BigSignedInteger b, BigSignedInteger& gcd, BigSignedInteger& co1, BigSignedInteger& co2 );

		/**
		 * @brief Performs modular arithmetic operations on two BigIntegers.
		 *
		 * This method supports modular addition, subtraction, multiplication, and division of
		 * two BigIntegers. The specific operation is determined by the `mode` parameter.
		 *
		 * @param mode The arithmetic mode specifying the operation to be performed
		 *             (Addition, Subtraction, Multiplication, or Division).
		 * @param a The first BigInteger operand.
		 * @param b The second BigInteger operand.
		 * @param modulus The modulus with respect to which the modular operation is performed.
		 * @return The result of the specified modular arithmetic operation (a op b) % modulus.
		 *
		 * @throws std::invalid_argument If the modulus is zero or, in the case of division,
		 *         the divisor is zero or the modulus is not prime.
		 * @throws std::invalid_argument If the modular inverse for division cannot be found,
		 *         ensuring (a * b) mod modulus != (a * inverse(b)) mod modulus.
		 */
		static BigSignedInteger ModuloArithmetic( ArithmeticMode mode, const BigSignedInteger& a, const BigSignedInteger& b, const BigSignedInteger& modulus );

		static BigSignedInteger ModuloInverse( const BigSignedInteger& a, const BigSignedInteger& b );

		size_t SipHash( const BigSignedInteger& Integer, std::vector<uint8_t>* keys = nullptr ) const;
	};

	class HashFunction
	{
	public:
		// 使用 lambda 表达式来调用自定义的 hash 方法
		size_t operator()( const BigInteger& key ) const
		{
			return key.SipHash( key );
		}

		// 使用 lambda 表达式来调用自定义的 hash 方法
		size_t operator()( const BigSignedInteger& key ) const
		{
			return key.SipHash( key );
		}
	};

	class Montgomery
	{
	private:
		// Modulus
		BigInteger modulo;
		// Modulus * 2
		BigInteger modulo2;
		// Base (radix)
		BigInteger radix;
		BigInteger radix_square;
		// Negative inverse of the modulus
		BigInteger modulo_inverse_neg;
		size_t	   radix_blocks;

		BigInteger GetR() const;

		BigInteger ModuloR( const BigInteger& a ) const;

		BigInteger DivisionR( const BigInteger& a ) const;

		BigInteger ReduceLazy( const BigInteger& a ) const;

		BigInteger Reduce( const BigInteger& a ) const;

	public:
		Montgomery( const BigInteger& new_modulo );

		BigInteger ToInt( const BigInteger& a ) const;

		BigInteger ToMontgomery( const BigInteger& a ) const;

		BigInteger Addtion( const BigInteger& a, const BigInteger& b ) const;

		BigSignedInteger Addtion( const BigSignedInteger& a, const BigSignedInteger& b ) const;

		BigInteger Substraction( const BigInteger& a, const BigInteger& b ) const;

		BigSignedInteger Substraction( const BigSignedInteger& a, const BigSignedInteger& b ) const;

		BigInteger Multiplication( const BigInteger& a, const BigInteger& b ) const;

		BigSignedInteger Multiplication( const BigSignedInteger& a, const BigSignedInteger& b ) const;

		BigInteger Inverse( const BigInteger& a ) const;

		BigSignedInteger Inverse( const BigSignedInteger& a ) const;

		BigInteger Power( BigInteger base, const BigInteger& index ) const;

		BigSignedInteger Power( BigSignedInteger base, const BigInteger& index ) const;
	};

	//If the number is greater than or equal to 1, then this algorithm can be used to calculate the kth root of a BigInteger.
	class ShiftingKthRoot
	{
	public:
		ShiftingKthRoot( const uint64_t k ) 
			: k_( k ) 
		{}

		BigInteger operator()( const BigInteger& n ) const
		{
			return ComputeRoot( n );
		}

		void SetKthRoot(const uint64_t k) 
		{
			if(0 == k)
			{
				throw std::invalid_argument("k cannot be zero.");
			}

			this->k_ = k;
		}

	private:
		uint64_t k_ = 1; // kth of root

		BigInteger BinarySearch( const BigInteger& low, const BigInteger& high, const std::function<bool( const BigInteger& )>& condition ) const
		{
			BigInteger left = low;
			BigInteger right = high;
			while ( left < right )
			{
				BigInteger mid = ( left + right ) / 2;
				if ( condition( mid ) )
				{
					left = mid + 1;
				}
				else
				{
					right = mid;
				}
			}
			return left;
		}

		BigInteger ComputeRoot(const BigInteger& n) const
		{
			// Number of bits per block
			uint64_t blockBits = 64;

			// Initialize remainder and current root estimate
			BigInteger remainder = 0;
			BigInteger rootEstimate = 0;

			// B represents 2^blockBits (1 << blockBits)
			BigInteger B = BigInteger(1) << blockBits;

			// Bk_bits represents the number of bits needed for k blocks
			uint64_t totalBits = blockBits * k_;

			// Bk_mask is a mask to extract totalBits from n
			BigInteger bitMask = (BigInteger(1) << totalBits) - 1;

			// Calculate the number of iterations required, using uint64_t to prevent overflow
			uint64_t numIterations = (n.BitLength() + totalBits - 1) / totalBits;

			// Loop over each block of bits in reverse order
			for (uint64_t i = numIterations; i > 0; --i)
			{
				// Adjust the index to match the correct bit position
				uint64_t index = i - 1;

				// Extract the current block of bits from n
				BigInteger alpha = (n >> (index * totalBits)) & bitMask;

				// Shift the current root estimate by blockBits
				BigInteger shiftedRoot = rootEstimate << blockBits;

				// Calculate the current power of the root estimate
				BigInteger currentPower = (rootEstimate.Power(k_)) << totalBits;

				// Add alpha to the remainder shifted by totalBits
				BigInteger shiftedRemainder = (remainder << totalBits) + alpha;

				// Calculate the sum of the current power and the shifted remainder
				BigInteger powerPlusRemainder = currentPower + shiftedRemainder;

				// Define the condition for binary search
				auto condition = [&](const BigInteger& beta)
				{
					return (shiftedRoot + beta).Power(k_) <= powerPlusRemainder;
				};

				// Perform binary search to find the appropriate beta
				BigInteger beta = BinarySearch(BigInteger(1), B, condition) - 1;

				// Update the current root estimate with beta
				rootEstimate = shiftedRoot + beta;

				// Update the remainder
				remainder = shiftedRemainder - ((shiftedRoot + beta).Power(k_) - currentPower);
			}

			// Return the final root estimate
			return rootEstimate;
		}
	};

}  // namespace TwilightDream::BigInteger

#endif
