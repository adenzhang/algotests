#include <ftl/ftl.h>

using namespace std;
using namespace ftl::serialization;

typedef vector<int> IntVec;
typedef vector<vector<int>> IntVecVec;
typedef vector<string> StrVec;
typedef unordered_map<int, int> IntIntHashMap;
typedef map<int, int> IntIntTreeMap;

using TreeNode = ftl::TreeNode<int>;

/* Given an array A of non-negative integers, the array is squareful if for every pair of adjacent elements, their sum is a perfect square.

Return the number of permutations of A that are squareful.Two permutations A1 and A2 differ if and only if there is some index i such that A1[i] != A2[i]. */

struct Graph {
    using children_type = std::set<size_t>;
    const bool bBidirect;
    size_t N;
    std::vector<children_type> adj;
    std::vector<size_t> countNodes;

    Graph(size_t n, bool bidirect = true)
        : bBidirect(bidirect)
        , N(n)
        , adj(n)
        , countNodes(n, 1)
    {
    }

    void setNodeCount(size_t i, size_t k)
    {
        assert(k > 0);
        N += k - countNodes[i];
        countNodes[i] = k;
    }
    void addEdge(size_t i, size_t j)
    {
        adj[i].insert(j);
        if (bBidirect)
            adj[j].insert(i);
    }

    void countCompletePath(size_t i, size_t nVisited, size_t& count)
    {
        if (countNodes[i] == 0)
            return;
        if (++nVisited == N) {
            ++count;
            return;
        }
        --countNodes[i];

        for (auto e : adj[i]) {
            if (countNodes[e])
                countCompletePath(e, nVisited, count);
        }
        ++countNodes[i];
    }
};
class Solution {
public:
    bool isSqr(int x)
    {
        int y = sqrt(x);
        return y * y == x || (y + 1) * (y + 1) == x;
    }
    int numSquarefulPerms(vector<int>& A)
    {
        std::map<int, size_t> nodes;
        for (auto x : A)
            ++nodes[x];
        Graph g(nodes.size());
        size_t i = 0;
        for (auto it = nodes.begin(); it != nodes.end(); ++it, ++i)
            g.setNodeCount(i, it->second);

        i = 0;
        for (auto it = nodes.begin(); it != nodes.end(); ++it, ++i) {
            if (it->second > 1 && isSqr(it->first * 2))
                g.addEdge(i, i);
            size_t j = i + 1;
            for (auto it2 = std::next(it); it2 != nodes.end(); ++it2, ++j)
                if (isSqr(it->first + it2->first))
                    g.addEdge(i, j);
        }

        size_t count = 0;
        i = 0;
        for (auto it = nodes.begin(); it != nodes.end(); ++it, ++i)
            g.countCompletePath(i, 0, count);
        return count;
    }
};

TEST_FUNC(numSquarefulPerms_tests)
{
    {
        vector<int> A = { 1, 17, 8 };
        Solution sln;
        REQUIRE(2 == sln.numSquarefulPerms(A));
    }
    {
        vector<int> A = { 2, 2, 2 };
        Solution sln;
        REQUIRE(1 == sln.numSquarefulPerms(A));
    }
}
