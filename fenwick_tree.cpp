#include <iostream>
#include <vector>

struct FenwickTree {
private:
    int n, p;
    std::vector<long long> data;

public:
    FenwickTree(int n_) : n(n_), p(1), data(n + 1) {
        while (p * 2 <= n) p *= 2;
    }

    void add(int i, long long value) {
        for (i++; i <= n; i += i & -i) data[i] += value;
    }

    long long sum(int i) const { // [0, i], O(log(n))
        long long result = 0;
        for (i++; i; i -= i & -i) result += data[i];
        return result;
    }

    long long range_sum(int left, int right) const {  // [left, right], O(log(n))
        return sum(right) - (left > 0 ? sum(left - 1) : 0);
    }

    int lower_bound(long long w) {
        if (w <= 0) return 0;
        int x = 0;
        for (int k = p; k; k /= 2) {
            if (x + k <= n && data[x + k] < w) {
                w -= data[x + k], x += k;
            }
        }
        return x;
    }
};


// https://atcoder.jp/contests/practice2/tasks/practice2_b
int main() {
    int N, Q;
    std::cin >> N >> Q;

    FenwickTree f(N);

    for (int i = 0; i < N; i++) {
        long long a;
        std::cin >> a;
        f.add(i, a);
    }

    while (Q--) {
        int query_type;
        std::cin >> query_type;

        if (query_type == 0) {
            int p;
            long long x;
            std::cin >> p >> x;
            f.add(p, x);
        } else {
            int l, r;
            std::cin >> l >> r;
            std::cout << f.range_sum(l, r - 1) << std::endl;
        }
    }
    
    return 0;
}
