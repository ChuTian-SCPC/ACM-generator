#pragma once
#include "test_basic.hpp"
using namespace generator::all;

template<typename T>
class NormalDistributionValidator {
private:
    std::vector<T> samples_;
    
public:
    void collect_samples(NormalDistribution<T>& dist, int num_samples = 10000) {
        samples_.clear();
        samples_.reserve(num_samples);
        for (int i = 0; i < num_samples; ++i) {
            samples_.push_back(dist.rand());
        }
    }
    
    // 计算样本均值
    double mean() const {
        return std::accumulate(samples_.begin(), samples_.end(), 0.0) / samples_.size();
    }
    
    // 计算样本方差
    double variance() const {
        double m = mean();
        double var = 0.0;
        for (T x : samples_) {
            var += (x - m) * (x - m);
        }
        return var / (samples_.size() - 1);
    }
    
    // 计算样本标准差
    double stddev() const {
        return std::sqrt(variance());
    }
    
    // 计算偏度（三阶矩）
    double skewness() const {
        double m = mean();
        double s = stddev();
        double sum = 0.0;
        for (T x : samples_) {
            double z = (x - m) / s;
            sum += z * z * z;
        }
        return sum / samples_.size();
    }
    
    // 计算峰度（四阶矩）
    double kurtosis() const {
        double m = mean();
        double s = stddev();
        double sum = 0.0;
        for (T x : samples_) {
            double z = (x - m) / s;
            sum += z * z * z * z;
        }
        return sum / samples_.size() - 3.0; // 超额峰度
    }
    
    // 验证统计量
    void validate_statistics(double expected_mean, double expected_stddev, double tolerance = 0.1) {
        double sample_mean = mean();
        double sample_stddev = stddev();
        double sample_skewness = skewness();
        double sample_kurtosis = kurtosis();
        
        CHECK_THAT(sample_mean, Catch::Matchers::WithinAbs(expected_mean, tolerance * expected_stddev));
        CHECK_THAT(sample_stddev, Catch::Matchers::WithinAbs(expected_stddev, tolerance * expected_stddev));
        CHECK_THAT(sample_skewness, Catch::Matchers::WithinAbs(0.0, tolerance * 2));
        CHECK_THAT(sample_kurtosis, Catch::Matchers::WithinAbs(0.0, tolerance * 2));
    }
};

TEST_CASE("NormalDistribution", "[distribution][NormalDistribution]") {
    init_gen();
    NormalDistribution<double> dist1(0.0, 1.0);
    NormalDistributionValidator<double> validator1;
    validator1.collect_samples(dist1);
    validator1.validate_statistics(0.0, 1.0);

    loop_check([]() {
        double mean = rand_real("[0.000000, 10]");
        double stddev = rand_real("(0.000000, 10]");
        NormalDistribution<double> dist(mean, stddev);
        NormalDistributionValidator<double> validator;
        validator.collect_samples(dist);
        validator.validate_statistics(mean, stddev);
        return true;
    }, 10);
}