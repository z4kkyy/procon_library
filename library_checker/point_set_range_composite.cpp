#include <iostream>
#include <concepts>
#include <vector>
#include <span>
#include <utility>
#include <random>

constexpr int MOD1 = 1000000007;
constexpr int MOD2 = 998244353;

template <const int& mod>
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
	constexpr ModInt operator--(int) { *this -= 1; return *this + 1; }

	bool operator==(ModInt rhs) const { return x == rhs.x; }
	bool operator!=(ModInt rhs) const { return x != rhs.x; }
	bool operator<=(ModInt rhs) const { return x <= rhs.x; }
	bool operator>=(ModInt rhs) const { return x >= rhs.x; }
	bool operator<(ModInt rhs) const { return x < rhs.x; }
	bool operator>(ModInt rhs) const { return x > rhs.x; }

    friend std::istream& operator>>(std::istream& s, ModInt<mod>& a) { s >> a.x; return s; }
    friend std::ostream& operator<<(std::ostream& s, ModInt<mod> a) { s << a.x; return s; }

	int value() {
		return this->x;
	}

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
        std::mt19937 mt;
		ModInt z = mt();

		while (~q & 1) q >>= 1, m++;
		while (z.pow((mod - 1) / 2) != mod - 1) z = mt();

		ModInt c = z.pow(q);
		ModInt t = pow(q);
		ModInt r = pow((q + 1) / 2);

		for (; m > 1; m--) {
			ModInt tmp = t.pow(1 << (m - 2));
            if (tmp != 1) r = r * c, t = t * c * c;
			c = c * c;
		}

		return std::min(r.x, mod - r.x);
	}
};

static int MOD = MOD2;
using mint = ModInt<MOD>;

template<typename T, typename F>
concept BinaryOperation = requires(F f, T a, T b) {
    { f(a, b) } -> std::same_as<T>;
};

template<typename T, typename F>
requires BinaryOperation<T, F>
class SegmentTree {
private:
    int n;
    std::vector<T> data;
    F f;
    T e;

public:
    SegmentTree(int n_, F f_, T e_) : n(1), f(std::move(f_)), e(std::move(e_)) {
        while (n < n_) n <<= 1;
        data.assign(n << 1, e);
    }

    T operator[](int i) const { return data[n + i]; }

    void update(int i, const T& x) {
        data[i += n] = x;
        while (i >>= 1) data[i] = f(data[(i << 1) | 0], data[(i << 1) | 1]);
    }

    void build(std::span<const T> v = {}) {
        for (size_t i = 0; i < v.size(); i++) data[n + i] = v[i];
        for (int i = n - 1; i > 0; i--) data[i] = f(data[(i << 1) | 0], data[(i << 1) | 1]);
    }

    T query(int a, int b) const {  // [a, b)
        if (a >= b) return e;
        T vl = e, vr = e;
        for (int l = a + n, r = b + n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) vl = f(vl, data[l++]);
            if (r & 1) vr = f(data[--r], vr);
        }
        return f(vl, vr);
    }
};
/*
    // example
    using T = int;
    auto f = [] (T x1, T x2) { return std::max(x1, x2); };
    const T ex = std::numeric_limits<T>::min();
    SegmentTree<T, decltype(f)> seg(n, f, ex);
*/

int main() {
    int N, Q;
    std::cin >> N >> Q;

    std::vector<std::pair<mint, mint>> v;

    for (int i = 0; i < N; i++) {
        mint a, b;
        std::cin >> a >> b;
        v.emplace_back(a, b);
    }

    using T = std::pair<mint, mint>;
    auto f = [] (T x1, T x2) {
        auto [a, b] = x1;
        auto [c, d] = x2;
        return T{a * c, b * c + d};
    };
    const T ex{1, 0};

    SegmentTree<T, decltype(f)> seg(N, f, ex);

    seg.build(v);

    while (Q--) {
        int type;
        std::cin >> type;
        
        if (type == 0) {
            int p;
            mint c, d;
            std::cin >> p >> c >> d;
            seg.update(p, T{c, d});
        } else {
            int l, r;
            mint x;
            std::cin >> l >> r >> x;
            auto [a, b] = seg.query(l, r);
            std::cout << a * x + b << std::endl;
        }
    }

    return 0;
}
