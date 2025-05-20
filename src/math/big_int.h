#ifndef _SGPCET_BIG_INT_H_
#define _SGPCET_BIG_INT_H_

#ifndef _SGPCET_BIG_INT_BASE_H_
#include "math/big_int_base.h"
#endif //!_SGPCET_BIG_INT_BASE_H_
#ifndef _SGPCET_CHAR_SET_CHECK_H_
#include "math/char_set_check.h"
#endif //!_SGPCET_CHAR_SET_CHECK_H_

namespace generator {
    namespace math {
        class BigInt : public BigIntCalculator<BigInt> {
        public:
            friend class BigIntCalculator<BigInt>;
            const static u32 COMPRESS_BIT = 25;
            constexpr static u32 COMPRESS_MOD = 1 << COMPRESS_BIT;
            constexpr static u32 COMPRESS_MASK = COMPRESS_MOD - 1;
            using FromStrFunction = std::function<void(BigInt&, const std::string&)>;
            using InputFunction = std::function<void(std::istream& is, BigInt& num)>;
        protected:
            FromStrFunction _from_str_function;
            InputFunction _input_function;
            int _in_out_base; // -1 : default(10)
        public:

            BigInt() : _in_out_base(-1) { 
                set_value(0);
                set_from_str_default();
                set_input_default();
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            BigInt(T val)  : _in_out_base(-1) {
                set_value(val);
                set_from_str_default();
                set_input_default();
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            BigInt(T val, int base)  : _in_out_base(base) {
                set_value(val);
                set_from_str_default();
                set_input_default();
            }

            BigInt(const BigInt& other) : 
                BigIntCalculator<BigInt>(other), _from_str_function(other._from_str_function), 
                _input_function(other._input_function), _in_out_base(other._in_out_base) {}
            
            BigInt& operator=(const BigInt& other) {
                if (this != &other) {
                    BigIntCalculator<BigInt>::operator=(other);
                    _from_str_function = other._from_str_function;
                    _input_function = other._input_function;
                    _in_out_base = other._in_out_base;
                }
                return *this;
            }

            BigInt(BigInt&& other) noexcept : 
                BigIntCalculator<BigInt>(std::move(other)), _from_str_function(std::move(other._from_str_function)),
                _input_function(std::move(other._input_function)), _in_out_base(std::move(other._in_out_base)) {}

            BigInt& operator=(BigInt&& other) noexcept {
                if (this != &other) {
                    BigIntCalculator<BigInt>::operator=(std::move(other));
                    _from_str_function = other._from_str_function;
                    _input_function = other._input_function;
                    _in_out_base = other._in_out_base;
                } 
                return *this;
            }

            void set_from_str(FromStrFunction from_str_function) {
                _from_str_function = from_str_function; 
            }

            void set_from_str_default() {
                _from_str_function = [](BigInt& val, const std::string& s) { val.__default_from_str(s); }; 
            }

            void from_str(const std::string& s, int base = -1) {
                _in_out_base = base;
                _from_str_function(*this, s);
            }

            void set_input(InputFunction input_function) {
                _input_function = input_function;
            }

            void set_input_default() {
                _input_function = [](std::istream& is, BigInt& num) { num.__default_input(is); };
            }

            friend std::istream& operator>>(std::istream& is, BigInt& num) {
                num._input_function(is, num);
                return is;
            }

            _SET_GET_VALUE(int, in_out_base);

        protected:
            int __parse_base(const std::string& s, int& p) {
                int n = s.size();
                if (n == 0) _msg::__fail_msg(_msg::_defl, "string with zero number can't be read.");
                if (s[p] == '0') {
                    if (p + 1 == n) return -1;
                    if (s[p + 1] == 'x') {
                        p += 2;
                        return 16; 
                    }
                    else if (s[p + 1] == 'b') {
                        p += 2;
                        return 2; 
                    } else {
                        return 8; 
                    }
                }
                return -1;
            }
            int __parse_negative(const std::string& s) {
                _is_negative = false;
                int p = 0, n = s.size();
                while(s[p] == '+' || s[p] == '-') {
                    if (s[p] == '-') _is_negative ^= 1;
                    p++;
                    if (p == n) _msg::__fail_msg(_msg::_defl, "string with zero number can't be read.");
                }
                return p;
            }

            void __judge_base() {
                if (_in_out_base == 0 || _in_out_base == 1 || _in_out_base < -1) {
                    _msg::__fail_msg(_msg::_defl, "base can't be 0 or 1 or negative.");
                }
                _setting::CharSetCheck::enable_size(_in_out_base == -1 ? 10 : _in_out_base);
            }

            static BigInt __get_base_pow(int base, int p) {
                if (!_setting::big_int_cache_pow) return BigInt::pow(base, p);
                static int origin_base = -1;
                static std::vector<BigInt> base_pow;
                if (origin_base != base) {
                    origin_base = base;
                    base_pow.clear();
                    base_pow.emplace_back(1);
                }
                if (base_pow.size() > p && base_pow[p] != 0) return base_pow[p];
                else if (base_pow.size() <= p ) base_pow.resize(p + 1);
                BigInt result(1);
                BigInt a(base);
                int b = p;
                int add = 1;
                int sum = 0;
                while (b) {
                    if (b & 1) {
                        result = result * base;
                        sum |= add;
                        base_pow[sum] = result;
                    }
                    base = base * base;
                    b >>= 1;
                    add <<= 1;
                    if (add <= _setting::vector_limit && add >= base_pow.size()) base_pow.resize(add + 1);
                    base_pow[add] = base;
                }

                base_pow[p] = BigInt::pow(base, p);
                return base_pow[p];
            }

            BigInt __from_string(const std::string& s, int start, int end, int base, int limit) {
                if (end - start < limit) {
                    i64 num = 0;
                    auto& mp = _setting::BigNumberSetting::labels_map();
                    for (int i = start; i < end; i++) {
                        num *= base;
                        int v;
                        if (_setting::big_int_valid_digits) {
                            auto it = mp.find(std::string(1, s[i]));
                            if (it == mp.end()) {
                                _msg::__fail_msg(_msg::_defl, tools::string_format("string with invalid label %c can't be read.", s[i]));
                            }
                            v = it->second;
                            if (v >= base) {
                                _msg::__fail_msg(_msg::_defl, tools::string_format("string with invalid number %d can't be read.", v));
                            }
                        }
                        else v = mp[std::string(1, s[i])];
                        num += v;
                    }
                    auto res =  BigInt(num);
                    return res;
                }                    
                int mid = (start + end) / 2;
                BigInt high = __from_string(s, start, mid, base, limit);
                BigInt low = __from_string(s, mid, end, base, limit);
                auto res = high * __get_base_pow(base, end - mid) + low;
                return res;
            }

            void __from_string(BigInt& val, const std::string& s, int start, int end, int base, int limit) {
                val.data_ref() = std::move(__from_string(s, start, end, base, limit).data());
            }

            void __from_string_pow2(BigInt& val, const std::string& s, int base) {
                int t = __calculate_pow2(base);
                int n = s.size();
                int bit = __digits();
                u64 add = 0;
                int p = 0;
                int j = 0;  
                auto& mp = _setting::BigNumberSetting::labels_map();
                _data.resize(n * t / bit);
                for (int i = n - 1; i >= 0; i--) {
                    u64 v;
                    if (_setting::big_int_valid_digits) {
                        auto it = mp.find(std::string(1, s[i]));
                        if (it == mp.end()) {
                            _msg::__fail_msg(_msg::_defl, tools::string_format("string with invalid label %c can't be read.", s[i]));
                        }
                        v = it->second;
                        if (v >= base) {
                            _msg::__fail_msg(_msg::_defl, tools::string_format("string with invalid number %d can't be read.", v));
                        }
                    }
                    else v = mp[std::string(1, s[i])];
                    add += (v << p);
                    p += t;
                    if (p >= bit) {
                        _data.emplace_back(add & COMPRESS_MASK);
                        p -= bit;
                        add >>= bit;
                    }
                }
                if (add) _data.emplace_back(add);
            }

            void __default_from_str(const std::string& s) {
                __judge_base();
                _setting::CharSetCheck::enable_default_read();
                int p = __parse_negative(s);
                if (_setting::big_int_parse_prefix) {
                    int b = b = __parse_base(s, p);
                    if (b != -1 && _in_out_base != -1 && _in_out_base!= b) {
                        _msg::__fail_msg(_msg::_defl, "base is not match.");
                    }                    
                }

                _data.clear();
                int actual_base = _in_out_base == -1 ? 10 : _in_out_base;
                if (s.empty()) _msg::__fail_msg(_msg::_defl, "empty string can't be read.");
                if (BigInt::__is_pow2(actual_base)) __from_string_pow2(*this, s, actual_base);
                else {
                    int limit = __calculate_limit(actual_base);
                    __from_string(*this, s, 0, s.size(), actual_base, limit);
                }
            }

            int __calculate_limit(int base) {
                int cut = __base();
                int limit = 0;
                while(cut >= base) {
                    cut /= base;
                    limit++;
                }
                return limit;
            }

            int __calculate_pow2(int base) {
                int x = 0;
                while(base % 2 == 0) {
                    base >>= 1;
                    x++;
                }
                return x;
            }

            void __default_input(std::istream& is) {
                std::string s;
                is >> s;
                from_str(s, _in_out_base);
            }

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
            i32 __digits() const { return COMPRESS_BIT; }
        };
    } // namespace math
} // namespace generator

#endif // !_SGPCET_BIG_INT_H_
