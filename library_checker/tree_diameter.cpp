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

            for (auto& e : edges[v]) {
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


// https://judge.yosupo.jp/problem/tree_diameter
int main() {
    int N;
    std::cin >> N;

    Dijkstra dj(N);

    for (int i = 0; i < N - 1; i++) {
        int a, b;
        long long c;
        std::cin >> a >> b >> c;

        dj.add_edge(a, b, c);
        dj.add_edge(b, a, c);
    }

    dj.exec(0);
    int s = std::max_element(dj.dist.begin(), dj.dist.end()) - dj.dist.begin();
    dj.exec(s);
    int t = std::max_element(dj.dist.begin(), dj.dist.end()) - dj.dist.begin();

    std::vector<int> route;
    dj.route(route, s, t);

    std::cout << dj.dist[t] << ' ' << route.size() << std::endl;
    for (int i = 0; i < route.size(); i++) {
        std::cout << route[i] << (i == route.size() - 1 ? '\n' : ' ');
    }
    return 0;
}
