#ifndef _SGPCET_COMMAND_PATH_H_
#define _SGPCET_COMMAND_PATH_H_

#ifndef _SGPCET_PATH_H_
#include "path.h"
#endif // !_SGPCET_PATH_H_

namespace generator {
  namespace io {


    class CommandPath {
    protected:
        Path _path;
        std::string _args;
        bool _enable_default_args;
    public: 
        CommandPath() : _path(Path()), _args(""), _enable_default_args(true) {}
        CommandPath(CommandPath&& other) noexcept : 
            _path(std::move(other._path)), _args(std::move(other._args)), _enable_default_args(std::move(other._enable_default_args)) {}
        CommandPath(const CommandPath& other) noexcept : 
            _path(other._path), _args(other._args), _enable_default_args(other._enable_default_args) {}
        template<typename T, typename = typename std::enable_if<IsPathConstructible<T>::value>::type>
        CommandPath(T&& s) : _path(std::forward<T>(s)), _args(""), _enable_default_args(true) {}
        template<typename T, typename = typename std::enable_if<IsPathConstructible<T>::value>::type>
        CommandPath(T&& s, std::string args) : _path(std::forward<T>(s)), _args(args), _enable_default_args(false) {}
        CommandPath& operator=(CommandPath&& other) noexcept {
            if (this != &other) { 
                _path = std::move(other._path);
                _args = std::move(other._args);
                _enable_default_args = std::move(other._enable_default_args);
            }
            return *this;
        }
        
        int run() { 
            _path.full();
            int code = std::system(command().c_str());
            return code; 
        }

        _SET_GET_VALUE(std::string, args)
        template <typename...Args>
        void add_args(const Args&... others) {
            _args = tools::string_join(" ", _args, others...);
        }         
        void clear_args() { _args.clear(); }

        _GET_VALUE(Path, path)
        template <typename T>
        typename std::enable_if<IsPathConstructible<T>::value, void>::type
        set_path(T path) {
            _path = Path(path);
        }

        _SET_GET_VALUE(bool, enable_default_args)

        std::string command() { return tools::string_join(" ", _path, _args);}
    };
    
  } // namespace io
} // namespace generator

#endif // !_SGPCET_COMMAND_PATH_H_
