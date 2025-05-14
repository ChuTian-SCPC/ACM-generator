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
            const static i32 NTT_THRESHOLD = 1000;
        protected:
            i32 _base;
            i32 _digits;
        public:

            BigIntBase() {
                set_base(10);
            }

            void set_base(i32 base) {
                _base = base;
                for (_digits = 1; _base * base <= MAX_BASE; _digits++, _base *= base);
                this->set_value(0);
            }
        protected:
            template <typename T1, typename T2, typename T3>
            void __carry(T1& add, T2& base, T3 new_val) {
                add += (T1)new_val;
                base = add % (T1)_base;
                add /= (T1)_base;
            } 

            template <typename T1, typename T2, typename T3>
            void __borrow(T1& add, T2& base, T3 new_val) {
               add += (T1)new_val - (T1)base + 1;
               base = add % (T1)_base + (T1)_base - 1;
               add /= (T1)base; 
            }

            i32 __base() { return _base; }
        };
    } // namespace math
} // namespace generator

#endif // !_SGPCET_BIG_INT_BASE_H_
