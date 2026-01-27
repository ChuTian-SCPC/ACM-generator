#ifndef _SGPCET_READ_IMPL_H_
#define _SGPCET_READ_IMPL_H_

#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_


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
    }


    // // -----------------------------
    // // 占位类定义（可替换为你的实际 Limits 类型）
    // // -----------------------------
    // template<typename T> struct VarLimits {};
    // struct StringLimits {};
    // template<typename V> struct VectorLimits {};
    // template<typename V1, typename V2> struct PairLimits {};

    // // -----------------------------
    // // 默认 ReadImpl：匹配失败会报错
    // // -----------------------------
    // template<typename T, typename Param, typename Enable = void>
    // struct ReadImpl {
    //     static T apply(const Param& p) {
    //         _msg::__fail_msg(_msg::_defl, "invalid reader"); // 默认报错
    //         return T{}; // 避免警告
    //     }
    // };

    // // -----------------------------
    // // 数值类型特化（int, double 等）
    // // -----------------------------
    // template<typename T>
    // struct ReadImpl<T, VarLimits<T>,
    //     typename std::enable_if<std::is_arithmetic<T>::value>::type> {
    //     static T apply(const VarLimits<T>& p) {
    //         // TODO: 实现 int/double 读取逻辑
    //         return T{};
    //     }
    // };

    // // -----------------------------
    // // string 类型特化
    // // -----------------------------
    // template<>
    // struct ReadImpl<std::string, StringLimits> {
    //     static std::string apply(const StringLimits& p) {
    //         // TODO: 实现 string 读取逻辑
    //         return std::string{};
    //     }
    // };

    // // -----------------------------
    // // std::vector<V> 类型特化
    // // -----------------------------
    // template<typename V>
    // struct ReadImpl<std::vector<V>, VectorLimits<V>> {
    //     static std::vector<V> apply(const VectorLimits<V>& p) {
    //         // TODO: 实现 vector<V> 读取逻辑
    //         return std::vector<V>{};
    //     }
    // };

    // // -----------------------------
    // // std::pair<V1,V2> 类型特化
    // // -----------------------------
    // template<typename V1, typename V2>
    // struct ReadImpl<std::pair<V1,V2>, PairLimits<V1,V2>> {
    //     static std::pair<V1,V2> apply(const PairLimits<V1,V2>& p) {
    //         // TODO: 实现 pair<V1,V2> 读取逻辑
    //         return std::pair<V1,V2>{};
    //     }
    // };

    // // -----------------------------
    // // 封装函数 read<T>(Param)
    // // -----------------------------
    // template<typename T, typename Param>
    // T read(const Param& p) {
    //     return ReadImpl<T, Param>::apply(p);
    // }

    // class BasicVar {
    // public:
    //     virtual ~BasicVar() {}
    //     virtual void read() = 0;          // 调用 read<T>(limit)
    //     virtual void* get_ptr() = 0;      // 返回内部存储地址（类型擦除）
    // };

    // template<typename T, typename LimitType>
    // class Var : public BasicVar {
    //     T value;
    //     LimitType limit;
    // public:
    //     Var(const LimitType& l) : limit(l) {}
        
    //     virtual void read() override {
    //         value = read<T>(limit);
    //     }

    //     virtual void* get_ptr() override {
    //         return &value;
    //     }

    //     T get() const { return value; }
    // };

    // class Line {
    //     std::vector<BasicVar*> vars;
    // public:
    //     ~Line() {
    //         for (auto v : vars) delete v;
    //     }

    //     template<typename T, typename LimitType>
    //     void addVar(const LimitType& limit) {
    //         vars.push_back(new Var<T, LimitType>(limit));
    //     }

    //     void readAll() {
    //         for (auto v : vars) v->read();
    //     }

    //     template<typename T>
    //     T get(std::size_t idx) {
    //         return *static_cast<T*>(vars[idx]->get_ptr());
    //     }
    // };


}

#endif // !_SGPCET_READ_IMPL_H_

