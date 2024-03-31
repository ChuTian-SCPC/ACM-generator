#include"testlib.h"
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unordered_map>
#include <thread>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
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
            OutStream() : output_stream_ptr_(&std::cerr) {}
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
                }
                else{
                    file_.open(filename);
                    if (!file_.is_open()) {
                        std::cerr << "Error opening file: " << filename << std::endl;
                        output_stream_ptr_ = &std::cerr;
                        std::cerr << "Using std::cerr" << std::endl;
                    } else {
                        output_stream_ptr_ = &file_;
                    }
                }
            }

            void close() {
                if (file_.is_open()) {
                    file_.close();
                    output_stream_ptr_ = &std::cerr;
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

        private:
            std::ostream* output_stream_ptr_;
            std::ofstream file_;
        };

        // error msg outstream, default on console
        // may unsafe
    #ifdef _WIN32
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
#else
        char _path_split = '/';
#endif
        std::string _path_splits = "/\\";
        template <typename U>
        struct IsPath;

        class Path {
        private:
            std::string _path;
        public:
            Path() : _path("") {}
            Path(std::string s) : _path(s) {}
            Path(const char *s) : _path(std::string(s)) {}

            std::string path() const { return _path; }
            const char* cname() const { return _path.c_str(); }

            void change(std::string s) { _path = s; }
            void change(const char *s) { _path = std::string(s); }
            void change(Path other_path) { _path = other_path.path(); }

            bool __file_exists() {
                std::ifstream file(_path.c_str());
                return file.good();
            }

            bool __directory_exists() {
            #ifdef _WIN32
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

            Path __folder_path() {
                if(this->__directory_exists()) {
                    return Path(_path);
                }
                else {
                    size_t pos = _path.find_last_of(_path_splits);
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
                    io::__fail_msg(io::_err, "%s is not a file path or the file doesn't exist.", _path.c_str());
                    return "";
                }
                size_t pos = _path.find_last_of(_path_splits);
                std::string file_full_name = pos == std::string::npos ? _path : _path.substr(pos + 1);
                size_t pos_s = file_full_name.find_first_of(".");
                std::string file_name = pos_s == std::string::npos ? file_full_name : file_full_name.substr(0, pos_s);
                return file_name;
            }

            void full() {
            #ifdef _WIN32
                char buffer[MAX_PATH];
                if (GetFullPathNameA(_path.c_str(), MAX_PATH, buffer, nullptr) == 0) {
                    io::__fail_msg(io::_err, "can't find full path :%s.", _path.c_str());
                }
            #else
                char buffer[1024];
                if (realpath(_path.c_str(), buffer) == nullptr) {
                    io::__fail_msg(io::_err, "can't find full path :%s.", _path.c_str());
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
            #ifdef _WIN32
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

        template <typename U>
        struct IsPath {
            template <typename V>
            static constexpr auto check(V *)
            -> decltype(std::declval<V>().path(), std::true_type());

            template <typename V>
            static constexpr std::false_type check(...);

            static constexpr bool value =
                    decltype(check<U>(nullptr))::value;
        };

        OutStream::OutStream(Path& path) {
            open(path.path());
        }

        template <typename T, typename... Args>
        Path __path_join(const T path, const Args &... args) {
            return Path(path).join(args...);
        }

        Path __lib_path() {
            return Path(__FILE__);
        }

        Path __current_path() {
        #ifdef _WIN32
            char buffer[MAX_PATH];
            GetModuleFileName(NULL, buffer, MAX_PATH);
        #else
            char buffer[1024];
            ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer));
            if (length != -1) {
                buffer[length] = '\0';
            }
        #endif
            std::string executable_path(buffer);

            size_t pos = executable_path.find_last_of(_path_splits);
            if (pos != std::string::npos) {
                std::string folder_path = executable_path.substr(0, pos);
                return Path(folder_path);
            }
            return Path("");
        }

        template <typename T>
        Path __full_path(T p) {
            Path path(p);
            path.full();
            return path;
        }
        
        bool __create_directory(Path& path) {
            if(path.__directory_exists())  return true;
            return mkdir(path.cname(),0777) == 0;
        }

        void __create_directories(Path& path) {
            std::istringstream ss(path.path());
            std::string token;
            Path current_path("");
            while (std::getline(ss, token, _path_split)) {
                current_path = __path_join(current_path, token);
            #ifdef _WIN32
                if(current_path.path().find_first_of(_path_splits) == std::string::npos && current_path.path().back() == ':') {
                    continue;
                }
            #else
                if(current_path.path().size() == 1 && current_path.path()[0] == _path_split) {
                    continue;
                }
            #endif
                if (!__create_directory(current_path)) {
                    io::__fail_msg(io::_err, "Error in creating folder : %s.",current_path.cname());
                }
            }
        }

        void __close_output_file_to_console(){
            #ifdef _WIN32
                freopen("CON", "w", stdout);
            #else
                freopen("/dev/tty", "w", stdout);
            #endif
        }
        
        void __close_input_file_to_console(){
            #ifdef _WIN32
                freopen("CON", "r", stdin);
            #else
                freopen("/dev/tty", "r", stdin);
            #endif
        }

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

        void __fake_arg(const char* format="",...){
            FMT_TO_RESULT(format, format, _format);
            _format = "gengrator " + _format;
            auto _fake_argvs = __split_string_to_char_array(_format.c_str());
            int _fake_argc = 0;
            while (_fake_argvs[_fake_argc] != nullptr) {
                ++_fake_argc;
            }
            prepareOpts(_fake_argc,_fake_argvs);
        }
        
        // equal to registerGen(argc, argv, 1);
        void init_gen(int argc,char* argv[]) {
            registerGen(argc, argv, 1);
        }

        // no argvs's register
        // unsafe but may easier to use
        void init_gen() {
            char * __fake_argvs[] = {(char*)"generator"};
            registerGen(1, __fake_argvs , 1);
        }
        
        enum End{
            In,
            Out,
            Ans,
            Log,
            Logc,
            Exe,
            MaxEnd  
        };
        std::string _file_end[MaxEnd] = {
            ".in",
            ".out",
            ".ans",
            ".log",
            ".logc",
            ".exe"
        };
        
        std::string __end_with(int x, End end) {
            return std::to_string(x) + _file_end[end];
        }
        
        std::string __end_with(const char* text, End end) {
            return std::string(text) + _file_end[end];
        }
        
        std::string __end_with(std::string text, End end) {
            return text + _file_end[end];
        }
        
        void __write_input_file(int x) {
            Path testcases_folder = __path_join(__current_path(), "testcases");
            __create_directories(testcases_folder);
            Path file_path = __path_join(testcases_folder, __end_with(x, In));
            freopen(file_path.cname(), "w", stdout);
            io::__success_msg(io::_err,"Successfully create input file %s",file_path.cname());
        }
        
        void __write_output_file(int x) {
            Path testcases_folder = __path_join(__current_path(), "testcases");
            __create_directories(testcases_folder);
            Path input_path = __path_join(testcases_folder, __end_with(x, In));
            if(!input_path.__file_exists()) {
                io::__fail_msg(io::_err,"Input file %s don't exist!",input_path.cname());
            }
            Path output_path = __path_join(testcases_folder, __end_with(x, Out));
            freopen(input_path.cname(), "r", stdin);
            freopen(output_path.cname(), "w", stdout);
            io::__success_msg(io::_err,"Successfully create output file %s", output_path.cname());
        }
        
        bool __input_file_exists(int x) {
            Path file_path = __path_join(__current_path(), "testcases", __end_with(x, In));
            return file_path.__file_exists();
        }
        
        std::vector<int> __get_inputs() {
            std::vector<int> inputs;
            Path folder_path = __path_join(__current_path(), "testcases");
        #ifdef _WIN32
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
        
        #define make_inputs(__start, __end, __Func, ...) do{ \
            for(int __case = (__start); __case <= (__end); __case++){ \
                __write_input_file(__case); \
                __fake_arg(__VA_ARGS__); \
                __Func; \
                __close_output_file_to_console(); \
            } \
        } while(0)
        
        #define make_outputs(__start,__end,__Func) do{ \
            for(int __case = (__start); __case <= (__end); __case++){ \
                __write_output_file(__case); \
                __Func; \
                __close_input_file_to_console(); \
                __close_output_file_to_console(); \
            } \
        } while(0)
        
        #define fill_inputs(__num,__Func,...) do{ \
            int __sum = (__num); \
            for(int __case = (1); __sum; __case++) { \
                if(!__input_file_exists(__case)) { \
                    __sum--; \
                    __write_input_file(__case); \
                    __fake_arg(__VA_ARGS__); \
                    __Func; \
                    __close_output_file_to_console(); \
                } \
            } \
        }while(0)
        
        #define fill_outputs(__Func) do{ \
            auto __inputs =  __get_inputs(); \
            for(auto __case : __inputs) { \
                __write_output_file(__case); \
                __Func; \
                __close_input_file_to_console(); \
                __close_output_file_to_console(); \
            } \
        }while(0)
        
        template<typename T>
        void make_inputs_exe(int start,int end,T path,const char* format = "",...){
            Path data_path(path);
            data_path.full();
            if (!data_path.__file_exists()) {
                io::__fail_msg(io::_err, "data file %s doesn't exist.", data_path.cname());
            }
            FMT_TO_RESULT(format,format,_format);
            Path testcases_folder = __path_join(__current_path(), "testcases");
            __create_directories(testcases_folder);
            for(int i = start;i <= end; i++){
                Path file_path = __path_join(testcases_folder, __end_with(i, In)) ;
                std::string command = data_path.path() + " " + _format + " > " + file_path.path();
                int return_code = std::system(command.c_str());
                if(return_code == 0) {
                    io::__success_msg(io::_err,"Successfully create input file %s",file_path.cname());
                }
                else {
                    io::__error_msg(io::_err,"Someting error in creating input file %s",file_path.cname());
                }   
            }
        }
        
        template<typename T>
        void make_outputs_exe(int start,int end, T path){
            Path std_path(path);
            std_path.full();
            if (!std_path.__file_exists()) {
                io::__fail_msg(io::_err, "std file %s doesn't exist.", std_path.cname());
            }
            Path testcases_folder = __path_join(__current_path(), "testcases");
            __create_directories(testcases_folder);
            for(int i = start;i <= end; i++){
                Path read_path = __path_join(testcases_folder, __end_with(i, In));
                if(!read_path.__file_exists()) {
                    io::__fail_msg(io::_err,"Input file %s don't exist!",read_path.cname());
                }
                Path write_path = __path_join(testcases_folder, __end_with(i, Out));
                std::string command = std_path.path() + " < " + read_path.path() + " > " + write_path.path();
                int return_code = std::system(command.c_str());
                if(return_code == 0) {
                    io::__success_msg(io::_err,"Successfully create output file %s",write_path.cname());
                }
                else {
                    io::__error_msg(io::_err,"Someting error in creating output file %s",write_path.cname());
                }
            }
        }
        
        template<typename T>
        void fill_inputs_exe(int sum, T path, const char* format = "",...){
            Path data_path(path);
            data_path.full();
            if (!data_path.__file_exists()) {
                io::__fail_msg(io::_err, "data file %s doesn't exist.", data_path.cname());
            }
            FMT_TO_RESULT(format,format,_format);
            Path testcases_folder = __path_join(__current_path(), "testcases");
            __create_directories(testcases_folder);
            for(int i = 1;sum; i++){
                Path file_path = __path_join(testcases_folder, __end_with(i, In)) ;
                if(file_path.__file_exists()){
                    continue;
                }
                sum--;
                std::string command = data_path.path() + " " + _format + " > " + file_path.path();
                int return_code = std::system(command.c_str());
                if(return_code == 0) {
                    io::__success_msg(io::_err,"Successfully create/open input file %s",file_path.cname());
                }
                else {
                    io::__error_msg(io::_err,"Someting error in creating/opening input file %s",file_path.cname());
                }   
            }
        }
        
        template<typename T>
        void fill_outputs_exe(T path){
            Path std_path(path);
            std_path.full();
            if (!std_path.__file_exists()) {
                io::__fail_msg(io::_err, "std file %s doesn't exist.", std_path.cname());
            }
            Path testcases_folder = __path_join(__current_path(), "testcases");
            __create_directories(testcases_folder);
            std::vector<int> inputs =  __get_inputs();
            for(auto i:inputs){
                Path read_path = __path_join(testcases_folder, __end_with(i, In));
                Path write_path = __path_join(testcases_folder, __end_with(i, Out));
                std::string command = std_path.path()+ " < " + read_path.path() + " > " + write_path.path();
                int return_code = std::system(command.c_str());
                if(return_code == 0) {
                    io::__success_msg(io::_err,"Successfully create output file %s",write_path.cname());
                }
                else {
                    io::__error_msg(io::_err,"Someting error in creating output file %s",write_path.cname());
                }
            }
        }
        
        enum ResultState{
            R_UNKNOWN,
            R_AC,
            R_WA,
            R_TLE,
            R_TLEANDAC,
            R_TLEANDWA,
            R_ERROR,
            R_Max
        };
        
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
        #ifdef _WIN32
            Path checker_path(__path_join(folder_path, "windows", __end_with(checker_name[checker], Exe)));
        #else
            Path checker_path(__path_join(folder_path, "linux", checker_name[checker]));
        #endif
            return checker_path;
        }
        
        std::vector<Path> __get_compare_files() {
            return std::vector<Path>();
        }

        template<typename T, typename... Args>
        std::vector<Path> __get_compare_files(T first, Args... args) {
            std::vector<Path> result;
            Path first_path(first);
            first_path.full();
            if(!first_path.__file_exists()) {
                __warn_msg(_err, "Compare program file %s doesn't exits.",first_path.cname());
            }
            else {
                result.emplace_back(first_path);
            }
            std::vector<Path> rest = __get_compare_files(args...);
            result.insert(result.end(), rest.begin(), rest.end());
            return result;
        }
        
        void __terminate_process(void* process) {
        #ifdef _WIN32
            TerminateProcess(reinterpret_cast<HANDLE>(process), 0);
            CloseHandle(reinterpret_cast<HANDLE>(process));
        #else
            pid_t pid = static_cast<pid_t>(reinterpret_cast<long long>(process));
            kill(pid, SIGTERM);
        #endif
        }
        
        int __run_program(
            Path& program,
            Path& input_file,
            Path& output_file,
            int time_limit) 
        {
            auto start_time = std::chrono::steady_clock::now();
        #ifdef _WIN32
            SECURITY_ATTRIBUTES sa;
            sa.nLength = sizeof(sa);
            sa.lpSecurityDescriptor = NULL;
            sa.bInheritHandle = TRUE;       
            
            HANDLE hInFile = CreateFileA(input_file.cname(),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                &sa,
                OPEN_EXISTING ,
                FILE_ATTRIBUTE_NORMAL,
                NULL );
            
            HANDLE hOutFile = CreateFileA(output_file.cname(),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_WRITE | FILE_SHARE_READ,
                &sa,
                CREATE_ALWAYS ,
                FILE_ATTRIBUTE_NORMAL,
                NULL );

            PROCESS_INFORMATION pi; 
            STARTUPINFOA si;
            BOOL ret = FALSE; 
            DWORD flags = CREATE_NO_WINDOW;

            ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
            ZeroMemory(&si, sizeof(STARTUPINFOA));
            si.cb = sizeof(STARTUPINFOA); 
            si.dwFlags |= STARTF_USESTDHANDLES;
            si.hStdInput = hInFile;
            si.hStdError = NULL;
            si.hStdOutput = hOutFile;
            char *c_program = const_cast<char *>(program.cname());
            ret = CreateProcessA(
                    NULL,
                    c_program,
                    NULL,
                    NULL,
                    TRUE,
                    flags,
                    NULL,
                    NULL,
                    &si,
                    &pi);
            if (ret) 
            {
                if (WaitForSingleObject(pi.hProcess, time_limit) == WAIT_TIMEOUT) {
                    __terminate_process(pi.hProcess);
                };
                auto end_time = std::chrono::steady_clock::now();
                return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            }
            else {
                return -1; 
            }   
        #else
            pid_t pid = fork();

            if (pid == 0) {
                int input = open(input_file.cname(), O_RDONLY);
                if (input == -1) {
                    __warn_msg(_err, "Fail to open input file %s.", input_file.cname());
                    exit(EXIT_FAILURE);
                }

                int output = open(output_file.cname(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                if (output == -1) {
                    close(input);
                    __warn_msg(_err, "Fail to open output file %s.", output_file.cname());
                    exit(EXIT_FAILURE);
                }

                dup2(input, STDIN_FILENO);
                dup2(output, STDOUT_FILENO);

                close(input);
                close(output);

                execl(program.cname(), program.cname(), nullptr);
                
                __warn_msg(_err, "Fail to run program %s", program.cname());
                exit(EXIT_FAILURE);
            } 
            else if (pid > 0) {
                auto limit = std::chrono::milliseconds(time_limit);

                int status;
                auto result = waitpid(pid, &status, WNOHANG);

                while (result == 0 && std::chrono::steady_clock::now() - start_time < limit) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    result = waitpid(pid, &status, WNOHANG);
                }

                if (result == 0) {
                    __terminate_process(reinterpret_cast<void*>(pid));
                } 

                auto end_time = std::chrono::steady_clock::now();
                return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            } 
            else {
                return -1;
                __warn_msg(_err, "Fail to fork.");
            }
        #endif
        }
        
        bool __enable_judge_ans(int runtime, int time_limit, ResultState& result) {
            if (runtime == -1) {
                result = R_ERROR;
                return false;
            }
            if (runtime > time_limit) {
                result = R_TLE;
                return runtime <= (int)(time_limit * 1.5);
            }
            else {
                return true;
            }
        }
        
        void __check_result(
            Path& input_file, 
            Path& output_file,
            Path& ans_file,
            Path& testlib_out_file,
            Path& checker,
            ResultState& result,
            std::string& testlib_result)
        {
            std::string command = 
                checker.path() + " " +
                input_file.path() + " " +
                output_file.path() + " " +
                ans_file.path() + " 2> " +
                testlib_out_file.path();
            system(command.c_str());
            std::ifstream check_stream(testlib_out_file.path());
            std::string line;
            while(check_stream >> line){
                testlib_result += line;
                testlib_result += " ";
            }
            check_stream.close();
            if(testlib_result.substr(0,2) == "ok") {
                result = result == R_TLE ? R_TLEANDAC : R_AC;
            }
            else {
                result = result == R_TLE ? R_TLEANDWA : R_WA;
            }
            return;
        } 

        void __check_once(
            int id,
            Path& program,
            int time_limit,
            Path& checker,
            Path& ans_file,
            Path& testlib_out_file,
            int& runtime,
            ResultState& result,
            std::string& testlib_result) 
        {
            Path input_file(__path_join(__current_path(), "testcases", __end_with(id, In)));
            Path output_file(__path_join(__current_path(), "testcases", __end_with(id, Out)));
            runtime = __run_program(program, input_file, ans_file, 2 * time_limit);
            if(__enable_judge_ans(runtime, time_limit, result)) {
                __check_result(
                    input_file, output_file, ans_file, testlib_out_file,
                    checker, result, testlib_result);
            }   
        }

        void __report_results(
            int start,
            int end,
            std::vector<int>& runtimes,
            std::vector<ResultState>& results,
            std::vector<std::string>& testlib_results,
            std::vector<int>& results_count,
            Path& log_path) 
        {
            OutStream log(log_path);       
            for (int i = start; i <= end; i++) {
                int idx = i - start;
                if (results[idx] == R_UNKNOWN || results[idx] == R_ERROR) {
                    __run_err_msg(_err, true, i);
                    __run_err_msg(log, false, i);
                }
                if (results[idx] == R_AC) {
                    __ac_msg(_err, true, i, runtimes[idx]);
                    __ac_msg(log, false, i, runtimes[idx]);
                }
                if (results[idx] == R_WA) {
                    __wa_msg(_err, true, i, runtimes[idx], testlib_results[idx]);
                    __wa_msg(log, false, i, runtimes[idx], testlib_results[idx]);
                }
                if (results[idx] == R_TLEANDAC) {
                    __tle_ac_msg(_err, true, i, runtimes[idx]);
                    __tle_ac_msg(log, false, i, runtimes[idx]);
                }
                if (results[idx] == R_TLEANDWA) {
                    __tle_wa_msg(_err, true, i, runtimes[idx], testlib_results[idx]);
                    __tle_wa_msg(log, false, i, runtimes[idx], testlib_results[idx]);
                }
                if (results[idx] == R_TLE) {
                    __tle_msg(_err, true, i, runtimes[idx]);
                    __tle_msg(log, false, i, runtimes[idx]);
                }
            }
            int case_count = end - start + 1;

            __info_msg(_err, "Total results :");
            __info_msg(_err, "%s : %d / %d", __color_ac(true).c_str(), results_count[R_AC], case_count);
            __info_msg(_err, "%s : %d / %d", __color_wa(true).c_str(), results_count[R_WA], case_count);
            __info_msg(_err, "%s : %d / %d", __color_tle(true).c_str(), results_count[R_TLE], case_count);
            __info_msg(_err, "%s : %d / %d", __color_tle_ac(true).c_str(), results_count[R_TLEANDAC], case_count);
            __info_msg(_err, "%s : %d / %d", __color_tle_wa(true).c_str(), results_count[R_TLEANDWA], case_count);
            __info_msg(_err, "%s : %d / %d", __color_run_err(true).c_str(), results_count[R_UNKNOWN] + results_count[R_ERROR], case_count);
            __info_msg(_err, "The report is in %s file.", log_path.cname());
            _err.println("");

            __info_msg(log, "Total results :");
            __info_msg(log, "%s : %d / %d", __color_ac(false).c_str(), results_count[R_AC], case_count);
            __info_msg(log, "%s : %d / %d", __color_wa(false).c_str(), results_count[R_WA], case_count);
            __info_msg(log, "%s : %d / %d", __color_tle(false).c_str(), results_count[R_TLE], case_count);
            __info_msg(log, "%s : %d / %d", __color_tle_ac(false).c_str(), results_count[R_TLEANDAC], case_count);
            __info_msg(log, "%s : %d / %d", __color_tle_wa(false).c_str(), results_count[R_TLEANDWA], case_count);
            __info_msg(log, "%s : %d / %d", __color_run_err(false).c_str(), results_count[R_UNKNOWN] + results_count[R_ERROR], case_count);

            log.close();
        }

        void __compare_once(int start, int end, Path& program, int time_limit, Path& checker) {
            Path compare_path(__path_join(__current_path(), "cmp"));
            std::string program_name = program.__file_name();
            Path ans_folder_path(__path_join(compare_path, program_name));
            __create_directories(ans_folder_path);
            Path testlib_out_file(__path_join(ans_folder_path, __end_with("__checker", Logc)));
            int case_count = end - start + 1;
            std::vector<int> runtimes(case_count, -1);
            std::vector<ResultState> results(case_count, R_UNKNOWN);
            std::vector<std::string> testlib_results(case_count);
            std::vector<int> results_count(R_Max, 0);
            for (int i = start; i <= end; i++) {
                Path ans_file(__path_join(ans_folder_path, __end_with(i, Ans)));
                int idx = i - start;
                __check_once(
                    i, program, time_limit, checker, ans_file, testlib_out_file,
                    runtimes[idx], results[idx], testlib_results[idx]);
                results_count[results[idx]]++;
            }
            testlib_out_file.__delete_file();
            Path log_path(__path_join(compare_path, __end_with(program_name, Log)));
            __info_msg(_err,"Test results for program %s :",program.cname());
            __report_results(start, end, runtimes, results, testlib_results, results_count, log_path);
            return;
        }

        template<typename... Args>
        void compare(int start, int end, int time_limit, Path checker_path, Args ...args) {
            checker_path.full();
            if (!checker_path.__file_exists()) {
                __fail_msg(_err, "Checker file %s doesn't exist.", checker_path.cname());
            }
            std::vector<Path> programs = __get_compare_files(args...);
            for(Path& program : programs) {
                __compare_once(start, end, program, time_limit, checker_path);
            } 
            return;
        }

        template<typename... Args>
        void compare(int start, int end, int time_limit, std::string checker_path, Args ...args) {
            compare(start, end, time_limit, Path(checker_path), args...);
        }

        template<typename... Args>
        void compare(int start, int end, int time_limit, const char* checker_path, Args ...args) {
            compare(start, end, time_limit, Path(checker_path), args...);
        }

        template<typename... Args>
        void compare(int start, int end, int time_limit, Checker checker, Args ...args) {
            Path checker_path = __get_default_checker_file(checker);
            compare(start, end, time_limit, checker_path, args...);
        }
    }

    namespace rand{

        std::pair<long long,long long> __format_to_int_range(std::string s){
            size_t open = s.find_first_of("[(");
            size_t close = s.find_first_of(")]");
            size_t comma = s.find(',');
            auto string_to_int = [&](size_t from, size_t to) -> long long{
                std::string str = s.substr(from + 1, to - from - 1);
                try {
                    long long value = std::stoll(str);
                    return value;
                }
                catch (const std::invalid_argument &e) {
                    io::__fail_msg(io::_err,"%s is an invalid argument.", str.c_str());
                }
                catch (const std::out_of_range &e) {
                    io::__fail_msg(io::_err,"%s is out of range.", str.c_str());
                }
                return 0LL;
            };
            if(open == std::string::npos || close == std::string::npos || comma == std::string::npos){
                io::__fail_msg(io::_err,"%s is an invalid range.",s.c_str());
            }
            long long left = string_to_int(open, comma);
            long long right = string_to_int(comma, close);
            if(s[open] == '('){
                left ++;
            }
            if(s[close] == ')'){
                right --;
            }
            return std::make_pair(left,right);
        }

        // equal to rnd.next(n),a int in [0,n-1]
        template <typename T = int>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_int(T n){
            T x = rnd.next(n);
            return x;
        }

        // equal to rnd.next(from,to),a int in [from,to]
        template <typename T = int>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_int(T from, T to){
            T x = rnd.next(from, to);
            return x;
        }

        // equal to rnd.next(from,to),a int in [from,to]
        template <typename T = long long, typename U = long long>
        typename std::enable_if<std::is_integral<T>::value && std::is_integral<U>::value, long long>::type
        rand_int(T from, U to){
            long long x = rnd.next((long long)from, (long long)to);
            return x;
        }

        // rand a integer number satisfied the given range
        long long rand_int(const char* format,...) {
            FMT_TO_RESULT(format, format, _format);
            std::pair<long long,long long> range = __format_to_int_range(_format);
            long long x = rnd.next(range.first,range.second);
            return x;
        }

        // rand a odd number between [1,n]
        template <typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, long long>::type
        rand_odd(T n){
            long long nl = (long long)n;
            long long r = (nl - (nl % 2 == 0) - 1LL)/2;
            if(r < 0) {
                io::__fail_msg(io::_err,"There is no odd number between [1,%lld].",nl);
            }
            long long x = rnd.next(0LL,r);
            x = x * 2 + 1;
            return x;
        }

        // rand a odd number between [from,to]
        template <typename T = long long, typename U = long long>
        typename std::enable_if<std::is_integral<T>::value && std::is_integral<U>::value, long long>::type
        rand_odd(T from, U to){
            long long froml = (long long)from;
            long long tol = (long long)to;
            long long l = (froml + (froml % 2 == 0) - 1LL)/2;
            long long r = (tol - (tol % 2 == 0) - 1LL)/2;
            if(l > r) {
                io::__fail_msg(io::_err,"There is no odd number between [%lld,%lld].",froml,tol);
            }
            long long x = rnd.next(l, r);
            x = x * 2 + 1;       
            return x;
        }

        // rand a odd number satisfied the given range
        long long rand_odd(const char* format,...) {
            FMT_TO_RESULT(format, format, _format);
            std::pair<long long,long long> range = __format_to_int_range(_format);
            long long x = rand_odd(range.first,range.second);
            return x;
        }

        // rand a even number between [0,n]
        template <typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, long long>::type
        rand_even(T n){
            long long nl = (long long)n;
            long long r = (nl - (nl % 2 == 1))/2;
            if(r < 0) {
                io::__fail_msg(io::_err,"There is no even number between [0,%lld].",nl);
            }
            long long x = rnd.next(0LL,r);
            x = x * 2;
            return x;
        }

        // rand a even number between [from,to]
        template <typename T = long long, typename U = long long>
        typename std::enable_if<std::is_integral<T>::value && std::is_integral<U>::value, long long>::type
        rand_even(T from, U to){
            long long froml = (long long)from;
            long long tol = (long long)to;
            long long l = (froml + (froml % 2 == 1))/2;
            long long r = (tol - (tol % 2 == 1))/2;
            if(l > r) {
                io::__fail_msg(io::_err,"There is no even number between [%lld,%lld].",froml,tol);
            }
            long long x = rnd.next(l, r);
            x = x * 2;       
            return x;
        }

        // rand a even number satisfied the given range
        long long rand_even(const char* format,...) {
            FMT_TO_RESULT(format, format, _format);
            std::pair<long long,long long> range = __format_to_int_range(_format);
            long long x = rand_even(range.first,range.second);
            return x;
        }

        // enable T is double or can be change to double
        template <typename T>
        constexpr bool is_double_valid() {
            return std::is_floating_point<T>::value || std::is_convertible<T, double>::value;
        }

        // change input to double
        template <typename T>
        double __change_to_double(T n){
            double _n;
            if(std::is_floating_point<T>::value){
                _n = n;
            }
            else if(std::is_convertible<T, double>::value){
                _n = static_cast<double>(n);
                io::__warn_msg(io::_err,"Input is not a real number, change it to %lf.Please ensure it's correct.",_n);
            }
            else{
                io::__fail_msg(io::_err,"Input is not a real number, and can't be changed to it.");
            }
            return _n;
        }

        // equal to rnd.next(),a real number in [0,1)
        double rand_real() {
            double x = rnd.next();
            return x;
        }

        // equal to rnd.next(n),a real number in [0,n)
        template <typename T = double>
        double rand_real(T n) {
            double _n = __change_to_double(n);
            double x = rnd.next(_n);
            return x;
        }

        // equal to rnd.next(form,to),a real number in [from,to)
        template <typename T = double>
        typename std::enable_if<is_double_valid<T>(), double>::type
        rand_real(T from,T to) {
            double _from = __change_to_double(from);
            double _to = __change_to_double(to);
            double x = rnd.next(_from, _to);
            return x;
        }

        // equal to rnd.next(form,to),a real number in [from,to)
        template <typename T = double,typename U = double>
        typename std::enable_if<is_double_valid<T>() && is_double_valid<U>(), double>::type
        rand_real(T from,U to) {
            double _from = __change_to_double(from);
            double _to = __change_to_double(to);
            double x = rnd.next(_from, _to);
            return x;
        }



        std::pair<double,double> __format_to_double_range(std::string s){
            int accuarcy = 1;
            size_t open = s.find_first_of("[(");
            size_t close = s.find_first_of(")]");
            size_t comma = s.find(',');
            auto string_to_double = [&](size_t from, size_t to) -> double{
                std::string str = s.substr(from + 1, to - from - 1);
                try {
                    double value = std::stod(str);
                    if(std::isnan(value)){
                        io::__fail_msg(io::_err,"Exist a nan number.");
                    }
                    if(std::isinf(value)){
                        io::__fail_msg(io::_err,"Exist an inf number.");
                    }
                    int digit = 1;
                    bool is_decimal_part = false;
                    bool is_scientific_part = false;
                    std::string scientific_part = "";
                    for(auto c:str){
                        if(is_decimal_part == true){
                            if(c >= '0' && c <= '9'){
                                digit ++;
                            }
                            else{
                                is_decimal_part = false;
                            }
                        }
                        if(is_scientific_part == true){
                            scientific_part += c;
                        }
                        if(c == '.'){
                            is_decimal_part = true;
                        }
                        if(c == 'e' || c == 'E'){
                            is_scientific_part = true;
                        }
                    }
                    if(scientific_part != ""){
                        int scientific_length = std::stoi(scientific_part);
                        digit -= scientific_length;
                    }
                    accuarcy = std::max(accuarcy, digit);
                    return value;
                }
                catch (const std::invalid_argument &e) {
                    io::__fail_msg(io::_err,"%s is an invalid argument.", str.c_str());
                }
                catch (const std::out_of_range &e) {
                    io::__fail_msg(io::_err,"%s is out of range.", str.c_str());
                }
                return 0.0;
            };
            if(open == std::string::npos || close == std::string::npos || comma == std::string::npos){
                io::__fail_msg(io::_err,"%s is an invalid range.",s.c_str());
            }
            double left = string_to_double(open, comma);
            double right = string_to_double(comma, close);
            double eps = 1.0;
            for(int i = 1;i <= accuarcy;i++){
                eps /= 10.0;
            }
            if(s[open] == '('){
                left += eps;
            }
            if(s[close] == ']'){
                right += eps;
            }
            return std::make_pair(left,right);
        }
        // return a real number satisfied the given range
        // can use format like [from,to],(from,to),[from,to),(from,to]
        // the accuracy is equal to the max digits of from/to
        double rand_real(const char* format,...) {
            FMT_TO_RESULT(format, format, _format);
            std::pair<double,double> range = __format_to_double_range(_format);
            double x = rnd.next(range.first,range.second);
            return x;
        }

        // return a real number in range (-1.0,1.0)
        double rand_abs(){
            double x = rnd.next();
            int op = rnd.next(0,1);
            return op?x:-x;
        }

        // return a real number in range (-n,n)
        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, T>::type
        rand_abs(T from) {
            double x = rand_real(from);
            int op = rnd.next(0,1);
            return op?x:-x;
        }

        // return a integer number in range [-n,n]
        template <typename T>
        typename std::enable_if<std::is_integral<T>::value, T>::type
        rand_abs(T from) {
            T x = rand_int(from);
            int op = rnd.next(0,1);
            return op?x:-x;
        }

        // return a integer number in range [-to,-from]U[from,to]
        template <typename T, typename U>
        typename std::enable_if<std::is_integral<T>() && std::is_integral<U>(), long long>::type
        rand_abs(T from, U to) {
            long long x = rand_int(from, to);
            int op = rnd.next(0,1);
            return op?x:-x;
        }

        // return a real number in range (-to,-from]U[from,to)
        template <typename T, typename U>
        typename std::enable_if<
            is_double_valid<T>() 
            && is_double_valid<U>()
            && !(std::is_integral<T>() 
                && std::is_integral<U>())
        , double>::type
        rand_abs(T from, U to) {
            double x = rand_real(from,to);
            int op = rnd.next(0,1);
            return op?x:-x;
        }

        // return a real number satisfied the given range and it's opposite
        double rand_abs(const char* format,...) {
            FMT_TO_RESULT(format, format, _format);
            std::pair<double,double> range = __format_to_double_range(_format);
            double x = rnd.next(range.first,range.second);
            int op = rnd.next(0,1);
            return op?x:-x;
        }

        enum CharType{
            LowerLetter,
            UpperLetter,
            Letter,
            Number,
            LetterNumber,
            ZeroOne,
            MAX
        };
        const std::string _PATTERN[MAX]={
            "[a-z]",
            "[A-Z]",
            "[a-zA-Z]",
            "[0-9]",
            "[a-zA-Z0-9]",
            "[01]"
        };
        // return a char use CharType
        // default is LowerLetter
        char rand_char(CharType type = LowerLetter){
            std::string s = rnd.next(_PATTERN[type]);
            return s.c_str()[0];
        }

        // return a char use testlib format
        char rand_char(const char* format,...) {
            FMT_TO_RESULT(format, format, _format);
            std::string s = rnd.next(_format);
            if(s.empty()) {
                io::__fail_msg(io::_err, "Can't generator a char from an empty string.");
            }
            return s.c_str()[0];
        }

        // return a string length is n, use CharType
        // CharType default is LowerLetter
        std::string rand_string(int n,CharType type = LowerLetter){
            std::string s = rnd.next("%s{%d}",_PATTERN[type].c_str(),n);
            return s;
        }

        // return a string length is in range [from,to],use CharType
        // CharType default is LowerLetter
        std::string rand_string(int from,int to,CharType type = LowerLetter){
            std::string s = rnd.next("%s{%d,%d}",_PATTERN[type].c_str(),from,to);
            return s;
        }

        // return a string length is n, use testlib format
        std::string rand_string(int n,const char* format,...){
            FMT_TO_RESULT(format, format, _format);
            std::string s = rnd.next("%s{%d}",_format.c_str(),n);
            return s;
        }

        // return a string length is in range [from,to],use testlib format
        std::string rand_string(int from,int to,const char* format,...){
            FMT_TO_RESULT(format, format, _format);
            std::string s = rnd.next("%s{%d,%d}",_format.c_str(),from,to);
            return s;
        }

        // equal to rnd.perm(n)
        // return a permutation length is n,begin from 0
        template <typename T>
        std::vector<T> rand_p(T n){
            std::vector<T> v = rnd.perm(n,T(0));
            return v;
        }

        // equal to rnd.perm(n,s)
        // return a permutation length is n,begin from s
        template <typename T,typename E>
        std::vector<E> rand_p(T n,E s){
            std::vector<E> v = rnd.perm(n,s);
            return v;
        }

        // equal to rnd.partition(size,sum)
        // return a positive integer vector,which sum equals to sum
        template<typename T>
        std::vector<T> rand_sum(int size,T sum) {
            auto v = rnd.partition(size,sum);
            return v;
        }
        // equal to rnd.partition(size,sum,min_part)
        // return a integer vector,which elements not less than min_part and sum equals to sum
        template<typename T>
        std::vector<T> rand_sum(int size,T sum,T min_part) {
            auto v = rnd.partition(size,sum,min_part);
            return v;
        }
        
        template<typename T>
        void __rand_small_sum(std::vector<T>&v, T sum, T limit) {
            int size = v.size();
            std::vector<int> rand_v(size);
            for(int i = 0; i < size; i++)
                rand_v[i] = i;
            shuffle(rand_v.begin(), rand_v.end());
            int last = size - 1;
            while(sum--) {
                int rand_pos = rnd.next(0, last);
                int add_pos = rand_v[rand_pos];
                v[add_pos]++;
                if(v[add_pos] >= limit) {
                    std::swap(rand_v[last], rand_v[rand_pos]);
                    last--;
                }
            }
            return;
        }
        
        template<typename T>
        bool __rand_large_sum(std::vector<T>&v, T& sum, T limit) {
            int size = v.size();
            std::vector<int> rand_v(size);
            for(int i = 0; i < size; i++)
                rand_v[i] = i;
            shuffle(rand_v.begin(), rand_v.end());
            int last = size - 1;
            T once_add_base = sum / 1000000;
            while(sum > 1000000) {
                int rand_pos, add_pos, once_add;
                do {
                    rand_pos = rnd.next(0, last);
                    add_pos = rand_v[rand_pos];
                    // 95% ~ 105% for once add
                    T once_add_l = std::max(T(0), once_add_base - once_add_base / 20);
                    T once_add_r = std::min(once_add_base + once_add_base / 20, sum);
                    if (once_add_l > once_add_r) {
                        return sum > 1000000;
                    }
                    once_add = rnd.next(once_add_l, once_add_r);
                }while(v[add_pos] + once_add > limit);         
                v[add_pos] += once_add;
                sum -= once_add;
                if(v[add_pos] >= limit) {
                    std::swap(rand_v[last], rand_v[rand_pos]);
                    last--;
                }
            }
            return false;
        }
        
        // return a integer vector,which elements in range [from,to] and sum equals to sum
        template<typename T>
        std::vector<T> rand_sum(int size,T sum,T from,T to) {
            if(size < 0){
                io::__fail_msg(io::_err,"Size of the vector can't less than zero.");
            }
            if(size > 10000000){
                io::__warn_msg(
                    io::_err,
                    "Size of the vector is greater than %s,since it's too large!",
                    std::to_string(size).c_str()
                );
            }
            if(from > to){
                io::__fail_msg(
                    io::_err,
                    "Vaild range [%s,%s],to can't less than from.",
                    std::to_string(from).c_str(),
                    std::to_string(to).c_str()
                );
            }
            if(size * from > sum || size * to < sum){
                io::__fail_msg(
                    io::_err,
                    "Sum of the vector is in range [%s,%s],but need sum = %s.",
                    std::to_string(from * size).c_str(),
                    std::to_string(to * size).c_str(),
                    std::to_string(sum).c_str());
            }
            if(size == 0) {
                if(sum != 0){
                    io::__fail_msg(io::_err,"Sum of the empty vector must be 0.");
                }
                return std::vector<T>();
            }

            T ask_sum = sum;

            std::vector<T> v(size,0);
            sum -= from * size;
            T limit = to - from;
            if (sum <= 1000000) {
                __rand_small_sum(v, sum, limit);
            }
            else if (limit * size - sum <=1000000){
                __rand_small_sum(v, limit * size - sum, limit);
                for (int i = 0; i < size; i++) {
                    v[i] = limit - v[i];
                }
            }
            else {
                while(__rand_large_sum(v, sum, limit));
                __rand_small_sum(v, sum, limit);
            }
            for (int i = 0; i < size; i++) {
                v[i] += from;
            }

            T result_sum = 0;
            for(int i = 0;i < size;i++){
                if(v[i] < from || v[i] > to){
                    io::__error_msg(
                        io::_err,
                        "The %d%s number %s is out of range [%s,%s].Please notice author to fix bug.",
                        i+1,
                        englishEnding(i+1).c_str(),
                        std::to_string(v[i]).c_str(),
                        std::to_string(from).c_str(),
                        std::to_string(to).c_str()
                    );
                }
                result_sum += v[i];
            }
            if (result_sum != ask_sum){
                io::__error_msg(
                    io::_err,
                    "Sum of the vector is equal to %s,not %s.Please notice author to fix bug.",
                    std::to_string(result_sum).c_str(),
                    std::to_string(ask_sum).c_str()
                );
            }
            return v;
        }

        // return a rand vector length equal to n
        template <typename T>
        std::vector<T> rand_vector(int size, std::function<T()> func) {
            std::vector<T> v;
            for(int i = 0;i < size; i++){
                T x = func();
                v.emplace_back(x);
            }
            return v;
        }

        // return a rand vector length in range [from,to]
        template <typename T>
        std::vector<T> rand_vector(int from,int to, std::function<T()> func) {
            std::vector<T> v;
            int size = rnd.next(from,to);
            for(int i = 0;i < size; i++){
                T x = func();
                v.emplace_back(x);
            }
            return v;
        }

        // return a element by given probability.
        // Con is map or unordered_map
        // <Key,Value> return type is Key,Value type must be integer
        // means element Key has Value/sum(Value) be choosen
        template <typename Con>
        typename std::enable_if<
                (std::is_same<Con, std::map<typename Con::key_type, typename Con::mapped_type>>::value ||
                std::is_same<Con, std::unordered_map<typename Con::key_type, typename Con::mapped_type>>::value) &&
                std::is_integral<typename Con::mapped_type>::value,
                typename Con::key_type
        >::type
        rand_prob(const Con& map)
        {
            using KeyType = typename Con::key_type;
            using ValueType = typename Con::mapped_type;
            std::vector<KeyType> elements;
            std::vector<ValueType> probs;
            long long sum = 0;
            for(auto iter:map){
                elements.emplace_back(iter.first);
                ValueType value = iter.second;
                if (value < 0) {
                    io::__fail_msg(io::_err, "Value can't less than 0.");
                }
                sum += value;
                probs.emplace_back(sum);
            }
            if (sum <= 0) {
                io::__fail_msg(io::_err, "Sum of the values must greater than 0.");
            }
            long long p = rand_int(1LL,sum);
            auto pos = lower_bound(probs.begin(),probs.end(),p) - probs.begin();
            return *(elements.begin() + pos);
        }

        // use vector index + `offset` be new vector elements
        // the value of the vector represents the number of times this index will appear in the new vector
        // return the shuffled new vector
        template<typename Iter>
        std::vector<int> shuffle_index(Iter begin, Iter end, int offset = 0) {
            int tot = 0;
            std::vector<int> res;
            for (Iter i = begin; i != end; i++) {
                int x = *i;
                if (x < 0) {
                    io::__fail_msg(io::_err, "Elements must be non negtive number.");
                }
                tot += x;
                if (tot > 10000000) {
                    io::__fail_msg(io::_err, "Sum of the elements must equal or less than 10^7");
                }
                while (x--) {
                    res.emplace_back((i - begin) + offset);
                }
                shuffle(res.begin(),res.end());
            }
            return res;
        }

        std::vector<int> shuffle_index(std::vector<int> v, int offset = 0) {
            return shuffle_index(v.begin(), v.end(), offset);
        }
    }

    namespace graph {
        namespace basic {
            class BasicEdge {
            private:
                int _u, _v;

            public:
                BasicEdge(int u, int v) : _u(u), _v(v) {
                   
                }

                int cu() const { return _u; }
                int cv() const { return _v; }

                int& u() { return _u; }
                int& v() { return _v;}

                void set_u(int u) { _u = u; }
                void set_v(int v) { _v = v; } 

                friend bool operator<(const BasicEdge a, const BasicEdge b) {
                    if (a._u == b._u) {
                        return a._v < b._v;
                    }
                    return a._u < b._u;
                }

                friend bool operator<=(const BasicEdge a, const BasicEdge b) {
                    if (a._u == b._u) {
                        return a._v <= b._v;
                    }
                    return a._u <= b._u;
                }

                friend bool operator>(const BasicEdge a, const BasicEdge b) {
                    if (a._u == b._u) {
                        return a._v > b._v;
                    }
                    return a._u > b._u;
                }

                friend bool operator>=(const BasicEdge a, const BasicEdge b) {
                    if (a._u == b._u) {
                        return a._v >= b._v;
                    }
                    return a._u >= b._u;
                }     

                friend bool operator==(const BasicEdge a, const BasicEdge b) {
                    return a._u == b._u && a._v == b._v;
                }

                friend bool operator!=(const BasicEdge a, const BasicEdge b) {
                    return a._u != b._u|| a._v != b._v;
                }

            };

            template<typename T>
            class _Edge : public BasicEdge {
            private:
                T _w;
                typedef std::function<void(std::ostream&, const _Edge<T>&)> OutputFunction;
                OutputFunction _output_function;
            public:
                _Edge(int u, int v, T w) : BasicEdge(u, v), _w(w) {
                    _output_function = 
                        [this](std::ostream& os, const _Edge<T>& edge) { 
                            this->default_output(os); 
                        };
                }

                T cw() const { return _w; }
                T& w() { return _w; }

                void set_w(T w) { _w = w; }

               void default_output(std::ostream& os) const {
                    os << this->cu() << " " << this->cv() << " " << _w;
                }

                void set_output(OutputFunction func) {
                    _output_function = func;
                }

                void set_output_default() {
                    _output_function = 
                        [this](std::ostream& os, const _Edge<T>& edge) { 
                            this->default_output(os); 
                        };
                }

                friend std::ostream& operator<<(std::ostream& os, const _Edge<T>& edge) {
                    edge._output_function(os, edge);
                    return os;
                }
            };

            template<>
            class _Edge<void> : public BasicEdge {
            private:
                typedef std::function<void(std::ostream&, const _Edge<void>&)> OutputFunction;
                OutputFunction _output_function;
            public:
                _Edge(int u, int v) : BasicEdge(u, v) {
                    _output_function = 
                        [this](std::ostream& os, const _Edge<void>& edge) { 
                            this->default_output(os); 
                        };
                }

                void default_output(std::ostream& os) const {
                    os << this->cu() << " " << this->cv() ;
                }

                void set_output(OutputFunction func) {
                    _output_function = func;
                }

                void set_output_default() {
                    _output_function = 
                        [this](std::ostream& os, const _Edge<void>& edge) { 
                            this->default_output(os); 
                        };
                }

                friend std::ostream& operator<<(std::ostream& os, const _Edge<void>& edge) {
                    edge._output_function(os, edge);
                    return os;
                }
            };
        }

        namespace unweight {
            typedef basic::_Edge<void> Edge;

            //common tree
            class Tree {
            public:
                enum TreeGenerator {
                    RandomFather,
                    Pruefer
                };
            protected:      
                int _node; // the number of nodes in the tree  
                int _begin_node; // index of the first node                   
                bool _is_rooted;
                // use if `_is_rooted` is true,
                int _root;
                std::vector <Edge> _edge;
                std::vector<int> _p;
                
                // output format
                bool _output_node;
                bool _output_root;
                bool _swap_node;// true means output `father son` or `son father` by random
                
                TreeGenerator _tree_generator;

            public:
                Tree(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                     TreeGenerator tree_generator = RandomFather) :
                        _node(node),
                        _begin_node(begin_node),
                        _is_rooted(is_rooted),
                        _root(root - begin_node),
                        _output_node(true),
                        _output_root(true),
                        _swap_node(_is_rooted ? false : true),
                        _tree_generator(tree_generator)
                {}

                void set_node(int node) { _node = node; }

                void set_is_rooted(int is_rooted) { _is_rooted = is_rooted; }

                void set_root(int root) {
                    _root = root - _begin_node;
                    if (!_is_rooted) {
                       io::__warn_msg(io::_err, "Unrooted Tree, set root is useless."); 
                    }
                }

                void set_begin_node(int begin_node) { 
                    if (_is_rooted) {
                        _root += _begin_node;
                        _root -= begin_node;
                    }
                    _begin_node = begin_node; 
                }

                void set_output_node(bool output_node) { _output_node = output_node; }

                void set_output_root(bool output_root) { _output_root = output_root; }

                void set_swap_node(bool swap_node) { _swap_node = swap_node; }

                void set_tree_generator(TreeGenerator tree_generator) { _tree_generator = tree_generator; }

                void use_random_father() { _tree_generator = RandomFather; }

                void use_pruefer() { _tree_generator = Pruefer; }

                int root() {
                    if (!_is_rooted) {
                        io::__warn_msg(io::_err, "Unrooted Tree, root is useless.");
                    }
                    return _root + _begin_node;
                }

                std::vector <Edge> edge() { return _edge; }

                virtual void gen() {
                    __init();
                    if (_node == 1) {
                        return;
                    }
                    if (_tree_generator == Pruefer) {
                        std::vector<int> times = rand::rand_sum(_node, _node - 2, 0);
                        std::vector<int> pruefer = rand::shuffle_index(times);
                        __pruefer_decode(pruefer);
                    } else {
                        __gen_once();
                    }
                }


                friend std::ostream &operator<<(std::ostream &os, const Tree &tree) {
                    std::string first_line = "";
                    if (tree._output_node) {
                        first_line += std::to_string(tree._node);
                    }
                    if (tree._is_rooted && tree._output_root) {
                        if (first_line != "") {
                            first_line += " ";
                        }
                        first_line += std::to_string(tree._root + tree._begin_node);
                    }
                    if (first_line != "") {
                        os << first_line;
                        if (tree._node > 1) {
                            os << "\n";
                        }
                    }
                    int cnt = 0;
                    for (Edge e: tree._edge) {
                        os << e;
                        if (++cnt < tree._node - 1) {
                            os << "\n";
                        }
                    }
                    return os;
                }

                void println() {
                    std::cout<<*this<<std::endl;
                }
            protected:
                void __judge_comman_limit() {
                    if (_node <= 0) {
                        io::__fail_msg(io::_err, "Number of nodes must be a positive integer, but found %d.", _node);
                    }

                    if (_is_rooted && (_root < 0 || _root > _node)) {
                        io::__fail_msg(
                                io::_err,
                                "restriction of the root is [%d, %d], but found %d.",
                                _begin_node, 
                                _node + _begin_node - 1, 
                                _root + _begin_node);
                    }
                }

                virtual void __judge_self_limit() {}

                void __judge_limits() {
                    __judge_comman_limit();
                    __judge_self_limit();
                }

                void __init() {
                    __judge_limits();
                    _edge.clear();
                    _p = rnd.perm(_node, 0);
                    if (_is_rooted) {
                        for (int i = 1; i < _node; i++) {
                            if (_p[i] == _root) {
                                std::swap(_p[0], _p[i]);
                                break;
                            }
                        }
                    }
                }

                void __add_edge(int u, int v) {
                    u += _begin_node;
                    v += _begin_node;
                    if (!_swap_node || rnd.next(2)) {
                        _edge.emplace_back(u, v);
                    } else {
                        _edge.emplace_back(v, u);
                    }
                }

                virtual void __gen_tree() {
                    for (int i = 1; i < _node; i++) {
                        int f = rnd.next(i);
                        __add_edge(_p[f], _p[i]);
                    }
                }

                void __pruefer_decode(std::vector<int> pruefer) {
                    if (_node == 2) {
                        if (_is_rooted) {
                            __add_edge(_root, 1 ^ _root);
                        } else {
                            __add_edge(0, 1);
                        }
                        return;
                    }

                    if (_is_rooted) {
                        int n = pruefer.size();
                        bool exist = false;
                        for (int i = 0; i < n; i++) {
                            if (pruefer[i] == _root) {
                                std::swap(pruefer[i], pruefer[n - 1]);
                                exist = true;
                                break;
                            }
                        }
                        if (!exist) {
                            pruefer[n - 1] = _root;
                        }
                    }

                    std::vector<int> degree(_node, 1);
                    for (auto x: pruefer) {
                        degree[x]++;
                    }
                    int ptr = 0;
                    while (degree[ptr] != 1) {
                        ptr++;
                    }
                    int leaf = ptr;
                    for (auto u: pruefer) {
                        __add_edge(u, leaf);
                        degree[u]--;
                        if (degree[u] == 1 && u < ptr) {
                            leaf = u;
                        } else {
                            do {
                                ptr++;
                            } while (degree[ptr] != 1);
                            leaf = ptr;
                        }
                    }
                    int u = leaf;
                    int v = _node - 1;
                    if (_is_rooted && v == _root) {
                        std::swap(u, v);
                    }
                    __add_edge(u, v);
                    shuffle(_edge.begin(), _edge.end());
                }

                void __gen_once() {
                    __gen_tree();
                    shuffle(_edge.begin(), _edge.end());
                }
            };

            class Chain : public Tree {
            public:
                Chain(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1) :
                        Tree(node, begin_node, is_rooted, root) {}

                virtual void gen() {
                    __init();
                    if (_node == 1) {
                        return;
                    }
                    __gen_once();
                }

            protected:
                virtual void __gen_tree() {
                    for (int i = 1; i < _node; i++) {
                        __add_edge(_p[i - 1], _p[i]);
                    }
                }
            };

            class Flower : public Tree {
            public:
                Flower(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1) :
                        Tree(node, begin_node, is_rooted, root) {}

                virtual void gen() {
                    __init();
                    if (_node == 1) {
                        return;
                    }
                    __gen_once();
                }

            protected:
                virtual void __gen_tree() {
                    for (int i = 1; i < _node; i++) {
                        __add_edge(_p[0], _p[i]);
                    }
                }
            };

            class HeightTree : public Tree {
            protected:
                int _height;
            public:
                HeightTree(int node = 1, int begin_node = 1, int root = 1, int height = -1) :
                        Tree(node, begin_node, true, root),
                        _height(height) {}
                
                void set_is_rooted(bool is_rooted) = delete;
                void set_height(int height) { _height = height; }

                virtual void gen() {
                    if (_height == -1) {
                        _height = rnd.next(_node == 1 ? 1 : 2, _node);
                    }
                    __init();
                    if (_node == 1) {
                        return;
                    }
                    __gen_once();
                }

                void gen(int height) {
                    _height = height;
                    gen();
                }

                template<typename T = int, typename U = int>
                typename std::enable_if<std::is_integral<T>::value && std::is_integral<U>::value, void>::type
                gen(T l_limit, U r_limit) {
                    _height = (int) rand::rand_int(l_limit, r_limit);
                    gen();
                }

                void gen(const char *format, ...) {
                    FMT_TO_RESULT(format, format, _format);
                    _height = (int) rand::rand_int(_format.c_str());
                    gen();
                }

            protected:
                virtual void __judge_self_limit() {
                    if (_height > _node || (_node > 1 && _height <= 1) || _height < 1) {
                        io::__fail_msg(io::_err, "restriction of the height is [%d,%d].\n", _node == 1 ? 1 : 2,
                                        _node);
                    }
                }

                virtual void __gen_tree() {
                    std::vector<int> number(_height, 1);
                    int w = _node - _height;
                    for (int i = 1; i <= w; i++) {
                        number[rnd.next(1, _height - 1)]++;
                    }
                    int l = 0, r = 0, k = 0;
                    for (int i = 1; i < _node; i++) {
                        if (r + number[k] == i) {
                            l = r;
                            r += number[k];
                            k++;
                        }
                        int f = rnd.next(l, r - 1);
                        __add_edge(_p[f], _p[i]);
                    }
                }
            };

            class DegreeTree : public Tree {
            protected :
                int _max_degree;

            public:
                DegreeTree(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1, int max_degree = -1)
                        :
                        Tree(node, begin_node, is_rooted, root, Pruefer),
                        _max_degree(max_degree) {}

                void set_max_degree(int max_degree) { _max_degree = max_degree; }

                virtual void gen() {
                    if (_max_degree == -1) {
                        if (_node == 1) {
                            _max_degree = 0;
                        } else if (_node == 2) {
                            _max_degree = 1;
                        } else {
                            _max_degree = rnd.next(2, _node - 1);
                        }
                    }
                    __init();
                    if (_node == 1) {
                        return;
                    }
                    std::vector<int> times = rand::rand_sum(_node, _node - 2, 0, _max_degree - 1);
                    std::vector<int> pruefer = rand::shuffle_index(times);
                    __pruefer_decode(pruefer);
                }

                void gen(int max_degree) {
                    _max_degree = max_degree;
                    gen();
                }

                template<typename T = int, typename U = int>
                typename std::enable_if<std::is_integral<T>::value && std::is_integral<U>::value, void>::type
                gen(T l_limit, U r_limit) {
                    _max_degree = (int) rand::rand_int(l_limit, r_limit);
                    gen();
                }

                void gen(const char *format, ...) {
                    FMT_TO_RESULT(format, format, _format);
                    _max_degree = (int) rand::rand_int(_format.c_str());
                    gen();
                }

            protected:
                virtual void __judge_self_limit() {
                    if (_max_degree > _node - 1) {
                        io::__warn_msg(io::_err,
                                        "The max degree limit %d is greater than node - 1, equivalent to use Tree::gen_pruefer",
                                        _max_degree);
                    }
                    if (_node == 1 && _max_degree < 0) {
                        io::__fail_msg(io::_err,
                                        "The max degree limit of 1 node's tree is greater than or equal to 0, but found %d.",
                                        _max_degree);
                    }
                    if (_node == 2 && _max_degree < 1) {
                        io::__fail_msg(io::_err,
                                        "The max degree limit of 2 node's tree is greater than or equal to 1, but found %d.",
                                        _max_degree);
                    }
                    if (_node > 2 && _max_degree < 2) {
                        io::__fail_msg(io::_err,
                                        "The max degree limit of 3 or more node's tree is greater than or equal to 2, but found %d.",
                                        _max_degree);
                    }
                }
            };

            class SonTree : public Tree {
            protected :
                int _max_son;

            public:
                SonTree(int node = 1, int begin_node = 1, int root = 1, int max_son = -1) :
                        Tree(node, begin_node, true, root, Pruefer),
                        _max_son(max_son) {}

                void set_is_rooted(bool is_rooted) = delete;
                void set_max_son(int max_son) { _max_son = max_son; }

                virtual void gen() {
                    if (_max_son == -1) {
                        if (_node == 1) {
                            _max_son = 0;
                        } else if (_node == 2) {
                            _max_son = 1;
                        } else {
                            _max_son = rnd.next(2, _node - 1);
                        }
                    }
                    __init();
                    if (_node == 1) {
                        return;
                    }

                    int max_degree = _max_son + 1;
                    std::vector<int> times = rand::rand_sum(_node, _node - 2, 0, max_degree - 1);
                    if (times[_root] == max_degree - 1) {
                        int p;
                        do {
                            p = rnd.next(0, _node - 1);
                        } while (p == _root || times[p] == max_degree - 1);
                        std::swap(times[_root], times[p]);
                    }
                    std::vector<int> pruefer = rand::shuffle_index(times);
                    __pruefer_decode(pruefer);
                }

                void gen(int max_degree) {
                    _max_son = max_degree;
                    gen();
                }

                template<typename T = int, typename U = int>
                typename std::enable_if<std::is_integral<T>::value && std::is_integral<U>::value, void>::type
                gen(T l_limit, U r_limit) {
                    _max_son = (int) rand::rand_int(l_limit, r_limit);
                    gen();
                }

                void gen(const char *format, ...) {
                    FMT_TO_RESULT(format, format, _format);
                    _max_son = (int) rand::rand_int(_format.c_str());
                    gen();
                }

            protected:
                virtual void __judge_self_limit() {
                    if (_max_son > _node - 1) {
                        io::__warn_msg(io::_err,
                                        "The max son limit %d is greater than node - 1, equivalent to use Tree::gen_pruefer",
                                        _max_son);
                    }
                    if (_node == 1 && _max_son < 0) {
                        io::__fail_msg(io::_err,
                                        "The max son limit of 1 node's tree is greater than or equal to 0, but found %d.",
                                        _max_son);
                    }
                    if (_node == 2 && _max_son < 1) {
                        io::__fail_msg(io::_err,
                                        "The max son limit of 2 node's tree is greater than or equal to 1, but found %d.",
                                        _max_son);
                    }
                    if (_node > 2 && _max_son < 2) {
                        io::__fail_msg(io::_err,
                                        "The max son limit of 3 or more node's tree is greater than or equal to 2, but found %d.",
                                        _max_son);
                    }
                }
            };

            class BasicGraph {
            protected:
                int _node, _side;// number of node, number of edge
                int _begin_node;
                std::vector <Edge> _edge;
                std::map<Edge, bool> _e;
                bool _direction;
                bool _multiply_edge;
                bool _self_loop;
                bool _connect;
                bool _swap_node;
                bool _output_node;
                bool _output_side;    
            public:
                BasicGraph(
                        int node,
                        int side,
                        int begin_node,
                        bool direction,
                        bool multiply_edge,
                        bool self_loop,
                        bool connect,
                        bool output_node,
                        bool output_side) :
                        _node(node),
                        _side(side),
                        _begin_node(begin_node),
                        _direction(direction),
                        _multiply_edge(multiply_edge),
                        _self_loop(self_loop),
                        _connect(connect),
                        _swap_node(_direction ? false : true),
                        _output_node(output_node),
                        _output_side(output_side) {}

                BasicGraph(
                        int node,
                        int side,
                        int begin_node,
                        bool direction,
                        bool multiply_edge,
                        bool self_loop,
                        bool connect,
                        bool swap_node,
                        bool output_node,
                        bool output_side) :
                        _node(node),
                        _side(side),
                        _begin_node(begin_node),
                        _direction(direction),
                        _multiply_edge(multiply_edge),
                        _self_loop(self_loop),
                        _connect(connect),
                        _swap_node(swap_node),
                        _output_node(output_node),
                        _output_side(output_side) {}

                void set_node(int node) { _node = node; }

                void set_node_count(int count) { set_node(count); }

                void set_side(int side) { _side = side; }

                void set_side_count(int count) { set_side(count); }

                void set_edge_count(int count) { set_side(count); }

                void set_begin_node(int begin_node) { _begin_node = begin_node; }

                void set_direction(bool direction) { _direction = direction; }

                void set_multiply_edge(bool multiply_edge) { _multiply_edge = multiply_edge; }

                void set_self_loop(bool self_loop) { _self_loop = self_loop; }

                void set_connect(bool connect) { _connect = connect; }

                void set_swap_node(bool swap_node) { _swap_node = swap_node; }

                std::vector <Edge> edge() { return _edge; }

                virtual void gen() {
                    __init();
                    int m = _side;
                    if (_connect) {
                        m -= _node - 1;
                        __gen_connect();
                    }
                    __gen_edges(m);
                    shuffle(_edge.begin(), _edge.end());
                }

                void set_output_node(bool output_node) { _output_node = output_node; }

                void set_output_node_count(bool output_node_count) { set_output_node(output_node_count); }

                void set_output_side(bool output_side) { _output_side = output_side; }

                void set_output_side_count(bool output_side_count) { set_output_side(output_side_count); }

                void set_output_edge_count(bool output_edge_count) { set_output_side(output_edge_count); }

                friend std::ostream &operator<<(std::ostream &os, const BasicGraph &graph) {
                    std::string first_line = "";
                    first_line += graph.__format_output_node();
                    if (graph._output_side) {
                        if (first_line != "") {
                            first_line += " ";
                        }
                        first_line += std::to_string(graph._side);
                    }
                    if (first_line != "") {
                        os << first_line;
                        if (graph._side > 0) {
                            os << "\n";
                        }
                    }
                    int cnt = 0;
                    for (Edge e: graph._edge) {
                        os << e;
                        if (++cnt < graph._side) {
                            os << "\n";
                        }
                    }
                    return os;
                }
                
                void println() {
                    std::cout<<*this<<std::endl;
                }
            protected:
                virtual std::string __format_output_node() const {
                    if (_output_node) {
                        return std::to_string(_node);
                    }
                    return "";
                }

                bool __judge_self_loop(int u, int v) {
                    return !_self_loop && u == v;
                }

                bool __judge_multiply_edge(int u, int v) {
                    if (_multiply_edge) {
                        return false;
                    }
                    if (_e[{u, v}]) {
                        return true;
                    }
                    // if(!_direction && _e[{v, u}]) {
                    //     return true;
                    // }
                    return false;
                }

                void __add_edge(int u, int v) {
                    if (!_multiply_edge) {
                        _e[{u, v}] = true;
                        if (!_direction) {
                            _e[{v, u}] = true;
                        }
                    }
                    u += _begin_node;
                    v += _begin_node;
                    if (!_swap_node || rnd.next(2)) {
                        _edge.emplace_back(u, v);
                    } else {
                        _edge.emplace_back(v, u);
                    }
                }

                void __add_edge(Edge e) {
                    int u = e.u();
                    int v = e.v();
                    __add_edge(u, v);
                }

                virtual void __judge_upper_limit() {
                    long long limit;
                    if (!_multiply_edge) {
                        limit = (long long) _node * (long long) (_node - 1) / 2;
                        if (_direction) {
                            limit *= 2;
                        }
                        if (_self_loop) {
                            limit += _node;
                        }
                        if (_side > limit) {
                            io::__fail_msg(io::_err, "number of edges must less than or equal to %lld.", limit);
                        }
                    }
                }

                virtual void __judge_lower_limit() {
                    if (_side < 0) {
                        io::__fail_msg(io::_err, "number of edges must be a non-negative integer.");
                    }
                    if (_connect && _side < _node - 1) {
                        io::__fail_msg(io::_err, "number of edges must greater than or equal to %d.", _node - 1);
                    }
                }

                virtual void __judge_self_limit() {}

                void __judge_limits() {
                    __judge_lower_limit();
                    __judge_upper_limit();
                    __judge_self_limit();
                }

                virtual Edge __rand_edge() {
                    int u, v;
                    do {
                        u = rnd.next(_node);
                        v = rnd.next(_node);
                    } while (__judge_self_loop(u, v) || __judge_multiply_edge(u, v));
                    return {u, v};
                }

                virtual void __self_init() {}

                void __init() {
                    _edge.clear();
                    if (!_multiply_edge) {
                        _e.clear();
                    }
                    __self_init();
                    __judge_limits();
                }

                virtual void __gen_connect() {
                    Tree tree(_node, 0);
                    tree.gen();
                    std::vector <Edge> edge = tree.edge();
                    for (auto e: edge) {
                        __add_edge(e);
                    }
                }

                virtual void __gen_edges(int m) {
                    while (m--) {
                        __add_edge(__rand_edge());
                    }
                }
            };

            class Graph : public BasicGraph {
            public:
                Graph(int node = 1, int side = 0, int begin_node = 1) :
                        BasicGraph(
                                node, side, begin_node,
                                false, false, false, false,
                                true, true) {}
            };

            class BipartiteGraph : public BasicGraph {
            public:
                enum OutputNodeType {
                    Node,
                    LeftRight,
                    NodeLeft,
                    NodeRight
                };
            protected:
                int _left, _right;
                bool _different_part;
                OutputNodeType _output_node_type;
                std::vector<int> _part[2];
                std::vector<int> _degree[2];
                int _d[2];
            public:
                BipartiteGraph(int node = 1, int side = 0, int begin_node = 1, int left = -1) :
                        BasicGraph(
                                node, side, begin_node,
                                false, false, false, false, false,
                                true, true),
                        _left(left),
                        _different_part(false),
                        _output_node_type(Node) {}

                void set_direction(bool direction) = delete;

                void set_self_loop(bool self_loop) = delete;

                void set_swap_node(bool swap_node) { _swap_node = swap_node; }

                void set_different_part(bool different_part) { _different_part = different_part; }

                void set_left(int left) {
                    _left = left;
                    _right = _node - _left;
                }

                void set_right(int right) {
                    _right = right;
                    _left = _node - _right;
                }

                void set_left_right(int left, int right) {
                    if (left + right < 0) {
                        io::__fail_msg(
                                io::_err,
                                "number of left part nodes add right part nodes must greater than 0."
                                "But found %d + %d = %d",
                                left, right, left + right);
                    }
                    _left = left;
                    _right = right;
                    _node = left + right;
                }

                void set_output_node_type(OutputNodeType type) { _output_node_type = type; }

                void use_type_node() { _output_node_type = Node; }

                void use_type_left_right() { _output_node_type = LeftRight; }

                void use_type_node_left() { _output_node_type = NodeLeft; }

                void use_type_node_right() { _output_node_type = NodeRight; }

            protected:
                virtual std::string __format_output_node() const {
                    std::string str = "";
                    if (_output_node) {
                        if (_output_node_type == Node) {
                            str += std::to_string(_node);
                        } else if (_output_node_type == LeftRight) {
                            str += std::to_string(_left);
                            str += " ";
                            str += std::to_string(_right);
                        } else if (_output_node_type == NodeLeft) {
                            str += std::to_string(_node);
                            str += " ";
                            str += std::to_string(_left);
                        } else if (_output_node_type == NodeRight) {
                            str += std::to_string(_node);
                            str += " ";
                            str += std::to_string(_right);
                        }
                    }
                    return str;
                }

                void __rand_left() {
                    if (_left >= 0) {
                        return;
                    }
                    int l = 0, r = _node / 2, limit;
                    if (!_multiply_edge) {
                        if (_side > r * (_node - r)) {
                            io::__fail_msg(
                                    io::_err,
                                    "number of edges must less than or equal to %d.",
                                    r * (_node - r));
                        }
                        while (l <= r) {
                            int mid = (l + r) / 2;
                            if (mid * (_node - mid) < _side) {
                                l = mid + 1;
                            } else {
                                limit = r;
                                r = mid - 1;
                            }
                        }
                    } else {
                        limit = 1;
                    }
                    _left = rnd.next(limit, _node / 2);
                    _right = _node - _left;
                }

                virtual void __self_init() {
                    __rand_left();
                    _right = _node - _left;
                    for (int i = 0; i < 2; i++) {
                        _part[i].clear();
                    }
                    if (_different_part) {
                        _part[0] = rnd.perm(_left, 0);
                        _part[1] = rnd.perm(_right, 0);
                        for (auto &x: _part[1]) {
                            x += _left;
                        }
                    } else {
                        std::vector<int> p = rnd.perm(_node, 0);
                        for (int i = 0; i < _left; i++) {
                            _part[0].push_back(p[i]);
                        }
                        for (int i = _left; i < _node; i++) {
                            _part[1].push_back(p[i]);
                        }
                    }
                    if (_connect) {
                        _degree[0].resize(_left, 1);
                        _degree[1].resize(_right, 1);
                        for (int i = _left; i < _node - 1; i++) {
                            _degree[0][rnd.next(_left)]++;
                        }
                        for (int i = _right; i < _node - 1; i++) {
                            _degree[1][rnd.next(_right)]++;
                        }
                        _d[0] = _node - 1;
                        _d[1] = _node - 1;
                    }
                }

                virtual void __judge_self_limit() {
                    if (_left < 0) {
                        io::__fail_msg(io::_err, "Left part size must greater than or equal to 0, but found %d",
                                        _left);
                    }
                    if (_right < 0) {
                        io::__fail_msg(io::_err, "Left part size must greater than or equal to 0, but found %d",
                                        _right);
                    }
                }

                virtual void __judge_upper_limit() {
                    if (!_multiply_edge) {
                        long long limit = _left * _right;
                        if (limit < _side) {
                            io::__fail_msg(io::_err, "number of edges must less than or equal to %lld, but found %d.",
                                            limit, _side);
                        }
                    }
                }

                virtual Edge __rand_edge() {
                    int u, v;
                    do {
                        u = rnd.any(_part[0]);
                        v = rnd.any(_part[1]);
                    } while (__judge_multiply_edge(u, v));
                    return {u, v};
                }

                void __add_part_edge(int f, int i, int j) {
                    int u = _part[f][i];
                    int v = _part[f ^ 1][j];
                    if (f == 1) {
                        std::swap(u, v);
                    }
                    __add_edge(u, v);
                    _d[0]--;
                    _d[1]--;
                    _degree[f][i]--;
                    _degree[f ^ 1][j]--;
                }

                virtual void __gen_connect() {
                    int f = 0;
                    while (_d[0] + _d[1] > 0) {
                        for (int i = 0; i < (f == 0 ? _left : _right); i++) {
                            if (_degree[f][i] == 1) {
                                if (_d[f] == 1) {
                                    for (int j = 0; j < (f == 0 ? _right : _left); j++) {
                                        if (_degree[f ^ 1][j] == 1) {
                                            __add_part_edge(f, i, j);
                                        }
                                    }
                                } else {
                                    int j;
                                    do {
                                        j = rnd.next(f == 0 ? _right : _left);
                                    } while (_degree[f ^ 1][j] < 2);
                                    __add_part_edge(f, i, j);
                                }
                            }
                        }
                        f ^= 1;
                    }
                }

                virtual void __gen_edges(int m) {
                    while (m--) {
                        __add_edge(__rand_edge());
                    }
                    if (_different_part) {
                        for (auto &edge: _edge) {
                            int &u = edge.u();
                            int &v = edge.v();
                            if (u - _begin_node >= _left) {
                                u -= _left;
                            }
                            if (v - _begin_node >= _left) {
                                v -= _left;
                            }
                        }
                    }
                }
            };

            class DAG : public BasicGraph {
            protected:
                std::vector<int> _p;
            public:
                DAG(int node = 1, int side = 0, int begin_node = 1) :
                        BasicGraph(
                                node, side, begin_node,
                                true, false, false, false,
                                true, true) {}

                void set_direction(bool direction) = delete;

                void set_self_loop(bool self_loop) = delete;

            protected:
                virtual void __self_init() {
                    _p = rnd.perm(_node, 0);
                }

                virtual void __gen_connect() {
                    for (int i = 1; i < _node; i++) {
                        int f = rnd.next(i);
                        __add_edge(_p[f], _p[i]);
                    }
                }

                virtual Edge __rand_edge() {
                    int u, v;
                    do {
                        u = rnd.next(_node);
                        v = rnd.next(_node);
                        if (u > v) {
                            std::swap(u, v);
                        }
                        u = _p[u];
                        v = _p[v];
                    } while (__judge_self_loop(u, v) || __judge_multiply_edge(u, v));
                    return {u, v};
                }
            };

            class CycleGraph : public BasicGraph {
            public:
                CycleGraph(int node = 3, int begin_node = 1) :
                        BasicGraph(
                                node, node, begin_node,
                                false, false, false, true,
                                true, true) {}

                void set_multiply_edge() = delete;

                void set_connect() = delete;

                void set_self_loop() = delete;

                void set_side(int side) = delete;

                void set_side_count(int count) = delete;

                void set_edge_count(int count) = delete;

                virtual void gen() {
                    __init();
                    std::vector<int> p = rnd.perm(_node, 0);
                    for (int i = 0; i < _node; i++) {
                        __add_edge(p[i], p[(i + 1) % _node]);
                    }
                    shuffle(_edge.begin(), _edge.end());
                }

            protected:
                virtual void __self_init() {
                    _side = _node;
                }

                virtual void __judge_lower_limit() {
                    if (_node < 3) {
                        io::__fail_msg(io::_err, "number of nodes must greater than or equal to 3, but found %d.",
                                        _node);
                    }
                }
            };

            class WheelGraph : public BasicGraph {
            public:
                WheelGraph(int node = 4, int begin_node = 1) :
                        BasicGraph(
                                node, 2 * node - 2, begin_node,
                                false, false, false, true,
                                true, true) {}

                void set_multiply_edge() = delete;

                void set_connect() = delete;

                void set_self_loop() = delete;

                void set_side(int side) = delete;

                void set_side_count(int count) = delete;

                void set_edge_count(int count) = delete;

                virtual void gen() {
                    __init();
                    std::vector<int> p = rnd.perm(_node, 0);
                    for (int i = 0; i < _node - 1; i++) {
                        __add_edge(p[i], p[(i + 1) % (_node - 1)]);
                        __add_edge(p[i], p[_node - 1]);
                    }
                    shuffle(_edge.begin(), _edge.end());
                }

            protected:
                virtual void __self_init() {
                    _side = 2 * _node - 2;
                }

                virtual void __judge_lower_limit() {
                    if (_node < 4) {
                        io::__fail_msg(io::_err, "number of nodes must greater than or equal to 4, but found %d.",
                                        _node);
                    }
                }
            };

            class GridGraph : public BasicGraph {
            protected:
                int _row, _column;
                std::vector<int> _p;
            public:
                GridGraph(int node = 1, int side = 0, int begin_node = 1, int row = -1) :
                        BasicGraph(
                                node, side, begin_node,
                                false, false, false, false,
                                true, true),
                        _row(row) {}

                void set_self_loop(bool self_loop) = delete;

                void set_row(int row) { _row = row; }


            protected:
                int __count_side(int row, int column) {
                    long long xl = (long long) row;
                    long long yl = (long long) column;
                    long long sum = xl * (yl - 1) + yl * (xl - 1) - 2 * (xl * yl - _node);
                    if (_direction) {
                        sum *= 2;
                    }
                    return (int) sum;
                }

                virtual void __judge_upper_limit() {
                    long long limit = 0;
                    if (!_multiply_edge) {
                        limit = __count_side(_row, _column);
                        if (_direction) {
                            limit *= 2;
                        }
                        if (_side > limit) {
                            io::__fail_msg(io::_err, "number of edges must less than or equal to %lld, but found %d.",
                                            limit, _side);
                        }
                    }
                }

                virtual void __judge_self_limit() {
                    if (_row < 0) {
                        io::__fail_msg(io::_err, "number of rows must greater than 0, but found %d.", _row);
                    }
                    if (_column < 0) {
                        io::__fail_msg(io::_err, "number of columns must greater than 0, but found %d.", _column);
                    }
                }

                void __rand_row() {
                    if (!_multiply_edge) {
                        std::pair<int, int> max = {0, 0};
                        std::vector<int> possible;
                        for (int i = 1; i <= _node; i++) {
                            int x = i, y = (_node + i - 1) / i;
                            long long w = __count_side(x, y);
                            if (_direction) {
                                w *= 2;
                            }
                            if (w > max.first) {
                                max = {w, i};
                            }
                            if (w >= _side) {
                                possible.push_back(i);
                            }
                        }
                        if (possible.size() == 0) {
                            _side = max.first;
                            io::__warn_msg(io::_err,
                                            "number of edges is large than the maximum possible, use upper edges limit %d.",
                                            _side);
                            _row = max.second;
                        } else {
                            _row = rnd.any(possible);
                        }
                    } else {
                        _row = rnd.next(1, _node);
                    }
                }

                virtual void __self_init() {
                    _p = rnd.perm(_node, 0);
                    if (_row == -1) {
                        __rand_row();
                    }
                    _column = (_node + _row - 1) / _row;
                }

                virtual void __gen_connect() {
                    for (int i = 0; i < _row; i++) {
                        for (int j = 1; j < _column; j++) {
                            int x = i * _column + j, y = x - 1;
                            if (x >= _node) continue;
                            __add_edge(_p[x], _p[y]);
                        }
                        int x = i * _column, y = (i + 1) * _column;
                        if (x < _node && y < _node) {
                            __add_edge(_p[x], _p[y]);
                        }
                    }
                }

                virtual Edge __rand_edge() {
                    int d[4][2] = {{1, 0},
                                   {-1, 0},
                                   {0, 1},
                                   {0, -1}};
                    int pos, k, px, py, nxt;
                    do {
                        pos = rnd.next(_node);
                        k = rnd.next(4);
                        px = pos / _column + d[k][0];
                        py = pos % _column + d[k][1];
                        nxt = px * _column + py;
                    } while (px < 0 || px >= _row || py < 0 || py >= _column || nxt >= _node ||
                             __judge_multiply_edge(_p[pos], _p[nxt]));
                    return {_p[pos], _p[nxt]};
                }
            };

            class PseudoTree : public BasicGraph {
            protected:
                int _cycle;
                std::vector<int> _p;
            public:
                PseudoTree(int node = 3, int begin_node = 1, int cycle = -1) :
                        BasicGraph(
                                node, node, begin_node,
                                false, false, false, true,
                                true, true),
                        _cycle(cycle) {}

                void set_direction(bool direction) = delete;

                void set_multiply_edge(bool multiply_edge) = delete;

                void set_self_loop(bool self_loop) = delete;

                void set_connect(bool connect) = delete;

                void set_cycle(int cycle) { _cycle = cycle; }

                void set_side(int side) = delete;

                void set_side_count(int count) = delete;

                void set_edge_count(int count) = delete;

                virtual void gen() {
                    __init();
                    __gen_cycle();
                    __gen_edges();
                    shuffle(_edge.begin(), _edge.end());
                }

            protected:
                PseudoTree(int node, int begin_node, int cycle, bool direction) :
                        BasicGraph(
                                node, node, begin_node,
                                direction, false, false, true,
                                true, true),
                        _cycle(cycle) {}

                virtual void __self_init() {
                    _side = _node;
                    _p = rnd.perm(_node, 0);
                    if (_cycle == -1) {
                        _cycle = rnd.next(3, _node);
                    }
                }

                virtual void __judge_self_limit() {
                    if (_cycle < 3 || _cycle > _node) {
                        io::__fail_msg(io::_err, "cycle size must in range [3, %d], but found %d.", _node, _cycle);
                    }
                }

                virtual void __judge_lower_limit() {
                    if (_node < 3) {
                        io::__fail_msg(io::_err,
                                        "number of nodes in cycle graph must greater than or equal to 3, but found %d.",
                                        _cycle);
                    }
                }

                virtual void __gen_cycle() {
                    CycleGraph cycle(_cycle, 0);
                    cycle.set_swap_node(_swap_node);
                    cycle.gen();
                    std::vector <Edge> edge = cycle.edge();
                    for (auto &e: edge) {
                        __add_edge(_p[e.u()], _p[e.v()]);
                    }
                }

                virtual void __gen_edges() {
                    for (int i = _cycle; i < _node; i++) {
                        int f = rnd.next(i);
                        __add_edge(_p[i], _p[f]);
                    }
                }
            };

            class PseudoInTree : public PseudoTree {
            public:
                PseudoInTree(int node = 3, int begin_node = 1, int cycle = -1) :
                        PseudoTree(node, begin_node, cycle, true) {}

                void set_swap_node(bool swap_node) = delete;
            };

            class PseudoOutTree : public PseudoTree {
            public:
                PseudoOutTree(int node = 3, int begin_node = 1, int cycle = -1) :
                        PseudoTree(node, begin_node, cycle, true) {}

                void set_swap_node(bool swap_node) = delete;

            protected:
                virtual void __gen_edges() {
                    for (int i = _cycle; i < _node; i++) {
                        int f = rnd.next(i);
                        __add_edge(_p[f], _p[i]);
                    }
                }
            };

            class Cactus : public BasicGraph {
            protected:
                std::vector<int> _p;
            public:
                Cactus(int node = 1, int side = 0, int begin_node = 1) :
                    BasicGraph(
                        node, side, begin_node,
                        false, false, false, true,
                        true, true)
                {}
                void set_direction(bool direction) = delete;

                void set_multiply_edge(bool multiply_edge) = delete;

                void set_self_loop(bool self_loop) = delete;

                void set_connect(bool connect) = delete;

                virtual void gen() {
                    __init();
                    std::vector<std::vector<int>> cycles;
                    int m = _side - (_node - 1);
                    for (int i = 2; i <= 2 * m; i += 2) {
                        std::vector<int> pre;
                        if (i == 2) {
                            pre.emplace_back(0);
                        }
                        pre.emplace_back(i);
                        pre.emplace_back(i - 1);
                        cycles.emplace_back(pre);
                    }
                    int len = cycles.size();
                    int add = len == 0 ? 0 : rnd.next(0, _node - (2 * m + 1));
                    for (int i = 2 * m + 1; i <= 2 * m + add; i++) {
                        int w = rnd.next(len);
                        cycles[w].emplace_back(i);
                    }
                    for (int i = 2 * m + add + (len != 0); i < _node; i++) {
                        cycles.emplace_back(1, i);
                    }
                    shuffle(cycles.begin() + 1, cycles.end());
                    for(size_t i = 0; i < cycles.size(); i++) {
                        std::vector<int> pre = cycles[i];
                        if (i != 0) {
                            int w = rnd.next(i);
                            pre.push_back(rnd.any(cycles[w]));
                        }
                        if(pre.size() == 1) {
                            continue;
                        }
                        else if(pre.size() == 2) {
                            __add_edge(_p[pre[0]], _p[pre[1]]);
                        }
                        else {
                            CycleGraph cycle(pre.size(), 0);
                            cycle.gen();
                            std::vector<Edge> edge = cycle.edge();
                            for(auto& e : edge) {
                                __add_edge(_p[pre[e.u()]], _p[pre[e.v()]]);
                            }
                        }
                    }
                    shuffle(_edge.begin(), _edge.end());
                }
            protected:
                virtual void __judge_upper_limit() {
                    int limit = _node - 1 + (_node - 1) / 2;
                    if (_side > limit) {
                        io::__fail_msg(io::_err, "number of edges must less than or equal to %d, but found %d.",
                                        limit, _side);
                    }
                }

                virtual void __self_init() {
                    _p = rnd.perm(_node, 0);
                }
            };
        }

        namespace edge_weight{}
        namespace point_weight{}
        namespace both_weight{}
    }
    
    namespace all{
        using namespace generator::io;
        using namespace generator::rand;
        using namespace generator::graph;
    }
}
#ifdef _WIN32
#undef mkdir
#endif
