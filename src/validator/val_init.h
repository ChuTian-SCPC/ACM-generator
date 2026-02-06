#ifndef _SGPCET_VAL_INIT_H_
#define _SGPCET_VAL_INIT_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_

namespace validate {
    namespace reader {
        using namespace generator;
        class _ValidateAutoReadEof {
        private:
            static bool _registered;

        public:
            static void _auto_read_eof() {
                if (testlibFinalizeGuard.readEofCount != 0) return;
                if (inf.reader != NULL) inf.readEof(); // inf may be destroyed before this function if meet some error in reading
            }

            _ValidateAutoReadEof() {
                if (_registered) return;
                _registered = true;
                atexit(_auto_read_eof);
            }

            _ValidateAutoReadEof(const _ValidateAutoReadEof&) = delete;
            _ValidateAutoReadEof& operator=(const _ValidateAutoReadEof&) = delete;
        };
        bool _ValidateAutoReadEof::_registered = false;

        template<typename T>
        struct TypeName {
            static std::string short_name() { return "var"; } 
            static std::string full_name() { return "var"; }
        };

        template<> struct TypeName<int> { 
            static std::string short_name() { return "int"; } 
            static std::string full_name() { return "int"; }
        };
        template<> struct TypeName<unsigned int> { 
            static std::string short_name() { return "uint"; } 
            static std::string full_name() { return "unsigned int"; }
        };
        template<> struct TypeName<float> { 
            static std::string short_name() { return "float"; } 
            static std::string full_name() { return "float"; }
        };
        template<> struct TypeName<double> { 
            static std::string short_name() { return "double"; } 
            static std::string full_name() { return "double"; }
        };
        template<> struct TypeName<long long> { 
            static std::string short_name() { return "longlong"; } 
            static std::string full_name() { return "long long"; }
        };
        template<> struct TypeName<unsigned long long> { 
            static std::string short_name() { return "ulonglong"; } 
            static std::string full_name() { return "unsigned long long"; }
        };
        template<> struct TypeName<char> { 
            static std::string short_name() { return "char"; } 
            static std::string full_name() { return "char"; }
        };
        template<> struct TypeName<bool> { 
            static std::string short_name() { return "bool"; } 
            static std::string full_name() { return "bool"; }
        };
        template<> struct TypeName<std::string> { 
            static std::string short_name() { return "string"; } 
            static std::string full_name() { return "string"; }
        };

        template<typename T, typename U>
        struct TypeName<std::pair<T,U>> {
            static std::string short_name() {
                return "pair<" + TypeName<T>::short_name() + "," + TypeName<U>::short_name() + ">";
            }
        };

        template<typename T>
        struct TypeName<std::vector<T>> {
            static std::string short_name() {
                return "vector<" + TypeName<T>::short_name() + ">";
            }
        };

        template<typename T>
        struct TypeName<std::queue<T>> {
            static std::string short_name() {
                return "queue<" + TypeName<T>::short_name() + ">";
            }
        };

        template<typename T>
        struct TypeName<std::stack<T>> {
            static std::string short_name() {
                return "stack<" + TypeName<T>::short_name() + ">";
            }
        };

        template<typename T>
        struct TypeName<std::deque<T>> {
            static std::string short_name() {
                return "deque<" + TypeName<T>::short_name() + ">";
            }
        }; 

        template<typename... Args>
        struct _TupleName;

        template<typename First, typename... Rest>
        struct _TupleName<First, Rest...> {
            static void append(std::string &s) {
                s += TypeName<First>::short_name();
                if (sizeof...(Rest) > 0) {
                    s += ",";
                    _TupleName<Rest...>::append(s);
                }
            }
        };

        template<> struct _TupleName<> { static void append(std::string&) {} };

        template<typename... Args>
        struct TypeName<std::tuple<Args...>> {
            static std::string short_name() {
                std::string s = "tuple<";
                _TupleName<Args...>::append(s);
                s += ">";
                return s;
            }
        };

        template<typename K, typename V>
        struct TypeName<std::map<K,V>> {
            static std::string short_name() {
                return "map<" + TypeName<K>::short_name() + "," + TypeName<V>::short_name() + ">";
            }
        };

        template<typename K, typename V>
        struct TypeName<std::unordered_map<K,V>> {
            static std::string short_name() {
                return "unordered_map<" + TypeName<K>::short_name() + "," + TypeName<V>::short_name() + ">";
            }
        };

        template<typename K, typename V>
        struct TypeName<std::multimap<K,V>> {
            static std::string short_name() {
                return "multimap<" + TypeName<K>::short_name() + "," + TypeName<V>::short_name() + ">";
            }
        };

        template<typename K>
        struct TypeName<std::set<K>> {
            static std::string short_name() {
                return "set<" + TypeName<K>::short_name() + ">";
            }
        };

        template<typename K>
        struct TypeName<std::unordered_set<K>> {
            static std::string short_name() {
                return "unordered_set<" + TypeName<K>::short_name() + ">";
            }
        };

        template<typename K>
        struct TypeName<std::multiset<K>> {
            static std::string short_name() {
                return "multiset<" + TypeName<K>::short_name() + ">";
            }
        };

        template<typename T>
        class _AutoName {
        protected:
            static int var_count;
        public:
            _AutoName() {
                var_count++;
            }
            
            std::string name() {
                return tools::string_format("<unnamed>%s_%d", TypeName<T>::short_name().c_str(), var_count);
            }
        };

        template<typename T>
        int _AutoName<T>::var_count = 0;
    }

}


#endif // !_SGPCET_VAL_INIT_H_
