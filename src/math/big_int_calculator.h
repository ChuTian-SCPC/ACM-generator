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
        };
    } // namespace math
} // namespace generator

#endif // !_SGPCET_BIG_INT_CALCULATOR_H_
