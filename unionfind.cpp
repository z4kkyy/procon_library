#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>

struct UnionFind {
private:
    std::vector<int> par;

public:
    std::unordered_map<int, std::vector<int>> groups;
    std::unordered_map<int, std::vector<std::pair<int, int>>> group_edges;
    std::vector<std::pair<int, int>> edges;
    bool has_cycle;

    UnionFind(const int n_) : par(n_, -1), has_cycle(false) {}

    bool merge(const int x, const int y) {
        edges.emplace_back(x, y);
        int root_x = root(x);
        int root_y = root(y);
        if (root_x == root_y) {
            has_cycle = true;
            return false;
        }
        if (par[root_x] > par[root_y]) std::swap(root_x, root_y);
        par[root_x] += par[root_y];
        par[root_y] = root_x;
        return true;
    }

    bool merge(const std::pair<int, int>& p) {
        return merge(p.first, p.second);
    }

    int root(int x) {
        if (par[x] < 0) return x;
        return par[x] = root(par[x]);
    }

    bool issame(const int x, const int y) {
        return root(x) == root(y);
    }

    int rank(const int x) {
        return -par[root(x)];
    }

    int edge_cnt(const int x) {
        const auto it = group_edges.find(root(x));
        return it != group_edges.end() ? it->second.size() : 0;
    }

    void make_groups() {
        for (int i = 0; i < static_cast<int>(par.size()); i++) {
            groups[root(i)].push_back(i);
        }
        for (const auto& p : edges) {
            group_edges[root(p.first)].push_back(p);
        }
    }
};


// https://judge.yosupo.jp/problem/unionfind
int main() {
    int N, Q;
    std::cin >> N >> Q;

    UnionFind uf(N);

    while (Q--) {
        int t, u, v;
        std::cin >> t >> u >> v;

        if (t == 0) {
            uf.merge(u, v);
        } else {
            std::cout << (uf.issame(u, v) ? 1 : 0) << std::endl;
        }
    }

    return 0;
}
