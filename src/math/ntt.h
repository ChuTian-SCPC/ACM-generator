#ifndef _SGPCET_NTT_H_
#define _SGPCET_NTT_H_

#ifndef _SGPCET_SETTING_H_
#include "basic/setting.h"
#endif //!_SGPCET_SETTING_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif //!_SGPCET_LOGGER_H_

namespace generator {
    namespace math {
        template <typename T = long long, T MOD = 998244353, T G = 3>
        class NTT {
        public:
            static T qpow(T a, T b) {
                T res = 1;
                while (b) {
                    if (b & 1) res = (res * a) % MOD;
                    a = (a * a) % MOD;
                    b >>= 1;
                }
                return res;
            }
        
            static T inv(T a) {
                return qpow(a, MOD - 2);
            }
        
            static void bit_reverse(std::vector<T>& a) {
                int n = a.size();
                for (int i = 1, j = 0; i < n; i++) {
                    int bit = n >> 1;
                    for (; j & bit; bit >>= 1)
                        j ^= bit;
                    j ^= bit;
                    if (i < j)
                        std::swap(a[i], a[j]);
                }
            }
        
            static void transform(std::vector<T>& a, bool inverse = false) {
                int n = a.size();
                bit_reverse(a);
        
                for (int len = 2; len <= n; len <<= 1) {
                    T wn = qpow(G, (MOD - 1) / len);
                    if (inverse) wn = inv(wn);
        
                    for (int i = 0; i < n; i += len) {
                        T w = 1;
                        for (int j = 0; j < len / 2; j++) {
                            T u = a[i + j];
                            T v = (w * a[i + j + len / 2]) % MOD;
                            a[i + j] = (u + v) % MOD;
                            a[i + j + len / 2] = (u - v + MOD) % MOD;
                            w = (w * wn) % MOD;
                        }
                    }
                }
        
                if (inverse) {
                    T inv_n = inv(n);
                    for (T& x : a)
                        x = (x * inv_n) % MOD;
                }
            }
        
            static std::vector<T> multiply(const std::vector<T>& a, const std::vector<T>& b) {
                std::vector<T> fa(a.begin(), a.end()), fb(b.begin(), b.end());
                int n = 1;
                while (n < a.size() + b.size())
                    n <<= 1;
        
                fa.resize(n);
                fb.resize(n);
        
                transform(fa, false);
                transform(fb, false);
        
                for (int i = 0; i < n; i++)
                    fa[i] = (fa[i] * fb[i]) % MOD;
        
                transform(fa, true);
                return fa;
            }
        };
        
        template <typename T = long long, T BASE = 10, T MOD1 = 998244353, T G1 = 3, T MOD2 = 985661441, T G2 = 3>
        class CrtMultiplier {
        private:
            // 预计算的CRT参数
            static constexpr T crt_mod = MOD1 * MOD2;
            static T m1_inv_m2; // MOD1在MOD2下的逆元
            
            class NTT1 : public NTT<T, MOD1, G1> {};
            class NTT2 : public NTT<T, MOD2, G2> {};
        public:
            static std::vector<T> multiply(const std::vector<T>& a, const std::vector<T>& b) {
                auto fa1 = NTT1::multiply(a, b);
                auto fa2 = NTT2::multiply(a, b);
                if (fa1.size() != fa2.size()) {
                    _msg::__error_msg(_msg::_defl, "NTT1 and NTT2 result size not equal, NTT1 size: %d, NTT2 size: %d", fa1.size(), fa2.size());
                }
                int n = fa1.size();
                std::vector<T> res(n);
                for (int i = 0; i < n; i++) {
                    res[i] = crt(fa1[i], fa2[i]);
                }
                
                // 处理进位并规范化
                return normalize(res);
            }
        private:
            // 初始化CRT参数
            static void init_crt() {
                if (m1_inv_m2 == 0) {
                    T x, y;
                    exgcd(MOD1, MOD2, x, y);
                    m1_inv_m2 = (x % MOD2 + MOD2) % MOD2;
                }
            }

            // 扩展欧几里得算法
            static void exgcd(T a, T b, T& x, T& y) {
                if (b == 0) {
                    x = 1;
                    y = 0;
                    return;
                }
                exgcd(b, a % b, y, x);
                y -= a / b * x;
            }

            // 中国剩余定理
            static T crt(T r1, T r2) {
                init_crt();
                T diff = (r2 - r1) % MOD2;
                if (diff < 0) diff += MOD2;
                T k = (diff * m1_inv_m2) % MOD2;
                return (r1 + k * MOD1) % crt_mod;
            }

            // 规范化结果
            static std::vector<T> normalize(std::vector<T>& num) {
                std::vector<T> res;
                T carry = 0;
                for (T x : num) {
                    x += carry;
                    res.push_back(x % BASE);
                    carry = x / BASE;
                }
                while (carry > 0) {
                    res.push_back(carry % BASE);
                    carry /= BASE;
                }
                while (res.size() > 1 && res.back() == 0) {
                    res.pop_back();
                }
                return res;
            }
        };
        template <typename T, T BASE, T MOD1, T G1, T MOD2, T G2>
        T CrtMultiplier<T, BASE, MOD1, G1, MOD2, G2>::m1_inv_m2 = 0;
    } // namespace math

} // namespace generator

#endif // !_SGPCET_NTT_H_
