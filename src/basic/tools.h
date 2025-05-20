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

    std::chrono::steady_clock::time_point now() { return std::chrono::steady_clock::now(); }

    long long duration(std::chrono::steady_clock::time_point& start) {
        auto end = now();
        long long d = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        start = now();
        return d;
    }
    
  } // namespace tools
} // namespace generator

#endif // !_SGPCET_TOOLS_H_
