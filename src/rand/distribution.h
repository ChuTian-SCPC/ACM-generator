#ifndef _SGPCET_DISTRIBUTION_H_
#define _SGPCET_DISTRIBUTION_H_

#ifndef _SGPCET_NUMERIC_H_
#include "numeric.h"
#endif // !_SGPCET_NUMERIC_H_

namespace generator {
    namespace rand_numeric {
        template<typename T = double>
        class NormalDistribution {
            private:
            T mean_, stddev_;
            T spare_;
            bool has_spare_;
            
        public:
            explicit NormalDistribution(T mean = 0, T stddev = 1) 
                : mean_(mean), stddev_(stddev), has_spare_(false) {}
            
            T rand() {
                return static_cast<T>(__rand());
            }

            T rand(T from, T to) {
                T x;
                do {
                    x = rand();
                } while (x < from || x > to);
                return x;
            }

        private:
            double __rand() {
                if (has_spare_) {
                    has_spare_ = false;
                    return mean_ + stddev_ * spare_;
                }
                
                double u1 = rand_numeric::rand_real<double>();
                double u2 = rand_numeric::rand_real<double>();
                
                u1 = std::max(u1, std::numeric_limits<double>::epsilon());
                u2 = std::max(u2, std::numeric_limits<double>::epsilon());
                
                double z0 = std::sqrt(-2 * std::log(u1)) * std::cos(2 * _setting::PI * u2);
                double z1 = std::sqrt(-2 * std::log(u1)) * std::sin(2 * _setting::PI * u2);
                
                spare_ = z1;
                has_spare_ = true;
                
                return mean_ + stddev_ * z0;               
            }
        };

        template<typename T = double>
        class LogNormalDistribution {
        private:
            T mu_, sigma_;
            NormalDistribution<T> normal_dist_;
            
        public:
            explicit LogNormalDistribution(T mu = 0, T sigma = 1) 
                : mu_(mu), sigma_(sigma), normal_dist_(mu, sigma) {}
            
            T rand() {
                return std::exp(normal_dist_.rand());
            }

            T rand(T from, T to) {
                T x;
                do {
                    x = rand();
                } while (x < from || x > to);
                return x;
            }
        };
    }
}

#endif // _SGPCET_DISTRIBUTION_H_
