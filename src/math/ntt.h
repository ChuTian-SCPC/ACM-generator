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

    } // namespace math

} // namespace generator

#endif // !_SGPCET_NTT_H_
