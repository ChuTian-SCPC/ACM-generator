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
            const static u32 COMPRESS_BIT = 20;
            constexpr static u32 COMPRESS_MOD = 1 << COMPRESS_BIT;
            constexpr static u32 COMPRESS_MASK = COMPRESS_MOD - 1;
            using FromStrFunction = std::function<void(BigInt&, const std::string&)>;
            using InputFunction = std::function<void(std::istream& is, BigInt& num)>;
            using ToStrFunction = std::function<std::string(const BigInt& num, int)>;
            using OutputFunction = std::function<void(std::ostream& os, const BigInt& num)>;
        protected:
            FromStrFunction _from_str_function;
            InputFunction _input_function;
            ToStrFunction _to_str_function;
            OutputFunction _output_function;
            int _in_out_base; // -1 : default(10)
        public:

            BigInt() : _in_out_base(-1) { 
                set_value(0);
                set_from_str_default();
                set_input_default();
                set_to_str_default();
                set_output_default();
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            BigInt(T val)  : _in_out_base(-1) {
                set_value(val);
                set_from_str_default();
                set_input_default();
                set_to_str_default();
                set_output_default();
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            BigInt(T val, int base)  : _in_out_base(base) {
                set_value(val);
                set_from_str_default();
                set_input_default();
                set_to_str_default();
                set_output_default();
            }

            BigInt(const std::string& s, int base = -1) : _in_out_base(base) {
                set_from_str_default();
                set_input_default();
                set_to_str_default();
                set_output_default();
                from_str(s, base);
            }

            BigInt(const BigInt& other) : 
                BigIntCalculator<BigInt>(other), 
                _from_str_function(other._from_str_function), 
                _input_function(other._input_function), 
                _to_str_function(other._to_str_function),
                _output_function(other._output_function),
                _in_out_base(other._in_out_base) {}
            
            BigInt& operator=(const BigInt& other) {
                if (this != &other) {
                    BigIntCalculator<BigInt>::operator=(other);
                    _from_str_function = other._from_str_function;
                    _input_function = other._input_function;
                    _to_str_function = other._to_str_function;
                    _output_function = other._output_function;
                    _in_out_base = other._in_out_base;
                }
                return *this;
            }

            BigInt(BigInt&& other) noexcept : 
                BigIntCalculator<BigInt>(std::move(other)), 
                _from_str_function(std::move(other._from_str_function)),
                _input_function(std::move(other._input_function)), 
                _to_str_function(std::move(other._to_str_function)),
                _output_function(std::move(other._output_function)),
                _in_out_base(std::move(other._in_out_base)) {}

            BigInt& operator=(BigInt&& other) noexcept {
                if (this != &other) {
                    BigIntCalculator<BigInt>::operator=(std::move(other));
                    _from_str_function = std::move(other._from_str_function);
                    _input_function = std::move(other._input_function);
                    _to_str_function = std::move(other._to_str_function);
                    _output_function = std::move(other._output_function);
                    _in_out_base = std::move(other._in_out_base);
                } 
                return *this;
            }

            BigInt& operator=(const std::string& str) {
                from_str(str, _in_out_base);
                return *this;
            }

            BigInt& operator=(const char* str) {
                from_str(str, _in_out_base);
                return *this;
            }

            template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
            BigInt& operator=(T val) {
                set_value(val);
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

            void set_to_str(ToStrFunction to_str_function) {
                _to_str_function = to_str_function;
            }

            void set_to_str_default() {
                _to_str_function = [](const BigInt& num, int base) { return num.__default_to_str(base); };
            }

            std::string to_str(int base = -1) const {
                return _to_str_function(*this, base);
            }

            void set_output(OutputFunction output_function) {
                _output_function = output_function; 
            }

            void set_output_default() {
                _output_function = [](std::ostream& os, const BigInt& num) { num.__default_output(os); }; 
            }

            friend std::ostream& operator<<(std::ostream& os, const BigInt& num) {
                num._output_function(os, num);
                return os; 
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
                        result = result * a;
                        sum |= add;
                        base_pow[sum] = result;
                    }
                    a = a * a;
                    b >>= 1;
                    add <<= 1;
                    if (add <= _setting::vector_limit) {
                        if (add >= base_pow.size()) base_pow.resize(add + 1);
                        base_pow[add] = a;
                    }
                }

                base_pow[p] = result;
                return base_pow[p];
            }

            BigInt __from_string(const std::string& s, int start, int end, int base, int limit) {
                if (end - start < limit) {
                    i64 num = 0;
                    auto& mp = _setting::BigNumberSetting::labels_map();
                    for (int i = start; i < end; i++) {
                        num *= base;
                        int v = mp[std::string(1, s[i])];
                        if (v >= base) _msg::__fail_msg(_msg::_defl, 
                            tools::string_format("string with out base(%d) character %c(%d) can't be read.", base, s[i], v));
                        num += v;
                    }
                    auto res = BigInt(num);
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

            void __from_string_pow2(BigInt& val, const std::string& s, int start, int base) {
                int t = __calculate_pow2(base);
                int n = s.size();
                int bit = __digits();
                u64 add = 0;
                int p = 0;
                auto& mp = _setting::BigNumberSetting::labels_map();
                for (int i = n - 1; i >= start; i--) {
                    u64 v = mp[std::string(1, s[i])];
                        if (v >= base) _msg::__fail_msg(_msg::_defl, 
                            tools::string_format("string with out base(%d) character %c(%d) can't be read.", base, s[i], v));
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
                _setting::CharSetCheck::enable_default_read();
                int p = __parse_negative(s);
                int actual_base = _in_out_base == -1 ? 10 : _in_out_base;
                if (_setting::big_int_parse_prefix) {
                    int b = __parse_base(s, p);
                    if (b != -1 && _in_out_base != -1 && _in_out_base != b) {
                        _msg::__fail_msg(_msg::_defl, "base is not match.");
                    }
                    if (b != -1) {
                        actual_base = b;
                        _in_out_base = b;
                    }   
                }

                _data.clear();     
                _setting::CharSetCheck::enable_base(actual_base);
                if (s.empty()) _msg::__fail_msg(_msg::_defl, "empty string can't be read.");
                if (BigInt::__is_pow2(actual_base)) __from_string_pow2(*this, s, p, actual_base);
                else {
                    int limit = __calculate_limit(actual_base);
                    __from_string(*this, s, p, s.size(), actual_base, limit);
                }
            }

            std::string __to_string_pow2(int base) const {
                int t = __calculate_pow2(base);
                int bit = __digits();
                int n = _data.size();
                std::vector<i64> res;
                i64 add = 0;
                i64 mask = base - 1;
                auto& digits = _setting::BigNumberSetting::labels();
                for (int i = 0, j = 0;;) {
                    if (j < t) {
                        if (i < n) add += ((i64)_data[i] << j);
                        else if (add == 0) break;
                        j += bit;
                        i++;
                    }
                    res.push_back(add & mask);
                    add >>= t;
                    j -= t;
                } 
                while(res.size() > 1 && res.back() == 0) res.pop_back();
                if (res.size() == 1 && res.back() == 0) return "0";
                std::ostringstream oss;
                if (_is_negative) oss << '-';
                for (int i = res.size() - 1; i >= 0; i--) oss << digits[res[i]];
                return oss.str();
            }

            static BigIntBase __get_out_base_pow(int out_base, int p) {
                static std::vector<BigIntBase> out_base_pow;
                static int origin_base = -1;
                if (origin_base!= out_base) {
                    origin_base = out_base;
                    out_base_pow.clear();
                    BigIntBase base;
                    base.set_radix(out_base);
                    base.set_value(1);
                    out_base_pow.emplace_back(base);
                }
                if (out_base_pow.size() > p && out_base_pow[p]!= 0) return out_base_pow[p];
                else if (out_base_pow.size() <= p ) out_base_pow.resize(p + 1);
                BigIntBase result, mul;
                result.set_radix(out_base);
                result.set_value(1);
                mul.set_radix(out_base);
                mul.set_value(COMPRESS_MOD);
                int add = 1;
                int sum = 0;
                while (p) {
                    if (p & 1) {
                        result *= mul;
                        sum |= add;
                        out_base_pow[sum] = result;
                    }
                    mul *= mul;
                    p >>= 1;
                    add <<= 1;
                    if (add <= _setting::vector_limit) {
                        if (add >= out_base_pow.size()) out_base_pow.resize(add + 1);
                        out_base_pow[add] = mul;
                    }
                }
                out_base_pow[p] = result;
                return out_base_pow[p];
            }

            BigIntBase transbase(int out_base) const {
                if (size() <= 8) {
                    BigIntBase sum, base, own_base, mul, t;
                    sum.set_radix(out_base);
                    base.set_radix(out_base);
                    own_base.set_radix(out_base);
                    mul.set_radix(out_base);
                    t.set_radix(out_base);
                    own_base.set_value(COMPRESS_MOD);
                    base.set_value(1);
                    for (int i = 0; i < size(); i++) {
                        t.set_value(_data[i]);
                        mul = base * t;
                        sum += mul;
                        base *= own_base;
                    }
                    return sum;
                } else {
                    int mid = size() / 2;
                    BigInt low = BigInt(*this);
                    low._data.resize(mid);
                    BigInt high = BigInt(*this);
                    auto& hdata = high._data;
                    hdata.assign(hdata.begin() + mid, hdata.end());
                    auto low_res = low.transbase(out_base);
                    auto high_res = high.transbase(out_base);
                    return high_res * __get_out_base_pow(out_base, mid) + low_res;
                }
            }

            std::string __to_string(int base) const {
                auto sum = transbase(base);
                std::vector<int> res;
                i64 add = 0;
                int n = sum.size();
                int d = sum.digits();
                auto& data = sum.data_ref();
                for (int i = 0, j = 0;;) {
                    if (j < 1) {
                        if (i < n) add += data[i];
                        else if (add == 0) break;
                        j += d;
                        i++;
                    }
                    res.push_back(add % base);
                    add /= base;
                    j--;
                }
                while(res.size() > 1 && res.back() == 0) res.pop_back();
                if (res.size() == 1 && res.back() == 0) return "0";
                std::ostringstream oss;
                if (_is_negative) oss << '-';
                auto& digits = _setting::BigNumberSetting::labels();
                for (int i = res.size() - 1; i >= 0; i--) oss << digits[res[i]];
                return oss.str();
            }

            std::string __default_to_str(int base) const {
                int out_base;
                if (base == -1) out_base = _in_out_base == -1 ? 10 : _in_out_base;
                else out_base = base;
                _setting::CharSetCheck::enable_base(out_base);
                _setting::CharSetCheck::enable_default_write();
                if (__is_zero()) return "0";
                if (BigInt::__is_pow2(out_base)) return __to_string_pow2(out_base);
                else return __to_string(out_base);
            }

            void __default_output(std::ostream& os) const {
                os << __default_to_str(_in_out_base == -1 ? 10 : _in_out_base);
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

            int __calculate_pow2(int base) const {
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

            void __copy_status(const BigInt& other) {
                _in_out_base =  other._in_out_base;
            }
        };
    } // namespace math
} // namespace generator

#endif // !_SGPCET_BIG_INT_H_
