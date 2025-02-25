#ifndef _SGPCET_VALIDATE_H_
#define _SGPCET_VALIDATE_H_

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
        __validate_impl(const std::vector<int>& indices, T program, std::string case_name) {
            _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("Validate", _enum::Color::Green));
            __check_program_valid(program);
            std::unordered_map<int, bool> results;
            _ProgramTypeT<T> validator = __validator_program(program);
            Path folder = __validate_folder(case_name);
            __create_directories(folder);
            for (int i : indices) {
                Path log = __path_join(folder, __end_with(i, _enum::_VAL));
                Path input = __input_file_path(__path_join(__current_path(), case_name), i);
                _msg::__info_msg(_msg::_defl, tools::string_format("Checking input validity : %s", input.cname()));
                ReturnState state = __run_program(validator, input, _setting::_default_path, log, 
                    _setting::time_limit_inf, _enum::_VALIDATOR);
                _msg::_ColorMsg result_msg = __is_success(state.exit_code) ? _msg::_success : _msg::_fail;
                _msg::__info_msg(_msg::_defl, "Result : ", result_msg);
                results[i] = __is_success(state.exit_code);
            }
            __report_iov_summary_logs(results, _enum::_VALID);
        }

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        validate(int start, int end, T program, std::string case_name = _setting::testcase_folder) {
            std::vector<int> indices;
            Path folder = __path_join(__current_path(), case_name);
            for(int i = start; i <= end; i++)
                if (__input_file_exists(folder, i)) indices.emplace_back(i);
            __validate_impl(indices, program, case_name);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        validate(T program, std::string case_name = _setting::testcase_folder) {
            __validate_impl(__get_all_inputs(case_name), program, case_name);
        }
    } // namespace io
} // namespace generator

#endif // !_SGPCET_VALIDATE_H_
