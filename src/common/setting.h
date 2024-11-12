#ifndef _SGPCET_SETTING_H_
#define _SGPCET_SETTING_H_

#ifndef _SGPCET_COMMON_H_
#include "common.h"
#endif // !_SGPCET_COMMON_H_

namespace generator {
  namespace io {
    std::string __lib_path() {
        return __FILE__;
    }
  }; // namespace io
  
  namespace _setting {
    int _vector_limit = 10000000; // 1e7
    int _node_limit = 10000000;
    int _edge_limit = 10000000;
    
    int time_limit_inf = -1; 
    int _test_case_limit = 1000;

  #ifdef ON_WINDOWS
    char _path_split = '\\';
    char _other_split = '/';
  #else
    char _path_split = '/';
    char _other_split = '\\';
  #endif // ON_WINDOWS
    
    const char* _default_path = "";
  
    std::string _checker_folder = "checker";
  #ifdef ON_WINDOWS
    std::string _sub_checker_folder = "windows";
  #else
    std::string _sub_checker_folder = "linux";
  #endif // ON_WINDOWS
    
    std::string _lib_folder = io::__lib_path();
    std::string _testcase_folder = "testcases";
    std::string _compare_folder = "cmp";
    std::string _hack_folder = "hack";
    std::string _validate_folder = "validate";
    
    int _function_count = 1;
    
    bool same_log_for_class = false;
    
    std::string _first_generator_argv = "generator";
    std::string _first_checker_argv = "checker";
    std::string _first_validator_argv = "validator";
    
    double time_limit_over_ratio = 2;
    double time_limit_check_ratio = 2;

    const int _error_return = -1;
  };// namespace _setting
  
};// namespace generator

#endif // !_SGPCET_SETTING_H_
