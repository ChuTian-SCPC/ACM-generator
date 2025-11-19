#ifndef _SGPCET_NUMERIC_H_
#define _SGPCET_NUMERIC_H_

#ifndef _SGPCET_NUMBER_CONST_H_
#include "rand/number_const.h"
#endif // !_SGPCET_NUMBER_CONST_H_
#ifndef _SGPCET_NUMBER_FORMAT_H_
#include "rand/number_format.h"
#endif // !_SGPCET_NUMBER_FORMAT_H_

namespace generator {
    namespace rand_numeric {
        bool rand_bool() {
            return rnd.next(2);
        }

        template<typename T>
        T __rand_int_impl(T x) {
            return rnd.next(x);
        }
        
        template<>
        unsigned int __rand_int_impl<unsigned int>(unsigned int n) {
            long long nl = n;
            long long x = rnd.next(nl);
            return (unsigned int)x;
        }
        
        template<>
        unsigned long long __rand_int_impl<unsigned long long>(unsigned long long n) {
            if (n == 0) {
                _msg::__fail_msg(_msg::_defl, "n must greater than 0.");
            }
            long long ask = (1LL << 32);
            unsigned long long limit = __UNSIGNED_LONG_LONG_MAX / n * n;
            unsigned long long z;
            do {
                long long x = rnd.next(ask);
                long long y = rnd.next(ask);
                z = ((unsigned long long)x << 32)^((unsigned long long)y);
            } while(z >= limit);
            return z % n;
        }
        
        template<>
        unsigned long __rand_int_impl<unsigned long>(unsigned long n) {
            if (__CHECK_UNSIGNED_LONG_MAX == __UNSIGNED_LONG_LONG_MAX) return (unsigned long)__rand_int_impl<unsigned long long>(n);
            else return (unsigned long)__rand_int_impl<unsigned int>(n);
        }
        
        template<typename T>
        T __rand_int_impl(T from, T to) {
            return rnd.next(from, to);
        }
        
        template<>
        unsigned long long __rand_int_impl<unsigned long long>(unsigned long long from, unsigned long long to) {
            if (from > to) {
                _msg::__fail_msg(_msg::_defl, tools::string_format("range [%llu, %llu] is not valid.", from, to));
            }
            if (from == __UNSIGNED_LONG_LONG_MIN && to == __UNSIGNED_LONG_LONG_MAX) {
                unsigned long long result = __rand_int_impl<unsigned long long>(from, to / 2);
                int x = rand_bool();
                return x ? result * 2ULL + 1 : result * 2ULL;
            }
            return __rand_int_impl<unsigned long long>(to - from + 1ULL) + from;
        }
        
        template<>
        long long __rand_int_impl<long long>(long long from, long long to) {
            if (from > to) {
                _msg::__fail_msg(_msg::_defl, tools::string_format("range [%lld, %lld] is not valid.", from, to));
            }
            if ((from < 0 && to < 0) || (from > 0 && to > 0)) {
                return rnd.next(from, to);
            }
            else {
                unsigned long long froml = (unsigned long long)from - __LONG_LONG_MIN;
                unsigned long long tol = (unsigned long long)to - __LONG_LONG_MIN;
                unsigned long long result = __rand_int_impl<unsigned long long>(froml, tol);
                if (result >= __CHECK_ABS_LONG_LONG_MIN) return (long long)(result - __CHECK_ABS_LONG_LONG_MIN);
                else return (long long)result - __CHECK_ABS_LONG_LONG_MIN;
            }
        }
        
        template<>
        unsigned long __rand_int_impl<unsigned long>(unsigned long from, unsigned long to) {
            if (__CHECK_UNSIGNED_LONG_MAX == __UNSIGNED_LONG_LONG_MAX) return (unsigned long)__rand_int_impl<unsigned long long>(from, to);
            else return (unsigned long)__rand_int_impl<unsigned int>(from, to);
        }

        template <typename T = int>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_int(T n){
            T x = __rand_int_impl<T>(n);
            return x;
        }

        template <typename T = int>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_int(T from, T to){
            T x = __rand_int_impl<T>(from, to);
            return x;
        }

        template <typename R = long long, typename T = long long, typename U = long long>
        typename std::enable_if<
            std::is_integral<R>::value &&
            std::is_convertible<T, R>::value && 
            std::is_convertible<U, R>::value, R>::type
        rand_int(T from, U to){
            R from_r = tools::__change_to_int<T, R>(from, "from");
            R to_r = tools::__change_to_int<U, R>(to, "to");
            R x = __rand_int_impl<R>(from_r, to_r);
            return x;
        }

        template <typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_int(std::string format) {
            std::pair<T, T> range = tools::__format_to_int_range<T>(format);
            T x = __rand_int_impl<T>(range.first, range.second);
            return x;
        }

        template <typename T>
        T __to_odd_need_limit(T n, bool lower) {
            if (n % 2 == 0) lower ?  n++ : n--;
            return (n - 1) / 2;
        }
         
        template <typename T>
        T __rand_odd_impl(T from, T to) {
            if (to < from || (to == from && to % 2 == 0)) {
                _msg::__fail_msg(_msg::_defl, tools::string_format("There is no odd number between [%s, %s].", 
                    std::to_string(from).c_str(), std::to_string(to).c_str()));
            }
            T l = __to_odd_need_limit(from, true);
            T r = __to_odd_need_limit(to, false);
            T v = rand_int(l, r);
            return v * 2 + 1;
        }

        template <typename T>
        T __rand_odd_impl(T n) {
            if (n <= 1) {
                _msg::__fail_msg(_msg::_defl, tools::string_format("There is no odd number between [1, %s).",
                    std::to_string(n).c_str()));
            }
            return __rand_odd_impl<T>(0, n - 1);
        }  

        template <typename T = int>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_odd(T n){
            return __rand_odd_impl<T>(n);
        }
        
        template <typename T = int>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_odd(T from, T to){
            return __rand_odd_impl<T>(from, to);
        }

        template <typename R = long long, typename T = long long, typename U = long long>
        typename std::enable_if<
            std::is_integral<R>::value &&
            std::is_convertible<T, R>::value && 
            std::is_convertible<U, R>::value, R>::type
        rand_odd(T from, U to){
            R from_r = tools::__change_to_int<T, R>(from, "from");
            R to_r = tools::__change_to_int<U, R>(to, "to");
            return __rand_odd_impl<R>(from_r, to_r);
        }

        template <typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_odd(std::string format) {
            std::pair<T, T> range = tools::__format_to_int_range<T>(format);
            return __rand_odd_impl<T>(range.first,range.second);
        }

        template <typename T>
        T __to_even_need_limit(T n, bool lower) {
            if (n % 2 != 0) lower ? n++ : n--;
            return n / 2;
        }
        
        template <typename T>
        T __rand_even_impl(T from, T to) {
            if (to < from || (to == from && to % 2 != 0)) {
                _msg::__fail_msg(_msg::_defl, tools::string_format("There is no even number between [%s, %s].", 
                    std::to_string(from).c_str(), std::to_string(to).c_str())); 
            }
            T l = __to_even_need_limit(from, true);
            T r = __to_even_need_limit(to, false);
            T v = rand_int(l, r);
            return v * 2;
        }

        template <typename T>
        T __rand_even_impl(T n) {
            if (n <= 0) {
                _msg::__fail_msg(_msg::_defl, tools::string_format("There is no even number between [0, %s).", 
                    std::to_string(n).c_str()));
            }
            return __rand_even_impl<T>(0, n - 1);
        }

        template <typename T = int>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_even(T n){
            return __rand_even_impl<T>(n);
        }
        
        template <typename T = int>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_even(T from, T to){
            return __rand_even_impl<T>(from, to);
        }

        template <typename R = long long, typename T = long long, typename U = long long>
        typename std::enable_if<
            std::is_integral<R>::value &&
            std::is_convertible<T, R>::value && 
            std::is_convertible<U, R>::value, R>::type
        rand_even(T from, U to){
            R from_r = tools::__change_to_int<T, R>(from, "from");
            R to_r = tools::__change_to_int<U, R>(to, "to");
            return __rand_even_impl<R>(from_r, to_r);
        }

        template <typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_even(std::string format) {
            std::pair<T, T> range = tools::__format_to_int_range<T>(format);
            return __rand_even_impl<T>(range.first,range.second);
        }

        double rand_real() {
            double x = rnd.next();
            return x;
        }

        template <typename R = double, typename T = double>
        typename std::enable_if<
            std::is_floating_point<R>::value &&
            std::is_convertible<T, double>::value, R>::type
        rand_real(T n) {
            double n_d = tools::__change_to_double<T, double>(n, "to");
            double x = rnd.next(n_d);
            return x;
        }

        template <typename R = double, typename T = double>
        typename std::enable_if<
            std::is_floating_point<R>::value &&
            std::is_convertible<T, double>::value, R>::type
        rand_real(T from, T to) {
            double from_d = tools::__change_to_double<T, double>(from, "from");
            double to_d = tools::__change_to_double<T, double>(to, "to");
            double x = rnd.next(from_d, to_d);
            return x;
        }

        template <typename R = double, typename T = double, typename U = double>
        typename std::enable_if<
            std::is_floating_point<R>::value &&
            std::is_convertible<T, double>::value &&
            std::is_convertible<U, double>::value, R>::type
        rand_real(T from, U to) {
            double from_d = tools::__change_to_double<T, double>(from, "from");
            double to_d = tools::__change_to_double<U, double>(to, "to");
            double x = rnd.next(from_d, to_d);
            return x;
        }

        template <typename T = double>
        typename std::enable_if<std::is_floating_point<T>::value, T>::type
        rand_real(std::string format) {
            std::pair<T, T> range = tools::__format_to_double_range<T>(format);
            double x = rnd.next(range.first, range.second);
            return x;
        }

        template <typename T>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        __rand_value(std::string s) {
            return rand_int<T>(s.c_str());
        }
        
        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, T>::type
        __rand_value(std::string s) {
            return rand_real<T>(s.c_str());
        }

        template <typename T>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        __rand_value(T n) {
            return rand_int<T>(n);
        }
        
        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, T>::type
        __rand_value(T n) {
            return rand_real<T>(n);
        }

        template <typename T>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        __rand_value(T from, T to) {
            return rand_int<T>(from, to);
        }
        
        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, T>::type
        __rand_value(T from, T to) {
            return rand_real<T>(from, to);
        }

        double rand_abs(){
            double x = rand_real();
            return rand_bool() ? x : -x;
        }

        template <typename T>
        typename std::enable_if<IsNumeric<T>::value, T>::type
        rand_abs(T n) {
            T x = __rand_value<T>(n);
            return rand_bool() ? x : -x;
        }
        
        template <typename T>
        typename std::enable_if<IsNumeric<T>::value , T>::type
        rand_abs(T from, T to) {
            T x = __rand_value<T>(from, to);
            return rand_bool() ? x : -x;
        }

        template <typename R = double, typename T, typename U>
        typename std::enable_if<
            IsNumeric<R>::value &&
            std::is_convertible<T, R>::value &&
            std::is_convertible<U, R>::value, R>::type
        rand_abs(T from, U to) {
            R from_r = tools::__change_to_value<T, R>(from, "from");
            R to_r = tools::__change_to_value<U, R>(to, "to");
            R x = __rand_value<R>(from_r, to_r);
            return rand_bool() ? x : -x;
        }

        template <typename T = double>
        typename std::enable_if<IsNumeric<T>::value, T>::type
        rand_abs(std::string format) {
            T x = __rand_value<T>(format);
            return rand_bool() ? x : -x;
        }       

        char rand_char(_enum::CharType type = _enum::LowerLetter){
            std::string s = rnd.next(_PATTERN[type]);
            return s.c_str()[0];
        }

        char rand_char(std::string format) {
            std::string s = rnd.next(format);
            if(s.empty()) {
                _msg::__fail_msg(_msg::_defl, "can't generator a char from an empty string.");
            }
            return s.c_str()[0];
        }

        template<typename KeyType>
        class ProbTable {
        private:
            std::vector<KeyType> _elements;
            std::vector<unsigned long long> _probs;
        public:
            ProbTable() = default;

            template <typename Con>
            ProbTable(const Con& map) {
                __add(map);
            }

            template<typename ValueType>
            ProbTable(KeyType key, ValueType value) {
                __add(key, value);
            }

            template<typename Con>
            void add(const Con& map) {
                __add(map);
            }

            template<typename ValueType>
            void add(const KeyType& key, ValueType value) {
                __add(key, value);
            }

            KeyType rand() const {
                if (_probs.empty()) {
                    _msg::__fail_msg(_msg::_defl, "can't generator a value from an empty ProbTable.");
                }
                unsigned long long sum = _probs.back();
                if (sum == 0) {
                    _msg::__fail_msg(_msg::_defl, "sum of the values must greater than 0, but found 0.");
                }
                long long p = rand_int(1ULL, sum);
                auto pos = lower_bound(_probs.begin(), _probs.end(), p) - _probs.begin();
                return *(_elements.begin() + pos);
            }

            void clear() {
                _elements.clear();
                _probs.clear();
            }
        private:
            template <typename Con>
            typename std::enable_if<
                (std::is_same<Con, std::map<typename Con::key_type, typename Con::mapped_type>>::value ||
                std::is_same<Con, std::unordered_map<typename Con::key_type, typename Con::mapped_type>>::value) &&
                std::is_convertible<typename Con::mapped_type, unsigned long long>::value &&
                std::is_convertible<typename Con::key_type, KeyType>::value,
                void
            >::type 
            __add(const Con& map) {
                using ValueType = typename Con::mapped_type;
                for (auto iter : map) {
                    __add(iter.first, iter.second);
                }
            }

            template <typename ValueType>
            typename std::enable_if<std::is_constructible<ValueType, unsigned long long>::value, void>::type
            __add(const KeyType& key, ValueType value) {
                unsigned long long sum = _probs.empty() ? 0ULL : _probs.back();
                if (value < 0) {
                    _msg::__warn_msg(_msg::_defl, "value can't less than 0, but found (key : ", key,
                        ", value : ", value, "). Ignore this element.");
                } else {
                    _elements.emplace_back(key);
                    sum += static_cast<unsigned long long>(value);
                    _probs.emplace_back(sum);
                }
                if (_elements.size() > _setting::vector_limit) {
                    _msg::__fail_msg(_msg::_defl, 
                        tools::string_format("the size of the ProbTable must less than %d(_setting::vector_limit), but found %d.",
                            _setting::vector_limit, _elements.size()));
                }
            }
        };

        template <typename Con>
        typename std::enable_if<
                (std::is_same<Con, std::map<typename Con::key_type, typename Con::mapped_type>>::value ||
                std::is_same<Con, std::unordered_map<typename Con::key_type, typename Con::mapped_type>>::value) &&
                std::is_integral<typename Con::mapped_type>::value,
                typename Con::key_type
        >::type
        rand_prob(const Con& map)
        {
            using KeyType = typename Con::key_type;
            using ValueType = typename Con::mapped_type;
            ProbTable<KeyType> prob(map);
            return prob.rand();
        }

        template<typename KeyType>
        KeyType rand_prob(const ProbTable<KeyType>& prob) {
            return prob.rand();
        }

        template<typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, std::pair<T, T>>::type
        rand_range(T from, T to) {
            T l = rand_int<T>(from, to);
            T r = rand_int<T>(from, to);
            if (l > r) std::swap(l, r);
            return std::make_pair(l, r);
        }

        template <typename R = long long, typename T = long long, typename U = long long>
        typename std::enable_if<
            std::is_integral<R>::value &&
            std::is_convertible<T, R>::value && 
            std::is_convertible<U, R>::value, std::pair<R, R>>::type
        rand_range(T from, U to) {
            R from_r = tools::__change_to_value<T, R>(from, "from");
            R to_r = tools::__change_to_value<U, R>(to, "to");
            return rand_range<R>(from_r, to_r);
        }
    } // namespace rand_numeric
} // namespace generator

#endif // !_SGPCET_NUMERIC_H_
