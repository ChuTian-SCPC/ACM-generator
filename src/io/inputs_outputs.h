#ifndef _SGPCET_INPUTS_OUTPUTS_H_
#define _SGPCET_INPUTS_OUTPUTS_H_

#ifndef _SGPCET_IO_REPORTER_H_
#include "io_reporter.h"
#endif // !_SGPCET_IO_REPORTER_H_
#ifndef _SGPCET_PROGRAM_H_
#include "program.h"
#endif //!_SGPCET_PROGRAM_H_

namespace generator {
    namespace io {

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        __make_inputs_impl(const std::vector<int>& inputs, T program) {
            __check_program_valid(program);
            __create_directories(__testcases_folder());
            std::unordered_map<int, bool> results;
            _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("Generator(Inputs)", _enum::Color::Green));
            for (int i : inputs) {
                Path input = __testcase_input_file_path(i);
                _msg::__info_msg(_msg::_defl, tools::string_format("Generating input : %s", input.cname()));
                ReturnState state = __run_program(
                    __generator_program(program, i), _setting::_default_path, input, _setting::_default_path, 
                    _setting::time_limit_inf, _enum::_GENERATOR);
                results[i] = __is_success(state.exit_code);
            }
            __report_iov_summary_logs(results, _enum::_INPUT);            
        }

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        make_inputs(int start, int end, T program) {     
            std::vector<int> inputs;
            for (int i = start; i <= end; i++) inputs.emplace_back(i);
            __make_inputs_impl(inputs, program);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        make_inputs(int index, T program) {
            __make_inputs_impl({index}, program);
        }

        std::vector<int> __find_not_exist_inputs(int sum) {
            if (sum <= 0) _msg::__fail_msg(_msg::_defl, tools::string_format("The number of inputs must be a positive number, but found %d.", sum));
            std::vector<int> inputs;
            __create_directories(__testcases_folder());
            for (int i = 1; sum; i++) {
                if (i > _setting::test_case_limit) {
                    _msg::__warn_msg(_msg::_defl, tools::string_format("Testcases are over the test_case_limit(%d).", _setting::test_case_limit));
                    return inputs;
                }
                if (!__testcase_input_file_exists(i)) {
                    sum--;
                    inputs.emplace_back(i);
                }
            }
            return inputs;
        } 

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        fill_inputs(int sum, T program) {     
            __make_inputs_impl(__find_not_exist_inputs(sum), program);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        fill_inputs(T program) {
            __make_inputs_impl(__find_not_exist_inputs(1), program);
        }

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        __make_outputs_impl(const std::vector<int>& outputs, T program, int time_limit) {
            __check_program_valid(program);
            std::unordered_map<int, bool> results;
            _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("Generator(Outputs)", _enum::Color::Green));
            for (int i : outputs) {
                Path input = __testcase_input_file_path(i);
                Path output = __testcase_output_file_path(i);
                _msg::__info_msg(_msg::_defl, tools::string_format("Generating output : %s", output.cname()));
                ReturnState state = __run_program(
                    __result_program(program), input, output, _setting::_default_path, time_limit, _enum::_RESULT);
                results[i] = __is_success(state.exit_code) && !__time_limit_exceed(state.time, time_limit);
                if (__is_error(state.exit_code)) _msg::__info_msg(_msg::_defl, _run_error_color);
                if (__time_limit_exceed(state.time, time_limit)) _msg::__info_msg(_msg::_defl, _tle_color,
                    tools::string_format(" running time(%dms) is greater than the time limit(%dms).", state.time, time_limit));
            }
            __report_iov_summary_logs(results, _enum::_OUTPUT);  
        }

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        make_outputs(int start, int end, T program, int time_limit = _setting::time_limit_inf) {     
            std::vector<int> outputs;
            for (int i = start; i <= end; i++) 
                if (__testcase_input_file_exists(i)) outputs.emplace_back(i);
            __make_outputs_impl(outputs, program, time_limit);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        make_outputs(int index, T program, int time_limit = _setting::time_limit_inf) {     
            __make_outputs_impl({index}, program, time_limit);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        fill_outputs(T program, bool cover_exist = true, int time_limit = _setting::time_limit_inf) {     
            std::vector<int> outputs;
            std::vector<int> inputs = __get_all_inputs();
            for (int i : inputs) {
                if (!cover_exist && __testcase_output_file_exists(i)) continue;
                outputs.emplace_back(i);
            }
            __make_outputs_impl(outputs, program, time_limit);
        }
    } // namespace io
} // namespace generator

#endif // !_SGPCET_INPUTS_OUTPUTS_H_
