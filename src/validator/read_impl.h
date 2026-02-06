#ifndef _SGPCET_READ_IMPL_H_
#define _SGPCET_READ_IMPL_H_

#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_
#ifndef _SGPCET_VAL_INIT_H_
#include "val_init.h"
#endif // !_SGPCET_VAL_INIT_H_


namespace validate {
    namespace reader {
        using namespace generator;
        // testlib think end-of-line is also a blank, but we let it become not to support read multi-line token
        bool is_blank(char c) {
            return c == SPACE || c == TAB;
        }

        bool char_match(char c1, char c2) {
            if (is_blank(c1) && is_blank(c2)) return true;
            if (isEoln(c1) && isEoln(c2)) return true;
            if (isEof(c1) && isEof(c2)) return true;
            return c1 == c2;
        }

        // testlib readEoln consider LF on Linux and CR+LF on Windows
        // but the text format can be auto convert to LF on Windows now
        // so we support the format not as strict as testlib
        void read_eoln() {
            auto& reader = inf.reader;
            inf.lastLine = reader->getLine();
            int cur = reader->nextChar();
            if (!isEoln(cur)) {
                reader->unreadChar(cur);
                _msg::__fail_pe_msg(_msg::_defl, "Expected EOLN");
            } else if (cur == CR) {
                int next = reader->nextChar();
                if (next != LF) {
                    reader->unreadChar(next);
                    reader->unreadChar(cur);
                    _msg::__fail_pe_msg(_msg::_defl, "Expected EOLN");
                }
            }
        }

        bool is_blank_split(const std::string& s) {
            int n = s.size();
            if (n == 0) return true;
            if (n == 1 && isBlanks(s[0])) return true;
            return false;
        }

        // testlib has no readWordTo which use a specified split but only white-space
        // so we wirte a read_word function similar to testlib's readWordTo which use the given split to split the token
        // like some time format "12:34:56", we can use read_word(":") to get number
        // `read_split` mean whether move the reader pointer to the split front or not
        // example: `12:34:56`
        // pointer will at `2` if `read_split` is false, pointer will move to the split front which nextChar will match the split[0]
        // pointer will at `:` if `read_split` is true, pointer will stop at the split end which nextChar will match the next token
        // testlib isBlanks will stop at white-space, end-of-line and tab
        // we add a `multi_line` parameter to support multi-line token, like `12\n34` will be get if `multi_line` is true
        // so we don't use isBlank to check end-of-line, use is_blank instead
        // the split match will stop at the first match, for example `12:::` use `read_word("::")` will get `12`
        std::string read_word(std::string split, bool read_split = false, bool multi_line = false) {
            std::string result = "";

            auto& reader = inf.reader;
            inf.lastLine = reader->getLine(); // testlib need this information
            int cur = reader->nextChar();
            if (cur == EOFC) _msg::__fail_eof_msg(_msg::_defl, "Unexpected end of file - token expected");
            
            int pos = 0;
            int n = split.size();
            std::string match = "";
            while(!(isEof(cur) || (!multi_line && isEoln(cur)))) {
                if (n == 0) result += char(cur); // if no split and multi_line is true, we read the whole txt
                else if (char_match(cur, split[pos])) {
                    match += char(cur);
                    pos++;
                    if (pos == n) break;
                } else {
                    if (pos) {
                        pos = 0;
                        result += match;
                        match = "";
                    } 
                    result += char(cur);
                }

                if (result.size() > inf.maxTokenLength) {
                    _msg::__fail_pe_msg(_msg::_defl, 
                        tools::string_format("Length of token exceeds %d, token is '%s...'", int(inf.maxTokenLength), __testlib_part(result).c_str()));
                }
                cur = reader->nextChar();
            }

            if (!is_blank_split(split)) {
                if (pos != n) _msg::__fail_msg(_msg::_defl, tools::string_format("no match for split '%s'", split.c_str()));
                else if (!read_split) {
                    while(pos) {
                        reader->unreadChar(match[pos - 1]);
                        pos--;
                    }
                }
            } else {
                reader->unreadChar(cur);
                if (read_split) {
                    if (isEof(cur)) inf.readEof();
                    else if (isEoln(cur)) read_eoln();
                    else inf.readChar();
                }
            }

            if (result.size() == 0) _msg::__fail_eof_msg(_msg::_defl, "Unexpected end of file or white-space - token expected");
            return result;
        }

        template<typename T>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        __string_to_int(const std::string& s, _enum::StringConvertError& err) {
            T result = 0;
            err = _enum::StringConvertError::SUCCESS;
            int n = s.size();
            int max_len = std::numeric_limits<T>::digits10 + 1;
            bool is_negative = false;
            if (n == 0) err = _enum::StringConvertError::INVALID_FORMAT;
            else {
                int start = 0;
                if (s[0] == '-') {
                    is_negative = true;
                    start = 1;
                }
                else if (s[0] == '+') start = 1;
                if (n - start > 2 && s[start] == '0') {
                    err = _enum::StringConvertError::LEADING_ZERO;
                    while(start < n && s[start] == '0') start++;
                    if (start == n) {
                        is_negative = true;
                        return T(0);
                    }
                }
                if (!std::numeric_limits<T>::is_signed && is_negative) {
                    err = _enum::StringConvertError::INVALID_FORMAT;
                    return T(0);
                }
                
                if (start == n) err = _enum::StringConvertError::INVALID_FORMAT;
                else if (n - start > max_len) err = _enum::StringConvertError::OUT_OF_RANGE;
                else if (n - start == max_len) {
                    std::string max_str = std::to_string(std::numeric_limits<T>::max());
                    std::string min_str = std::to_string(std::numeric_limits<T>::min());
                    std::string abs_min_str = min_str.substr(1);
                    if (is_negative) { //  unsigned has return before this
                        if (s.substr(start) > abs_min_str) err = _enum::StringConvertError::OUT_OF_RANGE;
                        // min value will overflow use result = result * 10 + digit to calculate
                        else if (s.substr(start) == abs_min_str) return std::numeric_limits<T>::min(); 
                    } else {
                        if (s.substr(start) > max_str) err = _enum::StringConvertError::OUT_OF_RANGE;
                        else if (s.substr(start) == max_str) return std::numeric_limits<T>::max();
                    }
                }

                for (int i = start; i < n; i++) {
                    if (s[i] < '0' || s[i] > '9') {
                        err = _enum::StringConvertError::INVALID_FORMAT;
                        return T(0);
                    }
                    result = result * 10 + (s[i] - '0');
                }
                if (is_negative) result = -result;
            }
            if (err == _enum::StringConvertError::SUCCESS && result == 0 && is_negative) err = _enum::StringConvertError::NEGATIVE_ZERO;
            return result;
        }

        template<typename T>
        typename std::enable_if<std::is_floating_point<T>::value, T>::type
        __string_to_double(const std::string& s, _enum::StringConvertError& err, bool& is_scientific, int& point_digits_count) {
            T result = 0.0;
            err = _enum::StringConvertError::SUCCESS;
            is_scientific = false;
            point_digits_count = 0;
            int n = s.size();
            if (n == 0) err = _enum::StringConvertError::INVALID_FORMAT;
            else {
                int sign_count = 0;
                int dot_count = 0;
                int e_count = 0;
                int digit_count = 0;
                int dot_pos = -1;
                for (int i = 0; i < n; i++) {
                    char c = s[i];
                    if (c == '-' || c == '+') {
                        if (i != 0 && s[i - 1] != 'e' && s[i - 1] != 'E') err = _enum::StringConvertError::INVALID_FORMAT;
                        sign_count++;
                    } else if (c == '.') {
                        if (i == n - 1 || i == 0 || 
                            (i > 0 && (s[i - 1] < '0' || s[i - 1] > '9')) ||
                            (i < n - 1 && (s[i + 1] < '0' || s[i + 1] > '9'))) 
                            err = _enum::StringConvertError::INVALID_FORMAT;
                        dot_count++;
                        dot_pos = i;
                    } else if (c == 'e' || c == 'E') {
                        if (i == n - 1 || i == 0) err = _enum::StringConvertError::INVALID_FORMAT;
                        e_count++;
                        is_scientific = true;
                    } else if (c >= '0' && c <= '9') {
                        digit_count++;
                    } else {
                        err = _enum::StringConvertError::INVALID_FORMAT;
                    }
                }
                
                if (digit_count == 0 || dot_count > 1 || e_count > 1 || sign_count > e_count + 1) 
                    err = _enum::StringConvertError::INVALID_FORMAT;
                if (dot_pos == -1) point_digits_count = 0;
                else point_digits_count = n - dot_pos - 1;
            }

            if (err == _enum::StringConvertError::SUCCESS) {
                std::istringstream iss(s);
                iss >> result;
                if (iss.fail()) {
                    result = std::numeric_limits<T>::quiet_NaN();
                    err = _enum::StringConvertError::OUT_OF_RANGE;
                }
                iss.clear();
            } else {
                return T(0);
            }
            
            if (result == T(0.0) && s[0] == '-') {
                err = _enum::StringConvertError::NEGATIVE_ZERO;
                result = T(0.0);
            }
            int start = 0, end = 0;
            if (s[0] == '-' || s[0] == '+') start++;
            for (int i = start; i < n; i++) {
                if (s[i] < '0' || s[i] > '9') {
                    end = i;
                    break;
                }
            }
            if (end - start > 1 && s[start] == '0') err = _enum::StringConvertError::LEADING_ZERO;

            return result;
        }

        template<typename T>
        class ErrorReporter {
        public:
            static void report_int(const std::string& name, _enum::StringConvertError error, const std::string& word) {
                if (error == _enum::StringConvertError::INVALID_FORMAT) {
                    _msg::__fail_pe_msg(_msg::_defl, 
                        tools::string_format("%s: \"%s\" is invalid integer format", name.c_str(), word.c_str()));
                }
                if (error == _enum::StringConvertError::OUT_OF_RANGE) {
                    _msg::__fail_pe_msg(_msg::_defl, 
                        tools::string_format("%s: \"%s\" is out of %s range", 
                            name.c_str(), word.c_str(), TypeName<T>::full_name().c_str()));
                }
                if (error == _enum::StringConvertError::NEGATIVE_ZERO) {
                    _msg::__warn_msg(_msg::_defl, 
                        tools::string_format("%s: \"%s\" is negative zero", name.c_str(), word.c_str()));
                }
                if (error == _enum::StringConvertError::LEADING_ZERO) {
                    _msg::__warn_msg(_msg::_defl, 
                        tools::string_format("%s: \"%s\" has leading zero", name.c_str(), word.c_str()));
                }
            }
        };
    }

}

#endif // !_SGPCET_READ_IMPL_H_

