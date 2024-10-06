#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>

struct WeightedUnionFind {
private:
    std::vector<int> par;
    std::vector<long long> diff_weight;

public:
    std::unordered_map<int, std::vector<int>> groups;
    std::unordered_map<int, std::vector<std::pair<int, int>>> group_edges;
    std::vector<std::tuple<int, int, long long>> edges;
    bool has_cycle;

    WeightedUnionFind(int n_) : par(n_, -1), diff_weight(n_, 0), has_cycle(false) {}

    bool merge(int x, int y, long long w) {
        edges.emplace_back(x, y, w);
        w += weight(x); w -= weight(y);
        x = root(x); y = root(y);
        if (x == y) {
            has_cycle = true;
            return false;
        }
        if (par[x] > par[y]) {
            std::swap(x, y);
            w = -w;
        }
        par[x] += par[y], par[y] = x;
        diff_weight[y] = w;
        return true;
    }

    int root(const int x) {
        if (par[x] < 0) return x;
        int r = root(par[x]);
        diff_weight[x] += diff_weight[par[x]];
        return par[x] = r;
    }

    bool issame(const int x, const int y) {
        return root(x) == root(y);
    }

    long long diff(const int x, const int y) {
        return weight(y) - weight(x);
    }

    long long weight(const int x) {
        root(x);
        return diff_weight[x];
    }

    int rank(int x) {
        return -par[root(x)];
    }

    int edge_cnt(const int x) {
        return group_edges[root(x)].size();
    }

    void make_groups() {
        for (int i = 0; i < (int)(par.size()); i++) {
            groups[root(i)].push_back(i);
        }
        for (auto& t : edges) {
            group_edges[root(std::get<0>(t))].emplace_back(std::get<0>(t), std::get<1>(t));
        }
    }
};


// https://atcoder.jp/contests/abc087/tasks/arc090_b
int main() {
    int N, M;
    std::cin >> N >> M;

    WeightedUnionFind uf(N);

    while (M--) {
        int L, R;
        long long D;
        std::cin >> L >> R >> D;
        L--; R--;

        if (uf.issame(L, R)) {
            if (uf.diff(L, R) != D) {
                std::cout << "No" << std::endl;
                return 0;
            }
        } else {
            uf.merge(L, R, D);
        }
    }
    std::cout << "Yes" << std::endl;

    return 0;
}
