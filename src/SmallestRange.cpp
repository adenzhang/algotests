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
    // https://leetcode.com/problems/smallest-range/
    vector<int> smallestRange(vector<vector<int>>& nums)
    {
        struct Elem {
            size_t iVec;
            size_t idx;
        };
        const auto greaterVal = [&](const Elem& i, const Elem& j) {
            return nums[i.iVec][i.idx] > nums[j.iVec][j.idx];
        };
        priority_queue<Elem, deque<Elem>, decltype(greaterVal)> q(greaterVal); // min queue
        const auto INF = numeric_limits<size_t>::max();
        size_t minWidth = INF;
        Elem startp, endp; // record
        const auto width = [&](const Elem& i, const Elem& j) -> size_t {
            return nums[j.iVec][j.idx] - nums[i.iVec][i.idx]; // i.value < j.value
        };

        deque<Elem> window;
        vector<size_t> countForVec(nums.size(), 0); // elements of each vetor in window
        size_t nVecInWindow = 0; // count of vectors having elements in q
        for (size_t i = 0; i < nums.size(); ++i) { // init q
            q.push(Elem{ i, 0 });
        }
        while (!q.empty()) {
            auto e = q.top();
            q.pop();
            if (nums[e.iVec].size() > e.idx + 1) {
                q.push(Elem{ e.iVec, e.idx + 1 }); // advance
            }
            window.push_back(e);

            if (++countForVec[e.iVec] == 1) {
                ++nVecInWindow;
            }
            while (countForVec[window.front().iVec] > 1) {
                --countForVec[window.front().iVec];
                window.pop_front();
            }
            if (nVecInWindow == nums.size()) {
                if (auto w = width(window.front(), window.back()); w < minWidth) {
                    startp = window.front();
                    endp = window.back();
                    minWidth = w;
                }
            }
        }
        return minWidth == INF ? vector<int>{} : vector<int>{ nums[startp.iVec][startp.idx], nums[endp.iVec][endp.idx] };
    }

    bool isAlienSorted(vector<string>& words, string order)
    {
        int8_t seq[26];
        const auto Comp = [&](const string& a, const string& b) -> int {
            for (auto p = a.c_str(), q = b.c_str(); true; ++p, ++q) {
                if (!*p)
                    return *q ? (-1) : 0;
                if (!*q)
                    return 1;
                auto c = seq[(*p) - 'a'] - seq[(*q) - 'a'];
                if (c != 0)
                    return c;
            }
            return 0;
        };
        for (int i = 0; i < 26; ++i)
            seq[order[i] - 'a'] = i;

        for (size_t i = 1; i < words.size(); ++i)
            if (Comp(words[i - 1], words[i]) > 0)
                return false;
        return true;
    }

    template <class C, class T>
    T find_parent(C& par, T i)
    {
        if (par[i] != i)
            par[i] = find_parent(par, i);
        return par[i];
    }
    template <class C, class T>
    bool union_parent(C& par, T i, T j)
    {
        auto p = find_parent(par, i), q = find_parent(par, j);
        if (p == q)
            return false;
        par[p] = q;
        return true;
    }
    //https://leetcode.com/contest/weekly-contest-114/problems/array-of-doubled-pairs/
    bool canReorderDoubled0(vector<int>& A)
    {
        unordered_map<int, size_t> m;
        int equalPairs = 0;
        for (auto x : A) {
            if (m[x * 2] > 0) {
                ++equalPairs;
                --m[x * 2];
            } else if (x % 2 == 0 && m[x / 2] > 0) {
                ++equalPairs;
                --m[x / 2];
            } else {
                ++m[x];
            }
        }
        return A.size() / 2 == equalPairs;
    }

    template <class C, class T>
    void countPairs(C& c, T x, int& n)
    {
        if (x == 0) {
            auto d = c[x] / 2;
            c[x] -= d * 2;
            n += d;
            return;
        }
        if (x % 2 == 0 && c[x / 2] > 0)
            countPairs(c, x / 2, n);
        auto d = min(c[x], c[x * 2]);
        c[x] -= d;
        c[x * 2] -= d;
        n += d;
    }
    bool canReorderDoubled(vector<int>& A)
    {
        unordered_map<int, size_t> m; // value: count
        int n = 0;
        for (auto x : A) {
            ++m[x];
        }
        for (auto x : A) {
            countPairs(m, x, n);
        }
        return A.size() / 2 == n;
    }
};
TEST_FUNC(SmallestRange_tests)
{
    Solution sln;
    {
        vector<vector<int>> A = { { 4, 10, 15, 24, 26 }, { 0, 9, 12, 20 }, { 5, 18, 22, 30 } };
        vector<int> res{ 20, 24 };
        REQUIRE(res == sln.smallestRange(A));
    }
    {
        vector<string> A = { "hello", "leetcode" };
        REQUIRE(true == sln.isAlienSorted(A, "hlabcdefgijkmnopqrstuvwxyz"));
    }
    {
        vector<string> A = { "word", "world", "row" };
        REQUIRE(false == sln.isAlienSorted(A, "worldabcefghijkmnpqstuvxyz"));
    }
    {
        vector<string> A = { "apple", "app" };
        REQUIRE(false == sln.isAlienSorted(A, "abcdefghijklmnopqrstuvwxyz"));
    }
    {
        vector<int> A = { 0, 0, 0, 0, 0 };
        REQUIRE(true == sln.canReorderDoubled(A));
    }
    {
        vector<int> A = { 0, 0 };
        REQUIRE(true == sln.canReorderDoubled(A));
    }
    {
        vector<int> A = { 3, 1, 3, 6 };
        REQUIRE(false == sln.canReorderDoubled(A));
    }
    {
        vector<int> A = { 2, 1, 2, 6 };
        REQUIRE(false == sln.canReorderDoubled(A));
    }
    {
        vector<int> A = { 4, -2, 2, -4 };
        REQUIRE(true == sln.canReorderDoubled(A));
    }
    {
        vector<int> A = { 1, 2, 4, 16, 8, 4 };
        REQUIRE(false == sln.canReorderDoubled(A));
    }
    {
        vector<int> A = { 2, 1, 2, 1, 1, 1, 2, 2 };
        REQUIRE(true == sln.canReorderDoubled(A));
    }
    {
        vector<int> A = { -1, 4, 6, 8, -4, 6, -6, 3, -2, 3, -3, -8 };
        REQUIRE(true == sln.canReorderDoubled(A));
    }
}
