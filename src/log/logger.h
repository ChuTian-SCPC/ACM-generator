#ifndef _SGPCET_LOGGER_H_
#define _SGPCET_LOGGER_H_

#ifndef _SGPCET_COLOR_H_
#include "color.h"
#endif // !_SGPCET_COLOR_H_
#ifndef _SGPCET_TOOLS_H_
#include "common/tools.h"
#endif // !_SGPCET_TOOLS_H_
#ifndef _SGPCET_ENUM_H_
#include "common/enum.h"
#endif // !_SGPCET_ENUM_H_

namespace generator {
    namespace io {
        class Path;
    } // namespace io

    namespace _msg {

        class OutStream {
        protected:
          std::ostream* _stream;
          bool _control;
          std::string _path;
          std::unordered_set<std::string> _logs;
          bool _log_same;
        public:
          OutStream(bool log_same = true) : _log_same(log_same) { __default_output(); }
          OutStream(std::ostream& stream, bool log_same = true) : 
            _stream(&stream), _control(false), _path(""), _log_same(log_same)
          {
            _stream->iword(_colorize_index) = true;
          }
          OutStream(const io::Path& path, bool log_same = true);
          OutStream(const std::string& path, bool log_same = false) : _log_same(log_same) {
            open(path);
          }
          ~OutStream() { close(); }
          
          OutStream(const OutStream&) = delete;
          OutStream& operator=(const OutStream&) = delete;
          
          OutStream(OutStream&& other) noexcept
            : _stream(std::move(other._stream)), _control(other._control), _path(std::move(other._path)) {
            _control = false;
          }
          
          OutStream& operator=(OutStream&& other) noexcept {
            if (this != &other) {
              close(); 
              _stream = std::move(other._stream);
              _control = other._control;
              _path = std::move(other._path);
              other._control = false; 
            }
            return *this;
          }
          
          void swap(OutStream& other) noexcept {
            std::swap(_stream, other._stream);
            std::swap(_control, other._control);
            std::swap(_path, other._path);
            std::swap(_logs, other._logs);
            std::swap(_log_same, other._log_same);
          }
          
          template <typename T>
          OutStream& operator<<(const T& data) {
            *_stream << data;
            return *this;
          }
          
          OutStream& operator<<(std::ostream& (*manipulator)(std::ostream&)) {
            manipulator(*_stream);
            return *this;
          }
          
          void print() { return; }
          
          template <typename T>
          void print(const T& data) { this->operator<<(data); }
          
          void println() { this->operator<<(std::endl); }
          
          template <typename T>
          void println(const T& data) { print(data); this->operator<<(std::endl); }
          
          template <typename T, typename... Args>
          void print(const T& data, const Args&... args) {
            print(data);
            print(args...);
          }
          
          template <typename T, typename... Args>
          void println(const T& data, const Args&... args) {
            print(data);
            println(args...);
          }
          
          template <typename... Args>
          bool same_log(const Args&... args) {
            if (!__enable_log_same()) {
              if (__has_same_logs(args...)) return true;
              __store_logs(args...);
            }
            return false;
          }
            
        protected:
          void open(std::string path) {
            close(); 
            if (!path.empty()) {
              auto file = std::ofstream(path);
              if (file.is_open()) {
                _stream = &file;
                _control = true; 
                _path = path;
                return; 
              }
            }
            std::cerr << "Error opening file: " << path << std::endl;
            std::cerr << "Using std::cerr instead." << std::endl;
            __default_output(); 
          }
          
          void close() {
            if (_control && _stream) dynamic_cast<std::ofstream*>(_stream)->close();
          }
          
          void __default_output() {
            _stream = &std::cerr;
            _control = false;
            _path = "";
            _stream->iword(_colorize_index) = true;
          }
          
          bool __enable_log_same() {
            return _log_same || _setting::same_log_for_class;
          }
          
          template <typename... Args>
          void __store_logs(const Args&... args) {
            std::string log = tools::string_join("", args...);
            _logs.insert(log);
          }
          
          template <typename... Args>
          bool __has_same_logs(const Args&... args) {
            std::string log = tools::string_join("", args...);
            return _logs.find(log) != _logs.end();
          }
      };
      
      template <typename... Args>
      void __fail_msg(OutStream& out, Args... args) {
        out.println(_fail, " ", args...);
        exit(EXIT_FAILURE);
      }
      
      template <typename... Args>
      void __success_msg(OutStream& out, Args... args) {
        if (!out.same_log(_success, " ",args...))
          out.println(_success, " ", args...);
        return;
      }
      
      template <typename... Args>
      void __info_msg(OutStream& out, Args... args) {
        if (!out.same_log(args...))
          out.println(args...);
        return;
      }
      
      template <typename... Args>
      void __warn_msg(OutStream& out, Args... args) {
        if (!out.same_log(_warn, " ", args...))
          out.println(_warn, " ", args...);
        return;
      }
      
      template <typename... Args>
      void __error_msg(OutStream& out, Args... args) {
        out.println(_error, " ", args...);
        exit(EXIT_FAILURE);
      }

      void __endl(OutStream& out, int count = 1) {
        for (int i = 1; i <= count; i++) out.println();
      }
      
      OutStream _defl; // default_log
    } // namespace _msg
} // namespace generator

#endif // !_SGPCET_LOGGER_H_
