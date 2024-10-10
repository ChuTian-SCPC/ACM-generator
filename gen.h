#ifndef SCPC_GENERATOR_USED_TESTLIB
#define SCPC_GENERATOR_USED_TESTLIB

#include"testlib.h"
#include <sstream>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <sys/stat.h>
#include <queue>
#include <stack>
#include <atomic>
#include <condition_variable>
#include <mutex>

#ifdef ON_WINDOWS
#include <direct.h>
#include <windows.h>
#include <winnt.h>
#include <winternl.h>
#include <process.h>
#include <processthreadsapi.h>
#define mkdir(dir, mode) _mkdir(dir)
#else
#include <unistd.h>
#include <limits.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/wait.h>
#endif
namespace generator{
    namespace io{
        class Path;
        class OutStream {
        public:
            OutStream() : output_stream_ptr_(&std::cerr), path_("std::cerr") {}
            OutStream(Path& path);
            OutStream(const std::string& filename) {
                open(filename);
            }

            ~OutStream() {
                close();
            }

            void open(const std::string& filename) {
                close();
                if (filename.empty()) {
                    output_stream_ptr_ = &std::cerr;
                    path_ = "std::cerr";
                }
                else{
                    file_.open(filename);
                    if (!file_.is_open()) {
                        std::cerr << "Error opening file: " << filename << std::endl;
                        output_stream_ptr_ = &std::cerr;
                        path_ = "std::cerr";
                        std::cerr << "Using std::cerr" << std::endl;
                    } else {
                        output_stream_ptr_ = &file_;
                        path_ = filename;
                    }
                }
            }

            void close() {
                if (file_.is_open()) {
                    file_.close();
                    output_stream_ptr_ = &std::cerr;
                    path_ = "std::cerr";
                }
            }

            template <typename T>
            OutStream& operator<<(const T& data) {
                *output_stream_ptr_ << data;
                return *this;
            }
            OutStream& operator<<(std::ostream& (*manipulator)(std::ostream&)) {
                manipulator(*output_stream_ptr_);
                return *this;
            }

            void printf(const char* format, ...) {
                FMT_TO_RESULT(format, format, _format);
                *output_stream_ptr_ << _format;
            }

            void println(const char* format, ...) {
                FMT_TO_RESULT(format, format, _format);
                *output_stream_ptr_ << _format << std::endl;
            }

            std::string path() {
                return path_;
            }

            const char* cpath() {
                return path_.c_str();
            }

        private:
            std::ostream* output_stream_ptr_;
            std::ofstream file_;
            std::string path_;
        };

        // error msg outstream, default on console
        // may unsafe
    #ifdef ON_WINDOWS
        OutStream _err("CON");
    #else
        OutStream _err("/dev/tty");
    #endif

        enum Color {
            Green,
            Red,
            Yellow,
            None
        };

        std::string color(const char *text, Color color) {
            std::string s = "";
            if (color == None) {
                return text;
            }    
            else if (color == Red) {
                s += "\033[1;31m";
            }
            else if (color == Green) {
                s += "\033[32m";
            }
            else if (color == Yellow) {
                s += "\033[1;33m";
            }
            s += text;
            s += "\033[0m";
            return s;
        }

        template <typename... Args>
        void __fail_msg(OutStream& out,const char* msg, Args... args) {
            out.printf("%s ", color("FAIL", Red).c_str());
            out.println(msg,args...,nullptr);
            exit(EXIT_FAILURE);
        }

        template <typename... Args>
        void __success_msg(OutStream& out,const char* msg, Args... args) {
            out.printf("%s ", color("SUCCESS", Green).c_str());
            out.println(msg,args..., nullptr);
            return;
        }

        template <typename... Args>
        void __info_msg(OutStream& out,const char* msg, Args... args) {
            out.println(msg,args..., nullptr);
            return;
        }

        template <typename... Args>
        void __warn_msg(OutStream& out,const char* msg, Args... args) {
            out.printf("%s ", color("WARN", Yellow).c_str());
            out.println(msg,args..., nullptr);
            return;
        }

        template <typename... Args>
        void __error_msg(OutStream& out,const char* msg, Args... args) {
            out.printf("%s ", color("ERROR", Red).c_str());
            out.println(msg,args...,nullptr);
            exit(EXIT_FAILURE);
        }

        std::string __color_ac(bool is_color = true) {
            return color("AC", is_color ? Green : None);
        }

        std::string __color_wa(bool is_color = true) {
            return color("WA", is_color ? Red : None);
        }

        std::string __color_tle(bool is_color = true) {
            return color("TLE", is_color ? Yellow : None);
        }

        std::string __color_tle_ac(bool is_color = true) {
            return __color_tle(is_color) + "(" + __color_ac(is_color) + ")";
        }

        std::string __color_tle_wa(bool is_color = true) {
            return __color_tle(is_color) + "(" + __color_wa(is_color) + ")";
        }

        std::string __color_run_err(bool is_color = true) {
            return color("ERROR", is_color ? Yellow : None);
        }

        void __ac_msg(OutStream& out,bool is_color,int case_id,int runtime){
            out.println(
                "Testcase %d : %s ,Runtime = %dms.",
                case_id,
                __color_ac(is_color).c_str(),
                runtime);
        }

        void __wa_msg(OutStream& out,bool is_color,int case_id,int runtime,std::string& result){
            out.printf(
                "Testcase %d : %s ,Runtime = %dms. ",
                case_id,
                __color_wa(is_color).c_str(),
                runtime);
            out.println("%s", color("checker return :", is_color ? Red : None).c_str());
            out.println("%s", result.c_str());
        }

        void __tle_ac_msg(OutStream& out,bool is_color,int case_id,int runtime){
            out.println(
                "Testcase %d : %s ,Runtime = %dms.",
                case_id,
                __color_tle_ac(is_color).c_str(),
                runtime);
        }

        void __tle_wa_msg(OutStream& out,bool is_color,int case_id,int runtime,std::string& result){
            out.printf(
                "Testcase %d : %s ,Runtime = %dms. ",
                case_id,
                __color_tle_wa(is_color).c_str(),
                runtime);
            out.println("%s", color("checker return :", is_color ? Red : None).c_str());
            out.println("%s", result.c_str());
        }

        void __tle_msg(OutStream& out,bool is_color,int case_id,int runtime){
            out.println(
                "Testcase %d : %s ,Runtime = %dms (killed).",
                case_id,
                __color_tle(is_color).c_str(),
                runtime);
        }

        void __run_err_msg(OutStream& out,bool is_color,int case_id){
            out.println(
                "Testcase %d : %s ,meet some error,pleace check it or report.",
                case_id,
                __color_run_err(is_color).c_str());
        }

#ifdef WIN32
        char _path_split = '\\';
        char _other_split = '/';
#else
        char _path_split = '/';
        char _other_split = '\\';
#endif
        template <typename U>
        struct IsPath;

        class Path {
        private:
            std::string _path;
        public:
            Path() : _path("") {}
            Path(const std::string& s) : _path(s) {}
            Path(const char *s) : _path(std::string(s)) {}
            Path(const Path& other) : _path(other._path) {}
            Path(Path&& other) noexcept : _path(std::move(other._path)) {}
            Path(std::string&& s) noexcept : _path(std::move(s)) {}
            Path& operator=(Path&& other) noexcept {
                if (this != &other) _path = std::move(other._path);
                return *this;
            }

            std::string path() const { return _path; }
            const char* cname() const { return _path.c_str(); }

            void change(std::string s) { _path = s; }
            void change(const char *s) { _path = std::string(s); }
            void change(Path other_path) { _path = other_path.path(); }
            
            bool __empty() { return _path.empty(); }
            
            bool __file_exists() {
                std::ifstream file(_path.c_str());
                return file.is_open();
            }

            bool __directory_exists() {
            #ifdef ON_WINDOWS
                struct _stat path_stat;
                if (_stat(_path.c_str(), &path_stat) != 0) {
                    return false;
                }

                return (path_stat.st_mode & _S_IFDIR) != 0;
            #else
                struct stat path_stat;
                if (stat(_path.c_str(), &path_stat) != 0)
                    return false;

                return S_ISDIR(path_stat.st_mode);
            #endif
            }
            
            void __unify_split() {
                for (auto& c : _path) {
                    if (c == _other_split) {
                        c = _path_split;
                    }
                }
            }

            Path __folder_path() {
                if(this->__directory_exists()) {
                    return Path(_path);
                }
                else {
                    __unify_split();
                    size_t pos = _path.find_last_of(_path_split);
                    if (pos != std::string::npos) {
                        return Path(_path.substr(0,pos));
                    }
                    else {
                        return Path();
                    }
                }
            }

            std::string __file_name() {
                if(!this->__file_exists()) {
                    __fail_msg(_err, "%s is not a file path or the file doesn't exist.", _path.c_str());
                    return "";
                }
                __unify_split();
                size_t pos = _path.find_last_of(_path_split);
                std::string file_full_name = pos == std::string::npos ? _path : _path.substr(pos + 1);
                size_t pos_s = file_full_name.find_first_of(".");
                std::string file_name = pos_s == std::string::npos ? file_full_name : file_full_name.substr(0, pos_s);
                return file_name;
            }

            void full() {
            #ifdef ON_WINDOWS
                char buffer[MAX_PATH];
                if (GetFullPathNameA(_path.c_str(), MAX_PATH, buffer, nullptr) == 0) {
                    __fail_msg(_err, "can't find full path :%s.", _path.c_str());
                }
            #else
                char buffer[PATH_MAX];
                if (realpath(_path.c_str(), buffer) == nullptr) {
                    __fail_msg(_err, "can't find full path :%s.", _path.c_str());
                }
            #endif
                _path = std::string(buffer);
            }

            void __delete_file() {
                if(this->__file_exists()) {
                    std::remove(_path.c_str());
                }
            }
        private:
            template <typename T>
            Path join_helper(const T &arg) const {
                std::string new_path = _path;
            #ifdef ON_WINDOWS
                if (!new_path.empty() && new_path.back() != _path_split)  {
                    new_path += _path_split;
                }
            #else
                if (new_path.empty() || new_path.back() != _path_split)  {
                    new_path += _path_split;
                }
            #endif
                return Path(new_path + arg);
            }
        public:
            Path join() const {
                return *this;
            }

            template <typename T, typename... Args>
            typename std::enable_if<!IsPath<T>::value, Path>::type
            join(const T &arg, const Args &... args) const {
                return join_helper(arg).join(args...);
            }

            template <typename T, typename... Args>
            typename std::enable_if<IsPath<T>::value, Path>::type
            join(const T &arg, const Args &... args) const {
                return join_helper(arg.path()).join(args...);
            }
        };
        
        template<typename T>
        struct IsPath {
            static constexpr bool value = std::is_same<T, Path>::value;
        };
        
        template<typename T>
        struct IsPathConstructible {
            static constexpr bool value = std::is_convertible<T, std::string>::value || IsPath<T>::value;
        };

        OutStream::OutStream(Path& path) {
            open(path.path());
        }

        template <typename T, typename... Args>
        typename std::enable_if<IsPathConstructible<T>::value, Path>::type
        __path_join(const T path, const Args &... args) {
            return Path(path).join(args...);
        }
        
        template<typename T>
        typename std::enable_if<IsPath<T>::value, void>::type
        __cmd_join_helper(std::string& s, const T& arg) {
            s += arg.path();
        }
        
        template<typename T>
        typename std::enable_if<!IsPath<T>::value, void>::type
        __cmd_join_helper(std::string& s, const T& arg) {
            s += arg;
        }
        
        template<typename T, typename... Args>
        typename std::enable_if<IsPath<T>::value, std::string>::type
        __cmd_join_helper(std::string& s, const T& arg, const Args&... args) {
            s += arg.path();
            s += " ";
            __cmd_join_helper(s, args...);
            return s;
        }
        
        template<typename T, typename... Args>
        typename std::enable_if<!IsPath<T>::value, void>::type
        __cmd_join_helper(std::string& s, const T& arg, const Args&... args) {
            s += arg;
            s += " ";
            __cmd_join_helper(s, args...);
        }
        
        template<typename... Args>   
        std::string __cmd_join(const Args&... args) {
            std::string s = "";
            __cmd_join_helper(s, args...);
            return s;
        }
        
        class CommandPath {
        public: 
            CommandPath() : _path(Path()), _args("") {}
            CommandPath(CommandPath&& other) noexcept : _path(std::move(other._path)), _args(std::move(other._args)) {}
            CommandPath(const CommandPath& other) noexcept : _path(other._path), _args(other._args) {}
            template<typename T, typename = typename std::enable_if<IsPathConstructible<T>::value>::type>
            CommandPath(T&& s) : _path(std::forward<T>(s)), _args("") {}
            template<typename T, typename = typename std::enable_if<IsPathConstructible<T>::value>::type>
            CommandPath(T&& s, std::string args) : _path(std::forward<T>(s)), _args(args) {}
            template<typename T, typename = typename std::enable_if<IsPathConstructible<T>::value>::type>
            CommandPath(T&& s, const char* format, ...) : _path(std::forward<T>(s)) {
                FMT_TO_RESULT(format, format, _format);
                _args = _format;
            }
            CommandPath& operator=(CommandPath&& other) noexcept {
                if (this != &other) { 
                    _path = std::move(other._path);
                    _args = std::move(other._args);
                }
                return *this;
            }
            
            int run() { 
                _path.full();
                int code = std::system(command().c_str());
                return code; 
            }
            
            std::string command() { return __cmd_join(_path, _args);}
            Path path() { return _path; }
            std::string args() { return _args; }
            
            Path& path_ref() { return _path; }
            std::string& args_ref() { return _args; }
            
            template<typename ...Args>
            void add_args(const Args& ...args) {
                _args = __cmd_join(_args, args...);
            }
            
            void clear_args() { _args.clear(); }
            
        protected:
            Path _path;
            std::string _args; 
        };
        
        template<typename T>
        struct IsFunctionConvertible {
            static constexpr bool value = std::is_convertible<T, std::function<void()>>::value;
        };
        
        class CommandFunc {
        public: 
            CommandFunc() : _func(nullptr), _args("") {}  
            CommandFunc(CommandFunc&& other) noexcept : _func(std::move(other._func)), _args(std::move(other._args)) {} 
            CommandFunc(const CommandFunc& other) noexcept : _func(other._func), _args(other._args) {}
            template<typename T, typename = typename std::enable_if<IsFunctionConvertible<T>::value>::type>
            CommandFunc(T&& func) : _func(std::forward<T>(func)), _args("") {}
            template<typename T, typename = typename std::enable_if<IsFunctionConvertible<T>::value>::type>
            CommandFunc(T&& func, std::string args) : _func(std::forward<T>(func)), _args(args) {}
            template<typename T, typename = typename std::enable_if<IsFunctionConvertible<T>::value>::type>
            CommandFunc(T&& func, const char* format, ...) : _func(std::forward<T>(func)) {
                FMT_TO_RESULT(format, format, _format);
                _args = _format;
            }
            CommandFunc& operator=(CommandFunc&& other) noexcept {
                if (this != &other) { 
                    _func = std::move(other._func);
                    _args = std::move(other._args);
                }
                return *this;
            }
            
            std::function<void()> func() { return _func; }
            std::string args() { return _args; }
            
            std::function<void()>& func_ref() { return _func; }
            std::string& args_ref() { return _args; }
            
            template<typename ...Args>
            void add_args(const Args& ...args) {
                _args = __cmd_join(_args, args...);
            }
            
            void clear_args() { _args.clear(); }
            
        protected:
            std::function<void()> _func;
            std::string _args; 
        };
        
        template<typename T>
        struct IsCommandPathConstructible {
            static constexpr bool value = std::is_constructible<T, CommandPath>::value;
        };
        
        template<typename T>
        struct IsCommandFuncConstructible {
            static constexpr bool value = std::is_constructible<T, CommandFunc>::value;
        };
        
        template<typename T>
        struct IsProgram {
            static constexpr bool value = IsCommandPathConstructible<T>::value || IsCommandFuncConstructible<T>::value;
        };
        
        template<typename T>
        struct IsProgramConstructible {
            static constexpr bool value = IsProgram<T>::value || IsPathConstructible<T>::value || IsFunctionConvertible<T>::value;
        };
        
        template<typename T>
        struct _ProgramType {
            using type = typename std::conditional<
                IsCommandPathConstructible<T>::value || IsPathConstructible<T>::value,   
                CommandPath,
                CommandFunc >::type;
        };

        template<typename T>
        using _ProgramTypeT = typename _ProgramType<T>::type;

        
        Path __lib_path() {
            return Path(__FILE__);
        }

        Path __current_path() {
        #ifdef ON_WINDOWS
            char buffer[MAX_PATH];
            GetModuleFileName(NULL, buffer, MAX_PATH);
        #else
            char buffer[PATH_MAX];
            ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer));
            if (length != -1) {
                buffer[length] = '\0';
            }
        #endif
            Path executable_path(buffer);
            return executable_path.__folder_path();
        }

        template <typename T>
        typename std::enable_if<IsPathConstructible<T>::value, Path>::type
        __full_path(T p) {
            Path path(p);
            path.full();
            return path;
        }
        
        bool __create_directory(Path& path) {
            if(path.__directory_exists())  return true;
            return mkdir(path.cname(),0777) == 0;
        }

        void __create_directories(Path path) {
            path.__unify_split();
            std::istringstream ss(path.path());
            std::string token;
            Path current_path("");
            while (std::getline(ss, token, _path_split)) {
                current_path = __path_join(current_path, token);
            #ifdef ON_WINDOWS
                if(current_path.path().find_first_of(_path_split) == std::string::npos && current_path.path().back() == ':') {
                    continue;
                }
            #else
                if(current_path.path().size() == 1 && current_path.path()[0] == _path_split) {
                    continue;
                }
            #endif
                if (!__create_directory(current_path)) {
                    __fail_msg(_err, "Error in creating folder : %s.",current_path.cname());
                }
            }
        }
        
        enum _End{
            _IN,
            _OUT,
            _ANS,
            _LOG,
            _LOGC,
            _EXE,
            _VAL,
            _MAX_END  
        };
        std::string _file_end[_MAX_END] = {
            ".in",
            ".out",
            ".ans",
            ".log",
            ".logc",
            ".exe",
            ".val"
        };
        
        std::string __end_with(int x, _End end) {
            return std::to_string(x) + _file_end[end];
        }
        
        std::string __end_with(const char* text, _End end) {
            return std::string(text) + _file_end[end];
        }
        
        std::string __end_with(std::string text, _End end) {
            return text + _file_end[end];
        }
        
        void __open_input_file(const char* file) {
            if (std::cin.eof())  std::cin.clear();
            if (freopen(file, "r", stdin) == NULL) {
                __error_msg(_err, "Fail to open file %s.", file);
            }
        }
        
        void __open_output_file(const char* file) {
            fflush(stdout);
            if (freopen(file, "w", stdout) == NULL) {
                __error_msg(_err, "Fail to open file %s.", file);
            }
        }
        
        void __open_error_file(const char* file) {
            fflush(stderr);
            if (freopen(file, "w", stderr) == NULL) {
                __error_msg(_err, "Fail to open file %s.", file);
            }
        }
        
        void __open_input_file(Path path) {
            __open_input_file(path.cname());
        }
        
        void __open_output_file(Path path) {
            __open_output_file(path.cname());
        }
        
        void __open_error_file(Path path) {
            __open_error_file(path.cname());
        }
        
        void __close_input_file_to_console() {
        #ifdef ON_WINDOWS
            __open_input_file("CON");
        #else
            __open_input_file("/dev/tty");
        #endif
        }
        
        void __close_output_file_to_console() {
        #ifdef ON_WINDOWS
            __open_output_file("CON");
        #else
            __open_output_file("/dev/tty");
        #endif
        }
        
        void __close_error_file_to_console() {
        #ifdef ON_WINDOWS
            __open_error_file("CON");
        #else
            __open_error_file("/dev/tty");
        #endif 
        }

        Path __testcases_folder() {
            return __path_join(__current_path(), "testcases");
        }
        
        void __ensure_file_folder(Path file) {
            __create_directories(file.__folder_path());
        }
        
        Path __input_file_path(int x) {
            return __path_join(__testcases_folder(), __end_with(x, _IN));
        }
        
        Path __output_file_path(int x) {
            return __path_join(__testcases_folder(), __end_with(x, _OUT));
        }
        
        bool __input_file_exists(int x) {
            return __input_file_path(x).__file_exists();
        }
        
        bool __output_file_exists(int x) {
            return __output_file_path(x).__file_exists();
        }

        std::vector<int> __get_all_inputs() {
            std::vector<int> inputs;
            Path folder_path = __testcases_folder();
        #ifdef ON_WINDOWS
            WIN32_FIND_DATA findFileData;
            HANDLE hFind = FindFirstFile(folder_path.join("*.in").cname(), &findFileData);

            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    Path file_path(__path_join(folder_path,findFileData.cFileName));
                    int num = std::stoi(file_path.__file_name());  
                    inputs.emplace_back(num);  
                } while (FindNextFile(hFind, &findFileData) != 0);

                FindClose(hFind);
            }
        #else
            DIR* dir = opendir(folder_path.cname());
            if (dir != nullptr) {
                struct dirent* entry;
                while ((entry = readdir(dir)) != nullptr) {
                    std::string file_name = entry->d_name;
                    if (file_name.size() >= 3 && file_name.substr(file_name.size() - 3) == ".in") {
                        int num = std::stoi(file_name.substr(0, file_name.size() - 3));
                        inputs.emplace_back(num);
                    }
                }
                closedir(dir);
            }
        #endif

            return inputs;
        } 
        
        
        void __terminate_process(void* process) {
        #ifdef ON_WINDOWS
            TerminateProcess(reinterpret_cast<HANDLE>(process), 0);           
        #else
            pid_t pid = static_cast<pid_t>(reinterpret_cast<long long>(process));
            kill(pid, SIGTERM);
        #endif
        }
        
        const int time_limit_inf = -1;
        const int _error_return = -1;
        const char* _default_path = "";
        
        bool __is_time_limit_inf(int time_limit) {
            return time_limit < 0;
        }
        
        enum _FuncProgramType {
            _GENERATOR,
            _CHECKER,
            _VALIDATOR,
            _RESULT,
            _OTHER  
        };
        
        char** __split_string_to_char_array(const char* input) {
            char** char_array = nullptr;
            char* cinput = const_cast<char*>(input);
            char* token = strtok(cinput, " ");
            int count = 0;

            while (token != nullptr) {
                char_array = (char**)realloc(char_array, (count + 1) * sizeof(char*));
                char_array[count] = strdup(token);
                ++count;
                token = strtok(nullptr, " ");
            }

            char_array = (char**)realloc(char_array, (count + 1) * sizeof(char*));
            char_array[count] = nullptr;
            return char_array;
        }
        
        int __fake_argc(char** fake_argvs) {
            int fake_argc = 0;
            while (fake_argvs[fake_argc] != nullptr) {
                ++fake_argc;
            }
            return fake_argc;    
        }
        
        char** __fake_argv(std::string stage, std::string args = ""){
            args = stage + " " + args;
            return __split_string_to_char_array(args.c_str());
        }
        
        void init_gen(int argc,char* argv[]) {
            registerGen(argc, argv, 1);
        }

        void init_gen() {
            char** fake_argv = __fake_argv("generator");
            int fake_argc = __fake_argc(fake_argv);
            registerGen(fake_argc, fake_argv , 1);
        }
        
        void __set_generator_args(std::string args) {
            char** fake_argv = __fake_argv("generator", args);
            int fake_argc = __fake_argc(fake_argv);
            registerGen(fake_argc, fake_argv , 1);
        }
        
        void __set_checker_args(std::string args) {
            char** fake_argv = __fake_argv("checker", args);
            int fake_argc = __fake_argc(fake_argv);
            registerTestlibCmd(fake_argc, fake_argv);
        }
        
        void __set_validator_args(std::string args) {
            if (args.empty()) {
                registerValidation();
            }
            else {
                char** fake_argv = __fake_argv("validator", args);
                int fake_argc = __fake_argc(fake_argv);
                registerValidation(fake_argc, fake_argv);                
            }
        }
        
        void __set_default_args() {
            __set_generator_args("");
        }
        
        void __set_args(std::string args, _FuncProgramType type) {
            switch(type) {
            case _GENERATOR:
                __set_generator_args(args);
                break;
            case _CHECKER:
                __set_checker_args(args);
                break;
            case _VALIDATOR:
                __set_validator_args(args);
                break;
            default:
                __set_default_args();
                break;
            }
        }
        
        struct ReturnState {
            int exit_code;
            int time;
        };
        
        template<typename T>
        typename std::enable_if<IsCommandFuncConstructible<T>::value, int>::type
        __run_child_program(T program, Path input, Path output, Path error, _FuncProgramType type) {
            if (!input.__empty()) __open_input_file(input);
            if (!output.__empty()) __open_output_file(output);
            if (!error.__empty()) __open_error_file(error);
            __set_args(program.args(), type);
            program.func()();
            return EXIT_SUCCESS;
        }
        
        template<typename T>
        typename std::enable_if<IsCommandPathConstructible<T>::value, int>::type
        __run_child_program(T program, Path input, Path output, Path error, _FuncProgramType) {
            if (!input.__empty()) program.add_args("<", input);
            if (!output.__empty()) program.add_args(">", output);
            if (!error.__empty()) program.add_args("2>", error);
            return program.run();
        } 

        template<typename T>
        typename std::enable_if<IsCommandFuncConstructible<T>::value, void>::type
        __close_files(Path input, Path output, Path error) {
            if (!input.__empty()) __close_input_file_to_console();
            if (!output.__empty()) __close_output_file_to_console();
            if (!error.__empty()) __close_error_file_to_console();
        }   

        template<typename T>
        typename std::enable_if<IsCommandPathConstructible<T>::value, void>::type
        __close_files(Path, Path, Path) {
            return;
        }            
        
        #ifdef ON_WINDOWS

        // unknown error for windows use fork run CommandPath
        template<typename T1, typename T2, typename T3, typename T4>
        typename std::enable_if<
            IsCommandPathConstructible<T1>::value &&
            IsPathConstructible<T2>::value &&
            IsPathConstructible<T3>::value &&
            IsPathConstructible<T4>::value, ReturnState>::type
        __run_child_process_program(T1 program, T2 input_file, T3 output_file, T4 error_file, int time_limit, _FuncProgramType func_type) {
            auto start_time = std::chrono::steady_clock::now();
            SECURITY_ATTRIBUTES sa;
            sa.nLength = sizeof(sa);
            sa.lpSecurityDescriptor = NULL;
            sa.bInheritHandle = TRUE;       
            
            HANDLE hInFile = INVALID_HANDLE_VALUE;
            HANDLE hOutFile = INVALID_HANDLE_VALUE;
            HANDLE hErrorFile = INVALID_HANDLE_VALUE;

            Path input_file_path(input_file);
            Path output_file_path(output_file);
            Path error_file_path(error_file);

            if (!input_file_path.__empty()) {
                if (!input_file_path.__file_exists())  return {_error_return, -1};
                hInFile = CreateFileA(Path(input_file).cname(),
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    &sa,
                    OPEN_EXISTING ,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL );
            }
            
            if (!output_file_path.__empty()) {
                hOutFile = CreateFileA(Path(output_file).cname(),
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_WRITE | FILE_SHARE_READ,
                    &sa,
                    CREATE_ALWAYS ,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL );                
            }

            if (!error_file_path.__empty()) {
                hErrorFile = CreateFileA(Path(error_file).cname(),
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_WRITE | FILE_SHARE_READ,
                    &sa,
                    CREATE_ALWAYS ,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL );                
            }

            PROCESS_INFORMATION pi; 
            STARTUPINFOA si;
            BOOL ret = FALSE; 
            DWORD flags = CREATE_NO_WINDOW;

            ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
            ZeroMemory(&si, sizeof(STARTUPINFOA));
            si.cb = sizeof(STARTUPINFOA); 
            si.dwFlags |= STARTF_USESTDHANDLES;
            si.hStdInput = hInFile != INVALID_HANDLE_VALUE ? hInFile : NULL;
            si.hStdError = hErrorFile != INVALID_HANDLE_VALUE ? hErrorFile : NULL;
            si.hStdOutput = hOutFile != INVALID_HANDLE_VALUE ? hOutFile : NULL;
            ret = CreateProcessA(NULL, const_cast<char *>(program.command().c_str()), NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
            if (ret) 
            {
                if (__is_time_limit_inf(time_limit))  WaitForSingleObject(pi.hProcess, INFINITE);
                else if (WaitForSingleObject(pi.hProcess, time_limit) == WAIT_TIMEOUT) __terminate_process(pi.hProcess);
                auto end_time = std::chrono::steady_clock::now();
                DWORD exitCode;
                GetExitCodeProcess(pi.hProcess, &exitCode);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                if (hInFile != INVALID_HANDLE_VALUE) CloseHandle(hInFile);
                if (hOutFile != INVALID_HANDLE_VALUE) CloseHandle(hOutFile);
                if (hErrorFile != INVALID_HANDLE_VALUE) CloseHandle(hErrorFile);
                return {(int)exitCode, (int)std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()};
            }
            else {
                if (hInFile != INVALID_HANDLE_VALUE) CloseHandle(hInFile);
                if (hOutFile != INVALID_HANDLE_VALUE) CloseHandle(hOutFile);
                if (hErrorFile != INVALID_HANDLE_VALUE) CloseHandle(hErrorFile);
                return {_error_return, -1};
            }   
        }

        // fork for windows
        // maybe not correct
        // https://gist.github.com/petrsmid/d96446beac825c8c0cf5a35240f444a8
        typedef struct _SECTION_IMAGE_INFORMATION {
            PVOID EntryPoint;
            ULONG StackZeroBits;
            ULONG StackReserved;
            ULONG StackCommit;
            ULONG ImageSubsystem;
            WORD SubSystemVersionLow;
            WORD SubSystemVersionHigh;
            ULONG Unknown1;
            ULONG ImageCharacteristics;
            ULONG ImageMachineType;
            ULONG Unknown2[3];
        } SECTION_IMAGE_INFORMATION, * PSECTION_IMAGE_INFORMATION;

        typedef struct _RTL_USER_PROCESS_INFORMATION {
            ULONG Size;
            HANDLE Process;
            HANDLE Thread;
            CLIENT_ID ClientId;
            SECTION_IMAGE_INFORMATION ImageInformation;
        } RTL_USER_PROCESS_INFORMATION, * PRTL_USER_PROCESS_INFORMATION;

        #define RTL_CLONE_PROCESS_FLAGS_CREATE_SUSPENDED	0x00000001
        #define RTL_CLONE_PROCESS_FLAGS_INHERIT_HANDLES		0x00000002
        #define RTL_CLONE_PROCESS_FLAGS_NO_SYNCHRONIZE		0x00000004

        #define RTL_CLONE_PARENT				0
        #define RTL_CLONE_CHILD					297

        typedef NTSTATUS(*RtlCloneUserProcess_f)(ULONG ProcessFlags,
            PSECURITY_DESCRIPTOR ProcessSecurityDescriptor /* optional */,
            PSECURITY_DESCRIPTOR ThreadSecurityDescriptor /* optional */,
            HANDLE DebugPort /* optional */,
            PRTL_USER_PROCESS_INFORMATION ProcessInformation);


        int fork()
        {
            DWORD parent_pid = GetCurrentProcessId();

            HMODULE mod;
            RtlCloneUserProcess_f clone_p;
            RTL_USER_PROCESS_INFORMATION process_info;
            NTSTATUS result;

            mod = GetModuleHandle("ntdll.dll");
            if (!mod)
                return -ENOSYS;

            clone_p = (RtlCloneUserProcess_f)GetProcAddress(mod, "RtlCloneUserProcess");
            if (clone_p == NULL)
                return -ENOSYS;

            /* lets do this */
            result = clone_p(RTL_CLONE_PROCESS_FLAGS_CREATE_SUSPENDED | RTL_CLONE_PROCESS_FLAGS_INHERIT_HANDLES, NULL, NULL, NULL, &process_info);

            if (result == RTL_CLONE_PARENT)
            {
                HANDLE me = 0, hp = 0, ht = 0, hcp = 0;
                DWORD pi, ti, mi;
                me = GetCurrentProcess();
                pi = (uintptr_t)process_info.ClientId.UniqueProcess;
                ti = (uintptr_t)process_info.ClientId.UniqueThread;

                hp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pi);
                ht = OpenThread(THREAD_ALL_ACCESS, FALSE, ti);

                ResumeThread(ht);
                CloseHandle(ht);
                CloseHandle(hp);
                return (int)pi;
            }
            else if (result == RTL_CLONE_CHILD)
            {
                /* fix stdio */
                FreeConsole();
                AttachConsole(parent_pid);

                return 0;
            }
            else 
                return -1;
                
            /* NOTREACHED */
            return -1;
        }

        #define WIFEXITED(status) ((status) != STILL_ACTIVE)
        #define WEXITSTATUS(status) (status)
        #define WNOHANG 1
        
        int waitpid(DWORD pid, int* status, int options) {
            HANDLE processHandle = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_INFORMATION, FALSE, pid);
            if (processHandle == NULL) {
                return -1;  
            }

            DWORD waitTime = (options & WNOHANG) ? 0 : INFINITE;  

            DWORD result = WaitForSingleObject(processHandle, waitTime);
            if (result == WAIT_FAILED) {
                CloseHandle(processHandle);
                return -1;  
            }

            if (result == WAIT_TIMEOUT) {
                CloseHandle(processHandle);
                return 0;  
            }

            DWORD exitCode;
            if (GetExitCodeProcess(processHandle, &exitCode)) {
                if (status) {
                    *status = exitCode;
                }
            }

            CloseHandle(processHandle);
            return pid;
        }
        #else
        #endif
        
        template<typename T1, typename T2, typename T3, typename T4>
        typename std::enable_if<
        #ifdef ON_WINDOWS
            IsCommandFuncConstructible<T1>::value &&
        #else
            IsProgram<T1>::value &&
        #endif   
            IsPathConstructible<T2>::value &&
            IsPathConstructible<T3>::value &&
            IsPathConstructible<T4>::value, ReturnState>::type
        __run_child_process_program(T1 program, T2 input_file, T3 output_file, T4 error_file, int time_limit, _FuncProgramType func_type) {
            auto start_time = std::chrono::steady_clock::now();
            pid_t pid = fork();
            Path input_file_path(input_file);
            Path output_file_path(output_file);
            Path error_file_path(error_file);            
            if (pid == 0) {
                __set_default_args();
                int exit_code = __run_child_program(program, input_file_path, output_file_path, error_file_path, func_type);
                __close_files<T1>(input_file_path, output_file_path, error_file_path);
                exit(exit_code);           
            } 
            else if (pid > 0) {

                __set_default_args();
                auto limit = std::chrono::milliseconds(time_limit);

                int status;
                
                if (__is_time_limit_inf(time_limit)) {
                    waitpid(pid, &status, 0);
                }
                else {
                    auto result = waitpid(pid, &status, WNOHANG);
                    while (result == 0 && std::chrono::steady_clock::now() - start_time < limit) {
                    #ifdef ON_WINDOWS
                        Sleep(10);
                    #else
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    #endif
                        result = waitpid(pid, &status, WNOHANG);
                    }     
                    if (result == 0) {
                        __terminate_process(reinterpret_cast<void*>(pid));
                    }   
                    result = waitpid(pid, &status, WNOHANG);              
                }
                
                auto end_time = std::chrono::steady_clock::now();              
                int exit_status = WEXITSTATUS(status);
                if (WIFEXITED(status) && exit_status == -1) {     
                    __warn_msg(_err, "Fail to run program or something error.");
                    return {_error_return, -1};
                } else {
                    return {exit_status, (int)std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()};   
                }    
                     
            } 
            else {
                __warn_msg(_err, "Fail to fork.");
                return {_error_return, -1};
            }            
        }    
        
        template<typename T1, typename T2, typename T3, typename T4>
        typename std::enable_if<
            IsProgram<T1>::value &&
            IsPathConstructible<T2>::value &&
            IsPathConstructible<T3>::value &&
            IsPathConstructible<T4>::value, ReturnState>::type
        __run_program(T1 program, T2 input_file, T3 output_file, T4 error_file, int time_limit, _FuncProgramType func_type) {
            __check_program_valid(program);
            return __run_child_process_program(program, input_file, output_file, error_file, time_limit, func_type);
        }
          
        bool default_seed = true;
        std::string default_stable_seed = "";
        
        bool __is_error(int return_code) {
            return return_code == _error_return;
        }
        
        bool __is_success(int return_code) {
            return return_code == EXIT_SUCCESS;
        }
        
        template<typename T>
        typename std::enable_if<IsCommandPathConstructible<T>::value, void>::type
        __check_program_valid(T program) {
            program.path().full();
            if (!program.path().__file_exists()) {
                __error_msg(_err, "File %s not exist.", program.path().cname());
            }
        }
        
        template<typename T>
        typename std::enable_if<IsCommandFuncConstructible<T>::value, void>::type
        __check_program_valid(T) {
            return;
        }
        
        template<typename T>
        typename std::enable_if<IsPathConstructible<T>::value, void>::type
        __check_program_valid(T program) {
            Path program_path(program);
            program_path.full();
            if (!program_path.__file_exists()) {
                __error_msg(_err, "File %s not exist.", program_path.cname());
            }
        }
        
        template<typename T>
        typename std::enable_if<IsFunctionConvertible<T>::value, void>::type
        __check_program_valid(T) {
            return;
        }
        
        std::string _input_stage = "input";
        std::string _output_stage = "output";
        
        enum _Stage {
          _INPUT,
          _OUTPUT,
          _VALID  
        };
        
        std::string __stage_name(_Stage stage) {
            if (stage == _INPUT) return "Make input";
            if (stage == _OUTPUT) return "Make output";
            if(stage == _VALID) return "Check input";
            return "UNKNOWN";
        }
        
        Path __stage_file(_Stage stage, int x) {
            return stage == _OUTPUT ? __output_file_path(x) : __input_file_path(x);
        }
        
        void __report_summary_logs(std::unordered_map<int, bool>& results, _Stage stage) {
            int count = 0;
            std::vector<int> errors;
            for (auto result : results) {
                count++;
                if (!result.second) errors.emplace_back(result.first);
            }
            __info_msg(_err, "%s :", color("Summary", Green).c_str());
            __info_msg(_err, "%s files (success / all) : %d / %d.", __stage_name(stage).c_str(), count - errors.size(), count);
            if (errors.size()) {
                __info_msg(_err, "Program meets errors in files :");
                for (int i : errors) {
                    __info_msg(_err, "  %s", __stage_file(stage, i).cname());
                }
                __info_msg(_err, "%s", color("Please Check.", Red).c_str());
            } else {
                __info_msg(_err, "%s", color("All Success.", Green).c_str());
            }
            _err.println("");
        }
        
        template<typename T>
        typename std::enable_if<IsProgram<T>::value, T>::type
        __generator_program(T program, int) {
            return program;
        }
        
        template<typename T>
        typename std::enable_if<IsFunctionConvertible<T>::value, CommandFunc>::type
        __generator_program(T program, int x) {
            std::string args = default_seed ? default_stable_seed + std::to_string(x) : "";
            return CommandFunc(program, args);
        }
        
        template<typename T>
        typename std::enable_if<IsPathConstructible<T>::value, CommandPath>::type
        __generator_program(T program, int x) {
            std::string args = default_seed ? default_stable_seed + std::to_string(x) : "";
            return CommandPath(program, args);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        __make_inputs_impl(const std::vector<int>& inputs, T program) {
            __check_program_valid(program);
            __create_directories(__testcases_folder());
            std::unordered_map<int, bool> results;
            __info_msg(_err, "%s", color("Generator(Inputs)", Green).c_str());
            for (int i : inputs) {
                Path input = __input_file_path(i);
                __info_msg(_err, "Generating input : %s", input.cname());
                ReturnState state = __run_program(
                    __generator_program(program, i), _default_path, input, _default_path, 
                    time_limit_inf, _GENERATOR);
                results[i] = __is_success(state.exit_code);
            }
            __report_summary_logs(results, _INPUT);            
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
            if (sum <= 0) __fail_msg(_err, "The quantity of inputs required must be a positive number, but found %d.", sum);
            std::vector<int> inputs;
            __create_directories(__testcases_folder());
            for (int i = 1; sum; i++) {
                if (!__input_file_exists(i)) {
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
        typename std::enable_if<IsProgram<T>::value, T>::type
        __result_program(T program) {
            return program;
        }
        
        template<typename T>
        typename std::enable_if<IsFunctionConvertible<T>::value, CommandFunc>::type
        __result_program(T program) {
            return CommandFunc(program);
        }
        
        template<typename T>
        typename std::enable_if<IsPathConstructible<T>::value, CommandPath>::type
        __result_program(T program) {
            return CommandPath(program);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        __make_outputs_impl(const std::vector<int>& outputs, T program) {
            __check_program_valid(program);
            std::unordered_map<int, bool> results;
            __info_msg(_err, "%s", color("Generator(Outputs)", Green).c_str());
            for (int i : outputs) {
                Path input = __input_file_path(i);
                Path output = __output_file_path(i);
                __info_msg(_err, "Generating output : %s", output.cname());
                ReturnState state = __run_program(
                    __result_program(program), input, output, _default_path, 
                    time_limit_inf, _RESULT);
                results[i] = __is_success(state.exit_code);
            }
            __report_summary_logs(results, _OUTPUT);  
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        make_outputs(int start, int end, T program) {     
            std::vector<int> outputs;
            for (int i = start; i <= end; i++) 
                if (__input_file_exists(i)) outputs.emplace_back(i);
            __make_outputs_impl(outputs, program);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        make_outputs(int index, T program) {     
            __make_outputs_impl({index}, program);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        fill_outputs(T program, bool cover_exist = true) {     
            std::vector<int> outputs;
            std::vector<int> inputs = __get_all_inputs();
            for (int i : inputs) {
                if (!cover_exist && __output_file_exists(i)) continue;
                outputs.emplace_back(i);
            }
            __make_outputs_impl(outputs, program);
        }
        
        enum Checker{
            lcmp,
            yesno,
            rcmp4,
            rcmp6,
            rcmp9,
            wcmp,
            MaxChecker
        };
        
        std::string checker_name[MaxChecker] = {
          "lcmp",
          "yesno",
          "rcmp4",
          "rcmp6",
          "rcmp9",
          "wcmp"  
        };
        
        Path __get_default_checker_file(Checker checker) {
            Path folder_path(__full_path(__path_join(__lib_path().__folder_path(), "checker")));
        #ifdef ON_WINDOWS
            Path checker_path(__path_join(folder_path, "windows", __end_with(checker_name[checker], _EXE)));
        #else
            Path checker_path(__path_join(folder_path, "linux", checker_name[checker]));
        #endif
            return checker_path;
        }
        
        enum ResultState {
            _UNKNOWN,
            _AC, 
            _WA, 
            _ERROR = 3,            
            _TLE = 4,
            _TLE_AC = 5, // TLE | AC
            _TLE_WA = 6, // TLE | WA
            _RESULT_STATE_MAX
        };

        inline ResultState operator|(ResultState lhs, ResultState rhs) {
            return static_cast<ResultState>(
                static_cast<std::underlying_type<ResultState>::type>(lhs) |
                static_cast<std::underlying_type<ResultState>::type>(rhs)
            );
        }

        inline ResultState& operator|=(ResultState& lhs, ResultState rhs) {
            lhs = lhs | rhs;
            return lhs;
        }
        
        template<typename T>
        struct IsDefaultChecker {
            static constexpr bool value = std::is_same<T, Checker>::value;
        };
        
        template<typename T>
        struct IsCheckerConstructible {
            static constexpr bool value = IsProgramConstructible<T>::value || IsDefaultChecker<T>::value;
        };
        
        template<typename T>
        struct _CheckerType {
            using type = typename std::conditional<
                IsCommandPathConstructible<T>::value || IsPathConstructible<T>::value ||IsDefaultChecker<T>::value,   
                CommandPath,
                CommandFunc >::type;
        };

        template<typename T>
        using _CheckerTypeT = typename _CheckerType<T>::type;
        
        template<typename T>
        typename std::enable_if<IsProgram<T>::value, T>::type
        __checker_porgram(T program) {
            return program;
        }

        template<typename T>
        typename std::enable_if<IsDefaultChecker<T>::value, CommandPath>::type
        __checker_porgram(T program) {
            CommandPath func(__get_default_checker_file(program));
            return func;
        }
        
        template<typename T>
        typename std::enable_if<IsFunctionConvertible<T>::value, CommandFunc>::type
        __checker_porgram(T program) {
            CommandFunc func(program);
            return func;
        }
        
        template<typename T>
        typename std::enable_if<IsPathConstructible<T>::value, CommandPath>::type
        __checker_porgram(T program) {
            CommandPath func(program);
            return func;
        }
        
        Path __compare_folder() {
            return __path_join(__current_path(), "cmp");
        }
        
        int _function_count = 0;
        
        template<typename T>
        typename std::enable_if<IsCommandPathConstructible<T>::value, std::string>::type
        __compare_program_name(T program) {
            return program.path().__file_name();
        }
        
        template<typename T>
        typename std::enable_if<IsCommandFuncConstructible<T>::value, std::string>::type
        __compare_program_name(T) {
            std::string name = "function" + std::to_string(_function_count);
            return name;
        }

        template<typename T>
        typename std::enable_if<IsCommandPathConstructible<T>::value, void>::type
        __increase_function_count() { return; }
        
        template<typename T>
        typename std::enable_if<IsCommandFuncConstructible<T>::value, void>::type
        __increase_function_count() { _function_count++; }
        
        template<typename T>
        Path __compare_program_folder(T program) {
            return __path_join(__compare_folder(), __compare_program_name(program));
        }
        
        bool __time_limit_exceed(int time, int time_limit) {
            return !__is_time_limit_inf(time) && time > time_limit;
        }
        
        bool __enable_judge_ans(int runtime, int time_limit, ResultState& result) {
            if (!__time_limit_exceed(runtime, time_limit)) return true;
            result = _TLE;
            return runtime <= (int)(time_limit * 1.5);
        }

        template<typename T>
        typename std::enable_if<IsProgram<T>::value, void>::type
        __check_result(Path& input_file, Path& std_output_file, Path& ans_file, Path& testlib_out_file,
            T checker, ResultState& result, std::string& testlib_result) {
            checker.add_args(input_file, ans_file, std_output_file);
            __run_program(checker, _default_path, _default_path, testlib_out_file, time_limit_inf, _CHECKER);       
            std::ifstream check_stream(testlib_out_file.path());
            std::string line;
            while(check_stream >> line){
                testlib_result += line;
                testlib_result += " ";
            }
            check_stream.close();            
            if (testlib_result.substr(0, 2) == "ok") result |= _AC;
            else result |= _WA;
        } 

        template<typename T, typename F>
        typename std::enable_if<IsProgram<T>::value && IsProgramConstructible<F>::value, void>::type
        __check_once(int id, F program, int time_limit, T checker, Path& ans_file, Path& testlib_out_file, 
            int& runtime, ResultState& result, std::string& testlib_result) {
            Path input_file = __input_file_path(id);
            Path output_file = __output_file_path(id);
            ReturnState state = __run_program(program, input_file, ans_file, _default_path, __is_time_limit_inf(time_limit) ? time_limit_inf : 2 * time_limit, _RESULT);
            if (!__is_success(state.exit_code)) {
                result = _ERROR;
                return;
            }
            runtime = state.time;
            if(__enable_judge_ans(runtime, time_limit, result)) {
                __check_result(
                    input_file, output_file, ans_file, testlib_out_file,
                    checker, result, testlib_result);
            }   
        }

        void __report_case_result(OutStream& log, int case_index, int runtime, ResultState result, std::string testlib_result) {

            if (result == _UNKNOWN || result == _ERROR) {
                __run_err_msg(_err, true, case_index);
                __run_err_msg(log, false, case_index);
            }
            if (result == _AC) {
                __ac_msg(_err, true, case_index, runtime);
                __ac_msg(log, false, case_index, runtime);
            }
            if (result == _WA) {
                __wa_msg(_err, true, case_index, runtime, testlib_result);
                __wa_msg(log, false, case_index, runtime, testlib_result);
            }
            if (result == _TLE_AC) {
                __tle_ac_msg(_err, true, case_index, runtime);
                __tle_ac_msg(log, false, case_index, runtime);
            }
            if (result == _TLE_WA) {
                __tle_wa_msg(_err, true, case_index, runtime, testlib_result);
                __tle_wa_msg(log, false, case_index, runtime, testlib_result);
            }
            if (result == _TLE) {
                __tle_msg(_err, true, case_index, runtime);
                __tle_msg(log, false, case_index, runtime);
            }
        }

        void __report_total_results(int case_count, OutStream& log, std::vector<int>& results_count) {
            __info_msg(_err, "Total results :");
            __info_msg(_err, "%s : %d / %d", __color_ac(true).c_str(), results_count[_AC], case_count);
            __info_msg(_err, "%s : %d / %d", __color_wa(true).c_str(), results_count[_WA], case_count);
            __info_msg(_err, "%s : %d / %d", __color_tle(true).c_str(), results_count[_TLE], case_count);
            __info_msg(_err, "%s : %d / %d", __color_tle_ac(true).c_str(), results_count[_TLE_AC], case_count);
            __info_msg(_err, "%s : %d / %d", __color_tle_wa(true).c_str(), results_count[_TLE_WA], case_count);
            __info_msg(_err, "%s : %d / %d", __color_run_err(true).c_str(), results_count[_UNKNOWN] + results_count[_ERROR], case_count);
            __info_msg(_err, "The report is in %s file.", log.cpath());
            _err.println("");

            __info_msg(log, "Total results :");
            __info_msg(log, "%s : %d / %d", __color_ac(false).c_str(), results_count[_AC], case_count);
            __info_msg(log, "%s : %d / %d", __color_wa(false).c_str(), results_count[_WA], case_count);
            __info_msg(log, "%s : %d / %d", __color_tle(false).c_str(), results_count[_TLE], case_count);
            __info_msg(log, "%s : %d / %d", __color_tle_ac(false).c_str(), results_count[_TLE_AC], case_count);
            __info_msg(log, "%s : %d / %d", __color_tle_wa(false).c_str(), results_count[_TLE_WA], case_count);
            __info_msg(log, "%s : %d / %d", __color_run_err(false).c_str(), results_count[_UNKNOWN] + results_count[_ERROR], case_count);
        }

        template<typename T>
        typename std::enable_if<IsProgram<T>::value, void>::type
        __compare_impl(std::map<int, int>& , int , T ) {
            return;
        } 

        template<typename T, typename F, typename... Args>
        typename std::enable_if<IsProgram<T>::value && IsProgramConstructible<F>::value, void>::type
        __compare_impl(std::map<int, int>& case_indices, int time_limit, T checker, F first, Args... args) {
            _ProgramTypeT<F> program = __result_program(first);
            __check_program_valid(program);
            __increase_function_count<_ProgramTypeT<F>>();
            Path ans_folder = __compare_program_folder(program);
            std::string program_name = __compare_program_name(program);
            __create_directories(ans_folder);
            Path testlib_out_file = __path_join(ans_folder, __end_with("__checker", _LOGC));
            int case_count = case_indices.size();
            std::vector<int> runtimes(case_count, -1);
            std::vector<ResultState> results(case_count, _UNKNOWN);
            std::vector<std::string> testlib_results(case_count);
            std::vector<int> results_count(_RESULT_STATE_MAX, 0);
            __info_msg(_err, "Test results for program %s :", color(program_name.c_str(), Green).c_str());
            Path log_path = __path_join(__compare_folder(), __end_with(program_name, _LOG));
            OutStream log(log_path); 
            for (auto cas : case_indices) {
                int real_index = cas.first;
                int vec_index = cas.second;
                Path ans_file = __path_join(ans_folder, __end_with(real_index, _ANS));
                __check_once(
                    real_index, program, time_limit, checker, ans_file, testlib_out_file,
                    runtimes[vec_index], results[vec_index], testlib_results[vec_index]);
                results_count[results[vec_index]]++;
                __report_case_result(log, real_index, runtimes[vec_index], results[vec_index], testlib_results[vec_index]);
            }
            testlib_out_file.__delete_file();
            __report_total_results(case_count, log, results_count);
            log.close();
            __compare_impl(case_indices, time_limit, checker, args...);
        }

        template<typename T, typename... Args>
        typename std::enable_if<IsCheckerConstructible<T>::value, void>::type
        compare(int start, int end, int time_limit, T checker, Args... args) {
            __info_msg(_err, "%s", color("Compare", Green).c_str());
            std::map<int, int> case_indices;
            int count = 0;
            _CheckerTypeT<T> checker_program = __checker_porgram(checker);
            for (int i = start; i <= end; i++) {
                if (__input_file_exists(i) && __output_file_exists(i)) {
                    case_indices[i] = count;
                    count ++;
                }
            }
            __compare_impl(case_indices, time_limit, checker_program, args...);
        }
        
        template<typename T, typename... Args>
        typename std::enable_if<IsCheckerConstructible<T>::value, void>::type
        compare(int time_limit, T checker, Args... args) {
            __info_msg(_err, "%s", color("Compare", Green).c_str());
            std::map<int, int> case_indices;
            int count = 0;
            _CheckerTypeT<T> checker_program = __checker_porgram(checker);
            for (int i : __get_all_inputs()) {
                if (__output_file_exists(i)) {
                    case_indices[i] = count;
                    count++;
                }
            }
            __compare_impl(case_indices, time_limit, checker_program, args...);
        }
        
        template<typename T1, typename T2>
        typename std::enable_if<IsPathConstructible<T1>::value && IsPathConstructible<T2>::value, void>::type
        __copy_file(T1 source, T2 destination) {
        #ifdef ON_WINDOWS
            std::string command = __cmd_join("copy", source, destination);
        #else
            std::string command = __cmd_join("cp", source, destination);
        #endif
            std::system(command.c_str());
        }
        
        Path __hack_folder() {
            return __path_join(__current_path(), "hack");
        }
        
        template<typename T1, typename T2, typename T3, typename T4>
        typename std::enable_if<
            IsProgramConstructible<T1>::value &&
            IsProgramConstructible<T2>::value &&
            IsProgramConstructible<T3>::value &&
            IsCheckerConstructible<T4>::value
        >::type
        hack(T1 generator_program, T2 std_program, T3 compare_program, T4 checker_program, int time_limit, 
            int max_try = 100, bool stop_when_wrong = true, bool copy_wrong_to_testcase = true, bool delete_correct = true) {
            if (max_try <= 0) {
                __fail_msg(_err, "At least try once.");
            }  
            default_stable_seed = "hack";
            __info_msg(_err, "%s", color("Hack", Green).c_str()); 
            _ProgramTypeT<T2> standard = __result_program(std_program);
            _ProgramTypeT<T3> other = __result_program(compare_program);
            _CheckerTypeT<T4> check = __checker_porgram(checker_program);
            __check_program_valid(standard);
            __check_program_valid(other);
            __check_program_valid(check);
            __create_directories(__hack_folder());
            bool all_correct = true;
            for (int i = 1; i <= max_try; i++) {
                Path input = __path_join(__hack_folder(), __end_with(i, _IN));
                Path output = __path_join(__hack_folder(), __end_with(i, _OUT));
                Path user = __path_join(__hack_folder(), __end_with(i, _ANS));
                ReturnState gen_state =__run_program(__generator_program(generator_program, i), _default_path, input, _default_path, time_limit_inf, _GENERATOR);
                if (!__is_success(gen_state.exit_code)) {
                    __fail_msg(_err, "generator meet error.");
                }
                ReturnState standard_state = __run_program(standard, input, output, _default_path, time_limit, _RESULT);
                if (!__is_success(standard_state.exit_code)) {
                    __fail_msg(_err, "std meet error.");
                }
                if (__time_limit_exceed(standard_state.time, time_limit)) {
                    __fail_msg(_err, "std using time(%dms) is greater than the timelimt(%dms).", standard_state.time, time_limit);
                }
                bool uncorrect = false;
                ReturnState compare_state = __run_program(other, input, user, _default_path, time_limit, _RESULT);
                if (!__is_success(compare_state.exit_code)) {
                    __fail_msg(_err, "compare meet error.");
                }                
                if (__time_limit_exceed(compare_state.time, time_limit)) {
                    uncorrect = true;
                    __tle_msg(_err, true, i, compare_state.time);
                }         
                else {
                    Path error_log = __path_join(__hack_folder(), __end_with("__hack", _LOGC));
                    ResultState state = _UNKNOWN;
                    std::string testlib_log;
                    __check_result(input, output, user, error_log, check, state, testlib_log);
                    if (state != _AC) {
                        uncorrect = true;
                        __wa_msg(_err, true, i, compare_state.time, testlib_log);
                    }
                    error_log.__delete_file();
                }
                if (uncorrect) {
                    all_correct = false;
                    if (copy_wrong_to_testcase) {
                        std::vector<int> next_input = __find_not_exist_inputs(1);
                        Path testcase_input = __input_file_path(next_input[0]);
                        Path testcase_output = __output_file_path(next_input[0]);
                        __copy_file(input, testcase_input);
                        __copy_file(output, testcase_output);
                        __info_msg(_err, "Standard input and output is moved to testcase folder:");
                        __info_msg(_err, "  input : %s", testcase_input.cname());
                        __info_msg(_err, "  output : %s", testcase_output.cname());
                    }
                    if (stop_when_wrong) break;
                }
                else {
                    __ac_msg(_err, true, i, compare_state.time);
                    if (delete_correct) {
                        input.__delete_file();
                        output.__delete_file();
                        user.__delete_file();
                    }
                }
            }
            if (all_correct) {
                __info_msg(_err, "Test %d case(s), %s", max_try, color("All Passed", Green).c_str());
            }
            _err.println("");
            default_stable_seed = "";
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, _ProgramTypeT<T>>::type
        __validator_program(T program) {
            return __result_program(program);
        }
        
        Path __validator_folder(std::string case_name = "testcases") {
            return __path_join(__current_path(), "validate", case_name);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        __validate_impl(const std::vector<int>& indices, T program) {
            __info_msg(_err, "%s", color("Validate", Green).c_str());
            __check_program_valid(program);
            std::unordered_map<int, bool> results;
            _ProgramTypeT<T> validator = __validator_program(program);
            Path folder = __validator_folder();
            __create_directories(folder);
            for (int i : indices) {
                Path log = __path_join(__validator_folder(), __end_with(i, _VAL));
                Path input = __input_file_path(i);
                __info_msg(_err, "Checking input validity : %s", input.cname());
                ReturnState state = __run_program(validator, input, _default_path, log, time_limit_inf, _VALIDATOR);
                __info_msg(_err, "Result : %s", __is_success(state.exit_code) ? color("SUCCESS", Green).c_str() : color("FAIL", Red).c_str());
                results[i] = __is_success(state.exit_code);
            }
            __report_summary_logs(results, _VALID);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        validate(int start, int end, T program) {
            std::vector<int> indices;
            for(int i = start; i <= end; i++)
                if (__input_file_exists(i)) indices.emplace_back(i);
            __validate_impl(indices, program);
        }
        
        template<typename T>
        typename std::enable_if<IsProgramConstructible<T>::value, void>::type
        validate(T program) {
            std::vector<int> indices;
            for(int i : __get_all_inputs())
                if (__input_file_exists(i)) indices.emplace_back(i);
            __validate_impl(indices, program);
        }
    }
    
    namespace all{
        using namespace generator::io;
    }
}
#ifdef ON_WINDOWS
#undef mkdir
#endif
#endif