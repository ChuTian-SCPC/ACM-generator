#ifndef _SGPCET_READ_IMPL_H_
#define _SGPCET_READ_IMPL_H_

#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_

namespace generator {
    namespace validate {
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
                if (n == 0) result += char(cur); // if no split and multi_line is false, we read the whole txt
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
        struct VarLimits {
            bool has_limits;
            T min, max;
        };

        struct CharLimits {
            enum LimitType {
                NONE,
                REQUIRED,
                RANGE,
            } limit_type;
            char required_char;
            char min_char, max_char;
        };

        struct StringLimits {
            bool enable_whitespace; // readToken or readLine
            bool has_pattern;
            std::string pattern;
        };

        template<typename T, typename Param>
        T read(const Param& p) {
            _msg::__fail_msg(_msg::_defl, "invaild reader");
        }

        template<typename T>
        int read(const VarLimits<int>& p) {
            if (p.has_limits) return inf.readInt(p.min, p.max);
            else return inf.readInt();
        }

        template<typename T>
        unsigned int read(const VarLimits<unsigned int>& p) {
            // testlib has no readUnsignedInt,
            // so we read long and cast it to unsigned int.
            if (p.has_limits) return (unsigned int)inf.readLong((long long)p.min, (long long)p.max);
            else return (unsigned int)inf.readLong();
        }

        template<typename T>
        long long read(const VarLimits<long long>& p) {
            if (p.has_limits) return inf.readLong(p.min, p.max);
            else return inf.readLong();
        }

        template<typename T>
        unsigned long long read(const VarLimits<unsigned long long>& p) {
            if (p.has_limits) return inf.readUnsignedLong(p.min, p.max);
            else return inf.readUnsignedLong();
        }

        template<typename T>
        double read(const VarLimits<double>& p) {
            if (p.has_limits) return inf.readDouble(p.min, p.max);
            else return inf.readDouble();
        }

        template<typename T>
        float read(const VarLimits<float>& p) {
            if (p.has_limits) return (float)inf.readDouble(p.min, p.max);
            else return (float)inf.readDouble();
        }

        template<typename T>
        char read(const CharLimits& p) {
            if (p.limit_type == CharLimits::REQUIRED) return inf.readChar(p.required_char);
            else {
                char c = inf.readChar();
                if (p.limit_type == CharLimits::RANGE) {
                    if (c < p.min_char || c > p.max_char) {
                        _msg::__fail_msg(_msg::_defl, tools::string_format("char %c out of range [%c, %c] ", c, p.min_char, p.max_char));
                    }
                }
                return c;
            }
        }

        template<typename T>
        std::string read(const StringLimits& p) {
            if (p.enable_whitespace) {
                if (p.has_pattern) return inf.readToken(p.pattern);
                else return inf.readToken();
            } else {
                if (p.has_pattern) return inf.readLine(p.pattern);
                else return inf.readLine();
            }
        }

        struct VectorBaseLimit {
            bool has_limits;
            int size;
            std::string split;
            bool end_extra_split;
        };

        template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
        struct VectorLimits : public VectorBaseLimit {

        };
    }
}

#endif // !_SGPCET_READ_IMPL_H_

