#ifndef _SGPCET_BIG_INT_H_
#define _SGPCET_BIG_INT_H_

#ifndef _SGPCET_BIG_INT_BASE_H_
#include "math/big_int_base.h"
#endif //!_SGPCET_BIG_INT_BASE_H_


namespace generator {
    namespace math {
        class BigInt : public BigIntCalculator<BigInt> {
        public:
            friend class BigIntCalculator<BigInt>;
            const static u32 COMPRESS_BIT = 25;
            constexpr static u32 COMPRESS_MOD = 1 << COMPRESS_BIT;
            constexpr static u32 COMPRESS_MASK = COMPRESS_MOD - 1;
        public:

            BigInt() { set_value(0);}

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            BigInt(T val) {
                set_value(val);
            }

            BigInt(const BigInt& other) : 
                BigIntCalculator<BigInt>(other) {}
            
            BigInt& operator=(const BigInt& other) {
                if (this != &other) {
                    BigIntCalculator<BigInt>::operator=(other);
                }
                return *this;
            }

            BigInt(BigInt&& other) noexcept : 
                BigIntCalculator<BigInt>(std::move(other)) {}

            BigInt& operator=(BigInt&& other) noexcept {
                if (this != &other) {
                    BigIntCalculator<BigInt>::operator=(std::move(other));
                } 
                return *this;
            }

        protected:
            template <typename T1, typename T2, typename T3>
            void __carry(T1& add, T2& basic_val, T3 new_val) {
                add += (T1)new_val;
                basic_val = (add & COMPRESS_MASK);
                add = (add >> COMPRESS_BIT);
            } 

            template <typename T1, typename T2, typename T3>
            void __borrow(T1& add, T2& basic_val, T3 new_val) {
                add += (T1)new_val;
                basic_val = (add & COMPRESS_MASK);
                add = (add >> COMPRESS_BIT);
            }

            i32 __base() const { return COMPRESS_MOD; }
            i32 __radix() const { return 2; }
            i32 __digits() const { return COMPRESS_BIT - 1; }
        };
    } // namespace math
} // namespace generator

#endif // !_SGPCET_BIG_INT_H_
