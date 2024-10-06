#include <iostream>
#include <map>
#include <random>
#include <chrono>
#include <vector>
#include <cmath>
#include <concepts>
#include <ranges>
#include <bit>

template<std::unsigned_integral T>
struct NumTheory {
private:
    std::mt19937_64 rng;

    T binpower(T base, T e, T mod) {
        T result = 1;
        base %= mod;
        while (e) {
            if (e & 1)
                result = (__uint128_t)result * base % mod;
            base = (__uint128_t)base * base % mod;
            e >>= 1;
        }
        return result;
    }

    bool check_composite(T n, T a, T d, int s) {
        T x = binpower(a, d, n);
        if (x == 1 || x == n - 1)
            return false;
        for (int r = 1; r < s; r++) {
            x = (__uint128_t)x * x % n;
            if (x == n - 1)
                return false;
        }
        return true;
    }

    T pollard_rho(T n, int max_iterations = 1'000'000) {
        if (n % 2 == 0)
            return 2;

        auto dist = std::uniform_int_distribution<T>{2, n - 1};
        T x = dist(rng);
        T y = x;
        T c = dist(rng);
        T d = 1;

        for (int i = 0; i < max_iterations && d == 1; ++i) {
            x = ((__uint128_t)x * x + c) % n;
            y = ((__uint128_t)y * y + c) % n;
            y = ((__uint128_t)y * y + c) % n;
            d = std::gcd(std::abs((long long)x - (long long)y), (long long)n);
        }

        return d;
    }

    T trial_division(T n) const {
        for (T i = 2; i * i <= n; ++i) {
            if (n % i == 0) return i;
        }
        return n;
    }

public:
    NumTheory() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

    bool is_prime(T n) {
        if (n < 2)
            return false;
        if (n < 4)
            return true;
        if (n % 2 == 0 || n % 3 == 0)
            return false;

        int r = std::countr_zero(n - 1);
        T d = (n - 1) >> r;

        constexpr std::array witnesses = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
        for (T a : witnesses | std::views::take_while([n](T x) { return x < n; })) {
            if (check_composite(n, a, d, r))
                return false;
        }
        return true;
    }

    std::map<T, T> factorize(T n) {
        std::map<T, T> factors;

        for (T i = 2; i * i <= n && i <= 1'000'000; ++i) {
            while (n % i == 0) {
                factors[i]++;
                n /= i;
            }
        }

        while (n > 1) {
            if (is_prime(n)) {
                factors[n]++;
                break;
            }
            T factor = pollard_rho(n);
            if (factor == n) {
                factor = trial_division(n);
            }
            factors[factor]++;
            n /= factor;
        }

        return factors;
    }

    T euler_phi(T n) {
        auto factors = factorize(n);
        __uint128_t result = n;
        for (const auto& [prime, count] : factors) {
            result *= (prime - 1);
            result /= prime;
        }
        return static_cast<T>(result);
    }

    T count_primes(T n) {
        if (n <= 1) return 0;
        if (n == 2) return 1;
        const T sq = std::sqrt(n);
        T s = (1 + sq) >> 1;
        std::vector<T> smalls(s), roughs(s), larges(s);

        for (T i = 0; i < s; i++) {
            if (i) smalls[i] = i;
            roughs[i] = i << 1 | 1;
            larges[i] = (n / (i << 1 | 1) - 1) >> 1;
        }

        std::vector<bool> skip(sq + 1);
        const auto divide = [](T n, T d) -> T { return static_cast<T>((long double)n / d); };
        const auto half = [](T n) -> T { return (n - 1) >> 1; };

        T pc = 0;
        for (T p = 3; p <= sq; p += 2) {
            if (skip[p]) continue;
            T q = p * p;
            if (q * q > n) break;
            skip[p] = true;
            for (T i = q; i <= sq; i += p << 1) skip[i] = true;
            T ns = 0;
            for (T k = 0; k < s; k++) {
                T i = roughs[k];
                if (skip[i]) continue;
                T d = i * p;
                larges[ns] = larges[k] - (d <= sq ? larges[smalls[d >> 1] - pc] : smalls[half(divide(n, d))]) + pc;
                roughs[ns++] = i;
            }
            s = ns;
            for (T i = half(sq), j = ((sq / p) - 1) | 1; j >= p; j -= 2) {
                T c = smalls[j >> 1] - pc;
                for (T e = (j * p) >> 1; i >= e; --i) smalls[i] -= c;
            }
            pc++;
        }

        larges[0] += (s + 2 * (pc - 1)) * (s - 1) / 2;
        for (T k = 1; k < s; k++) larges[0] -= larges[k];
        for (T l = 1; l < s; l++) {
            T q = roughs[l]; T m = n / q;
            T e = smalls[half(m / q)] - pc;
            if (e < l + 1) break;
            T t = 0;
            for (T k = l + 1; k <= e; k++) t += smalls[half(divide(m, roughs[k]))];
            larges[0] += t - (e - l) * (pc + l - 1);
        }
        return larges[0] + 1;
    }

    T find_primitive_root(T p) {
        if (p < 2) return -1;
        if (p == 2) return 1;
        if (!is_prime(p)) return -1;

        T phi = p - 1;
        auto factors = factorize(phi);

        for (T r = 2; r < p; ++r) {
            bool is_primitive = true;
            for (const auto& [prime_factor, _] : factors) {
                if (binpower(r, phi / prime_factor, p) == 1) {
                    is_primitive = false;
                    break;
                }
            }
            if (is_primitive) return r;
        }

        return -1;
    }

    T tetration(T a, T b, T m) {
        if (m == 1) return 0;
        if (a == 0) return (b & 1) ? 0 : 1;
        if (b == 0) return 1;
        if (b == 1) return a % m;

        bool use_phi = (b > 2) && !((a <= 9 && b == 3) || (a == 2 && b <= 5));

        if (use_phi) {
            T phi_m = euler_phi(m);
            return binpower(a, phi_m + tetration(a, b - 1, phi_m), m);
        } else {
            return binpower(a, tetration(a, b - 1, T(1e9)), m);
        }
    }
};


int main() {

    NumTheory<uint64_t> nt;

    // // https://judge.yosupo.jp/problem/primality_test
    // int Q;
    // std::cin >> Q;

    // while (Q--) {
    //     uint64_t N;
    //     std::cin >> N;
    //     std::cout << (nt.is_prime(N) ? "Yes" : "No") << std::endl;
    // }


    // // https://judge.yosupo.jp/problem/factorize
    // int Q;
    // std::cin >> Q;

    // while (Q--) {
    //     uint64_t a;
    //     std::cin >> a;

    //     auto mp = nt.factorize(a);

    //     uint64_t num = 0;
    //     for (auto& [p, n] : mp) {
    //         num += n;
    //     }
    //     std::cout << num << ' ';

    //     for (auto& [p, n] : mp) {
    //         for (int i = 0; i < n; i++) {
    //             std::cout << p << ' ';
    //         }
    //     }
    //     std::cout << std::endl;
    // }


    // //https://judge.yosupo.jp/problem/counting_primes
    // uint64_t N;
    // std::cin >> N;

    // std::cout << nt.count_primes(N) << std::endl;


    // // https://judge.yosupo.jp/problem/primitive_root
    // int Q;
    // std::cin >> Q;

    // while (Q--) {
    //     uint64_t p;
    //     std::cin >> p;
    //     std::cout << nt.find_primitive_root(p) << std::endl;
    // }

    // https://judge.yosupo.jp/problem/tetration_mod
    int T;
    std::cin >> T;

    while (T--) {
        uint64_t A, B, M;
        std::cin >> A >> B >> M;
        std::cout << nt.tetration(A, B, M) << std::endl;
    }

    return 0;
}
