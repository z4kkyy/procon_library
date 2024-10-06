#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>

struct Trie {
public:
    struct Node {
        int id;
        char c;
        std::unordered_map<char, size_t> next;
        int terminal_count = 0;
        int prefix_count = 0;
        int pass_count = 0;
    };

    std::vector<Node> nodes;
    long long total_count = 0;

    Trie() { nodes.emplace_back(); }

    void insert(std::string_view S) {
        size_t node_id = 0;
        nodes[node_id].prefix_count++;
        for (const char& x : S) {
            if (nodes[node_id].next.count(x) == 0) {
                nodes[node_id].next[x] = nodes.size();
                nodes.emplace_back();
            }
            node_id = nodes[node_id].next[x];
            nodes[node_id].prefix_count++;
            nodes[node_id].c = x;
            total_count += nodes[node_id].pass_count++;
        }
        nodes[node_id].terminal_count++;
        nodes[node_id].id = node_id;
    }

    int count(std::string_view S) const {
        size_t node_id = 0;
        for (const char& x : S) {
            auto it = nodes[node_id].next.find(x);
            if (it == nodes[node_id].next.end()) {
                return 0;
            }
            node_id = it->second;
        }
        return nodes[node_id].terminal_count;
    }

    int count_prefixing(std::string_view S, bool equal = true) const {
        size_t node_id = 0;
        for (const char& x : S) {
            auto it = nodes[node_id].next.find(x);
            if (it == nodes[node_id].next.end()) {
                return 0;
            }
            node_id = it->second;
        }
        const Node& N = nodes[node_id];
        return equal ? N.prefix_count : N.prefix_count - N.terminal_count;
    }

    size_t size() const {
        return nodes.size();
    }
};


// https://atcoder.jp/contests/abc353/tasks/abc353_e
int main() {
    int N;
    std::cin >> N;

    Trie trie;

    for (int i = 0; i < N; i++) {
        std::string S;
        std::cin >> S;
        trie.insert(S);
    }

    std::cout << trie.total_count << std::endl;

    return 0;
}
