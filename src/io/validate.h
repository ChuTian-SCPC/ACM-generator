#ifndef _SGPCET_VALIDATE_H_
#define _SGPCET_VALIDATE_H_

#ifndef _SGPCET_REPORTER_H_
#include "reporter.h"
#endif // _SGPCET_REPORTER_H_

namespace generator {
    namespace io {
        class _Validate : public _Reporter {
        protected:
            using TestResults = std::map<int, ReturnState>;
            _Program* _validator;
            TestResults _states;
            bool _default_judge;
            std::string _case_name;
            int _time_limit;
        public:
            _Validate() : _Reporter(), _validator(nullptr), _default_judge(true), _case_name(_setting::testcase_folder), _time_limit(_setting::time_limit_inf) {}

            template<typename T>
            _Validate(T&& validator, const std::string& case_name = _setting::testcase_folder, bool default_judge = true) : _Reporter(), _validator(nullptr), _default_judge(default_judge), _case_name(case_name) {
                __set_validator(std::forward<T>(validator));
            }
            ~_Validate() {
                if (_validator) delete _validator;
            }

            _GET_VALUE(_Program*, validator);
            _GET_VALUE(TestResults, states); 
            _SET_GET_VALUE(bool, default_judge);
            _SET_GET_VALUE(std::string, case_name);
            _SET_GET_VALUE(int, time_limit);

            template<typename T>
            void __set_validator(T&& validator) {
                if (_validator) delete _validator;
                _validator = __result_program(std::forward<T>(validator));
            }

            void __add_validate(int id) {
                if (!__testcase_input_file_exists(id)) return;
                _default_judge = false;
                _states[id] = ReturnState();
            }

            void __add_validate(int start, int end) {
                for (int i = start; i <= end; i++) __add_validate(i);
            }

            void __run() {
                _validator->__check_program_valid();
                Path folder = __validate_folder(_case_name);
                __create_directories(folder);
                if (_default_judge) {
                    for (auto& it : __get_all_inputs())
                        _states[it] = ReturnState();
                }
                int id = 1;
                for (auto& it : _states) {
                    Path log = __path_join(folder, __end_with(it.first, _enum::_VAL));
                    Path input = __input_file_path(__path_join(__current_path(), _case_name), it.first);
                    _msg::__flash_msg(_msg::_defl, "Validate : ", __ratio_msg(id, _states.size()));
                    id++;
                    it.second = _validator->__run_program(input, _setting::_default_path, log, _time_limit, _enum::_VALIDATOR);
                }
                _msg::__endl(_msg::_defl);
            }

            void __short_summary(_msg::OutStream& out) {
                int passed = 0;
                for (auto& state : _states) {
                    if (__is_success(state.second.exit_code))
                        passed++;
                }
                if (passed == _states.size()) __all_pass(out);
                else __pass_ratio_msg(out, passed, _states.size());
            }

            void __detail_summary(_msg::OutStream& out) {
                std::vector<Path> error_files;
                Path folder = __validate_folder(_case_name);
                for (auto& state : _states) {
                    if (!__is_success(state.second.exit_code))
                        error_files.push_back(__input_file_path(__path_join(__current_path(), _case_name), state.first));
                }
                if (error_files.empty()) __all_pass(out);
                else __meets_error_files(out, error_files);
            }
        };

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        validate(int start, int end, T program, std::string case_name = _setting::testcase_folder) {
            _Validate validator(std::forward<T>(program), case_name);
            validator.__add_validate(start, end);
            validator.__run();
            validator.__detail_summary(_msg::_defl);
        }

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        validate(T program, std::string case_name = _setting::testcase_folder) {
            _Validate validator(std::forward<T>(program), case_name);
            validator.__run();
            validator.__detail_summary(_msg::_defl);
        }
    } // namespace io
} // namespace generator

#endif // !_SGPCET_VALIDATE_H_
