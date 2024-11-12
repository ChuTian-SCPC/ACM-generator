#ifndef _SGPCET_IO_REPORTER_H_
#define _SGPCET_IO_REPORTER_H_

#ifndef _SGPCET_IO_INIT_H_
#include "io/io_init.h"
#endif // !_SGPCET_IO_INIT_H_

namespace generator {
    namespace io {
        _msg::_ColorMsg _ac("AC", _enum::Color::Green);
        _msg::_ColorMsg _wa("WA", _enum::Color::Red);
        _msg::_ColorMsg _tle("TLE", _enum::Color::Yellow);
        _msg::_ColorMsg _checker_return("checker return :", _enum::Color::Red);

        void __judge_msg(_msg::OutStream &out, _enum::_JudgeState state, int case_id, int runtime, const std::string &result) {
            out.print(tools::string_format("Testcase %d : ", case_id));
            if (_enum::__is_run_error(state)) {
                out.print(_msg::_error);
                out.println(" ,meet some error,pleace check it or report.");
                return;
            }
            out.print(__state_msg(state, true));
            if (_enum::__is_combine_state(state)) {
                out.print("(");
                out.print(__state_msg(state, false));
                out.print(")");
            }
            out.print(tools::string_format(" ,Runtime = %dms", runtime));
            if (_enum::__is_tle(state)) out.print(" (killed)");
            out.print(".");
            if (_enum::__has_wa(state)) {
                out.println(" ", _checker_return);
                out.println("  ", result);
            }
            out.println();
        }

        const char* __stage_name(_enum::_Stage stage) {
            if (stage == _enum::_INPUT) return "Make input";
            if (stage == _enum::_OUTPUT) return "Make output";
            if(stage == _enum::_VALID) return "Check input";
            return "UNKNOWN";
        }

        const char* __stage_file(_enum::_Stage stage, int x) {
            Path path = stage == _enum::_OUTPUT ? __output_file_path(x) : __input_file_path(x);
            return path.cname();
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
                    _msg::__info_msg(_msg::_defl, tools::string_format("  %s", __stage_file(stage, i)));
                _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("Please Check.", _enum::Color::Red));
            } else {
                _msg::__info_msg(_msg::_defl, _msg::_ColorMsg("All Success.", _enum::Color::Green));
            }
            _msg::__endl(_msg::_defl);
        }
    } // namespace io
} // namespace generator

#endif // !_SGPCET_IO_REPORTER_H_
