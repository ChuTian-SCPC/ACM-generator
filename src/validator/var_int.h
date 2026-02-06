#ifndef _SGPCET_VAL_INT_H_
#define _SGPCET_VAL_INT_H_

#ifndef _SGPCET_VAR_IMPL_H_
#include "var_impl.h"
#endif
#ifndef _SGPCET_NUMBER_FORMAT_H_
#include "rand/number_format.h"
#endif
#ifndef _SGPCET_READ_IMPL_H_
#include "read_impl.h"
#endif

namespace validate {
    namespace reader {
        using namespace generator;

        template<typename T, typename = std::enable_if<std::is_integral<T>::value>>
        class Int : public _Var<T, Int<T>> {
        protected:
            T _min, _max;
            bool _has_min_limit, _has_max_limit;
            std::string _split;
            bool _read_split;
        public:
            Int() : 
                _Var<T, Int<T>>(_setting::_empty_var_name), 
                _min(std::numeric_limits<T>::min()), _max(std::numeric_limits<T>::max()),
                _has_min_limit(false), _has_max_limit(false),
                _split(_setting::default_split), 
                _read_split(_setting::default_read_split) {};
            
            Int(T min, T max, const std::string& name = _setting::_empty_var_name) :
                _Var<T, Int<T>>(name), _min(min), _max(max),
                _has_min_limit(true), _has_max_limit(true),
                _split(_setting::default_split), 
                _read_split(_setting::default_read_split) {};

            Int(const Int& other) : 
                _Var<T, Int<T>>(other), 
                _min(other._min), _max(other._max),
                _has_min_limit(other._has_min_limit), _has_max_limit(other._has_max_limit),
                _split(other._split), _read_split(other._read_split) {};
            
            Int& operator=(const Int& other) {
                _Var<T, Int<T>>::operator=(other);
                _min = other._min;
                _max = other._max;
                _has_min_limit = other._has_min_limit;
                _has_max_limit = other._has_max_limit;
                _split = other._split;
                _read_split = other._read_split;
                return *this;
            }

            Int(Int&& other) : 
                _Var<T, Int<T>>(std::move(other)), 
                _min(other._min), _max(other._max),
                _has_min_limit(other._has_min_limit), _has_max_limit(other._has_max_limit),
                _split(std::move(other._split)), _read_split(other._read_split) {};
            
            Int& operator=(Int&& other) {
                _Var<T, Int<T>>::operator=(std::move(other));
                _min = other._min;
                _max = other._max;
                _has_min_limit = other._has_min_limit;
                _has_max_limit = other._has_max_limit;
                _split = std::move(other._split);
                _read_split = other._read_split;
                return *this;
            }

            _GET_VALUE(T, min)
            void set_min(T min) {
                _min = min;
                _has_min_limit = true;
            }
            _GET_VALUE(T, max)
            void set_max(T max) {
                _max = max;
                _has_max_limit = true;
            }
            void set_range(T min, T max) {
                set_min(min);
                set_max(max);
            }
            void set_range(const std::string& range) {
                auto limits = tools::__format_to_int_range<T>(range);
                set_range(limits.first, limits.second);
            }
            _GET_VALUE(bool, has_min_limit)
            _GET_VALUE(bool, has_max_limit)
            _SET_GET_VALUE(std::string, split)
            _SET_GET_VALUE(bool, read_split)

            virtual T read() override {
                std::string word = read_word(_split, _read_split);
                _enum::StringConvertError error;
                T result = __string_to_int<T>(word, error);
                ErrorReporter<T>::report_int(this->_name, error, word);
                if (_has_min_limit && result < _min) {
                    _msg::__fail_msg(_msg::_defl, 
                        tools::string_format("%s: %s is less than min %s", 
                            this->_name.c_str(), word.c_str(), std::to_string(_min).c_str()));
                }
                if (_has_max_limit && result > _max) {
                    _msg::__fail_msg(_msg::_defl, 
                        tools::string_format("%s: %s is greater than max %s", 
                            this->_name.c_str(), word.c_str(), std::to_string(_max).c_str()));
                }
                this->_value = result;
                return this->_value;
            }
        };
    }
}


#endif
