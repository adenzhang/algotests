#include <ftl/catch_or_ignore.h>

#include <algorithm>
#include <bitset>
#include <cassert>
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
    // https://leetcode.com/contest/weekly-contest-116/problems/least-operators-to-express-number/

    int leastOpsExpressTarget(int x, int T)
    {
        unordered_map<uint64_t, uint32_t> D; // D[i]: least number of x to express i

        uint64_t uppbound = x;
        int n = 1;
        while (uppbound < T) {
            uppbound *= x;
            ++n;
        }
        if (uppbound == T)
            return n - 1;

        vector<uint64_t> ex(n);
        for (uint64_t i = 0, y = x; i < n; ++i, y *= x) {
            ex[i] = y;
            D[y] = i + 1;
        }
        D[1] = 2;

        const function<int(uint32_t x, uint32_t T)> leastNumbersExpress = [&ex, &leastNumbersExpress, &D](uint32_t x, uint32_t T) -> int {
            int res;
            if (T < x) {
                res = 2 * T > x ? (1 + (x - T) * 2) : (T * 2);
                return res;
            }
            auto it = D.find(T);
            if (it != D.end())
                return it->second;
            auto p = lower_bound(ex.begin(), ex.end(), T);
            assert(p != ex.end());
            auto n = distance(ex.begin(), p) + 1;
            auto uppbound = *p;
            if (uppbound == T)
                res = n;
            else {
                auto low = n - 1 + leastNumbersExpress(x, T - uppbound / x);
                if (T - uppbound / x > uppbound - T)
                    res = min(low, n + leastNumbersExpress(x, uppbound - T));
                else
                    res = low;
            }
            D[T] = res;
            return res;
        };
        return leastNumbersExpress(x, T) - 1;
    }

    int
    leastOpsExpressTarget0(int x, int T)
    {
        const auto INF = numeric_limits<int64_t>::max();
        vector<uint64_t> D(2 * T + 1, INF);
        D[1] = 2; // 2 = x/x

        // populate dynamic states D
        // D[i]: least number of x to express i, where only *, +, / operators are used (operator- is not used), i in [1:2T]
        for (uint64_t i = 1, y = x, M = 2 * T; y < M; ++i, y *= x)
            D[y] = i;
        if (D[T] != INF)
            return D[T] - 1;
        for (uint64_t i = 2, M = 2 * T; i <= M; ++i) {
            if (D[i] != INF)
                continue;
            for (uint64_t j = 1, N = i / 2 + 1; j < N; ++j) {
                D[i] = min(D[i], D[j] + D[i - j]);
            }
        }

        vector<uint64_t> DD(T + 1, INF);
        // use operator-, i in [1:T]
        for (uint64_t i = 1; i <= T; ++i) {
            DD[i] = D[i];
            for (uint64_t j = 1; j < i; ++j) {
                DD[i] = min(DD[i], D[j + i] + D[j]);
            }
        }
        return DD[T] - 1;
    }
};
TEST_FUNC(leastOpsExpressTarget_tests)
{
    Solution sln;
    {
        REQUIRE(3 == sln.leastOpsExpressTarget(3, 10)); // 3*3+3/3
    }
    {
        REQUIRE(5 == sln.leastOpsExpressTarget(3, 19)); // 3*3+3*3+3/3
    }
    {
        REQUIRE(8 == sln.leastOpsExpressTarget(5, 501)); // 5 * 5 * 5 * 5 - 5 * 5 * 5 + 5 / 5
    }
    {
        REQUIRE(3 == sln.leastOpsExpressTarget(100, 100000000)); // 100 * 100 * 100 * 100
    }
    {
        REQUIRE(50 == sln.leastOpsExpressTarget(2, 125046)); // 100 * 100 * 100 * 100
    }
    {
        REQUIRE(17 == sln.leastOpsExpressTarget(3, 365)); // 100 * 100 * 100 * 100
    }
}
