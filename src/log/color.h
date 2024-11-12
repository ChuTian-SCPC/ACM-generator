#ifndef _SGPCET_COLOR_H_
#define _SGPCET_COLOR_H_

#ifndef _SGPCET_SETTING_H_
#include "common/setting.h"
#endif // !_SGPCET_SETTING_H_

namespace generator {
    
    namespace _msg {
      
    #ifdef ON_WINDOWS
      WORD _color_ansi[_enum::__color_index(_enum::Color::MaxColor)] = {
        FOREGROUND_RED,
        FOREGROUND_GREEN,
        FOREGROUND_GREEN | FOREGROUND_RED,
        FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED,
        0 // default
      };
    #else
      std::string _color_ansi[_enum::__color_index(_enum::Color::MaxColor)] = {
        "\033[1;31m",
        "\033[32m",
        "\033[1;33m",
        "\033[0m"
      };
    #endif // ON_WINDOWS
    
      FILE *__get_std_stream(const std::ostream &stream) {
        if (&stream == &std::cout)
          return stdout;
        else if ((&stream == &std::cerr) || (&stream == &std::clog))
          return stderr;

        return 0;
      }

      bool __is_atty(const std::ostream &stream) {
        FILE *std_stream = __get_std_stream(stream);
        
        if (!std_stream) return false;
        
      #ifdef ON_WINDOWS
        return _isatty(_fileno(std_stream));
      #else
        return isatty(fileno(std_stream));
      #endif // ON_WINDOWS
      }
      
      static int _colorize_index = std::ios_base::xalloc();
      
      bool __is_colorized(std::ostream &stream) {
        return __is_atty(stream) || static_cast<bool>(stream.iword(_colorize_index));
      }
    
    #ifdef ON_WINDOWS
      void __win_color(std::ostream &stream, _enum::Color color) {
        if (!__is_atty(stream)) return;
        HANDLE hTerminal = INVALID_HANDLE_VALUE;
        if (&stream == &std::cout) hTerminal = GetStdHandle(STD_OUTPUT_HANDLE);
        else if (&stream == &std::cerr) hTerminal = GetStdHandle(STD_ERROR_HANDLE);
        
        if (!_color_ansi[_enum::__color_index(_enum::Color::Default)]) {
          CONSOLE_SCREEN_BUFFER_INFO info;
          if (!GetConsoleScreenBufferInfo(hTerminal, &info))
            _color_ansi[_enum::__color_index(_enum::Color::Default)] = _color_ansi[_enum::__color_index(_enum::Color::Grey)];
          else
            _color_ansi[_enum::__color_index(_enum::Color::Default)] = info.wAttributes;
        }

        SetConsoleTextAttribute(hTerminal, _color_ansi[_enum::__color_index(color)]);
      }
    #endif // ON_WINDOWS
    
      std::ostream& __color(std::ostream &stream, _enum::Color color) {
        if (__is_colorized(stream)) {
        #ifdef ON_WINDOWS
          __win_color(stream, color);
        #else
          stream << _color_ansi[_enum::__color_index(color)];
        #endif // ON_WINDOWS
        }
        return stream;
      }
      
    #define _COLOR_FUNC(func, color) \
      std::ostream& func(std::ostream& stream) { \
          return __color(stream, color); \
      }
    
      _COLOR_FUNC(__red, _enum::Color::Red)
      _COLOR_FUNC(__green, _enum::Color::Green)
      _COLOR_FUNC(__yellow, _enum::Color::Yellow)
      _COLOR_FUNC(__color_reset, _enum::Color::Reset)
      _COLOR_FUNC(__color_default, _enum::Color::Default)
    
    #undef _COLOR_FUNC
    
      class _ColorMsg {
      protected:
        std::string msg_;
        _enum::Color color_;
      public:
        _ColorMsg(std::string msg, _enum::Color color = _enum::Color::Default) : msg_(msg), color_(color) {}
        friend std::ostream& operator<<(std::ostream& os, const _ColorMsg& color_msg) {
          __color(os, color_msg.color_);
          os << color_msg.msg_;
          os << __color_reset;
          return os;
        }
      };
      
      _ColorMsg _warn("WARN", _enum::Color::Yellow);
      _ColorMsg _success("SUCCESS", _enum::Color::Green);
      _ColorMsg _fail("FAIL", _enum::Color::Red);
      _ColorMsg _error("ERROR", _enum::Color::Red);
      
      _ColorMsg _ac("AC", _enum::Color::Green);
      _ColorMsg _wa("WA", _enum::Color::Red);
      _ColorMsg _tle("TLE", _enum::Color::Yellow);
      _ColorMsg _checker_return("checker return :", _enum::Color::Red);
      
    } // namespace _msg
}

#endif // !_SGPCET_COLOR_H_
