#ifndef _SGPCET_INPUT_H_
#define _SGPCET_INPUT_H_

#ifndef _SGPCET_REPORTER_H_
#include "reporter.h"
#endif // _SGPCET_REPORTER_H_

namespace generator {
    namespace io {
        class _Input : public _Reporter {
        protected:
            using TestResults = std::map<int, ReturnState>;
            std::map<int, _Program*> _gens;
            TestResults _states;
            int _time_limit;
        public:

            _Input() : _Reporter(), _time_limit(_setting::time_limit_inf) {}

            ~_Input() {
                for (auto& gen : _gens) {
                    delete gen.second;
                }
            }

            _GET_VALUE(TestResults, states);
            _SET_GET_VALUE(int, time_limit);
            
            template<typename T>
            void __add_input(int id, T&& gen) {
                _gens[id] = __generator_program(std::forward<T>(gen), id);
            }

            template<typename T>
            void __add_input(std::vector<int>& ids, T&& gen) {
                for (auto& id : ids) __add_input(id, std::forward<T>(gen));
            }

            template<typename T>
            void __fill_input(int count, T&& gen, bool from_cache = false) {
                if (count <= 0) _msg::__fail_msg(_msg::_defl, tools::string_format("The number of inputs must be a positive number, but found %d.", count));
                __create_directories(__testcases_folder());
                for (int i = 1; count; i++) {
                    if (i > _setting::test_case_limit) {
                        _msg::__warn_msg(_msg::_defl, tools::string_format("The number of inputs must be less than test_case_limit(%d)", _setting::test_case_limit));
                        return;
                    }
                    if (from_cache && _gens.find(i) != _gens.end()) continue;
                    if (!from_cache && __testcase_input_file_path(i).__file_exist()) continue;
                    __add_input(i, std::forward<T>(gen));
                    count--;
                }
            }

            void __run() {
                __create_directories(__testcases_folder());
                _states.clear();
                int id = 1;
                for (auto& gen : _gens) {
                    Path input = __testcase_input_file_path(gen.first);
                    _msg::__flash_msg(_msg::_defl, "Generate(Inputs) : ", __ratio_msg(id, _gens.size()));
                    id++;
                    _states[gen.first] = gen.second->__run_program(_setting::_default_path, input, _setting::_default_path, _time_limit, _enum::_FuncProgramType::_GENERATOR);
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
                for (auto& state : _states) {
                    if (!__is_success(state.second.exit_code))
                        error_files.push_back(__testcase_input_file_path(state.first));
                }
                if (error_files.empty()) __all_pass(out);
                else __meets_error_files(out, error_files);
            }

            bool __empty() {
                return _gens.empty();
            }

            int __size() {
                return _gens.size();
            }
        }; 

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        make_inputs(int start, int end, T gen) {  
            _Input input;
            for (int i = start; i <= end; i++) 
                input.__add_input(i, std::forward<T>(gen));
            input.__run();
            input.__detail_summary(_msg::_defl);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        make_inputs(int index, T gen) {
            make_inputs(index, index, gen);
        }

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        fill_inputs(int sum, T gen) {     
            _Input input;
            input.__fill_input(sum, std::forward<T>(gen));
            input.__run();
            input.__detail_summary(_msg::_defl);
        }

        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        fill_inputs(T gen) {
            fill_inputs(1, gen);
        }

    }; // namespace io
}; // namespace generator

#endif // _SGPCET_INPUT_H_
