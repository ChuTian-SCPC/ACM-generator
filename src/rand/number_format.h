#ifndef _SGPCET_NUMBER_FORMAT_H_
#define _SGPCET_NUMBER_FORMAT_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_

namespace generator {
    namespace rand_numeric {
        // 检查R是否可以包含T的所有值
        template<typename T, typename R>
        struct is_range_contained {
        private:
            static constexpr bool check_integral_range() {
                if (std::is_same<T, R>::value) return true;
                
                if (std::is_signed<T>::value && std::is_unsigned<R>::value) {
                    return std::numeric_limits<R>::max() >= std::numeric_limits<T>::max() &&
                        std::numeric_limits<T>::min() >= 0; 
                }
                
                if (std::is_unsigned<T>::value && std::is_signed<R>::value) {
                    return std::numeric_limits<R>::max() >= std::numeric_limits<T>::max();
                }
                
                return std::numeric_limits<R>::min() <= std::numeric_limits<T>::min() &&
                    std::numeric_limits<R>::max() >= std::numeric_limits<T>::max();
            }
            
        public:
            static constexpr bool value = 
                std::is_integral<T>::value &&
                std::is_integral<R>::value &&
                check_integral_range();
        };

        template<typename T>
        T __string_to_value(const std::string& s) {
            _msg::__error_msg(_msg::_defl, "Unsupported type.");
        }
        
        template<>
        float __string_to_value(const std::string& s) {
            return std::stof(s);
        }
        
        template<>
        double __string_to_value(const std::string& s) {
            return std::stod(s);
        }
        
        bool __is_real_format(const std::string& s) {
            return s.find_first_of("eE.") != std::string::npos;
        }
        
        template<>
        int __string_to_value(const std::string& s) {
            if (__is_real_format(s)) 
                return (int)__string_to_value<double>(s);
            return std::stoi(s);
        }
        
        template<>
        long __string_to_value(const std::string& s) {
            if (__is_real_format(s)) 
                return (long)__string_to_value<double>(s);
            return std::stol(s);
        }
        
        template<>
        unsigned long __string_to_value(const std::string& s) {
            if (__is_real_format(s)) 
                return (unsigned long)__string_to_value<double>(s);
            return std::stoul(s);
        }
        
        template<>
        long long __string_to_value(const std::string& s) {
            if (__is_real_format(s)) 
                return (long long)__string_to_value<double>(s);
            return std::stoll(s);
        }
        
        template<>
        unsigned long long __string_to_value(const std::string& s) {
            if (__is_real_format(s)) 
                return (unsigned long long)__string_to_value<double>(s);
            return std::stoull(s);
        }
        
        template<>
        unsigned int __string_to_value(const std::string& s) {
            return (unsigned int)__string_to_value<long long>(s);
        }

        std::string __sub_value_string(std::string&s, size_t from, size_t to) {
            return s.substr(from + 1, to - from - 1);
        }
        
        template <typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, std::pair<T, T>>::type
        __format_to_int_range(std::string s) {
            static std::unordered_map<std::string, std::pair<T, T>> range_cache;
            if(range_cache.find(s) != range_cache.end()) return range_cache[s];
            size_t open = s.find_first_of("[(");
            size_t close = s.find_first_of(")]");
            size_t comma = s.find(',');
            if(open == std::string::npos || close == std::string::npos || comma == std::string::npos) {
                _msg::__fail_msg(_msg::_defl, tools::string_format("%s is an invalid range.",s.c_str()));
            }
            T left = __string_to_value<T>(__sub_value_string(s, open, comma));
            T right = __string_to_value<T>(__sub_value_string(s, comma, close));
            if (s[open] == '(') left++;
            if (s[close] == ')') right--;
            _msg::__warn_msg(_msg::_defl, tools::string_format("translate format \"%s\" into range [%s, %s], please check.",
                s.c_str(), std::to_string(left).c_str(), std::to_string(right).c_str()));
            range_cache[s] = std::make_pair(left, right);
            return range_cache[s];
        }

        int __number_accuracy(const std::string& s) {
            static std::unordered_map<std::string, int> accuracy_cache;
            if(accuracy_cache.find(s) != accuracy_cache.end()) return accuracy_cache[s];
            int digit = 1;
            bool is_decimal_part = false;
            bool is_scientific_part = false;
            std::string scientific_part = "";
            for(auto c : s) {
                if(is_decimal_part == true){
                    if(c >= '0' && c <= '9') digit ++;
                    else is_decimal_part = false;
                }
                if(is_scientific_part == true) scientific_part += c;
                if(c == '.') is_decimal_part = true;
                if(c == 'e' || c == 'E') is_scientific_part = true;
            }
            if(scientific_part != ""){
                int scientific_length = std::stoi(scientific_part);
                digit -= scientific_length;
            }
            accuracy_cache[s] = digit;
            return accuracy_cache[s];
        }
        
        template <typename T = double>
        typename std::enable_if<std::is_floating_point<T>::value, std::pair<T, T>>::type
        __format_to_double_range(std::string s) {
            static std::unordered_map<std::string, std::pair<T, T>> range_cache;
            if(range_cache.find(s) != range_cache.end()) return range_cache[s];
            int accuracy = 1;
            size_t open = s.find_first_of("[(");
            size_t close = s.find_first_of(")]");
            size_t comma = s.find(',');
            if(open == std::string::npos || close == std::string::npos || comma == std::string::npos) {
                _msg::__fail_msg(_msg::_defl, tools::string_format("%s is an invalid range.",s.c_str()));
            }
            std::string left_str = __sub_value_string(s, open, comma);
            std::string right_str = __sub_value_string(s, comma, close);
            T left = __string_to_value<T>(left_str);
            T right = __string_to_value<T>(right_str);
            accuracy = std::max(accuracy, std::max(__number_accuracy(left_str), __number_accuracy(right_str)));
            double eps1 = std::pow(10.0, -accuracy + 1);
            double eps2 = std::pow(10.0, -accuracy);
            if(s[open] == '(') left += eps1;
            if(s[close] == ']') right += eps2;
            _msg::__warn_msg(_msg::_defl, tools::string_format("translate format \"%s\" into range [%.*f, %.*f), the accuracy is 10^{-%d}, please check.",
                s.c_str(), accuracy, left, accuracy, right, accuracy - 1));
            range_cache[s] = std::make_pair(left, right);
            return range_cache[s];
        }

        template<typename T>
        typename std::enable_if<std::is_integral<T>::value, std::pair<T, T>>::type
        __format_to_range(std::string s) {
            return __format_to_int_range<T>(s);
        }
        
        template<typename T>
        typename std::enable_if<std::is_floating_point<T>::value, std::pair<T, T>>::type
        __format_to_range(std::string s) {
            return __format_to_double_range<T>(s);
        }

        template <typename T, typename R>
        typename std::enable_if<is_range_contained<T, R>::value, R>::type
        __change_to_int(T value, std::string) {
            return static_cast<R>(value);
        }

        template <typename T, typename R>
        typename std::enable_if<!is_range_contained<T, R>::value, R>::type
        __change_to_int(T value, std::string name) {
            static std::unordered_map<T, R> cache;
            if(cache.find(value) != cache.end()) return cache[value];
            R result = static_cast<R>(value);
            std::string value_s = std::to_string(value);
            std::string result_s = std::to_string(result);
            _msg::__warn_msg(_msg::_defl, tools::string_format("change %s number : %s -> %s, please check.",
                name.c_str(), value_s.c_str(), result_s.c_str()));
            cache[value] = result;
            return result;
        }

        template <typename T, typename R>
        typename std::enable_if<std::is_same<T, R>::value, R>::type
        __change_to_double(T value, std::string){
            return value;
        }

        template <typename T, typename R>
        typename std::enable_if<!std::is_same<T, R>::value, R>::type
        __change_to_double(T value, std::string name){
            static std::unordered_map<T, R> cache;
            if(cache.find(value) != cache.end()) return cache[value];
            R result = static_cast<R>(value);
            std::string value_s = std::to_string(value);
            std::string result_s = std::to_string(result);
            _msg::__warn_msg(_msg::_defl,  tools::string_format("change %s number : %s -> %s, please check.",
                name.c_str(), value_s.c_str(), result_s.c_str()));
            cache[value] = result;
            return result;
        }

        template <typename T, typename R>
        typename std::enable_if<std::is_integral<R>::value, R>::type
        __change_to_value(T value, std::string name) {
            return __change_to_int<T, R>(value, name);
        }
    
        template <typename T, typename R>
        typename std::enable_if<std::is_floating_point<R>::value, R>::type
        __change_to_value(T value, std::string name) {
            return __change_to_double<T, R>(value, name);
        }

    } // namespace rand_numeric
} // namespace generator

#endif // !_SGPCET_NUMBER_FORMAT_H_
