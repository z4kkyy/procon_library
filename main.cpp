#include <bits/stdc++.h>
using namespace std;

#define dout(x) cout << #x << " = " << (x) << endl
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define rrep(i, n) for (int i = (int)(n - 1); i >= 0; i--)
#define rep2(i, s, n) for (int i = (int)(s); i < (int)(n); i++)
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define rup(a, b) (a / b + (a % b && (a ^ b) >= 0))

using str = string;
using ll = long long;
using ld = long double;
using u64 = uint64_t;
using u128 = __uint128_t;
using pll = pair<ll, ll>;
using pii = pair<int, int>;
using pli = pair<ll, int>;
using pil = pair<int, ll>;

constexpr ll LINF = numeric_limits<ll>::max();
constexpr int INF = numeric_limits<int>::max();
constexpr int MAX = 2000020;
constexpr int MOD1 = 1000000007;
constexpr int MOD2 = 998244353;
const int di[4] = {1, 0, -1, 0};
const int dj[4] = {0, 1, 0, -1};
const int di2[8] = {0, 1, 0, -1, 1, 1, -1, -1};
const int dj2[8] = {1, 0, -1, 0, 1, -1, 1, -1};

template <typename T, typename U> inline bool chmax(T& lhs, U rhs) { return lhs < rhs ? (lhs = rhs, true) : false; }
template <typename T, typename U> inline bool chmin(T& lhs, U rhs) { return lhs > rhs ? (lhs = rhs, true) : false; }
template <class T> inline T max(vector<T> x) { return max_element(x.begin(), x.end()); }
template <class T> inline T min(vector<T> x) { return min_element(x.begin(), x.end()); }
template <class T> inline T sum(vector<T> x) { return reduce(x.begin(), x.end()); }
template <class... T> constexpr auto min(T... a) { return min(initializer_list<common_type_t<T...>>{a...}); }
template <class... T> constexpr auto max(T... a) { return max(initializer_list<common_type_t<T...>>{a...}); }
template <typename T> inline T bpow(T a, int n) { T r(1); while (n) { if (n & 1) r *= a; a *= a; n >>= 1; } return r; }
template <typename T> inline void uniq(vector<T>& v) { sort(all(v)); v.erase(unique(all(v)), v.end()); }
template <typename T> inline void comp(vector<T>& v) { vector<T> u = v; uniq(u); for (T& x : v) { x = lower_bound(all(u), x) - u.begin(); } }
template <typename T> inline T gcd_all(vector<T>& v) { return reduce(all(v), v[0], [] (T x, T y) { return gcd(x, y); }); }
template <typename T> inline T lcm_all(vector<T>& v) { return reduce(all(v), v[0], [] (T x, T y) { return lcm(x, y); }); }

template <typename T, typename U> ostream& operator<<(ostream& os, const pair<T, U>& p) { return os << p.first << " " << p.second; }
template <typename T, typename U> istream& operator>>(istream& is, pair<T, U>& p) { return is >> p.first >> p.second; }
template <typename T> ostream& operator<<(ostream& os, const vector<T>& v) { rep(i, (int)v.size()) { if (i) os << " "; os << v[i]; } return os; }
template <typename T> istream& operator>>(istream& is, vector<T>& v) { for (auto& x : v) is >> x; return is; }
template <typename T> ostream& operator<<(ostream& os, const set<T>& st) { int c(0); for (auto& x : st) { if (c) os << " "; os << x; c++; } return os; }

using pli = pair<ll, int>;

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


void solve() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;

    Dijkstra dj(n);

    rep(i, m) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        dj.add_edge(a, b, c);
    }

    dj.exec(s);
    vector<int> route;
    dj.route(route, s, t);

    if (dj.dist[t] == LINF) {
        cout << -1 << endl;
    } else {
        cout << dj.dist[t] << " " << route.size() - 1 << endl;
        rep(i, route.size() - 1) {
            cout << route[i] << " " << route[i + 1] << endl;
        }
    }
    return;
}


int main() {

    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    // cout << fixed << setprecision(15);

    solve();

    return 0;
}
