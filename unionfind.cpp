#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>

struct UnionFind {
private:
    std::vector<int> par;

public:
    std::unordered_map<int, std::vector<int>> groups;
    std::unordered_map<int,std::vector<std::pair<int, int>>> group_edges;
    std::vector<std::pair<int, int>> edges;
    bool has_cycle;

    UnionFind(int n_) : par(n_,-1), has_cycle(false) {}

    bool merge(int x, int y) {
        edges.emplace_back(x, y);
        x = root(x), y = root(y);
        if (x == y) {
            has_cycle = true;
            return false;
        }
        if (par[x] > par[y]) std::swap(x, y);
        par[x] += par[y];
        par[y] = x;
        return true;
    }

    bool merge(std::pair<int, int> p) { return merge(p.first, p.second); }

    int root(int x) { return (par[x] < 0) ? x : (par[x] = root(par[x])); }

    bool issame(int x, int y) { return root(x) == root(y); }

    int rank(int x) { return -par[root(x)]; }

    int edge_cnt(int x) { return group_edges[root(x)].size(); }

    void make_groups() {
        for (int i = 0; i < (int)(par.size()); i++) {
            groups[root(i)].push_back(i);
        }
        for (auto& p : edges) {
            group_edges[root(p.first)].push_back(p);
        }
        return;
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
