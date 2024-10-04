#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <cassert>

constexpr long long LINF = std::numeric_limits<long long>::max();
using pli = std::pair<long long, int>;

struct Dijkstra {
private:
    struct edge { int to; long long cost; };
    int n;
    std::vector<std::vector<edge>> edges;
    std::vector<int> pre;

public:
    std::vector<long long> dist, way;

    Dijkstra(int i) : n(i), edges(i), pre(i), dist(i, LINF), way(i) {}

    void add_edge(int from, int to, long long cost) {
        edges[from].emplace_back(to, cost);
    }

    void exec(int s) {
        std::priority_queue<pli, std::vector<pli>, std::greater<pli>> que;
        dist.assign(n, LINF), pre.assign(n, 0LL), way.assign(n, 0LL);
        dist[s] = 0LL; way[s] = 1LL; que.emplace(0LL, s);

        while (!que.empty()) {
            auto [cost, v] = que.top(); que.pop();
            if (dist[v] < cost) continue;

            for (auto &e : edges[v]) {
                if (dist[e.to] >= dist[v] + e.cost) {
                    way[e.to] += way[v];
                    if (dist[e.to] == dist[v] + e.cost) continue;
                    dist[e.to] = dist[v] + e.cost;
                    pre[e.to] = v;
                    que.emplace(dist[e.to], e.to);
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


// https://judge.yosupo.jp/problem/shortest_path
int main() {
    int N, M, s, t;
    std::cin >> N >> M >> s >> t;

    Dijkstra dj(N);

    while (M--) {
        int a, b;
        long long c;
        std::cin >> a >> b >> c;

        dj.add_edge(a, b, c);
    }

    dj.exec(s);

    if (dj.dist[t] == LINF) {
        std::cout << -1 << std::endl;
    } else {
        std::vector<int> route;
        dj.route(route, s, t);
        std::cout << dj.dist[t] << ' ' << route.size() - 1 << std::endl;

        for (size_t i = 0; i < route.size() - 1; i++) {
            std::cout << route[i] << ' ' << route[i + 1] << std::endl;
        }
    }

    return 0;
}
