#ifndef _SGPCET_OUTPUT_H_
#define _SGPCET_OUTPUT_H_

#ifndef _SGPCET_REPORTER_H_
#include "reporter.h"
#endif // _SGPCET_REPORTER_H_

namespace generator {
    namespace io {
        class _Output : public _Reporter {
        protected:
            using TestResults = std::map<int, ReturnState>;
            _Program* _std;
            TestResults _states;
            bool _cover_exist;
            int _time_limit;
        public:
            _Output() : _Reporter(), _std(nullptr), _cover_exist(true), _time_limit(_setting::time_limit_inf) {}

            template<typename T>
            _Output(T&& std, bool cover_exist = true, int time_limit = _setting::time_limit_inf) : _Reporter(), _std(nullptr), _cover_exist(cover_exist), _time_limit(time_limit) {
                __set_std(std::forward<T>(std));
            }

            ~_Output() {
                if (_std) delete _std;
            }

            _GET_VALUE(_Program*, std);
            _GET_VALUE(TestResults, states);
            _SET_GET_VALUE(bool, cover_exist);
            _SET_GET_VALUE(int, time_limit);

            template<typename T>
            void __set_std(T&& std) {
                if (_std) delete _std;
                _std = __result_program(std::forward<T>(std));
            }

            void __add_output(int id) {
                if (!__testcase_input_file_exists(id)) return;
                if (!_cover_exist && __testcase_output_file_exists(id)) return;
                _states[id] = ReturnState();
            }

            void __add_output(int start, int end) {
                for (int i = start; i <= end; i++) __add_output(i);
            }

            void __fill_output() {
                for (auto& it : __get_all_inputs()) __add_output(it);
            }

            void __run() {
                _std->__check_program_valid();
                int id = 1;
                for (auto& it : _states) {
                    Path input = __testcase_input_file_path(it.first);
                    Path output = __testcase_output_file_path(it.first);
                    _msg::__flash_msg(_msg::_defl, "Generate(Outputs) : ", __ratio_msg(id, _states.size()));
                    id++;
                    it.second = _std->__run_program(input, output, _setting::_default_path, _time_limit, _enum::_FuncProgramType::_RESULT);
                }
                _msg::__endl(_msg::_defl);
            }

            void __short_summary(_msg::OutStream& out) {
                std::vector<int> error_files;
                int run_time = 0;
                for (auto& state : _states) {
                    if (!__is_success(state.second.exit_code) || __time_limit_exceed(state.second.time, _time_limit))
                        error_files.push_back(state.first);
                    run_time = std::max(run_time, state.second.time);
                }
                __run_time_msg(out, run_time);
                if (error_files.empty()) __all_pass(out);
                else __meets_error_files(out, error_files, _states.size());
            }

            _msg::_ColorMsg __state_msg(ReturnState state) {
                if (!__is_success(state.exit_code)) return _run_error_msg;
                if (__time_limit_exceed(state.time, _time_limit)) return _tle_msg;
                return _success_msg;
            }

            void __detail_summary(_msg::OutStream& out) {
                _Table table(out);
                table.add_titles({"Case ID", "State", "Time Used"});
                int count = 0;
                for (auto& state: _states) {
                    count++;
                    table.add_cell(0, count, std::to_string(state.first));
                    table.add_cell(1, count, __state_msg(state.second));
                    if (!__is_success(state.second.exit_code)) continue;
                    table.add_cell(2, count, tools::string_format(" %dms", state.second.time));
                }
                table.draw();
            }
        };

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        make_outputs(int start, int end, T program, int time_limit = _setting::time_limit_inf) {
            _Output output(program, true, time_limit);
            output.__add_output(start, end);
            output.__run();
            output.__detail_summary(_msg::_defl);
        }

        template<typename T>
        typename std::enable_if<!IsProgramConstructible<T>::value, void>::type
        make_outputs(int index, T program, int time_limit = _setting::time_limit_inf) {
            make_outputs(index, index, program, time_limit);
        }

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        fill_outputs(T program, bool cover_exist = true, int time_limit = _setting::time_limit_inf) {
            _Output output(program, cover_exist, time_limit);
            output.__fill_output();
            output.__run();
            output.__detail_summary(_msg::_defl);
        }

    } 
}

#endif // _SGPCET_OUTPUT_H_
