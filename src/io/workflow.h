#ifndef _SGPCET_WORKFLOW_H_
#define _SGPCET_WORKFLOW_H_

#ifndef _SGPCET_INPUT_H_
#include "input.h"
#endif // !_SGPCET_INPUT_H_
#ifndef _SGPCET_OUTPUT_H_
#include "output.h" 
#endif // !_SGPCET_OUTPUT_H_
#ifndef _SGPCET_VALIDATE_H_
#include "validate.h"
#endif // !_SGPCET_VALIDATE_H_
#ifndef _SGPCET_CHECKER_H_
#include "checker.h"
#endif // !_SGPCET_CHECKER_H_
#ifndef _SGPCET_COMPARE_H_
#include "compare.h"
#endif // !_SGPCET_COMPARE_H_
#ifndef _SGPCET_HACK_H_
#include "hack.h"
#endif // !_SGPCET_HACK_H_

namespace generator {
    namespace io {
        class Workflow : public _Reporter {
        protected:
            using Name = std::string;
            using TestCase = std::pair<Name, int>;
            using Time = int;

            std::map<Name, _Program*> _programs;
            Name _std;
            Name _checker;
            Name _validator;
            
            _Input _input;
            std::map<Name, _Hack> _hacks;
            struct _CompareCase {
                int _start, _end;
                _Program* _program;

                _CompareCase(_Program* program) : _start(_setting::_auto_int), _end(_setting::_auto_int), _program(program) {}

                _CompareCase(int start, int end, _Program* program) : _start(start), _end(end), _program(program) {}

            };

            std::vector<_CompareCase> _compares;

            Time _time_limit;
            Time _time_limit_for_std;
            Time _time_limit_for_generator;
            Time _time_limit_for_checker;
            Time _time_limit_for_validator;

            bool _delete_fail_testcase;

            // output
            bool _cover_exist;

            // hack
            bool _copy_wrong_to_testcase;
            bool _delete_correct;

            // log
            bool _detail_report_on_console;
            bool _detail_report_on_file;
            _msg::OutStream _file;

            enum class State {
                UNKNOWN,
                GEN_FAIL,
                VAL_FAIL,
                STD_TLE,
                STD_RE,
                SUCCESS
            };

            std::map<int, State> _states;
        public:
            Workflow(int time_limit = _setting::time_limit_inf) :
                _Reporter(),
                _std(_setting::_empty_program_name),
                _checker(""),
                _validator(_setting::_empty_program_name),
                _input(),
                _time_limit(time_limit),
                _time_limit_for_std(_setting::time_limit_inf),
                _time_limit_for_generator(_setting::time_limit_inf),
                _time_limit_for_checker(_setting::time_limit_inf),
                _time_limit_for_validator(_setting::time_limit_inf),
                _delete_fail_testcase(true),
                _cover_exist(true),
                _copy_wrong_to_testcase(true),
                _delete_correct(true),
                _detail_report_on_console(false),
                _detail_report_on_file(true),
                _file(Path("./summary.log"))
            {
                init_gen();
                set_checker(_enum::Checker::lcmp);
            }
            
            ~Workflow() {
                for (auto& name : _programs) {
                    if (name.second != nullptr) delete name.second;
                }
            }

            _GET_VALUE(Name, std)
            _GET_VALUE(Name, checker)
            _GET_VALUE(Name, validator)
            _SET_GET_VALUE(Time, time_limit)
            _SET_GET_VALUE(Time, time_limit_for_std)
            _SET_GET_VALUE(Time, time_limit_for_generator)
            _SET_GET_VALUE(Time, time_limit_for_checker)
            _SET_GET_VALUE(Time, time_limit_for_validator)
            _SET_GET_VALUE(bool, delete_fail_testcase)
            _SET_GET_VALUE(bool, cover_exist)
            _SET_GET_VALUE(bool, copy_wrong_to_testcase)
            _SET_GET_VALUE(bool, delete_correct)
            _SET_GET_VALUE(bool, detail_report_on_console)
            _SET_GET_VALUE(bool, detail_report_on_file)

            template<typename T>
            void add_program(T&& program) {
                __add_program(std::forward<T>(program));
            }

            template<typename N, typename T>
            typename std::enable_if<std::is_convertible<N, std::string>::value, void>::type
            add_program(N&& name, T&& program) {
                __add_program(std::string(std::forward<N>(name)), std::forward<T>(program));
            }

            template<typename T>
            void set_std(T&& std) {
                _Program* program = __find_program(std::forward<T>(std));
                _std = program->name();
            }

            template<typename N, typename T>
            typename std::enable_if<std::is_convertible<N, std::string>::value, void>::type
            set_std(N&& name, T&& std) {
                _Program* program = __add_program(std::string(std::forward<N>(name)), std::forward<T>(std));
                _std = program->name();
            }

            template<typename T>
            void set_checker(T&& checker) {
                _Program* program = __find_program(std::forward<T>(checker));
                _checker = program->name();
            }

            template<typename N, typename T>
            typename std::enable_if<std::is_convertible<N, std::string>::value, void>::type
            set_checker(N&& name, T&& checker) {
                _Program* program = __add_program(std::string(std::forward<N>(name)), std::forward<T>(checker));
                _checker = program->name();
            }

            template<typename T>
            void set_validator(T&& validator) {
                _Program* program = __find_program(std::forward<T>(validator));
                _validator = program->name();
            }

            template<typename N, typename T>
            typename std::enable_if<std::is_convertible<N, std::string>::value, void>::type
            set_validator(N&& name, T&& validator) {
                _Program* program = __add_program(std::string(std::forward<N>(name)), std::forward<T>(validator));
                _validator = program->name();
            }

            template<typename T>
            void make_inputs(int start, int end, T&& program) {
                __make_inputs(start, end, std::forward<T>(program));
            }

            template<typename T>
            void make_inputs(int index, T&& program) {
                __make_inputs(index, index, std::forward<T>(program));
            }

            template<typename T>
            void fill_inputs(int sum, T&& program) {
                __fill_inputs(sum, std::forward<T>(program));
            }

            template<typename T>
            void fill_inputs(T&& program) {
                __fill_inputs(1, std::forward<T>(program));
            }

            template<typename...Args>
            void compare(Args&&... args) {
                __add_compare(std::forward<Args>(args)...);
            }

            template<typename G, typename T>
            void hack(G&& generator, T&& program, int max_try = 100, bool stop_when_wrong = true, int start_index = _setting::_auto_int) {
                _Program *g = __find_program(std::forward<G>(generator));
                _Program *p = __find_program(std::forward<T>(program));
                if (start_index == _setting::_auto_int) 
                    _hacks[g->name()].__add_comparer(p, max_try, stop_when_wrong);
                else
                    _hacks[g->name()].__add_comparer(p, max_try, start_index, stop_when_wrong);
            }

            void run() {
                __begin_summary_report();
                if (!__check_requires()) return;
                __generate_inputs();
                __validate();
                __generate_outputs();
                __clean_failed_testcase();
                __hack();
                __compare();
                _msg::__info_msg(_msg::_defl, tools::string_format("Report is in file %s", Path(_file.path()).full_path().cname()));
            }

        protected:
            void __check_program_valid(const Name& name, _Program* p) {
                if (!p->__check_program_valid()) {
                    _msg::__fail_msg(_msg::_defl, tools::string_format("program %s is not valid", name.c_str()));
                }
            }

            template<typename T>
            typename std::enable_if<!IsDefaultChecker<T>::value, _Program*>::type
            __add_program(const Name& name, T&& program) {
                _Program* p = __program(std::forward<T>(program));
                __check_program_valid(name, p);
                if (_programs.find(name) != _programs.end()) delete _programs[name];
                p->set_name(name);
                _programs[name] = p;
                return p;
            }

            template<typename T>
            typename std::enable_if<IsDefaultChecker<T>::value, _Program*>::type
            __add_program(const Name& name, T&& program) {
                _Program* p = __checker_program(std::forward<T>(program));
                __check_program_valid(name, p);
                if (_programs.find(name) != _programs.end()) delete _programs[name];
                p->set_name(name);
                _programs[name] = p;
                return p;
            }

            template<typename T>
            typename std::enable_if<!IsDefaultChecker<T>::value, _Program*>::type
            __add_program(T&& program) {
                _Program* p = __program(std::forward<T>(program));
                Name name = p->name();
                __check_program_valid(name, p);
                if (_programs.find(name) != _programs.end()) delete _programs[name];
                _programs[name] = p;
                return p;
            }

            template<typename T>
            typename std::enable_if<IsDefaultChecker<T>::value, _Program*>::type
            __add_program(T&& program) {
                _Program* p = __checker_program(std::forward<T>(program));
                Name name = p->name();
                __check_program_valid(name, p);
                if (_programs.find(name) != _programs.end()) delete _programs[name];
                _programs[name] = p;
                return p;
            }

            _Program* __find_program_by_name(const Name& name) {
                if (_programs.find(name) == _programs.end()) return nullptr;
                return _programs[name];
            }

            template<typename T>
            typename std::enable_if<IsDefaultChecker<T>::value, _Program*>::type
            __find_program(T&& index) {
                _Program* p = __find_program_by_name(checker_name[index]);
                if (p != nullptr) return p;
                p = __checker_program(std::forward<T>(index));
                return __add_program(p);
            }

            template<typename T>
            typename std::enable_if<std::is_convertible<T, std::string>::value, _Program*>::type
            __find_program(T&& name) {
                _Program* p = __find_program_by_name(std::string(name));
                if (p != nullptr) return p;
                return __add_program(std::forward<T>(name));
            }
            
            template<typename T>
            typename std::enable_if<!IsDefaultChecker<T>::value && !std::is_convertible<T, std::string>::value, _Program*>::type
            __find_program(T&& name) {
                return __add_program(std::forward<T>(name));
            }

            template<typename T>
            void __make_inputs(int start, int end, T&& program) {
                _Program* p = __find_program(std::forward<T>(program));
                for (int i = start; i <= end; i++)
                    _input.__add_input(i, p);
            }

            template<typename T>
            void __fill_inputs(int count, T&& program) {
                _Program* p = __find_program(std::forward<T>(program));
                _input.__fill_input(count, p, true);
            }

            void __add_compare() {}

            template<typename T, typename... Args>
            typename std::enable_if<
                IsProgramConstructible<T>::value &&
                ArgsAllTrue<IsProgramConstructible<Args>::value...>::value, 
                void>::type
            __add_compare(T&& program, Args&&... args) {
                _Program* p = __find_program(std::forward<T>(program));
                _compares.push_back(_CompareCase(p));
                __add_compare(std::forward<Args>(args)...);
            }

            void __add_compare(int, int) {}

            template<typename T, typename... Args>
            typename std::enable_if<
                IsProgramConstructible<T>::value &&
                ArgsAllTrue<IsProgramConstructible<Args>::value...>::value, 
                void>::type
            __add_compare(int start, int end, T&& program, Args&&... args) {
                _Program* p = __find_program(std::forward<T>(program));
                _compares.push_back(_CompareCase(start, end, p));
                __add_compare(start, end, std::forward<Args>(args)...);
            }

            template<typename T, typename... Args>
            typename std::enable_if<
                IsProgramConstructible<T>::value,
                void>::type
            __add_compare(int start, int end, T&& program, int next_start, int next_end, Args&&... args) {
                _Program* p = __find_program(std::forward<T>(program));
                _compares.push_back(_CompareCase(start, end, p));
                __add_compare(next_start, next_end, std::forward<Args>(args)...);
            }

            std::string __time_format(int time) {
                if (time == _setting::time_limit_inf) return "inf";
                return tools::string_format("%dms", time);
            }

            void __report_program(_msg::OutStream& out, const Name& name, const Name& stage_name) {
                if (name == _setting::_empty_program_name) 
                    _msg::__info_msg(out, tools::string_format("  No %s", stage_name.c_str()));
                else 
                    _msg::__info_msg(out, tools::string_format("  %s name : %s", stage_name.c_str(), name.c_str()));
            }

            std::string __bool_format(bool b) {
                return b ? "true" : "false";
            }

            void __begin_summary_report(_msg::OutStream& out, bool detail, bool console) {
                if (!detail) {
                    if (console) _msg::__info_msg(out, "Start Workflow :");
                    _msg::__info_msg(out, tools::string_format("  Programs Count : %d", _programs.size()));
                }
                else {
                    _Table table(out);
                    table.add_titles({"Program Name", "Path"});
                    int count = 1;
                    for (auto& p : _programs) {
                        table.add_cell(0, count, p.first);
                        table.add_cell(1, count, p.second->__path_or_func());
                        count++;
                    }
                    table.draw();
                    __report_program(out, _std, "Standard Program");
                    __report_program(out, _checker, "Checker");
                    __report_program(out, _validator, "Validator");                    
                }
                _msg::__info_msg(out, tools::string_format("  Test Cases Count : %d", _input.__size()));
                int hack_size = 0;
                for (auto& h : _hacks) hack_size += h.second.__size();  
                _msg::__info_msg(out, tools::string_format("  Hack Programs Count : %d", hack_size));
                _msg::__info_msg(out, tools::string_format("  Compare Programs Count : %d", _compares.size()));
                _msg::__info_msg(out, "  Time Limit:");
                _msg::__info_msg(out, tools::string_format("    std       : %s", __time_format(_time_limit_for_std).c_str()));
                _msg::__info_msg(out, tools::string_format("    compare   : %s / %s", 
                    __time_format(_time_limit).c_str(), __time_format(__time_limit_extend(_time_limit)).c_str()));
                _msg::__info_msg(out, tools::string_format("    hack user : %s", __time_format(_time_limit).c_str()));
                _msg::__info_msg(out, tools::string_format("    generator : %s", __time_format(_time_limit_for_generator).c_str()));
                _msg::__info_msg(out, tools::string_format("    validator : %s", __time_format(_time_limit_for_validator).c_str()));
                _msg::__info_msg(out, tools::string_format("    checker   : %s", __time_format(_time_limit_for_checker).c_str()));
                if (detail) {
                    _msg::__info_msg(out, "  Setting :");
                    _msg::__info_msg(out, tools::string_format("    delete fail testcase          : %s", __bool_format(_delete_fail_testcase).c_str()));
                    _msg::__info_msg(out, tools::string_format("    (output) cover exist          : %s", __bool_format(_cover_exist).c_str()));
                    _msg::__info_msg(out, tools::string_format("    (hack) copy wrong to testcase : %s", __bool_format(_copy_wrong_to_testcase).c_str()));
                    _msg::__info_msg(out, tools::string_format("    (hack) delete correct         : %s", __bool_format(_delete_correct).c_str()));
                    _msg::__info_msg(out, tools::string_format("    detail report on console      : %s", __bool_format(_detail_report_on_console).c_str()));
                    _msg::__info_msg(out, tools::string_format("    detail report on file         : %s", __bool_format(_detail_report_on_file).c_str()));
                }
                
            }

            void __begin_summary_report() {
                __begin_summary_report(_msg::_defl, _detail_report_on_console, true);
                __begin_summary_report(_file, _detail_report_on_file, false);
                _msg::__endl(_file);
            }

            void __info_msg(const std::string& msg) {
                _msg::__info_msg(_msg::_defl, msg);
                _msg::__info_msg(_file, msg);
            }

            void __warn_msg(const std::string& msg) {
                _msg::__warn_msg(_msg::_defl, msg);
                _msg::__warn_msg(_file, msg);
            }

            bool __check_requires() {
                if (_input.__empty()) {
                    __info_msg("No Generator.");
                    return false;
                }
                if (_std == _setting::_empty_program_name) {
                    __info_msg("No Standard Program.");
                    return false;
                }
                if ((!_hacks.empty() || ! _compares.empty()) &&
                    _checker == _setting::_empty_program_name) {
                    __info_msg("No Checker.");
                    return false;
                }
                return true;
            }

            void __generate_inputs() {
                _input.set_time_limit(_time_limit_for_generator);
                _input.__run();
                if (_detail_report_on_console) _input.__detail_summary(_msg::_defl);
                else _input.__short_summary(_msg::_defl);
                _msg::__info_msg(_file, "Generate(Inputs) :");
                if (_detail_report_on_file) _input.__detail_summary(_file);
                else _input.__short_summary(_file);
                _msg::__endl(_file);
                auto states = _input.states();
                for (auto& s : states) {
                    if (!__is_success(s.second.exit_code)) _states[s.first] = State::GEN_FAIL;
                    else _states[s.first] = State::SUCCESS;
                }
            }

            void __validate() {
                if (_validator == _setting::_empty_program_name) {
                    __warn_msg("You'd better have a Validator to ensure the generated testcases are valid.");
                    return;
                }
                _Validate validator;
                validator.__set_validator(__find_program(_validator));
                validator.set_time_limit(_time_limit_for_validator);
                for (auto& s : _states) {
                    if (s.second == State::SUCCESS) validator.__add_validate(s.first);
                }
                validator.__run();
                if (_detail_report_on_console) validator.__detail_summary(_msg::_defl);
                else validator.__short_summary(_msg::_defl);
                _msg::__info_msg(_file, "Validate :");
                if (_detail_report_on_file) validator.__detail_summary(_file);
                else validator.__short_summary(_file);
                _msg::__endl(_file);
                auto states = validator.states();
                for (auto& s : states) {
                    if (!__is_success(s.second.exit_code)) _states[s.first] = State::VAL_FAIL;
                }
            }

            void __generate_outputs() {
                _Output output;
                output.__set_std(__find_program(_std));
                output.set_cover_exist(_cover_exist);
                output.set_time_limit(_time_limit_for_std);
                for (auto& s : _states) {
                    if (s.second == State::SUCCESS) output.__add_output(s.first);
                }
                output.__run();
                if (_detail_report_on_console) output.__detail_summary(_msg::_defl);
                else output.__short_summary(_msg::_defl);
                _msg::__info_msg(_file, "Generate(Outputs) :");
                if (_detail_report_on_file) output.__detail_summary(_file);
                else output.__short_summary(_file);
                _msg::__endl(_file);
                auto states = output.states();
                for (auto& s : states) {
                    if (!__is_success(s.second.exit_code)) _states[s.first] = State::STD_RE;
                    if (__time_limit_exceed(s.second.time, _time_limit_for_std)) _states[s.first] = State::STD_TLE;
                }
            }

            void __clean_failed_testcase() {
                if (!_delete_fail_testcase) return;
                for (auto& s : _states) {
                    if (s.second != State::SUCCESS) {
                        Path input = __testcase_input_file_path(s.first);
                        Path output = __testcase_output_file_path(s.first);
                        input.__delete_file();
                        output.__delete_file();
                    }
                }
            }

            void __hack() {
                if (_hacks.empty()) return;
                _msg::__info_msg(_file, "Hack :");
                int count = 1;
                for (auto& h : _hacks) {
                    _Hack& hack = h.second;
                    if (hack.__empty()) continue;
                    hack.set_time_limit(_time_limit);
                    hack.__set_generator(__find_program(h.first));
                    hack.set_time_limit_for_generator(_time_limit_for_generator);
                    hack.__set_std(__find_program(_std));
                    hack.set_time_limit_for_std(_time_limit_for_std);
                    hack.__set_checker(__find_program(_checker));
                    hack.set_time_limit_for_checker(_time_limit_for_checker);
                    hack.__set_validator(__find_program(_validator));
                    hack.set_time_limit_for_validator(_time_limit_for_validator);
                    if (_validator != _setting::_empty_program_name) {
                        hack.__set_validator(__find_program(_validator));
                        hack.set_time_limit_for_validator(_time_limit_for_validator);
                    }
                    hack.set_copy_wrong_to_testcase(_copy_wrong_to_testcase);
                    hack.set_delete_correct(_delete_correct);
                    hack.set_sub_folder_name(hack.generator()->name());
                    hack.__hack();
                    if (_detail_report_on_console) hack.__detail_summary(_msg::_defl);
                    else hack.__short_summary(_msg::_defl);
                    if (_detail_report_on_file) hack.__detail_summary(_file);
                    else hack.__short_summary(_file);
                    count++;
                }         
                _msg::__endl(_file);
            }

            void __compare() {
                if (_compares.empty()) return;
                _Compare compare;
                compare.set_time_limit(_time_limit);
                compare.__set_checker(__find_program(_checker));
                compare.set_time_limit_for_checker(_time_limit_for_checker);
                for (auto& c : _compares) {
                    if (c._start == _setting::_auto_int && c._end == _setting::_auto_int) 
                        compare.__add_testcase(c._program);
                    else
                        compare.__add_testcase(c._program, c._start, c._end);
                }
                compare.__compare();
                if (_detail_report_on_console) compare.__detail_summary(_msg::_defl);
                else compare.__short_summary(_msg::_defl);
                _msg::__info_msg(_file, "Compare :");
                if (_detail_report_on_file) compare.__detail_summary(_file);
                else compare.__short_summary(_file);
                _msg::__endl(_file);
            }

        };
    } // namespace io
} // namespace generator

#endif // _SGPCET_WORKFLOW_H_
