#ifndef _SGPCET_BIG_INT_BASE_H_
#define _SGPCET_BIG_INT_BASE_H_

#ifndef _SGPCET_BIG_INT_CALCULATOR_H_
#include "math/big_int_calculator.h"
#endif //!_SGPCET_BIG_INT_CALCULATOR_H_

namespace generator {
    namespace math {
        class BigIntBase : public BigIntCalculator<BigIntBase> {
        public:
            friend class BigIntCalculator<BigIntBase>;
            const static i32 MAX_BASE = 1 << 15;
        protected:
            i32 _radix;
            i32 _base;
            i32 _digits;
        public:

            BigIntBase() {
                set_radix(10);
                set_value(0);
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            BigIntBase(T val) {
                set_radix(10);
                set_value(val);
            }

            BigIntBase(const BigIntBase& other) : 
                BigIntCalculator<BigIntBase>(other),
                _digits(other._digits), _radix(other._radix), _base(other._base) {}
            
            BigIntBase& operator=(const BigIntBase& other) {
                if (this != &other) {
                    BigIntCalculator<BigIntBase>::operator=(other);
                    _digits = other._digits;
                    _radix = other._radix;
                    _base = other._base; 
                }
                return *this;
            }

            BigIntBase(BigIntBase&& other) noexcept : 
                BigIntCalculator<BigIntBase>(std::move(other)), 
                _digits(other._digits), _radix(other._radix), _base(other._base) {}

            BigIntBase& operator=(BigIntBase&& other) noexcept {
                if (this != &other) {
                    BigIntCalculator<BigIntBase>::operator=(std::move(other));
                    _digits = other._digits;
                    _radix = other._radix;
                    _base = other._base;
                } 
                return *this;
            }

            void set_radix(i32 radix) {
                _radix = radix;
                _base = radix;
                for (_digits = 1; _base * radix <= MAX_BASE; _digits++, _base *= radix);
            }
        protected:
            template <typename T1, typename T2, typename T3>
            void __carry(T1& add, T2& basic_val, T3 new_val) {
                add += (T1)new_val;
                basic_val = add % (T1)_base;
                add /= (T1)_base;
            } 

            template <typename T1, typename T2, typename T3>
            void __borrow(T1& add, T2& basic_val, T3 new_val) {
               add += (T1)new_val - (T1)_base + 1;
               basic_val = add % (T1)_base + (T1)_base - 1;
               add /= (T1)_base; 
            }

            i32 __base() const { return _base; }
            i32 __radix() const { return _radix; }
            i32 __digits() const { return _digits; }
        };
    } // namespace math
} // namespace generator

#endif // !_SGPCET_BIG_INT_BASE_H_
