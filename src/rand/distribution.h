#ifndef _SGPCET_DISTRIBUTION_H_
#define _SGPCET_DISTRIBUTION_H_

#ifndef _SGPCET_NUMERIC_H_
#include "numeric.h"
#endif // !_SGPCET_NUMERIC_H_

namespace generator {
    namespace rand_numeric {
        template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
        class NormalDistribution {
            private:
            T mean_, stddev_;
            T spare_;
            bool has_spare_;
            
        public:
            explicit NormalDistribution(T mean = 0, T stddev = 1) 
                : mean_(mean), stddev_(stddev), has_spare_(false) {}
            
            T rand() {
                if (has_spare_) {
                    has_spare_ = false;
                    return mean_ + stddev_ * spare_;
                }
                
                T u1 = rand_numeric::rand_real<T>();
                T u2 = rand_numeric::rand_real<T>();
                
                u1 = std::max(u1, std::numeric_limits<T>::epsilon());
                u2 = std::max(u2, std::numeric_limits<T>::epsilon());
                
                T z0 = std::sqrt(-2 * std::log(u1)) * std::cos(2 * _setting::PI * u2);
                T z1 = std::sqrt(-2 * std::log(u1)) * std::sin(2 * _setting::PI * u2);
                
                spare_ = z1;
                has_spare_ = true;
                
                return mean_ + stddev_ * z0;
            }
        };

        template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
        class LogNormalDistribution {
        private:
            T mu_, sigma_;
            NormalDistribution<T> normal_dist_;
            
        public:
            explicit LogNormalDistribution(T mu = 0, T sigma = 1) 
                : mu_(mu), sigma_(sigma), normal_dist_(mu, sigma) {}
            
            T rand() {
                T u = normal_dist_.rand();
                while (u < 0) u = normal_dist_.rand();
                return std::exp(u);
            }
        };
    }
}

#endif // _SGPCET_DISTRIBUTION_H_
