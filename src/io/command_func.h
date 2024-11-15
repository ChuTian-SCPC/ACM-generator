#ifndef _SGPCET_COMMAND_FUNC_H_
#define _SGPCET_COMMAND_FUNC_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_
#ifndef _SGPCET_MACRO_H_
#include "basic/macro.h"
#endif // !_SGPCET_MACRO_H_

namespace generator {
    namespace io {
        template<typename T>
        struct IsFunctionConvertible {
            static constexpr bool value = std::is_convertible<T, std::function<void()>>::value;
        };

        class CommandFunc {
        protected:
            std::function<void()> _func;
            std::string _args;
            bool _enable_default_args;
        public: 
            CommandFunc() : _func(nullptr), _args(""), _enable_default_args(true) {}  
            CommandFunc(CommandFunc&& other) noexcept : 
                _func(std::move(other._func)), _args(std::move(other._args)), _enable_default_args(std::move(other._enable_default_args)) {} 
            CommandFunc(const CommandFunc& other) noexcept : 
                _func(other._func), _args(other._args), _enable_default_args(other._enable_default_args) {}
            template<typename T, typename = typename std::enable_if<IsFunctionConvertible<T>::value>::type>
            CommandFunc(T&& func) : _func(std::forward<T>(func)), _args(""), _enable_default_args(true) {}
            template<typename T, typename = typename std::enable_if<IsFunctionConvertible<T>::value>::type>
            CommandFunc(T&& func, std::string args) : _func(std::forward<T>(func)), _args(args), _enable_default_args(false) {}
            CommandFunc& operator=(CommandFunc&& other) noexcept {
                if (this != &other) { 
                    _func = std::move(other._func);
                    _args = std::move(other._args);
                    _enable_default_args = std::move(other._enable_default_args);
                }
                return *this;
            }
            
            _SET_GET_VALUE(std::string, args)
            template <typename...Args>
            void add_args(const Args&... others) {
                _args = tools::string_join(" ", _args, others...);
            }         
            void clear_args() { _args.clear(); }

            _GET_VALUE(std::function<void()>, func)
            template <typename T>
            typename std::enable_if<IsFunctionConvertible<T>::value, void>::type
            set_fun(T func) {
                _func = std::function<void()>(func);
            }

            _SET_GET_VALUE(bool, enable_default_args)
            
        };
    } // namespace io
    
} // namespace generator


#endif // !_SGPCET_COMMAND_FUNC_H_
