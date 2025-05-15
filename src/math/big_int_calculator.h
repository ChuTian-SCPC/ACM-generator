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
            std::vector<u32> _data;

            const static i32 NTT_THRESHOLD = 1000;
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

            TYPE operator+(const TYPE& other) {
                if ((_is_negative ^ other._is_negative) == 0) return TYPE::__add(static_cast<const TYPE&>(*this), other); // 同号
                else if (_is_negative) return TYPE::__sub(other, static_cast<const TYPE&>(*this)); // 负数 + 正数
                else return TYPE::__sub(static_cast<const TYPE&>(*this), other); // 正数 + 负数
            }

            TYPE operator-() {
                if (__is_zero()) _is_negative = false;
                else _is_negative = !_is_negative;
                return static_cast<TYPE&>(*this);
            }

            TYPE operator-(const TYPE& other) {
                if (_is_negative ^ other._is_negative) return TYPE::__add(static_cast<const TYPE&>(*this), other); // 异号
                else if (_is_negative) return TYPE::__sub(other, static_cast<const TYPE&>(*this)); // 负数 - 负数
                else return TYPE::__sub(static_cast<const TYPE&>(*this), other); // 正数 - 正数
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            TYPE operator*(const T& other) {
                return (*this) * TYPE(other);
            }

            TYPE operator*(const TYPE& other) {
                if (__is_zero() || other.__is_zero()) return TYPE();
                TYPE result;
                if (__one_size()) result = TYPE::__mul_int(other, _data[0]);
                if (other.__one_size()) result = TYPE::__mul_int(static_cast<const TYPE&>(*this), other._data[0]);
                if (this->size() < NTT_THRESHOLD || other.size() < NTT_THRESHOLD) result = TYPE::__simple_mul(static_cast<const TYPE&>(*this), other);
                else result = TYPE::__ntt_mul(static_cast<const TYPE&>(*this), other);
                result._is_negative = _is_negative ^ other._is_negative;
                return result;
            }

            _GET_VALUE(std::vector<u32>, data)
            _GET_VALUE(bool, is_negative)
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
                CrtMultiplier<u32> crt(base);
                auto data = crt.multiply(a._data, b._data);
                result._data = std::move(data);
                return result;
            }

            // -1 : a < b; 
            // 0  : a == b; 
            // 1  : a > b;
            int __abs_compare(const TYPE& a, const TYPE& b) {
                if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
                for (size_t i = a.size(); i-- > 0;) {
                    if (a._data[i] != b._data[i]) return a._data[i] < b._data[i] ? -1 : 1;
                }
                return 0;
            }

            bool __equal(const TYPE& a, const TYPE& b) {
               return a._is_negative == b._is_negative && __abs_compare(a, b) == 0; 
            }

            bool __less(const TYPE& a, const TYPE& b) {
                if (a._is_negative != b._is_negative) return a._is_negative;
                return a._is_negative? __abs_compare(a, b) > 0 : __abs_compare(a, b) < 0; 
            }

        };
    } // namespace math
} // namespace generator

#endif // !_SGPCET_BIG_INT_CALCULATOR_H_
