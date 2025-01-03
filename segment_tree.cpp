#include <iostream>
#include <concepts>
#include <vector>
#include <span>

template <typename F, typename S>
concept BinaryOperation = requires(F f, S a, S b) {
    { f(a, b) } -> std::convertible_to<S>;
};

template<typename S, typename F>
requires BinaryOperation<F, S>
class SegmentTree {
private:
    int n;
    std::vector<S> data;
    F f;
    S e;

    inline void update_node(int k) {
        data[k] = f(data[k << 1], data[(k << 1) | 1]);
    }

public:
    SegmentTree(int n_, F f_, S e_) : f(f_), e(e_) {
        n = 1;
        while (n < n_) n <<= 1;
        data.assign(n << 1, e);
    }

    S operator[](int i) const { return data[n + i]; }

    void update(int i, const S& x) {
        data[i += n] = x;
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

    S query(int a, int b) const {  // [a, b)
        if (a >= b) return e;
        S vl = e, vr = e;
        int l = a + n, r = b + n;
        while (l < r) {
            if (l & 1) vl = f(vl, data[l++]);
            if (r & 1) vr = f(data[--r], vr);
            l >>= 1;
            r >>= 1;
        }
        return f(vl, vr);
    }
};
/*
    // example
    using S = int;
    auto op = [] (S x1, S x2) { return std::max(x1, x2); };
    const S id = std::numeric_limits<S>::min();
    SegmentTree<S, decltype(op)> seg(n, op, id);
*/


// https://judge.yosupo.jp/problem/static_range_sum
int main() {
    int N, Q;
    std::cin >> N >> Q;

    std::vector<long long> A(N);
    for (long long& a : A) std::cin >> a;

    auto op = [](long long a, long long b) { return a + b; };
    SegmentTree<long long, decltype(op)> seg(N, op, 0);

    seg.build(A);

    while (Q--) {
        int l, r;
        std::cin >> l >> r;
        std::cout << seg.query(l, r) << std::endl;
    }

    return 0;
}
