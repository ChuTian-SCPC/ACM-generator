#ifndef _SGPCET_BIG_INT_CALCULATOR_H_
#define _SGPCET_BIG_INT_CALCULATOR_H_

#ifndef _SGPCET_MACRO_H_
#include "basic/macro.h"
#endif //!_SGPCET_MACRO_H_
#ifndef _SGPCET_NTT_H_
#include "ntt.h"
#endif // !_SGPCET_NTT_H_

namespace generator {
    namespace math {
        template <typename TYPE>
        class BigIntCalculator {
        public:
            using i32 = int32_t;
            using i64 = int64_t;
            using u32 = uint32_t;
            using u64 = uint64_t;
        protected:
            bool _is_negative;
            std::vector<i32> _data;
        public:

            BigIntCalculator() {
                set_value(0);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE& set_value(T val) {
                _data.clear();
                u64 v = val < 0 ? -val : val;
                _is_negative = val < 0;
                if (val == 0) {
                    _data.push_back(0);
                    _is_negative = false;
                    return static_cast<TYPE&>(*this);
                }
                i32 base = static_cast<TYPE*>(this)->__base();
                while (v) {
                    _data.push_back(v % base);
                    v /= base; 
                }
                return static_cast<TYPE&>(*this);
            }

            size_t size() { return _data.size(); }
            size_t size() const { return _data.size(); }

            // for test TEMPORARY
            TYPE& add(const TYPE& other) {
                return __add(other);
            }

            TYPE& mul(const TYPE& other) {
               return __ntt_mul(other); 
            }

            _GET_VALUE(std::vector<i32>, data)
            _GET_VALUE(bool, is_negative)
        protected:
            void __trim() {
                while (_data.size() > 1 && _data.back() == 0) _data.pop_back();
                if (_data.size() == 1 && _data.back() == 0) _is_negative = false;
            }

            TYPE& __add(const TYPE& other) {
                if (size() < other.size()) _data.resize(other.size(), 0);
                i64 add = 0;
                for (size_t i = 0; i < other.size(); i++) {
                    static_cast<TYPE*>(this)->__carry(add, _data[i], _data[i] + other._data[i]);
                }
                for (size_t i = other.size(); i < size(); i++) {
                    static_cast<TYPE*>(this)->__carry(add, _data[i], _data[i]); 
                }
                if(add) _data.push_back(add);
                __trim();
                return static_cast<TYPE&>(*this);
            }

            // 需要保证*this >= other
            TYPE& __sub(const TYPE& other) {
                i64 add = 0; 
                for (size_t i = 0; i < size(); i++) {
                    static_cast<TYPE*>(this)->__borrow(add, _data[i], _data[i] - other._data[i]);
                }
                return static_cast<TYPE&>(*this);
            }

            TYPE& __mul_int(u32 val)  {
                u64 add = 0;
                for (size_t i = 0; i < size(); i++) {
                    static_cast<TYPE*>(this)->__carry(add, _data[i], (u64)_data[i] * val);
                }
                i32 base = static_cast<TYPE*>(this)->__base();
                while (add) {
                    _data.push_back(add % base);
                    add /= base;
                }
                __trim();
                return static_cast<TYPE&>(*this);
            }

            TYPE& __simple_mul(const TYPE& other) {
                size_t len_a = size();
                size_t len_b = other.size();
                std::vector<i32> result(len_a + len_b, 0);
                for (size_t i = 0; i < len_a; i++) {
                    i64 add = 0;
                    for (size_t j = 0; j < len_b; j++) {
                        static_cast<TYPE*>(this)->__carry(add, result[i + j], (i64)_data[i] * other._data[j] + result[i + j]);
                    }
                    for (size_t k = i + len_b; add; k++) {
                        static_cast<TYPE*>(this)->__carry(add, result[k], add + result[k]);
                    }
                }
                _data = std::move(result);
                return static_cast<TYPE&>(*this);
            }

            TYPE& __ntt_mul(const TYPE& other) {
                const i32 base = static_cast<TYPE*>(this)->__base();
                CrtMultiplier<i32, base> crt;
                auto result = crt.multiply(_data, other._data);
                _data = std::move(result);
                return static_cast<TYPE&>(*this);
            }

            // 左移digits位（相当于乘 base^k）
            TYPE __shift_digits_left(size_t k) const {
                if (_data.empty() || (_data.size() == 1 && _data[0] == 0)) return static_cast<const TYPE&>(*this);
                TYPE res = static_cast<const TYPE&>(*this);
                res._data.insert(res._data.begin(), k, 0);
                return res;
            }

            // 右移digits位（相当于除 base^k）
            TYPE __shift_digits_right(size_t k) const {
                TYPE res = static_cast<const TYPE&>(*this);
                if (k >= res._data.size()) {
                    res._data = {0};
                    res._is_negative = false;
                } else {
                    res._data.erase(res._data.begin(), res._data.begin() + k);
                }
                return res;
            }

            TYPE operator<<(size_t bits) const {
                size_t digit_bits = static_cast<const TYPE*>(this)->__digits();
                size_t digits = bits / digit_bits;
                size_t rem = bits % digit_bits;

                TYPE res = __shift_digits_left(digits);
                if (rem == 0) return res;

                i32 base = static_cast<const TYPE*>(this)->__base();
                u64 carry = 0;
                for (size_t i = 0; i < res._data.size(); i++) {
                    u64 val = ((u64)res._data[i] << rem) + carry;
                    res._data[i] = val % base;
                    carry = val / base;
                }
                if (carry) res._data.push_back(carry);
                return res;
            }

            TYPE operator>>(size_t bits) const {
                size_t digit_bits = static_cast<const TYPE*>(this)->__digits();
                size_t digits = bits / digit_bits;
                size_t rem = bits % digit_bits;

                TYPE res = __shift_digits_right(digits);
                if (rem == 0) return res;

                i32 base = static_cast<const TYPE*>(this)->__base();
                u64 carry = 0;
                for (size_t i = res._data.size(); i-- > 0;) {
                    u64 val = ((u64)carry << digit_bits) + res._data[i];
                    res._data[i] = val >> rem;
                    carry = val & ((1ULL << rem) - 1);
                }
                res.__trim();
                return res;
            }

            TYPE __inv(const TYPE& a) const {
                if (a.size() == 1) {
                    u64 inv = static_cast<u64>(1) << (static_cast<const TYPE*>(this)->__digits() * 2);
                    return TYPE().set_value(inv / a._data[0]);
                }

                size_t half = a.size() / 2;
                TYPE low = a.__shift_digits_right(half);
                TYPE x = __inv(low);

                TYPE ax = a * x * x;
                size_t shift = 2 * half;
                ax = ax.__shift_digits_right(shift);
                x = x.__shift_digits_left(half);
                x = x + x - ax;
                x.__trim();
                return x;
            }

            struct __divmod_result {
                TYPE quot;
                TYPE rem;
            };

            __divmod_result __div_mod(const TYPE& a_raw, const TYPE& b_raw) const {
                TYPE a = a_raw, b = b_raw;
                bool neg = a._is_negative != b._is_negative;
                a._is_negative = b._is_negative = false;

                if (a < b) return {{0}, a};

                size_t shift = a.size() > b.size() * 2 ? a.size() - b.size() * 2 : 0;
                size_t digits = static_cast<const TYPE*>(this)->__digits();
                TYPE a_shifted = a << (shift * digits);
                TYPE b_shifted = b << (shift * digits);

                TYPE bi = __inv(b_shifted);
                TYPE prod = b_shifted * bi;
                if (prod >= (TYPE().set_value(1) << (2 * b_shifted.size() * digits))) {
                    prod = prod - b_shifted;
                    bi = bi - TYPE().set_value(1);
                }

                TYPE quot;
                while (true) {
                    TYPE c = (a_shifted * bi) >> (2 * b_shifted.size() * digits);
                    if (c == TYPE().set_value(0)) break;
                    TYPE tmp = b_shifted * c;
                    if (tmp > a_shifted) break;
                    a_shifted = a_shifted - tmp;
                    quot = quot + c;
                }

                while (b_shifted <= a_shifted) {
                    a_shifted = a_shifted - b_shifted;
                    quot = quot + TYPE().set_value(1);
                }

                quot._is_negative = neg;
                a_shifted._is_negative = a_raw._is_negative;

                TYPE rem = shift ? a_shifted >> (shift * digits) : a_shifted;
                return {quot, rem};
            }
        };
    } // namespace math
} // namespace generator

#endif // !_SGPCET_BIG_INT_CALCULATOR_H_
