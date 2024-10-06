#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <cassert>

constexpr long long LINF = std::numeric_limits<long long>::max();

struct BellmanFord {
private:
    struct edge { int from, to; long long cost; };
    int n, m;
    std::vector<edge> edges;
    std::vector<long long> pre;

public:
    std::vector<long long> dist;
    bool neg_cycle;
    std::vector<bool> neg;

    BellmanFord(int i, int j) : n(i), m(j), neg_cycle(false), edges(j), dist(i, LINF), pre(i), neg(i) {}

    void add_edge(int from, int to, long long cost) {
        edges.push_back({from, to, cost});
    }

    void exec(int s) {
        dist.assign(n, LINF), pre.assign(n, 0LL), neg.assign(n, 0LL);
        dist[s] = 0LL;

        for (int i = 0; i < n; i++) {
            for (auto& e : edges) {
                if (dist[e.from] != LINF && dist[e.to] > dist[e.from] + e.cost) {
                    dist[e.to] = dist[e.from] + e.cost;
                    pre[e.to] = e.from;
                    if (i == n - 1) neg_cycle = true;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (auto& e : edges) {
                if (dist[e.from] != LINF && dist[e.to] > dist[e.from] + e.cost) {
                    neg[e.to] = true;
                }
                if (neg[e.from]) {
                    neg[e.to] = true;
                }
            }
        }
    }

    void route(std::vector<int>& ret, int st, int to) {
        assert(ret.size() == 0);
        int t = to;
        ret.push_back(to);
        while (t != st) ret.push_back(t = pre[t]);
        std::reverse(ret.begin(), ret.end());
    }
};


// https://atcoder.jp/contests/abc061/tasks/abc061_d
int main() {
    int N, M;
    std::cin >> N >> M;

    BellmanFord bl(N, M);

    while (M--) {
        int a, b;
        long long c;
        std::cin >> a >> b >> c;
        a--; b--;

        bl.add_edge(a, b, -c);
    }

    bl.exec(0);

    if (bl.neg[N - 1]) {
        std::cout << "inf" << std::endl;
    } else {
        std::cout << -bl.dist[N - 1] << std::endl;
    }

    return 0;
}
