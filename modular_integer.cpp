#include <iostream>
#include <utility>
#include <random>

constexpr int MOD1 = 1000000007;
constexpr int MOD2 = 998244353;

template <const int &mod>
struct ModInt {
private:
	int x;

public:
	ModInt() : x(0) {}
	ModInt(long long x_) { if ((x = x_ % mod + mod) >= mod) x -= mod; }

	constexpr ModInt& operator+=(ModInt rhs) { if ((x += rhs.x) >= mod) x -= mod; return *this; }
	constexpr ModInt& operator-=(ModInt rhs) { if ((x -= rhs.x) < 0) x += mod; return *this; }
	constexpr ModInt& operator*=(ModInt rhs) { x = (unsigned long long)x * rhs.x % mod; return *this; }
	constexpr ModInt& operator/=(ModInt rhs) { x = (unsigned long long)x * rhs.inv().x % mod; return *this; }

	constexpr ModInt operator-() const noexcept { return -x < 0 ? mod - x : -x; }
	constexpr ModInt operator+(ModInt rhs) const noexcept { return ModInt(*this) += rhs; }
	constexpr ModInt operator-(ModInt rhs) const noexcept { return ModInt(*this) -= rhs; }
	constexpr ModInt operator*(ModInt rhs) const noexcept { return ModInt(*this) *= rhs; }
	constexpr ModInt operator/(ModInt rhs) const noexcept { return ModInt(*this) /= rhs; }

	constexpr ModInt& operator++() { *this += 1; return *this; }
	constexpr ModInt operator++(int) { *this += 1; return *this - 1; }
	constexpr ModInt& operator--() { *this -= 1; return *this; }
	constexpr ModInt operator--(int) { *this -= 1;return *this + 1; }

	bool operator==(ModInt rhs) const { return x == rhs.x; }
	bool operator!=(ModInt rhs) const { return x != rhs.x; }
	bool operator<=(ModInt rhs) const { return x <= rhs.x; }
	bool operator>=(ModInt rhs) const { return x >= rhs.x; }
	bool operator<(ModInt rhs) const { return x < rhs.x; }
	bool operator>(ModInt rhs) const { return x > rhs.x; }

    friend std::istream& operator>>(std::istream& s, ModInt<mod>& a) { s >> a.x; return s; }
    friend std::ostream& operator<<(std::ostream& s, ModInt<mod> a) { s << a.x; return s; }

    ModInt inv() {
		int a = x, b = mod, u = 1, v = 0, t;
		while (b > 0) { t = a / b, std::swap(a -= t * b, b), std::swap(u -= t * v, v); }
		return ModInt(u);
	}

    ModInt pow(long long n) const {
		ModInt ret(1), mul(x);
		while (n > 0) { if (n & 1) ret *= mul; mul *= mul; n >>= 1; }
		return ret;
	}

	int sqrt() const {
		if (x <= 1) return x;
		if (pow((mod - 1) / 2) != 1) return -1;
		int q = mod - 1, m = 0;
        std::mt19937 mt; ModInt z = mt();
		while (~q & 1) q >>= 1, m++;
		while (z.pow((mod - 1) / 2) != mod - 1) z = mt();
		ModInt c = z.pow(q), t = pow(q), r = pow((q + 1) / 2);
		for (; m > 1; m--) {
			ModInt tmp = t.pow(1 << (m - 2));
            if (tmp != 1) r = r * c, t = t * c * c;
			c = c * c;
		}
		return std::min(r.x, mod - r.x);
	}

};

static int MOD = MOD1;
using mint = ModInt<MOD>;


// https://judge.yosupo.jp/problem/sqrt_mod
int main() {
    int T;
    std::cin >> T;

    mint Y;
    while (T--) {
        std::cin >> Y >> MOD;
        std::cout << Y.sqrt() << std::endl;
    }
    return 0;
}
