#ifdef UNITTEST_MODE
#define TEST_MODE
#include <ftl/catch.hpp>
#endif

#ifdef TEST_MODE
#include <ftl/container_serialization.h>
#endif

#include <algorithm>
#include <bitset>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
using namespace ftl::serialization;

typedef vector<int> IntVector;
typedef unordered_map<int, int> IntIntHashMap;
typedef map<int, int> IntIntTreeMap;

/**
  https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k/description/
Return the length of the shortest, non-empty, contiguous subarray of A with sum at least K.

If there is no non-empty subarray with sum at least K, return -1.

 */
namespace ShortestSubarrayWithSumAtLeastK {

class Solution {
public:
    // let w is the min length of subarray, of which sum >= K.
    // for Moving window (j, i) ending at ai,
    // deque q which is an increasing serial si = sum(aj, ai)
    //      if si <= s[i-1], then ignore current
    //     if si > q.top(), then
    //             pop_back until current sum is the largest.
    //             q.pop_front() while q.back().sum - q.front().sum  > K. if v = q.back().i - q.front().i +1 < w, update w = v.
    int shortestSubarray(vector<int>& A, int K)
    {
        struct Sum {
            int s;
            int* b;
            int* i;
        };
        if (A.size() == 0)
            return -1;
        if (K <= 0)
            return 0;
        std::deque<Sum> q;
        int w = -1; // min width
        int s = 0; // sum
        int* b = &A.front(); // begin of window
        for (int *e = &A.front(), *END = &(*A.end()); e != END; ++e) {
            s += *e;
            if (s <= 0) {
                q.clear();
                s = 0; // clear window,  ensure si>0
                b = e + 1;
            } else {
                if (q.empty() || s > q.back().s) { // *e > 0, increasing
                    while (!q.empty() && q.back().s >= s)
                        q.pop_back();
                    // todo if q.empty()
                    q.push_back(Sum{ s, b, e });
                    // pop front until shink to min window
                    bool bPopped = false;
                    while (q.back().s - q.front().s > K) { // still > K after removing the first
                        bPopped = true;
                        q.pop_front();
                    }
                    if (bPopped) {
                        auto v = q.back().i - q.front().i + 1;
                        if (v < w || w < 0)
                            w = v;
                        b = q.front().i;
                    } else {
                        auto sum = q.back().s - q.front().s + q.front().s; // including the first
                        if (sum >= K && (sum < w || w < 0))
                            w = q.back().i - b + 1;
                        //                            w = q.back().i - q.front().i + 1;
                    }
                } else { // *e <=0, descreasing, then ignore
                }
            }
        }

        return w;
    }
};
}
#ifdef UNITTEST_MODE
TEST_CASE("ShortestSubarrayWithSumAtLeastK_tests", "")
#else
int main()
#endif

{
    ShortestSubarrayWithSumAtLeastK::Solution sln;
    //    {
    //        IntVector a{ 1 };
    //        REQUIRE(sln.shortestSubarray(a, 1) == 1);
    //    }
    //    {
    //        IntVector a{ 1, 2 };
    //        REQUIRE(sln.shortestSubarray(a, 4) == -1);
    //    }
    //    {
    //        IntVector a{ 2, -1, 2 };
    //        REQUIRE(sln.shortestSubarray(a, 3) == 3);
    //    }
    //    {
    //        IntVector a{ 77, 19, 35, 10, -14 };
    //        REQUIRE(sln.shortestSubarray(a, 19) == 1);
    //    }

    {
        IntVector a{ 84, -37, 32, 40, 95 };
        REQUIRE(sln.shortestSubarray(a, 167) == 3);
    }
}
