#pragma once
#include "test_basic.hpp"
using namespace generator::all;

template<typename T>
class NormalDistributionValidator {
private:
    std::vector<T> samples_;
    
public:
    void collect_samples(NormalDistribution<T>& dist, int num_samples = 1000000) {
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

template<typename T>
class LogNormalDistributionValidator {
private:
    std::vector<T> samples_;
    std::vector<T> log_samples_; // 存储样本的对数值
    
public:
    void collect_samples(LogNormalDistribution<T>& dist, int num_samples = 1000000) {
        samples_.clear();
        log_samples_.clear();
        samples_.reserve(num_samples);
        log_samples_.reserve(num_samples);
        
        for (int i = 0; i < num_samples; ++i) {
            T sample = dist.rand();
            samples_.push_back(sample);
            log_samples_.push_back(std::log(sample));
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
    
    // 验证对数变换后的样本是否服从正态分布
    void validate_log_normality(double mu, double sigma, double tolerance = 0.1) {
        // 计算对数样本的均值和标准差
        double log_mean = std::accumulate(log_samples_.begin(), log_samples_.end(), 0.0) / log_samples_.size();
        double log_var = 0.0;
        for (T x : log_samples_) {
            log_var += (x - log_mean) * (x - log_mean);
        }
        log_var /= (log_samples_.size() - 1);
        double log_stddev = std::sqrt(log_var);
        CHECK_THAT(log_mean, Catch::Matchers::WithinAbs(mu, tolerance * sigma));
        CHECK_THAT(log_stddev, Catch::Matchers::WithinAbs(sigma, tolerance * sigma));

    }
    
    // 验证原始样本的统计量是否符合对数正态分布的理论值
    void validate_statistics(double mu, double sigma, double tolerance = 0.1) {
        // 计算理论值
        double expected_mean = std::exp(mu + sigma * sigma / 2.0);
        double expected_variance = (std::exp(sigma * sigma) - 1.0) * std::exp(2.0 * mu + sigma * sigma);
        double expected_stddev = std::sqrt(expected_variance);
        
        // 计算样本统计量
        double sample_mean = mean();
        double sample_stddev = stddev();
        
        CHECK_THAT(sample_mean, Catch::Matchers::WithinAbs(expected_mean, tolerance * expected_mean));
        CHECK_THAT(sample_stddev, Catch::Matchers::WithinAbs(expected_stddev, tolerance * expected_stddev));
    }
    
    // 综合验证
    void validate(double mu, double sigma, double tolerance = 0.1) {
        validate_log_normality(mu, sigma, tolerance);
        validate_statistics(mu, sigma, tolerance);
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

TEST_CASE("LogNormalDistribution", "[distribution][LogNormalDistribution]") {
    init_gen();
    LogNormalDistribution<double> dist1(0.0, 1.0);
    LogNormalDistributionValidator<double> validator1;
    validator1.collect_samples(dist1);
    validator1.validate(0.0, 1.0);

    loop_check([]() {
        double mu = rand_real("[0.000000, 2]");
        double sigma = rand_real("(0.000000, 2]");
        LogNormalDistribution<double> dist(mu, sigma);
        LogNormalDistributionValidator<double> validator;
        validator.collect_samples(dist);
        validator.validate(mu, sigma, 0.2);
        return true;
    }, 10);
}