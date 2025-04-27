#ifndef _SGPCET_TOOLS_H_
#define _SGPCET_TOOLS_H_

#ifndef _SGPCET_COMMON_H_
#include "common.h"
#endif // !_SGPCET_COMMON_H_

namespace generator {
  namespace tools {
    template <typename T>
    void __join_helper(std::ostringstream& oss, std::string, const T& data) {
      oss << data;
    }
    
    template <typename T, typename... Args>
    void __join_helper(std::ostringstream& oss, std::string split, const T& data, const Args&... args) {
        oss << data; 
        oss << split; 
        __join_helper(oss, split, args...);
    }
    
    template <typename... Args>
    std::string string_join(std::string split, const Args&... args) {
        std::ostringstream oss;
        __join_helper(oss, split, args...);
        return oss.str();
    }
    
    template <typename... Args>
    std::string string_join(char split, const Args&... args) {
        std::ostringstream oss;
        __join_helper(oss, std::string(1, split), args...);
        return oss.str();
    }

    std::string string_format(const char* format, ...) {
        FMT_TO_RESULT(format, format, _format);
        return _format;
    }
    
  } // namespace tools
} // namespace generator

#endif // !_SGPCET_TOOLS_H_
