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
    int vector_limit = 10000000; // 1e7
    int node_limit = 10000000;
    int edge_limit = 10000000;
    long long edge_count_inf = -1;
    long long auto_edge_limit = -2;

    int _auto_int = -2;

    long long count_range_inf = -1;
    
    int time_limit_inf = -1; 
    int _time_limit_auto = -2;
    int test_case_limit = 1000;
    int _extra_run_time = 100;

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
  #elif defined(__APPLE__)
    std::string _sub_checker_folder = "macos";
  #else
    std::string _sub_checker_folder = "linux";
  #endif // ON_WINDOWS
    
    std::string _lib_folder = io::__lib_path();
    std::string testcase_folder = "testcases";
    std::string compare_folder = "cmp";
    std::string hack_folder = "hack";
    std::string validate_folder = "validate";
    std::string _empty_program_name = "";

    std::string input_suffix = ".in";
    std::string output_suffix = ".out";
    
    std::string _first_generator_argv = "generator";
    std::string _first_checker_argv = "checker";
    std::string _first_validator_argv = "validator";
    
    double time_limit_over_ratio = 2;
    double time_limit_check_ratio = 2;

    const int _error_return = -1;

    bool default_seed = true;
    std::string default_stable_seed = "";
    std::string default_hack_stable_seed = "hack";

    int _rand_sum_sum_limit = 1000000;

    const double PI = 3.14159265358979323846;

  };// namespace _setting
  
};// namespace generator

#endif // !_SGPCET_SETTING_H_
