#include <iostream>
#include <concepts>
#include <vector>
#include <span>

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
    auto f = [](T x1, T x2) { return std::max(x1, x2); };
    const T ex = std::numeric_limits<T>::min();
    SegmentTree<T, decltype(f)> seg(n, f, ex);
*/

int main() {
    int N, Q;
    std::cin >> N >> Q;

    std::vector<long long> A(N);
    for (long long& a : A)
        std::cin >> a;

    using T = long long;
    auto f = [](T x1, T x2) { return x1 + x2; };
    const T ex = 0LL;
    SegmentTree<T, decltype(f)> seg(N, f, ex);

    seg.build(A);

    while (Q--) {
        int type;
        std::cin >> type;

        if (type == 0) {
            int p;
            long long x;
            std::cin >> p >> x;
            seg.update(p, seg[p] + x);
        } else {
            int l, r;
            std::cin >> l >> r;
            std::cout << seg.query(l, r) << std::endl;
        }
    }

    return 0;
}
