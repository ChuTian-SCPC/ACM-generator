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
#endif
namespace generator{
    namespace msg{
        class OutStream {
        public:
            OutStream(const std::string& filename = "") {
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
                    output_stream_ptr_ = &std::cout;
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
                va_list args;
                va_start(args, format);
                char buffer[65536];
                vsnprintf(buffer, sizeof(buffer), format, args);
                va_end(args);

                *output_stream_ptr_ << buffer;
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

        // compare msg outstream, default on stderr
        // same with quitf, use freopen redirect to log file
        // may unsafe
        OutStream _cmp;

        template <typename... Args>
        void __fail_msg(OutStream& out,const char* msg, Args... args) {
            out.printf("\033[1;31mFAIL\033[0m ");
            out.printf(msg,args...,nullptr);
            out << std::endl;
            exit(EXIT_FAILURE);
        }

        template <typename... Args>
        void __success_msg(OutStream& out,const char* msg, Args... args) {
            out.printf("\033[32mSUCCESS\033[0m ");
            out.printf(msg,args..., nullptr);
            out << std::endl;
            return;
        }

        template <typename... Args>
        void __info_msg(OutStream& out,const char* msg, Args... args) {
            out.printf(msg,args..., nullptr);
            out << std::endl;
            return;
        }

        template <typename... Args>
        void __warn_msg(OutStream& out,const char* msg, Args... args) {
            out.printf("\033[1;33mWARN\033[0m ");
            out.printf(msg,args..., nullptr);
            out << std::endl;
            return;
        }

        template <typename... Args>
        void __error_msg(OutStream& out,const char* msg, Args... args) {
            out.printf("\033[1;31mERROR\033[0m ");
            out.printf(msg,args...,nullptr);
            out.printf("\n");
            out << std::endl;
            exit(EXIT_FAILURE);
        }

        void __ac_msg(OutStream& out,bool color,int case_id,int runtime){
            if(color){
                out.printf("Testcase %d :\033[32mAC\033[0m ,Runtime = %dms.",case_id,runtime);
            }
            else{
                out.printf("Testcase %d :AC ,Runtime = %dms.",case_id,runtime);
            }
            out << std::endl;
        }

        void __wa_msg(OutStream& out,bool color,int case_id,int runtime,std::string& result){
            if(color){
                out.printf("Testcase %d :\033[1;31mWA\033[0m ,Runtime = %dms. ",case_id,runtime);
                out.printf("\033[1;31mchecker return\033[0m :\n");
                out.printf("%s",result.c_str());
            }
            else{
                out.printf("Testcase %d :WA ,Runtime = %dms. ",case_id,runtime);
                out.printf("checker return :\n");
                out.printf("%s",result.c_str());
            }
            out << std::endl;
        }

        void __tleac_msg(OutStream& out,bool color,int case_id,int runtime){
            if(color){
                out.printf("Testcase %d :\033[1;33mTLE\033[0m(\033[32mAC\033[0m) ,Runtime = %dms.",case_id,runtime);
            }
            else{
                out.printf("Testcase %d :TLE(AC) ,Runtime = %dms.",case_id,runtime);
            }
            out << std::endl;
        }

        void __tlewa_msg(OutStream& out,bool color,int case_id,int runtime,std::string& result){
            if(color){
                out.printf("Testcase %d :\033[1;33mTLE\033[0m(\033[1;31mWA\033[0m) ,Runtime = %dms. ",case_id,runtime);
                out.printf("\033[1;31mchecker return :\033[0m\n");
                out.printf("%s",result.c_str());
            }
            else{
                out.printf("Testcase %d :TLE(WA) ,Runtime = %dms. ",case_id,runtime);
                out.printf("checker return :\n");
                out.printf("%s",result.c_str());
            }
            out << std::endl;
        }

        void __tle_msg(OutStream& out,bool color,int case_id,int runtime){
            if(color){
                out.printf("Testcase %d :\033[1;33mTLE\033[0m ,Runtime = %dms (killed).",case_id,runtime);  
            }
            else{
                out.printf("Testcase %d :TLE ,Runtime = %dms (killed).",case_id,runtime);
            }
            out << std::endl;
        }

        void __run_err_msg(OutStream& out,bool color,int case_id){
            if(color){
                out.printf("Testcase %d :\033[1;31mERROR\033[0m ,meet some error,pleace check it or report.",case_id);
            }
            else{
                out.printf("Testcase %d :ERROR ,meet some error,pleace check it or report.",case_id);
            }
            out << std::endl;
        }
    }

    namespace rand{
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

        // rand a odd number between [1,n]
        template <typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, long long>::type
        rand_odd(T n){
            long long nl = (long long)n;
            long long r = (nl - (nl % 2 == 0) - 1LL)/2;
            if(r < 0) {
                msg::__fail_msg(msg::_err,"There is no odd number between [1,%lld].",nl);
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
                msg::__fail_msg(msg::_err,"There is no odd number between [%lld,%lld].",froml,tol);
            }
            long long x = rnd.next(l, r);
            x = x * 2 + 1;       
            return x;
        }

        // rand a even number between [0,n]
        template <typename T = long long>
        typename std::enable_if<std::is_integral<T>::value, long long>::type
        rand_even(T n){
            long long nl = (long long)n;
            long long r = (nl - (nl % 2 == 1) - 1LL)/2;
            if(r < 0) {
                msg::__fail_msg(msg::_err,"There is no even number between [0,%lld].",nl);
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
            long long l = (froml + (froml % 2 == 1) - 1LL)/2;
            long long r = (tol - (tol % 2 == 1) - 1LL)/2;
            if(l > r) {
                msg::__fail_msg(msg::_err,"There is no even number between [%lld,%lld].",froml,tol);
            }
            long long x = rnd.next(l, r);
            x = x * 2;       
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
                msg::__warn_msg(msg::_err,"Input is not a real number, change it to %lf.Please ensure it's correct.",_n);
            }
            else{
                msg::__fail_msg(msg::_err,"Input is not a real number, and can't be changed to it.");
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
                        msg::__fail_msg(msg::_err,"Exist a nan number.");
                    }
                    if(std::isinf(value)){
                        msg::__fail_msg(msg::_err,"Exist an inf number.");
                    }
                    int digit = 0;
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
                    msg::__fail_msg(msg::_err,"%s is an invalid argument.", str.c_str());
                }
                catch (const std::out_of_range &e) {
                    msg::__fail_msg(msg::_err,"%s is out of range.", str.c_str());
                }
            };
            if(open == std::string::npos || close == std::string::npos || comma == std::string::npos){
                msg::__fail_msg(msg::_err,"%s is an invalid range.",s.c_str());
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

        // return a number in range (-n,n)
        // return value type depend on the input type
        template <typename T>
        auto rand_abs(T n) -> decltype(n) {
            int op = rnd.next(0,1);
            if(std::is_integral<T>::value){
                long long x = rnd.next(n);
                return op?x:-x;
            }
            else if(std::is_floating_point<T>::value){
                double x = rnd.next(n);
                return op?x:-x;
            }
            else{
                msg::__fail_msg(msg::_err,"Invalid type.");
            }
        }

        // return a real number in range (-to,-from]U[from,to)
        template <typename T, typename U>
        typename std::enable_if<is_double_valid<T>() && is_double_valid<U>(), double>::type
        rand_abs(T from, U to) {
            double x = rand_real(from,to);
            int op = rnd.next(0,1);
            return op?x:-x;
        }

        // return a integer number in range [-to,-from]U[from,to]
        template <typename T>
        typename std::enable_if<std::is_integral<T>::value,long long>::type
        rand_abs(T from, T to) {
            long long x = rnd.next(from,to);
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
        // return a integer vector,which elements in range [from,to] and sum equals to sum
        template<typename T>
        std::vector<T> rand_sum(int size,T sum,T from,T to) {
            if(size < 0){
                msg::__fail_msg(msg::_err,"Size of the vector can't less than zero.");
            }
            if(size > 10000000){
                msg::__warn_msg(
                    msg::_err,
                    "Size of the vector is greater than %s,since it's too large!",
                    std::to_string(size).c_str()
                );
            }
            if(from > to){
                msg::__fail_msg(
                    msg::_err,
                    "Vaild range [%s,%s],to can't less than from.",
                    std::to_string(from).c_str(),
                    std::to_string(to).c_str()
                );
            }
            if(size * from > sum || size * to < sum){
                msg::__fail_msg(
                    msg::_err,
                    "Sum of the vector is in range [%s,%s],but need sum = %s.",
                    std::to_string(from * size).c_str(),
                    std::to_string(to * size).c_str(),
                    std::to_string(sum).c_str());
            }
            if(size == 0) {
                if(sum != 0){
                    msg::__fail_msg(msg::_err,"Sum of the empty vector must be 0.");
                }
                return std::vector<T>();
            }

            T ask_sum = sum;

            std::vector<T> v(size,0);
            sum -= from * size;
            T right = to - from;
            for(int i = 0;i < size;i++){
                v[i] = rnd.next(std::max(T(0),sum - (size - i - 1) * right),std::min(sum,right));
                sum -= v[i];
                v[i] += from;
            }

            T result_sum = 0;
            for(int i = 0;i < size;i++){
                if(v[i] < from || v[i] > to){
                    msg::__error_msg(
                        msg::_err,
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
                msg::__error_msg(
                    msg::_err,
                    "Sum of the vector is equal to %s,not %s.Please notice author to fix bug.",
                    std::to_string(result_sum).c_str(),
                    std::to_string(ask_sum).c_str()
                );
            }
            return v;
        }

        // return a rand vector length equal to n
        template <typename T, typename Func>
        std::vector<T> rand_vector(int size,Func func) {
            std::vector<T> v;
            for(int i = 0;i < size; i++){
                T x = func();
                v.emplace_back(x);
            }
            return v;
        }

        // return a rand vector length in range [from,to]
        template <typename T, typename Func>
        std::vector<T> rand_vector(int from,int to,Func func) {
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
                sum += iter.second;
                probs.emplace_back(sum);
            }
            long long p = rand_int(1LL,sum);
            auto pos = lower_bound(probs.begin(),probs.end(),p) - probs.begin();
            return *(elements.begin() + pos);
        }

    }

    namespace io{
        std::string __get_lib_path() {
            return __FILE__;
        }
        std::string __get_current_path() {
    #ifdef _WIN32
        char buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::string executablePath(buffer);
        // 提取文件夹路径
        size_t lastSlashPos = executablePath.find_last_of('\\');
        if (lastSlashPos != std::string::npos) {
            std::string folderPath = executablePath.substr(0, lastSlashPos);
            return folderPath;
        }
    #else
        char buffer[1024];
        ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer));
        if (length != -1) {
            buffer[length] = '\0';
            std::string executablePath(buffer);

            // 提取文件夹路径
            size_t lastSlashPos = executablePath.find_last_of('/');
            if (lastSlashPos != std::string::npos) {
                std::string folderPath = executablePath.substr(0, lastSlashPos);
                return folderPath;
            }
        }
    #endif

        // 如果无法获取路径，则返回空字符串
        return "";
        }

        std::string __get_folder_path(std::string& path){
            size_t pos = path.find_last_of("/\\"); 
            if (pos != std::string::npos) {
                return path.substr(0,pos);
            }
            else{
                return std::string("");
            }
        }

        std::string __get_file_name(std::string& path){
            size_t pos = path.find_last_of("/\\");
            if (pos != std::string::npos) {
                std::string file_full_name = path.substr(pos + 1);
                size_t pos_s = file_full_name.find_first_of(".");
                if( pos_s != std::string::npos) {
                    std::string file_name = file_full_name.substr(0,pos_s);
                    return file_name;
                }
                else{
                    return file_full_name;
                }
            } else {
                size_t pos_s = path.find_first_of(".");
                if( pos_s != std::string::npos) {
                    std::string file_name = path.substr(0,pos_s);
                    return file_name;
                }
                else{
                    return path;
                }
            }
            return "";
        }

        bool __folder_exists(const std::string& folder_path) {
            struct stat info;
            if (stat(folder_path.c_str(), &info) != 0) {
                return false;
            }
            return (info.st_mode & S_IFDIR) != 0;
        }

        bool __file_exists(const std::string& filename) {
            std::ifstream file(filename);
            return file.good();
        }

        bool __create_directory(const std::string& path) {
            if(__folder_exists(path))  return true;
            return mkdir(path.c_str(),0777) == 0;
        }

        void __create_directories(const std::string& path) {
            std::istringstream ss(path);
            std::string token;
            std::string current_path = "";

        #ifdef __WIN32
            while (std::getline(ss, token, '\\')) {
                current_path += token + "\\";
                if (current_path=="\\") {
                    continue;
                }
                if (!__create_directory(current_path)) {
                    msg::__fail_msg(msg::_err,"Error in creating folder : %s",current_path.c_str());
                }
            }
        #else
            while (std::getline(ss, token, '/') ) {
                current_path += token + "/";
                if (current_path=="/") {
                    continue;
                }
                if (!__create_directory(current_path)) {
                    msg::__fail_msg(msg::__err,"Error in creating folder : %s",current_path.c_str());
                }
            }
        #endif
            return;
        }

        void __close_file(){
            #ifdef _WIN32
                freopen("CON", "w", stdout);
            #else
                freopen("/dev/tty", "w", stdout);
            #endif
        }

        std::string __path_join(std::string s,std::string t){
        #ifdef _WIN32
            return s + "\\" + t;
        #else 
            return s + "/" + t;
        #endif
        }

        std::string __path_join(std::string s,const char* t){
        #ifdef _WIN32
            return s + "\\" + std::string(t);
        #else 
            return s + "/" + std::string(t);
        #endif
        }

        std::string __path_join(const char* s,std::string t){
        #ifdef _WIN32
            return std::string(s) + "\\" + t;
        #else 
            return std::string(s) + "/" + t;
        #endif
        }

        std::string __path_join(const char* s,const char* t){
        #ifdef _WIN32
            return std::string(s) + "\\" + std::string(t);
        #else 
            return std::string(s) + "/" + std::string(t);
        #endif
        }

        std::string __get_full_path(std::string& path){
        #ifdef _WIN32
            char buffer[MAX_PATH];
            if (GetFullPathNameA(path.c_str(), MAX_PATH, buffer, nullptr) != 0) {
                return std::string(buffer);
            } else {
                msg::__error_msg(msg::_err,"Error in get full path of file: %s.",path.c_str());
            }
        #else
            char buffer[PATH_MAX];
            if (realpath(path.c_str(), buffer) != nullptr) {
                return std::string(buffer);
            } else {
                msg::__error_msg(msg::__err,"Error in get full path of file: %s.",path.c_str());
            }
        #endif
        }

        char** __split_string_to_char_array(const char* input) {
            char** char_array = nullptr;
            char* mutableInput = const_cast<char*>(input);
            char* token = strtok(mutableInput, " ");
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

        struct Path{
            std::string _path;
            Path() = default;
            Path(std::string &s) {
                _path = __get_current_path();
                _path += s;
            };
            Path(const char *s) {
                _path = __get_current_path();
                _path += std::string(s);
            };
            void change_path(std::string &s) {
                std::string path = s;
                path = __get_full_path(path);
                if(!__file_exists(path)){
                    msg::__warn_msg(msg::_err,"Fail to find generator file %s.",path.c_str());
                    return;
                }
                _path = path;
            }
            void change_path(const char *s) {
                std::string path = std::string(s);
                path = __get_full_path(path);
                if(!__file_exists(path)){
                    msg::__warn_msg(msg::_err,"Fail to find generator file %s.",path.c_str());
                    return;
                }
                _path = path;
            }
            void join_current_path(std::string &s) {
                _path = __get_current_path();
                _path += s;
            }
            void join_current_path(const char *s) {
                _path = __get_current_path();
                _path += std::string(s);
            }
        };

    #ifdef _WIN32       
        Path testcases_folder_path("\\testcases");
        Path compare_folder_path("\\cmp");
        Path std_path("\\std.exe");
        Path data_path("\\data.exe");
        Path checker_path("\\checker.exe");
    #else
        Path testcases_folder_path("/testcases");
        Path compare_folder_path("/cmp");
        Path std_path("/std");
        Path data_path("/data");
        Path checker_path("/checker");
    #endif

        void __write_input_file(int x){
            std::string folder_path = testcases_folder_path._path;
            __create_directories(folder_path);
            std::string in_file = std::to_string(x) + ".in";
            std::string file_path = __path_join(folder_path,in_file) ;
            freopen(file_path.c_str(), "w", stdout);
            msg::__success_msg(msg::_err,"Successfully create input file %s",file_path.c_str());
        }

        void __write_output_file(int x){
            std::string folder_path = testcases_folder_path._path;
            __create_directories(folder_path);
            std::string read_file = std::to_string(x) + ".in";
            std::string read_path = __path_join(folder_path, read_file);
            if(!__file_exists(read_path)) {
                msg::__fail_msg(msg::_err,"Input file %s don't exist!",read_path.c_str());
            }
            freopen(read_path.c_str(), "r", stdin);
            std::string write_file = std::to_string(x) + ".out";
            std::string write_path = __path_join(folder_path, write_file);
            freopen(write_path.c_str(), "w", stdout);
            msg::__success_msg(msg::_err,"Successfully create output file %s",write_path.c_str());
        }

        bool __input_file_exists(int x){
            std::string folder_path = testcases_folder_path._path;
            std::string in_file = std::to_string(x) + ".in";
            std::string file_path = __path_join(folder_path,in_file);
            return __file_exists(file_path);
        }

        std::vector<int> __get_inputs() {
            std::vector<int> inputs;
            std::string folder_path = testcases_folder_path._path;
        #ifdef _WIN32
            WIN32_FIND_DATA findFileData;
            HANDLE hFind = FindFirstFile((folder_path + "\\*.in").c_str(), &findFileData);

            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    std::string file_name = findFileData.cFileName;
                    int num = std::stoi(__get_file_name(file_name));
                    inputs.push_back(num);
                } while (FindNextFile(hFind, &findFileData) != 0);

                FindClose(hFind);
            }
        #else
            DIR* dir = opendir(folder_path.c_str());
            if (dir != nullptr) {
                struct dirent* entry;
                while ((entry = readdir(dir)) != nullptr) {
                    std::string file_name = entry->d_name;
                    if (file_name.size() >= 3 && file_name.substr(file_name.size() - 3) == ".in") {
                        int num = std::stoi(__get_file_name(file_name));
                        inputs.push_back(num);
                    }
                }
                closedir(dir);
            }
        #endif

            return inputs;
        }

        // make(generator) input files from __start to __end
        // store in folder testcases, name is #number.in
        // can given testlib style arguments
        #define make_inputs(__start, __end, __Func, ...) do{ \
            for(int __case = (__start);__case <= __end;__case++){ \
                __write_input_file(__case); \
                __fake_arg(__VA_ARGS__); \
                __Func; \
                __close_file(); \
            } \
        } while(0)

        // make(generator) output files from __start to __end
        // store in folder testcases, name is #number.iout
        #define make_outputs(__start,__end,__Func) do{ \
            for(int __case = (__start);__case <= __end;__case++){ \
                __write_output_file(__case); \
                __Func; \
                __close_file(); \
            } \
        } while(0)

        // make(generator) __num input files if files don't exist
        // use __Func
        #define fill_inputs(__num,__Func,...) do{ \
            int __sum = (__num); \
            for(int __case = (1);__sum;__case++) { \
                if(!__input_file_exists(__case)) { \
                    __sum--; \
                    __write_input_file(__case); \
                    __fake_arg(__VA_ARGS__); \
                    __Func; \
                    __close_file(); \
                } \
            } \
        }while(0)

        // make(generator) all output files use __Func
        #define fill_outputs(__Func) do{ \
            auto __inputs =  __get_inputs(); \
            for(auto __case:__inputs) { \
                __write_output_file(__case); \
                __Func; \
                __close_file(); \
            } \
        }while(0)

        // make(generator) input files from start to end
        // use ".\\data.exe"(default) ["./data" on Linux]
        void make_inputs_exe(int start,int end,const char* format = "",...){
            FMT_TO_RESULT(format,format,_format);
            std::string folder_path = testcases_folder_path._path;
            __create_directories(folder_path);
            for(int i = start;i <= end; i++){
                std::string in_file = std::to_string(i) + ".in";
                std::string file_path = __path_join(folder_path,in_file) ;
                std::string command = data_path._path + " " + _format + " >" + file_path;
                int return_code = std::system(command.c_str());
                if(return_code == 0) {
                    msg::__success_msg(msg::_err,"Successfully create input file %s",file_path.c_str());
                }
                else {
                    msg::__error_msg(msg::_err,"Someting error in creating input file %s",file_path.c_str());
                }   
            }
        }

        // make(generator) input files from start to end
        // use ".\\std.exe"(default) ["./std" on Linux]
        void make_outputs_exe(int start,int end){
            std::string folder_path = testcases_folder_path._path;
            __create_directories(folder_path);
            for(int i = start;i <= end; i++){
                std::string read_file = std::to_string(i) + ".in";
                std::string read_path = __path_join(folder_path, read_file);
                if(!__file_exists(read_path)) {
                    msg::__fail_msg(msg::_err,"Input file %s don't exist!",read_path.c_str());
                }
                std::string write_file = std::to_string(i) + ".out";
                std::string write_path = __path_join(folder_path, write_file);
                std::string command = std_path._path + " <" + read_path + " >" + write_path;
                int return_code = std::system(command.c_str());
                if(return_code == 0) {
                    msg::__success_msg(msg::_err,"Successfully create output file %s",write_path.c_str());
                }
                else {
                    msg::__error_msg(msg::_err,"Someting error in creating output file %s",write_path.c_str());
                }
            }
        }

        void fill_inputs_exe(int sum,const char* format = "",...){
            FMT_TO_RESULT(format,format,_format);
            std::string folder_path = testcases_folder_path._path;
            __create_directories(folder_path);
            for(int i = 1;sum; i++){
                std::string in_file = std::to_string(i) + ".in";
                std::string file_path = __path_join(folder_path,in_file) ;
                if(__file_exists(file_path)){
                    continue;
                }
                sum--;
                std::string command = data_path._path + " " + _format + " >" + file_path;
                int return_code = std::system(command.c_str());
                if(return_code == 0) {
                    msg::__success_msg(msg::_err,"Successfully create/open input file %s",file_path.c_str());
                }
                else {
                    msg::__error_msg(msg::_err,"Someting error in creating/opening input file %s",file_path.c_str());
                }   
            }
        }

        void fill_outputs_exe(){
            std::string folder_path = testcases_folder_path._path;
            __create_directories(folder_path);
            auto inputs =  __get_inputs();
            for(auto x:inputs){
                std::string read_file = std::to_string(x) + ".in";
                std::string read_path = __path_join(folder_path, read_file);
                std::string write_file = std::to_string(x) + ".out";
                std::string write_path = __path_join(folder_path, write_file);
                std::string command = std_path._path + " <" + read_path + " >" + write_path;
                int return_code = std::system(command.c_str());
                if(return_code == 0) {
                    msg::__success_msg(msg::_err,"Successfully create output file %s",write_path.c_str());
                }
                else {
                    msg::__error_msg(msg::_err,"Someting error in creating output file %s",write_path.c_str());
                }
            }
        }

        // equal to registerGen(argc, argv, 1);
        void init_gen(int argc,char* argv[]) {
            registerGen(argc, argv, 1);
        }

        // no argvs's register
        // unsafe but may easier to use
        void init_gen() {
            char * __fake_argvs[] = {"generator"};
            registerGen(1, __fake_argvs , 1);
        }

        void __run_program(int x,std::string& program,int time_limit,std::string& logv_file) {
            std::string input_file = std::to_string(x) + ".in";
            std::string output_file = std::to_string(x) + ".ans";
            std::string program_name = __get_file_name(program);
            std::string input_file_path = __path_join(testcases_folder_path._path, input_file);
            std::string output_file_path = __path_join(compare_folder_path._path, program_name);
            output_file_path = __path_join(output_file_path, output_file);
            std::string command = program;
            std::string lib_path = __get_lib_path();
            std::string judger_path = __path_join(__get_folder_path(lib_path),"checker");
            judger_path = __path_join(judger_path,"judger.py");
            std::string py_command = 
                "python " + 
                judger_path + " " +
                command + " " +
                std::to_string(x) + " " +
                input_file_path + " " +
                output_file_path + " " +
                logv_file + " " +
                std::to_string(time_limit);

            int result = system(py_command.c_str());
            
            if (result != 0) {
                msg::__fail_msg(msg::_err, "Fail in test %lld, program = %s.", x, program_name.c_str());
            }
        }

        enum ResultState{
            R_UNKNOWN,
            R_AC,
            R_WA,
            R_TLE,
            R_TLEANDAC,
            R_TLEANDWA,
            R_ERROR
        };

        void __compare_once(int start,int end,std::string& program,int time_limit,std::string& checker){
            std::string program_name = __get_file_name(program);
            std::string logv_file = __path_join(compare_folder_path._path,program_name + std::string(".logv"));
            std::string ans_folder_path = __path_join(compare_folder_path._path,program_name);
            __create_directories(ans_folder_path);
            for(int i = start;i <= end; i++){
                __run_program(i,program,time_limit * 2,logv_file);
            }
            msg::OutStream log_file(logv_file.substr(0,logv_file.length() - 1));
            std::vector<ResultState> result(end - start + 1,R_UNKNOWN);
            std::vector<int> runtime(end - start + 1, -1);
            std::vector<std::string> testlib_result(end - start + 1);
            std::ifstream read_logv(logv_file);

            for(int i = start; i <= end ;i++){
                int x,t;
                read_logv >> x >> t;
                x -= start;
                runtime[x] = t;
                if(t == -1){
                    result[x] = R_ERROR;
                }
                else if(t > 1.5 * time_limit) {
                    result[x] = R_TLE;
                }
                else{
                    std::string input_file = std::to_string(i) + ".in";
                    std::string output_file = std::to_string(i) + ".out";
                    std::string ans_file = std::to_string(i) + ".ans";
                    std::string input_file_path = __path_join(testcases_folder_path._path, input_file);
                    std::string output_file_path = __path_join(testcases_folder_path._path, output_file);
                    std::string ans_file_path = __path_join(ans_folder_path, ans_file);
                    std::string check_tmp_log_path = __path_join(ans_folder_path, "__checker.logc");
                    std::string command = 
                        checker + " " +
                        input_file_path + " " +
                        output_file_path + " " +
                        ans_file_path + " 2>" +
                        check_tmp_log_path;
                    int command_result = system(command.c_str());
                    std::string check_result = "";
                    std::ifstream check_stream(check_tmp_log_path);
                    std::string line;
                    while(check_stream >> line){
                        check_result += line;
                        check_result += " ";
                    }
                    check_stream.close();
                    if(check_result.substr(0,2) == "ok"){
                        if(t > time_limit){
                            result[x] = R_TLEANDAC;
                        }
                        else{
                            result[x] = R_AC;
                        }
                    }
                    else{
                        if(t > time_limit){
                            result[x] = R_TLEANDWA;
                        }
                        else{
                            result[x] = R_WA;
                        }
                        testlib_result[x] = check_result;
                    }
                }
            }
            read_logv.close();
            std::remove(logv_file.c_str());
            std::remove(__path_join(ans_folder_path, "__checker.logc").c_str());
            msg::__info_msg(msg::_err,"test for program %s :",program.c_str());
            int number_ac = 0, number_wa = 0, number_tle = 0,number_err = 0;
            for (int i = start; i <= end; i++){
                int index = i - start;
                if(result[index] == R_UNKNOWN || result[index] == R_ERROR){
                    msg::__run_err_msg(log_file,false,i);
                    msg::__run_err_msg(msg::_err,true,i);
                    number_err ++;
                }
                if(result[index] == R_AC){
                    msg::__ac_msg(log_file,false,i,runtime[index]);
                    msg::__ac_msg(msg::_err,true,i,runtime[index]);
                    number_ac ++;
                }
                if(result[index] == R_WA){
                    msg::__wa_msg(log_file,false,i,runtime[index],testlib_result[index]);
                    msg::__wa_msg(msg::_err,true,i,runtime[index],testlib_result[index]);
                    number_wa ++;
                }
                if(result[index] == R_TLEANDAC){
                    msg::__tleac_msg(log_file,false,i,runtime[index]);
                    msg::__tleac_msg(msg::_err,true,i,runtime[index]);
                    number_tle ++;
                }
                if(result[index] == R_TLEANDWA){
                    msg::__tlewa_msg(log_file,false,i,runtime[index],testlib_result[index]);
                    msg::__tlewa_msg(msg::_err,true,i,runtime[index],testlib_result[index]);
                    number_tle ++;
                }
                if(result[index] == R_TLE){
                    msg::__tle_msg(log_file,false,i,runtime[index]);
                    msg::__tle_msg(msg::_err,true,i,runtime[index]);
                    number_tle ++;
                }
            }
            msg::__info_msg(msg::_err,"program test done.");
            msg::__info_msg(msg::_err,"\033[32mAC\033[0m :%d/%d",number_ac,end - start + 1);
            msg::__info_msg(msg::_err,"\033[1;31mWA\033[0m :%d/%d",number_wa,end - start + 1);
            msg::__info_msg(msg::_err,"\033[1;33mTLE\033[0m :%d/%d",number_tle,end - start + 1);
            msg::__info_msg(msg::_err,"\033[1;31mERROR\033[0m :%d/%d",number_err,end - start + 1);
            msg::__info_msg(msg::_err,"\n");
            return;
        }

        std::vector<std::string> __get_checker_files() {
            return std::vector<std::string>();
        }

        template<typename T, typename... Args>
        std::vector<std::string> __get_checker_files(T first, Args... args) {       
            std::vector<std::string> result = {};
            std::string first_arg = std::string(first);
            std::string file = __get_full_path(first_arg);
            if(__file_exists(file)){
                result.emplace_back(file);
            }
            else{
                msg::__warn_msg(
                    msg::_err,
                    "Compare program file %s doesn't exist, full path is %s.",
                    std::string(first).c_str(),
                    file.c_str());
            }
            std::vector<std::string> rest = __get_checker_files(args...);
            result.insert(result.end(), rest.begin(), rest.end());
            
            return result;
        }

        // run program args_1,args_2,...,args_n from case start to end
        // set time limit, 2 * time_limit for run , 1.5 * time_limit for check
        // checker is the checker.exe path(writen by testlib)
        template<typename... Args>
        void compare(int start,int end,int time_limit,std::string checker,Args ... args){
            init_gen();
            std::string checker_file = __get_full_path(checker);
            if(!__file_exists(checker_file)){
                msg::__fail_msg(msg::_err,"Checker file %s doesn't exist, full path is %s.",checker.c_str(),checker_file.c_str());
            }
            std::vector<std::string> programs = __get_checker_files(args...);
            for(auto program:programs){
                __compare_once(start,end,program,time_limit,checker_file);
            }
        }

        enum Checker{
            lcmp,
            yesno,
            rcmp4,
            rcmp6,
            rcmp9,
            wcmp
        };

        // run program args_1,args_2,...,args_n from case start to end
        // set time limit, 2 * time_limit for run , 1.5 * time_limit for check
        // checker is the default checker
        template<typename... Args>
        void compare(int start,int end,int time_limit,Checker checker,Args ... args){
            init_gen();
            std::string lib_path = __get_lib_path();
            std::string checker_folder = __path_join(__get_folder_path(lib_path) ,"checker");
            std::string checker_file;
            if(checker == lcmp){
                checker_file = __path_join(checker_folder,"lcmp.exe");
            }
            else if(checker == yesno){
                checker_file = __path_join(checker_folder,"yesno.exe"); 
            }
            else if(checker == rcmp4){
                checker_file = __path_join(checker_folder,"rcmp4.exe"); 
            }
            else if(checker == rcmp6){
                checker_file = __path_join(checker_folder,"rcmp6.exe"); 
            }
            else if(checker == rcmp9){
                checker_file = __path_join(checker_folder,"rcmp9.exe"); 
            }
            else if(checker == wcmp){
                checker_file = __path_join(checker_folder,"wcmp.exe"); 
            }
            if(!__file_exists(checker_file)){
                msg::__fail_msg(msg::_err,"Checker file %s doesn't exist, some thing may wrong!",checker_file.c_str());
            }
            std::vector<std::string> programs = __get_checker_files(args...);
            for(auto program:programs){
                __compare_once(start,end,program,time_limit,checker_file);
            }
        }
    }

    namespace all{
        using namespace generator::msg;
        using namespace generator::rand;
        using namespace generator::io;
    }
}
