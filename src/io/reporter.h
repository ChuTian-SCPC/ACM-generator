#ifndef _SGPCET_REPORTER_H_
#define _SGPCET_REPORTER_H_

#ifndef _SGPCET_IO_INIT_H_
#include "io_init.h"
#endif // !_SGPCET_IO_INIT_H_

namespace generator {
    namespace io {

        class _Reporter {
        protected:
            static _msg::_ColorMsg _ac_msg;
            static _msg::_ColorMsg _wa_msg;
            static _msg::_ColorMsg _tle_msg;
            static _msg::_ColorMsg _run_error_msg;
            static _msg::_ColorMsg _checker_return_msg;
            static _msg::_ColorMsg _empty_msg;
            static _msg::_ColorMsg _success_msg;
            static _msg::_ColorMsg _fail_msg;
            static _msg::_ColorMsg _error_msg;
        protected:
            _msg::_ColorMsg __state_msg(_enum::_JudgeState state, bool consider_tle) {
                if (_enum::__is_run_error(state)) return _run_error_msg;
                if (consider_tle && _enum::__has_tle(state)) return _tle_msg;
                else if (_enum::__has_ac(state)) return _ac_msg;
                else if (_enum::__has_wa(state)) return _wa_msg;
                else return _empty_msg;
            }

            void __state_msg(_msg::OutStream &out, _enum::_JudgeState state) {
                out.print(__state_msg(state, true));
                if (_enum::__is_combine_state(state)) {
                    out.print("(");
                    out.print(__state_msg(state, false));
                    out.print(")");
                }
            }

            std::string __ratio_msg(int part, int total, std::string end = "") {
                return tools::string_format("%d / %d%s", part, total, end.c_str());
            }

            void __pass_ratio_msg(_msg::OutStream&out, int passed, int total) {
                out.print( _success_msg, " : ", __ratio_msg(passed, total, ", "));
                out.print(_fail_msg, " : ", __ratio_msg(total - passed, total));
                out.println();
            }

            void __show_files(_msg::OutStream& out, std::vector<Path>& files, int indent = 2) {
                for (auto& file : files) {
                    out.println(tools::string_format("%*s%s", indent, "", file.cname()));
                }
            }

            void __all_pass(_msg::OutStream& out) {
                out.println(_msg::_ColorMsg("All Success", _enum::Color::Green));
            }

            void __meets_error(_msg::OutStream& out) {
                out.println("Meets ", _error_msg, " in :");
            }

            void __meets_error_files(_msg::OutStream& out, std::vector<Path>& files) {
                __meets_error(out);
                __show_files(out, files);
            }

            void __run_time_msg(_msg::OutStream& out, int time_used) {
                out.println(tools::string_format("Runtime = %dms", time_used));
            }

        };

        _msg::_ColorMsg _Reporter::_ac_msg = _msg::_ColorMsg("AC", _enum::Color::Green);
        _msg::_ColorMsg _Reporter::_wa_msg = _msg::_ColorMsg("WA", _enum::Color::Red);
        _msg::_ColorMsg _Reporter::_tle_msg = _msg::_ColorMsg("TLE", _enum::Color::Yellow);
        _msg::_ColorMsg _Reporter::_run_error_msg = _msg::_ColorMsg("RE", _enum::Color::Red);
        _msg::_ColorMsg _Reporter::_checker_return_msg = _msg::_ColorMsg("checker return :", _enum::Color::Red);
        _msg::_ColorMsg _Reporter::_empty_msg = _msg::_ColorMsg("", _enum::Color::Default);
        _msg::_ColorMsg _Reporter::_success_msg = _msg::_ColorMsg("Success", _enum::Color::Green);
        _msg::_ColorMsg _Reporter::_fail_msg = _msg::_ColorMsg("Fail", _enum::Color::Red);
        _msg::_ColorMsg _Reporter::_error_msg = _msg::_ColorMsg("ERROR", _enum::Color::Red);

        class _Table {
        protected:
            using Msg = _msg::_ColorMsg;
            using Cell = std::vector<Msg>;
            using Row = std::vector<Cell>;
            _msg::OutStream& _out;
            std::vector<Row> _rows;
            int _column_count;
        public:
            _Table(_msg::OutStream& out) : _out(out), _column_count(0) {}

            void new_row() {
                _rows.emplace_back();
                _rows.back().resize(_column_count);
            }

            template<typename T>
            void add_titles(const std::vector<T>& titles) {
                for (auto& title : titles) {
                    new_row();
                    add_cell(_rows.size() - 1, 0, Cell({Msg(title)}));
                }
            }
            
            template<typename T>
            void add_titles(std::initializer_list<T> titles) {
                add_titles(std::vector<T>(titles));
            }

            void add_cell(int row, int col, Cell cell) {
                if (_rows.size() <= row) _rows.resize(row + 1);
                if (_rows[row].size() <= col) {
                    _rows[row].resize(col + 1);
                    _column_count = std::max(_column_count, col + 1);
                }
                _rows[row][col] = cell;
            }

            template<typename T>
            void add_cell(int row, int col, T&& cell) {
                add_cell(row, col, Cell({Msg(std::forward<T>(cell))}));
            }

            void push_cell(Cell cell) {
                if (_rows.empty()) new_row();
                add_cell(_rows.size() - 1, _rows.back().size(), cell);
            }

            void add_msg(int row, int col, Msg msg) {
                if (_rows.size() <= row) _rows.resize(row + 1);
                if (_rows[row].size() <= col) {
                    _rows[row].resize(col + 1);
                    _column_count = std::max(_column_count, col + 1);
                }
                _rows[row][col].push_back(msg);
            }

            void add_msg(int row, int col, const std::string& msg) {
                add_msg(row, col, Msg(msg));
            }

            void add_msg(int row, int col, const char* msg) {
                add_msg(row, col, Msg(msg));
            }

            template<typename T>
            void push_msg(T&& msg) {
                add_msg(_rows.size() - 1, _rows.back().size(), std::forward<T>(msg));
            }

            int cell_size(Cell& cell) {
                int size = 0;
                for (auto& msg : cell) size += msg.size();
                return size;
            }

            int row_size(Row& row) {
                int size = 0;
                for (auto& cell : row) size = std::max(size, cell_size(cell));
                return size;
            }

            void draw() {
                std::vector<int> lens;
                for (auto& row : _rows) {
                    row.resize(_column_count);
                    lens.push_back(row_size(row));
                }
                for (int i = 0; i < _column_count * 2 + 1; i++) {
                    if (i % 2 == 0) {
                        _out.print("+");
                        for (auto& len : lens) {
                            for (int j = 0; j < len; j++) _out.print("-");
                            _out.print("+");
                        }
                    } else {
                        _out.print("|");              
                        for (int j = 0; j < _rows.size(); j++) {
                            Cell& cell = _rows[j][i / 2];
                            int size = cell_size(cell);
                            for(auto&msg : cell) _out.print(msg);
                            _out.print(tools::string_format("%*s", lens[j] - size, ""));
                            _out.print("|");
                        }
                    }
                    _out.println();
                }
            }
        };

    } // namespace io
} // namespace generator

#endif // !_SGPCET_REPORTER_H_
