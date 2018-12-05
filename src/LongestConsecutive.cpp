#include <ftl/catch_or_ignore.h>

#include <algorithm>
#include <bitset>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <ftl/container_serialization.h>
#include <ftl/indexed_tree.h>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace ftl::serialization;

typedef vector<int> IntVec;
typedef vector<vector<int>> IntVecVec;
typedef vector<string> StrVec;
typedef unordered_map<int, int> IntIntHashMap;
typedef map<int, int> IntIntTreeMap;

class Solution {
public:
    using ParentMap = unordered_map<int, pair<int, size_t>>; // {v: {parentV, size}}
    int find_parent(ParentMap& parent, int i)
    {
        if (parent[i].first != i)
            parent[i].first = find_parent(parent, parent[i].first);
        return parent[i].first;
    }
    // return the new size
    size_t union_parent(ParentMap& parent, int i, int j)
    {
        auto p = find_parent(parent, i), q = find_parent(parent, j);
        if (p == q)
            return false;
        parent[p].first = q;
        parent[q].second += parent[p].second;
        return parent[q].second;
    }
    int longestConsecutive(vector<int>& nums)
    {
        size_t res = 1;
        const size_t N = nums.size();
        if (N < 2)
            return N;
        ParentMap parent(N);
        for (auto i : nums)
            parent[i] = make_pair(i, (size_t)1);

        unordered_set<int> q;
        for (auto i : nums) {
            if (q.count(i + 1)) {
                auto m = union_parent(parent, i, i + 1);
                if (m > res)
                    res = m;
            }
            if (q.count(i - 1)) {
                auto m = union_parent(parent, i, i - 1);
                if (m > res)
                    res = m;
            }
            q.insert(i);
        }
        return res;
    }
};

TEST_FUNC(longestConsecutive_tests)
{
    Solution sln;
    {
        vector<int> A = { 100, 4, 200, 1, 3, 2 };
        REQUIRE(4 == sln.longestConsecutive(A));
    }
}
