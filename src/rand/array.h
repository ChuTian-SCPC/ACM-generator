#ifndef _SGPCET_ARRAY_H_
#define _SGPCET_ARRAY_H_

#ifndef _SGPCET_NUMERIC_H_
#include "rand/numeric.h"
#endif // !_SGPCET_NUMERIC_H_
#ifndef _SGPCET_SETTING_H_
#include "basic/setting.h"
#endif // !_SGPCET_SETTING_H_

namespace generator {
    namespace rand_array {
        void __judge_vector_lower_bound(int to, std::string type) {
            if (to < 0) {
                _msg::__fail_msg(_msg::_defl,
                    tools::string_format("size of the %s can't less than 0, but found %d.", type.c_str(), to));
            }
        }

        void __judge_vector_upper_bound(int from, std::string type) {
            if (from > _setting::vector_limit) {
                _msg::__fail_msg(_msg::_defl,
                    tools::string_format("size of the %s can't greater than the vector_limit(%d), but found %d.", 
                    type.c_str(), _setting::vector_limit, from));
            }
        }

        template <typename T>
        void __judge_range(T from, T to) {
            if (from > to) {
                _msg::__fail_msg(_msg::_defl, 
                    tools::string_format("invalid range [%s, %s], to can't less than from.",
                    std::to_string(from).c_str(), std::to_string(to).c_str()));
            }
        }

        void __judge_count_range(long long from, long long to) {
            if (from == _setting::count_range_inf) _msg::__fail_msg(_msg::_defl, "from can't be inf");
            if (from < 0) _msg::__fail_msg(_msg::_defl, tools::string_format("from can't be less than 0, but found %lld.", from));
            if (to != _setting::count_range_inf && from > to) {
                _msg::__fail_msg(_msg::_defl, 
                    tools::string_format("invalid range [%s, %s], to can't less than from.",
                    std::to_string(from).c_str(), std::to_string(to).c_str()));
            }
        }

        std::string rand_string(int n, _enum::CharType type = _enum::LowerLetter) {
            __judge_vector_lower_bound(n, "string");
            __judge_vector_upper_bound(n, "string");
            std::string s = rnd.next("%s{%d}", rand_numeric::_PATTERN[type].c_str(), n);
            return s;
        }

        std::string rand_string(int from, int to, _enum::CharType type = _enum::LowerLetter) {
            __judge_range(from, to);
            __judge_vector_lower_bound(to, "string");
            __judge_vector_upper_bound(from, "string");
            std::string s = rnd.next("%s{%d,%d}", rand_numeric::_PATTERN[type].c_str(), from, to);
            return s;
        }

        std::string rand_string(int n, std::string format){
            __judge_vector_lower_bound(n, "string");
            __judge_vector_upper_bound(n, "string");
            std::string s = rnd.next("%s{%d}", format.c_str() ,n);
            return s;
        }

        std::string rand_string(int from, int to, std::string format){
            __judge_range(from, to);
            __judge_vector_lower_bound(to, "string");
            __judge_vector_upper_bound(from, "string");
            std::string s = rnd.next("%s{%d,%d}", format.c_str(), from, to);
            return s;
        }

        std::string rand_string(std::string format) {
            return rnd.next(format);
        }

        template <typename T>
        std::vector<T> rand_p(T n){
            __judge_vector_lower_bound(n, "vector");
            __judge_vector_upper_bound(n, "vector");
            std::vector<T> v = rnd.perm(n, T(0));
            return v;
        }

        template <typename T,typename E>
        std::vector<E> rand_p(T n, E s){
            __judge_vector_lower_bound(n, "vector");
            __judge_vector_upper_bound(n, "vector");
            std::vector<E> v = rnd.perm(n, s);
            return v;
        }

        template<typename T>
        typename std::enable_if<std::is_integral<T>::value , std::vector<T> >::type
        rand_sum(int size,T sum) {
            __judge_vector_lower_bound(size, "vector");
            __judge_vector_upper_bound(size, "vector");
            auto v = rnd.partition(size,sum);
            return v;
        }

        template<typename T>
        typename std::enable_if<std::is_integral<T>::value , std::vector<T> >::type
        rand_sum(int size,T sum,T min_part) {
            __judge_vector_lower_bound(size, "vector");
            __judge_vector_upper_bound(size, "vector");
            __judge_range(T(0), min_part);
            auto v = rnd.partition(size,sum,min_part);
            return v;
        }
        
        template<typename T>
        void __rand_small_sum(std::vector<T>&v, T sum, T limit) {
            int size = v.size();
            std::vector<int> rand_v(size);
            for(int i = 0; i < size; i++)
                rand_v[i] = i;
            shuffle(rand_v.begin(), rand_v.end());
            int last = size - 1;
            for (int i = 0; i <= last; i++) {
                if (v[rand_v[i]] >= limit) {
                    std::swap(rand_v[last], rand_v[i]);
                    last--;
                }
            }
            while(sum--) {
                int rand_pos = rnd.next(0, last);
                int add_pos = rand_v[rand_pos];
                v[add_pos]++;
                if(v[add_pos] >= limit) {
                    std::swap(rand_v[last], rand_v[rand_pos]);
                    last--;
                }
            }
            return;
        }
        
        template<typename T>
        bool __rand_large_sum(std::vector<T>&v, T& sum, T limit) {
            int size = v.size();
            std::vector<int> rand_v(size);
            for(int i = 0; i < size; i++)
                rand_v[i] = i;
            shuffle(rand_v.begin(), rand_v.end());
            int last = size - 1;
            T once_add_base = sum / _setting::_rand_sum_sum_limit;
            while(sum > _setting::_rand_sum_sum_limit) {
                int rand_pos, add_pos, once_add;
                do {
                    rand_pos = rnd.next(0, last);
                    add_pos = rand_v[rand_pos];
                    // 95% ~ 105% for once add
                    T once_add_l = std::max(T(0), once_add_base - once_add_base / 20);
                    T once_add_r = std::min(once_add_base + once_add_base / 20, sum);
                    if (once_add_l > once_add_r) {
                        return sum > _setting::_rand_sum_sum_limit;
                    }
                    once_add = rnd.next(once_add_l, once_add_r);
                }while(v[add_pos] + once_add > limit);         
                v[add_pos] += once_add;
                sum -= once_add;
                if(v[add_pos] >= limit) {
                    std::swap(rand_v[last], rand_v[rand_pos]);
                    last--;
                }
            }
            return false;
        }
        
        template<typename T>
        typename std::enable_if<std::is_integral<T>::value , std::vector<T> >::type
        rand_sum(int size, T sum, T from, T to) {
            __judge_vector_lower_bound(size, "vector");
            __judge_vector_upper_bound(size, "vector");
            __judge_range(from, to);
            if(size * from > sum || size * to < sum){
                _msg::__fail_msg(_msg::_defl,
                    tools::string_format("sum of the vector is in range [%lld, %lld], but need sum = %s.",
                    (long long)from * size, (long long)to * size, std::to_string(sum).c_str()));
            }
            if(size == 0) {
                if(sum != 0){
                    _msg::__fail_msg(_msg::_defl, "sum of the empty vector must be 0.");
                }
                return std::vector<T>();
            }

            T ask_sum = sum;

            std::vector<T> v(size,0);
            sum -= from * size;
            T limit = to - from;
            if (sum <= _setting::_rand_sum_sum_limit) {
                __rand_small_sum(v, sum, limit);
            }
            else if (limit * size - sum <= _setting::_rand_sum_sum_limit){
                __rand_small_sum(v, limit * size - sum, limit);
                for (int i = 0; i < size; i++) {
                    v[i] = limit - v[i];
                }
            }
            else {
                while(__rand_large_sum(v, sum, limit));
                __rand_small_sum(v, sum, limit);
            }
            for (int i = 0; i < size; i++) {
                v[i] += from;
            }

            T result_sum = 0;
            for(int i = 0;i < size;i++){
                if(v[i] < from || v[i] > to){
                    _msg::__error_msg(_msg::_defl, 
                        tools::string_format("the %d%s number %s is out of range [%s, %s], please notice author to fix bug.",
                        i + 1, englishEnding(i + 1).c_str(), std::to_string(v[i]).c_str(), 
                        std::to_string(from).c_str(), std::to_string(to).c_str()));
                }
                result_sum += v[i];
            }
            if (result_sum != ask_sum){
                _msg::__error_msg(_msg::_defl,
                    tools::string_format("sum of the vector is equal to %s, not %s, please notice author to fix bug.",
                    std::to_string(result_sum).c_str(), std::to_string(ask_sum).c_str()));
            }
            return v;
        }

        template <typename R = long long, typename S, typename T, typename U>
        typename std::enable_if<
            std::is_integral<R>::value &&
            std::is_convertible<S, R>::value && 
            std::is_convertible<T, R>::value && 
            std::is_convertible<U, R>::value, std::vector<R> >::type
        rand_sum(int size, S sum, T from, U to) {
            __judge_vector_lower_bound(size, "vector");
            __judge_vector_upper_bound(size, "vector");
            R sum_r = rand_numeric::__change_to_int<S, R>(sum, "sum");
            R from_r = rand_numeric::__change_to_int<T, R>(from, "from");
            R to_r = rand_numeric::__change_to_int<U, R>(to, "to");
            __judge_range(from_r, to_r);
            return rand_sum<R>(size, sum_r, from_r, to_r);
        }

        template <typename T>
        std::vector<T> rand_vector(int size, std::function<T()> func) {
            __judge_vector_lower_bound(size, "vector");
            __judge_vector_upper_bound(size, "vector");
            std::vector<T> v(size);
            for(int i = 0; i < size; i++){
                T x = func();
                v[i] = x;
            }
            return v;
        }

        template <typename T>
        std::vector<T> rand_vector(int from, int to, std::function<T()> func) {
            __judge_range(from, to);
            __judge_vector_lower_bound(to, "vector");
            __judge_vector_upper_bound(from, "vector");
            int size = rnd.next(from, to);
            return rand_vector(size, func);
        }

        template<typename T>
        typename std::enable_if<std::is_integral<T>::value, std::vector<T> >::type
        rand_vector(int size, T from, T to, _enum::VectorOrder order = _enum::VectorOrder::Random, _enum::VectorUniqueness uniqueness = _enum::VectorUniqueness::Duplicate) {
            __judge_range(from, to);
            std::vector<T> v;
            if (uniqueness == _enum::VectorUniqueness::Distinct)  v = rnd.distinct(size, from, to);
            else v = rand_vector<T>(size, [&from, &to]() { return rand_numeric::rand_int(from, to); });
            if (order == _enum::VectorOrder::Ascending) sort(v.begin(), v.end());
            else if (order == _enum::VectorOrder::Descending) sort(v.begin(), v.end(), std::greater<T>());
            return v;
        }

        template<typename R = long long, typename T, typename U>
        typename std::enable_if<std::is_integral<R>::value &&
            std::is_convertible<T, R>::value && 
            std::is_convertible<U, R>::value, std::vector<R> >::type
        rand_vector(int size, T from, U to, _enum::VectorOrder order = _enum::VectorOrder::Random, _enum::VectorUniqueness uniqueness = _enum::VectorUniqueness::Duplicate) {
            R from_r = rand_numeric::__change_to_int<T, R>(from, "from");
            R to_r = rand_numeric::__change_to_int<U, R>(to, "to");
            return rand_vector<R>(size, from_r, to_r, order, uniqueness);
        }

        template<typename T>
        class CountRange {
        private:
            T _value;
            long long _from, _to;
        public:
            CountRange(T value, long long from = 0LL, long long to = _setting::count_range_inf) : _value(value), _from(from), _to(to) {}

            _SET_GET_VALUE(T, value)
            _SET_GET_VALUE(long long, from)
            _SET_GET_VALUE(long long, to)

            void set_range(long long from, long long to) {
                _from = from;
                _to = to;
            }

            void set_range(std::string s) {
                auto p = rand_numeric::__format_to_int_range<long long>(s);
                set_range(p.first, p.second);
            }

        };

        template <typename T>
        std::vector<T> rand_vector(int size, std::vector<CountRange<T>> limits) {
            __judge_vector_lower_bound(size, "vector");
            __judge_vector_upper_bound(size, "vector");
            long long lower_bound = 0;
            long long upper_bound = 0;
            std::vector<int> result;
            std::vector<std::pair<T, long long>> vars;
            for (auto& p : limits) {
                long long from = p.from();
                long long to = p.to();
                T value = p.value();
                __judge_count_range(from, to);
                lower_bound += from;
                if (upper_bound != _setting::count_range_inf) {
                    if (to != _setting::count_range_inf) upper_bound += to;
                    else upper_bound = _setting::count_range_inf;
                }
                if (lower_bound > size) {
                    _msg::__fail_msg(_msg::_defl, 
                        tools::string_format("sum of count range from must less than or equal to vector size %d, but found %lld.", size, lower_bound));
                }
                for(int i = 0; i < from; i++){
                    result.push_back(value);
                }
                if (to == _setting::count_range_inf) vars.emplace_back(value, _setting::count_range_inf);
                else if (to - from > 0) vars.emplace_back(value, to - from);
            }
            if (upper_bound != _setting::count_range_inf && upper_bound < size) {
                _msg::__fail_msg(_msg::_defl, 
                    tools::string_format("sum of count range to must greater than or equal to vector size %d, but found %lld.", size, upper_bound));
            }
            int n = vars.size();
            int sum = size - lower_bound;
            while(sum--) {
                int x = rand_numeric::rand_int(n);
                result.push_back(vars[x].first);
                if (vars[x].second != _setting::count_range_inf) {
                    vars[x].second--;
                    if (vars[x].second == 0) {
                        std::swap(vars[x], vars[n - 1]);
                        n--;
                    }
                }
            }
            shuffle(result.begin(), result.end());
            return result;   
        }
       
        template<typename Iter>
        std::vector<int> shuffle_index(Iter begin, Iter end, int offset = 0) {
            int tot = 0;
            std::vector<int> res;
            for (Iter i = begin; i != end; i++) {
                int x = *i;
                if (x < 0) _msg::__fail_msg(_msg::_defl, "elements must be non negtive number.");
                tot += x;
                __judge_vector_upper_bound(tot, "vector");
                while (x--) res.emplace_back((i - begin) + offset);
            }
            shuffle(res.begin(),res.end());
            return res;
        }

        std::vector<int> shuffle_index(std::vector<int> v, int offset = 0) {
            return shuffle_index(v.begin(), v.end(), offset);
        }

        std::string __rand_palindrome_impl(int n, int p, std::string char_type) {
            __judge_vector_lower_bound(n, "string");
            __judge_vector_upper_bound(n, "string");
            if (p < 0) {
                 _msg::__fail_msg(_msg::_defl,
                 tools::string_format("palindrome part length must be a non-negative integer, but found %d.", n));
            }
            if (p > n) {
                _msg::__fail_msg(_msg::_defl,
                tools::string_format("palindrome length must less than or equal to string length %d, but found %d.", n, p));
            }
            std::string palindrome_part(p, ' ');
            for (int i = 0; i < (p + 1) / 2; i++) {
                char c = rand_numeric::rand_char(char_type);
                palindrome_part[i] = c;
                palindrome_part[p - i - 1] = c;
            }
            std::string result(n, ' ');
            int pos_l = rnd.next(0, n - p);
            int pos_r = pos_l + p - 1;
            for (int i = 0; i < n; i++) {
                if (i < pos_l || i > pos_r) result[i] = rand_numeric::rand_char(char_type);
                else result[i] = palindrome_part[i - pos_l];
            }
            return result;
        }

        std::string rand_palindrome(int n, int p, _enum::CharType type = _enum::LowerLetter) {
            return __rand_palindrome_impl(n, p, rand_numeric::_PATTERN[type]);
        }

        std::string rand_palindrome(int n, int p, std::string format) {
            return __rand_palindrome_impl(n, p, format);
        }    
        
        // see https://www.luogu.com.cn/article/f3xefa5v
        std::string rand_bracket_seq(int len, std::string brackets = "()") {
            __judge_vector_lower_bound(len, "string");
            __judge_vector_upper_bound(len, "string");
            if (len < 0 || len % 2) {
                _msg::__fail_msg(_msg::_defl, 
                    tools::string_format("length must be positive even number, but found %d.", len));
            }
            std::stack<int> st;
            std::string open = "";
            std::string close = "";
            int n = brackets.size();
            if (n == 0 || n % 2) {
                _msg::__fail_msg(_msg::_defl, 
                    tools::string_format("bracket must appear in pairs and the length must be greater than 0, but found %d.", n));
            }
            for (int i = 0; i < n; i++) {
                if (i % 2 == 0) open += brackets[i];
                else close += brackets[i];
            }
            std::vector<bool> arr(len, false); // 0 : open ; 1 : close
            for (int i = 0; i < len / 2; i++) arr[i] = true;
            shuffle(arr.begin(), arr.end());
            int start = 0, end = len;
            bool has_invaild = true;
            while (has_invaild) {
                has_invaild = false;
                int sum = 0;
                for (int i = start; i < end; i++) {
                    sum += arr[i] ? -1 : 1; 
                    if (sum >= 0) continue;
                    for (int j = i + 1; j < end; j++) {
                        sum += arr[j] ? -1 : 1;
                        if (sum < 0) continue;
                        int l = j - i - 1;
                        std::rotate(arr.begin() + i + 1, arr.begin() + j + 1, arr.begin() + end);
                        std::copy_backward(arr.begin() + end - l - 1, arr.begin() + end - 1, arr.begin() + end);
                        std::transform(arr.begin() + end - l, arr.begin() + end, arr.begin() + end - l, std::logical_not());
                        arr[i] = 0; 
                        arr[end - l - 1] = 1;
                        start = i + 1; 
                        end = end - l - 1;
                        has_invaild = true;
                        break;
                    }
                }
            }
            std::string res(len, ' ');
            for (int i = 0; i < len; i++) {
                if (arr[i] == 0) {
                    int p = rand_numeric::rand_int(n / 2);
                    res[i] = open[p];
                    st.push(p);
                } else {
                    res[i] = close[st.top()];
                    st.pop();
                }
            }
            return res;
        }
        
        std::string rand_bracket_seq(int from, int to, std::string brackets = "()") {
            __judge_range(from, to);
            __judge_vector_lower_bound(from, "string");
            __judge_vector_upper_bound(to, "string");
            int len = rand_numeric::rand_even(from, to);
            return rand_bracket_seq(len, brackets);
        }

        template<typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, std::vector<std::pair<T, T>>>::type
        rand_range_query(int q, T from, T to) {
            __judge_vector_lower_bound(q, "vector");
            __judge_vector_upper_bound(q, "vector");
            std::vector<std::pair<T, T>> res(q);
            for (int i = 0; i < q; i++) res[i] = rand_numeric::rand_range<T>(from, to);
            return res;
        }

        template<typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, std::vector<std::pair<T, T>>>::type
        rand_range_query(int q, T from, T to, T lower, T upper) {
            __judge_vector_lower_bound(q, "vector");
            __judge_vector_upper_bound(q, "vector");
            long long range = to - from + 1;
            if (range * q < lower || lower < q) _msg::__fail_msg(_msg::_defl,
                tools::string_format("lower must in range [%d, %lld], but found %lld.", q, range, lower));
            __judge_range(lower, upper);
            if (range * q < upper || upper < q) _msg::__fail_msg(_msg::_defl,
                tools::string_format("upper must in range [%d, %lld], but found %lld.", q, range, upper));
            T sum = rand_numeric::rand_int(lower, upper);
            auto ranges = rand_sum(q, sum, 0, range);
            std::vector<std::pair<T, T>> res(q);
            for (int i = 0; i < q; i++) {
                T l = rand_numeric::rand_int<T>(from, (T)(to - ranges[i] + 1));
                res[i] = std::make_pair(l, l + ranges[i] - 1);
            }
            return res;
        }

        template<typename R = long long, typename T = long long, typename U = long long>
        typename std::enable_if<
            std::is_integral<R>::value &&
            std::is_convertible<T, R>::value && 
            std::is_convertible<U, R>::value, std::vector<std::pair<R, R>>>::type
        rand_range_query(int q, T from, U to) {
            R from_r = rand_numeric::__change_to_value<T, R>(from, "from");
            R to_r = rand_numeric::__change_to_value<U, R>(to, "to");
            return rand_range_query<R>(q, from_r, to_r);
        }

        template<typename R = long long, typename T = long long, typename U = long long, typename F = long long, typename S = long long>
        typename std::enable_if<
            std::is_integral<R>::value &&
            std::is_convertible<T, R>::value && 
            std::is_convertible<U, R>::value &&
            std::is_convertible<F, R>::value &&
            std::is_convertible<S, R>::value, std::vector<std::pair<R, R>>>::type
        rand_range_query(int q, T from, U to, F lower, S upper) {
            R from_r = rand_numeric::__change_to_value<T, R>(from, "from");
            R to_r = rand_numeric::__change_to_value<U, R>(to, "to");
            R lower_r = rand_numeric::__change_to_value<F, R>(lower, "lower");
            R upper_r = rand_numeric::__change_to_value<S, R>(upper, "upper");
            return rand_range_query<R>(q, from_r, to_r, lower_r, upper_r);
        }

        template<typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, std::vector<std::pair<T, T>>>::type
        rand_range_query(int q, T from, T to, T lower) {
            T upper = q * (to - from + 1);
            return rand_range_query<T>(q, from, to, lower, upper);
        }

        template<typename R = long long, typename T = long long, typename U = long long, typename F = long long>
        typename std::enable_if<
            std::is_integral<R>::value &&
            std::is_convertible<T, R>::value && 
            std::is_convertible<U, R>::value &&
            std::is_convertible<F, R>::value , std::vector<std::pair<R, R>>>::type
        rand_range_query(int q, T from, U to, F lower) {
            R from_r = rand_numeric::__change_to_value<T, R>(from, "from");
            R to_r = rand_numeric::__change_to_value<U, R>(to, "to");
            R lower_r = rand_numeric::__change_to_value<F, R>(lower, "lower");
            return rand_range_query<R>(q, from_r, to_r, lower_r);
        }

    } // namespace rand_array
} // namespace generator

#endif // !_SGPCET_ARRAY_H_
