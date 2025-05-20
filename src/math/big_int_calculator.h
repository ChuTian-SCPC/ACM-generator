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

            struct DivResult {
                TYPE quotient; // 商
                TYPE remainder; // 余数 

                DivResult() : quotient(0), remainder(0) {}
                DivResult(TYPE q, TYPE r) : quotient(q), remainder(r) {}
                TYPE first() { return quotient; }
                TYPE second() { return remainder; }
            };
        protected:
            bool _is_negative;
            std::vector<u32> _data;
        public:

            BigIntCalculator() {
                set_value(0);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            BigIntCalculator(T val) {
                set_value(val);
            }
            
            BigIntCalculator(const TYPE& other) : _data(other._data), _is_negative(other._is_negative) {}

            BigIntCalculator& operator=(const TYPE& other) {
                if (this != &other) {
                    _data = other._data;
                    _is_negative = other._is_negative;
                }
                return *this; 
            }

            BigIntCalculator(TYPE&& other) noexcept : _data(std::move(other._data)), _is_negative(other._is_negative) {}

            BigIntCalculator& operator=(TYPE&& other) noexcept {
                if (this!= &other) {
                    _data = std::move(other._data);
                    _is_negative = other._is_negative;
                }
                return *this; 
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

            TYPE operator+() const {
                TYPE result(static_cast<const TYPE&>(*this));
                return result;
            }

            TYPE operator+(const TYPE& other) const {
                if ((_is_negative ^ other._is_negative) == 0) return TYPE::__add(static_cast<const TYPE&>(*this), other); // 同号
                else if (_is_negative) return TYPE::__sub(other, static_cast<const TYPE&>(*this)); // 负数 + 正数
                else return TYPE::__sub(static_cast<const TYPE&>(*this), other); // 正数 + 负数
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE operator+(const T& a) const {
                return (*this) + TYPE(a);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            friend TYPE operator+(const T& a, const TYPE& b) { 
                return b + a; 
            }

            TYPE operator+=(const TYPE& other) {
                *this = *this + other;
                return static_cast<TYPE&>(*this); 
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE operator+=(const T& a) {
                return operator+=(TYPE(a));
            }

            TYPE operator++() {
               *this = *this + 1;
               return static_cast<TYPE&>(*this); 
            }

            TYPE operator++(int) {
                TYPE result(static_cast<const TYPE&>(*this));
                *this = *this + 1;
                return result; 
            }

            TYPE operator-() const {
                TYPE result(static_cast<const TYPE&>(*this));
                if (result.__is_zero()) result._is_negative = false;
                else result._is_negative = !result._is_negative;
                return result;
            }

            TYPE operator-(const TYPE& other) const {
                if (_is_negative ^ other._is_negative) return TYPE::__add(static_cast<const TYPE&>(*this), other); // 异号
                else if (_is_negative) return TYPE::__sub(other, static_cast<const TYPE&>(*this)); // 负数 - 负数
                else return TYPE::__sub(static_cast<const TYPE&>(*this), other); // 正数 - 正数
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE operator-(const T& a) const {
                return (*this) - TYPE(a);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            friend TYPE operator-(const T& a, const TYPE& b) {
                return TYPE(a) - b;
            }

            TYPE operator-=(const TYPE& other) {
                *this = *this - other;
                return static_cast<TYPE&>(*this); 
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE operator-=(const T& a) {
                return operator-=(TYPE(a)); 
            }

            TYPE operator--() {
                *this = *this - 1;
                return static_cast<TYPE&>(*this); 
            }

            TYPE operator--(int) {
                TYPE result(static_cast<const TYPE&>(*this));
                *this = *this - 1;
                return result; 
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE operator*(const T& other) const {
                return (*this) * TYPE(other);
            }

            TYPE operator*(const TYPE& other) const {
                if (__is_zero() || other.__is_zero()) return TYPE();
                if (this == &other) return TYPE::__ntt_square(other);
                TYPE result;
                if (__one_size()) result = TYPE::__mul_int(other, _data[0]);
                if (other.__one_size()) result = TYPE::__mul_int(static_cast<const TYPE&>(*this), other._data[0]);
                if (this->size() + other.size() < _setting::ntt_threshold) result = TYPE::__simple_mul(static_cast<const TYPE&>(*this), other);
                else result = TYPE::__ntt_mul(static_cast<const TYPE&>(*this), other);
                result._is_negative = _is_negative ^ other._is_negative;
                return result;
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            friend TYPE operator*(const T& a, const TYPE& b) { 
                return b * a; 
            }

            TYPE operator*=(const TYPE& other) {
                *this = *this * other;
                return static_cast<TYPE&>(*this); 
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE operator*=(const T& a) {
                return operator*=(TYPE(a));
            }

            DivResult div_mod(const TYPE& other) const {
                if (this == &other) return DivResult{TYPE(1), TYPE(0)};
                if (other.__one_size()) {
                    DivResult result = TYPE::__div_int(static_cast<const TYPE&>(*this), (u32)other._data[0]);
                    if (other._is_negative) result.quotient._is_negative = !result.quotient._is_negative;
                    return result;
                }
                if (size() + other.size() < _setting::fast_div_threshold) return TYPE::__simple_div(static_cast<const TYPE&>(*this), other);
                else return TYPE::__fast_div(static_cast<const TYPE&>(*this), other);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            DivResult div_mod(const T& other) const {
                return div_mod(TYPE(other));
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE operator/(const T& other) {
                return div_mod(other).first();
            }

            TYPE operator/(const TYPE& other) {
                return div_mod(other).first(); 
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            friend TYPE operator/(const T& a, const TYPE& b) { 
                return TYPE(a) / b; 
            }

            TYPE operator/=(const TYPE& other) {
                *this = *this / other;
                return static_cast<TYPE&>(*this);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE operator/=(const T& a) {
                return operator/=(TYPE(a));
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE operator%(const T& other) {
                return div_mod(other).second();
            }

            TYPE operator%(const TYPE& other) {
                return div_mod(other).second();  
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            friend TYPE operator%(const T& a, const TYPE& b) {
                return TYPE(a) % b;
            }

            TYPE operator%=(const TYPE& other) {
                *this = *this % other;
                return static_cast<TYPE&>(*this); 
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE operator%=(const T& a) {
                return operator%=(TYPE(a));
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            static TYPE pow(const TYPE& a, T b) {
                if (b < 0) {
                    if (a.__is_zero()) _msg::__error_msg(_msg::_defl, "Zero cannot be raised to a negative power.");
                    else if (a.__is_one()) return TYPE(1);
                    else return TYPE(0);
                }
                if (b == 0) return TYPE(1);
                if (b == 1) return TYPE(a);
                TYPE result(1);
                TYPE base(a);
                while (b) {
                    if (b & 1) result = result * base;
                    base = base * base;
                    b >>= 1;
                }
                return result;
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            static TYPE pow(T a, T b) {
                return pow(TYPE(a), b);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE& pow(T b) {
                *this = TYPE::pow(*this, b);
                return static_cast<TYPE&>(*this);
            }

            bool operator==(const TYPE& other) const {
                return TYPE::__equal(static_cast<const TYPE&>(*this), other);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            bool operator==(const T& other) const {
                return *this == TYPE(other); 
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            friend bool operator==(const T& a, const TYPE& b) { 
                return b == a; 
            }

            bool operator!=(const TYPE& other) const {
                return !operator==(other);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            bool operator!=(const T& other) const {
                return *this != TYPE(other);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            friend bool operator!=(const T& a, const TYPE& b) {
                return b != a;
            }

            bool operator<(const TYPE& other) const {
                return TYPE::__less(static_cast<const TYPE&>(*this), other);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            bool operator<(const T& other) const {
                return *this < TYPE(other);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            friend bool operator<(const T& a, const TYPE& b) {
                return b > a;
            }

            bool operator<=(const TYPE& other) const {
                return operator<(other) || operator==(other); 
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            bool operator<=(const T& other) const {
                return *this <= TYPE(other);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            friend bool operator<=(const T& a, const TYPE& b) {
                return b >= a;
            }

            bool operator>(const TYPE& other) const {
                return !operator<=(other);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            bool operator>(const T& other) const {
                return *this > TYPE(other);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            friend bool operator>(const T& a, const TYPE& b) {
                return b < a;
            }

            bool operator>=(const TYPE& other) const {
                return !operator<(other);  
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            bool operator>=(const T& other) const {
                return *this >= TYPE(other);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            friend bool operator>=(const T& a, const TYPE& b) {
                return b <= a;
            }

            static TYPE abs(const TYPE& a) {
                TYPE result(a);
                result._is_negative = false;
                return result;
            }

            _SET_GET_VALUE(std::vector<u32>, data)
            _SET_GET_VALUE(bool, is_negative)
        protected:
            void __trim() {
                while (_data.size() > 1 && _data.back() == 0) _data.pop_back();
                if (_data.size() == 1 && _data.back() == 0) _is_negative = false;
            }

            bool __is_zero() {
                return _data.size() == 1 && _data[0] == 0;
            }

            bool __is_zero() const {
                return _data.size() == 1 && _data[0] == 0;
            }

            bool __is_one() {
                return _data.size() == 1 && _data[0] == 1;
            }

            bool __is_one() const {
                return _data.size() == 1 && _data[0] == 1; 
            }

            bool __one_size() {
                return _data.size() == 1;
            }

            bool __one_size() const {
                return _data.size() == 1; 
            }

            // 值相加，保证符号相同
            static TYPE __add(const TYPE& a, const TYPE& b) {
                TYPE result;
                result._is_negative = a._is_negative;
                result._data.resize(std::max(a.size(), b.size()), 0); 
                u64 add = 0;
                for (size_t i = 0; i < result.size(); i++) {
                    u64 sum = 0;
                    if (i < a.size()) sum += a._data[i];
                    if (i < b.size()) sum += b._data[i];
                    result.__carry(add, result._data[i], sum);
                }
                if (add) result._data.push_back(add);
                result.__trim();
                return result;
            }

            // 值相减
            static TYPE __sub(const TYPE& a, const TYPE& b) {
                TYPE result;
                result._data.resize(std::max(a.size(), b.size()), 0);
                i64 borrow = 0;
                for (size_t i = 0; i < result.size(); i++) {
                    i64 diff = 0;
                    if (i < a.size()) diff += a._data[i];
                    if (i < b.size()) diff -= b._data[i];
                    result.__borrow(borrow, result._data[i], diff); 
                }
                if (borrow) {
                    result._is_negative = true;
                    i64 add = 1;
                    i32 base = result.__base();
                    for (size_t i = 0; i < result.size(); i++) {
                        result.__carry(add, result._data[i], base - result._data[i] - 1);
                    }
                }
                result.__trim();
                return result;
            }

            // 值相乘
            static TYPE __mul_int(const TYPE& a, u32 b) {
                TYPE result;
                result._data.resize(a.size(), 0);
                u64 add = 0;
                for (size_t i = 0; i < result.size(); i++) {
                    result.__carry(add, result._data[i], (u64)a._data[i] * b); 
                }
                i32 base = result.__base();
                while (add) {
                    result._data.push_back(add % base);
                    add /= base;
                }
                result.__trim();
                return result;
            }
            
            static TYPE __simple_mul(const TYPE& a, const TYPE& b) {
                size_t len_a = a.size();
                size_t len_b = b.size();
                TYPE result;
                result._data.resize(len_a + len_b, 0); 
                for (size_t i = 0; i < len_a; i++) {
                    u64 add = 0, va = a._data[i]; 
                    for (size_t j = 0; j < len_b; j++) {
                        result.__carry(add, result._data[i + j], va * (u64)b._data[j] + result._data[i + j]); 
                    }
                    for (size_t k = i + len_b; add; k++) {
                        result.__carry(add, result._data[k], result._data[k]); 
                    }
                }
                result.__trim();
                return result;
            }

            static TYPE __ntt_mul(const TYPE& a, const TYPE& b) {
                TYPE result;
                i32 base = a.__base();
                auto data = CrtMultiplier<u32>::multiply(a._data, b._data, base);
                result._data = std::move(data);
                return result;
            }

            static TYPE __ntt_square(const TYPE& a) {
                if (a.__is_zero()) return TYPE();
                TYPE result;
                i32 base = a.__base();
                auto data = CrtMultiplier<u32>::square(a._data, base); 
                result._data = std::move(data);
                result._is_negative = false;
                return result;
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            static int __log_pow2(T x) {
                int n = 0;
                while(x >>= 1) n++;
                return n;
            }

            static bool __is_pow2(i32 x) {
                return x && ((x & (x - 1)) == 0);
            }

            static TYPE __left_shift(const TYPE& a, i32 shift) {
                if (shift < 0) _msg::__error_msg(_msg::_defl, "Shift value cannot be negative.");
                if (shift == 0) return TYPE(a);
                int d = a.__digits();
                int sum = shift / d;
                int res = shift % d;
                TYPE result;
                result._is_negative = a._is_negative;
                result._data.resize(sum, 0);
                TYPE b(a);
                b = TYPE::__mul_int(a, TYPE::__radix_pow(a, res));
                result._data.insert(result._data.end(), b._data.begin(), b._data.end());
                result.__trim();
                return result;
            }

            static TYPE __left_shift_bits(const TYPE& a, i32 shift) {
                if (shift < 0) _msg::__error_msg(_msg::_defl, "Shift value cannot be negative.");
                if (shift == 0) return TYPE(a);
                int radix = a.__radix();
                if (radix == 2) return TYPE::__left_shift(a, shift);
                if (__is_pow2(radix)) { // 2的幂次
                    int log_radix = __log_pow2(radix);
                    TYPE result = TYPE::__left_shift(a, shift / log_radix);
                    return result * TYPE::pow(2, shift % log_radix);
                }
                else {
                    TYPE m = TYPE::pow(2, shift);
                    TYPE result(a);
                    return result * m;
                }
            }

            static TYPE __right_shift(const TYPE& a, i32 shift) {
                if (shift < 0) _msg::__error_msg(_msg::_defl, "Shift value cannot be negative.");
                if (shift == 0) return TYPE(a);
                if (shift > a.size() * a.__digits()) return TYPE(0);
                int d = a.__digits();
                int sum = shift / d;
                int res = shift % d;
                TYPE result;
                result._is_negative = a._is_negative;
                result._data.insert(result._data.begin(), a._data.begin() + sum, a._data.end());
                result = TYPE::__div_int(result, TYPE::__radix_pow(a, res)).first();
                result.__trim();
                return result;
            }

            static TYPE __right_shift_bits(const TYPE& a, i32 shift) {
                if (shift < 0) _msg::__error_msg(_msg::_defl, "Shift value cannot be negative.");
                if (shift == 0) return TYPE(a);
                int radix = a.__radix();
                if (radix == 2) return TYPE::__right_shift(a, shift);
                if (__is_pow2(radix)) { // 2的幂次
                    int log_radix = __log_pow2(radix);
                    TYPE result = TYPE::__right_shift(a, shift / log_radix);
                    return result / TYPE::pow(2, shift % log_radix);
                }
                else {
                    TYPE m = TYPE::pow(2, shift);
                    TYPE result(a);
                    return result / m;
                } 
            }

            static DivResult __div_int(const TYPE& a, u32 b) {
                if (b == 0) _msg::__error_msg(_msg::_defl, "Division by zero.");
                if (b == 1) return DivResult(TYPE(a), TYPE(0));
                if (a.__is_zero()) return DivResult(TYPE(0), TYPE(0));
                TYPE q; // 商
                u64 r = 0; // 余数
                q._data.resize(a.size(), 0);
                u64 base = (u64)a.__base();
                for (int i = a.size() - 1; i >= 0; i--) {
                    r = r * base + a._data[i];
                    q._data[i] = r / b;
                    r %= b;
                }
                q.__trim();
                q._is_negative = a._is_negative;
                TYPE res(r);
                res._is_negative = a._is_negative;
                return DivResult(q, res);
            }

            static DivResult __simple_div(const TYPE& a, const TYPE& b) {
                if (b.__is_zero()) _msg::__error_msg(_msg::_defl, "Division by zero.");
                if (b.__is_one()) {
                    if (b._is_negative) return DivResult(TYPE(-a), TYPE(0));
                    else return DivResult(TYPE(a), TYPE(0));
                }
                if (a.__is_zero()) return DivResult(TYPE(0), TYPE(0));
                if (__abs_compare(a, b) < 0) return DivResult(TYPE(0), a);
                TYPE q, r; // 商和余数
                TYPE xa = TYPE::abs(a);
                TYPE xb = TYPE::abs(b);
                TYPE p(1); // 幂次
                while (xa >= xb) {
                    xb = TYPE::__mul_int(xb, 2);
                    p = TYPE::__mul_int(p, 2);
                }
                while (p >= 1) {
                    if (xa >= xb) {
                        xa -= xb;
                        q += p;
                    }
                    xb = TYPE::__div_int(xb, 2).first();
                    p = TYPE::__div_int(p, 2).first();
                }
                q._is_negative = a._is_negative ^ b._is_negative;
                q.__trim();
                r = a - q * b;
                return DivResult(q, r);
            }

            static TYPE __newton_inv(const TYPE& num, i32 n) {
                if (num == 0) _msg::__error_msg(_msg::_defl, "Division by zero.");
                if (n - num.size() <= _setting::newton_threshold) {
                    TYPE sum = TYPE::__left_shift(TYPE(1), n);
                    return TYPE::__simple_div(sum, num).first();
                }

                i32 k = (n - num.size() + 2) >> 1;
                i32 k2 = k > num.size() ? 0 : num.size() - k;
                TYPE x = TYPE::__right_shift(num, k2);
                i32 n2 = k + x.size();
                TYPE y = __newton_inv(x, n2);
                TYPE a = y + y;
                TYPE b = num * TYPE::__ntt_square(y);
                return TYPE::__left_shift(a, n - n2 - k2) - TYPE::__right_shift(b, (2 * (n2 + k2) - n)) - 1;
            }

            static DivResult __fast_div(const TYPE &a, const TYPE &b) {
                if (b.__is_zero()) _msg::__error_msg(_msg::_defl, "Division by zero.");
                if (b.__is_one()) {
                    if (b._is_negative) return DivResult(TYPE(-a), TYPE(0));
                    else return DivResult(TYPE(a), TYPE(0));
                }
                if (a.__is_zero()) return DivResult(TYPE(0), TYPE(0));
                if (__abs_compare(a, b) < 0) return DivResult(TYPE(0), a);              
                TYPE xa = TYPE::abs(a);
                TYPE xb = TYPE::abs(b);
                i32 k = xa.size() - xb.size() + 2;
                i32 k2 = k > xb.size() ? 0 : xb.size() - k;
                TYPE adj = TYPE::__right_shift(xb, k2);
                if (k2 != 0) adj++;
                i32 n2 = k + adj.size();
                TYPE inv = __newton_inv(adj, n2);
                TYPE q = TYPE::__left_shift(xa * inv, n2 + k2);
                TYPE r = xa - q * xb;
                while (r >= xb) {
                    q++;
                    r -= xb;
                }
                q._is_negative = a._is_negative ^ b._is_negative;
                r._is_negative = a._is_negative;
                q.__trim();
                r.__trim();
                return DivResult(q, r);
            }

            // -1 : a < b; 
            // 0  : a == b; 
            // 1  : a > b;
            static int __abs_compare(const TYPE& a, const TYPE& b) {
                if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
                for (size_t i = a.size(); i-- > 0;) {
                    if (a._data[i] != b._data[i]) return a._data[i] < b._data[i] ? -1 : 1;
                }
                return 0;
            }

            static bool __equal(const TYPE& a, const TYPE& b) {
               return a._is_negative == b._is_negative && __abs_compare(a, b) == 0; 
            }

            static bool __less(const TYPE& a, const TYPE& b) {
                if (a._is_negative != b._is_negative) return a._is_negative;
                return a._is_negative? __abs_compare(a, b) > 0 : __abs_compare(a, b) < 0; 
            }

            static i64 __radix_pow(const TYPE& a, i32 b) {
                static std::unordered_map<i32, std::vector<i64>> radix_pow;
                int radix = a.__radix();
                if (radix_pow.find(radix) == radix_pow.end()) {
                    radix_pow.insert({radix, std::vector<i64>({1})}); 
                }
                auto &v = radix_pow[radix];
                int os = v.size();
                if (v.size() <= b) {
                    i64 base = v.back();
                    v.resize(b + 1);
                    for (size_t i = os; i <= b; i++) {
                        base *= radix;
                        v[i] = base;
                    }
                }
                return v[b];
            }

        };
    } // namespace math
} // namespace generator

#endif // !_SGPCET_BIG_INT_CALCULATOR_H_
