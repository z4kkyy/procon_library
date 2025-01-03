#include <iostream>
#include <concepts>
#include <vector>
#include <limits>
#include <span>
#include <numeric>
#include <algorithm>
#include <utility>
#include <random>

template <typename F, typename S>
concept BinaryOperation = requires(F f, S a, S b) {
    { f(a, b) } -> std::convertible_to<S>;
};

template <typename G, typename S, typename E>
concept MappingFunction = requires(G g, S data, E update) {
    { g(data, update) } -> std::convertible_to<S>;
};

template <typename H, typename E>
concept CompositionFunction = requires(H h, E existing, E update) {
    { h(existing, update) } -> std::convertible_to<E>;
};

template <typename S, typename E, typename F, typename G, typename H>
requires BinaryOperation<F, S> &&
        MappingFunction<G, S, E> &&
        CompositionFunction<H, E>
class LazySegmentTree {
private:
    int n, size, log;
    std::vector<S> data;
    std::vector<E> lazy;
    F f;
    G g;
    H h;
    S e;
    E id;

    inline void update_node(int k) {
        data[k] = f(data[k << 1], data[(k << 1) | 1]);
    }

    inline void apply_node(int k, const E& x) {
        data[k] = g(data[k], x);
        if (k < size) lazy[k] = h(lazy[k], x);
    }

    inline void push(int k) {
        if (k >= size || lazy[k] == id) return;
        apply_node(k << 1, lazy[k]);
        apply_node((k << 1) | 1, lazy[k]);
        lazy[k] = id;
    }

public:
    LazySegmentTree(int n_, F f_, G g_, H h_, S e_, E id_)
        : f(f_), g(g_), h(h_), e(e_), id(id_) {
        n = 1;
        log = 0;
        while (n < n_) {
            n <<= 1;
            log++;
        }
        size = n;
        data.assign(n << 1, e);
        lazy.assign(size, id);
    }

    S operator[](int i) {
        i += size;
        for (int j = log; j >= 1; j--) {
            push(i >> j);
        }
        return data[i];
    }

    void update(int i, const S& x) {
        i += n;
        for (int j = log; j >= 1; j--) {
            push(i >> j);
        }
        data[i] = x;
        while (i >>= 1) update_node(i);
    }

    void build(std::span<const S> v = {}) {
        for (int i = 0; i < (int)v.size(); i++) {
            data[n + i] = v[i];
        }
        for (int i = n - 1; i > 0; i--) {
            update_node(i);
        }
    }

    void update(int a, int b, const E& x) {  // [a, b)
        if (a >= b) return;
        a += size;
        b += size;

        for (int i = log; i >= 1; i--) {
            if (((a >> i) << i) != a) push(a >> i);
            if (((b >> i) << i) != b) push((b - 1) >> i);
        }

        int l = a, r = b;
        while (l < r) {
            if (l & 1) apply_node(l++, x);
            if (r & 1) apply_node(--r, x);
            l >>= 1;
            r >>= 1;
        }

        for (int i = 1; i <= log; i++) {
            if (((a >> i) << i) != a) update_node(a >> i);
            if (((b >> i) << i) != b) update_node((b - 1) >> i);
        }
    }

    S query(int a, int b) {  // [a, b)
        if (a >= b) return e;
        a += size;
        b += size;

        for (int i = log; i >= 1; i--) {
            if (((a >> i) << i) != a) push(a >> i);
            if (((b >> i) << i) != b) push((b - 1) >> i);
        }

        S vl = e, vr = e;
        while (a < b) {
            if (a & 1) vl = f(vl, data[a++]);
            if (b & 1) vr = f(data[--b], vr);
            a >>= 1;
            b >>= 1;
        }
        return f(vl, vr);
    }
};
/*
    // example
    using S = int;
    using E = int;

    auto op = [](S a, S b) -> S { return std::min(a, b); };
    auto mapping = [](S data, E update) -> S { return std::min(data, update); };
    auto composition = [](E existing, E update) -> E { return std::min(existing, update); };

    S id = std::numeric_limits<int>::max();
    E id_e = std::numeric_limits<int>::max();

    LazySegmentTree<S, decltype(op), E, decltype(mapping), decltype(composition)>
        seg(n, op, mapping, composition, id, id_e);
*/

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

int main() {
    int N, Q;
    std::cin >> N >> Q;

    using S = std::pair<mint, mint>;
    using E = std::pair<mint, mint>;

    auto op = [](S x, S y) -> S {
        auto [b1, c1] = x;
        auto [b2, c2] = y;
        return std::make_pair(b1 * b2, b2 * c1 + c2);
    };

    auto mapping = [](S data, E update) -> S {
        return update;
    };

    auto composition = [](E existing, E update) -> E {
        return update;
    };

    S id = std::make_pair(mint(1), mint(0));
    E id_e = std::make_pair(mint(1), mint(0));

    LazySegmentTree<S, E, decltype(op), decltype(mapping), decltype(composition)>
        seg(N, op, mapping, composition, id, id_e);

    std::vector<std::pair<mint, mint>> A;
    for (int i = 0; i < N; i++) {
        mint a, b;
        std::cin >> a >> b;
        A.emplace_back(a, b);
    }

    seg.build(A);

    while (Q--) {
        int t;
        std::cin >> t;
        std::cout << "-----------------------------------" << std::endl;
        for (int i = 0; i < N; i++) {
            auto [a, b] = seg[i];
            std::cout << "(" << a << " " << b << ")";
        }
        std::cout << std::endl;
        if (t == 0) {
            int l, r;
            mint c, d;
            std::cin >> l >> r >> c >> d;
            std::cout << "update" << std::endl;
            std::cout << "l = " << l << " r = " << r << std::endl;
            std::cout << "c = " << c << " d = " << d << std::endl;
            seg.update(l, r, std::make_pair(c, d));
        } else {
            int l, r;
            mint x;
            std::cin >> l >> r >> x;
            auto [a, b] = seg.query(l, r);
            std::cout << "query" << std::endl;
            std::cout << "l = " << l << " r = " << r << std::endl;
            std::cout << "a = " << a << " b = " << b << std::endl;
            std::cout << a * x + b << std::endl;
        }
        for (int i = 0; i < N; i++) {
            auto [a, b] = seg[i];
            std::cout << "(" << a << " " << b << ")";
        }
        std::cout << std::endl;
    }

    return 0;
}
