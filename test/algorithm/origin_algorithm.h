#include "../generator.h"

namespace _origin {
    using namespace generator;
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
                _msg::__fail_msg(_msg::_defl, "value can't less than 0, but found (key : ", iter.first,
                    ", value : ", iter.second, ").");
            }
            sum += value;
            probs.emplace_back(sum);
        }
        if (sum <= 0) {
            _msg::__fail_msg(_msg::_defl, 
                tools::string_format("sum of the values must greater than 0, but found %lld.", sum));
        }
        long long p = rand_int(1LL,sum);
        auto pos = lower_bound(probs.begin(),probs.end(),p) - probs.begin();
        return *(elements.begin() + pos);
    }

    void __rand_bracket_open(std::string& res, std::string& open, std::stack<int>& st, int& limit) {
        limit--;
        int pos = rnd.next(open.size());
        st.push(pos);
        res += open[pos];
    }
    
    void __rand_bracket_close(std::string& res, std::string& close, std::stack<int>& st) {
        int pos = st.top();
        st.pop();
        res += close[pos];
    }
    
    std::string rand_bracket_seq(int len, std::string brackets = "()") {
        rand_array::__judge_vector_lower_bound(len, "string");
        rand_array::__judge_vector_upper_bound(len, "string");
        if (len < 0 || len % 2) {
            _msg::__fail_msg(_msg::_defl, 
                tools::string_format("length must be positive even number, but found %d.", len));
        }
        std::stack<int> st;
        std::string open = "";
        std::string close = "";
        int n = brackets.size();
        if (n == 0 || n %2) {
            _msg::__fail_msg(_msg::_defl, 
                tools::string_format("bracket must appear in pairs and the length must be greater than 0, but found %d.", n));
        }
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) open += brackets[i];
            else close += brackets[i];
        }
        std::string res = "";
        int limit = len / 2;
        while(limit) {
            if (st.empty()) __rand_bracket_open(res, open, st, limit);
            else if(rand_numeric::rand_bool()) __rand_bracket_open(res, open, st, limit);
            else __rand_bracket_close(res, close, st);
        };
        while(!st.empty())  __rand_bracket_close(res, close, st);
        return res;
    }
}