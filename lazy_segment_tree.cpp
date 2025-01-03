#include <iostream>
#include <concepts>
#include <vector>
#include <limits>
#include <span>
#include <numeric>
#include <algorithm>

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
        if (lazy[k] == id) return;
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

    LazySegmentTree<S, E, decltype(op), decltype(mapping), decltype(composition)>
        seg(n, op, mapping, composition, id, id_e);
*/



int main() {
    int h, w, n;
    std::cin >> h >> w >> n;
    std::vector<int> r(n), c(n), l(n);
    for (int i = 0; i < n; i++) {
        std::cin >> r[i] >> c[i] >> l[i];
        c[i]--;
    }

    std::vector<int> ord(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::sort(ord.begin(), ord.end(), [&](int i, int j) { return r[i] > r[j]; });

    std::vector<int> ans(n);

    using S = int;
    using E = int;

    auto op = [](S a, S b) -> S { return std::min(a, b); };
    auto mapping = [](S data, E update) -> S { return std::min(data, update); };
    auto composition = [](E existing, E update) -> E { return std::min(existing, update); };

    S id = std::numeric_limits<int>::max();
    E id_e = std::numeric_limits<int>::max();

    LazySegmentTree<S, E, decltype(op), decltype(mapping), decltype(composition)>
        seg(w, op, mapping, composition, id, id_e);

    seg.build(std::vector<int>(w, h + 1));

    for (int i: ord) {
        ans[i] = seg.query(c[i], c[i] + l[i]) - 1;
        seg.update(c[i], c[i] + l[i], ans[i]);
    }

    for (int i = 0; i < n; i++) {
        std::cout << ans[i] << std::endl;
    }

    return 0;
}