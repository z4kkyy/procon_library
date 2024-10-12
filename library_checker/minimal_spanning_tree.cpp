#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <algorithm>

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

int main() {
    int N, M;
    std::cin >> N >> M;

    std::vector<std::tuple<long long, int, int, int>> edges(M);
    for (int i = 0; i < M; ++i) {
        int a, b;
        long long c;
        std::cin >> a >> b >> c;
        edges[i] = {c, a, b, i};
    }

    std::sort(edges.begin(), edges.end());

    UnionFind uf(N);
    long long total_weight = 0;
    std::vector<int> selected_edges;
    int edges_count = 0;

    for (const auto& [weight, u, v, index] : edges) {
        if (!uf.issame(u, v)) {
            uf.merge(u, v);
            total_weight += weight;
            selected_edges.push_back(index);
            edges_count++;
            if (edges_count == N - 1) break;
        }
    }

    std::cout << total_weight << std::endl;
    for (int i = 0; i < selected_edges.size(); ++i) {
        std::cout << selected_edges[i];
        if (i < selected_edges.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;

    return 0;
}
