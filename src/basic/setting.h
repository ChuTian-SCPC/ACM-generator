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
    
    int time_limit_inf = -1; 
    int test_case_limit = 1000;

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

    std::string input_suffix = ".in";
    std::string output_suffix = ".out";
    
    int _function_count = 0;
    
    bool same_log_for_class = false;
    
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

    struct BigNumberSetting {

      static void init_labels() {
        upper_labels();       
      }

      static void upper_labels() {
        set_labels("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
      }

      static void lower_labels() {
        set_labels("0123456789abcdefghijklmnopqrstuvwxyz");
      }

      static void set_labels(const std::vector<std::string>& l) {
        __set_labels(l);
      }

      static void set_labels(const std::string& l) {
        __set_labels(l);
      }

      static void set_labels(int idx, const std::string& l) {
        auto& _labels = labels();
        auto& _labels_map = labels_map();
        auto& _multi_label = __multi_label();
        if (_labels.size() < idx + 1) _labels.resize(idx + 1);
        else if (_labels[idx] != "") {
          if (_labels[idx].size() != 1) _multi_label--;
          _labels_map.erase(_labels[idx]);
        }
        _labels[idx] = l;
        _labels_map[l] = idx; 
        if (l.size() != 1) _multi_label++;
      }

      static void set_labels(int idx, char l) {
        set_labels(idx, std::string(1, l));
      }
    
      static std::vector<std::string>& labels() {
          static std::vector<std::string> instance;
          return instance;
      }

      static std::unordered_map<std::string, int>& labels_map() {
          static std::unordered_map<std::string, int> instance;
          return instance;
      }

      static bool is_single_label() {
        return __multi_label() == 0;
      }

      static bool has_empty_or_same_label() {
        return labels().size() != labels_map().size();
      }
    private:
      static int& __multi_label() {
        static int multi_label = 0;
        return multi_label;
      }

      template<typename T>
      static void __set_labels(const T& l) {
        auto& _labels = labels();
        auto& _labels_map = labels_map();
        auto& _multi_label = __multi_label();
        _labels.clear();
        _labels_map.clear();
        _labels.resize(l.size());
        _multi_label = 0;
        for (int i = 0; i < l.size(); i++) set_labels(i, l[i]);
      }
    };

    namespace {
      struct BigNumberSettingInitializer {
        BigNumberSettingInitializer() {
          BigNumberSetting::init_labels();
        }
      } _big_number_setting_initializer;  
    }


  };// namespace _setting
  
};// namespace generator

#endif // !_SGPCET_SETTING_H_
