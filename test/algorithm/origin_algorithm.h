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
}