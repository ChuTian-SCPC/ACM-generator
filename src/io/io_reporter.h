#ifndef _SGPCET_IO_REPORTER_H_
#define _SGPCET_IO_REPORTER_H_

#ifndef _SGPCET_IO_INIT_H_
#include "io/io_init.h"
#endif // !_SGPCET_IO_INIT_H_

namespace generator {
    namespace io {
        _msg::_ColorMsg _ac_color("AC", _enum::Color::Green);
        _msg::_ColorMsg _wa_color("WA", _enum::Color::Red);
        _msg::_ColorMsg _tle_color("TLE", _enum::Color::Yellow);
        _msg::_ColorMsg _run_error_color("RE/UNK", _enum::Color::Red);
        _msg::_ColorMsg _checker_return_color("checker return :", _enum::Color::Red);

        _msg::_ColorMsg __state_msg(_enum::_JudgeState state, bool consider_tle) {
            if (_enum::__is_run_error(state)) return _run_error_color;
            if (consider_tle && _enum::__has_tle(state)) return _tle_color;
            else if (_enum::__has_ac(state)) return _ac_color;
            else return _wa_color;
        }

        void __state_msg(_msg::OutStream &out, _enum::_JudgeState state) {
            out.print(__state_msg(state, true));
            if (_enum::__is_combine_state(state)) {
                out.print("(");
                out.print(__state_msg(state, false));
                out.print(")");
            }
        }

        void __judge_msg(_msg::OutStream &out, _enum::_JudgeState state, int case_id, int runtime, const std::string &result) {
            out.print(tools::string_format("Testcase %d : ", case_id));
            if (_enum::__is_run_error(state)) {
                out.print(_msg::_error);
                out.println(" ,meet some error,pleace check it or report.");
                return;
            }
            __state_msg(out, state);
            out.print(tools::string_format(" ,Runtime = %dms", runtime));
            if (_enum::__is_tle(state)) out.print(" (killed)");
            out.print(".");
            if (_enum::__has_wa(state)) {
                out.println(" ", _checker_return_color);
                out.print("  ", result);
            }
            out.println();
        }

        const char* __stage_name(_enum::_Stage stage) {
            if (stage == _enum::_INPUT) return "Make input";
            if (stage == _enum::_OUTPUT) return "Make output";
            if(stage == _enum::_VALID) return "Check input";
            return "UNKNOWN";
        }

        Path __stage_file(_enum::_Stage stage, int x) {
            return stage == _enum::_OUTPUT ? __testcase_output_file_path(x) : __testcase_input_file_path(x);
        }

        void __report_iov_summary_logs(std::unordered_map<int, bool>& results, _enum::_Stage stage) {
            int count = 0;
            std::vector<int> errors;
            for (auto result : results) {
                count++;
                if (!result.second) errors.emplace_back(result.first);
            }
            _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("Summary", _enum::Color::Green), " :");
            _msg::__info_msg(_msg::_defl, tools::string_format("%s files (success / all) : %d / %d.", __stage_name(stage), count - errors.size(), count));
            if (errors.size()) {
                _msg::__info_msg(_msg::_defl, "Program meets errors in files :");
                for (int i : errors) 
                    _msg::__info_msg(_msg::_defl, tools::string_format("  %s", __stage_file(stage, i).cname()));
                _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("Please Check.", _enum::Color::Red));
            } else {
                _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("All Success.", _enum::Color::Green));
            }
            _msg::__endl(_msg::_defl);
        }

        void __report_comapre_stream_logs(int case_count, _msg::OutStream& stream, std::vector<int>& results_count) {
            _msg::__info_msg(stream, "Total results :");
            int error_count = 0;
            for (_enum::_JudgeState state = _enum::_JudgeState::_UNKNOWN; state < _enum::_JudgeState::_JUDGE_STATE_MAX; ++state) {
                if (_enum::__is_run_error(state)) {
                    error_count += results_count[_enum::__state_index(state)];
                    continue;
                }
                __state_msg(stream, state);
                _msg::__info_msg(stream, tools::string_format(" : %d / %d", results_count[_enum::__state_index(state)], case_count));
            }
            __state_msg(stream, _enum::_JudgeState::_ERROR);
            _msg::__info_msg(stream, tools::string_format(" : %d / %d", error_count, case_count));
            
        }

        void __report_compare_logs(int case_count, _msg::OutStream& log, std::vector<int>& results_count) {
            __report_comapre_stream_logs(case_count, log, results_count);
            __report_comapre_stream_logs(case_count, _msg::_defl, results_count);
            _msg::__info_msg(_msg::_defl, tools::string_format("The report is in %s file.", log.path().c_str()));
            _msg::__endl(_msg::_defl);
        }
    } // namespace io
} // namespace generator

#endif // !_SGPCET_IO_REPORTER_H_
