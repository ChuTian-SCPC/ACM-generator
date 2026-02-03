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
            static std::string get() { return "var"; } 
        };

        template<> struct TypeName<int> { static std::string get() { return "int"; } };
        template<> struct TypeName<unsigned int> { static std::string get() { return "uint"; } };
        template<> struct TypeName<float> { static std::string get() { return "float"; } };
        template<> struct TypeName<double> { static std::string get() { return "double"; } };
        template<> struct TypeName<long long> { static std::string get() { return "longlong"; } };
        template<> struct TypeName<unsigned long long> { static std::string get() { return "ulonglong"; } };
        template<> struct TypeName<char> { static std::string get() { return "char"; } };
        template<> struct TypeName<bool> { static std::string get() { return "bool"; } };
        template<> struct TypeName<std::string> { static std::string get() { return "string"; } };

        template<typename T, typename U>
        struct TypeName<std::pair<T,U>> {
            static std::string get() {
                return "pair<" + TypeName<T>::get() + "," + TypeName<U>::get() + ">";
            }
        };

        template<typename T>
        struct TypeName<std::vector<T>> {
            static std::string get() {
                return "vector<" + TypeName<T>::get() + ">";
            }
        };

        template<typename T>
        struct TypeName<std::queue<T>> {
            static std::string get() {
                return "queue<" + TypeName<T>::get() + ">";
            }
        };

        template<typename T>
        struct TypeName<std::stack<T>> {
            static std::string get() {
                return "stack<" + TypeName<T>::get() + ">";
            }
        };

        template<typename T>
        struct TypeName<std::deque<T>> {
            static std::string get() {
                return "deque<" + TypeName<T>::get() + ">";
            }
        };

        template<typename T>
        struct TypeName<std::list<T>> {
            static std::string get() {
                return "list<" + TypeName<T>::get() + ">";
            }
        };
        

        template<typename... Args>
        struct _TupleName;

        template<typename First, typename... Rest>
        struct _TupleName<First, Rest...> {
            static void append(std::string &s) {
                s += TypeName<First>::get();
                if (sizeof...(Rest) > 0) {
                    s += ",";
                    _TupleName<Rest...>::append(s);
                }
            }
        };

        template<> struct _TupleName<> { static void append(std::string&) {} };

        template<typename... Args>
        struct TypeName<std::tuple<Args...>> {
            static std::string get() {
                std::string s = "tuple<";
                _TupleName<Args...>::append(s);
                s += ">";
                return s;
            }
        };

        template<typename K, typename V>
        struct TypeName<std::map<K,V>> {
            static std::string get() {
                return "map<" + TypeName<K>::get() + "," + TypeName<V>::get() + ">";
            }
        };

        template<typename K, typename V>
        struct TypeName<std::unordered_map<K,V>> {
            static std::string get() {
                return "unordered_map<" + TypeName<K>::get() + "," + TypeName<V>::get() + ">";
            }
        };

        template<typename K, typename V>
        struct TypeName<std::multimap<K,V>> {
            static std::string get() {
                return "multimap<" + TypeName<K>::get() + "," + TypeName<V>::get() + ">";
            }
        };

        template<typename K>
        struct TypeName<std::set<K>> {
            static std::string get() {
                return "set<" + TypeName<K>::get() + ">";
            }
        };

        template<typename K>
        struct TypeName<std::unordered_set<K>> {
            static std::string get() {
                return "unordered_set<" + TypeName<K>::get() + ">";
            }
        };

        template<typename K>
        struct TypeName<std::multiset<K>> {
            static std::string get() {
                return "multiset<" + TypeName<K>::get() + ">";
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
                return tools::string_format("<unnamed>%s_%d", TypeName<T>::get().c_str(), var_count);
            }
        };

        template<typename T>
        int _AutoName<T>::var_count = 0;
    }

}


#endif // !_SGPCET_VAL_INIT_H_
