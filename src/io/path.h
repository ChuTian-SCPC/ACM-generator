#ifndef _SGPCET_PATH_H_
#define _SGPCET_PATH_H_

#ifndef _SGPCET_SETTING_H_
#include "basic/setting.h"
#endif // !_SGPCET_SETTING_H_
#ifndef _SGPCET_MACRO_H_
#include "basic/macro.h"
#endif // !_SGPCET_MACRO_H_
#ifndef _SGPCET_TOOLS_H_
#include "basic/tools.h"
#endif // !_SGPCET_TOOLS_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_

namespace generator {
  namespace io {
    class Path {
    protected:
      std::string _path;
    public:
      Path() : _path("") {}
      Path(const std::string& s) : _path(s) {}
      Path(const char *s) : _path(std::string(s)) {}
      Path(const Path& other) : _path(other._path) {}
      Path(Path&& other) noexcept : _path(std::move(other._path)) {}
      Path(std::string&& s) noexcept : _path(std::move(s)) {}
      Path& operator=(Path&& other) noexcept {
          if (this != &other) _path = std::move(other._path);
          return *this;
      }
      Path& operator=(const Path& other) {
          if (this != &other) _path = other._path;
          return *this;
      }
      Path& operator=(const std::string& s) {
          _path = s;
          return *this;
      }
      
      _SET_GET_VALUE(std::string, path)
      void set_path(const char* path) { _path = std::string(path); }
      void set_path(Path other_path) { _path = other_path.path(); }
      const char* cname() const { return _path.c_str(); }
      
      bool __empty() { return _path.empty(); }
      bool __file_exist() {
        std::ifstream file(_path.c_str());
        return file.is_open();
      }

      void __ensure_file_exist() {
        if (__empty()) return;
        __file_exist();
      }
      
      bool __directory_exists() {
        #ifdef ON_WINDOWS
          struct _stat path_stat;
          if (_stat(_path.c_str(), &path_stat) != 0) return false;
          return (path_stat.st_mode & _S_IFDIR) != 0;
        #else
          struct stat path_stat;
          if (stat(_path.c_str(), &path_stat) != 0) return false;
          return S_ISDIR(path_stat.st_mode);
        #endif
      }
      
      void __unify_split() {
        for (auto& c : _path) 
          if (c == _setting::_other_split)
            c = _setting::_path_split;
      }
      
      Path __folder_path() {
        if (__directory_exists()) return Path(_path);
        __unify_split();
        size_t pos = _path.find_last_of(_setting::_path_split);
        if (pos != std::string::npos) return Path(_path.substr(0, pos));
        return Path();
      }
      
      std::string __file_name() {
        if (!__file_exist()) 
          _msg::__fail_msg(_msg::_defl, 
            tools::string_format("%s is not a file or the file doesn't exist.", _path.c_str()));
        __unify_split();
        size_t pos = _path.find_last_of(_setting::_path_split);
        std::string file_full_name = pos == std::string::npos ? _path : _path.substr(pos + 1);
        size_t pos_s = file_full_name.find_last_of('.');
        std::string file_name = pos_s == std::string::npos ? file_full_name : file_full_name.substr(0, pos_s);
        return file_name;
      }
      
      void full() {
        #ifdef ON_WINDOWS
          char buffer[MAX_PATH];
          if (GetFullPathNameA(_path.c_str(), MAX_PATH, buffer, nullptr) == 0) 
            _msg::__fail_msg(_msg::_defl, tools::string_format("Can't find full path :%s.", _path.c_str()));
        #else
          char buffer[PATH_MAX];
          if (realpath(_path.c_str(), buffer) == nullptr) 
            _msg::__fail_msg(_msg::_defl, tools::string_format("Can't find full path :%s.", _path.c_str()));
        #endif
          _path = std::string(buffer);
      }
      
      Path full_path() {
        Path other;
        other.set_path(_path);
        other.full();
        return other;
      }
      
      void __delete_file() {
        if(this->__file_exist()) std::remove(_path.c_str());
      }
      
      friend std::ostream& operator<<(std::ostream& os, const Path& path) {
        os << path._path;
        return os;
      }
      
      template <typename... Args>
      Path join(const Args&... args) {
      #ifdef ON_WINDOWS
        if (this->__empty()) return tools::string_join(_setting::_path_split, args...);
      #endif // ON_WINDOWS
        std::string path_join = tools::string_join(_setting::_path_split, _path, args...);
        return Path(path_join);
      }
    };
    
    template <typename T, typename... Args>
    Path __path_join(const T& path, const Args &... args) {
      return Path(path).join(args...);
    }
    
    template<typename T>
    struct IsPath {
      static constexpr bool value = std::is_same<T, Path>::value;
    };
    
    template<typename T>
    struct IsPathConstructible {
      static constexpr bool value = std::is_convertible<T, std::string>::value || IsPath<T>::value;
    };

    Path __current_path() {
    #ifdef ON_WINDOWS
        char buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, MAX_PATH);
    #elif defined(__APPLE__)
        char buffer[PATH_MAX];
        uint32_t size = sizeof(buffer);
        _NSGetExecutablePath(buffer, &size);
    #else
        char buffer[PATH_MAX];
        ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer));
        if (length != -1) {
            buffer[length] = '\0';
        }
    #endif
        Path executable_path(buffer);
        return executable_path.__folder_path();
    }

    template <typename T>
    typename std::enable_if<IsPathConstructible<T>::value, Path>::type
    __full_path(T p) {
        Path path(p);
        path.full();
        return path;
    }
    
    bool __create_directory(Path& path) {
        if(path.__directory_exists())  return true;
        return mkdir(path.cname(),0777) == 0;
    }

    void __create_directories(Path path) {
        path.__unify_split();
        std::istringstream ss(path.path());
        std::string token;
        Path current_path("");
        while (std::getline(ss, token, _setting::_path_split)) {
            current_path = __path_join(current_path, token);
        #ifdef ON_WINDOWS
            if(current_path.path().find_first_of( _setting::_path_split) == std::string::npos && current_path.path().back() == ':') continue;
        #else
            if(current_path.path().size() == 1 && current_path.path()[0] ==  _setting::_path_split) continue;
        #endif
            if (!__create_directory(current_path)) 
              _msg::__fail_msg(_msg::_defl, tools::string_format("Error in creating folder : %s.",current_path.cname()));
        }
    }
    
    template<typename T1, typename T2>
    typename std::enable_if<IsPathConstructible<T1>::value && IsPathConstructible<T2>::value, void>::type
    __copy_file(T1 source, T2 destination) {
    #ifdef ON_WINDOWS
        CopyFile(Path(source).cname(), Path(destination).cname(), true);
    #else
        std::string command = tools::string_join(" ", "cp", source, destination);
        std::system(command.c_str());
    #endif
        
    }

  } // namespace io
  
  namespace _msg {
    OutStream::OutStream(const io::Path& path, bool log_same) {
      open(path.path());
      _log_same = log_same;
    }
  } // namespace _msg
}

#endif // !_SGPCET_PATH_H_
